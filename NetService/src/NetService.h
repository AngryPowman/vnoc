/*
 * NetService.h
 *
 *  Created on: 2011-9-30
 *      Author: AngryPowman
 */

#ifndef NETSERVICE_H_
#define NETSERVICE_H_

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <memory.h>

using namespace std;

#define SOCKET_ERROR 					-1
#define MAX_FDS									256
#define MAX_EVENTS							20
#define MAX_LISTEN_QUEUE			SOMAXCONN
#define MAX_EPOLL_TIME_OUT		120

enum NET_ERROR
{
	OK = 0,
	SOCKET_FAILED = 1,
	BIND_FAILED = 2,
	LISTEN_FAILED = 3,
	EPOLL_FAILED = 4,
	GET_FD_OPT_FAILED = 5,
	SET_FD_OPT_FAILED = 6,
	EPOLL_CTL_FAILED = 7
};

class NetService
{
public:
	NetService();
	virtual ~NetService();

public:
	bool		start(unsigned int u_port);
	int		send_pkg(int client_fd, const char *package, size_t length);
	int		recv_pkg(int sock_fd, char *buff, size_t len);
	void		close_fd(int sock_fd);

public:
	NET_ERROR get_net_error();

protected:
	virtual void on_accept(int client_fd, struct sockaddr_in addr);
	virtual void on_receive(int client_fd, struct sockaddr_in addr, const char *buffer, const size_t buff_len);
	virtual void on_close(int client_fd, struct sockaddr_in addr);

private:
	void set_net_error(NET_ERROR err);
	bool setnonblocking(int fd);

private:
	int listen_fd_;
	int epoll_fd_;
	epoll_event events_[MAX_EVENTS];
	NET_ERROR net_error_;
};


#endif /* NETSERVICE_H_ */
