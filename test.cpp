#include "Graph.h"
#include <stdlib.h>
#include <vector>
#include <string> 
#include <iostream> 
#include <tuple> 
using namespace std; 
int main(){
    Graph graph("example/test1.csv"); 
    std::vector<std::string> bfs_path = graph.nodes(); 
    printf("Size: %d\n", bfs_path.size()); 
    for(int i = 0; i < bfs_path.size(); i++)
        std::cout << bfs_path[i] << std::endl; 
    //Neighbor test
    printf("\nNeighbors:\n"); 
    std::vector<std::string> neighbors = graph.neighbors("1"); 
    for(int i = 0; i < neighbors.size(); i++)
        std::cout << neighbors[i] << std::endl; 
    //Shortest path test
    vector<tuple<string,string,double>> shortest_path = graph.shortest_path_weighted("A", "C"); 
        printf("Shortest Path Test:\n"); 

    for(auto& edge : shortest_path){
        std::cout << "(" << std::get<0>(edge) << ", " << std::get<1>(edge) << ", " << std::get<2>(edge) << ")" << std::endl; 
    }
    printf("\n"); 

    printf("Unweighted Shortest Path Test:\n"); 
    vector<string> unweighted_shortest_path = graph.shortest_path_unweighted("A", "C"); 
    for(std::string node : unweighted_shortest_path){
        printf("%s ", node.c_str()); 
    }
    printf("\n"); 

    printf("\nMST Test:\n");
    double smallest_threshold = graph.smallest_connecting_threshold("A", "B"); 
    printf("Value: %f\n", smallest_threshold); 
}
