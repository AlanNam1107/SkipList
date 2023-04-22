// 前向声明
template <typename K, typename V> class SkipList;
//实现节点的类模板
template <typename K, typename V> class Node {
  friend class SkipList<K, V>; //跳表类是友元类,可以访问Node类的私有成员

private:
  K key;                //键
  V value;              //值
  Node<K, V> **forward; //数组存储着指向下一个不同level的节点的指针
  int level;

public:
  Node();

  Node(const K &k, const V &v, int level);

  ~Node();

  K get_key() const;

  V get_value() const;

  int get_level() const;

  void set_level(int);

  void set_key(K);

  void set_value(V);
};

//实现节点的类模板的成员函数
template <typename K, typename V> Node<K, V>::Node() {
  this->forward = nullptr;
  this->level = -1;
}

template <typename K, typename V>
Node<K, V>::Node(const K &k, const V &v, int level) {
  this->key = k;
  this->value = v;
  this->level = level;

  // level+1是因为level是从0开始的
  this->forward = new Node<K, V> *[level + 1];
}

template <typename K, typename V> Node<K, V>::~Node() { delete[] forward; }

template <typename K, typename V> K Node<K, V>::get_key() const {
  return this->key;
}

template <typename K, typename V> V Node<K, V>::get_value() const {
  return this->value;
}

template <typename K, typename V> int Node<K, V>::get_level() const {
  return this->level;
}

template <typename K, typename V> void Node<K, V>::set_level(int level) {
  this->level = level;
}

template <typename K, typename V> void Node<K, V>::set_key(K key) {
  this->key = key;
}

template <typename K, typename V> void Node<K, V>::set_value(V value) {
  this->value = value;
}
