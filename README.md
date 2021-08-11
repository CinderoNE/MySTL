# MySTL

基于`c++11`实现了一个简单的`STL`，包含`basic_string`,容器以及部分算法。

## 简介


各函数的接口参考[api文档](https://www.apiref.com/cpp-zh/index.html)，绝大部分接口已经实现。实现的容器以及容器适配器包括。

> `vector`,`list`,`deque`,`stack`,`queue`,`priority_queue`,`set`,`multiset,map`,`multimap`,`unordered_set`,`unordered_multiset`,`unordered_map`,`unordered_multimap`

## 项目目的

加强自己对数据结构以及STL的理解，方便日后更快捷高效的使用。同时也可以让自己熟悉c++的模板编程以及各种语法和特性，提升自己的编程功力。

## Environment

在`msvc1929`和`g++9.3.0`均通过编译

## Build and Test

* Linux

  >./build.sh 构建
  >
  >./test 执行测试

* Windows

  > 用visual studio2019打开MSVC/MySTL.sln
  >
  > F5 执行测试

## Detail

* `my_string.h`：针对char类型实现的string，使用了16个字节的成员数组进行`小字符串优化`
* `my_vector.h`：随机访问容器`vector`，使用3个迭代器来实现，分别指向`已使用的空间头`，`已使用的空间尾后`,`已分配的空间尾后`
* `my_list.h`：双向链表实现的容器`list`，使用虚拟节点`node`来连接list的头尾
* `my_deque.h`：能快速前后插入和删除的随机访问容器`deque`，使用中控区来控制各个缓冲区，并在头尾缓冲区留出空间，因此在头部插入只需在头前放入元素，并向前移动迭代器即可，在头部删除只需删除头部元素，并向后移动迭代器即可，尾部操作同理
* `my_stack.h`：栈`stack`，`deque`容器的适配器，底层使用`deque`，且只使用`deque`的部分接口
* `my_queue.h`：先进先出队列`queue`，`deque`容器的适配器，底层使用`deque`，且只使用`deque`的部分接口
* `my_priority_queue.h`：优先队列`priority_queue`，`vector`容器的适配器，底层使用vector做存储空间，并使用`my_algorithm.h`中的`heap`算法来操作堆
* `my_rb_tree.h`：红黑树`rb_tree`，一种自平衡的二叉搜索树，红黑树的算法实现参考[维基百科](https://zh.wikipedia.org/wiki/%E7%BA%A2%E9%BB%91%E6%A0%91)
* `my_set.h`：包含`set`和`multiset`，以`rb_tree`作为底层数据结构，以Key作为`rb_tree`存储的元素和用来排序的元素。`set`不允许重复Key，使用`rb_tree`的`unique`接口，`multiset`允许重复Key，使用`rb_tree`的`multi`接口
* `my_map.h`：包含`map`和`multimap`，以`rb_tree`作为底层数据结构，以`std::pair<Key,Value>`作为`rb_tree`存储的元素，以Key作为排序的元素。`map`不允许重复Key，使用`rb_tree`的`unique`接口，`multimap`允许重复Key，使用`rb_tree`的`multi`接口
* `my_hash_table.h`：哈希表`hash_table`，使用`vector`来存放桶，每个桶都是一个单向链表。利用`hash`函数计算出元素的桶位置，并在那个桶中插入（具体实现参考代码）。哈希表的桶数都是2的倍数，方便寻找桶位置，具体的实现细节参考[java中的`HashMap`](https://www.zhihu.com/question/422840340)
* `my_unordered_set.h`：包含`unordered_set`和`unordered_multiset`，以`hast_table`作为底层容器，以Key作为`hast_table`存储的元素和用来`hash`的元素。`unordered_set`不允许重复Key，使用`hast_table`的`unique`接口，`unordered_multiset`允许重复Key，使用`hast_table`的`multi`接口
* `my_unordered_map.h`：包含`unordered_map`和`unordered_multimap`，以`hast_table`作为底层容器，以`std::pair<Key,Value>`作为`hast_table`存储的元素，以Key作为`hash`的元素。`unordered_map`不允许重复Key，使用`hast_table`的`unique`接口，`unordered_multimap`允许重复Key，使用`hast_table`的`multi`接口
* `my_algorithm.h`：实现少量算法（具体参考代码），包括`hash`函数对象，`heap`堆操作
* `my_iterator.h`：5类迭代器的定义、迭代器类型的判断，属性的萃取，以及关于迭代器的部分函数和`reverse_iterator`

