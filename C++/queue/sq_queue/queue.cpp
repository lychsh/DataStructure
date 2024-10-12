#include"queue.h"
#include<iostream>

/*
---------------------------------------基本操作-------------------------------------
*/

//初始化队列
void init_queue(queue &Q)
{
    Q.elem = new elemtype[MAX_SIZE];
    if(!Q.elem){     //申请内存失败
        std::cerr << "Init Error: 初始化失败" << std::endl;
        return ;
    }
    Q.front = 0;    //初始头尾指针设置为0
    Q.rear = 0;
    Q.length = 0;    //长度为0
}

//销毁队列
void Destroy_queue(queue &Q)
{
    if(!Q.elem){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
    delete Q.elem;    //销毁队列
    Q.elem = nullptr;

}

//判断队列是否为空
bool queue_empty(queue Q)
{
    if(!Q.elem){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
    if(Q.front == Q.rear){      //头尾指针相同为空
        return true;
    }
    return false;
}

//获取队列长度
int queue_length(queue Q)
{
    if(!Q.elem){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
    return Q.length;
}

//获取队列头元素
void get_head(queue Q, elemtype &e)
{
    if(!Q.elem){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
    if(Q.front == Q.rear){      //头尾指针相同为空
        std::cerr << "Empty Error: 队列为空" << std::endl;
        return ;
    }
    e = Q.elem[Q.front];   //返回首元素
}

//队尾插入元素
void enqueue(queue &Q, elemtype e)
{
    if(!Q.elem){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
    if((Q.front + 1) % MAX_SIZE == Q.rear || (Q.rear + 1) % MAX_SIZE == Q.front){
        std::cerr << "Full Error: 队列已满" << std::endl;
        return ;
    }
    Q.elem[Q.rear] = e;     //插入元素
    Q.rear = (Q.rear + 1) % MAX_SIZE;          //尾节点+1
    Q.length ++;
}

//队头弹出元素
void dequeue(queue &Q, elemtype &e)
{
    if(!Q.elem){     //队列不存在
        std::cerr << "Exist Error: 队列不存在" << std::endl;
        return ;
    }
    if(Q.front == Q.rear){      //头尾指针相同为空
        std::cerr << "Empty Error: 队列为空" << std::endl;
        return ;
    }
    e = Q.elem[Q.front];   //返回首元素
    Q.front = (Q.front + 1) % MAX_SIZE;    //头指针后移1
    Q.length --;
}

//遍历队列的元素，调用visit函数
void queue_traverse(queue Q, void (*visit)(elemtype))
{
    for(int i = Q.front; i % MAX_SIZE != Q.rear; i = (i + 1) % MAX_SIZE){
        visit(Q.elem[i]);
    }
}