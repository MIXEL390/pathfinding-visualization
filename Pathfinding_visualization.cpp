#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <SFML/Graphics.hpp>
#include <random>


class Graph {
public:
    int vertices; 
    std::vector<std::vector<int>> adjList; 

    Graph(int v) : vertices(v), adjList(v) {}

    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u);  // For not oriented
    }

    // BFS 
    std::vector<int> bfs(int start, int target) {
        return search(start, target, true);
    }

    // DFS 
    std::vector<int> dfs(int start, int target) {
        return search(start, target, false);
    }

private:
    // main search
    std::vector<int> search(int start, int target, bool isBFS) {
        std::vector<char> visited(vertices, false);
        std::vector<int> parent(vertices, -1);  // recreate path
        std::deque<int> q;  //  deque BFS \ DFS

        q.push_back(start);
        visited[start] = true;

        while (!q.empty()) {
            int node = isBFS ? q.front() : q.back();
            if (!isBFS) q.pop_back(); else q.pop_front();

            if (node == target) {
                return reconstructPath(parent, target);
            }

            // neighbor check
            for (int neighbor : adjList[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    parent[neighbor] = node;
                    if (isBFS) q.push_back(neighbor);
                    else q.push_front(neighbor);
                }
            }
        }
        return {};  
    }

    std::vector<int> reconstructPath(const std::vector<int>& parent, int target) {
        std::vector<int> path;
        for (int v = target; v != -1; v = parent[v]) {
            path.push_back(v);
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

public:
    void generateRandomGraph(int edgeProbability) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> prob(0, 100);

        for (int i = 0; i < vertices; ++i) {
            for (int j = i + 1; j < vertices; ++j) {
                if (prob(gen) < edgeProbability) {
                    addEdge(i, j);
                }
            }
        }
    }
};

// SFML visualising function
void visualizeGraph(const Graph& g, const std::vector<int>& path, const std::vector<int>& visitedNodes, bool isDFS) {
    sf::RenderWindow window(sf::VideoMode(800, 600), isDFS ? "DFS Visualization" : "BFS Visualization");

    std::vector<sf::CircleShape> nodes(g.vertices);
    std::vector<sf::VertexArray> edgesArr;

    // creating coordinates
    std::vector<sf::Vector2f> positions(g.vertices);
    for (int i = 0; i < g.vertices; ++i) {
        positions[i] = sf::Vector2f(rand() % 750 + 25, rand() % 550 + 25);
        nodes[i].setRadius(20);
        nodes[i].setFillColor(sf::Color::Green);
        nodes[i].setPosition(positions[i]);
    }

    // draw lines
    for (int i = 0; i < g.vertices; ++i) {
        for (int neighbor : g.adjList[i]) {
            if (i < neighbor) {
                sf::VertexArray line(sf::Lines, 2);
                line[0].position = positions[i];
                line[1].position = positions[neighbor];
                line[0].color = sf::Color::White;
                line[1].color = sf::Color::White;
                edgesArr.push_back(line);
            }
        }
    }

    // draw path
    sf::VertexArray pathLine(sf::LinesStrip, path.size());
    for (size_t i = 0; i < path.size(); ++i) {
        pathLine[i].position = positions[path[i]];
        pathLine[i].color = sf::Color::Red;
    }

    for (int i : visitedNodes) {
        nodes[i].setFillColor(sf::Color::Blue); 
    }

    // Main visual.. cycle
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        for (const auto& edge : edgesArr) {
            window.draw(edge);
        }
        for (const auto& node : nodes) {
            window.draw(node);
        }
        window.draw(pathLine);

        window.display();
    }
}

int main() {
    Graph g(10);
    g.generateRandomGraph(50);

    int start = 0;
    int target = 9;

    // Test BFS
    std::vector<int> bfsPath = g.bfs(start, target);
    if (!bfsPath.empty()) {
        std::cout << "BFS Path found: ";
        for (int v : bfsPath) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "BFS Path not found." << std::endl;
    }

    // Test DFS
    std::vector<int> dfsPath = g.dfs(start, target);
    if (!dfsPath.empty()) {
        std::cout << "DFS Path found: ";
        for (int v : dfsPath) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    else {
        std::cout << "DFS Path not found." << std::endl;
    }

    visualizeGraph(g, bfsPath, bfsPath, false);  // Äëÿ BFS
    visualizeGraph(g, dfsPath, dfsPath, true);   // Äëÿ DFS

    return 0;
}