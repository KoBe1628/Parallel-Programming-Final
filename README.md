
# Parallel BFS Project

## Project Title: Parallel Breadth-First Search (BFS) in Graphs

### Objective
Implement a parallel BFS algorithm using OpenMP for large graph traversal and compare its performance with the serial BFS algorithm.

### Description
Breadth-First Search (BFS) is a fundamental graph traversal algorithm. The goal is to parallelize BFS to handle large graphs more efficiently using multiple threads.

### Files
- `main.cpp`: Contains the implementation of the serial and parallel BFS algorithms and the main function.
- `CMakeLists.txt`: For those using CMake to build the project.

### Prerequisites
- C++ compiler with OpenMP support (e.g., GCC, Clang)
- CMake (optional, if using the provided `CMakeLists.txt`)

### Compilation Instructions

#### Using g++ (GNU Compiler Collection)
1. Open a terminal.
2. Navigate to the directory containing `main.cpp`.
3. Compile the code using g++ with OpenMP support:
   ```sh
   g++ -fopenmp -o parallel_bfs main.cpp
   ```

#### Using CMake
1. Open a terminal.
2. Navigate to the project directory containing the `CMakeLists.txt` file.
3. Create a build directory and navigate into it:
   ```sh
   mkdir build
   cd build
   ```
4. Run CMake to generate the build files:
   ```sh
   cmake ..
   ```
5. Build the project:
   ```sh
   cmake --build .
   ```

### Running the Program
1. After compilation, run the executable from the terminal:
   ```sh
   ./parallel_bfs
   ```
2. The program will display the number of available threads and prompt you to enter the number of threads to use:
   ```
   Number of available threads: 4
   Enter the number of threads to use (1 to 4):
   ```
3. Enter the desired number of threads and press Enter.
4. The program will then generate a random graph, perform both serial and parallel BFS, and display the execution times for each, along with the speedup achieved.

### Sample Input and Expected Output
#### Input
- Number of threads to use: `3`

#### Expected Output
```
Number of available threads: 4
Enter the number of threads to use (1 to 4): 3
Number of threads set to: 3
Serial BFS Time: 0.0057253 seconds
Parallel BFS Time: 0.0068441 seconds
Serial BFS was faster.
Processed graph with 1000 nodes and 5000 edges.
Speedup: 0.836531
```

### Explanation of Output
- **Serial BFS Time**: Time taken by the serial BFS algorithm.
- **Parallel BFS Time**: Time taken by the parallel BFS algorithm.
- **Comparison**: A message indicating which implementation was faster.
- **Graph Details**: Number of nodes and edges in the generated graph.
- **Speedup**: The ratio of serial BFS time to parallel BFS time, indicating the efficiency of parallelization.

### Additional Information
- **Graph Generation**: The graph is randomly generated with a specified number of nodes and edges.
- **Performance Variability**: The performance and speedup may vary depending on the graph structure and the system's hardware.

### Troubleshooting
- Ensure your compiler supports OpenMP. For GCC, this is typically included by default.
- If you encounter issues with CMake, verify that CMake is installed and properly configured in your system's PATH.
