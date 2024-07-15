#include "Graph.h"
#include <stdlib.h>
#include <unordered_map>
#include <fstream>
#include <iostream> 
#include <string>
#include <queue> 
#include <sstream> 
#include <unordered_set>
#include <set> 
#include <utility>
#include <tuple> 
#include <algorithm> 
#include <limits> 

Graph::Graph(const char* const & edgelist_csv_fn) {
    edges = 0; 
    ifstream my_file(edgelist_csv_fn);      // open the file
    string line;                     // helper var to store current line
    while(getline(my_file, line)) {  // read one line from the file
        istringstream ss(line);      // create istringstream of current line
        string A, B, weight; // helper vars
        getline(ss, A, ',');     // store first column in "first"
        getline(ss, B, ',');    // store second column in "second"
        getline(ss, weight, '\n');    // store third column column in "third"
        
        if(!adjlist.size())
            root = A; 

        adjlist[A][B] = std::stod(weight); 
        adjlist[B][A] = std::stod(weight); 
        
        ++edges; 
    }
    my_file.close();                 // close file when done
}

unsigned int Graph::num_nodes() {
    // TODO
    return adjlist.size(); 
}

vector<string> Graph::nodes() {
    // Return nodes in ANY order 
    // Note that this is return by value NOT reference
    std::vector<std::string> path; 
    std::unordered_map<std::string, int> visited_nodes; 
    for(auto& it : adjlist){
        if(!visited_nodes.count(it.first)){
            //Perform BFS for each connected component 
            std::queue<std::string> queue; 
            queue.push(it.first);
            while(!queue.empty()){
                std::string curr = queue.front();
                queue.pop();
                path.push_back(curr); 
                visited_nodes[curr] = true; 
                for(auto& edge : adjlist[curr]){
                    if(!visited_nodes.count(edge.first)){
                        visited_nodes[edge.first] = false;
                        queue.push(edge.first); 
                    } 
                }
            }
        }
    }
    return path; 
}

unsigned int Graph::num_edges() {
    // TODO
    return edges; 
}

unsigned int Graph::num_neighbors(string const & node_label) {
    // TODO
    if(adjlist.count(node_label))
        return adjlist[node_label].size(); 
    return 0; 
}

double Graph::edge_weight(string const & u_label, string const & v_label) {
    // TODO
    if(adjlist.count(u_label))
        if(adjlist[u_label].count(v_label))
            return adjlist[u_label][v_label]; 
    return -1; 
}

vector<string> Graph::neighbors(string const & node_label) {
    // TODO
    std::vector<std::string> neighbors; 
    if(adjlist.count(node_label))
        for(auto& it : adjlist[node_label])
            neighbors.push_back(it.first); 
    return neighbors; 
}

//struct to compare pairs 
struct compare_pairs{
    bool operator()(const pair<double, std::string>& A, const pair<double, std::string>& B){
        return A.first < B.first; 
    }
};

vector<string> Graph::shortest_path_unweighted(string const & start_label, string const & end_label) {
    // TODO
    std::set<std::pair<double, std::string>> PQ; 
    std::unordered_map<std::string, std::string> dad; 
    std::unordered_map<std::string, double> distance; 
    std::unordered_map<std::string, bool> visited; 
    PQ.insert(std::make_pair(0, start_label)); 
    dad[start_label] = start_label; 
    distance[start_label] = 0; 
    while(!PQ.empty()){
        std::pair<double, std::string> pair = *PQ.begin(); 
        PQ.erase(PQ.begin()); 
        std::string curr = pair.second; 
        //printf("%s\n", curr.c_str()); 
        visited[curr] = true; 
        if(curr == end_label){
            //debug
            for(auto& it : dad){
                //printf("%s -> %s\n", it.second.c_str(), it.first.c_str());
            }
            //return 
            std::vector<std::string> path; 
            if(start_label == end_label)
                path.push_back(start_label); 
            else{
                std::string curr = end_label;
                do{
                    path.push_back(curr); 
                    curr = dad[curr]; 
                } while(curr != dad[curr]);
                path.push_back(start_label); 
                std::reverse(path.begin(), path.end()); 
            }
            return path; 
        }
        else{
            for(auto& edge : adjlist[curr]){
                std::string neighbor = edge.first; 
                const double weight = 1; 
                //If the neighbor is not touched and not visited 
                if(!visited.count(neighbor)){
                    visited[neighbor] = false; 
                    dad[neighbor] = curr; 
                    const double path_cost = distance[curr] + weight;
                    //printf("(New node) %s -> %s (%f)\n", curr.c_str(), neighbor.c_str(), weight);
                    distance[neighbor] = path_cost; 
                    PQ.insert(std::make_pair(path_cost, neighbor)); 
                }
                //Update if a shorter path is found for a previously seen node
                else if(!visited[neighbor]){
                    const double old_path_cost = distance[neighbor]; 
                    //printf("%s->%s (Old: %f) (Maybe: %f)\n", curr.c_str(), neighbor.c_str(), old_path_cost, distance[curr] + weight); 
                    if(distance[curr] + weight < old_path_cost){
                        //Delete old entry
                        auto old_entry = PQ.find(std::make_pair(old_path_cost, neighbor)); 
                        if(old_entry != PQ.end()){
                            dad[neighbor] = curr; 
                            PQ.erase(old_entry); 
                            PQ.insert(std::make_pair(distance[curr] + weight, neighbor)); 
                        } 
                    }
                }
            }
        }
    }
    //printf("Tough luck :(\n)"); 
    vector<string> tuff;
    return tuff; 
}

vector<tuple<string,string,double>> Graph::shortest_path_weighted(string const & start_label, string const & end_label) {
    // TODO
    std::set<std::pair<double, std::string>> PQ; 
    std::unordered_map<std::string, std::string> dad; 
    std::unordered_map<std::string, double> distance; 
    std::unordered_map<std::string, bool> visited; 
    PQ.insert(std::make_pair(0, start_label)); 
    dad[start_label] = start_label; 
    distance[start_label] = 0; 
    while(!PQ.empty()){
        //Remove closest node from fringe, mark as visited. 
        std::pair<double, std::string> pair = *PQ.begin(); 
        PQ.erase(PQ.begin()); 
        std::string curr = pair.second; 
        //printf("%s\n", curr.c_str()); 
        visited[curr] = true; 
        //Break if we have found our destination node 
        if(curr == end_label){
            //debug
            for(auto& it : dad){
                //printf("%s -> %s\n", it.second.c_str(), it.first.c_str());
            }
            //return 
            std::vector<std::tuple<std::string, std::string, double>> path; 
            if(start_label == end_label)
                path.push_back(std::make_tuple(start_label, start_label, -1)); 
            else{
                std::string curr = end_label;
                do{
                    std::string parent = dad[curr]; 
                    const double weight = adjlist[parent][curr]; 
                    path.push_back(std::make_tuple(parent, curr, weight)); 
                    curr = dad[curr]; 
                } while(curr != dad[curr]);
                std::reverse(path.begin(), path.end()); 
            }
            return path; 
        }
        else{
            for(auto& edge : adjlist[curr]){
                std::string neighbor = edge.first; 
                const double weight = edge.second; 
                //If the neighbor is not touched and not visited 
                if(!visited.count(neighbor)){
                    visited[neighbor] = false; 
                    dad[neighbor] = curr; 
                    const double path_cost = distance[curr] + weight;
                    //printf("(New node) %s -> %s (%f)\n", curr.c_str(), neighbor.c_str(), weight);
                    distance[neighbor] = path_cost; 
                    PQ.insert(std::make_pair(path_cost, neighbor)); 
                }
                //Update if a shorter path is found for a previously seen node
                else if(!visited[neighbor]){
                    const double old_path_cost = distance[neighbor]; 
                    //printf("%s->%s (Old: %f) (Maybe: %f)\n", curr.c_str(), neighbor.c_str(), old_path_cost, distance[curr] + weight); 
                    if(distance[curr] + weight < old_path_cost){
                        //Delete old entry
                        auto old_entry = PQ.find(std::make_pair(old_path_cost, neighbor)); 
                        if(old_entry != PQ.end()){
                            dad[neighbor] = curr; 
                            distance[neighbor] = distance[curr] + weight; 
                            PQ.erase(old_entry); 
                            PQ.insert(std::make_pair(distance[curr] + weight, neighbor)); 
                        } 
                    }
                }
            }
        }
    }
    //printf("Tough luck :(\n)"); 
    vector<tuple<string,string,double>> tuff;
    return tuff; 
}

vector<vector<string>> Graph::connected_components(double const & threshold) {
    // TODO
    std::vector<std::vector<std::string>> result; 
    std::unordered_map<std::string, bool> visited_nodes; 
    for(auto& it : adjlist){
        if(!visited_nodes.count(it.first)){
            std::vector<std::string> connected_component; 
            //Perform BFS for each connected component 
            std::queue<std::string> queue; 
            queue.push(it.first);
            while(!queue.empty()){
                std::string curr = queue.front();
                queue.pop();
                connected_component.push_back(curr); 
                visited_nodes[curr] = true; 
                for(auto& edge : adjlist[curr]){
                    if(!visited_nodes.count(edge.first)){
                        if(edge.second <= threshold){
                            visited_nodes[edge.first] = false;
                            queue.push(edge.first); 
                        }
                    } 
                }
            }
            result.push_back(connected_component); 
        }
    }
    return result; 
}

double Graph::smallest_connecting_threshold(string const & start_label, string const & end_label) {
    if(start_label == end_label)
        return 0;  
    std::unordered_set<std::string> MST;
    std::set<std::pair<double, std::string>> PQ; 
    std::unordered_map<std::string, double> weights; 
    std::unordered_map<std::string, bool> visited; 
    std::unordered_map<std::string, std::string> dad; 
    weights[start_label] = -1; 
    PQ.insert(std::make_pair(-1, start_label)); 
    bool path_exists = false; 
    //printf("MST\n"); 
    while(!PQ.empty()){
        std::pair<double, std::string> pair = *PQ.begin(); 
        PQ.erase(PQ.begin());
        std::string node = pair.second;
        visited[node] = true; 
        MST.insert(node); 
        //printf("%s, %f\n", node.c_str(), pair.first); 
        if(node == end_label)
            path_exists = true; 
        for(auto& edge : adjlist[node]){
            std::string neighbor = edge.first; 
            const double weight = edge.second; 
            if(!visited.count(neighbor)){
                PQ.insert(std::make_pair(weight, neighbor)); 
                weights[neighbor] = weight; 
                dad[neighbor] = node; 
                visited[neighbor] = false; 
            }
            else if(!visited[neighbor]){
                double old_weight = weights[neighbor]; 
                if(weight < old_weight){
                    auto old_entry = PQ.find(std::make_pair(old_weight, neighbor)); 
                    if(old_entry != PQ.end()){
                        PQ.erase(old_entry); 
                        weights[neighbor] = weight; 
                        dad[neighbor] = node; 
                        PQ.insert(std::make_pair(weight, neighbor)); 
                    }
                }
            }
        }
    }
    if(path_exists){
        double max_weight = 0; 
        std::string curr = end_label; 
        do{
            std::string parent = dad[curr]; 
            double weight = adjlist[parent][curr]; 
            if(weight > max_weight)
                max_weight = weight; 
            curr = parent; 
        } while(curr != start_label); 
        return max_weight; 
    }
    else return -1; 
    
}
