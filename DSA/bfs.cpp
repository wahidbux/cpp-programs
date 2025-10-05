#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Function to perform BFS
void BFS(int start, vector<vector<int>>& adjList, vector<bool>& visited) {
    queue<int> q;
    visited[start] = true;
    q.push(start);

    cout << "BFS traversal starting from node " << start << ": ";

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        // Visit all adjacent nodes
        for (int neighbor : adjList[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
    cout << endl;
}

int main() {
    int n, edges;
    cout << "Enter number of nodes: ";
    cin >> n;
    cout << "Enter number of edges: ";
    cin >> edges;

    // Adjacency list
    vector<vector<int>> adjList(n);

    cout << "Enter edges (u v) for each edge:\n";
    for (int i = 0; i < edges; i++) {
        int u, v;
        cin >> u >> v;
        // For undirected graph, add both ways
        adjList[u].push_back(v);
        adjList[v].push_back(u);
    }

    vector<bool> visited(n, false);
    int startNode;
    cout << "Enter starting node for BFS: ";
    cin >> startNode;

    BFS(startNode, adjList, visited);

    return 0;
}
