//
// Created by Shawn Zhao on 2023/3/24.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H
#include <vector>
class SkipNode {
public: // 很危险，这些 kv需要private
    int key; // 索引节点
    int value; // 对应节点的值
    std::vector<SkipNode*> next;
    SkipNode(int k, int v, int level);
};

class SkipList {
public:
    SkipNode *head; // 头节点
    int maxLevel; // 节点最大的层数

    //整型的最小值和最大值
    static const int minInt = std::numeric_limits<int>::min();
    static const int maxInt = std::numeric_limits<int>::max();

    //constructor
    SkipList(int maxLevel, int iniValue);

    //destructor
    ~SkipList() {
        delete head;
        delete tail;
    }

    // 随机层数
    int randomLevel();
    void Print_List(int MaxL, int Num_KV);
    //插入，查找，删除操作
    SkipNode* insert(int key, int value);
    SkipNode* search(int key);
    SkipNode* deleteNode(int key);

private:
    SkipNode *tail; // 尾节点
    // 找到当前node或者skiplist的最大层数
    int nodelevel(std::vector<SkipNode*> p);
};


#endif //SKIPLIST_SKIPLIST_H
