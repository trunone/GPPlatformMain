#include "SendImage.h"
#include "VisionStatus.h"
using namespace Robot;
using namespace std;

SendImage* SendImage::m_UniqueInstance = new SendImage();

SendImage::SendImage() {
}

SendImage::~SendImage() {
}

void SendImage::Initialize() {
}

void SendImage::Process() {
    if(!VisionStatus::sendimg_enable) return ;

    FILE *file_fd=fopen("Thief.jpg","r+");
    unsigned long pos=ftell(file_fd);
    fseek(file_fd,0L,SEEK_END);
    unsigned long  size=ftell(file_fd);
    fseek(file_fd,pos,SEEK_SET);
    unsigned char buf[size];
    int readBytes=fread(buf,1,size,file_fd);

    LinuxSocket client;
    client.create();
    if( client.connect ( "192.168.137.200" , port ) )
    {

        client.send(buf,readBytes);

        if(fclose(file_fd)!=0)
        {
            //printf("fclose() error! \n");
        }
        else
        {
            //printf("fclose ##file_fd## success!\n");
        }
        //exit(0);
    }
    VisionStatus::sendimg_enable = false;
}
