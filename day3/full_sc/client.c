#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
//#define SERVER_PORT		6000    //
//#define SERVER_IP		"192.168.99.112"	//服务器IP地址
struct pt_arg{

  int connect_fd;
  char *buf;
};
void *read_server(void *r_arg){
    struct pt_arg *arg1=(struct pt_arg*)r_arg;
    int connect_fd=arg1->connect_fd;
    char *buf=arg1->buf;
    int read_size;
    int file_fd;
    char *filename;
    char file_path[100]="./c_dir/";
    while(1){
      
    read_size=read(connect_fd, buf, strlen(buf));
    if(read_size<0){
	  printf("read error\n");
	}else if(read_size==0){
	  printf("connect_fd (%d) is closed\n",connect_fd);
	  close(connect_fd);
	  break;
	  }
    if(/*is filename*/strncmp(buf, "inforn", 6)){
      filename=buf+6; 
      printf("filename:%s\n",filename);
      strcat(file_path,filename);
      file_fd=open(filename,O_RDWR|O_CREAT,0664);
    }else{

      write(file_fd, buf, strlen(buf));
    }
    memset(buf, '\0', strlen(buf));




  }


    return 0;
} 
void *key_input(void *arg){

    struct pt_arg *arg1=(struct pt_arg*)arg;
    int connect_fd=arg1->connect_fd;
    char *buf=arg1->buf;
  	while (1)
	{
		printf(">");
		fgets(buf, sizeof(buf), stdin);
		if (strcmp(buf, "quit\n") == 0)
		{
			printf("client will quit!\n");
			break;
		}
		write(connect_fd, buf, strlen(buf));
	}
    return 0;


}
int main(int argc, const char *argv[])
{
	int connect_fd = -1;
	struct sockaddr_in server;
	socklen_t saddrlen = sizeof(server);
 
	memset(&server, 0, sizeof(server));
	
	connect_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (connect_fd < 0)
	{
		printf("socket error!\n");
		return -1;
	}
	int port=6000;
	char *ip="127.0.0.1";
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(ip);
 
	if (connect(connect_fd, (struct sockaddr *)&server, saddrlen) < 0)
	{
		printf("connect failed!\n");
		return -1;
	}
 
	char buf[256] = {0};
	char read_buf[256] = {0};
    struct pt_arg r_arg;
    struct pt_arg w_arg;
    r_arg.connect_fd=connect_fd;
    r_arg.buf=read_buf;
    w_arg.connect_fd=connect_fd;
    w_arg.buf=buf;
    /*ptre*/
    pthread_t pt[2];
    pthread_create(&pt[0],NULL,key_input,(void*)&w_arg);
    pthread_create(&pt[1],NULL,read_server,(void*)&r_arg);
    //pthread_join(pt,NULL);
	/*while (1)
	{
		printf(">");
		fgets(buf, sizeof(buf), stdin);
		if (strcmp(buf, "quit\n") == 0)
		{
			printf("client will quit!\n");
			break;
		}
		write(connect_fd, buf, sizeof(buf));
	}*/
	close(connect_fd);
 
	return 0;
}
