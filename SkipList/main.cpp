#include <iostream>
#include <random>
#include "SkipList.h"

// SkipNode 节点信息
SkipNode::SkipNode(int k, int v, int level):key(k), value(v) {
    for (int i = 0; i < level; ++i) {
        next.push_back(nullptr);
    }
}

// SkipList constructor
SkipList::SkipList(int maxLevel, int iniValue): maxLevel(maxLevel) {
    head = new SkipNode(minInt, iniValue, maxLevel);
    tail = new SkipNode(maxInt, iniValue, maxLevel);

    for (int i = 0; i < maxLevel; ++i) {
        head -> next[i] = tail;
    }
}

// random level
int SkipList::randomLevel() {
    int random_level = 1;
    int seed = time(NULL);
    static std::default_random_engine e(seed);
    static std::uniform_int_distribution<int> u(0, 1);
    while (u(e) && random_level < maxLevel)
    {
        random_level++;
    }
    return random_level;
}

int SkipList::nodelevel(std::vector<SkipNode *> p) {
    return p.size();
}

/*
 * 查找
 * 由于列表有序，首先找到小于该结点的最近的结点，如果下一个结点等于目标结点，则返回该结点。
 * */
SkipNode *SkipList::search(int key) {
    // 根据key值进行查找
    SkipNode* tmp = head;
    // 获取当前节点的level
    int current_level = nodelevel(tmp -> next);
    // 找到当前节点的最右节点值
    // 也就是找到当前层可以遍历最远的节点，且此值为能找到的比当前值小，最大的索引值
    for (int i = (current_level - 1); i >= 0; --i) {
        while (tmp -> next[i] != nullptr && tmp -> next[i] -> key < key) {
            tmp = tmp -> next[i];
        }
    }
    tmp = tmp -> next[0]; //向右前进一步
    if (tmp -> key ==key) {
        std::cout << "This key " << key << " has been found" << std::endl;
        return tmp;
    }
    else {
        return nullptr;
    }
}

/*插入：
1）首先用查找函数来判断该结点是否已经存在，如果存在，则更新该结点的值
2）获取新结点的随机层数
3）找到合适的插入位置
4）插入，并调整每层前后node的指针*/
SkipNode *SkipList::insert(int key, int value) {
    int x_level = randomLevel();
    SkipNode* new_node = nullptr;
    SkipNode* tmp = head;
    // 先search，然后是否存在位置然后再插入
    new_node = SkipList::search(key);
    // 跟新value
    if (new_node) {
        new_node->value = value;
        std::cout << "This node " << key << " has already existed. And its value has been updated to " << value << std::endl;
        return head;
    }

    std::cout << "key: " << key << ", randomLevel: " << x_level << std::endl;
    new_node = new SkipNode(key, value, x_level);
    for (int i = (x_level - 1); i >= 0; --i) {
        // 非常重要，当前最高的索引值且在当前节点的左边！！！
        while (tmp -> next[i] != nullptr && tmp -> next[i] -> key < key) {
            tmp = tmp -> next[i];
        }
        //此时tmp为第i层小于key的最近的结点，接下来完成连接即可
        new_node -> next[i] = tmp -> next[i];
        tmp -> next[i] = new_node;
    }
    return head;
}

/*删除：
1) 用 find方法判断该结点是否存在. 如果不存在，则返回当前list, 并告知该结点不存在
2) 找到小于该结点的最近的结点
3) 更改该结点每层的前面的结点的指针*/
SkipNode *SkipList::deleteNode(int key) {
    // 指向查找的的位置要删除的位置
    SkipNode* index = SkipList::search(key);
    if (!index) { //节点不存在
        std::cout << "This deleting node" << key << "doesn't exist" << std::endl;
        return head;
    }
    else {// 考虑问题就是断链
        SkipNode* tmp = head;
        int head_level = nodelevel(tmp -> next);
        int x_level = index -> next.size();

        std::cout << "The deleting node " << key << "'s level is " << x_level << std::endl;

        for (int i = (x_level - 1); i >= 0; --i) {
            while (tmp -> next[i] != nullptr && tmp -> next[i] -> key < key) {
                tmp = tmp -> next[i];
            }
            // 找到当前节点大于的值
            tmp -> next[i] = index -> next[i];
            std::cout << "This node " << key << " has been deleted from level " << i << std::endl;
        }
        return head;
    }
}

void SkipList::Print_List(int Max_level, int num_of_kv) {
    for (int i = Max_level - 1; i >= 0; i--) {

        std::cout << "head-->";
        SkipNode *tmp = head -> next[i];
        for (int j = 0; j < num_of_kv + 1; j++) {
            if (tmp != nullptr && tmp -> key != SkipList::maxInt) {
                std::cout << tmp->key << "-->";
            }
            else {
                std::cout << "tail";
                break;
            }
            tmp = tmp->next[i];
        }
        std::cout << std::endl;
    }
}

int main() {
    int num_of_level = 5;//5层的跳表
    SkipList list = SkipList(num_of_level, 100);

    //假设插入num_of_kv个键值对
    int num_of_kv = 10;
    for (int i = 0; i < num_of_kv; i++) {
        list.insert(i, 100 * i);
    }

    //层序打印跳表
    SkipNode *head = list.head;
    list.Print_List(num_of_level, num_of_kv);
    /*
    for (int i = num_of_level - 1; i >= 0; i--) {

        std::cout << "head-->";
        SkipNode *tmp = head -> next[i];
        for (int j = 0; j < num_of_kv + 1; j++) {
            if (tmp != nullptr && tmp -> key != SkipList::maxInt) {
                std::cout << tmp->key << "-->";
            }
            else {
                std::cout << "tail";
                break;
            }
            tmp = tmp->next[i];
        }
        std::cout << std::endl;
    }
    */

    //删除测试
    std::cout << "**************************删除key为5的结点后*************************" << std::endl;
    list.deleteNode(5);
    list.insert(10, 2136123);

    //层序打印跳表
    list.Print_List(num_of_level, num_of_kv);
    system("pause");
    return 0;
}
