#include "tinyxml.h"
#include "tinystr.h"
#include <iostream>
#include <string.h>
using namespace std;
int main()
{	
	
	TiXmlDocument doc("Status.xml");
	doc.LoadFile();
	TiXmlPrinter printer;
	//printer.SetIndent( "\t" );
	
	TiXmlElement* root = doc.RootElement();//Status
	TiXmlElement* element = root->FirstChildElement();//ColorModel
	TiXmlElement* model = element->FirstChildElement();//Model
	TiXmlAttribute* type= model->FirstAttribute();//Model type
	const char *www="www";
	double x[7]={0.1,0.2,0.3,0.4,0.5,0.6,0.7};
	int i=6;
	cout<<type->Value()<<endl;
	for(;model != NULL;model=model->NextSiblingElement()){
		TiXmlElement* modelchild=model->FirstChildElement();
		for(;modelchild != NULL;modelchild=modelchild->NextSiblingElement()){//get node information
			string informationType=modelchild->Value();
			string information=modelchild->FirstAttribute()->Value();
			modelchild->SetDoubleAttribute(www,x[i--]);
			//modelchild->RemoveAttribute(www);
			doc.SaveFile();
			cout <<	informationType << ":" << information << endl;
		}
	}
	doc.Accept( &printer );
	fprintf( stdout, "%s", printer.CStr() );
	//printf("%s", printer.CStr());
	
}
