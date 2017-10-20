#include "list.hh"
#include "vector.hh"
#include <cstdio>

using namespace phundrak;

int main(void) {

  printf("Hello 1!\n");
  list<char> test {'C', 'a', 'r', 't', 'i', 'e', 'r'};
  printf("Hello !\n");

  for(auto c : test) {
    printf("%c\n", c);
  }

  list<int> test_unique {1,1,1,1,1,2,2,2,3,3,3,4,4,1,1,5,1,2,1,1,3,3,3};
  printf("Elements before unique():\n");
  for(const auto& elem : test_unique)
    printf("%2d,", elem);

  test_unique.unique();

  return 0;
}
