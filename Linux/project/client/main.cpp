#include <stdio.h>
#include <iostream>
#include "LinuxNetwork.h"

#include <string.h>

using namespace Robot;
using namespace std;
int main(){
	int port=1234;
	int yy;
	string host = "127.0.0.1";
	LinuxSocket rec;
	string aa = "test";
	rec.create();
	cout << aa << endl;
	if(rec.connect ( host , port )) {	
			
		rec.recv(aa);
		cout << aa;	
	}
	
	return 0;
}
         
