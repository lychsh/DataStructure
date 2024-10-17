#include"linkstack.h"


//节点比较函数
int datacmy(elemtype e1, elemtype e2)
{
     if (e1.length() > e2.length()){    //e1 > e2 返回1
        return 1;
    }
    else if(e1.length() < e2.length()){  //e1 < e2 返回-1
        return -1;
    }
    else{
        if (e1 > e2){
            return 1;
        }
        else if (e1 < e2){
            return -1;
        }
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
