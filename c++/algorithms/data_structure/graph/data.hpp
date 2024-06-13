#include <iostream>
#include <cstdlib>
#include <vector>

using INT = long long;

using Graph = std::vector<std::vector<INT>>;

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
