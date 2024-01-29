#include <algorithm>
#include "httpServer.hpp"

#define SERVERPORT 8080

void sigchld_handler(int signal);

httpServer::httpServer() {
    createTCPSocket();
}

httpServer::~httpServer() {
    close(socketfd);
}

/**
 * creates a TCP Socket which listens to the sensor to the browser ports (8080)
 */
void httpServer::createTCPSocket() {
    // create a TCP socket
    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    // exit if failed to create the socket
    if (socketfd == 0) {
        std::perror("socket failure");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVERPORT);

    // Removes the "Adress already in use" error message
    int allow = 1;
    if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &allow, sizeof allow) == -1) {
        std::perror("socket option failure");
        exit(EXIT_FAILURE);
    }

    // bind socket
    if (bind(socketfd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        std::perror("bind failure");
        exit(EXIT_FAILURE);
    }

}

/**
 *  Listens to the created tcp port and if a browser connects to the socket the method sends an answer to the calling browser
 */
void httpServer::listenPort() {
    // backlog is 10 which means 10 pending connections can be queued up before refusing
    if (listen(socketfd, 10) == -1) {
        std::perror("listen failure");
        exit(EXIT_FAILURE);
    }
    struct sigaction signalAction;
    signalAction.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&signalAction.sa_mask);
    signalAction.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &signalAction, nullptr) == -1) {
        perror("sigaction");
        exit(1);
    }
    while (true) {
        if ((httpSocket = accept(socketfd, (struct sockaddr *) &address, (socklen_t *) &addrlength)) == -1) {
            std::perror("accept failure");
            continue;
        }

        if (!fork()) {
            close(socketfd);
            sendAnswer();
            close(httpSocket);
            exit(0);
        }
        close(httpSocket);
    }
}

/**
 * Sends a valid html site back to the caller
 */

void httpServer::sendAnswer() {

    char buffer[1024]{};

    // read socket input
    read(httpSocket, buffer, 1024);

    // check for HTTP GET request method
    bool isHTTPGET = checkForHTTPGET(buffer);
    if (!isHTTPGET) {
        return;
    }

    // extract uri from http GET
    std::string uri = readURI(convertToString(buffer, sizeof buffer / sizeof(char)));

    std::string history;
    std::string tmpHTML;
    std::string text;

    // check if file exists
    if (!fileExists(uri) && uri != "allsensors.csv") {
        text = "HTTP/1.1 404 Not Found\nContent-Type: text/html\nContent-Length: ";
        history = "404 - Page does not exist!";
    } else {
        // get corresponding sensor data history
        history = processURI(uri);

        // Answer - build html page
        text = "HTTP/1.1 200 OK\nContent-Type: text/json\nContent-Length: ";
    }

    //tmpHTML.append(createHTMLBegin(uri));
    tmpHTML.append(createHTMLBody(history));
    //tmpHTML.append(createHTMLEnd());
    text.append(std::to_string(tmpHTML.length()));
    text.append("\n\n");
    text.append(tmpHTML);

    // write html page back to socket
    write(httpSocket, text.c_str(), text.length());

    close(httpSocket);
}

/**
 * Reads a given file and puts all its content into a vector of strings
 * @param filename provides the name of the file which will be opened
 * @return All lines of the file
 */
std::vector<std::string> httpServer::readCSV(const std::string &filename) {
    std::string line;
    std::vector<std::string> lines;
    std::ifstream ReadCSV(filename);

    if (!ReadCSV.is_open()) {
        return lines;
    }
    while (getline(ReadCSV, line)) {
        lines.push_back(line);
    };

    ReadCSV.close();

    lines.erase(lines.begin());
    return lines;
}

/**
 * Checks for HTTP GET request method
 * @param buffer found URI string
 * @return true if the HTTP request method is GET, false otherwise
 */
bool httpServer::checkForHTTPGET(const std::string &buffer) {
    std::string uriString;

    for (auto item : buffer) {
        if (item == ' ') {
            if (uriString == "GET") {
                return true;
            } else {
                return false;
            }
        } else {
            if (item == '/') {
                continue;
            }
            uriString += item;
        }
    }
    return false;
}

/**
 * Reads the URI from a given buffer string
 * @param buffer found URI string
 * @return URI as a string, if no URI is found then an empty string gets returned
 */
std::string httpServer::readURI(const std::string &buffer) {
    bool found = false;
    std::string uriString;

    for (auto item : buffer) {
        if (item == ' ') {
            if (found) {
                return uriString;
            } else {
                found = true;
                uriString = "";
            }
        } else {
            if (item == '/') {
                continue;
            }
            uriString += item;
        }
    }

    return " ";
}

/**
 * Reads the User-Agent from a given buffer string
 * @param buffer found URI string
 * @return User-Agent as a string, if no User-Agent is found then an empty string gets returned
 */
std::string httpServer::readUserAgent(const std::string &buffer) {
    bool found = false;
    std::string uriString;

    for (auto item : buffer) {
        if (item == ' ') {
            if (uriString.find("Agent") != std::string::npos) {
                uriString = "";
                found = true;
                continue;
            }

            if (found) {
                return uriString;
            }
        } else {
            if (item == '/') {
                continue;
            }
            uriString += item;
        }
    }

    return " ";
}

/**
 *
 * @param fileName
 * @return
 */

std::string httpServer::processURI(const std::string &fileName) {
    if (fileName == "allsensors.csv") {
        // Print latest value of all sensors
        std::vector<std::string> files;
        std::vector<std::string> filesContent;
        std::stringstream result;

        for (const auto &entry : std::filesystem::directory_iterator("/")) {
            if (entry.path().string().find(".csv") != std::string::npos) {
                files.push_back(entry.path());
            }
        }
        std::sort(files.begin(), files.end());

        for (const std::string &s : files) {
            std::string file = s.substr(1);

            std::vector<std::string> lines = readCSV(file);
            if (lines.empty()) {
                continue;
            }
            std::string latestLine = lines[lines.size() - 1];

            filesContent.push_back(latestLine);
        }

        for (const std::string &s : filesContent) {
            std::vector<std::string> tmp = splitCSVLine(s);
            result << "{ " << "\n";
            result << " \"sensors\":[ " << "\n";
            for (unsigned int i = 0; i < filesContent.size(); i++) {
                std::vector<std::string> line = splitCSVLine(filesContent.at(i));
                result << generateJSON(line, i == filesContent.size() - 1);
            }
            result << " ]\n";
            result << "}";
            return result.str();
        }
    } else {
        // Print history for one specific sensor
        std::vector<std::string> lines = readCSV(fileName);
        if (lines.empty()) {
            return " ";
        }
        std::string latestLine = lines[lines.size() - 1];

        std::stringstream sensorHistory;
        sensorHistory << "{ " << "\n";
        sensorHistory << " \"sensors\":[ " << "\n";
        for (unsigned int i = 0; i < lines.size(); i++) {
            std::vector<std::string> line = splitCSVLine(lines.at(i));
            sensorHistory << generateJSON(line, i == lines.size() - 1);
        }
        sensorHistory << " ]\n";
        sensorHistory << "}";
        return sensorHistory.str();
    }
}


/**
 * Converts a char pointer to string
 * @param stringPointer given char pointer
 * @param size of the pointer
 * @return the converted string
 */

std::string httpServer::convertToString(char *stringPointer, int size) {
    std::string s;
    for (int i = 0; i < size; i++) {
        s.push_back(stringPointer[i]);
    }
    return s;
}

/**
 * Creates a valid HTML-Header
 * @return valid HTML-Header as a string
 */
std::string httpServer::createHTMLBegin(const std::string &title) {

    std::stringstream htmlBegin;
    htmlBegin << "<!DOCTYPE html>\n";
    htmlBegin << "<html>\n";
    htmlBegin << "<head>\n";
    htmlBegin << "<title>" << title << "</title>\n\n";
    htmlBegin << "<meta charset=\"UTF-8\">\n";
    htmlBegin << "<META HTTP-EQUIV=\"refresh\" CONTENT=\"15\">\n";
    htmlBegin << "</head>\n";
    htmlBegin << "<body>\n";
    return htmlBegin.str();
}

/**
 * Creates a valid HTML-Ending
 * @return valid HTML-Header as a string
 */
std::string httpServer::createHTMLEnd() {
    std::stringstream htmlEnd;
    htmlEnd << "</body>\n";
    htmlEnd << "</html>\n";
    return htmlEnd.str();
}

/**
 *
 * @param line
 * @return
 */
std::vector<std::string> httpServer::splitCSVLine(const std::string &line) {
    std::stringstream ssLine;
    ssLine.str(line);
    std::string part;
    std::vector<std::string> splitLine;

    while (getline(ssLine, part, ',')) {
        splitLine.push_back(part);
    }

    return splitLine;
}

/**
 *
 * @param content
 * @return
 */
std::string httpServer::createHTMLBody(const std::string &content) {
    std::stringstream htmlBody;
    htmlBody << content;
    return htmlBody.str();
}

/**
 * {
 * "properties[":
 *  {
 *   "sensortyp":
 *    {
 *     "value": "string"
 *    },
 *    "sensortyp":
 *    {
 *     "value": "string"
 *    }
 *   }
 *  }
 * @param currentObject
 * @return
 */

std::string httpServer::generateJSON(const std::vector<std::string> &fileLine, bool end) {
    std::string jsonObject;
    std::stringstream jsonStream;
    jsonStream << " { " << "\n";
    jsonStream << "\"ID\": " << fileLine.at(0) << ", \n";
    jsonStream << "\"Value\":" << fileLine.at(1) << ", \n";
    jsonStream << "\"Timestamp\":" << fileLine.at(2) << " \n";
    if (end) {
        jsonStream << "}" << "\n";
    } else {
        jsonStream << "}," << "\n";
    }
    return jsonStream.str();
}

bool httpServer::fileExists(const std::string &fileName) {
    std::ifstream f(fileName);
    return f.good();
}

void sigchld_handler(int signal) {
    int saved_errno = errno;
    while (waitpid(-1, nullptr, WNOHANG) > 0);
    errno = saved_errno;
}
