#include "BinNode.h" //����������ڵ���
#include <algorithm>
#include "../../homework2/Queue.h"
template <typename T> class BinTree { //������ģ����
protected:
    int _size; BinNode<T>* _root; //��ģ�����ڵ�
    virtual int updateHeight( BinNode<T>* x ); //���½ڵ�x�ĸ߶�
    void updateHeightAbove( BinNode<T>* x ); //���½ڵ�x�������ȵĸ߶�
public:

    BinTree() : _size( 0 ), _root( nullptr ) {} //���캯��
    ~BinTree() { if ( 0 < _size ) remove( _root ); } //��������
    int size() const { return _size; } //��ģ
    bool empty() const { return !_root; } //�п�
    BinNode<T>* root() const { return _root; } //����

    BinNode<T>* insertAsRoot( T const& e ); //������ڵ�
    BinNode<T>* insertAsLC( BinNode<T>* x, T const& e ); //��������
    BinNode<T>* insertAsRC( BinNode<T>* x, T const& e ); //�����Һ���
    BinNode<T>* attachAsLC( BinNode<T>* x, BinTree<T>* &Tree ); //����������
    BinNode<T>* attachAsRC( BinNode<T>* x, BinTree<T>* &Tree ); //����������
    int remove ( BinNode<T>* x ); //����ɾ��
    BinTree<T>* secede ( BinNode<T>* x ); //��������

    template <typename VST> //������
    void travLevel( VST& visit ) { if ( _root ) _root->travLevel( visit ); } //��α���
    template <typename VST> //������
    void travPre( VST& visit ) { if ( _root ) _root->travPre( visit ); } //�������
    template <typename VST> //������
    void travPre_R( VST& visit ) { if ( _root ) _root->travPre_R( visit ); } //����������ݹ�棩
    template <typename VST> //������
    void travIn( VST& visit ) { if ( _root ) _root->travIn( visit ); } //�������
    template <typename VST> //������
    void travIn_R( BinNode<T>* x, VST& visit ) { if ( _root ) _root->travIn_R( visit ); } //����������ݹ�棩
    template <typename VST> //������
    void travPost( BinNode<T>* x, VST& visit ) { if ( _root ) _root->travPost( visit ); } //�������
    template <typename VST> //������
    void travPost_R( BinNode<T>* x, VST& visit ) { if ( _root ) _root->travPost_R( visit ); } //����������ݹ�棩

    bool operator<( BinTree<T> const& t ) //�Ƚ������������в��䣩
        { return _root && t._root && lt( _root, t._root ); }
    bool operator>( BinTree<T> const& t ) //�Ƚ������������в��䣩
        { return _root && t._root && gt( _root, t._root ); }
    bool operator<=( BinTree<T> const& t ) //�е���
       { return _root && t._root && (( _root == t._root )||lt( _root, t._root )); }
    bool operator>=( BinTree<T> const& t ) //�е���
       { return _root && t._root && (( _root == t._root )||gt( _root, t._root )); }
    bool operator==( BinTree<T> const& t ) //�е���
       { return _root && t._root && ( _root == t._root ); }
}; //BinTree 

template <typename T> int BinTree<T>::updateHeight( BinNode<T>* x ) { //���½ڵ�x�ĸ߶�
    return x->height = 1 + std::max( stature( x->lc ), stature( x->rc ) ); //ע�⣺�����stature()��Ϊx->height
} //updateHeight

template <typename T> void BinTree<T>::updateHeightAbove( BinNode<T>* x ) { //���½ڵ�x�������ȵĸ߶�
    while ( x ) { //��x����������������������x
        updateHeight( x ); //���µ�ǰ�����������ڵ�ĸ߶�
        x = x->parent; //�ظ��׷����������������
    } //while
} //updateHeightAbove

template <typename T> BinNode<T>* BinTree<T>::insertAsRoot( T const& e ) { //������ڵ�
    _size = 1; return _root = new BinNode<T>( e ); //�������ڵ�
} //insert

template <typename T> BinNode<T>* BinTree<T>::insertAsLC( BinNode<T>* x, T const& e ) { //��������
    _size++; x->insertAsLC( e ); //e��Ϊx�����ӣ�ԭ�ޣ�����
    updateHeightAbove( x ); //����x�����������ȵĸ߶�
    return x->lc; //�����½ڵ�λ��
} //insert

template <typename T> BinNode<T>* BinTree<T>::insertAsRC( BinNode<T>* x, T const& e ) { //�����Һ���
    _size++; x->insertAsRC( e ); //e��Ϊx���Һ��ӣ�ԭ�ޣ�����
    updateHeightAbove( x ); //����x�����������ȵĸ߶�
    return x->rc; //�����½ڵ�λ��
} //insert

template <typename T> BinNode<T>* BinTree<T>::attachAsLC( BinNode<T>* x, BinTree<T>* &S ) { //����������
    if ( ( x->lc = S->_root ) ) //T1�ǿգ���
        x->lc->parent = x; //��T1�ĸ��ڵ�x��Ϊx������
    _size += S->_size; //����ȫ����ģ
    updateHeightAbove( x ); //����x�����������ȵĸ߶�
    S->_root = nullptr; S->_size = 0; release( S ); S = nullptr; //�ͷ�ԭ��
    return x; //���ؽ���λ��
} //attach

template <typename T> BinNode<T>* BinTree<T>::attachAsRC( BinNode<T>* x, BinTree<T>* &S ) { //����������
    if ( ( x->rc = S->_root ) ) //T2�ǿգ���
        x->rc->parent = x; //��T2�ĸ��ڵ���Ϊx���Һ���
    _size += S->_size; //����ȫ����ģ
    updateHeightAbove( x ); //����x�����������ȵĸ߶�
    S->_root = nullptr; S->_size = 0; release( S ); S = nullptr; //�ͷ�ԭ��
    return x; //���ؽ���λ��
} //attach

template <typename T> int BinTree<T>::remove ( BinNode<T>* x ) { //����ɾ���㷨��ɾ����λ��x���ڵ�Ϊ�������������ظ�����ԭ�ȵĹ�ģ
    FromParentTo( *x ) = nullptr; //�ж����Ը��ڵ��ָ��(����xΪ�Ϸ�λ��)
    updateHeightAbove( x->parent ); //�������ȸ߶�
    int n = removeAt( x ); _size -= n; return n; //ɾ������x�����¹�ģ������ɾ���ڵ�����
} //remove

template <typename T> static int removeAt( BinNode<T>* x ) { //����ɾ���㷨��ɾ����λ��x���ڵ�Ϊ�������������ظ�����ԭ�ȵĹ�ģ
    if ( !x ) return 0; //�ݹ��������
    int n = 1 + removeAt( x->lc ) + removeAt( x->rc ); //�ݹ��ͷ���������
    delete[] x; //�ͷű�ժ���ڵ㣬������ɾ���ڵ�����
    return n;
} //removeAt

template <typename T> BinTree<T>* BinTree<T>::secede( BinNode<T>* x ) { //������x�ӵ�ǰ����ժ����������ת��Ϊһ�ö�������
    FromParentTo( *x ) = nullptr; //�ж����Ը��ڵ��ָ��
    updateHeightAbove( x->parent ); //����ԭ�����������ȵĸ߶�
    BinTree<T>* S = new BinTree<T>; S->_root = x; x->parent = nullptr; //������xΪ��
    S->_size = x->size(); _size -= S->_size; return S; //���¹�ģ�����ط������������
} //secede

template <typename T ,typename VST> //Ԫ�����͡�������
void travPre_R(BinNode<T>* x, VST& visit) {
    if (!x) return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}

template <typename T ,typename VST> //Ԫ�����͡�������
void travIn_R(BinNode<T>* x, VST& visit) {
    if (!x) return;
    travIn_R(x->lc, visit);
    visit(x->data);
    travIn_R(x->rc, visit);
}

template <typename T ,typename VST> //Ԫ�����͡�������
void travPost_R(BinNode<T>* x, VST& visit) {
    if (!x) return;
    travPost_R(x->lc, visit);
    travPost_R(x->rc, visit);
    visit(x->data);
}

template <typename T> template <typename VST> //Ԫ�����͡�������
void BinNode<T>::travLevel ( VST& visit ) { //��������α����㷨
    Queue<BinNode<T>*> Q; //��������
    Q.enqueue ( this ); //���ڵ����
    while ( !Q.empty() ) { //�ڶ����ٴα��֮ǰ����������
        BinNode<T>* x = Q.dequeue(); visit ( x->data ); //ȡ�����׽ڵ㲢����֮
        if ( HasLChild ( *x ) ) Q.enqueue ( x->lc ); //�������
        if ( HasRChild ( *x ) ) Q.enqueue ( x->rc ); //�Һ������
    }
}

template <typename T>
static BinNode<T>* merge ( BinNode<T>* a, BinNode<T>* b ) {
    if ( !a ) return b; //�˻����
    if ( !b ) return a; //�˻����
    if ( a->data > b->data ) swap ( a, b ); //ȷ��b����
    BinNode<T>* & al = a->lc, & ar = a->rc; //a����������
    BinNode<T>* & bl = b->lc, & br = b->rc; //b����������
    al = merge ( al, bl ); //�ϲ�a����������b
    al->parent = a; //�����¸��ӹ�ϵ
    ar = merge ( ar, br ); //�ϲ�a����������b
    ar->parent = a; //�����¸��ӹ�ϵ
    return a; //���غϲ����������
}

template <typename T> BinNode<T>* BinNode<T>::succ() {
    BinNode<T>* s = this; //��¼��̵���ʱ����
    if ( rc ) {
        s = rc; //�����Һ��ӣ���ֱ�Ӻ�̱����������У�����ؾ���
        while ( HasLChild ( *s ) ) s = s->lc; //�����С���Ľڵ�
    }
    else { //����ֱ�Ӻ��Ӧ�ǡ�����ǰ�ڵ���������������е�������ȡ�
        while ( IsRChild ( *s ) ) s = s->parent; //������������֧�����ϳ����Ϸ��ƶ�
        s = s->parent; //����ٳ����Ϸ��ƶ�һ�������ִ�ֱ�Ӻ�̣�������ڣ�
    }
    return s;
}