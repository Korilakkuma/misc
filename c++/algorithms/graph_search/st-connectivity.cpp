#include "data.hpp"

static void st_connectivity(const Graph &graph, INT s);

std::vector<bool> seen;

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cout << "./st_connectivity s t" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT s = std::stoll(argv[1]);
  INT t = std::stoll(argv[2]);

  Graph graph = createDirectedGraph();

  seen.assign(graph.size(), false);

  st_connectivity(graph, s);

  if (seen[t]) {
    std::cout << "s-t (" << s << ", " << t << ") connectivity exists" << std::endl;
  } else {
    std::cout << "s-t (" << s << ", " << t << ") connectivity not found" << std::endl;
  }

  return 0;
}

static void st_connectivity(const Graph &graph, INT s) {
  seen[s] = true;

  for (INT next_v : graph[s]) {
    if (seen[next_v]) {
      continue;
    }

    st_connectivity(graph, next_v);
  }
}
