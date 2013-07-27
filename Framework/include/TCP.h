#ifndef _TCP_H_
#define _TCP_H_
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
using namespace std;
//----------------------------

namespace Robot
{
	class TCP : public VisionModule
	{
	

	private:
		static TCP* m_UniqueInstance;
		
		TCP();
	public:
		static TCP* GetInstance() { return m_UniqueInstance; }
		
		~TCP();
		void Process();
	};
}
#endif
