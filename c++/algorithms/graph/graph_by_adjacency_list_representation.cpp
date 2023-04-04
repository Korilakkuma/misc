#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

using INT   = long long;
using Graph = std::vector<std::vector<INT>>;

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "./graph_by_adjacency_list_representation N M [directed]" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1], NULL, 10);
  INT M = std::stoll(argv[2], NULL, 10);

  bool directed = false;

  if (argc == 4) {
    directed = (std::string(argv[3]) == "directed");
  }

  Graph g(N);

  for (INT i = 0; i < M; i++) {
    INT s;
    INT d;

    std::cout << "Type source vertex > ";
    std::cin >> s;

    std::cout << "Type destination vertex > ";
    std::cin >> d;

    g[s].push_back(d);

    if (!directed) {
      // Undirected Graph
      g[d].push_back(s);
    }
  }

  for (INT i = 0; i < N; i++) {
    for (INT j = 0; j < g[i].size(); j++) {
      if (directed) {
        std::cout << i << " -> " << g[i][j] << std::endl;
      } else {
        std::cout << i << " <-> " << g[i][j] << std::endl;
      }
    }
  }

  return 0;
}
