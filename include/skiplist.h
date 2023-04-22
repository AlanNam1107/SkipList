#pragma once
#include "node.h"
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
//数据存储文件夹
#define DATA_DIR "../store/data"

//实现跳表的类模板
template <typename K, typename V> class SkipList {
private:
  Node<K, V> *head;    //头节点
  const int max_level; //跳表的最大层数
  int level;           //跳表的当前层数
  int size;            //跳表的大小
  std::mutex mtx;      //临界区锁

public:
  SkipList(int max_level);

  ~SkipList();

  int get_size() const;

  int get_max_level() const;

  int get_level() const;

  Node<K, V> *get_head() const;

  void insert(K, V);

  void remove(K);

  V search(K);

  void print();

  void save();

  void load();

  int get_random_level();
};

//实现跳表的类模板的成员函数
template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level) : max_level(max_level) {
  this->size = 0;
  this->level = 0;

  K k;
  V v;
  //初始化头节点
  this->head = new Node<K, V>(k, v, this->max_level);
}

template <typename K, typename V> SkipList<K, V>::~SkipList() {
  Node<K, V> *x = this->head;
  Node<K, V> *y = NULL;

  while (x != NULL) {
    y = x->forward[0];
    delete x;
    x = y;
  }
}

template <typename K, typename V> int SkipList<K, V>::get_size() const {
  return this->size;
}

template <typename K, typename V> int SkipList<K, V>::get_max_level() const {
  return this->max_level;
}

template <typename K, typename V> int SkipList<K, V>::get_level() const {
  return this->level;
}

template <typename K, typename V> Node<K, V> *SkipList<K, V>::get_head() const {
  return this->head;
}

template <typename K, typename V> void SkipList<K, V>::insert(K key, V value) {
  // std::lock_guard<std::mutex> lock(mtx);
  Node<K, V> *update[this->max_level + 1];
  Node<K, V> *x = this->head;

  // 从最高层开始，找到每一层的插入位置
  for (int i = this->level; i >= 0; i--) {
    while (x->forward[i] != NULL && x->forward[i]->get_key() < key) {
      x = x->forward[i];
    }
    update[i] = x;
  }

  // 如果key已经存在，则更新value
  if (x->forward[0] != NULL && x->forward[0]->get_key() == key) {
    x->forward[0]->set_value(value);
    return;
  }

  // 生成一个随机层数
  int random_level = this->get_random_level();

  // 如果随机层数大于当前跳表的层数，则更新跳表的层数
  if (random_level > this->level) {
    for (int i = this->level + 1; i <= random_level; i++) {
      update[i] = this->head;
    }
    this->level = random_level;
  }

  // 创建一个新的节点
  x = new Node<K, V>(key, value, random_level);

  // 将新节点插入到跳表中
  for (int i = 0; i <= random_level; i++) {
    x->forward[i] = update[i]->forward[i];
    update[i]->forward[i] = x;
  }

  this->size++;
}

template <typename K, typename V> void SkipList<K, V>::remove(K key) {
  // std::lock_guard<std::mutex> lock(mtx);
  Node<K, V> *update[this->max_level + 1];
  Node<K, V> *x = this->head;

  // 从最高层开始，找到每一层的删除位置
  for (int i = this->level; i >= 0; i--) {
    while (x->forward[i] != NULL && x->forward[i]->get_key() < key) {
      x = x->forward[i];
    }
    update[i] = x;
  }

  // 如果key不存在，则直接返回
  if (x->forward[0] == NULL || x->forward[0]->get_key() != key) {
    return;
  }

  // 将节点从跳表中删除并释放内存
  x = x->forward[0];
  for (int i = 0; i <= x->get_level(); i++) {
    update[i]->forward[i] = x->forward[i];
  }
  delete x;

  // 如果删除的节点是最高层的节点，则更新跳表的层数
  while (this->level > 0 && this->head->forward[this->level] == NULL) {
    this->level--;
  }

  this->size--;
}

template <typename K, typename V> V SkipList<K, V>::search(K key) {
  // std::lock_guard<std::mutex> lock(mtx);

  Node<K, V> *x = this->head;

  // 从最高层开始，找到每一层的查找位置
  for (int i = this->level; i >= 0; i--) {
    while (x->forward[i] != NULL && x->forward[i]->get_key() < key) {
      x = x->forward[i];
    }
  }

  // 如果key存在，则返回value
  if (x->forward[0] != NULL && x->forward[0]->get_key() == key) {
    return x->forward[0]->get_value();
  }

  // 如果key不存在，则返回NULL
  return NULL;
}

template <typename K, typename V> void SkipList<K, V>::print() {
  // std::lock_guard<std::mutex> lock(mtx);
  Node<K, V> *x = this->head->forward[0];

  while (x != NULL) {
    std::cout << x->get_key() << ":" << x->get_value() << std::endl;
    x = x->forward[0];
  }
}

template <typename K, typename V> void SkipList<K, V>::save() {
  // std::lock_guard<std::mutex> lock(mtx);

  std::ofstream data_file(DATA_DIR, std::ios::out | std::ios::trunc);
  Node<K, V> *x = this->head->forward[0];

  // 将跳表的数据写入文件
  while (x != NULL) {
    data_file << x->get_key() << " " << x->get_value() << std::endl;
    x = x->forward[0];
  }
}

template <typename K, typename V> void SkipList<K, V>::load() {
  // std::lock_guard<std::mutex> lock(mtx);

  std::ifstream data_file(DATA_DIR, std::ios::in);
  K key;
  V value;

  // 从文件中读取跳表的数据
  while (data_file >> key >> value) {
    this->insert(key, value);
  }
}

template <typename K, typename V> int SkipList<K, V>::get_random_level() {
  int level = 0;

  // 生成一个随机层数 层数越高，概率越小 每多一层要乘上1/2
  //通常概率为1/2或者1/4
  while (rand() % 2) {
    level++;
  }

  // 如果随机层数大于最大层数，则返回最大层数
  return (level < this->max_level) ? level : this->max_level;
}
