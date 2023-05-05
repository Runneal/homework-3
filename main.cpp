#include "HuffTree.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
using namespace std;
int main() {
    // 输入一个字符串作为示例
    int freq[26];
    ifstream file("example.txt");
    if (!file.is_open()) {
        cout << "Failed to open file." << endl;
        return 1;
    }
    map<char, int> freq;
    char c;
    while (file.get(c)) {
        if (isalpha(c)) {
            c = toupper(c);
            freq[c]++;
        }
    }
    file.close();
    // 统计字符频率并构建哈夫曼树
    HuffNode<char>** nodes = new HuffNode<char>*[26] {nullptr};
    int n = 0;
    for (int i = 0; i < 128; ++i) {
        if (freq[i]) {
            nodes[n++] = new HuffNode<char>(freq[i], static_cast<char>(i));
        }
    }
    HuffTree<char> tree;
    tree.build_tree(nodes, n);
    // 对字符串进行编码
    string s ;
    cout<<"输入要编码的字符串：";
    cin>>s;
    string code = "";
    for (char c : s) {
        auto node = tree.find_node(tree.getroot(), c);
        if (node) {
            code += node->code.bits2string(26);
        }
    }
    // 对编码结果进行解码
    auto p = tree.getroot();
    string decoded = "";
    for (char c : code) {
        if (c == '0') {
            p = p->left;
        } else {
            p = p->right;
        }
        if (!p->left && !p->right) { // 找到叶子节点
            decoded += p->ch;
            p = tree.getroot(); // 回到根节点
        }
    }
    // 输出编码和解码结果
    std::cout << "编码结果为：" << code << endl;
    std::cout << "解码结果为：" << decoded << endl;
    // 释放占用的内存
    delete[] nodes;
    tree.destroy_tree(tree.getroot());
    return 0;
}