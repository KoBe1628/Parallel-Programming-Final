#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
#include <chrono>
#include <random>
#include <limits>

using namespace std;

// Function to generate a random graph
vector<vector<int>> generate_random_graph(int num_nodes, int num_edges) {
    vector<vector<int>> graph(num_nodes);
    mt19937 gen(random_device{}());
    uniform_int_distribution<> dis(0, num_nodes - 1);

    for (int i = 0; i < num_edges; ++i) {
        int u = dis(gen);
        int v = dis(gen);
        if (u != v) {
            graph[u].push_back(v);
            graph[v].push_back(u);
        }
    }
    return graph;
}

// Serial BFS implementation
void bfs_serial(const vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);
    queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

// Parallel BFS implementation
void bfs_parallel(const vector<vector<int>>& graph, int start) {
    vector<bool> visited(graph.size(), false);
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int level_size = q.size();

        // Local queues for each thread
        vector<queue<int>> local_queues(omp_get_max_threads());

#pragma omp parallel
        {
            int thread_id = omp_get_thread_num();
            queue<int>& local_q = local_queues[thread_id];

#pragma omp for
            for (int i = 0; i < level_size; ++i) {
                int node = -1;

#pragma omp critical
                {
                    if (!q.empty()) {
                        node = q.front();
                        q.pop();
                    }
                }

                if (node != -1) {
                    for (int neighbor : graph[node]) {
                        if (!visited[neighbor]) {
#pragma omp critical
                            {
                                if (!visited[neighbor]) {
                                    visited[neighbor] = true;
                                    local_q.push(neighbor);
                                }
                            }
                        }
                    }
                }
            }
        }

        // Merge local queues into the global queue
        for (int i = 0; i < omp_get_max_threads(); ++i) {
            while (!local_queues[i].empty()) {
                q.push(local_queues[i].front());
                local_queues[i].pop();
            }
        }
    }
}

int main() {
    // Detect and show the number of available threads
    int max_threads = omp_get_max_threads();
    cout << "Number of available threads: " << max_threads << endl;

    // Ask the user for the number of threads
    int num_threads;
    while (true) {
        cout << "Enter the number of threads to use (1 to " << max_threads << "): ";
        cin >> num_threads;
        if (cin.fail() || num_threads < 1 || num_threads > max_threads) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and " << max_threads << "." << endl;
        }
        else {
            break;
        }
    }

    // Set the number of threads for OpenMP
    omp_set_num_threads(num_threads);
    cout << "Number of threads set to: " << num_threads << endl;

    // Generate a larger, more complex graph
    int num_nodes = 1000;
    int num_edges = 5000;
    vector<vector<int>> graph = generate_random_graph(num_nodes, num_edges);

    int start_node = 0;

    // Measure time for serial BFS
    auto start = chrono::high_resolution_clock::now();
    bfs_serial(graph, start_node);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_serial = end - start;
    cout << "Serial BFS Time: " << duration_serial.count() << " seconds\n";

    // Measure time for parallel BFS
    start = chrono::high_resolution_clock::now();
    bfs_parallel(graph, start_node);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration_parallel = end - start;
    cout << "Parallel BFS Time: " << duration_parallel.count() << " seconds\n";

    // Display which implementation was more efficient
    if (duration_serial < duration_parallel) {
        cout << "Serial BFS was faster.\n";
    }
    else {
        cout << "Parallel BFS was faster.\n";
    }

    // Display the graph size
    cout << "Processed graph with " << num_nodes << " nodes and " << num_edges << " edges.\n";

    // Calculate and display the speedup
    double speedup = duration_serial.count() / duration_parallel.count();
    cout << "Speedup: " << speedup << endl;

    return 0;
}
