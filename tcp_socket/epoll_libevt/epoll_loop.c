#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define SERV_IP "192.168.206.137"
#define SERV_PORT 9527
#define MAXCONN 1024 //最大连接数

int g_epfd;
struct myevent_t g_events[MAXCONN + 1];

struct myevent_t
{
	int fd;
	int events;
	void* arg;
	void (*call_back)(int fd,int events,void* arg);
	int status;//0-->没有添加到红黑树上,1-->添加到红黑树上
	char buf[BUFSIZ];
	int len;
	long last_active;//最近活跃时间
};

void eventset(struct myevent_t* ev,int fd,void (*call_back)(int,int,void*),void* arg)
{
	ev->fd = fd;
	ev->call_back = call_back;
	ev->events = 0;
	ev->arg = arg;
	ev->status = 0;//还没有添加到红黑树上
	memset(ev->buf,0,sizeof(ev->buf));//清空为0
	ev->len = 0;
	ev->last_active = time(NULL);
}

void eventadd(int epfd,int events,struct myevent_t *ev)
{
	struct epoll_event event;
	event.events = ev->events = events;
	event.data.ptr = ev;
	int op;

	if(ev->status == 1)
		op = EPOLL_CTL_MOD;	
	else
		op = EPOLL_CTL_ADD;
	int ret = epoll_ctl(epfd,op,ev->fd,&event);
	event.data.status = 1;//添加到树上
	if(ret < 0)
	{
		fprintf(stderr,"epoll_ctl in %s err %s\n",__func__,strerror(errno));
	}
	return;
}

void eventdel(int epfd,struct myevent_t* ev)
{
	struct epoll_event epv;
	if(ev->status != 1)
		return;
	epv.data.ptr = ev;
	ev->status = 0;
	epoll_ctl(epfd,EPOLL_CTL_DEL,ev->fd,&epv);
}

void acceptconn(int fd,int events,void* arg)
{
	struct myevent_t* ev = (struct myevent_t*)arg;
	struct sockaddr_in sin;
	socklen_t len;
	len = sizof(sin);
	int connfd = accept(fd,&sin,&len);
	//设置这个文件描述符为非阻塞的
	fcntl(connfd,F_SETFL,O_NONBLOCK);
	do{
		for(int i = 0;i< MAXCONN;i++)
		{
			if(g_events[i].status == 0)
				break;
		}
		if(i == MAXCONN)
		{
			printf("too many client\n");
			break;
		}
		eventset(&g_events[i],connfd,recvdata,&g_events[i]);
		eventadd(g_epfd,EPOLLIN,&g_events[i]);
	}while(0);
	printf("new connect [%s:%d][time:%ld],pos[%d]\n",
		inet_ntoa(sin.sin_addr),inet_ntohs(sin.sin_port),g_events[i]->last_active,i);
	return;
}

void recvdata(int fd,int events,void* arg)
{
	int n;
	struct myevent_t *ev = (struct myevent_t*)arg;
	n = read(fd,ev->buf,sizeof(ev->buf));
	eventdel(g_efd,ev);
	if(n > 0)	
	{
		ev->len = n;	
		ev->buf[n] = '\0';
		printf("C[%d]:%s\n",fd,ev->buf);
		eventset(ev,ev->fd,senddata,ev);	
		eventadd(g_epfd,EPOLLOUT,ev);
	}
	else if(n == 0)
	{
		//客户端关闭
		close(ev->fd);
		printf("[fd=%d] pos[%ld], closed\n", fd, ev-g_events);
	}
	else
	{
		close(ev->fd);
        	printf("recv[fd=%d] error[%d]:%s\n", fd, errno, strerror(errno));
	}
}

void senddata(int fd,int events,void* arg)
{
	struct myevent_t* ev = (struct myevent_t*)arg;
	int len;
	len = send(fd,ev->buf,ev->len,0);
	eventdel(g_epfd,ev);
	if(len > 0)
	{
		eventset(ev,fd,recvdata,ev);
		eventadd(g_epfd,EPOLLIN,ev);
	}
	else
	{
		close(ev->fd);
	}
}

void initlistensocket(int g_epfd,unsigned short port)
{
	//创建一个监听的socket
	int listenfd = socket(AF_INET,SOCK_STREAM,0);
	//设置这个监听的文件描述符为非阻塞的
	fcntl(listenfd,F_SETFL,O_NONBLOCK);
	//初始化结构体
	//void eventset(struct myevent_t* ev,int lfd,void (*call_back)(int,int,void*),void* arg);
	eventset(&g_events[MAXCONN],listenfd,acceptconn,&g_events[MAXCONN]);
	//将该结构体添加到红黑树上
	//void eventadd(int efd,int events,struct myevent_t* ev);
	eventadd(g_epfd,EPOLLIN,&g_events[MAXCONN]);
	//绑定IP和port
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	inet_pron(AF_INET,SERV_IP,&sin.sin_addr.s_addr);
	bind(listenfd,(struct sockaddr*)&sin,sizeof(sin));
	//设置最大同时连接数
	listen(listenfd,64);
	return;	
}

int main(int args,char* argv[])
{
	unsigned short port = SERV_PORT;
	struct epoll_event events[MAXCONN];
	if(args == 2)
	{
		port = atoi(argv[1]);
	}
	//创建一个红黑树
	g_epfd = epoll_create(MAXCONN + 1);
	if(g_epfd < 0)
	{
		printf("create efd in %s err %s\n",__func__,strerror(errno));
	}
	//初始化用于连接的套接字
	initlistensocket(g_epfd,port);
	while(1)
	{
		int nfd = epoll_wait(g_epfd,events,MAXCONN+1,1000);
		for(i = 0;i <nfd;i++)
		{
			
		}
	}
}
