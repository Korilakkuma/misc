#include "data.hpp"

static const INT INF = 1 << 16;

static std::vector<std::vector<INT>> graph_source = {
  { 0, 1, 3 },
  { 0, 3, 100 },
  { 1, 2, 50 },
  { 1, 3, 57 },
  { 1, 4, 4 },
  { 2, 3, 10 },
  { 2, 4, 5 },
  { 3, 1, 5 },
  { 4, 2, 5 },
  { 4, 3, 25 },
  { 4, 5, 8 }
};

static void dijkstra_by_heap(const WeightedGraph &graph, INT start_v);

template<class T> bool chmin(T &a, T b) {
  if (a > b) {
    a = b;

    return true;
  }

  return false;
}

int main(int argc, char **argv) {
  INT N = 5;
  INT M = graph_source.size();

  WeightedGraph graph(N);

  for (INT i = 0; i < M; i++) {
    std::vector<INT> ve = graph_source[i];

    graph[ve[0]].push_back(Edge(ve[1], ve[2]));
  }

  dijkstra_by_heap(graph, 0);

  return 0;
}

static void dijkstra_by_heap(const WeightedGraph &graph, INT start_v) {
  INT N = graph.size();

  std::vector<INT> distances(N, INF);
  std::priority_queue<std::pair<INT, INT>, std::vector<std::pair<INT, INT>>, std::greater<std::pair<INT, INT>>> queue;

  distances[start_v] = 0;

  queue.push(std::make_pair(distances[start_v], start_v));

  while (!queue.empty()) {
    std::pair<INT, INT> pair = queue.top();

    INT d = pair.first;
    INT v = pair.second;

    queue.pop();

    if (d > distances[v]) {
      continue;
    }

    for (Edge edge : graph[v]) {
      if (chmin(distances[edge.to], (distances[v] + edge.weight))) {
        queue.push(std::make_pair(distances[edge.to], edge.to));
      }
    }
  }

  for (INT v = 0 ; v < N; v++) {
    if (distances[v] < INF) {
      std::cout << v << ": " << distances[v] << std::endl;
    } else {
      std::cout << v << " is not relaxation." << std::endl;
    }
  }
}
