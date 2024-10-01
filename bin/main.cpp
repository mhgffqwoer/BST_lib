#include "BST.hpp"
#include <set>
#include <iostream>
#include <iterator>

int main() {
	BST<int> bst1 = {4, 2, 6, 1, 3, 5, 7};

	//bst1.erase(0);

	for (auto it = bst1.cbegin(); it != bst1.cend(); ++it) {
		std::cout << *it << " ";
	}
	std::cout << "\n";

	for (auto it = bst1.cbegin(Preorder()); it != bst1.cend(Preorder()); ++it) {
		std::cout << *it << " ";
	}
	std::cout << "\n";
	
	for (auto it = bst1.cbegin(Postorder()); it != bst1.cend(Postorder()); ++it) {
		std::cout << *it << " ";
	}
	std::cout << "\n";

	return 0;
}