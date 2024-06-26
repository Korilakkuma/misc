#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <queue>

using INT = long long;

struct Edge {
  INT to;
  INT weight;
  Edge(INT to, INT weight) : to(to), weight(weight) {}
};

using Graph = std::vector<std::vector<INT>>;
using WeightedGraph = std::vector<std::vector<Edge>>;

static std::vector<std::vector<INT>> data = {
  { 8, 13 },
  { 4, 1 },
  { 4, 2 },
  { 4, 6 },
  { 1, 6 },
  { 1, 6 },
  { 2, 5 },
  { 2, 7 },
  { 6, 7 },
  { 3, 0 },
  { 3, 7 },
  { 7, 0 },
  { 0, 5 },
};

Graph graph(data.size());
WeightedGraph weighted_graph(data.size());

INT generate_random_value(INT max) {
  return (std::rand() * max) / (RAND_MAX + 1.0);
}

Graph &createDirectedGraph(void) {
  INT M = data.size();

  for (INT i = 0; i < M; i++) {
    std::vector<INT> vertices = data[i];

    graph[vertices[0]].push_back(vertices[1]);
  }

  return graph;
}

Graph &createUndirectedGraph(void) {
  INT M = data.size();

  for (INT i = 0; i < M; i++) {
    std::vector<INT> vertices = data[i];

    graph[vertices[0]].push_back(vertices[1]);
    graph[vertices[1]].push_back(vertices[0]);
  }

  return graph;
}

WeightedGraph &createWeightedDirectedGraph(void) {
  INT M = data.size();

  for (INT i = 0; i < M; i++) {
    std::vector<INT> vertices = data[i];
    INT weight = generate_random_value(10);

    weighted_graph[vertices[0]].push_back(Edge(vertices[1], weight));
  }

  return weighted_graph;
}

WeightedGraph &createWeightedUndirectedGraph(void) {
  INT M = data.size();

  for (INT i = 0; i < M; i++) {
    std::vector<INT> vertices = data[i];
    INT weight = generate_random_value(10);

    weighted_graph[vertices[0]].push_back(Edge(vertices[1], weight));
    weighted_graph[vertices[1]].push_back(Edge(vertices[0], weight));
  }

  return weighted_graph;
}

void dfs(const Graph &grach, INT v) {
  INT N = (INT)graph.size();

  std::vector<bool> seen(N, false);
  std::stack<INT> todos;

  seen[v] = true;
  todos.push(v);

  while (!todos.empty()) {
    INT next_v = todos.top();

    todos.pop();

    for (INT r : graph[next_v]) {
      if (seen[r]) {
        continue;
      }

      seen[r] = true;
      todos.push(r);
    }
  }
}

void bfs(const Graph &grach, INT v) {
  INT N = (INT)graph.size();

  std::vector<bool> seen(N, false);
  std::queue<INT> todos;

  seen[v] = true;
  todos.push(v);

  while (!todos.empty()) {
    INT next_v = todos.front();

    todos.pop();

    for (INT r : graph[next_v]) {
      if (seen[r]) {
        continue;
      }

      seen[r] = true;
      todos.push(r);
    }
  }
}
