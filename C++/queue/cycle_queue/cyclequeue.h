/*
链队列的实现
队列为带头尾指针的单向队列
实现队列的基本操作
*/
#include<string>
#include<stdbool.h>
#define MAX_SIZE 1000

typedef std::string elemtype;

typedef struct{         //节点定义
    int length;         //队列长度
    elemtype* elem;          //数据域
    int front;          //头尾游标
    int rear;
}queue;



/*
---------------------------------------基本操作-------------------------------------
*/

//初始化队列
void init_queue(queue &Q);

//销毁队列
void Destroy_queue(queue &Q);

//判断队列是否为空
bool queue_empty(queue Q);

//获取队列长度
int queue_length(queue Q);

//获取队列头元素
void get_head(queue Q, elemtype &e);

//队尾插入元素
void enqueue(queue &Q, elemtype e);

//队头弹出元素
void dequeue(queue &Q, elemtype &e);

//遍历队列的元素，调用visit函数
void queue_traverse(queue Q, void (*visit)(elemtype));
