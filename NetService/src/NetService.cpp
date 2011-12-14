/*
 * NetService.cpp
 *
 *  Created on: 2011-10-1
 *      Author: AngryPowman
 */

#include "NetService.h"

NetService::NetService()
{
	set_net_error(OK);
	listen_fd_ = 0;
	bzero(&events_, MAX_EVENTS);
}

NetService::~NetService()
{

}

bool NetService::start(unsigned int u_port)
{
	//创建监听套接字
	listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_fd_ == 0)
	{
		cerr << "Create socket >>> Failed." << endl;
		set_net_error(SOCKET_FAILED);
		return false;
	}

	cout << "Create socket >>> OK." << endl;

	//把监听套接字设置为非阻塞
	if (setnonblocking(listen_fd_) == false)
	{
		return false;
	}

	//绑定地址族
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(u_port);

	int bind_err = bind(listen_fd_, (const sockaddr*)&addr, sizeof(addr));
	if (bind_err == SOCKET_ERROR)
	{
		cerr << "Bind address >>> Failed." << endl;
		set_net_error(BIND_FAILED);
		return false;
	}

	cout << "Bind address >>> OK." << endl;

	//创建epoll
	epoll_fd_ = epoll_create(MAX_FDS);
	if (epoll_fd_ == SOCKET_ERROR)
	{
		cerr << "Create epoll >>> Failed." << endl;
		set_net_error(EPOLL_FAILED);
		return false;
	}

	cout << "Create epoll >>> OK." << endl;

	//注册epoll事件
	epoll_event ev;
	ev.data.fd = listen_fd_;
	ev.events = EPOLLIN | EPOLLET;

	int ep_ctl_err = epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, listen_fd_, &ev);
	if (ep_ctl_err == SOCKET_ERROR)
	{
		cerr << "epoll_ctl >> Failed." << endl;
		set_net_error(EPOLL_CTL_FAILED);
		return false;
	}

	//进入监听
	int listen_err = listen(listen_fd_, MAX_LISTEN_QUEUE);
	if (listen_err == SOCKET_ERROR)
	{
		cerr << "Listen >>> Failed." << endl;
		set_net_error(LISTEN_FAILED);
		return false;
	}

	cout << "Listening..." << endl;

	set_net_error(OK);

	//主循环
	while (true)
	{
		int nfds = epoll_wait(epoll_fd_, events_, MAX_EVENTS, MAX_EPOLL_TIME_OUT);
		if (nfds <= 0)
			continue;

		//遍历通知列表
		for (int i = 0; i < nfds; i++)
		{
			if (events_[i].data.fd == listen_fd_)
			{
				sockaddr_in addr_client;
				socklen_t addr_len = sizeof(addr_client);
				int fd_client = accept(listen_fd_, (sockaddr*)&addr_client, &addr_len);
				if (fd_client == SOCKET_ERROR)
				{
					cout << "accept failed." << endl;
					continue;
				}

				//把客户端套接字设置为非阻塞
				setnonblocking(fd_client);

				//设置socket属性
				epoll_event ev_client;
				ev_client.data.fd = fd_client;
				ev_client.events = EPOLLIN | EPOLLET;
				epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd_client, &ev_client);

				cout << "New Connection." << endl;

				on_accept(fd_client, addr_client);
			}
			else if (events_[i].events & EPOLLIN)
			{
				cout << "Recv Data." << endl;
			}

		}

	}


	return true;
}

int NetService::send_pkg(int client_fd, const char *package, size_t length)
{
	return 0;
}

int NetService::recv_pkg(int sock_fd, char *buff, size_t len)
{
	return 0;
}

void NetService::close_fd(int sock_fd)
{

}

void NetService::on_accept(int client_fd, struct sockaddr_in addr)
{
	cout << "on accept()" << endl;
}

void NetService::on_receive(int client_fd, struct sockaddr_in addr, const char *buffer, const size_t len)
{
	cout << "on receive()" << endl;
}

void NetService::on_close(int client_fd, struct sockaddr_in addr)
{

}

NET_ERROR NetService::get_net_error()
{
	return net_error_;
}

void NetService::set_net_error(NET_ERROR err)
{
	net_error_ = err;
}

bool NetService::setnonblocking(int fd)
{
	//获取文件描述符当前配置
	int opts = fcntl(fd, F_GETFL);
	if (opts < 0)
	{
		cerr << "Get fd options >>> Failed." << endl;
		set_net_error(GET_FD_OPT_FAILED);
		return false;
	}

	//把文件描述符设置为非阻塞
	opts |= O_NONBLOCK;
	int new_opts = fcntl(fd, F_SETFL, opts);
	if (new_opts < 0)
	{
		cerr << "Set fd options >>> Failed." << endl;
		set_net_error(SET_FD_OPT_FAILED);
		return false;
	}

	cout << "set nonblocking >>> OK." << endl;
	return true;
}
