// Final project, group 16
// Deus Ex Machina: a plot device
// November 2016
// C++ code

// Generates G-code from SVG paths.

#include "tinyxml2.h"

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <cstdlib>

#include "ToolPath.hpp"
#include "Vector2D.hpp"

using namespace tinyxml2;
using std::string;

// Convert path 
string dToG (ToolPath d, double scaleFactor, Vector2D origin) {
	// Example format:
	// m 0,1055.84 815.67999,0 0,-1055.68 L 0,0.16 0,1055.84 Z
	// M is T0 M6, G0
	// L is T1 M6, G1
	// V and A are treated as l
	// Z creates a G1 to the first point.

	std::stringstream output;

	Vector2D datum = d.datum;

	Vector2D point;
	bool relative = false;

	origin *= scaleFactor;

	Vector2D start;

	std::stringstream str;
	str << d.command;

	char curCom = 'L';
	char priorCom = '\0';

	int vecCount = 0;

	str.clear();
	std::cout << "Starting interpretation..." << std::endl<< std::endl;

	output << "(" << d.name << ": " << d.datum << ")" << std::endl;

	while (str) {

		// These can upper case or lower case.
		// Use a bitmask to always get upercase characters.
		if ((str.peek()&0b11011111) >= 'A' && (str.peek()&0b11011111) <= 'Z') {
			// Opcode!
			std::cout << "Operator is " << (char)str.peek() << std::endl;

			priorCom = curCom&0b11011111;

			char com = ' ';
			str >> com;

			relative = com > 'Z'; // lowercase is a relative command.

			com = com&0b11011111;

			std::cout << com << (relative?", relative":"") << std::endl;


			switch (com) {
			case 'M': // Disengage pen. Queue move, immediate.
				output << "T0 M6" << std::endl;
				output << "(M)" << std::endl;
				curCom = com;
				break;
			case 'C': // Engage pen. Queue move, curved..
				output << "(C)" << std::endl;
				curCom = '2';
				break;
			case 'L': // Disengage pen. Queue move, linear.
				output << "(L)" << std::endl;
				curCom = com;
				break;
			case 'Z': // Close shape.
				output << "G1 X" << (start).x << " Y" << (start).y << std::endl;
				output << "(Z)" << std::endl;
				point = start;
				curCom = 'L';
				break;
			default: // Unimplemented command.
				break;
			}



			// Read cooordinates
		} else if ((str.peek() >= '0' && str.peek() <= '9') || str.peek() == '-') {
			std::cout << "Vector start is " << (char)str.peek() << std::endl;
			// Should be a vector.

			Vector2D newPoint(0,0);
			str >> newPoint;

			std::cout << "Input: " << newPoint << (relative?", relative":"") << std::endl;

			if (relative) {
				point += newPoint;
			} else {
				point = newPoint + datum;
			}

			Vector2D curPoint = point;

			curPoint *= scaleFactor;

			std::cout << "Point: " << curPoint << ", "<< curPoint<< std::endl;

			// Perform translation from image space to real space.
			curPoint = curPoint + origin - Vector2D(0,curPoint.y)*2.0;

			//if (!relative) output << "T2 M6" << std::endl;

			switch (curCom) {
			case 'M': // Queue move, immediate.
				output << "G0 X" << curPoint.x << " Y" << curPoint.y << std::endl;
				curCom = 'L'; // Next coordinates are implicit Lines, with same relativity setting.
				start = curPoint;
				output << "T1 M6" << std::endl;
				break;
			case 'C': // Queue move, immediate.
				curCom = '1';
				if (relative)point -= newPoint;
				break;
			case 'L': // Queue move, linear.
				output << "G1 X" << curPoint.x << " Y" << curPoint.y << std::endl;
				break;
			case '2':
				curCom = '1';
				if (relative)point -= newPoint;
				break;
			case '1':
				curCom = '0';
				if (relative)point -= newPoint;
				break;
			case '0':
				curCom = '2';
				output << "G1 X" << curPoint.x << " Y" << curPoint.y << std::endl;
				break;
			default:
				break;
			}

			vecCount++;

		} else {
			str >> std::ws;
		}

	}

	std::cout << vecCount << std::endl;
	return output.str();
}

// Go through SVG tree arrangement to find all paths and add to ToolPath vector.
void gTree(XMLElement *r, std::vector<ToolPath> & output) {
	std::cout << "Element " << r->Name() << std::endl;
	XMLElement * temp = r->FirstChildElement("g");
	do  {
		std::cout << "Element " << temp->Name() << std::endl;
		if (temp->FirstChildElement("path")) {
			output.push_back(ToolPath(
					temp->Attribute("id") ? temp->Attribute("id") : "",
					temp->FirstChildElement("path")->Attribute("d"),
					temp->Attribute("transform") ? temp->Attribute("transform") : "")
				);
		} else {
			gTree(temp, output);
		}
	} while (temp = temp->NextSiblingElement("g"));
}

// Primary access point for program.
int main(int argc, char* argv[]) {
	XMLDocument doc;

	if (argc > 2) doc.LoadFile(argv[1]);
	else doc.LoadFile("Maple_Leaf.svg");
	// XMLDocument does not need closing, since it reads and parses all data before closing immediately.
	
	double scale = 0.3;

	std::ofstream fout;

	if (argc > 3) fout.open(argv[2]);
	else fout.open("G-out.ncc");

	XMLElement* svg = doc.RootElement();

	std::vector<ToolPath> paths;

	if (svg->FirstChildElement("g")) {
		gTree(svg,paths);
	} else {
		std::cout << "SVG root contains no g elements.\n";
		if (svg->FirstChildElement("path")) {
			paths.push_back(ToolPath("Primary",svg->FirstChildElement("path")->Attribute("d"),""));
		}
		if (svg->NoChildren()) {
			paths.push_back(ToolPath("Primary",svg->Attribute("d"),""));
		}
	}



	Vector2D origin(atof(svg->Attribute("width")),atof(svg->Attribute("height")));

	origin.x = 0;

	std::cout << "Origin is at " << origin << std::endl;

	for (std::vector<ToolPath>::const_iterator path = paths.begin(); path != paths.end(); ++path) {
		fout << dToG(*path,scale,origin);
	}
	fout.close();

	return 0;
}



