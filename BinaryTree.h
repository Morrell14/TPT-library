// Binary tree abstract base class
// Created by A. Student
// Modified by:

#ifndef _BINARY_TREE
#define _BINARY_TREE

#include "BinaryNode.h"

class BinaryTree
{
protected:
	BinaryNode* rootPtr;		// ptr to root node
	int count;							// number of nodes in tree

public:
	// "admin" functions
	BinaryTree() { rootPtr = 0; count = 0; }
	BinaryTree(const BinaryTree& tree) { }
	virtual ~BinaryTree() { destroyTree(rootPtr); }

	// common functions for all binary trees
	bool isEmpty() const { return count == 0; }
	int getCount() const { return count; }
	void clear() { destroyTree(rootPtr); rootPtr = 0; count = 0; }
	
	void inOrder(void visit(Book&)) const { _inorder(visit, rootPtr); }
	
	void printTree(void visit(Book&, int)) const { _printTree(visit, rootPtr, 1); }
	void printLeaves(void visit(Book&)) const { _printLeaves(visit, rootPtr); }

	// abstract functions to be implemented by derived class
	virtual bool insert(const Book& newData) = 0;
	
private:
	// delete all nodes from the tree
	void destroyTree(BinaryNode* nodePtr);

	// internal traverse
	void _inorder(void visit(Book&), BinaryNode* nodePtr) const;
	void _printTree(void visit(Book&, int), BinaryNode* nodePtr, int level) const;
	void _printLeaves(void visit(Book&), BinaryNode* nodePtr) const;
};

//Destroy the entire tree
void BinaryTree::destroyTree(BinaryNode* nodePtr)
{
	if (nodePtr) // != NULL
	{
		destroyTree(nodePtr->getLeftPtr());
		destroyTree(nodePtr->getRightPtr());
		delete nodePtr;
	}
}


//Inorder Traversal
void BinaryTree::_inorder(void visit(Book&), BinaryNode* nodePtr) const
{
	if (nodePtr) // != NULL
	{
		Book item = nodePtr->getItem();
		_inorder(visit, nodePtr->getLeftPtr());
		visit(item);
		_inorder(visit, nodePtr->getRightPtr());
	}
}


//Prints tree as an indented list

void BinaryTree::_printTree(void visit(Book&, int), BinaryNode* nodePtr, int level) const
{
	/* Write your code here */
	if (nodePtr) {

		Book item = nodePtr->getItem();
		visit(item, level);
		_printTree(visit, nodePtr->getRightPtr(), level + 1);
		_printTree(visit, nodePtr->getLeftPtr(), level + 1);
	}

}


void BinaryTree::_printLeaves(void visit(Book&), BinaryNode* nodePtr) const {

	if (!nodePtr)
		return;

	if (!nodePtr->getLeftPtr() && !nodePtr->getRightPtr()) {
		Book item = nodePtr->getItem();
		visit(item);
		return;
	}

	if (nodePtr->getLeftPtr())
		_printLeaves(visit, nodePtr->getLeftPtr());
	if (nodePtr->getRightPtr())
		_printLeaves(visit, nodePtr->getRightPtr());

}
#endif

