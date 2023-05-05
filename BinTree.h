#include "BinNode.h" //引入二叉树节点类
#include <algorithm>
#include "../../homework2/Queue.h"
template <typename T> class BinTree { //二叉树模板类
protected:
    int _size; BinNode<T>* _root; //规模、根节点
    virtual int updateHeight( BinNode<T>* x ); //更新节点x的高度
    void updateHeightAbove( BinNode<T>* x ); //更新节点x及其祖先的高度
public:

    BinTree() : _size( 0 ), _root( nullptr ) {} //构造函数
    ~BinTree() { if ( 0 < _size ) remove( _root ); } //析构函数
    int size() const { return _size; } //规模
    bool empty() const { return !_root; } //判空
    BinNode<T>* root() const { return _root; } //树根

    BinNode<T>* insertAsRoot( T const& e ); //插入根节点
    BinNode<T>* insertAsLC( BinNode<T>* x, T const& e ); //插入左孩子
    BinNode<T>* insertAsRC( BinNode<T>* x, T const& e ); //插入右孩子
    BinNode<T>* attachAsLC( BinNode<T>* x, BinTree<T>* &Tree ); //接入左子树
    BinNode<T>* attachAsRC( BinNode<T>* x, BinTree<T>* &Tree ); //接入右子树
    int remove ( BinNode<T>* x ); //子树删除
    BinTree<T>* secede ( BinNode<T>* x ); //子树分离

    template <typename VST> //操作器
    void travLevel( VST& visit ) { if ( _root ) _root->travLevel( visit ); } //层次遍历
    template <typename VST> //操作器
    void travPre( VST& visit ) { if ( _root ) _root->travPre( visit ); } //先序遍历
    template <typename VST> //操作器
    void travPre_R( VST& visit ) { if ( _root ) _root->travPre_R( visit ); } //先序遍历（递归版）
    template <typename VST> //操作器
    void travIn( VST& visit ) { if ( _root ) _root->travIn( visit ); } //中序遍历
    template <typename VST> //操作器
    void travIn_R( BinNode<T>* x, VST& visit ) { if ( _root ) _root->travIn_R( visit ); } //中序遍历（递归版）
    template <typename VST> //操作器
    void travPost( BinNode<T>* x, VST& visit ) { if ( _root ) _root->travPost( visit ); } //后序遍历
    template <typename VST> //操作器
    void travPost_R( BinNode<T>* x, VST& visit ) { if ( _root ) _root->travPost_R( visit ); } //后序遍历（递归版）

    bool operator<( BinTree<T> const& t ) //比较器（其余自行补充）
        { return _root && t._root && lt( _root, t._root ); }
    bool operator>( BinTree<T> const& t ) //比较器（其余自行补充）
        { return _root && t._root && gt( _root, t._root ); }
    bool operator<=( BinTree<T> const& t ) //判等器
       { return _root && t._root && (( _root == t._root )||lt( _root, t._root )); }
    bool operator>=( BinTree<T> const& t ) //判等器
       { return _root && t._root && (( _root == t._root )||gt( _root, t._root )); }
    bool operator==( BinTree<T> const& t ) //判等器
       { return _root && t._root && ( _root == t._root ); }
}; //BinTree 

template <typename T> int BinTree<T>::updateHeight( BinNode<T>* x ) { //更新节点x的高度
    return x->height = 1 + std::max( stature( x->lc ), stature( x->rc ) ); //注意：这里的stature()即为x->height
} //updateHeight

template <typename T> void BinTree<T>::updateHeightAbove( BinNode<T>* x ) { //更新节点x及其祖先的高度
    while ( x ) { //从x出发，向上逐层检查各代祖先x
        updateHeight( x ); //更新当前（即历代）节点的高度
        x = x->parent; //沿父亲方向继续考察其祖先
    } //while
} //updateHeightAbove

template <typename T> BinNode<T>* BinTree<T>::insertAsRoot( T const& e ) { //插入根节点
    _size = 1; return _root = new BinNode<T>( e ); //创建根节点
} //insert

template <typename T> BinNode<T>* BinTree<T>::insertAsLC( BinNode<T>* x, T const& e ) { //插入左孩子
    _size++; x->insertAsLC( e ); //e作为x的左孩子（原无）插入
    updateHeightAbove( x ); //更新x及其历代祖先的高度
    return x->lc; //返回新节点位置
} //insert

template <typename T> BinNode<T>* BinTree<T>::insertAsRC( BinNode<T>* x, T const& e ) { //插入右孩子
    _size++; x->insertAsRC( e ); //e作为x的右孩子（原无）插入
    updateHeightAbove( x ); //更新x及其历代祖先的高度
    return x->rc; //返回新节点位置
} //insert

template <typename T> BinNode<T>* BinTree<T>::attachAsLC( BinNode<T>* x, BinTree<T>* &S ) { //接入左子树
    if ( ( x->lc = S->_root ) ) //T1非空，则
        x->lc->parent = x; //令T1的根节点x作为x的左孩子
    _size += S->_size; //更新全树规模
    updateHeightAbove( x ); //更新x及其历代祖先的高度
    S->_root = nullptr; S->_size = 0; release( S ); S = nullptr; //释放原树
    return x; //返回接入位置
} //attach

template <typename T> BinNode<T>* BinTree<T>::attachAsRC( BinNode<T>* x, BinTree<T>* &S ) { //接入右子树
    if ( ( x->rc = S->_root ) ) //T2非空，则
        x->rc->parent = x; //令T2的根节点作为x的右孩子
    _size += S->_size; //更新全树规模
    updateHeightAbove( x ); //更新x及其历代祖先的高度
    S->_root = nullptr; S->_size = 0; release( S ); S = nullptr; //释放原树
    return x; //返回接入位置
} //attach

template <typename T> int BinTree<T>::remove ( BinNode<T>* x ) { //子树删除算法：删除以位置x处节点为根的子树，返回该子树原先的规模
    FromParentTo( *x ) = nullptr; //切断来自父节点的指针(假设x为合法位置)
    updateHeightAbove( x->parent ); //更新祖先高度
    int n = removeAt( x ); _size -= n; return n; //删除子树x，更新规模，返回删除节点总数
} //remove

template <typename T> static int removeAt( BinNode<T>* x ) { //子树删除算法：删除以位置x处节点为根的子树，返回该子树原先的规模
    if ( !x ) return 0; //递归基：空树
    int n = 1 + removeAt( x->lc ) + removeAt( x->rc ); //递归释放左、右子树
    delete[] x; //释放被摘除节点，并返回删除节点总数
    return n;
} //removeAt

template <typename T> BinTree<T>* BinTree<T>::secede( BinNode<T>* x ) { //将子树x从当前树中摘除，并将其转换为一棵独立子树
    FromParentTo( *x ) = nullptr; //切断来自父节点的指针
    updateHeightAbove( x->parent ); //更新原树中所有祖先的高度
    BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = nullptr; //新树以x为根
    S->_size = x->size(); _size -= S->_size; return S; //更新规模，返回分离出来的子树
} //secede

template <typename T ,typename VST> //元素类型、操作器
void travPre_R(BinNode<T>* x, VST& visit) {
    if (!x) return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}

template <typename T ,typename VST> //元素类型、操作器
void travIn_R(BinNode<T>* x, VST& visit) {
    if (!x) return;
    travIn_R(x->lc, visit);
    visit(x->data);
    travIn_R(x->rc, visit);
}

template <typename T ,typename VST> //元素类型、操作器
void travPost_R(BinNode<T>* x, VST& visit) {
    if (!x) return;
    travPost_R(x->lc, visit);
    travPost_R(x->rc, visit);
    visit(x->data);
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travLevel ( VST& visit ) { //二叉树层次遍历算法
    Queue<BinNode<T>*> Q; //辅助队列
    Q.enqueue ( this ); //根节点入队
    while ( !Q.empty() ) { //在队列再次变空之前，反复迭代
        BinNode<T>* x = Q.dequeue(); visit ( x->data ); //取出队首节点并访问之
        if ( HasLChild ( *x ) ) Q.enqueue ( x->lc ); //左孩子入队
        if ( HasRChild ( *x ) ) Q.enqueue ( x->rc ); //右孩子入队
    }
}

template <typename T>
static BinNode<T>* merge ( BinNode<T>* a, BinNode<T>* b ) {
    if ( !a ) return b; //退化情况
    if ( !b ) return a; //退化情况
    if ( a->data > b->data ) swap ( a, b ); //确保b不大
    BinNode<T>* & al = a->lc, & ar = a->rc; //a的左、右子树
    BinNode<T>* & bl = b->lc, & br = b->rc; //b的左、右子树
    al = merge ( al, bl ); //合并a的左子树和b
    al->parent = a; //并更新父子关系
    ar = merge ( ar, br ); //合并a的右子树和b
    ar->parent = a; //并更新父子关系
    return a; //返回合并后的新树根
}

template <typename T> BinNode<T>* BinNode<T>::succ() {
    BinNode<T>* s = this; //记录后继的临时变量
    if ( rc ) {
        s = rc; //若有右孩子，则直接后继必在右子树中，具体地就是
        while ( HasLChild ( *s ) ) s = s->lc; //最靠左（最小）的节点
    }
    else { //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”
        while ( IsRChild ( *s ) ) s = s->parent; //逆向地沿右向分支，不断朝左上方移动
        s = s->parent; //最后再朝右上方移动一步，即抵达直接后继（如果存在）
    }
    return s;
}