// Binary Search Tree ADT
// Created by Morrell Nioble
// Modified by:  

#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryTree.h"
#include "Book.h"
#include "BinaryNode.h"


class BinarySearchTree : public BinaryTree
{
public:
    // insert a node at the correct location
    bool insert(const Book& item);
    // remove a node if found
    bool remove(const Book &item);
    


private:
    // internal insert node: insert newNode in nodePtr subtree
    BinaryNode* _insert(BinaryNode* nodePtr, BinaryNode* newNode);

    // internal remove node: locate and delete target node under nodePtr subtree
    BinaryNode* _remove(BinaryNode* nodePtr, const Book target, bool& success);

    // delete target node from tree, called by internal remove node
    BinaryNode* deleteNode(BinaryNode* targetNodePtr);

    // remove the leftmost node in the left subtree of nodePtr
    BinaryNode* removeLeftmostNode(BinaryNode* nodePtr, Book& successor);
    
};


///////////////////////// public function definitions ///////////////////////////
//Wrapper for _insert - Inserting items within a tree

bool BinarySearchTree::insert(const Book& newEntry)
{
    BinaryNode* newNodePtr = new BinaryNode(newEntry);
    this->rootPtr = _insert(this->rootPtr, newNodePtr);
    return true;
}


//////////////////////////// private functions ////////////////////////////////////////////

//Implementation of the insert operation

BinaryNode* BinarySearchTree::_insert(BinaryNode* nodePtr, BinaryNode* newNodePtr)
{
    if (!nodePtr) // input node is null, return the newNodePtr
        return newNodePtr;

    else if (nodePtr->getItem().getISBN() > newNodePtr->getItem().getISBN()) // item for nodePtr > item for newNodePtr, insert newNodePtr in the left subtree
    {
        // call _insert with left child of nodePtr and newNodePtr
        // and set left child for nodePtr to node returned by the _insert method
        nodePtr->setLeftPtr(_insert(nodePtr->getLeftPtr(), newNodePtr));
        return nodePtr; // return the updated nodePtr
    }
    else // else insert newNodePtr in the right subtree
    {
        // call _insert with right child of nodePtr and newNodePtr
        // and set right child for nodePtr to node returned by the _insert method
        nodePtr->setRightPtr(_insert(nodePtr->getRightPtr(), newNodePtr));
        return nodePtr; // return the updated nodePtr
    }
}

bool BinarySearchTree::remove(const Book& target)
{
    bool isSuccessful = false;
    rootPtr = _remove(rootPtr, target, isSuccessful);
    return isSuccessful;
}
 
BinaryNode* BinarySearchTree::_remove(BinaryNode* nodePtr,
	const Book target, bool& success)

{
	if (nodePtr == 0)
	{
		success = false;
		return 0;
	}
	if (nodePtr->getItem().getISBN() > target.getISBN())
		nodePtr->setLeftPtr(_remove(nodePtr->getLeftPtr(), target, success));
	else if (nodePtr->getItem().getISBN() < target.getISBN())
		nodePtr->setRightPtr(_remove(nodePtr->getRightPtr(), target, success));
	else
	{
		nodePtr = deleteNode(nodePtr);
		success = true;
	}
	return nodePtr;
}

BinaryNode* BinarySearchTree::deleteNode(BinaryNode* nodePtr)
{
	if (nodePtr->isLeaf())
	{
		delete nodePtr;
		nodePtr = 0;
		return nodePtr;
	}
	else if (nodePtr->getLeftPtr() == 0)
	{
		BinaryNode* nodeToConnectPtr = nodePtr->getRightPtr();
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;
	}
	else if (nodePtr->getRightPtr() == 0)
	{
		BinaryNode* nodeToConnectPtr = nodePtr->getLeftPtr();
		delete nodePtr;
		nodePtr = 0;
		return nodeToConnectPtr;
	}
	else
	{
		Book newNodeValue;
		nodePtr->setRightPtr(removeLeftmostNode(nodePtr->getRightPtr(), newNodeValue));
		nodePtr->setItem(newNodeValue);
		return nodePtr;
	}
}

BinaryNode* BinarySearchTree::removeLeftmostNode(BinaryNode* nodePtr,
	Book& successor)
{
	if (nodePtr->getLeftPtr() == 0)
	{
		successor = nodePtr->getItem();
		return deleteNode(nodePtr);
	}
	else
	{
		nodePtr->setLeftPtr(removeLeftmostNode(nodePtr->getLeftPtr(), successor));
		return nodePtr;
	}
}


#endif
