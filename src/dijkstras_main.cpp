#include "dijkstras.h"
#include <iostream>

int main() {
    std::string filename = "medium.txt";
    
    Graph G;
    file_to_graph(filename, G);
    
    int source = 0;
    std::vector<int> previous;
    std::vector<int> distances = dijkstra_shortest_path(G, source, previous);
    
    for (int dest = 0; dest < G.numVertices; dest++) {
        std::vector<int> path = extract_shortest_path(distances, previous, dest);
        std::cout << source << " ";
        for (size_t i = 1; i < path.size(); i++) {
            std::cout << path[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Total cost is " << distances[dest] << std::endl;
    }
    
    return 0;
}