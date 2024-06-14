#include <iostream>
#include <cstdlib>
#include <vector>

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
