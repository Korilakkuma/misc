#include <vector>

using INT = long long;

struct Edge {
  INT to;
  INT w;

  Edge(INT to, INT w) : to(to), w(w) {}
};

class WeightedGraph {
  public:
    void create_graph(INT M, bool directed) {
      for (INT i = 0; i < M; i++) {
        INT src;
        INT dest;
        INT weight;

        std::cout << "Type source vertex > ";
        std::cin >> src;

        std::cout << "Type destination vertex > ";
        std::cin >> dest;

        std::cout << "Type destination weight > ";
        std::cin >> weight;

        Edge src_edge(dest, weight);
        Edge dest_edge(src, weight);

        graph[src].push_back(dest_edge);

        if (!directed) {
          graph[dest].push_back(src_edge);
        }
      }
    }

    std::size_t size() {
      return graph.size();
    }

    std::vector<Edge> edges(INT n) {
      return graph[n];
    }

    bool empty() {
      return graph.empty();
    }

  private:
    std::vector<std::vector<Edge>> graph;
};
