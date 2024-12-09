#ifndef TREE_H
#define TREE_H

#include<iostream>
#include<vector>


//自闭合标签
const std::vector<std::string> SINGLE_TAGs = {
        "base", "link", "img", "input", "br", "hr", "!DOCTYPE", "area",
        "command", "embed", "keygen",  "meta", "param", 
        "source", "track", "wbr", "!doctype"
};

//标签类别
enum TagType{
    Single,    //单标签
    StartTag,     //开始标签
    EndTag,        //结束标签
    Text,        //文本
    WholeTag      //标签块
};

typedef struct Tag{
    std::string content;     //标签内容
    TagType type;           //简化标签种类
    int start, end;         //标签起始和终点位置
}Tag;

//标签属性
typedef struct TagAttr{
    std::string name;    //属性名
    std::string value;   //属性值
}TagAttr;

//tag标签
typedef struct ElemType{    //节点元素类型
    std::string tag_name;  //标签名称
    TagType type;           //标签类型
    int start, end;       //标签块的起始和终点位置
    std::vector<TagAttr> attrs;      //标签的属性表
}ElemType;

//树的结点
typedef struct CSNode{
    ElemType data;
    CSNode *firstchild, *nextsibling;
}CSNode, *CSTree;

#include<stdio.h>
#include"tree.h"
#include<sstream>
#include <fstream>
#include<queue>
#include<stack>

//初始化空树
void init_tree(CSTree &T);

//根据tag标签字符串创建树结点
void create_node(CSTree &node, Tag tag);

//后序遍历销毁树
void destroy_tree(CSTree &T);

//判断是否是自闭合标签
bool is_single_tag(std::string tag_name);

//将文件路径中的转义字符/改为//
void transfer_path(std::string &path);

//读取文件
bool read_file(std::string &html, std::string path);

//移除多余空格
std::string remove_extraspace(std::string tag, size_t len);


//从html字符串切割出tag标签数组
void split_tags(std::string html, std::vector<Tag> &tags);

//构造树
void create_tree(CSTree &T, std::vector<Tag> &tags);

//清空树
void clear_tree(CSTree &T);

//判断树是否为空树
bool tree_empty(CSTree T);

//返回树的深度
int tree_depth(CSTree T);

//判断某个结点是否在树中
bool in_tree(CSTree T, CSTree cur_e);


//结点cur_e赋值为value
void assign(CSTree T, CSTree &cur_e, ElemType value);
 
//返回非根结点的双亲
CSTree parent(CSTree T, CSTree cur_e);

//返回结点cur_e的最左孩子
CSTree left_child(CSTree T, CSTree cur_e);

//返回结点的右兄弟
CSTree right_sibling(CSTree T, CSTree cur_e);

//插入c为T中p指向结点的第i棵子树
void insert_child(CSTree &T, CSNode p, int i, CSTree C);

//删除T中p指向结点的第i棵子树
void delete_child(CSTree &T, CSNode p, int i);

//按先序对T中的每个结点调用函数visit一次
void PreOrderTraverse(CSTree T, void (*visit)(CSTree node));

//按中序对T中的每个结点调用函数visit一次
void InOrderTraverse(CSTree T, void (*visit)(CSTree node));

//按后序对T中的每个结点调用函数visit一次
void PostOrderTraverse(CSTree T, void (*visit)(CSTree node));

#endif   //TREE_H