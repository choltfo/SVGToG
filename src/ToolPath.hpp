// Final project, group 16
// Deus Ex Machina: a plot device
// November 2016
// C++ code

// Declares a struct for a ToolPath.

#ifndef TOOLPATH_HPP_
#define TOOLPATH_HPP_

#include "Vector2D.hpp"
#include <vector>
#include <string>

// Charles Holtforster
// A named path starting from a place.
struct ToolPath {
	std::string name;
	std::string command;
	Vector2D datum;

	ToolPath () {
		name = "";
		command = "";
		datum = Vector2D(0,0);
	}
	
	// Charles Holtforster
	// A constructor with far too much data processing inside it.
	ToolPath (std::string na, std::string com, std::string transform) {
		name = na;
		command = com;
		if (transform.find("translate(") != std::string::npos) {
			std::stringstream parser;
			parser << transform;
			parser.ignore(100,'(');
			parser >> datum;
		} else {
			datum = Vector2D(0,0);
		}
	}
};



#endif /* TOOLPATH_HPP_ */
