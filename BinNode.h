// #define BinNodePosi(T) BinNode<T>* //�ڵ�λ��
// template <typename T> using BinNodePosi = BinNode<T>*; //�ڵ�λ��
#define stature(p) ((p) ? (p)->height : -1) //�ڵ�߶ȣ������߶�Ϊ-1��
#define IsRoot(x) (!((x).parent)) //�ж��Ƿ���ڵ�
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc)) //�ж��Ƿ�����
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc)) //�ж��Ƿ��Һ���
#define HasParent(x) (!IsRoot(x)) //�ж��Ƿ��и���
#define HasLChild(x) ((x).lc) //�ж��Ƿ�������
#define HasRChild(x) ((x).rc) //�ж��Ƿ����Һ���
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //�ж��Ƿ��к���
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //�ж��Ƿ�����������
#define IsLeaf(x) (!HasChild(x)) //�ж��Ƿ�ΪҶ�ӽڵ�
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc) //�ֵܽڵ�
#define uncle(x) (IsLChild(*((x)->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc) //����ڵ�
#define FromParentTo(x) (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc)) //���Ը��׵�����
typedef enum { RB_RED, RB_BLACK } RBColor; //�ڵ���ɫ

template <typename T> struct BinNode { //�������ڵ�ģ����
// ��Ա
   T data; //��ֵ
   BinNode<T>* parent; BinNode<T>* lc; BinNode<T>* rc; //���ڵ㼰���Һ���
   int height; //�߶ȣ�ͨ�ã�
   int npl; //Null Path Length����ʽ�ѣ�Ҳ��ֱ����height���棩
   RBColor color; //��ɫ���������
// ���캯��
    BinNode() : 
        parent( nullptr ), lc( nullptr ), rc( nullptr ), height( 0 ), npl( 1 ), color( RB_RED ) {} //Ĭ��
    BinNode( T e, BinNode<T>* p = nullptr, BinNode<T>* lc = nullptr, BinNode<T>* rc = nullptr, int h = 0, int l = 1, RBColor c = RB_RED )
        : data( e ), parent( p ), lc( lc ), rc( rc ), height( h ), npl( l ), color( c ) {} //����
// �����ӿ�
    int size(); //ͳ�Ƶ�ǰ�ڵ����������༴����Ϊ���������Ĺ�ģ
    BinNode<T>* insertAsLC( T const& ); //��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
    BinNode<T>* insertAsRC( T const& ); //��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
    BinNode<T>* succ(); //ȡ��ǰ�ڵ��ֱ�Ӻ��
    template <typename VST> //������
    void travLevel( VST& ); //������α���
    template <typename VST> //������
    void travPre( VST& ); //�����������
    template <typename VST> //������
    void travIn( VST& ); //�����������
    template <typename VST> //������
    void travPost( VST& ); //�����������
    bool operator<( BinNode const& bn ) //�Ƚ������������в��䣩
        { return data < bn.data; }
    bool operator>( BinNode const& bn ) 
        { return data > bn.data; }
    bool operator<=( BinNode const& bn ) 
        { return data <= bn.data; }
    bool operator>=( BinNode const& bn ) 
        { return data >= bn.data; }
    bool operator==( BinNode const& bn ) //�е���
        { return data == bn.data; }
    bool operator!=( BinNode const& bn ) 
        { return data != bn.data; }
}; //BinNode

template <typename T> BinNode<T>* BinNode<T>::insertAsLC( T const& e ) { //��Ϊ��ǰ�ڵ�����Ӳ����½ڵ�
    return lc = new BinNode( e, this ); //�����½ڵ���Ϊ��ǰ�ڵ������
}
template <typename T> BinNode<T>* BinNode<T>::insertAsRC( T const& e ) { //��Ϊ��ǰ�ڵ���Һ��Ӳ����½ڵ�
    return rc = new BinNode( e, this ); //�����½ڵ���Ϊ��ǰ�ڵ���Һ���
}