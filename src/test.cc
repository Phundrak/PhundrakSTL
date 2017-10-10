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

  return 0;
}
