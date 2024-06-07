#include "data.hpp"

using Interval = std::pair<INT, INT>;

static INT scheduling(std::vector<Interval> &list);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./scheduling N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  std::vector<Interval> list(N);

  for (INT i = 0; i < N; i++) {
    list[i].first  = generate_random_value(10);
    list[i].second = list[i].first + generate_random_value(10);

    std::cout << "(" << list[i].first << ", " << list[i].second << ") ";
  }

  std::cout << std::endl;

  INT number_of_tasks = scheduling(list);

  std::cout << std::endl;
  std::cout << "the maximum number of tasks is " << number_of_tasks << std::endl;

  return 0;
}

static INT scheduling(std::vector<Interval> &list) {
  std::sort(list.begin(), list.end(), [](const Interval &a, const Interval &b) {
    return a.second < b.second;
  });

  for (INT i = 0; i < list.size(); i++) {
    std::cout << "(" << list[i].first << ", " << list[i].second << ") ";
  }

  std::cout << std::endl;

  INT number_of_tasks = 0;

  INT end_time = 0;

  for (INT i = 0; i < list.size(); i++) {
    if (list[i].first < end_time) {
      continue;
    }

    ++number_of_tasks;

    end_time += list[i].second;
  }

  return number_of_tasks;
}
