#include <iostream>
#include <vector>

using INT = long long;

static const INT INF = 1 << 16;

struct Graph {
  struct Edge {
    INT from;
    INT to;
    INT capacity;
    INT reverse_index;

    Edge(INT from, INT to, INT capacity, INT reverse_index) : from(from), to(to), capacity(capacity), reverse_index(reverse_index) {}
  };

  private:
    std::vector<std::vector<Edge>> adjacency_list;

  public:
    Graph(INT N = 0) : adjacency_list(N) {}

    std::size_t size() {
      return adjacency_list.size();
    }

    std::vector<Edge> &operator [] (INT i) {
      return adjacency_list[i];
    }

    Edge &reverse_edge(const Edge &edge) {
      return adjacency_list[edge.to][edge.reverse_index];
    }

    void run_flow(Edge &edge, INT flow) {
      edge.capacity -= flow;
      reverse_edge(edge).capacity += flow;
    }

    void add_edge(INT from, INT to, INT capacity) {
      INT reverse_from = static_cast<INT>(adjacency_list[from].size());
      INT reverse_to   = static_cast<INT>(adjacency_list[to].size());

      adjacency_list[from].push_back(Edge(from, to, capacity, reverse_to));
      adjacency_list[to].push_back(Edge(to, from, 0, reverse_from));
    }
};

static INT ford_fulkerson(Graph &graph, INT s, INT t);
static INT find_flow(Graph &graph, INT v, INT t, INT f);

static std::vector<std::vector<INT>> graph_source = {
  { 0, 1, 5 },
  { 0, 3, 5 },
  { 1, 2, 4 },
  { 1, 3, 37 },
  { 2, 4, 56 },
  { 2, 5, 9 },
  { 3, 2, 3 },
  { 3, 4, 9 },
  { 4, 5, 2 }
};

std::vector<bool> seen;

int main(int argc, char **argv) {
  INT N = 6;
  INT M = graph_source.size();

  Graph graph(N);

  for (INT e = 0; e < M; e++) {
    std::vector<INT> data = graph_source[e];

    graph.add_edge(data[0], data[1], data[2]);
  }

  std::cout << "max flow is " << ford_fulkerson(graph, 0, (N - 1)) << std::endl;

  return 0;
}

static INT ford_fulkerson(Graph &graph, INT s, INT t) {
  INT max_flow = 0;

  while (true) {
    seen.assign(static_cast<INT>(graph.size()), false);

    INT flow = find_flow(graph, s, t, INF);

    if (flow == 0) {
      return max_flow;
    }

    max_flow += flow;
  }

  return max_flow;
}

static INT find_flow(Graph &graph, INT v, INT t, INT f) {
  if (v == t) {
    return f;
  }

  seen[v] = true;

  for (auto &edge : graph[v]) {
    if (seen[edge.to]) {
      continue;
    }

    if (edge.capacity == 0) {
      continue;
    }

    INT flow = find_flow(graph, edge.to, t, std::min(f, edge.capacity));

    if (flow == 0) {
      continue;
    }

    graph.run_flow(edge, flow);

    return flow;
  }

  return 0;
}
