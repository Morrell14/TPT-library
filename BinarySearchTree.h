// Binary Search Tree ADT
// Created by A. Student
// Modified by:  

#ifndef _BINARY_SEARCH_TREE
#define _BINARY_SEARCH_TREE

#include "BinaryTree.h"


class BinarySearchTree : public BinaryTree
{
public:
    // insert a node at the correct location
    bool insert(const Book& item);
    // remove a node if found
    //bool remove(const ItemType &item);
    // find a target node


private:
    // internal insert node: insert newNode in nodePtr subtree
    BinaryNode* _insert(BinaryNode* nodePtr, BinaryNode* newNode);

    
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

    else if (nodePtr->getItem().getTitle() > newNodePtr->getItem().getTitle()) // item for nodePtr > item for newNodePtr, insert newNodePtr in the left subtree
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

#endif
