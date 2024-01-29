#include "centralServer.hpp"
#include "httpServer.hpp"
#include <wait.h>


int main() {
    centralServer *central = new centralServer();
    pid_t udpPID;
    int status;
    switch (udpPID = fork()) {
        case -1:
            std::cerr << "Can not fork" << std::endl;
            exit(1);
        case 0:
            central->getCurrentHttpServer()->listenPort();
            exit(0);
        default:
            central->receivePackets();
            waitpid(udpPID, &status, WNOHANG);
    }
    delete central;
    return 0;
}


