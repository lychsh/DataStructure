/*
实现链栈的ADT（抽象数据结构）,使用带头节点的单向链表
节点包含数据域(string类型)和指针域
基于实现的链栈实现一些基础功能，如中缀表达式转后缀表达式，括号匹配......
*/


#include<iostream>
#include<stdbool.h>

typedef std::string elemtype;

//链栈节点
typedef struct{
    elemtype data;     // 数据域
    stacknode* next;   //指针域
}stacknode, *stack;


//栈的初始化
void init_stack(stack &S);

//判断栈是否为空
void stack_empty(stack S);

//压栈
void push(stack &S, elemtype e);

//出栈
void pop(stack &S, elemtype &e);

//取栈顶元素
void get_top(stack S, elemtype &e);

//销毁栈
void destroy_stack(stack &S);

//清空栈
void clear_stack(stack &S);

//栈长
int stack_length(stack S);

//遍历栈进行visit操作
void stack_traverse(stack &S, void (*visit)(elemtype e));
