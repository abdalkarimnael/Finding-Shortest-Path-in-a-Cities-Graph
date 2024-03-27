/////////---->>> Name: Abdalkarim Eiss ----- ID: 1200015 ---- P4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_SIZE 100
#define Inf 9999

/// List
typedef struct list* ptr;
struct list {
    int index;
    int distance;
    ptr next;
};
typedef ptr List;

/// City
struct city {
    char name[100];
};
typedef struct city* City;

/// Graph
struct allGraph {
    int numOfCities;
    City city[MAX_SIZE];
    List adjList[MAX_SIZE];
};
typedef struct allGraph* Graph;

/// Function to print all cities
void printAllCities(const char* fileName) {
    FILE* input = fopen(fileName, "r");
    printf("All cities:\n\n");
    char line[1000];
    while (fgets(line, sizeof(line), input) != NULL) {
        char source[100];
        char destination[100];
        int distance;
        if (sscanf(line, "%s\t%s\t%d", source, destination, &distance) == 3) {
            printf("%s -> %s ( %d Km )\n\n", source, destination, distance);
        }
    }
    printf("\n");
    fclose(input);
}
/*void printAllCities(Graph graph) {
    printf("List of cities:\n");
    for (int i = 0; i < graph->numOfCities; i++) {
        printf("%d. %s\n", i + 1, graph->city[i]->name);
    }
}*/

/// Function to construct graph
Graph constructGraph() {
    Graph graph = (Graph)malloc(sizeof(struct allGraph));
    graph->numOfCities = 0;
    for (int i = 0; i < MAX_SIZE; i++) {
        graph->adjList[i] = NULL;
    }
    return graph;
}

/// Function to insert a city into the graph
void insertCity(Graph graph, char* nameOfCity) {
    City newCity = (City)malloc(sizeof(struct city));
    strcpy(newCity->name, nameOfCity);
    graph->city[graph->numOfCities] = newCity;
    graph->numOfCities++;
}

/// Function to search for the index of a city in the graph
int getTheIndexOfCity(Graph graph, char* nameOfCity) {
    for (int i = 0; i < graph->numOfCities; i++) {
        if (strcmp(graph->city[i]->name, nameOfCity) == 0) {
            return i;
        }
    }
    return -1;
}

/// Function to add an edge between two cities to make directed graph
void makeAnEdge(Graph graph, int src, int dest, int distance) {
    List newEdge = (List)malloc(sizeof(struct list));
    newEdge->index = dest;
    newEdge->distance = distance; /// Assign the distance
    newEdge->next = graph->adjList[src];
    graph->adjList[src] = newEdge;

//    newEdge = (List)malloc(sizeof(struct list));
//    newEdge->index = src;
//    newEdge->distance = distance; /// Assign the distance
//    newEdge->next = graph->adjList[dest];
//    graph->adjList[dest] = newEdge;
}
/*void makeAnEdge(Graph graph, const char* source, const char* destination, int distance) {
    // Get the indices of the source and destination cities
    int sourceIndex = getTheIndexOfCity(graph, source);
    int destIndex = getTheIndexOfCity(graph, destination);

    // Create a new node for the adjacency list
    List newNode = malloc(sizeof(List));
    newNode->index = destIndex;
    newNode->distance = distance;
    newNode->next = NULL;

    // Find the correct position to insert the new node in the adjacency list
    List current = graph->adjList[sourceIndex];
    List prev = NULL;

    while (current != NULL && current->index < destIndex) {
        prev = current;
        current = current->next;
    }

    // Insert the new node in the adjacency list
    if (prev == NULL) {
        // Insert at the beginning
        newNode->next = graph->adjList[sourceIndex];
        graph->adjList[sourceIndex] = newNode;
    } else {
        // Insert in the middle or at the end
        prev->next = newNode;
        newNode->next = current;
    }
}*/
/// Function to connect two cities in the graph
void citiesConnection(Graph graph, char* firstCity, char* secondCity, int distance) {
    int indexOfFirstCity = getTheIndexOfCity(graph, firstCity);
    int indexOfSecondCity = getTheIndexOfCity(graph, secondCity);

    if (indexOfFirstCity != -1 && indexOfSecondCity != -1) {
        makeAnEdge(graph, indexOfFirstCity, indexOfSecondCity, distance);
    }
}

////BFS function
void bfs(Graph graph, char* source, char* destination) {
    int visited[MAX_SIZE] = {0};
    int indexOfFirstCity = getTheIndexOfCity(graph, source);
    int indexOfSecondCity = getTheIndexOfCity(graph, destination);

    /// Create a queue for BFS
    int queue[MAX_SIZE];
    int front = 0, rear = 0;

    visited[indexOfFirstCity] = 1;
    queue[rear++] = indexOfFirstCity;

    /// Store the previous vertex and distance for each vertex
    int previous[MAX_SIZE];
    int distance[MAX_SIZE];
    memset(previous, -1, sizeof(previous));
    memset(distance, 0, sizeof(distance));

    while (front != rear) {
        /// Dequeue a vertex from the queue
        int vertex = queue[front++];

        /// Check if the destination is reached
        if (vertex == indexOfSecondCity) {
            break;
        }

        /// Get all adjacent vertices of the dequeued vertex
        List adjlist = graph->adjList[vertex];

        while (adjlist != NULL) {
            int adjIndex = adjlist->index;

            if (!visited[adjIndex]) {
                visited[adjIndex] = 1;
                queue[rear++] = adjIndex;
                previous[adjIndex] = vertex;
                distance[adjIndex] = distance[vertex] + adjlist->distance;
            }

            adjlist = adjlist->next;
        }
    }

    /// Print the full route from source to destination
    int current = indexOfSecondCity;
    if (previous[current] == -1) {
        printf("No path found from %s to %s\n", source, destination);
    } else {
        printf("Shortest path using BFS algorithm from %s to %s:\n", source, destination);

        /// Store the cities in a stack to reverse the order
        int stack[MAX_SIZE];
        int top = 0;

        while (current != -1) {
            stack[top++] = current;
            current = previous[current];
        }

        /// Print the cities in the correct order
        for (int i = top - 1; i >= 0; i--) {
            printf("%s ", graph->city[stack[i]]->name);
            if (i > 0) {
                printf("-> ");
            }
        }

        printf("\n");

        /// To print the total distance
        printf("Total Distance: %d\n", distance[indexOfSecondCity]);
    }
}
//////////////
////// Dijkstra's algorithm
void dijkstra(Graph graph, int source, int destination) {
    /// Initialize the distance array
    int* distance = malloc(graph->numOfCities * sizeof(int));
    for (int i = 0; i < graph->numOfCities; i++) {
        distance[i] = Inf;  /// infinity
    }

    /// Initialize the seen set
    bool* seen = malloc(graph->numOfCities * sizeof(bool));
    for (int i = 0; i < graph->numOfCities; i++) {
        seen[i] = false;
    }

    /// Set the distance of the source to 0
    distance[source] = 0;

    /// Create a min heap
    struct node {
        int index;
        int weight;
        struct node* next;
    };

    /// Add the source to the min heap
    struct node* minHeap = malloc(sizeof(struct node));
    minHeap->index = source;
    minHeap->weight = 0;
    minHeap->next = NULL;

    /// Initialize the previous array
    int previous[MAX_SIZE];
    memset(previous, -1, sizeof(previous));

    /// Iterate until the min heap is empty
    while (minHeap != NULL) {
        /// Remove the minimum element from the min heap
        struct node* current = minHeap;
        minHeap = current->next;

        /// Mark the current index as seen
        seen[current->index] = true;

        /// If the destination is reached, break the loop
        if (current->index == destination) {
            break;
        }

        /// Iterate through all outgoing edges from the current index
        struct node* temp = graph->adjList[current->index];
        while (temp != NULL) {
            /// If the adjacent index is not seen and the distance to the adjacent index is less than the current distance
            if (!seen[temp->index] && distance[temp->index] > distance[current->index] + temp->weight) {
                /// Update the distance to the adjacent index
                distance[temp->index] = distance[current->index] + temp->weight;

                /// Update the previous vertex
                previous[temp->index] = current->index;

                /// Add the adjacent index to the min heap
                struct node* newNode = malloc(sizeof(struct node));
                newNode->index = temp->index;
                newNode->weight = distance[temp->index];
                newNode->next = minHeap;
                minHeap = newNode;
            }

            temp = temp->next;
        }
    }

    /// Check if a path exists from source to destination
    if (previous[destination] == -1) {
        printf("No path found from %s to %s\n", graph->city[source]->name, graph->city[destination]->name);
    } else {
        /// Print the shortest path from source to destination
        printf("Shortest path using Dijkstra Algorithm from %s to %s: ", graph->city[source]->name, graph->city[destination]->name);

        /// Store the cities in a stack to reverse the order
        int stack[MAX_SIZE];
        int top = 0;
        int current = destination;

        while (current != -1) {
            stack[top++] = current;
            current = previous[current];
        }

        //// Print the cities in the correct order
        for (int i = top - 1; i >= 0; i--) {
            printf("%s", graph->city[stack[i]]->name);
            if (i > 0) {
                printf(" -> ");
            }
        }

        //// Calculate the total distance of the shortest path
        int totalDistance = distance[destination];

        printf("\nTotal Distance by using Dijkstra Algorithm: %d\n", totalDistance);
    }

    //// Free the memory
    free(distance);
    free(seen);
}

////Save BFS function info to file
void bfsToFile(Graph graph, char* source, char* destination, FILE* out) {
    int visited[MAX_SIZE] = {0};
    int indexOfFirstCity = getTheIndexOfCity(graph, source);
    int indexOfSecondCity = getTheIndexOfCity(graph, destination);

    /// Create a queue for BFS
    int queue[MAX_SIZE];
    int front = 0, rear = 0;

    visited[indexOfFirstCity] = 1;
    queue[rear++] = indexOfFirstCity;

    /// Store the previous vertex and distance for each vertex
    int previous[MAX_SIZE];
    int distance[MAX_SIZE];
    memset(previous, -1, sizeof(previous));
    memset(distance, 0, sizeof(distance));

    while (front != rear) {
        /// Dequeue a vertex from the queue
        int vertex = queue[front++];

        /// Check if the destination is reached
        if (vertex == indexOfSecondCity) {
            break;
        }

        /// Get all adjacent vertices of the dequeued vertex
        List adjlist = graph->adjList[vertex];

        while (adjlist != NULL) {
            int adjIndex = adjlist->index;

            if (!visited[adjIndex]) {
                visited[adjIndex] = 1;
                queue[rear++] = adjIndex;
                previous[adjIndex] = vertex;
                distance[adjIndex] = distance[vertex] + adjlist->distance;
            }

            adjlist = adjlist->next;
        }
    }

    /// Print the full route from source to destination
    int current = indexOfSecondCity;
    if (previous[current] == -1) {
        fprintf(out,"No path found from %s to %s\n", source, destination);
    } else {
        fprintf(out,"Shortest path using BFS algorithm from %s to %s:\n", source, destination);

        /// Store the cities in a stack to reverse the order
        int stack[MAX_SIZE];
        int top = 0;

        while (current != -1) {
            stack[top++] = current;
            current = previous[current];
        }

        /// Print the cities in the correct order
        for (int i = top - 1; i >= 0; i--) {
            fprintf(out,"%s ", graph->city[stack[i]]->name);
            if (i > 0) {
                fprintf(out,"-> ");
            }
        }

        fprintf(out,"\n");

        /// To print the total distance
        fprintf(out,"Total Distance: %d\n", distance[indexOfSecondCity]);
    }
}
//////////////
////// Save Dijkstra's algorithm info to output file
void dijkstraToFile(Graph graph, int source, int destination, FILE* out) {
    /// Initialize the distance array
    int* distance = malloc(graph->numOfCities * sizeof(int));
    for (int i = 0; i < graph->numOfCities; i++) {
        distance[i] = Inf;  // infinity
    }

    /// Initialize the seen set
    bool* seen = malloc(graph->numOfCities * sizeof(bool));
    for (int i = 0; i < graph->numOfCities; i++) {
        seen[i] = false;
    }

    /// Set the distance of the source to 0
    distance[source] = 0;

    /// Create a min heap
    struct node {
        int index;
        int weight;
        struct node* next;
    };

    /// Add the source to the min heap
    struct node* minHeap = malloc(sizeof(struct node));
    minHeap->index = source;
    minHeap->weight = 0;
    minHeap->next = NULL;

    /// Initialize the previous array
    int previous[MAX_SIZE];
    memset(previous, -1, sizeof(previous));

    /// Iterate until the min heap is empty
    while (minHeap != NULL) {
        /// Remove the minimum element from the min heap
        struct node* current = minHeap;
        minHeap = current->next;

        /// Mark the current index as seen
        seen[current->index] = true;

        /// If the destination is reached, break the loop
        if (current->index == destination) {
            break;
        }

        /// Iterate through all outgoing edges from the current index
        struct node* temp = graph->adjList[current->index];
        while (temp != NULL) {
            /// If the adjacent index is not seen and the distance to the adjacent index is less than the current distance
            if (!seen[temp->index] && distance[temp->index] > distance[current->index] + temp->weight) {
                // Update the distance to the adjacent index
                distance[temp->index] = distance[current->index] + temp->weight;

                // Update the previous vertex
                previous[temp->index] = current->index;

                // Add the adjacent index to the min heap
                struct node* newNode = malloc(sizeof(struct node));
                newNode->index = temp->index;
                newNode->weight = distance[temp->index];
                newNode->next = minHeap;
                minHeap = newNode;
            }

            temp = temp->next;
        }
    }

    /// Check if a path exists from source to destination
    if (previous[destination] == -1) {
        fprintf(out,"No path found from %s to %s\n", graph->city[source]->name, graph->city[destination]->name);
    } else {
        /// Print the shortest path from source to destination
        fprintf(out,"Shortest path using Dijkstra Algorithm from %s to %s: ", graph->city[source]->name, graph->city[destination]->name);

        /// Store the cities in a stack to reverse the order
        int stack[MAX_SIZE];
        int top = 0;
        int current = destination;

        while (current != -1) {
            stack[top++] = current;
            current = previous[current];
        }

        //// Print the cities in the correct order
        for (int i = top - 1; i >= 0; i--) {
            fprintf(out,"%s", graph->city[stack[i]]->name);
            if (i > 0) {
                fprintf(out," -> ");
            }
        }

        //// Calculate the total distance of the shortest path
        int totalDistance = distance[destination];

        fprintf(out,"\nTotal Distance by using Dijkstra Algorithm: %d\n", totalDistance);
    }

    //// Free the memory
    free(distance);
    free(seen);
}

/////////// Main function
int main() {
    /// Header for the system
    printf("\t\t---------------------------\n");
    printf("\t\t\b\bWELCOME TO Cities Shortest Distances SYSTEM\n");
    printf("\t\t---------------------------\n");
    printf("\t\t\t\t\t\tCreated by: A.N.A\n\n");

    //// Menu
    int q = 0;
    int choice;
    char source[100];
    char destination[100];

    /// Construct the Graph
    Graph graph = constructGraph();
    /// Menu
    while (q != 1) {
        printf("What do you need from the menu?\n\t");
        printf("1. Read all cities information from the input fileand construct the graph.\n\t2. Enter the source city.\n\t3. Enter the destination city and then"
               " print the full route of the shortest path including"
               " the distance between each two cities and the total shortest cost for "
               "both algorithms (Dijkstra and BFS).\n\t");
        printf("4. Exit and save all information in step 3 in shortest_distance.txt file.\n\tEnter:");
        scanf("%d", &choice);
        FILE* input;
        FILE* output;
        char line[1000];
        switch (choice) {
            /// To read and load the data from input file
            case 1:
                input = fopen("Cities.txt", "r");
                if(input == NULL){
                    printf("\nFile doesn't exist!!\n");
                }
                while (fgets(line, sizeof(line), input) != NULL) {
                    char source[100];
                    char destination[100];
                    int distance;
                    if (sscanf(line, "%s\t%s\t%d", source, destination, &distance) == 3) {
                        insertCity(graph, source); ///to insert the city
                        insertCity(graph, destination);
                        citiesConnection(graph, source, destination,distance);
                    }
                }
                fclose(input);
                /// To display all data
                printAllCities("Cities.txt");
                break;
            /// To get the source city from user
            case 2:
                printf("Enter the source city: ");
                scanf("%s", source);
                break;

            /// To get the destination city from user
            case 3:
                printf("Enter the destination city: ");
                scanf("%s", destination);
                /// Call BFS function to find the path and distance from source to destination cities
                bfs(graph, source, destination);
                int SourceIndex= getTheIndexOfCity(graph,source);
                int DestIndex= getTheIndexOfCity(graph,destination);
                /// Call Dijkstra function to find the shortest path and distance from source to destination cities
                dijkstra(graph,SourceIndex,DestIndex);
                printf("\n");
                break;
            //// Function to save all operations from case 3 in shortest_distance.txt file
            case 4:
                /// Save information in shortest_distance.txt file
                output = fopen("shortest_distance.txt", "w");
                if(output == NULL){
                    printf("\nFile doesn't exist!!\n");
                }
                bfsToFile(graph,source,destination,output);
                dijkstraToFile(graph,SourceIndex,DestIndex,output);
                fprintf(output,"\n---------------------------------------\n");
                printf("\nSave to file done !\n");
                /// Perform any necessary operations before exiting
                q = 1; // Set q to 1 to exit the while loop
                break;

            default:
                printf("\nThis choice isn't available !!\n");
                break;
        }
    }

    return 0;
}