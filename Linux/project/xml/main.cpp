#include "tinyxml.h"
#include "tinystr.h"
#include <iostream>
#include <string.h>

using namespace std;
int main()
{	
	
	
	//TiXmlDocument doc("Robot_Test.xml");
	TiXmlDocument doc("Status.xml");
	doc.LoadFile();
	//printer.SetIndent( "\t" );
	
	//string element_for;
	double x,y,z;
	string name_for_dir;
	string value_for_dir;
	char *ManualDirection="ManualDirection";
	char *Vision="Vision";
	TiXmlElement* root = doc.RootElement();//Status
	TiXmlElement* element = root->FirstChildElement();//ManualDirection,Vision
	for(;element != NULL;element=element->NextSiblingElement()){
		cout<<element->Value()<<endl;
		if(strcmp(element->Value(),ManualDirection)==0){
				TiXmlElement* modelchild=element->FirstChildElement();
				for(;modelchild != NULL;modelchild=modelchild->NextSiblingElement()){//node information
					TiXmlAttribute* location=modelchild->FirstAttribute();
					for(;location != NULL;location=location->Next()){
						name_for_dir=location->Name();	
						if(name_for_dir=="x"){
							x=atof(location->Value());
						}else if(name_for_dir=="y"){
							y=atof(location->Value());
						}else if(name_for_dir=="w"){
							z=atof(location->Value());
						}
					
						//value_for_dir=location->Value();
						//cout <<	name_for_dir << ":" << value_for_dir << endl;
					}
				}

		}else if(strcmp(element->Value(),Vision)==0){
				TiXmlElement* model=element->FirstChildElement();
				for(;model != NULL;model=model->NextSiblingElement()){
					//cout<<model->FirstAttribute()->Value()<<endl;
					TiXmlElement* modelchild=model->FirstChildElement();
					for(;modelchild != NULL;modelchild=modelchild->NextSiblingElement()){//node information
						string informationType=modelchild->Value();
						string parameter=modelchild->FirstAttribute()->Value();
						//modelchild->SetDoubleAttribute(www,0.85);
						cout <<	informationType << ":" << parameter << endl;
					}
				}
		}
	}
	cout<<"X:"<<x<<endl<<"Y:"<<y<<endl<<"Z:"<<z<<endl;
	//doc.Accept( &printer );
	//fprintf( stdout, "%s", printer.CStr() );
	//printf("%s", printer.CStr());
}
