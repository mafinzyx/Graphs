#pragma once
#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <crtdbg.h>
struct Node {
    int data;
    Node* next;

    Node(int value) : data(value), next(nullptr) {}
    ~Node() {}
};

class Stack {
private:
    Node* top;
public:
    Stack() { top = nullptr; }
    ~Stack();
    void push(int value);
    int pop();
    int topValue() const;
    bool isEmpty() const;
    int size() const;
};

Stack::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

int Stack::size() const {
    int count = 0;
    Node* current = top;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void Stack::push(int value) {
    Node* newNode = new Node(value);
    newNode->next = top;
    top = newNode;
}

int Stack::pop() {
    if (isEmpty()) {
        return 0;
    }
    int value = top->data;
    Node* temp = top;
    top = top->next;
    delete temp;
    return value;
}

int Stack::topValue() const {
    if (isEmpty()) {
        return 0;
    }
    return top->data;
}

bool Stack::isEmpty() const {
    return top == nullptr;
}