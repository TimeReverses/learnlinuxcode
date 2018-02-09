#ifndef _WRAP_H_
#define _WRAP_H_
void sys_err(const char* msg);
int Accept(int sockfd,struct sockaddr* addr,socklen_t* addrlen);
int Socket(int domain,int type,int protocol);
int Bind(int sockfd,const struct sockaddr* addr,socklen_t addrlen);
int Listen(int sockfd,int backlog);
int Connect(int sockfd,const struct sockaddr* addr,socklen_t addrlen);
ssize_t Read(int fd,void* buf,size_t count);
ssize_t Write(int fd,void* buf,size_t count);
int Close(int fd);

#endif
