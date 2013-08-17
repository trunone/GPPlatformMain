#include <stdio.h>
#include <iostream>
#include <tinyxml.h>
#include "LinuxNetwork.h"

#include <string.h>

using namespace Robot;
using namespace std;
int main() {
    TiXmlDocument doc("Robot_Test.xml");
    doc.LoadFile();
    TiXmlPrinter printer;
    printer.SetStreamPrinting();
    doc.Accept( &printer );
    printf("%s", printer.CStr());
    LinuxSocket client;
    client.create();
    if(client.connect ( "192.168.137.75" , 10373 )) {
        client.send(printer.CStr());
    }

    return 0;
}

