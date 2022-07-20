#pragma once

#include <string>
using namespace std;

namespace yazi {
namespace server {

class Server
{
public:
    Server();
    ~Server();

    void listen(const string & ip, int port);
    void start();
    void set_threads(int threads);
    void set_connects(int connects);
    void set_wait_time(int wait_time);

private:
    string m_ip;
    int m_port;
    int m_threads;
    int m_connects;
    int m_wait_time;
};

}}
