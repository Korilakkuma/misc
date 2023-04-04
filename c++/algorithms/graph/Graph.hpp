#include <vector>

using INT = long long;

class Graph {
  public:
    Graph(INT N): graph(N) {}

    void create_graph(INT M, bool directed) {
      for (INT i = 0; i < M; i++) {
        INT src;
        INT dest;

        std::cout << "Type source vertex > ";
        std::cin >> src;

        std::cout << "Type destination vertex > ";
        std::cin >> dest;

        graph[src].push_back(dest);

        if (!directed) {
          graph[dest].push_back(src);
        }
      }
    }

    std::size_t size() {
      return graph.size();
    }

    std::vector<INT> edges(INT v) {
      return graph[v];
    }

    bool empty() {
      return graph.empty();
    }

  private:
    std::vector<std::vector<INT>> graph;
};
