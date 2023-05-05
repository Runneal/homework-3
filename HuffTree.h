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
class HuffNode { // ���������ڵ�
public:
    int weight; // Ȩ��
    T ch; // �ڵ������ַ�
    HuffNode *left, *right; // ���Һ���ָ��
    Bitmap code; // �ýڵ��Ӧ�ı���

    HuffNode(int w = 0, T c = '\0') : weight(w), ch(c), left(nullptr), right(nullptr) {}

    bool operator<(const HuffNode &rhs) const {
        return weight > rhs.weight; // ����ʹ�á�>�����ǡ�<�����Ա�֤��С��������
    }
    bool operator==(const HuffNode &rhs) const {
        return weight == rhs.weight;
    }
    bool isleaf() { return !left && !right; }
};

template <typename T> class HuffTree : public BinTree<HuffChar<T>> {
private:
    HuffNode<T> *root; // ���ڵ�
public:
    HuffTree() : root(nullptr) {}
    HuffTree(int freq[], int n);  // ���캯���������ַ�Ƶ���б���ַ�����
    ~HuffTree();  // ��������
    HuffNode<T> *getroot() const { return root; } // ��ȡ���ڵ�
    void build_tree(HuffNode<T> *nodes[], int n);  // �ýڵ����鹹�� Huffman ��
    void destroy_tree(HuffNode<T> *p);  // �ͷ����� Huffman ����ռ�õĿռ�
    HuffNode<T>* find_node(HuffNode<T>* root, const T& ch); // ���ҽڵ�
    HuffNode<T>* find_node(const HuffTree<T>& tree, const T& ch);
    void encode(HuffNode<T>* root, const Bitmap& code); // ����
    void encode(const HuffTree<T>& tree);
    void decode(HuffNode<T> *root, Bitmap& code, size_t &idx, T &ch); // ����
    void decode(const HuffTree<T>& tree);
    bool operator<(const HuffTree<T>& rhs) const { return root->weight > rhs.root->weight; }
    bool operator==(const HuffTree<T>& rhs) const { return root->weight == rhs.root->weight; }
    bool isleaf(HuffNode<T>* p) { return !p->left && !p->right; }
};

// ���캯��ʵ��
template <typename T> HuffTree<T>::HuffTree(int freq[], int n) : root(NULL) {
    // ����ÿһ���ڵ㣨�洢�ַ��Ͷ�ӦȨ�أ�
    HuffNode<T> **nodes = new HuffNode<T> *[n];
    for (int i = 0; i < n; ++i) {
        nodes[i] = new HuffNode<T> ((unsigned char)i, freq[i]); //freq�����Ӧ���±�����ַ���ASCII��
    }
    build_tree(nodes, n); // ���� Huffman ��
    delete[] nodes; // �ͷŽڵ�������ռ�õĿռ�
}

// ��������ʵ��
template <typename T> HuffTree<T>::~HuffTree() {
    destroy_tree(root);
}


// �����׼�㷨�������ֺ����޷���ģ�����˵�����������в���������
namespace my_utils {
    template <typename T>
    void swap(T& a, T& b) {
        T t = std::move(a);
        a = std::move(b);
        b = std::move(t);
    }
}

// �ýڵ����鹹����������
template <typename T>
void HuffTree<T>::build_tree(HuffNode<T> *nodes[], int n) {
    // ʹ�ñ�׼�����㷨�Խڵ��б��������
    std::sort(nodes, nodes + n, [](const auto& a, const auto& b){
        return a->weight < b->weight;
    });

    // ѭ���ϲ��ڵ㣬ֱ��ֻʣ��һ���ڵ�
    for (int i = 0; i < n - 1; ++i) {
        // �����½ڵ���Ϊ�ϲ����
        HuffNode<T> *new_node = new HuffNode<T> (nodes[i]->weight + nodes[i+1]->weight, 0);
        new_node->left = nodes[i];
        new_node->right = nodes[i+1];

        // ���´���������
        nodes[i+1] = new_node;

        // ��δ����Ľڵ���������
        for (int j = i+1; j < n-1 && (*(nodes+j))->weight > (*(nodes+j+1))->weight; ++j) {
            my_utils::swap(nodes[j], nodes[j+1]);
        }
    }

    root = nodes[n-1]; // ���ʣ�µĽڵ㼴Ϊ���ڵ�
}
// �ͷ����� Huffman ����ռ�õĿռ�
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
        return true; // ����ҵ���Ŀ���ַ������� true
    }

    if (encode(root->left, ch, code)) { // �����������в���Ŀ���ַ�
        code.set(0); // ���֧�� 0 ��ʾ
        return true;
    }
    if (encode(root->right, ch, code)) { // �����������û��Ŀ���ַ��������������в���
        code.set(1); // �ҷ�֧�� 1 ��ʾ
        return true;
    }

    return false; // �������ʧ�ܣ����� false
}

template<typename T>
bool decode(HuffNode<T> *root, Bitmap& code, size_t &idx, T &ch) {
    if (!root) return false;

    while (code.test(idx)) { // ���֧Ϊ 0���ҷ�֧Ϊ 1
        root = (code.set(++idx)) ? root->right : root->left;
        if (!root) return false;
        if (root->isleaf()) {
            ch = root->ch;
            ++idx;
            return true;
        }
    }

    return false; // ������������� Huffman ����û���ҵ���Ӧ�ַ�����˵������ı����������󣬷��� false
}

template<typename T> HuffNode<T>* find_node(HuffNode<T>* root, const T& ch) { // ���ҽڵ㣬�Ӹ��ڵ㿪ʼ
    if (!root) { // �����ǰ�ڵ�Ϊ�գ����ؿ�ָ��
        return nullptr;
    }

    if (root->isleaf() && root->ch == ch) { // ��ǰ�ڵ���Ҷ�ӽڵ����ַ�ƥ��
        return root;
    }

    HuffNode<T>* node = find_node(root->left, ch); // �������������в���
    if (!node) { // �����������û���ҵ��������������м�������
        node = find_node(root->right, ch);
    }
    return node; // �����ҵ��Ľڵ�
}

template<typename T>
HuffNode<T>* find_node(const HuffTree<T>& tree, const T& ch) {
    return find_node(tree.getroot(), ch);
}