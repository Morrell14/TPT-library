#pragma once
/**~*~*~*~*
   Stack template
*~*/
#ifndef STACK_ADT
#define STACK_ADT

template <class T>
class Stack
{
private:
    // Structure for the stack nodes
    struct StackNode
    {
        T value;          // Value in the node
        StackNode* next;  // Pointer to next node
    };

    StackNode* top;     // Pointer to the stack top
    int length;

public:
    // Constructor
    Stack() { top = NULL; length = 0; }
    // Destructor
    ~Stack();

    // Stack operations:
    void push(T);
    T pop();
    T peek() { StackNode* node = top; return node->value; }
    bool isEmpty() { if (length > 0) return false; else return true; }
    int getLength() { return length; }
};

/**~*~*~*~*
  Member function push inserts the argument onto
  the stack.
*~**/
template <class T>
void Stack<T>::push(T item)
{
    StackNode* newNode; // Pointer to a new node

    // Allocate a new node and store num there.
    newNode = new StackNode;
    //cout << "in push" << endl;
    if (!newNode)
        return;
    newNode->value = item;

    if (isEmpty())
    {
        top = newNode;
        newNode->next = NULL;
        length++;
    }
    else
    {
        // Update links and counter
        newNode->next = top;
        top = newNode;
        length++;
    }
}



/**~*~*~*~*
  Member function pop deletes the value at the top
  of the stack and returns it.
  Assume stack is not empty.
*~**/
template <class T>
T Stack<T>::pop()
{
    //cout << "in pop" << endl;
    StackNode* node = top->next;
    T val = top->value;
    delete top;
    top = node;
    length--;
    return val;
}




/**~*~*~*~*
  Destructor:
  Traverses the list deleting each node (without calling pop)
*~**/
template <class T>
Stack<T>::~Stack()
{
    StackNode* currNode;
    StackNode* nextNode;
    //cout << "in destructor" << endl;
    // Position nodePtr at the top of the stack.
    currNode = top;

    // Traverse the list deleting each node.
    while (currNode)
    {
        //cout << "in destructor loop" << endl;
        nextNode = currNode->next;
        delete currNode;
        currNode = nextNode;
    }
}

#endif
