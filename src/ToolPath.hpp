/*
 * ToolPath.hpp
 *
 *  Created on: Nov 2, 2016
 *      Author: Charles
 */

#ifndef TOOLPATH_HPP_
#define TOOLPATH_HPP_

#include <vector>
#include <string>

// Temporary measure.
struct Shape {
	std::string name;
	std::string command;

	Shape (std::string na, std::string com) {
		name = na;
		command = com;
	}
};

/**
 * Should encompass all the information that the plotter is capable of reproducing.
 * Will contain a vector of instructions which give the relative speeds of the X and Y motors,
 * 	the number of the drawing implement, and when this operation should take place.
 * Must be sorted, easily read, and
 *
 */
class ToolPath {
public:
	std::vector<Shape> shapes;
};


#endif /* TOOLPATH_HPP_ */
