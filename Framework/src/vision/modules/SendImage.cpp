#include "SendImage.h"
using namespace Robot;
using namespace std;

SendImage* SendImage::m_UniqueInstance = new SendImage();

SendImage::SendImage() {
}

SendImage::~SendImage() {
}

void SendImage::Initialize() {
	client.create();
}

void SendImage::Process(){
	printf("client start ...\n");
	if(client.connect ( "192.168.230.1" , port ))
	{
		printf("Connect successful!\n");
	}
	if( client.is_valid() )
	{
  	//printf("connect success!\n");
	FILE *file_fd=fopen("1.jpg","r+");
	unsigned long pos=ftell(file_fd);
	fseek(file_fd,0L,SEEK_END);
	unsigned long  size=ftell(file_fd);
	fseek(file_fd,pos,SEEK_SET);
	char buf[70000];
  	//printf("send ...\n");

  	int readBytes=fread(buf,1,sizeof(buf),file_fd);
    	if(readBytes==0)
     	{	
     		//printf("one picture transmit over!\n");
     		//break;
     	}
     	else
     	{
        	//printf(" read from file to buf ##%d## bytes\n",readBytes);
        }
 
    	if(client.send(buf,readBytes))
     	{
      		
printf("send to server\n");
      	}
      	else
       	{
        	printf("send error! \n");
        }
       
   	if(fclose(file_fd)!=0)
   	{
   		//printf("fclose() error! \n");
   	}
   	else
   	{
   		//printf("fclose ##file_fd## success!\n");
	}
		exit(0);
	}
}
