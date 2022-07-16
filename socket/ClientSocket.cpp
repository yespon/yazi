#include "ClientSocket.h"
#include "Logger.h"
using namespace yazi::utility;
using namespace yazi::socket;

#include <cerrno>
using namespace std;

#include <cstring>
using namespace std;

ClientSocket::ClientSocket() : Socket()
{
}

ClientSocket::ClientSocket(const string& ip, int port) : Socket(ip, port)
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sockfd < 0)
    {
        error("create client socket error: errno=%d errstr=%s", errno, strerror(errno));
        return;
    }
    Socket::connect(ip, port);
}

ClientSocket::~ClientSocket()
{
    close();
}
