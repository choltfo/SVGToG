//============================================================================
// Name        : SVGToG.cpp
// Author      : Charles Holtforster
// Version     :
// Copyright   :
// Description : Converts SVGs to some sort of tool path.
//============================================================================

#include "tinyxml2.h"

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "ToolPath.hpp"
#include "Vector2D.hpp"


//namespace tx2 = tinyxml2; // Becuase "tinyxml2" is long.

using namespace tinyxml2;
using std::string;
//using namespace std;

double sFactor = 20.0;

void gToG (std::string coords, std::ofstream fout) {
	std::stringstream ss;
	ss << coords;
}

string dToG (string d, double scaleFactor = 1) {
	// Example format:
	// m 0,1055.84 815.67999,0 0,-1055.68 L 0,0.16 0,1055.84 Z
	// M is T0 M6, G0
	// L is T1 M6, G1
	// V and A are treated as l
	// Z creates a G1 to the first point.

	std::stringstream output;

	Vector2D point;

	std::stringstream str;
	str << d;

	char lastCom = '\0';

	char rip = ' ';

	str.clear();
	std::cout << "Starting interpretation." << std::endl;

	while (str.peek() > 0) {
		//std::cout << "Peek is " << (char)str.peek() << " (" << (char)(str.peek()&0b11011111)<< ")"<< std::endl;
		//while (!(str.peek() == '\n' || str.peek() == '\r' || str.peek() == ' ') && !str.fail()) {
		while (str.peek() > 0 &&
				!((
				(str.peek()&0b11011111) >= 'A' && (str.peek()&0b11011111) <= 'Z') ||
				(str.peek() >= '0' && str.peek() <= '9'))
			) {
			str >> rip; // Munch through, one char at a time!
			//std::cout << "Ripped " << (char)rip << std::endl;
			//std::cout << str.str() << std::endl;
		}
		// std::cout << "Found data: " << (char)str.peek()<< std::endl;
		// These can upper case or lower case.
		if ((str.peek()&0b11011111) >= 'A' && (str.peek()&0b11011111) <= 'Z') {
			// Opcode!
			char com = ' ';
			str >> com;
			com = com &0b11011111;

			switch (com) {
			case 'M': // Disengage pen. Queue move, immediate.
				output << "T0 M6" << std::endl;
				std::cout << "T0 M6" << std::endl;
				lastCom = com;
				break;
			case 'L': // Disengage pen. Queue move, linear.
				// TODO: configure tool changes properly.
				output << "T1 M6" << std::endl;
				lastCom = com;
				break;
			}
		} else {
			// Should be a vector.
			str >> point;
			point.x *= scaleFactor;
			point.y *= scaleFactor;
			switch (lastCom) {
			case 'M': // Queue move, immediate.
				output << "G0 X" << point.x << " Y" << point.y << std::endl;
				break;
			case 'L': // Queue move, linear.
				output << "G1 X" << point.x << " Y" << point.y << std::endl;
				break;
			}
		}
		//std::cout << "Peek is " << str.peek() << std::endl;
	}

	//std::cout << output.str() << std::endl;
	std::cout << "Done!" << std::endl;

	return output.str();
}

int main() {
	XMLDocument doc;
	doc.LoadFile("TestSVG.svg");

	std::ofstream fout("G-out.ncc");


	ToolPath tp;

	XMLElement* svg = doc.RootElement();

	std::cout << svg->Name() << std::endl;

	string path = "";

	if (svg->FirstChildElement("g")) {
		if (svg->FirstChildElement("g")->FirstChildElement("g")) {
			if (svg->FirstChildElement("g")->FirstChildElement("g")->FirstChildElement("path")) {
				path = svg->FirstChildElement("g")->FirstChildElement("g")->FirstChildElement("path")->Attribute("d");
			}
		} else if (svg->FirstChildElement("g")->FirstChildElement("path")) {
			path = svg->FirstChildElement("g")->FirstChildElement("path")->Attribute("d");
		}
	} else {
		std::cout << "SVG root contains no g elements.\n";
		if (svg->FirstChildElement("path")) {
			path = svg->FirstChildElement("path")->Attribute("d");
		}
		if (svg->NoChildren()) {
			// Hopefully, it's an SVG file with a single path element!
			path = svg->Attribute("d");
		}
	}



	std::cout << path << std::endl;
	std::cout << dToG(path,0.01) << std::endl;

	//std::cout << "file contains svg element!" << std::endl;


	// Dig into the network.

	/*for(XMLElement* e = svg->FirstChildElement("g"); e != NULL; e = e->NextSiblingElement("g")) {

	    std::string wmName = e->Attribute("id");
	    std::string points = e->FirstChildElement("path")->Attribute("d");

	    tp.shapes.push_back(Shape(wmName,points));

	    std::cout << "Item: " << wmName << ": " << points << std::endl;
	    tp.shapes[0];
	}*/



	return 0;
}



