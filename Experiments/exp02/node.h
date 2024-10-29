#include<string>

#ifndef NODE_H
#define NODE_H

enum HtmlElemClass{
    None,        //非法标签
    Text,      //文本内容
    Single,      //自闭合标签
    TextInline,   //只嵌套文本的行内元素
    Inline,      //普通行内元素--可以嵌套行内元素和自闭合标签
    SpecialBlock,   //特殊块级元素-只能嵌套行内元素
    Block,         //块级元素, 包括特殊行内元素-<a>
    Table_3,      //表格中的td,th
    Table_2,    //表格中的tr标签
    Table_1,     //表格中的caption，thead, tbody, tfooter
    Table       //table
};

typedef struct elemtype{    //节点元素类型
    std::string tag;       //标签内容
    std::string tag_name;  //标签名称
    HtmlElemClass type;    //标签类型
}elemtype;

#endif // NODE_H