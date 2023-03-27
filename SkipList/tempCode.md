# 20230327 复现

```c++
#include <iostream>
#include <random>
#include "Skiplist.h"

// SkipNode constructor
SkipNode::SkipNode(int k, int v, int level) : key(k), value(v) {
    for (int i = 0; i < level; ++i) {
        next.push_back(nullptr);
    }
}

SkipList::SkipList(int maxLevel, int iniValue) : maxLevel(maxLevel) {
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

    while (u(e) && random_level < maxLevel) {
        random_level++;
    }
    return random_level;
}


int SkipList::nodelevel(std::vector<SkipNode *> p) {
    return p.size();
}

// search
SkipNode *SkipList::search(int key) {
    /*
    SkipNode* current = head;
    int curlevel = nodelevel(head -> next);
    while (current -> next[curlevel] != nullptr && current -> next[curlevel] -> key < key) {
        current = current -> next[curlevel];
    }
    current = current -> next[curlevel];
    if (current -> key == key)
        return current;
    else
        return nullptr;
    */
    SkipNode* tmp = head;
    int currentlevel = nodelevel(tmp -> next);
    for (int i = (currentlevel - 1); i >= 0; --i) {
        while (tmp -> next[i] != nullptr && tmp -> next[i] -> key < key) {
            tmp = tmp -> next[i];
        }
    }
    tmp = tmp -> next[0];
    if (tmp -> key == key) {
        std::cout << "This key " << key << " has been found" << std::endl;
        return tmp;
    }
    else {
        return nullptr;
    }
}

// insert
SkipNode *SkipList::insertNode(int key, int value) {
    int x_level = randomLevel(); // 构建新的random层数
    SkipNode* new_node = nullptr;
    SkipNode* tmp = head;
    new_node = SkipList::search(key);
    if (new_node) { //查找到节点
        new_node -> value = value;
        std::cout << "This node " << key << " has already existed. And its value has been updated to " << value << std::endl;
        return head;
    }

    std::cout << "key: " << key << ", randomLevel: " << x_level << std::endl;
    new_node = new SkipNode(key, value, x_level);

    for (int i = x_level - 1; i >= 0; --i) {
        while (tmp -> next[i] != nullptr && tmp -> next[i] -> key < key) {
            tmp = tmp -> next[i];
        }
        new_node -> next[i] = tmp -> next[i];
        tmp -> next[i] = new_node;
    }
    return head;
}

SkipNode *SkipList::deleteNode(int key) {
    SkipNode* index = search(key);
    if (!index) { //节点不存在
        std::cout << "This deleting node" << key << "doesn't exist" << std::endl;
        return head;
    }
    else {
        SkipNode* tmp = head;
        //int x_level = SkipList::nodelevel(index -> next);
        int x_level = index -> next.size();
        std::cout << "The deleting node " << key << "'s level is " << x_level << std::endl;

        for (int i = x_level - 1; i >= 0; --i) {
            while (tmp -> next[i] != nullptr && tmp -> next[i] -> key < key) {
                tmp = tmp -> next[i];
            }
            tmp -> next[i] = index -> next[i];
            std::cout << "This node " << key << " has been deleted from level " << i << std::endl;
        }
        return head;
    }
}

// print

void SkipList::Print_List(int MaxL, int Num_KV) {
    for (int i = MaxL - 1; i >= 0; --i) {
        std::cout << "head --> ";
        SkipNode* tmp = head -> next[i];
        for (int j = 0; j < Num_KV + 1; ++j) {
            if (tmp != nullptr && tmp -> key != SkipList::maxInt) {
                std::cout << tmp -> key << " --> ";
            }
            else {
                std::cout << "tail";
                break;
            }
            tmp = tmp -> next[i];
        }
        std::cout << std::endl;
    }
}
```

