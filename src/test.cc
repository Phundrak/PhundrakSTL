#include "list.hh"
#include "vector.hh"
#include <iostream>

using namespace phundrak;
using std::cout;

int main(void) {

  list<char> test {'C', 'a', 'r', 't', 'i', 'e', 'r'};

  for(auto c : test)
    cout << c << "  ";

  cout << "\n";

  list<int> test_unique {1,1,1,1,1,2,2,2,3,3,3,4,4,1,1,5,1,2,1,1,3,3,3};
  printf("Elements before unique():\n");
  for(const auto& elem : test_unique)
    cout << elem << "  ";

  cout << "\n";
  test_unique.unique();
  for(const auto& elem : test_unique)
    cout << elem << "  ";
  cout << "\n";

  return 0;
}
