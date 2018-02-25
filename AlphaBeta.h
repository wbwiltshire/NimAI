#pragma once

#include <algorithm>
#include <iostream>
#include <string>
#include <limits>
#include <vector>

class Node {
public:
	std::vector<Node*> children;
	std::string name;
	int value;

	//ctor
	Node(std::string n, int v);
	~Node();
	Node& AddChild(std::string, int);

private:
	Node * child;
};