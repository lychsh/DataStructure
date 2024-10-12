#include"stack.h"
#include<memory.h>

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
    if(S.base){     //栈已存在
        std::cerr << "Exist Error: 栈已存在" << std::endl;
        return ;
    }
    S.base = new elemtype[STACK_INIT_SIZE];    
    S.top = S.base;    //初始栈为空，栈顶指针指向栈底
    S.stacksize = STACK_INIT_SIZE;    //容量为初始容量
}

//判断栈是否为空
bool stack_empty(stack S)
{
    if(!S.base){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    if(S.base == S.top){    //栈顶指针指向栈底为空
        return true;
    }
    return false;        //否则非空
}

//栈重申请内存,申请成功自动释放原内存空间
elemtype* stack_realloc(elemtype* src, elemtype* end, size_t size)
{
    elemtype* newbase = new elemtype[size];
    if(!newbase){      //新栈空间申请失败
        std::cerr << "CapacityExpand Error: 扩容失败" << std::endl;
        return nullptr;
    } 
    elemtype *cur = src, *new_cur = newbase;    //从栈底开始遍历
    while(cur <= end){     
        *new_cur++ = *cur++;    //复制内存
    }
    delete [] src;     //释放原栈空间
    src = nullptr;

    return newbase;     //返回新栈底指针
}

//压栈
void push(stack &S, elemtype e)
{
    if (!S.base){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    //判断是否满容，如果满容就扩容
    if(S.top - S.base >= S.stacksize){
        elemtype* newbase = stack_realloc(S.base, S.top, S.stacksize + STACK_INCREMENT);    //重申请栈空间
        if(!newbase){      //新栈空间申请失败
            std::cerr << "CapacityExpand Error: 扩容失败" << std::endl;
            return ;
        } 
        S.base = newbase;                              //更新栈的栈顶指针和栈底指针
        S.top = S.base + S.stacksize;
        S.stacksize += STACK_INCREMENT;               //更新容量
    }
    *S.top++ = e;   //压栈，栈顶指针上移一位
}

//出栈
void pop(stack &S, elemtype &e)
{
    if(!S.base){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    if(S.top == S.base){   //栈为空
        std::cerr << "Empty Error: 栈为空" << std::endl;
        return ;
    }
    e = *--S.top;   //返回栈顶元素,栈顶指针回退一位
}

//取栈顶元素
void get_top(stack S, elemtype &e)
{
    if(!S.base){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    if(S.top == S.base){   //栈为空
        std::cerr << "Empty Error: 栈为空" << std::endl;
        return ;
    }
    e = *(S.top - 1);    //返回栈顶元素
}

//销毁栈
void destroy_stack(stack &S)
{
    if(!S.base){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    delete[] S.base;    //销毁栈
    S.base = nullptr;   //避免指针悬空
    S.top = nullptr;
}

//清空栈
void clear_stack(stack &S)
{
    if(!S.base){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    elemtype* cur = S.base;    //从栈底开始遍历
    while(cur < S.top){     
        *cur++ = "";           //置空
    }
    S.top = S.base;      //栈顶指针回退到栈底
}

//栈长
int stack_length(stack S)
{
    if(!S.base){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return -1;
    }
    return S.top - S.base;
}

//遍历栈进行visit操作
void stack_traverse(stack &S, void (*visit)(elemtype e))
{
    if(!S.base){     //栈不存在
        std::cerr << "Don't Exist Error: 栈不存在" << std::endl;
        return ;
    }
    elemtype* cur = S.base;    //从栈底开始遍历
    while(cur < S.top){     
        visit(*cur);
        cur++;                 //指针上移
    }
}
