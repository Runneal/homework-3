#include "HuffTree.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
using namespace std;
int main() {
    // ����һ���ַ�����Ϊʾ��
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
    // ͳ���ַ�Ƶ�ʲ�������������
    HuffNode<char>** nodes = new HuffNode<char>*[26] {nullptr};
    int n = 0;
    for (int i = 0; i < 128; ++i) {
        if (freq[i]) {
            nodes[n++] = new HuffNode<char>(freq[i], static_cast<char>(i));
        }
    }
    HuffTree<char> tree;
    tree.build_tree(nodes, n);
    // ���ַ������б���
    string s ;
    cout<<"����Ҫ������ַ�����";
    cin>>s;
    string code = "";
    for (char c : s) {
        auto node = tree.find_node(tree.getroot(), c);
        if (node) {
            code += node->code.bits2string(26);
        }
    }
    // �Ա��������н���
    auto p = tree.getroot();
    string decoded = "";
    for (char c : code) {
        if (c == '0') {
            p = p->left;
        } else {
            p = p->right;
        }
        if (!p->left && !p->right) { // �ҵ�Ҷ�ӽڵ�
            decoded += p->ch;
            p = tree.getroot(); // �ص����ڵ�
        }
    }
    // �������ͽ�����
    std::cout << "������Ϊ��" << code << endl;
    std::cout << "������Ϊ��" << decoded << endl;
    // �ͷ�ռ�õ��ڴ�
    delete[] nodes;
    tree.destroy_tree(tree.getroot());
    return 0;
}