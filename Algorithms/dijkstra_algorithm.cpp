/*
 * =====================================================================================
 *
 * Filename:  dijkstra_algorithm.cpp
 * 
 * Author:  Hagemaruwu
 *
 * Description:  An implementation of Dijkstra's Shortest Path Algorithm using an
 * adjacency list and a priority queue (min-heap) for efficiency.
 *
 * =====================================================================================
 *
 * Algorithm:  Dijkstra's Shortest Path Algorithm
 * Language:  C++
 *
 * Description:  Finds the shortest paths from a single source vertex to all other
 * vertices in a weighted, directed, or undirected graph.
 *
 * Constraint:  This implementation is for graphs with non-negative edge weights.
 *
 * Time Complexity:  O(E log V) where V is the number of vertices and E is the
 * number of edges. The log V factor comes from the priority queue
 * operations (insertion and extraction).
 *
 * Space Complexity:  O(V + E) to store the graph in an adjacency list.
 * Additional space for the distance array O(V) and priority queue O(V).
 *
 * =====================================================================================
 *
 * METHODOLOGY
 * -----------
 * Dijkstra's algorithm works by maintaining a set of distances from the source
 * node to all other nodes. It iteratively selects the unvisited node with the
 * smallest known distance and explores its neighbors.
 *
 * 1.  Initialization:
 * - A distance array `dist` is created, with the source vertex's distance
 * set to 0 and all others to infinity.
 * - A priority queue is initialized, containing the source node with a
 * priority of 0. It's configured as a min-heap to always provide the
 * node with the smallest distance.
 *
 * 2.  Main Loop:
 * - While the priority queue is not empty, extract the vertex `u` with the
 * smallest distance.
 * - For each neighbor `v` of `u`, perform a "relaxation" step:
 * If the path from the source to `v` through `u` is shorter than the
 * currently known distance to `v`, update the distance of `v`.
 * (i.e., if dist[u] + weight(u,v) < dist[v]).
 * - If a distance is updated, the neighbor `v` is added to the priority queue
 * with its new, shorter distance.
 *
 * 3.  Termination:
 * - The algorithm ends when the priority queue is empty, meaning all reachable
 * nodes have been visited and their shortest paths from the source have
 * been finalized.
 *
 * =====================================================================================
 */

#include <iostream>
#include <vector>
#include <queue>
#include <limits>

// Use a type alias for cleaner code, representing a pair of {weight, vertex}
using iPair = std::pair<int, int>;

// Function to perform Dijkstra's algorithm
void dijkstra(int V, std::vector<iPair> adj[], int src) {
    // A priority queue to store vertices that are being preprocessed.
    // We use std::greater to make it a min-heap.
    // The pair stores {distance, vertex}. We store distance first to sort by it.
    std::priority_queue<iPair, std::vector<iPair>, std::greater<iPair>> pq;

    // Create a vector for distances and initialize all distances as infinite.
    std::vector<int> dist(V, std::numeric_limits<int>::max());

    // Insert source itself in priority queue and initialize its distance as 0.
    pq.push({0, src});
    dist[src] = 0;

    // The main loop continues until the priority queue is empty.
    while (!pq.empty()) {
        // Extract the vertex with the minimum distance value.
        int u = pq.top().second;
        pq.pop();

        // Iterate through all adjacent vertices of the extracted vertex 'u'.
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;

            // Relaxation step: If there is a shorter path to v through u.
            if (dist[u] != std::numeric_limits<int>::max() && dist[u] + weight < dist[v]) {
                // Update the distance of v.
                dist[v] = dist[u] + weight;
                // Push the updated vertex to the priority queue.
                pq.push({dist[v], v});
            }
        }
    }

    // Print the calculated shortest distances
    std::cout << "Vertex\t Distance from Source " << src << "\n";
    std::cout << "------\t ----------------------\n";
    for (int i = 0; i < V; ++i) {
        if (dist[i] == std::numeric_limits<int>::max()) {
            std::cout << i << "\t\t" << "INF" << "\n";
        } else {
            std::cout << i << "\t\t" << dist[i] << "\n";
        }
    }
}

// Main function to create a graph and run Dijkstra's algorithm
int main() {
    // Number of vertices in the graph
    int V = 9;

    // Adjacency list representation of the graph
    std::vector<iPair> adj[V];

    // Function to add an edge to the graph
    auto addEdge = [&](int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // For an undirected graph
    };

    // Creating the graph from the example
    addEdge(0, 1, 4);
    addEdge(0, 7, 8);
    addEdge(1, 2, 8);
    addEdge(1, 7, 11);
    addEdge(2, 3, 7);
    addEdge(2, 8, 2);
    addEdge(2, 5, 4);
    addEdge(3, 4, 9);
    addEdge(3, 5, 14);
    addEdge(4, 5, 10);
    addEdge(5, 6, 2);
    addEdge(6, 7, 1);
    addEdge(6, 8, 6);
    addEdge(7, 8, 7);

    // Set the source vertex
    int source = 0;

    // Run Dijkstra's algorithm
    dijkstra(V, adj, source);

    return 0;
}