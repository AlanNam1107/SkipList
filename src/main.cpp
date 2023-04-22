/**
 * @file
 * @brief 跳表的实现
 * @author Alan Nam
 * @date 2023-04-22
 */
#include "skiplist.h"
#include <iostream>
#include <string>

int main(int, char **) {

  SkipList<int, std::string> sl(32);

  sl.load();

  sl.insert(1, "a");
  sl.insert(3, "c");
  sl.insert(4, "d");
  sl.insert(4, "dd");
  sl.insert(6, "f");
  sl.insert(2, "b");
  sl.insert(7, "g");

  std::cout << "skipList size:" << sl.get_size() << std::endl;

  sl.print();

  std::cout << "search 4:" << sl.search(4) << std::endl;

  sl.remove(4);

  std::cout << "skipList size:" << sl.get_size() << std::endl;

  sl.print();

  sl.save();

  return 0;
}
