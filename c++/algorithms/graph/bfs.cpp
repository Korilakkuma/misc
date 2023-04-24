#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>
#include "Graph.hpp"

static void breadth_first_search(Graph &g, INT s);

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "./bfs N M" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1], NULL, 10);
  INT M = std::stoll(argv[2], NULL, 10);

  Graph g(N);

  g.create_graph(M, false);

  INT s;

  std::cout << "Search vertex > ";
  std::cin >> s;

  breadth_first_search(g, s);

  return 0;
}

static void breadth_first_search(Graph &g, INT s) {
  std::vector<bool> seens(g.size(), false);
  std::queue<INT> todos;

  seens[s] = true;
  todos.push(s);

  while (!todos.empty()) {
    INT v = todos.front();

    todos.pop();

    std::cout << " -> " << v << std::endl;

    for (auto nv : g.edges(v)) {
      if (seens[nv]) {
        continue;
      }

      seens[nv] = true;
      todos.push(nv);

      std::cout << " -> " << nv << std::endl;
    }
  }
}
