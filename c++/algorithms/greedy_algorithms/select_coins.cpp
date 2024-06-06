#include "data.hpp"

std::vector<INT> coins = { 500, 100, 50, 10, 5, 1 };

static INT select_coins(INT N);

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "./select_coins N" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT N = std::stoll(argv[1]);

  if (N < 0) {
    std::cerr << "./select_coins N (>= 0)" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  INT number_of_coins = select_coins(N);

  std::cout << "the minimum number of coins is " << number_of_coins << std::endl;

  return 0;
}

static INT select_coins(INT N) {
  INT number_of_coins = 0;

  for (INT i = 0; i < coins.size(); i++) {
    INT coin = coins[i];

    INT c = N / coin;

    N -= (coin * c);

    number_of_coins += c;
  }

  return number_of_coins;
}
