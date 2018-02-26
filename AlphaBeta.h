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
	std::vector<int> piles;
	int value;

	//ctor
	Node(std::string n, int v);
	~Node();
	Node* AddChild(std::string, int, std::vector<int>);

private:
	Node * child;
};

class Tree {
public:
	//ctor
	Tree(Node* n) : root(n) {}
	int size();
	void size(int);
	Node* nodes();
private:
	int nodeCount;
	Node* root;
};

int alphaBeta(Node*, int, int, int, bool);
