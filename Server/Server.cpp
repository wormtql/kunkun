//
// Created by darkdawn on 2019/8/29.
//

#include <sys/types.h>
#include <gtk/gtk.h>
#include <mysql/mysql.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/shm.h>
#include <time.h>

#define MYPORT    8000                          //server's listen port
#define PORT      8081                          //the begin of srever's chat port
#define BUFSIZE   200
#define MAX_USER_NUM 50

using namespace std;

gint userNum;
gchar online_user[MAX_USER_NUM][20];
gint  ary_sockfd[MAX_USER_NUM];                         //arrar for clients' sockfd
gint global_current_sockfd;

void int_to_string( int i, char *s ){
	s[0] = i/1000+'0'; i %= 1000;
	s[1] = i/100+'0'; i %= 100;
	s[2] = i/10+'0';
	s[3] = i%10+'0';
	s[4] = '\0';
}
/****************************************************
 * Description : the function of pthread
 * Prameter    :
 * Return      : void *
 * Date        : 2019.8.29
 * Ps          :
 ****************************************************/
void* _pthread_entrance(void* p)
 {
    gchar buf[BUFSIZE];
    // char temp[100];
    // char msg_to_self[100];
    // char msg_to_opp[100];
    // char to_who[100];
    gint recv_len;
    gint current_user_ID;
    gint current_user_sockfd = global_current_sockfd;
    // int opposite_user_sockfd;
    int i;
    for(i=0;i<MAX_USER_NUM;i++)
        if(current_user_sockfd==ary_sockfd[i])
            break;
    current_user_ID=i;

    while(1)
    {
        memset(buf,0,sizeof(buf));
        recv_len=recv(current_user_sockfd,buf,BUFSIZE,0);    //reserve log request
        g_print("%d %s\n",recv_len, buf);

        if(recv_len<=0)
        {
            userNum--;
            memset(online_user[current_user_ID],0,sizeof(online_user[0]));
            ary_sockfd[current_user_ID]=0;
            close(current_user_sockfd);
            return NULL;
        }

    }
}
/****************************************************
 * Description : init server, the main function entry
 * Prameter    :
 * Return      :
 * Date        : 2019.8.29
 * Ps          :
 ****************************************************/
int main(int argc, char* argv[])
{
	int i;

	gchar buf[BUFSIZE];

	gint listen_sockfd, server_sockfd, client_sockfd;
	struct sockaddr_in listen_sockaddr, client_sockaddr, server_sockaddr; //套接字地址结构
	socklen_t listen_len, client_len, server_len;

	struct in_addr sin_addr, client_ip;

	pthread_t ptid[MAX_USER_NUM];
	g_print("\n====================== server initialization ======================\n");

	listen_sockfd = socket(AF_INET,SOCK_STREAM,0);//创建套接字
	if( listen_sockfd == -1){
		g_print("create socket error!\n");//创建时出错
		exit(1);
	}else{
		g_print("create socket success!\n");
	}

	listen_sockaddr.sin_family = AF_INET; //套接字类型
	listen_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); //自动获取本机的IP地址
	listen_sockaddr.sin_port = htons(MYPORT); //端口号
	listen_len = sizeof(struct sockaddr_in);

	int on = 1;                              //允许重复使用本地地址和套接字绑定
	setsockopt(listen_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	if(bind(listen_sockfd,(struct sockaddr*)&listen_sockaddr,listen_len)<0){ //向指定端口绑定
		g_print("bind error!\n");//绑定时出错
		exit(1);
	}else{
		g_print("bind success!\n");
	}

	if(listen(listen_sockfd,8)<0){ //监听
		g_print("listen error!\n");//监听出错
		exit(1);
	}

	for(;;){ //死循环,等侍用户连接
		client_len = sizeof(struct sockaddr_in);

		client_sockfd = accept(listen_sockfd,(struct sockaddr*)&client_sockaddr,&client_len);//取得用户连接的套接字
		if(client_sockfd<0){
			g_print("accept error!\n");//连接出错
			break;
		}else{
			g_print("accept success!\n");
			if( userNum >= MAX_USER_NUM ){
				sprintf(buf, "Too much users are online!\nConnect fail!\n");
				send(client_sockfd,buf,strlen(buf),0);
				close(client_sockfd);
				continue;
			}else{
				sin_addr = client_sockaddr.sin_addr;
				int_to_string( PORT+userNum, buf );
				send(client_sockfd,buf,strlen(buf),0); // 分配端口信息
			}
		}

		server_sockfd = socket(AF_INET,SOCK_STREAM, 0); // 定义套接字类型
		server_sockaddr.sin_family = AF_INET;
		server_sockaddr.sin_port = htons(PORT+userNum);
		server_sockaddr.sin_addr = sin_addr;
		server_len = sizeof(struct sockaddr_in);

		memset(&(server_sockaddr.sin_zero),0,sizeof(server_sockaddr.sin_zero));
        sleep(1);

        for(i=0;i<MAX_USER_NUM;i++)
            if(ary_sockfd[i]==0)
                break;
        ary_sockfd[i]=server_sockfd;
        global_current_sockfd=server_sockfd;

		if (connect(server_sockfd,(struct sockaddr *)&server_sockaddr,server_len)<0){
			g_print("connect error!\n");
			break;
		}else{
			g_print("connect succuss!\n");
			userNum++;
        }
        pthread_create(&ptid[userNum],NULL,_pthread_entrance,NULL);

	}
	close(listen_sockfd); //关闭套接字
	return 0;
}
