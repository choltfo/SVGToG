//============================================================================
// Name        : SVGToG.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>

#include "tinyxml2.h"

#include "ToolPath.hpp"


//namespace tx2 = tinyxml2; // Becuase "tinyxml2" is long.

using namespace tinyxml2;

//using namespace std;

int main() {
	XMLDocument doc;
	doc.LoadFile( "TestSVG.svg" );

	ToolPath tp;

	std::cout << doc.FirstChildElement("svg")->FirstChildElement("g")->Attribute("id") << std::endl;

	XMLElement* root = doc.FirstChildElement("svg")->FirstChildElement("g");
	for(XMLElement* e = root->FirstChildElement("g"); e != NULL; e = e->NextSiblingElement("g")) {

	    std::string wmName = e->Attribute("id");
	    std::string points = e->FirstChildElement("path")->Attribute("d");

	    tp.shapes.push_back(Shape(wmName,points));

	    std::cout << wmName << ": " << points << std::endl;
	}



	return 0;
}



