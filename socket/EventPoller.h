#pragma once
#include <stddef.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <errno.h>
#include <string.h>

namespace yazi {
namespace socket {

class EventPoller
{
    friend class SocketHandler;

public:
    /**
     * @brief 构造函数.
     * @param et   默认是ET模式
     */
    EventPoller(bool et = true);
    ~EventPoller();

    /**
     * @brief 生成 epoll句柄.
     * @param max_connections epoll服务需要支持的最大连接数
     */
    void create(int max_connections);

    /**
     * @brief 添加监听句柄.
     * @param fd    句柄
     * @param ptr   辅助的数据, 可以后续在epoll_event中获取到
     * @param event 需要监听的事件EPOLLIN|EPOLLOUT
     *             
     */
    void add(int fd, void * ptr, __uint32_t events);

    /**
     * @brief 修改句柄事件.
     * @param fd    句柄
     * @param ptr   辅助的数据, 可以后续在epoll_event中获取到
     * @param event 需要监听的事件EPOLLIN|EPOLLOUT
     */
    void mod(int fd, void * ptr, __uint32_t events);

    /**
     * @brief 删除句柄事件.
     * @param fd    句柄
     * @param ptr   辅助的数据, 可以后续在epoll_event中获取到
     * @param event 需要监听的事件EPOLLIN|EPOLLOUT
     */
    void del(int fd, void * ptr, __uint32_t events);

    /**
     * @brief 等待时间.
     * @param millsecond    毫秒
     * @return int          有事件触发的句柄数
     */
    int wait(int millsecond);

protected:

    /**
     * @brief 控制 epoll，将EPOLL设为边缘触发EPOLLET模式
     * @param fd       句柄，在create函数时被赋值
     * @param ptr      辅助的数据, 可以后续在epoll_event中获取到
     * @param events   需要监听的事件
     *                 EPOLLIN  表示对应的文件描述符可以读
     *                 EPOLLOUT 表示对应的文件描述符可以写
     *                 EPOLLPRI 表示对应的文件描述符有紧急的数据可读
     *                 EPOLLERR 表示对应的文件描述符发生错误
     *                 EPOLLHUP 表示对应的文件描述符被挂断
     *                 EPOLLET  表示对应的文件描述符有事件发生
     * @param op       控制 epoll 文件描述符上的事件：注册、修改、删除
     *                 EPOLL_CTL_ADD：注册新的fd到epfd中
     *                 EPOLL_CTL_MOD：修改已经注册的fd的监听事件
     *                 EPOLL_CTL_DEL：从 epfd中删除一个fd；
     * 
     */
    void ctrl(int fd, void * ptr, __uint32_t events, int op);

protected:

    /**
     * epoll
     */
    int m_epfd;

    /**
     * 最大连接数
     */
    int m_max_connections;

    /**
     * 事件集
     */
    struct epoll_event * m_events;

    /**
     * 是否是ET模式
     */
    bool m_et;
};

}}
