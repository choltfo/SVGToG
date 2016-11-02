//============================================================================
// Name        : SVGToG.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

#include "tinyxml2.h"

namespace tx2 = tinyxml2; // Becuase "tinyxml2" is long.

//using namespace std;



int main() {


	tx2::XMLDocument doc;
	doc.LoadFile("TestSVG.SVG");

	return 0;
}
