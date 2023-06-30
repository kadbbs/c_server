/*===============================================================
*   Copyright (C) 2023 All rights reserved.
*   
*   文件名称：net_client.c
*   创 建 者：liujing
*   创建日期：2023年06月28日
*   描    述：
*
*   更新日志：
*
================================================================*/



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
 
//#define SERVER_PORT		6000    //
//#define SERVER_IP		"127.0.0.1"	//服务器IP地址
 
int main(int argc, char *argv[])
{
	if(argc!=3){

		printf("argument error\n");
		return -1;
	
	
	}
	int server_port=atoi(argv[2]);    //

	printf("server_port%d",server_port);
	char *server_ip=argv[1];	//服务器IP地址
	int connect_fd = -1;
	struct sockaddr_in server;
	socklen_t saddrlen = sizeof(server);
	memset(&server, 0, sizeof(server));					
	connect_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (connect_fd < 0){
											printf("socket error!\n");
												return -1;							}		 
	server.sin_family = AF_INET;
	server.sin_port = htons(server_port);									server.sin_addr.s_addr = inet_addr(server_ip);											if (connect(connect_fd, (struct sockaddr *)&server, saddrlen) < 0)
										{
																printf("connect failed!\n");
																		return -1;
																			}
											 
												char buf[256] = {0};
													while (1){printf(">");
													fgets(buf, sizeof(buf), stdin);
													if (strcmp(buf, "quit\n") == 0){															printf("client will quit!\n");break;}						write(connect_fd, buf, sizeof(buf));
			}								close(connect_fd);									return 0;
}
