#include "BinTree.h"
#include <iostream>
#include <cstdio>
using namespace std;
inline void print(int e) { printf("%d ", e); }
int main()
{
    BinTree<int> tree;
    tree.insertAsRoot(1);
    tree.insertAsLC(tree.root(), 2);
    tree.insertAsRC(tree.root(), 3);
    tree.insertAsLC(tree.root()->lc, 4);
    tree.insertAsRC(tree.root()->lc, 5);
    cout<<tree.size()<<endl;
    cout<<tree.root()->data<<endl;
}