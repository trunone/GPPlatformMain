#include "TCP.h"
using namespace Robot;
using namespace std;
void TCP::Process(){
	struct hostent *h;
	printf("client start ...\n");
	FILE *file_fd=fopen("a.jpg","r+");
	unsigned long pos=ftell(file_fd);
	fseek(file_fd,0L,SEEK_END);
	unsigned long  size=ftell(file_fd);
	fseek(file_fd,pos,SEEK_SET);
 
	char buf[size];
	cout<<size<<endl;
	int socket_connect=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server_addr;
 
	bzero(&server_addr,sizeof(server_addr));
 

	server_addr.sin_family  =AF_INET;
	server_addr.sin_port  =htons(port);
	server_addr.sin_addr.s_addr=inet_addr(IP);
 
	printf("connect ...\n");
	if(connect(socket_connect,(struct sockaddr *)&server_addr,sizeof(server_addr))==-1)
	{
		printf("connect error!\n");
  		exit(1);
  	}
  	printf("connect success!\n");
  	printf("send ...\n");
  	int sendSequence=0;
  	while(1)
  	{
  	int readBytes=fread(buf,1,sizeof(buf),file_fd);
    	if(readBytes==0)
     	{	
     		printf("one picture transmit over!\n");
     		break;
     	}
     	else
     	{
        	printf(" read from file to buf ##%d## bytes\n",readBytes);
        }
    	int sendBytes=send(socket_connect,buf,readBytes,0);
    	if(sendBytes<0)
     	{
      		printf("send error! \n");
      		exit(1);
      	}
      	else
       	{
        	sendSequence++;
        	printf("%d send to server:%d bytes\n",sendSequence,sendBytes);
        }
	    	bzero(buf,sizeof(buf));
	}
	if(close(socket_connect)!=0)
   	{
   		printf("close() error! \n");
   	}	
   	else
   	{
   		printf("close ##socket_connect## success!\n");
   	}
       
   	if(fclose(file_fd)!=0)
   	{
   		printf("fclose() error! \n");
   	}
   	else
   	{
   		printf("fclose ##file_fd## success!\n");
	}
}
