#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stack>
#include "Graph.hpp"

static void depth_first_search(Graph &g, INT s);
static void depth_first_search_by_recursive(Graph &g, std::vector<bool> &seens, INT v);

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "./dfs N M [recursive]" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1], NULL, 10);
  INT M = std::stoll(argv[2], NULL, 10);

  Graph g(N);

  g.create_graph(M, false);

  INT s;

  std::cout << "Search vertex > ";
  std::cin >> s;

  if ((argc == 4) && (std::string(argv[3]) == "recursive")) {
    std::vector<bool> seens(N, false);

    for (INT v = 0; v < N; v++) {
      if (seens[v]) {
        continue;
      }

      std::cout << " -> " << v << std::endl;

      depth_first_search_by_recursive(g, seens, v);
    }
  } else {
    depth_first_search(g, s);
  }

  return 0;
}

static void depth_first_search(Graph &g, INT s) {
  std::vector<bool> seens(g.size(), false);
  std::stack<INT> todos;

  seens[s] = true;
  todos.push(s);

  while (!todos.empty()) {
    INT v = todos.top();

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

static void depth_first_search_by_recursive(Graph &g, std::vector<bool> &seens, INT v) {
  seens[v] = true;

  for (auto nv : g.edges(v)) {
    if (seens[nv]) {
      continue;
    }

    std::cout << " -> " << nv << std::endl;

    depth_first_search_by_recursive(g, seens, nv);
  }
}
