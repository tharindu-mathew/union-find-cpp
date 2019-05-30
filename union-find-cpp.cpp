// union-find-cpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <unordered_map>
#include <memory>
#include <cassert>


struct UnionFind {
	struct Node {
		Node* parent;
		int val;
		int rank;
		Node(int val_) : rank(0), val(val_) {
			// this does not work due to using a shared ptr, go figure
			// you have to do it after shared ptr creation, see below ***
			parent = this;
		}
	};

	std::unordered_map<long, std::shared_ptr<Node>> m;

	Node* find(int val) {
		return find(&*m.at(val));
	}

	Node* find(Node* node) {
		if (node->parent == node) {
			return node;
		}

		node->parent = find(node->parent);

		return node->parent;
	}

	void union_dup(int a, int b) {
		Node* pa = find(a);
		Node* pb = find(b);

		if (pa == pb) return;

		if (pa->rank >= pb->rank) {
			pa->rank += pb->rank;
			pb->parent = pa;
		}
		else {
			pb->rank += pa->rank;
			pa->parent = pb;
		}
	}

	void insert(int val) {
		if (m.count(val) > 0) return;
		m[val] = std::make_shared<Node>(Node(val));
		// *** this is an intersting case, when using a shared ptr
		// after object creation the parent has to be explicitly set
		m[val]->parent = m[val].get();
	}

	bool equals(int val1, int val2) {
		return find(val1) == find(val2);
	}
};

int main() {
	UnionFind uf;
	uf.insert(1);
	uf.insert(2);


	//	std::cout << 1 << " ";
	std::cout << uf.find(1)->val << " " << uf.find(2)->val << "\n";


	uf.union_dup(1, 2);

	std::cout << uf.find(1)->val << " " << uf.find(2)->val << "\n";
	assert(uf.equals(1, 2));

	uf.insert(3);
	uf.insert(4);

	std::cout << uf.find(1)->val << " " << uf.find(3)->val << " " << uf.find(4)->val << "\n";

	uf.union_dup(3, 4);

	std::cout << uf.find(1)->val << " " << uf.find(3)->val << " " << uf.find(4)->val << "\n";
	assert(uf.equals(3, 4));
	assert(!uf.equals(1, 4));
	assert(!uf.equals(2, 3));

	uf.union_dup(2, 4);
	std::cout << uf.find(2)->val << " " << uf.find(4)->val << "\n";
	std::cout << uf.find(1)->val << " " << uf.find(2)->val << "\n";
	assert(uf.equals(2, 4));
	assert(uf.equals(1, 3));
	assert(uf.equals(1, 4));

	return 0;

} 
