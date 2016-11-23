/*
 * ToolPath.hpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Charles
 */

#ifndef TOOLPATH_HPP_
#define TOOLPATH_HPP_

#include "Vector2D.hpp"
#include <vector>
#include <string>

struct ToolPath {
	std::string name;
	std::string command;
	Vector2D datum; // And so, the mess begins

	ToolPath () {
		name = "";
		command = "";
		datum = Vector2D(0,0);
	}

	ToolPath (std::string na, std::string com, std::string transform) {
		name = na; // Well, maybe.
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
