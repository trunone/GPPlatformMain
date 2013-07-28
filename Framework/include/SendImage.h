#ifndef _SendImage_H_
#define _SendImage_H_
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<error.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<iostream>
#include "VisionModule.h"
#define port 13140
#define IP 127.0.0.1
using namespace std;
//----------------------------

namespace Robot
{
	class SendImage : public VisionModule
	{
	

	private:
		static SendImage* m_UniqueInstance;
		SendImage();
	public:
		
		static SendImage* GetInstance() { return m_UniqueInstance; }
		
		~SendImage();
        void Initialize();
		void Process();
	};
}
#endif
