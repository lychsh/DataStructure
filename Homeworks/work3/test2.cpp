#include<iostream>
#include<stdbool.h>

typedef int elemtype;

//链栈节点
typedef struct stacknode{
    elemtype data;     // 数据域
    stacknode *next;   //指针域
}stacknode, *stack;


//节点比较函数
int datacmy(elemtype e1, elemtype e2)
{
    if (e1 > e2){
        return 1;
    }
    else if (e1 < e2){
        return -1;
    }
    return 0;     //e1 = e2 返回0
}

//栈的初始化
void init_stack(stack &S)
{
    if(S){      //栈已存在
        std::cerr << "Exist Error: 栈已存在" << std::endl;
        return ;
    }
    S = new stacknode;
    if(!S){       //初始化栈失败
        std::cerr << "Init Error: 初始化栈失败" << std::endl;
        return ;
    }
    S->next = nullptr;
}

//判断栈是否为空
bool stack_empty(stack S)
{
    if(!S){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return false;
    }
    if(S->next == nullptr){   //栈空，返回true
        return true;
    }
    return false;            //否则，返回false
}

//压栈
void push(stack &S, elemtype e)
{
    if(!S){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    stacknode* newtop = new stacknode;
    if(!newtop){      //申请新栈顶节点失败
        std::cerr << "Push Error: 拓展栈顶失败" << std::endl;
        return ;
    }
    newtop->data = e;
    newtop->next = S->next;    //压入新栈顶节点
    S->next = newtop;
}

//出栈
void pop(stack &S, elemtype &e)
{
    if(!S){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    if(!S->next){   //栈已空
        std::cerr << "Empty Error: 栈已空" << std::endl;
        return ;
    }
    stack top = S->next;   //保存栈顶
    e = top->data;     //返回栈顶元素数据
    S->next = S->next->next;    //出栈
    delete top;        //释放栈顶元素
    top = nullptr;
}

//取栈顶元素
void get_top(stack S, elemtype &e)
{
    if(!S){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    if(!S->next){   //栈已空
        std::cerr << "Empty Error: 栈已空" << std::endl;
        return ;
    }
    e = S->next->data;   //返回栈顶元素数据
}

//销毁栈
void destroy_stack(stack &S)
{
    if(!S){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    stack cur = S->next, _next;   //保存当前节点和下一节点
    while(cur){
        _next = cur->next;        //更新next节点
        delete cur;                //销毁当前节点
        cur = _next;              //节点后移      
    }
    delete S;
    S = nullptr;
}

//清空栈
void clear_stack(stack &S)
{
    if(!S){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    //销毁保存数据的节点
    stack cur = S->next, _next;   //保存当前节点和下一节点
    while(_next){
        _next = cur->next;        //更新next节点
        delete cur;                //销毁当前节点
        cur = nullptr;
        cur = _next;              //节点后移      
    }
    S->next = nullptr;  //避免未定义错误
}

//栈长
int stack_length(stack S)
{
    if(!S){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return -1;
    }
    int len = 0;
    stack cur = S->next;   //当前节点
    while(cur){
        len ++;
        cur = cur->next;              //节点后移      
    }
    return len;
}

//遍历栈进行visit操作
void stack_traverse(stack &S, void (*visit)(elemtype e))
{
    if(!S){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    stack cur = S->next;   //当前节点
    while(cur){
        visit(cur->data);
        cur = cur->next;              //节点后移      
    }
}


void enqueue(stack &S1, stack &S2, elemtype e)
{
    if(!S1 || !S2){     //队列不存在
        std::cerr << "Don't Exist Error: 队列不存在" << std::endl;
        return ;
    }
    push(S1, e);
}

void dequeue(stack &S1, stack &S2, elemtype &e)
{
    if(!S1 || !S2){     //队列不存在
        std::cerr << "Don't Exist Error: 队列不存在" << std::endl;
        return ;
    }
    if(stack_empty(S1) && stack_empty(S2)){
        std::cout << "队列已空" << std::endl;
        return ;
    }
    elemtype tmp;
    if(stack_empty(S2) && !stack_empty(S1)){  //S1非空，S2空，先将S1元素弹出，压入S2
        while(!stack_empty(S1)){
            pop(S1, tmp);
            push(S2, tmp);
        }
        pop(S2, e);
        return ;
    }
    pop(S2, e);
}

int main()
{
    stack S1 = nullptr, S2 = nullptr;
    init_stack(S1);
    init_stack(S2);
    elemtype node;
    int n;
    int choose;
    int exit = 1;
    while(exit){
        std::cout << "--------------请根据操作输入相应的数字--------------" << std::endl; 
        std::cout << "**********************1.输入************************" << std::endl; 
        std::cout << "**********************2.弹出************************" << std::endl; 
        std::cout << "**********************3.压入************************" << std::endl; 
        std::cout << "**********************4.退出*************************" << std::endl; 
        std::cout << "请输入：" << std::endl;
        std::cin >> choose;
        switch(choose){
            case 1:
                std::cout << "输入插入的个数" << std::endl;
                std::cin >> n;
                std::cout << "输入插入的值, 空格隔开" << std::endl;
                for(int i = 0; i< n; i++){
                    std::cin >> node;
                    enqueue(S1, S2, node);
                }
                break;
            case 2:
                if(!stack_empty(S1) || !stack_empty(S2)){
                    std::cout << "队首元素为" << std::endl;
                    dequeue(S1, S2, node);
                    std::cout << node << std::endl;
                }
                else{
                    std::cout << "队列已空" << std::endl;
                }
                break;
            case 3:
                std::cout << "输入插入的值：" << std::endl;
                std::cin >> node;
                enqueue(S1, S2, node);
                break;
            case 4:
                destroy_stack(S1);
                destroy_stack(S2);
                exit = 0;
                break;
            default:
                break;
        }
    }
    
}