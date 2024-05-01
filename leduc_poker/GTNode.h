#ifndef GTNODE_H
#define GTNODE_H

#include <utility>

#include "Node.h"

struct GTNode {
	Node node;
	GTNode* nexts[3];

	GTNode(const int lastActions, const int lastAction, const int passedStreets) {
		Node node(lastActions, lastAction, passedStreets);
		this->node = std::move(node);
	}
};

#endif
