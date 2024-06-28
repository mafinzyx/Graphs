#pragma once
#include "Stack.h"
struct Graph {
    int n;
    Node** adjacency_list;
    int* component_sizes;

    Graph(int n) : n(n) {
        adjacency_list = new Node * [n + 1];
        component_sizes = new int[n + 1];
        for (int i = 0; i <= n; ++i) {
            adjacency_list[i] = nullptr;
        }
    }

    ~Graph() {
        for (int i = 0; i <= n; ++i) {
            deleteList(adjacency_list[i]);
            adjacency_list[i] = nullptr;
        }
        delete[] adjacency_list;
        delete[] component_sizes;

    }

    void addEdge(int u, int v) {
        Node* newNode = new Node(v);
        newNode->next = adjacency_list[u];
        adjacency_list[u] = newNode;
    }

    static void deleteList(Node* head) {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};