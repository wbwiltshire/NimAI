
#include "AlphaBeta.h"

//ctor
Node::Node(std::string n, int v) : name(n), value(v) {
	std::cout << " - Creatinging node: " << name;
}
//dtor
Node::~Node() {
	std::cout << "\tDestroying node: " << name << std::endl;
	for (Node* c : children) {
		delete c;
	}
}
Node* Node::AddChild(std::string n, int v, std::vector<int> p) {
	child = new Node(n, v);
	child->piles = p;
	children.push_back(child);
	return child;
}

int Tree::size() {
	return nodeCount;
}
void Tree::size(int s) {
	nodeCount = s;
}
Node* Tree::nodes() {
	return root;
}

// AlphaBeta Pruning function
int alphaBeta(Node* node, int depth, int alpha, int beta, bool isMax) {
	int bestValue = 0;
	int childValue = 0;
	std::vector<Node*> temp;

	if ((depth == 0) || (node->children.size() == 0)) {
		bestValue = node->value;
	}
	else if (isMax) {
		bestValue = alpha;

		// Recurse for all children of node.
		for (Node* n : node->children) {
			childValue = alphaBeta(n, depth - 1, bestValue, beta, false);
			bestValue = std::max(bestValue, childValue);
			if (beta <= bestValue) {
				std::cout << "\tcutoff: " << n->name << std::endl;
				break;
			}
		}
	}
	else {
		bestValue = beta;

		// Recurse for all children of node.
		for (Node* n : node->children) {
			childValue = alphaBeta(n, depth - 1, alpha, bestValue, true);
			bestValue = std::min(bestValue, childValue);
			if (bestValue <= alpha) {
				std::cout << "\tcutoff" << n->name << std::endl;
				break;
			}
		}
	}
	return bestValue;
}
