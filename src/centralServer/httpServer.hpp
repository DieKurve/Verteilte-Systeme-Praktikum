#ifndef VS_PRAKTIKUM_SRC_HTTPSERVER_H_
#define VS_PRAKTIKUM_SRC_HTTPSERVER_H_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <wait.h>

class httpServer {
public:

    httpServer();

    virtual ~httpServer();

    void sendAnswer();

    void createTCPSocket();

    std::vector<std::string> readCSV(const std::string &filename);

    void listenPort();

    bool checkForHTTPGET(const std::string &);

    bool fileExists(const std::string &);

    std::string readURI(const std::string &);

    std::string readUserAgent(const std::string &);

    std::string processURI(const std::string &);

    std::vector<std::string> splitCSVLine(const std::string &);

    std::string generateJSON(const std::vector<std::string> &, bool);

    std::string convertToString(char *stringPointer, int size);

    std::string createHTMLBegin(const std::string &);

    std::string createHTMLBody(const std::string &);

    std::string createHTMLEnd();

private:

    struct sockaddr_in address{};
    int addrlength = sizeof(address);
    int socketfd = 0;
    int httpSocket = 0;


};

#endif //VS_PRAKTIKUM_SRC_HTTPSERVER_H_
