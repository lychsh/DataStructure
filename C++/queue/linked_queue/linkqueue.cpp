#include"linkqueue.h"
#include<iostream>


//初始化队列
void init_queue(queue &Q)
{
    Q.front = new queuenode;
    if(!Q.front){     //申请内存失败
        std::cerr << "Init Error: 初始化失败" << std::endl;
        return ;
    }
    Q.front->next = nullptr;     //next指针置空
    Q.rear = Q.front;            //尾指针设置为头指针
}

//销毁队列
void Destroy_queue(queue &Q)
{
    if(!Q.front){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
    queueptr cur = Q.front->next, _next = nullptr;
    while(cur){     //释放节点元素
        _next = cur->next;     //保存下一节点
        delete cur;    //释放当前节点
        cur = _next;    //指针后移
    }
    delete Q.front;    //释放头结点
    Q.front = nullptr;   //指针置空
    Q.rear = nullptr;
}

//判断队列是否为空
bool queue_empty(queue Q)
{
    if(!Q.front){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return false;
    }
    if(Q.front->next){    //至少有一个节点
        return true;
    }
    return false;
}

//获取队列长度
int queue_length(queue Q)
{
    if(!Q.front){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return -1;
    }
    queueptr cur = Q.front->next;
    int count = 0;
    while(cur){     //释放节点元素
    count ++;       //元素个数计数
        cur = cur->next;    //指针后移
    }
    return count;
}

//获取队列头元素
void get_head(queue Q, elemtype &e)
{
    if(!Q.front){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
    if(Q.front == Q.rear){    //队列为空
        std::cerr << "Empty Error: 队列为空" << std::endl;
        return ;
    }
    e = Q.front->next->data;   //返回队列头元素数据
}

//队尾插入元素
void enqueue(queue &Q, elemtype e)
{
    if(!Q.front || !Q.rear){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
    queueptr newnode = new queuenode;    //新节点
    if(!newnode){       //判断节点是否申请成功
        std::cerr << "Enqueue Error: 插入失败" << std::endl;
        return ;
    }
    newnode->next = nullptr;
    Q.rear->next = newnode;       //插入新节点
    Q.rear = newnode;            //更新尾节点
}

//队头弹出元素
void dequeue(queue &Q, elemtype &e)
{
    if(!Q.front){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
    if(Q.front == Q.rear){    //队列为空
        std::cerr << "Empty Error: 队列为空" << std::endl;
        return ;
    }

    queueptr first = Q.front->next;         //保存队列首节点
    if(Q.rear == first){     //如果队列首节点就是尾节点
        delete first;
        Q.rear = Q.front;     //尾指针指向头节点
        return ;
    }
    //队列元素多于一个时
    Q.front->next = Q.front->next->next;    //删除队列首节点
    delete first;
    first = nullptr;
}

//遍历队列的元素，调用visit函数
void queue_traverse(queue Q, void (*visit)(queuenode))
{
    if(!Q.front){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
}