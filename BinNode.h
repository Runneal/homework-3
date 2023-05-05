// #define BinNodePosi(T) BinNode<T>* //节点位置
// template <typename T> using BinNodePosi = BinNode<T>*; //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（空树高度为-1）
#define IsRoot(x) (!((x).parent)) //判断是否根节点
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc)) //判断是否左孩子
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc)) //判断是否右孩子
#define HasParent(x) (!IsRoot(x)) //判断是否有父亲
#define HasLChild(x) ((x).lc) //判断是否有左孩子
#define HasRChild(x) ((x).rc) //判断是否有右孩子
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //判断是否有孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //判断是否有两个孩子
#define IsLeaf(x) (!HasChild(x)) //判断是否为叶子节点
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc) //兄弟节点
#define uncle(x) (IsLChild(*((x)->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc) //叔叔节点
#define FromParentTo(x) (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc)) //来自父亲的引用
typedef enum { RB_RED, RB_BLACK } RBColor; //节点颜色

template <typename T> struct BinNode { //二叉树节点模板类
// 成员
   T data; //数值
   BinNode<T>* parent; BinNode<T>* lc; BinNode<T>* rc; //父节点及左、右孩子
   int height; //高度（通用）
   int npl; //Null Path Length（左式堆，也可直接用height代替）
   RBColor color; //颜色（红黑树）
// 构造函数
    BinNode() : 
        parent( nullptr ), lc( nullptr ), rc( nullptr ), height( 0 ), npl( 1 ), color( RB_RED ) {} //默认
    BinNode( T e, BinNode<T>* p = nullptr, BinNode<T>* lc = nullptr, BinNode<T>* rc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED )
        : data( e ), parent( p ), lc( lc ), rc( rc ), height( h ), npl( l ), color( c ) {} //参数
// 操作接口
    int size(); //统计当前节点后代总数，亦即以其为根的子树的规模
    BinNode<T>* insertAsLC( T const& ); //作为当前节点的左孩子插入新节点
    BinNode<T>* insertAsRC( T const& ); //作为当前节点的右孩子插入新节点
    BinNode<T>* succ(); //取当前节点的直接后继
    template <typename VST> //操作器
    void travLevel( VST& ); //子树层次遍历
    template <typename VST> //操作器
    void travPre( VST& ); //子树先序遍历
    template <typename VST> //操作器
    void travIn( VST& ); //子树中序遍历
    template <typename VST> //操作器
    void travPost( VST& ); //子树后序遍历
    bool operator<( BinNode const& bn ) //比较器（其余自行补充）
        { return data < bn.data; }
    bool operator>( BinNode const& bn ) 
        { return data > bn.data; }
    bool operator<=( BinNode const& bn ) 
        { return data <= bn.data; }
    bool operator>=( BinNode const& bn ) 
        { return data >= bn.data; }
    bool operator==( BinNode const& bn ) //判等器
        { return data == bn.data; }
    bool operator!=( BinNode const& bn ) 
        { return data != bn.data; }
}; //BinNode

template <typename T> BinNode<T>* BinNode<T>::insertAsLC( T const& e ) { //作为当前节点的左孩子插入新节点
    return lc = new BinNode( e, this ); //创建新节点作为当前节点的左孩子
}
template <typename T> BinNode<T>* BinNode<T>::insertAsRC( T const& e ) { //作为当前节点的右孩子插入新节点
    return rc = new BinNode( e, this ); //创建新节点作为当前节点的右孩子
}