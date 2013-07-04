#include <stdio.h>
#include <iostream>
#include "LinuxNetwork.h"



using namespace Robot;
using namespace std;
int main(){
	int port=1234;
	string aa;
	char w;
	w=0x08;
	LinuxServer new_sock;
        LinuxServer server (port);
	while(true){
        	cout << "[Waiting..]" << endl;            
            	server.accept ( new_sock );
            	cout << "[Accepted..]" << endl;
		while(true){
			
			new_sock >> aa;
			cout<<aa;
			//new_sock.send(w, 14);
			cout << "[success]" << endl;		
		}
		
	}
return 0;
}
         
