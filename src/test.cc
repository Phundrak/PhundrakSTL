#include "list.hh"
#include "vector.hh"
#include <iostream>

using phundrak::list;
using phundrak::vector;
using std::cout;

int main(void) {

  cout << "\n\nTest vecteur\n";

  vector<char> testvec;
  testvec.push_back('C');
  testvec.push_back('a');
  testvec.push_back('r');
  testvec.push_back('t');
  testvec.push_back('i');
  testvec.push_back('e');
  testvec.push_back('r');

  for(size_t i = 0; i < testvec.size(); ++i) {
    cout << testvec[i] << " ";
  }
  cout << std::endl;


  cout << "\n\nTest list\n";

  list<char> test{'C', 'a', 'r', 't', 'i', 'e', 'r'};

  for (auto c : test)
    cout << c << "  ";

  cout << "\n";

  list<int> test_unique{1, 1, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4,
                        4, 1, 1, 5, 1, 2, 1, 1, 3, 3, 3};
  printf("Elements before unique():\n");
  for (const auto &elem : test_unique)
    cout << elem << " ";

  cout << "\n";
  test_unique.unique();
  for (const auto &elem : test_unique)
    cout << elem << " ";
  cout << "\n";

  return 0;
}
