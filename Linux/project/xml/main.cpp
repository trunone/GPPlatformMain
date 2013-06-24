#include "tinyxml.h"
#include "tinystr.h"
#include <iostream>
#include <string.h>
using namespace std;
int main()
{
	TiXmlDocument doc("Status.xml");
	doc.LoadFile();
	
	TiXmlElement* root = doc.RootElement();//Status
	TiXmlElement* element = root->FirstChildElement();//ColorModel
	TiXmlElement* model = element->FirstChildElement();//Model
	TiXmlAttribute* type= model->FirstAttribute();//Model type
	
	cout<<type->Value()<<endl;
	for(;model != NULL;model=model->NextSiblingElement()){
		TiXmlElement* modelchild=model->FirstChildElement();
		for(;modelchild != NULL;modelchild=modelchild->NextSiblingElement()){//get node information
			string informationType=modelchild->Value();
			string information=modelchild->GetText();
			cout <<	informationType << ":" << information << endl;
		}
	}
	
		
}
