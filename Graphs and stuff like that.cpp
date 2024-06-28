#include "Graph.h"

void mySwap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

void quicksort(int* arr, int* extraArr, int left, int right, bool sortByExtraArr) {
    if (left < right) {
        int pivot = arr[(left + right) / 2];
        int pivot_index;
        if (sortByExtraArr) {
            pivot_index = extraArr[(left + right) / 2];
        }
        int i = left - 1, j = right + 1;
        while (true) {
            if (sortByExtraArr) {
                do { i++; } while (arr[i] > pivot || (arr[i] == pivot && extraArr[i] < pivot_index));
                do { j--; } while (arr[j] < pivot || (arr[j] == pivot && extraArr[j] > pivot_index));
            }
            else {
                do { i++; } while (arr[i] > pivot);
                do { j--; } while (arr[j] < pivot);
            }
            if (i >= j) break;
            mySwap(arr[i], arr[j]);
            if (sortByExtraArr) mySwap(extraArr[i], extraArr[j]);
        }
        quicksort(arr, extraArr, left, j, sortByExtraArr);
        quicksort(arr, extraArr, j + 1, right, sortByExtraArr);
    }
}

void readGraph(Graph& graph, int& n) {
    for (int j = 1; j <= n; ++j) {
        int s;
        scanf("%d", &s);

        for (int l = 0; l < s; ++l) { 
            int neighbor;
            scanf("%d", &neighbor);
            graph.addEdge(j, neighbor);
        }
    }
}


int* computeDegrees(const Graph& graph) {
    int* degrees = new int[graph.n];

    for (int i = 1; i <= graph.n; ++i) {
        int count = 0;
        Node* current = graph.adjacency_list[i];
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        degrees[i - 1] = count;
    }

    quicksort(degrees, nullptr, 0, graph.n - 1, false);
    return degrees;
}


void BFS(Graph& graph, int start, bool* visited) {
    Stack s;
    s.push(start);
    visited[start] = true;
    int size = 0;
    int* component_nodes = new int[graph.n];

    while (!s.isEmpty()) {
        int current = s.topValue();
        s.pop();
        size++;
        component_nodes[size - 1] = current;

        Node* neighbor = graph.adjacency_list[current];
        while (neighbor != nullptr) {
            int neighborData = neighbor->data;
            if (!visited[neighborData]) {
                s.push(neighborData);
                visited[neighborData] = true;
            }
            neighbor = neighbor->next;
        }
    }

    for (int i = 0; i < size; ++i) {
        graph.component_sizes[component_nodes[i]] = size;
    }

    delete[] component_nodes;
}

int countComponents(Graph& graph) {
    bool* visited = new bool[graph.n + 1];
    for (int i = 0; i <= graph.n; ++i) {
        visited[i] = false;
    }
    int components = 0;
    for (int i = 1; i <= graph.n; ++i) {
        if (!visited[i]) {
            int count_component;
            BFS(graph, i, visited);
            components++;
        }
    }

    delete[] visited;
    return components;
}


bool isBipartite(const Graph& graph) {
    int* color = new int[graph.n + 1];
    for (int i = 0; i <= graph.n; ++i) {
        color[i] = -1;
    }
    for (int start = 1; start <= graph.n; ++start) {
        if (color[start] == -1) {
            Stack s;
            s.push(start);
            color[start] = 0;
            while (!s.isEmpty()) {
                int v = s.topValue();
                s.pop();
                Node* neighbor = graph.adjacency_list[v];
                while (neighbor != nullptr) {
                    int neighborData = neighbor->data;
                    if (color[neighborData] == -1) {
                        color[neighborData] = color[v] ^ 1;
                        s.push(neighborData);
                    }
                    else if (color[neighborData] == color[v]) {

                        delete[] color;
                        return false;
                    }
                    neighbor = neighbor->next;
                }
            }
        }
    }
    delete[] color;
    return true;
}


void calculateEccentricities(const Graph& g, int* eccentricity, int& maxEccentricity) {
    int* visited = new int[g.n + 1] {};
    int* q = new int[g.n + 1];
    int currentMark = 1;

    maxEccentricity = 0;
    for (int i = 1; i <= g.n; ++i) {
        int componentSize = g.component_sizes[i];
        if (g.component_sizes[i] > 2 && g.adjacency_list[i] != nullptr && visited[i] != currentMark) {
            visited[i] = currentMark;
            int front = 0, rear = -1;
            q[++rear] = i;
            int level = 0;

            int verticesAdded = 0;

            while (front <= rear && verticesAdded < componentSize) {
                int size = rear - front + 1;
                verticesAdded += size;
                eccentricity[i] = level;
                if (verticesAdded >= componentSize) {
                    break;
                }
                while (size--) {
                    int u = q[front++];
                    Node* v = g.adjacency_list[u];
                    while (v != nullptr) {
                        if (visited[v->data] != currentMark) {
                            visited[v->data] = currentMark;
                            q[++rear] = v->data;
                        }
                        v = v->next;
                    }
                }
                level++;
            }
            if (eccentricity[i] > maxEccentricity) {
                maxEccentricity = eccentricity[i];
            }
            currentMark++;
        }
        else if (g.component_sizes[i] < 3 && g.adjacency_list[i] != nullptr) {
            eccentricity[i] = 1;
        }
        else {
            eccentricity[i] = 0;
        }
    }
    delete[] q;
    delete[] visited;
}


void initializeColoring(const Graph& g, int* colors, bool* available) {
    for (int i = 0; i <= g.n; ++i) {
        colors[i] = -1;
        available[i] = true;
    }
}

void colorNode(int u, const Graph& g, int* colors, bool* available) {
    Node* current = g.adjacency_list[u];
    while (current != nullptr) {
        int neighbor = current->data;
        if (colors[neighbor] != -1) {
            available[colors[neighbor]] = false;
        }
        current = current->next;
    }

    int cr;
    for (cr = 1; cr <= g.n; ++cr) {
        if (available[cr]) break;
    }

    colors[u] = cr;

    current = g.adjacency_list[u];
    while (current != nullptr) {
        int neighbor = current->data;
        if (colors[neighbor] != -1) {
            available[colors[neighbor]] = true;
        }
        current = current->next;
    }
}

void greedyColoring(const Graph& g, int* colors) {
    bool* available = new bool[g.n + 1];
    initializeColoring(g, colors, available);
    colors[1] = 1;

    for (int u = 2; u <= g.n; ++u) {
        colorNode(u, g, colors, available);
    }
    delete[] available;
}

void LFColoring(const Graph& g, int* colors) {
    bool* available = new bool[g.n + 1];
    initializeColoring(g, colors, available);

    int* degrees = new int[g.n];
    int* indices = new int[g.n];
    for (int i = 0; i < g.n; ++i) {
        int degree = 0;
        Node* current = g.adjacency_list[i + 1];
        while (current != nullptr) {
            ++degree;
            current = current->next;
        }
        degrees[i] = degree;
        indices[i] = i + 1;
    }

    quicksort(degrees, indices, 0, g.n - 1, true);

    for (int i = 0; i < g.n; ++i) {
        int u = indices[i];
        colorNode(u, g, colors, available);
    }

    delete[] available;
    delete[] degrees;
    delete[] indices;
}


void DSaturColoring(const Graph& g, int* colors) {
    bool* available = new bool[g.n + 1];
    initializeColoring(g, colors, available);

    int* saturations = new int[g.n + 1]();
    int* degrees = new int[g.n + 1];
    bool* processed = new bool[g.n + 1]();

    for (int i = 1; i <= g.n; ++i) {
        Node* current = g.adjacency_list[i];
        int degree = 0;
        while (current != nullptr) {
            ++degree;
            current = current->next;
        }
        degrees[i] = degree;
        if (degree == 0) {
            colors[i] = 1;
            processed[i] = true;
        }
    }

    for (int i = 0; i < g.n; ++i) {
        int max_saturation = -1, max_degree = -1, u = -1;
        for (int j = 1; j <= g.n; ++j) {
            if (!processed[j] && (saturations[j] > max_saturation ||
                (saturations[j] == max_saturation && degrees[j] > max_degree))) {
                max_saturation = saturations[j];
                max_degree = degrees[j];
                u = j;
            }
        }

        colorNode(u, g, colors, available);
        processed[u] = true;

        Node* current = g.adjacency_list[u];
        while (current != nullptr) {
            int v = current->data;
            if (!processed[v]) {
                bool* seen_colors = new bool[g.n + 1]();
                Node* neighbor = g.adjacency_list[v];
                while (neighbor != nullptr) {
                    if (colors[neighbor->data] != -1) {
                        seen_colors[colors[neighbor->data]] = true;
                    }
                    neighbor = neighbor->next;
                }
                int new_saturation = 0;
                for (int k = 0; k <= g.n; ++k) {
                    if (seen_colors[k]) {
                        ++new_saturation;
                    }
                }
                saturations[v] = new_saturation;
                delete[] seen_colors;
            }
            current = current->next;
        }
    }

    delete[] available;
    delete[] saturations;
    delete[] degrees;
    delete[] processed;
}


int countC4Subgraphs(const Graph& g) {
    int total_count = 0;
    for (int u = 1; u <= g.n; ++u) {
        for (int v = u + 1; v <= g.n; ++v) {
            int common_neighbors_count = 0;
            Node* temp1 = g.adjacency_list[u];
            Node* temp2 = g.adjacency_list[v];

            while (temp1) {
                while (temp2) {
                    if (temp1->data == temp2->data) {
                        ++common_neighbors_count;
                    }
                    temp2 = temp2->next;
                }
                temp1 = temp1->next;
                temp2 = g.adjacency_list[v];
            }

            total_count += common_neighbors_count * (common_neighbors_count - 1) / 2;
        }
    }
    return total_count / 2;
}


long long int countComplementEdges(const Graph& graph) {
    long long int totalEdges = (graph.n) * (static_cast<long long>(graph.n) - 1) / 2;
    int existingEdges = 0;

    for (int u = 1; u <= graph.n; ++u) {
        Node* neighbor = graph.adjacency_list[u];
        while (neighbor != nullptr) {
            existingEdges++;
            neighbor = neighbor->next;
        }
    }

    return totalEdges - (existingEdges / 2);
}


void printOutput(int& k, int** all_degrees, int* all_components, long long int* all_count, bool* all_bipartite, int** all_colorsGreedy,
    int** all_LFColoring, int** all_eccentricities, int* all_C4,int** all_SLF, int* num_vertices) {
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < num_vertices[i]; ++j) {
            printf("%d ", all_degrees[i][j]);
        }
        printf("\n");
        printf("%d\n", all_components[i]);
        if (all_bipartite[i]) {
            printf("T\n");
        }
        else {
            printf("F\n");
        }
        for (int j = 1; j <= num_vertices[i]; ++j) {
            printf("%d ", all_eccentricities[i][j]);
        }
        printf("?\n");
        for (int j = 1; j <= num_vertices[i]; ++j) {
            if (all_colorsGreedy[i][j] != -1) printf("%d ", all_colorsGreedy[i][j]);
        }
        printf("\n");
        for (int j = 1; j <= num_vertices[i]; ++j) {
            if (all_LFColoring[i][j] != -1) printf("%d ", all_LFColoring[i][j]);
        }
        printf("\n");
        for (int j = 1; j <= num_vertices[i]; ++j) {
            if (all_SLF[i][j] != -1) printf("%d ", all_SLF[i][j]);
        }
        printf("%d\n",all_C4[i]);
        printf("%lld\n", all_count[i]);
    }
}

void cleanMemory(int& k, int** all_degrees, int* all_components, long long int* all_count, bool* all_bipartite, int** all_colorsGreedy,
    int** all_LFColoring, int** all_eccentricities, int* all_C4,int** all_SLF, int* num_vertices) {
    for (int i = 0; i < k; ++i) {
        delete[] all_degrees[i];
        delete[] all_colorsGreedy[i];
        delete[] all_LFColoring[i];
        delete[] all_eccentricities[i];
        delete[] all_SLF[i];
    }
    delete[] all_eccentricities;
    delete[] all_degrees;
    delete[] all_components;
    delete[] all_count;
    delete[] all_bipartite;
    delete[] all_colorsGreedy;
    delete[] all_LFColoring;
    delete[] all_C4;
    delete[] all_SLF;
    delete[] num_vertices;
}

int main() {
    int k;
    scanf("%d", &k);
    int** all_degrees = new int* [k];
    int* all_components = new int[k];
    long long int* all_count = new long long int[k];
    bool* all_bipartite = new bool[k];
    int** all_colorsGreedy = new int* [k];
    int** all_LFColoring = new int* [k];
    int* num_vertices = new int[k];
    int** all_eccentricities = new int* [k];
    int* all_C4 = new int[k];
    int** all_SLF = new int* [k];

    for (int i = 0; i < k; ++i) {
        int n;
        scanf("%d", &n);
        num_vertices[i] = n;
        Graph graph(n);

        readGraph(graph, n);

        all_degrees[i] = computeDegrees(graph);
        all_components[i] = countComponents(graph);
        all_count[i] = countComplementEdges(graph);
        all_bipartite[i] = isBipartite(graph);

        int* colorsGreedy = new int[n + 1];
        int* colorsLF = new int[n + 1];
        int* colorsSLF = new int[n + 1];
        greedyColoring(graph, colorsGreedy);
        LFColoring(graph, colorsLF);
        all_colorsGreedy[i] = colorsGreedy;
        all_LFColoring[i] = colorsLF;
        
        int* eccentricities = new int[n + 1];
        int maxEccentricity;
        calculateEccentricities(graph, eccentricities, maxEccentricity);
        all_eccentricities[i] = eccentricities;
        DSaturColoring(graph, colorsSLF);
        all_SLF[i] = colorsSLF;
        all_C4[i] = countC4Subgraphs(graph);
    }

    printOutput(k, all_degrees, all_components, all_count, all_bipartite, all_colorsGreedy, all_LFColoring, all_eccentricities, all_C4, all_SLF, num_vertices);

    cleanMemory(k, all_degrees, all_components, all_count, all_bipartite, all_colorsGreedy, all_LFColoring, all_eccentricities, all_C4, all_SLF, num_vertices);

    return 0;
}