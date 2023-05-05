#include "BinTree.h"
#include "Bitmap.h"
#include <unordered_map>
#include <cstring>
#include <algorithm>
#include <utility>
template <typename T> class HuffChar {
public:
    T data;
    int weight;
    HuffChar() {}
    HuffChar(T d, int w) : data(d), weight(w) {}
    bool operator<(HuffChar<T> const& hc) { return weight > hc.weight; }
    bool operator==(HuffChar<T> const& hc) { return weight == hc.weight; }
};

template<typename T>
class HuffNode { // 哈夫曼树节点
public:
    int weight; // 权重
    T ch; // 节点代表的字符
    HuffNode *left, *right; // 左右孩子指针
    Bitmap code; // 该节点对应的编码

    HuffNode(int w = 0, T c = '\0') : weight(w), ch(c), left(nullptr), right(nullptr) {}

    bool operator<(const HuffNode &rhs) const {
        return weight > rhs.weight; // 这里使用“>”而非“<”可以保证从小到大排序。
    }
    bool operator==(const HuffNode &rhs) const {
        return weight == rhs.weight;
    }
    bool isleaf() { return !left && !right; }
};

template <typename T> class HuffTree : public BinTree<HuffChar<T>> {
private:
    HuffNode<T> *root; // 根节点
public:
    HuffTree() : root(nullptr) {}
    HuffTree(int freq[], int n);  // 构造函数，传入字符频率列表和字符总数
    ~HuffTree();  // 析构函数
    HuffNode<T> *getroot() const { return root; } // 获取根节点
    void build_tree(HuffNode<T> *nodes[], int n);  // 用节点数组构建 Huffman 树
    void destroy_tree(HuffNode<T> *p);  // 释放整个 Huffman 树所占用的空间
    HuffNode<T>* find_node(HuffNode<T>* root, const T& ch); // 查找节点
    HuffNode<T>* find_node(const HuffTree<T>& tree, const T& ch);
    void encode(HuffNode<T>* root, const Bitmap& code); // 编码
    void encode(const HuffTree<T>& tree);
    void decode(HuffNode<T> *root, Bitmap& code, size_t &idx, T &ch); // 解码
    void decode(const HuffTree<T>& tree);
    bool operator<(const HuffTree<T>& rhs) const { return root->weight > rhs.root->weight; }
    bool operator==(const HuffTree<T>& rhs) const { return root->weight == rhs.root->weight; }
    bool isleaf(HuffNode<T>* p) { return !p->left && !p->right; }
};

// 构造函数实现
template <typename T> HuffTree<T>::HuffTree(int freq[], int n) : root(NULL) {
    // 建立每一个节点（存储字符和对应权重）
    HuffNode<T> **nodes = new HuffNode<T> *[n];
    for (int i = 0; i < n; ++i) {
        nodes[i] = new HuffNode<T> ((unsigned char)i, freq[i]); //freq数组对应的下标就是字符的ASCII码
    }
    build_tree(nodes, n); // 构建 Huffman 树
    delete[] nodes; // 释放节点数组所占用的空间
}

// 析构函数实现
template <typename T> HuffTree<T>::~HuffTree() {
    destroy_tree(root);
}


// 解决标准算法库中种种函数无法对模板参数说明符变量进行操作的问题
namespace my_utils {
    template <typename T>
    void swap(T& a, T& b) {
        T t = std::move(a);
        a = std::move(b);
        b = std::move(t);
    }
}

// 用节点数组构建哈夫曼树
template <typename T>
void HuffTree<T>::build_tree(HuffNode<T> *nodes[], int n) {
    // 使用标准排序算法对节点列表进行排序
    std::sort(nodes, nodes + n, [](const auto& a, const auto& b){
        return a->weight < b->weight;
    });

    // 循环合并节点，直到只剩下一个节点
    for (int i = 0; i < n - 1; ++i) {
        // 创建新节点作为合并结果
        HuffNode<T> *new_node = new HuffNode<T> (nodes[i]->weight + nodes[i+1]->weight, 0);
        new_node->left = nodes[i];
        new_node->right = nodes[i+1];

        // 更新待排序序列
        nodes[i+1] = new_node;

        // 对未排序的节点重新排序
        for (int j = i+1; j < n-1 && (*(nodes+j))->weight > (*(nodes+j+1))->weight; ++j) {
            my_utils::swap(nodes[j], nodes[j+1]);
        }
    }

    root = nodes[n-1]; // 最后剩下的节点即为根节点
}
// 释放整个 Huffman 树所占用的空间
template <typename T> void HuffTree<T>::destroy_tree(HuffNode<T> *p) {
    if (p == NULL) return;
    destroy_tree(p->left);
    destroy_tree(p->right);
    delete p;
}

template<typename T>
bool encode(HuffNode<T> *root, const T &ch, Bitmap& code) {
    if (!root) return false;

    if (root->isleaf() && root->ch == ch) {
        return true; // 如果找到了目标字符，返回 true
    }

    if (encode(root->left, ch, code)) { // 先在左子树中查找目标字符
        code.set(0); // 左分支用 0 表示
        return true;
    }
    if (encode(root->right, ch, code)) { // 如果左子树中没有目标字符，则在右子树中查找
        code.set(1); // 右分支用 1 表示
        return true;
    }

    return false; // 如果查找失败，返回 false
}

template<typename T>
bool decode(HuffNode<T> *root, Bitmap& code, size_t &idx, T &ch) {
    if (!root) return false;

    while (code.test(idx)) { // 左分支为 0，右分支为 1
        root = (code.set(++idx)) ? root->right : root->left;
        if (!root) return false;
        if (root->isleaf()) {
            ch = root->ch;
            ++idx;
            return true;
        }
    }

    return false; // 如果遍历完整个 Huffman 树都没有找到对应字符，则说明输入的编码序列有误，返回 false
}

template<typename T> HuffNode<T>* find_node(HuffNode<T>* root, const T& ch) { // 查找节点，从根节点开始
    if (!root) { // 如果当前节点为空，返回空指针
        return nullptr;
    }

    if (root->isleaf() && root->ch == ch) { // 当前节点是叶子节点且字符匹配
        return root;
    }

    HuffNode<T>* node = find_node(root->left, ch); // 优先在左子树中查找
    if (!node) { // 如果左子树中没有找到，则在右子树中继续查找
        node = find_node(root->right, ch);
    }
    return node; // 返回找到的节点
}

template<typename T>
HuffNode<T>* find_node(const HuffTree<T>& tree, const T& ch) {
    return find_node(tree.getroot(), ch);
}