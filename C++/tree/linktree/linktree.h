#include<iostream>

typedef std::string elemtype;

typedef enum pointerTag{
    Link,
    Thread
};

typedef struct BiThrNode{
    elemtype data;
    struct BiThrNode *lchild, *rchild;
    pointerTag Ltag, Rtag;
}BiThrNode, *BiThrTree;


BiThrTree pre;
void inorder_traverse(BiThrTree T)
{
    BiThrNode *p = T->lchild;
    while(p != T){
        while(p->Ltag == Link){    //最左节点
            p = p->lchild;
        }
        std::cout << p->data << std::endl;
        while(p->Rtag == Thread && p->rchild != T){
            p = p->rchild;
            std::cout << p->data << std::endl;
        }
        p = p->rchild;
    }
}

bool inorder_threading(BiThrTree &Thrt, BiThrTree T)
{
    Thrt = new BiThrNode;
    if(!(Thrt)) {
        return false;
    }
    Thrt->Ltag = Link;
    Thrt->Rtag = Thread;
    Thrt->rchild = Thrt;
    if(!T){
        Thrt->lchild = Thrt;
    }
    else{
        Thrt->lchild = T;
        BiThrTree pre = Thrt;
        in_threading(T);
        pre->rchild = Thrt;
        pre->Rtag = Thread;
        Thrt->rchild = pre;
    }
    return true;
}

void in_threading(BiThrTree p){
    if(p){
        in_threading(p->lchild);
        if(!p->lchild){
            p->Ltag = Thread;
            p->lchild = pre;
        }
        if(!pre->rchild){
            pre->Rtag = Thread;
            pre->rchild = p;
        }
        pre = p;
        in_threading(p->rchild);
    }
}