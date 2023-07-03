/*===============================================================
*   Copyright (C) 2023 All rights reserved.
*   
*   文件名称：net.c
*   创 建 者：liujing
*   创建日期：2023年06月28日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <math.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#define SERVER_PORT 6000
#define SERVER_IP "127.0.0.1"


int my_strncmp(char *str1,char *str2,int n){


  if(strncmp(str1,str2,n)==0){

    return 1;
  }else if(strncmp(str1,str2,n)!=0){

    return 0;
  }
  return -1;
}
int file_yn(char *name,char *filelist[]);
/*struct sockadder{
	u_short sa_family;
	char sa_data[14];

}

struct sockaddr_in{
	u_short sin_family;
	u_short sin_port;
	struct in_addr sin_addr;
	char sin_zero[8];

}

struct sockaddr_un{

	sa_family_t sun_family;
	char sun_path[108];

}*/
int get_file_list(char **filelist){

  
    char *server_tree_dir="./s_dir/";
    DIR *dir_tree=opendir(server_tree_dir);
    struct dirent *dir_volu;
    int i=0;
    while((dir_volu=readdir(dir_tree))!=NULL){
      filelist[i]=dir_volu->d_name;
	  printf("filename:%s",filelist[i]);
	  printf("\n");
	  i++; 
    }
	closedir(dir_tree);

  return 0;

}

int listen_fd =-1;

int main(){

	
    /*char *server_tree_dir="./s_dir/";
    DIR *dir_tree=opendir(server_tree_dir);
    struct dirent *dir_volu;
    int i=0;
    char *filelist[100];
    while((dir_volu=readdir(dir_tree))!=NULL){
      filelist[i]= dir_volu->d_name;
	  printf("filename:%s",filelist[i]);
	  printf("\n");
	  i++; 
    }
	closedir(dir_tree);*/
	//char file_path_volu[9]="./one.txt";
    //char *file_path="./one.txt";
//	int file_fd=open("./one.txt",O_RDWR|O_CREAT|O_APPEND,0664);
    char *filelist[100];
	struct sockaddr_in server;
	struct sockaddr_in client;
	socklen_t saddrlen =sizeof(server);
	socklen_t caddrlen =sizeof(client);

	int socket_server=socket(AF_INET,SOCK_STREAM,0);
	if(socket_server<0){
		printf("socket error\n");
		return -1;
	
	}
	server.sin_family=AF_INET;
	server.sin_port=htons(SERVER_PORT);
	server.sin_addr.s_addr=inet_addr(SERVER_IP);

	


	int bind_server=bind(socket_server,(struct sockaddr *)&server,saddrlen);
	if(bind_server<0){
	
		printf("bind error\n");
		return -1;
	
	}

	if(listen(socket_server,5)<0){
	
		printf("listen error\n");
		return -1;
	
	
	}
    int file_fd=-1;
	char rbuf[256]={0};
	char read_buf[256]={0};
	int read_size=0;
	int push_file=-1;
	int down_file=-1;
    int push_client_fd=-2;
    int push_flog=-2;
    int clone_flog=-2;
    char *file_name;
    char file_path[100]="./s_dir/";
	while(1){
	
		int new_fd =accept(socket_server,(struct sockaddr *)&client,&caddrlen);
		if(new_fd<0){
			perror("accept");
			return -1;
		}
		while(1){
 			read_size=read(new_fd,rbuf,sizeof(rbuf));
			if(read_size<0){
				printf("read error\n");
			}else if(read_size==0){
				printf("client (%d) is closed\n",new_fd);
				close(new_fd);
				break;
			}
			printf("recv:%s\n",rbuf);
            int str_push=my_strncmp(rbuf,"pushc:",6);
            int str_clone=my_strncmp(rbuf,"clone:",6);
            int inform=!str_push^str_clone;
            if(!inform){
              //file_path=strcpy(file_path_volu, rbuf+6);
			  	strcpy(file_path,"./s_dir/");
                file_name=strstr(rbuf,":")+1;
				file_name[strlen(file_name)-1]='\0';
				printf("path_name=%s\n",file_name);
                strncat(file_path,file_name,strlen(file_name));
                printf("path_name=%s\n",file_name);
            }
			if(str_push|inform){
              if(str_push){
                printf("push_state\n");
	            file_fd=open(file_path,O_RDWR|O_CREAT|O_APPEND,0664);
				memset(file_path,'\0',sizeof(file_path));
			  }
				//client to server data
			  write(file_fd,rbuf,strlen(rbuf));
			  memset(rbuf,'\0',sizeof(rbuf));
			
			}else if(str_clone){
			
                //file_path="./one.txt";
                printf("get clone state\n");
				//server to client data
                get_file_list(filelist);
			    if(/*hava file*/file_yn(file_name,filelist)){
                  if(str_clone){
				    push_client_fd=open(file_path,O_RDONLY,0);
					memset(file_path,'\0',sizeof(file_path));
                    char bao_name[100]="inforn";
                    strcat(bao_name,file_name);
				    write(new_fd,bao_name,strlen(bao_name));
					usleep(100);
					strcpy(bao_name,"inforn");
			        memset(read_buf,'\0',sizeof(read_buf));
                	}
                  while(1){
				    int l_size=read(push_client_fd,read_buf,sizeof(read_buf));
                    if(l_size<=0){
                      printf("filesize=0\n");
                      break;

                    }
                    
				    write(new_fd,read_buf,strlen(read_buf));
                    printf("write:%s",read_buf);
			        memset(read_buf,'\0',sizeof(read_buf));
                  }
			    }else{
					printf("no file\n");
				}
				
			}
			/*write(file_fd,rbuf,strlen(rbuf));
			memset(rbuf,'\0',sizeof(rbuf));
			if(push_file|strcmp(rbuf,file_path)){
				push_client_fd=open(file_path,O_RDONLY,0);
				read_size=read(push_client_fd,read_buf,sizeof(read_buf));
				write(new_fd,read_buf,strlen(read_buf));
			
			}*/
		}
	}
	close(socket_server);
	return 0;
}

int file_yn(char *name,char **filelist){
  int i=0;
  int yn;
  for(i=0;i<100;i++){
	int n=strlen(name)>strlen(filelist[i])?strlen(filelist[i]):strlen(name);
    yn=strncmp(name, filelist[i],n);
    printf("filename:%s\n",filelist[i]);
	if(yn==0)
      return 1;

  }
  return 0;
}
