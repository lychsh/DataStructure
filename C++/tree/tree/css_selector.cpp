#include"tree.h"
#include<stack>
#include <algorithm>
#include <cctype>
#include<set>
#include <unordered_set>
#include<windows.h>
#include <curl/curl.h>

//操作的类型
enum Operation{
    Wrong,    //错误指令
    Help,     //接受错误或空指令时输出帮助内容
    Exit,     //退出指令
    Read,      //读取文件
    Fetch,     //给定url获取网页内容
    Query,     //查询
    KQuery,    //对返回nodeslist表第k个node的子树查询
    Out,       //输出
    KOutInnerText,   //输出第k个node内嵌文本
    KOutHtml,        //输出第k个nodehtml代码块
    KOutHref,         //输出第k个node（a标签）的href属性
    List            //打印nodeslist列表
};


int Counter = 0;   //全局操作计数器
const int PrintSTagLen = 50;   //选择器得到列表输出标签最大长度, 超过时用填充符省略  
const std::string Padding  = "...";    //node标签内容超出最大长度时填充符


//字符串切割,返回切割出的子串的个数
int split_string(std::string str, char delimiter, std::vector<std::string> &result)
{
    result.clear();
    int start = 0, end, count = 0;
    int size = str.length();    //字符串长度
    while(start < size){
        if(str[start] == ' ' || str[start] == delimiter){    //跳过空格和分隔符
            start++;
            continue;
        }
        end = str.find_first_of(delimiter, start);   //子串结尾
        if(end < 0 || end >= size){                  //找到字符串结尾，说明只剩最后一个子串
            result.push_back(str.substr(start, size - start));   //加入子串
            count ++;
            break;
        }
        if(end != start){           //找到子串
            result.push_back(str.substr(start, end - start));   //加入子串
            start= end + 1;     //继续切割剩余子串
            count ++;
        }
        else{
            start ++;
        }
    }
    return count;    //返回子串个数
}



//通过属性名匹配结点
bool match_by_attrname(CSTree node, std::string attrname)
{
    if(!node){     //node结点为空，直接返回
        return false;
    }
    for (const auto& attr: node->data.attrs){   //遍历属性表比较
        if(attr.name == attrname){       //如果属性名相同,返回真
            return true;
        }
    }  
    return false; 
}

//通过属性值子串匹配结点
bool match_by_subvalue(CSTree node, std::string attrname, std::string subvalue)
{
    if(!node){     //node结点为空，直接返回
        return false;
    }
    for (const auto& attr: node->data.attrs){   //遍历属性表比较
        if(attr.name == attrname && attr.value.find(subvalue) != -1){       //如果属性相同,返回真
            return true;
        }
    } 
    return false;  
}

//通过属性值子串匹配结点
bool match_by_subattrvalues(CSTree node, std::string attrname, std::vector<std::string> subvalues)
{
    if(!node){     //node结点为空，直接返回
        return false;
    }
    for (const auto& attr: node->data.attrs){   //遍历属性表比较
        if(attr.name == attrname){  //找到属性
            for (const auto& subval: subvalues){   //依次比较子串
                if(attr.value.find(subval) == -1){     //子串不存在返回假
                    return false;
                }
            }
            return true;   //所有子串均存在返回真
        }
    } 
    return false;  
}

//通过属性匹配结点
bool match_by_attr(CSTree node, std::string attrname, std::string attrvalue)
{
    if(!node){     //node结点为空，直接返回
        return false;
    }
    //属性名和值都匹配成功返回真，否则返回假
    for (const auto& attr: node->data.attrs){   //遍历属性表比较
        if(attr.name == attrname && attr.value == attrvalue){  //属性名和属性值都相同，返回真
            return true;   
        }
    }
    return false;  
}


//判断pattern是不是target的前缀
bool is_prefix(std::string target, std::string pattern)
{
    int size1 = target.size(), size2 = pattern.size();
    for(int i = 0; i < size1 && i < size2; i++){
        if(target[i] != pattern[i]){
            return false;
        }
    }
    return true;
}

//判断pattern是不是target的后缀
bool is_suffix(std::string target, std::string pattern)
{
    int size1 = target.size(), size2 = pattern.size();
    if(size2 > size1){return false;}
    for(int i = size1 - 1, j = size2 - 1; i >= 0 && j >= 0; i--, j--){
        if(target[i] != pattern[j]){
            return false;
        }
    }
    return true;
}


//通过属性值前缀匹配
bool match_by_valueprefix(CSTree node, std::string attrname, std::string valueprefix)
{
    if(!node){     //node结点为空，直接返回
        return false;
    }
    for (const auto& attr: node->data.attrs){   //遍历属性表比较
        //如果属性值以valueprefix开头，返回真
        if(attr.name == attrname && is_prefix(attr.value, valueprefix)){  
            return true;   
        }
    }
    return false;  
}

//通过属性值后缀匹配
bool match_by_valuesuffix(CSTree node, std::string attrname, std::string valuesuffix)
{
    if(!node){     //node结点为空，直接返回
        return false;
    }
    for (const auto& attr: node->data.attrs){   //遍历属性表比较
        //如果属性值以valuesuffix结尾，返回真
        if(attr.name == attrname && is_suffix(attr.value, valuesuffix)){  
            return true;   
        }
    }
    return false;  
}



//通过标签名匹配结点
bool match_by_tagname(CSTree node, std::string tagname)
{
    if(!node){     //node结点为空，直接返回
        return false;
    }
    if(node->data.tag_name == tagname || tagname == "*"){       //如果属性名相同或则是通配符,返回真
        return true;
    }  
    return false; 
}

//通过标签和属性匹配结点
bool match_by_tag_attr(CSTree node, std::string tag, std::string attrname, std::string attrvalue)
{
    if(!node){     //node结点为空，直接返回
        return false;
    }
    //全部都匹配成功返回真，否则返回假
    if (match_by_tagname(node, tag) &&
        match_by_attr(node, attrname, attrvalue)){
            return true;
        }
    return false;  
}



//属性选择
void select_by_attr(CSTree T, std::string attrname, std::string attrvalue, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    if(!T){     //树空直接返回
        return ;
    }
    if(!tree_search){       //非树搜索，只对根结点访问
        if(match_by_attr(T, attrname, attrvalue)){   //单独判断根结点   
            nodes_set.insert(T);  
        }
        return ;
    }
    CSTree node = T->firstchild;
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            if(match_by_attr(node, attrname, attrvalue)){   //匹配节点   
                nodes_set.insert(node);    //加入新结点
            }
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
}

//属性名选择器
void select_by_attrname(CSTree T, std::string attrname, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    if(!T){     //树空直接返回
        return ;
    }
    if(!tree_search){       //非树搜索，只对根结点访问
        if(match_by_attrname(T, attrname)){   //单独判断根结点   
            nodes_set.insert(T);    
        }
        return ;
    }
    CSTree node = T->firstchild;
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            if(match_by_attrname(node, attrname)){   //匹配节点   
                nodes_set.insert(node);    //加入新结点
            }
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
}

//标签选择器
void select_by_tag(CSTree T, std::string tag, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    if(!T){     //树空直接返回
        return ;
    }
    if(!tree_search){       //非树搜索，只对根结点访问
        if(match_by_tagname(T, tag)){   //单独判断根结点    
            nodes_set.insert(T);  
        }
        return ;
    }
    CSTree node = T->firstchild;
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            if(match_by_tagname(node, tag)){   //匹配节点   
                nodes_set.insert(node);    //加入新结点
            }
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
}



//标签属性选择器
void select_by_tag_attr(CSTree T, std::string tag, std::string attrname,
                         std::string attrvalue, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    if(!T){     //树空直接返回
        return ;
    }
    if(!tree_search){       //非树搜索，只对根结点访问
        if(match_by_tag_attr(T, tag, attrname, attrvalue)){   //单独判断根结点      
            nodes_set.insert(T);   
        }
        return ;
    }
    CSTree node = T->firstchild;
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            if(match_by_tag_attr(node, tag, attrname, attrvalue)){   //匹配节点   
                nodes_set.insert(node);    //加入新结点
            }
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
}

//多属性值选择器
void select_by_attr_multivalue(CSTree T, std::string attrname, std::vector<std::string> subvalues, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    if(!T){     //树空直接返回
        return ;
    }
    if(!tree_search){       //非树搜索，只对根结点访问
        if(match_by_subattrvalues(T, attrname, subvalues)){   //先判断根结点  
            nodes_set.insert(T);    
        }
        return ;
    }
    CSTree node = T->firstchild;    //在左子树上进行查找
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            if(match_by_subattrvalues(node, attrname, subvalues)){   //匹配节点   
                nodes_set.insert(node);    //加入新结点
            }
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
}


//元素多属性值选择器
void select_by_tag_attr_multivalue(CSTree T, std::string tag_name, std::string attrname, std::vector<std::string> subvalues, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    if(!T){     //树空直接返回
        return ;
    }
    if(!tree_search){       //非树搜索，只对根结点访问
        if(match_by_tagname(T, tag_name) && match_by_subattrvalues(T, attrname, subvalues)){   //先判断根结点  
            nodes_set.insert(T);    
        }
        return ;
    }
    CSTree node = T->firstchild;    //在左子树上进行查找
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            if(match_by_tagname(node, tag_name) && match_by_subattrvalues(node, attrname, subvalues)){   //匹配节点   
                nodes_set.insert(node);    //加入新结点
            }
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
}


//单属性值子串选择器
void select_by_attr_subvalue(CSTree T, std::string attrname, std::string subvalue, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    if(!T){     //树空直接返回
        return ;
    }
    if(!tree_search){       //非树搜索，只对根结点访问
        if(match_by_subvalue(T, attrname, subvalue)){   //先判断根结点  
            nodes_set.insert(T);      
        }
        return ;
    }
    CSTree node = T->firstchild;    //在左子树上进行查找
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            if(match_by_subvalue(node, attrname, subvalue)){   //匹配节点   
                nodes_set.insert(node);    //加入新结点
            }
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
}

//根据属性值的开头（后接-符或者就是完整单词）选择
void select_by_headvalue(CSTree T, std::string attrname, std::string headvalue, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    if(!T){     //树空直接返回
        return ;
    }
    if(!tree_search){       //非树搜索，只对根结点访问
        if(match_by_valueprefix(T, attrname, headvalue + "-") || match_by_attr(T, attrname, headvalue)){   //先判断根结点  
            nodes_set.insert(T);   
        }
        return ;
    }
    CSTree node = T->firstchild;    //在左子树上进行查找
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            //如果属性值以headvalue-为前缀或者就是headvalue
            if(match_by_subvalue(node, attrname, headvalue + "-") || match_by_attr(node, attrname, headvalue)){     
                nodes_set.insert(node);    //加入新结点
            }
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
}


//根据属性值的前缀选择
void select_by_prefix(CSTree T, std::string attrname, std::string valueprefix, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    if(!T){     //树空直接返回
        return ;
    }
    if(!tree_search){       //非树搜索，只对根结点访问
        if(match_by_valueprefix(T, attrname, valueprefix)){   //先判断根结点  
            nodes_set.insert(T);    
        }
        return ;
    }
    CSTree node = T->firstchild;    //在左子树上进行查找
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            //如果属性值以headvalue-为前缀或者就是headvalue
            if(match_by_subvalue(node, attrname, valueprefix)){     
                nodes_set.insert(node);    //加入新结点
            }
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
}

//根据属性值后缀进行查找
void select_by_suffix(CSTree T, std::string attrname, std::string valuesuffix, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    if(!T){     //树空直接返回
        return ;
    }
    if(!tree_search){       //非树搜索，只对根结点访问
        if(match_by_valuesuffix(T, attrname, valuesuffix)){   //先判断根结点  
            nodes_set.insert(T);    
        }
        return ;
    }
    CSTree node = T->firstchild;    //在左子树上进行查找
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            //如果属性值以headvalue-为前缀或者就是headvalue
            if(match_by_valuesuffix(node, attrname, valuesuffix)){     
                nodes_set.insert(node);    //加入新结点
            }
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
}

//id属性选择器
void id_selector(CSTree T, std::string select, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    std::string attrvalue;
    int start = select.find_first_not_of(" #", 0);
    int end = select.find_first_of(" ", start);        //属性值结尾
    attrvalue = select.substr(1, end - start);         //获取属性值

    //在T树进行查找
    select_by_attr(T, "id", attrvalue, nodes_set, tree_search);
}

//标签选择器
void tag_selector(CSTree T, std::string tag_name, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    //从T树上查找
    select_by_tag(T, tag_name, nodes_set, tree_search);
}

//标签class属性选择器
void tag_class_selector(CSTree T, std::string select, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    std::vector<std::string> tag_class;   //tag和class属性值
    //获取标签和class
    int end = select.find_first_of(".", 0);
    std::string tag_name = select.substr(0, end);
    std::vector<std::string> subvalues;
    split_string(select.substr(end + 2, select.length() - end), '.', subvalues);

    //从T树上查找
    select_by_tag_attr_multivalue(T, tag_name, "class", subvalues, nodes_set, tree_search);     
}


//class属性选择器
void class_selector(CSTree T, std::string select, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    std::vector<std::string> subvalues;
    int num = split_string(select, '.', subvalues);   //按点进行切割
    if(num <= 0){       //select为空，直接返回
        return ;
    }
    //从T树上查找
    select_by_attr_multivalue(T, "class", subvalues, nodes_set, tree_search);
}



//子选择器， element1>element2，选择element1的直接子标签element2
void child_selector(std::unordered_set<CSTree> &nodes_set)
{
    std::unordered_set<CSTree> tmpset;
    for(auto &node: nodes_set){   //匹配每个node的相邻兄弟元素
        if(node->firstchild){
            tmpset.insert(node->firstchild);
        }
    }
    swap(tmpset, nodes_set);     //更新集合
}

//相邻兄弟选择器
void adjacen_sibling_selector(std::unordered_set<CSTree> &nodes_set)
{
    std::unordered_set<CSTree> tmpset;
    for(auto &node: nodes_set){   //匹配每个node的相邻兄弟元素
        if(node->nextsibling){
            tmpset.insert(node->nextsibling);
        }
    }
    swap(tmpset, nodes_set);     //更新集合
}

//兄弟选择器
void sibling_selector(std::unordered_set<CSTree> &nodes_set)
{
    std::unordered_set<CSTree> tmpset;
    CSTree node;
    for(auto &n: nodes_set){   //匹配每个node的相邻兄弟元素
        node = n->nextsibling;
        while(node){
            tmpset.insert(node);
            node = node->nextsibling;
        }
    }
    swap(tmpset, nodes_set);     //更新集合
}


//属性选择器
void attr_selector(CSTree T, std::string select, std::unordered_set<CSTree> &nodes_set, bool tree_search)
{
    std::string attrname, attrvalue;      // 属性名称和值
    int start, end;
    start = select.find_first_not_of("[ ", 0);
    end = select.find_first_of("=~|^]$ ");
    if(start <= 0 || end <= 0){           //如果选择器表达式不完整
        std::cout << "\033[32mOut[" << Counter << "]: \033[0mselector expression is incomplete" << std::endl;
        nodes_set.clear();                //清空集合，返回
        return ;
    }
    attrname = select.substr(start, end - start);           //获取属性名
    
    start = select.find_first_of("=~|^&$]", start);
    char s = select[start];
    start = select.find_first_not_of(" = ~|^&$", start);    //获取属性值
    if(start > 0){
        end = select.find_first_of(" ]", start);
        attrvalue = select.substr(start, end - start);
    }
    // 根据属性选择器类型进行查询
    if(s == ']'){               //根据属性名查找
        select_by_attrname(T, attrname, nodes_set, tree_search);
    }
    else if(s == '='){          //根据属性名和属性值查找
        select_by_attr(T, attrname, attrvalue, nodes_set, tree_search);
    }
    else if(s == '~'){          //选择属性值子串查找
        select_by_attr_subvalue(T, attrname, attrvalue, nodes_set, tree_search);
    }
    else if(s == '|'){          //根据属性值的开头（后接-符或者就是完整属性值）查找
        select_by_headvalue(T, attrname, attrvalue, nodes_set, tree_search);
    }  
    else if(s == '^'){          //根据属性值前缀查找
        select_by_prefix(T, attrname, attrvalue, nodes_set, tree_search);
    }
    else if(s == '$'){          //根据属性值后缀查找
        select_by_suffix(T, attrname, attrvalue, nodes_set, tree_search);
    }
    else {
        std::cout <<  "\033[32mOut[" << Counter << "]:\033[0m " << "\033[31mthe selector of child_selecotr should be '[attribute (operation) (attrvaue)]. operation: =~|^&\033[0m" << std::endl;
        return ;
    }

}


//查询函数，根据css选择器规则进行查询，按逗号选择器并列查询
void query(CSTree T, std::string selector, std::vector<CSTree> &nodeslist)
{
    if(!T){
        std::cout << "\033[32mOut[" << Counter << "]: \033[0m" << "\033[31mWarning: Haven't open file or fetch url\033[0m" << std::endl;
        return ;
    }
    std::vector<std::string> total_selectors;       //并列选择表,以逗号分隔
    std::vector<std::string> selectors;             //选择器表
    std::unordered_set<CSTree> tmpset, nodes_set;
    bool tree_search = true;                        //是否在整个树上查找
    nodes_set.insert(T);                            //插入根节点
    int total_num = split_string(selector, ',', total_selectors);     //先用逗号切割得到并列选择表
    for(int i = 0; i < total_num; i ++){                       //对并列表查询
        int num = split_string(total_selectors[i], ' ', selectors);      //分别用空格切割得到选择器表
        for(int j = 0; j < num; j++){                          //对选择器表查询
            // 如果是子选择器，兄弟选择器或者相邻兄弟选择器，更新node集合，下一次查找只对子树根结点查找
            if(selectors[j] == ">"){         //子选择器
                child_selector(nodes_set);
                tree_search = false;
                continue;
            }
            else if(selectors[j] == "+"){         //相邻兄弟选择器
                adjacen_sibling_selector(nodes_set);
                tree_search = false;
                continue;
            }
            else if(selectors[j] == "~"){         //兄弟选择器
                sibling_selector(nodes_set);
                tree_search = false;
                continue;
            }
            //如果是其他类型选择器，在整个树上查找
            for(const CSTree& node: nodes_set){
                if(selectors[j][0] == '.'){                       //class选择器
                    class_selector(node, selectors[j], tmpset, tree_search);
                }
                else if(selectors[j].find("[", 0) != -1){         //属性选择器
                    attr_selector(node, selectors[j], tmpset, tree_search);
                }
                else if(selectors[j][0] == '#'){                  //id选择器
                    id_selector(node, selectors[j], tmpset, tree_search);
                }
                else if(selectors[j] == "*"){                     //通配符选择器
                    tag_selector(node, "*", tmpset, tree_search);
                }
                else if(selectors[j].find(".", 0) != -1){         //标签class选择器
                    tag_class_selector(node, selectors[j], tmpset, tree_search);
                }
                else {                                      //标签选择器
                    tag_selector(node, selectors[j], tmpset, tree_search);
                }
            }
            tree_search = true;
            swap(tmpset, nodes_set);                      //更新nodes集合
            tmpset.clear();                               //清空缓存集合
        }
        // 将 set 中的元素复制到node列表中
        for(const CSTree& node: nodes_set){
            nodeslist.push_back(node);
        }
        nodes_set.clear();              //清空集合
        nodes_set.insert(T);            //压入根结点
    }
    // 按照在html中的先后顺序排序
    std::sort(nodeslist.begin(), nodeslist.end(), 
              [](const CSTree& a, const CSTree& b) {
                  return a->data.start < b->data.start;
              });
}


// 回调函数：处理网页内容并将其存储到 html 字符串中
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    std::string* html = reinterpret_cast<std::string*>(userp);
    html->append(reinterpret_cast<char*>(contents), total_size);
    return total_size;
}

// 获取网页内容的函数
bool fetch_url(std::string &html, const std::string& url) {
    CURL* curl = nullptr;
    CURLcode res;
    
    // 初始化 libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (curl) {
        // 设置要访问的 URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        // 设置回调函数来获取网页内容
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &html);
        
        // 执行请求
        res = curl_easy_perform(curl);
        // 检查请求是否成功
        if (res != CURLE_OK) {
            html.clear();  // 如果请求失败，返回空字符串
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return false;
        }
        // 清理 curl
        curl_easy_cleanup(curl);
    }
    // 清理 libcurl
    curl_global_cleanup();
    //预处理换行符
    int size = html.length();
    for(int i = 0; i < size; i++){
        if(html[i] == '\n' || html[i] == '\r'){
            html[i] = ' ';
        }
    }
    int start = html.find_first_of("<", 0);
    if(start > 0){       //去除多余字符串
        html = html.substr(start, html.length() - start);
    }
    //将网页源码转为小写
    std::transform(html.begin(), html.end(), html.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return true;  // 读取成功
}


//打印文本结点
void print_text(CSTree node)
{
    if(!node){    //空节点不打印
        return ;
    }
    if(node->data.type == Text){    //打印文本
        std::cout<< " " << node->data.tag_name << " ";
    }
}

//先序遍历，获取标签内的所有文本
void out_inner_text(CSTree T)
{
    if(!T){      //空结点返回
        return ;
    }
    std::cout << "\033[32mOut[" << Counter << "]: \033[0m" << std::endl;
    std::cout << "[" ;
    PreOrderTraverse(T->firstchild, print_text);    //先序遍历左子树输出文本
    std::cout << "]" << std::endl << std::endl;
} 


// //打印开始标签、自闭合标签和文本标签
// void print_tag(CSTree node)
// {
//     TagType type = node->data.type;
//     if(type == Text){    //文本直接打印
//         std::cout << node->data.tag_name << std::endl;
//     }
//     else if(type == StartTag || type == Single){    //如果是自闭合标签或者开始标签
//         std::cout << "<" << node->data.tag_name;    //打印标签名
//         for(const auto& attr: node->data.attrs){    //依次打印标签属性
//             std::cout << " " << attr.name << "=\"" << attr.value << "\"";
//         }
//         std::cout << ">" ;      //打印>符号
//     }
//     else{   //未知标签的打印
//         std::cout << "<! UNKNOW...>" ;
//     }
// }

//输出html块
void out_html(CSTree T, std::string html)
{
    if(!T){    //空节点返回
        return ;
    }
    int start, end;
    //tag块起始和终点位置
    start = T->data.start;     
    end = T->data.end;
    std::string out = html.substr(start, end - start + 1);   //获取tag子串
    std::cout << "\033[32mOut[" << Counter << "]: \033[0m" << std::endl;
    std::cout << "[ " << out <<  " ]" << std::endl << std::endl;                 //打印tag块
    /*
    //如果结点没有存储位置信息
    std::vector<CSTree> tags;    //输出标签辅助栈
    CSTree node;
    print_tag(T);     //输出最外层标签
    if(T->firstchild) {    //如果T有左子树，将左子树根压入栈中
        tags.push_back(T->firstchild);
    }
    while(!tags.empty()){
        node = tags.back();   //弹出栈顶
        tags.pop_back();
        while(node){          //向左依次打印标签
            print_tag(node);
            tags.push_back(node);  //打印过的标签入栈
            node = node->firstchild;
        }
        node = tags.back();     //弹出栈顶
        tags.pop_back();
        if(node->data.type == StartTag){    //如果是开始标签
            std::cout << "<" << node->data.tag_name << "/>" << std::endl;   //输出结束标签
        }
        tags.push_back(node->nextsibling);   //向右走一步
    }
    if(T->data.type == StartTag){     //如果最外层标签是开始标签，打印结束标签
        std::cout << "<" << T->data.tag_name << "/>" << std::endl;   //输出最外层结束标签
    }
    */
}

//获取a标签的href
void out_href(CSTree node)
{
    if(!node){    //空节点直接返回
        return ;
    }
    if(node->data.tag_name != "a"){     //如果不是a标签，打印错误，返回 
        std::cout << "\033[32mOut[" << Counter << "]:\033[0m " << "\033[31mOnly tag<a> has attribute 'href'\033[0m" << std::endl;
        return ;
    }  
    for(const auto&attr: node->data.attrs){    //遍历属性表，如果有href属性则打印
        if(attr.name == "href"){
            std::cout << "\033[32mOut[" << Counter << "]:\033[0m [ " << attr.value << " ]" << std::endl;    //输出href
            return ;
        }
    }
    std::cout << "\033[32mOut[" << Counter << "]:\033[0m " << "\033[31mThe element <" << node->data.tag_name << "> has no attribute 'href'\033[0m" << std::endl;
}

//打印选择器得到的nodeslist表
void print_nodeslist(std::vector<CSTree> &nodeslist, std::string &html)
{
    int start, len;   //标签开始和长度
    int n = nodeslist.size() - 1;   
    std::cout << std::endl << "[" ;
    for(int i = 0; i < n; i++){     //输出前n-1个node
        start = nodeslist[i]->data.start;
        len = nodeslist[i]->data.end - start + 1;  //标签长度
        if(len > PrintSTagLen){    //标签长度超过限制，使用padding表示省略 
            std::cout << i << " |" << html.substr(start, PrintSTagLen) << Padding << ", " << std::endl;
        }   
        else{    //否则直接输出
            std::cout << i << " |" << html.substr(start, len) << "," << std::endl;
        }
    }
    if(n >= 0){       //输出最后一个node结点标签
        start = nodeslist[n]->data.start;
        len = nodeslist[n]->data.end - start + 1;  //标签长度
        if(len > PrintSTagLen){    //标签长度超过限制，使用padding表示省略 
            std::cout << n << " |" << html.substr(start, PrintSTagLen) << Padding << "]" << std::endl;
        }   
        else{    //否则直接输出
            std::cout << n  << " |" << html.substr(start, len) << "]" << std::endl;
        }
        std::cout << std::endl;
    }
    else{
        std::cout << " ]" << std::endl << std::endl;
    }
}


//根据输入的操作字符串返回对应操作类型
Operation get_command(std::string command)
{
    // 使用 std::transform 将每个字符转换为小写
    std::transform(command.begin(), command.end(), command.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    Operation opt;     //操作类型
    if(command == "list"){
        opt = List;
    }
    else if(command == "query"){
        opt = Query;
    }
    else if(command == "read"){
        opt = Read;
    }
    else if(command == "fetch"){
        opt = Fetch;
    }
    else if(command == "out"){
        opt = Out;
    }
    else if(command == "innertext"){
        opt = KOutInnerText;
    }
    else if(command == "outhtml"){
        opt = KOutHtml;
    }
    else if(command == "href"){
        opt = KOutHref;
    }
    else if(command == "exit"){
        opt = Exit;
    }
    else if(command == "help"){
        opt = Help;
    }
    else{
        opt = Wrong;
    }
    return opt;
}

//解析输入命令
void parser_command(std::string command, int &k, Operation &opt, std::string &path, std::string &selector)
{
    std::string operation1, operation2, strk;    //操作和数字k的字符串
    int size = command.length(), start = 0, end;
    //获取第一个操作
    start = command.find_first_not_of(" .([", 0);
    end = command.find_first_of("([", start);
    if(end < 0) {end = size; }
    operation1 = command.substr(start, end - start);
    opt = get_command(operation1);              //指令一类型
    if(opt == Wrong){                   //如果操作1错误，打印错误并返回
        std::cout << "\033[32mOut[" << Counter << "]:\033[0m " << "\033[31mNameError: name '" << operation1 <<  "' is not defined\033[0m" << std::endl;
        return ;
    }
    if(opt == Read || opt == Fetch){    //如果是read命令，获取path或者url
        start = command.find_first_not_of(" (", end);
        end = command.find_first_of(") ", start + 1);
        path = command.substr(start, end - start);
        return ;
    }
    else if(opt == Query){              //如果是query命令，获取selector
        start = command.find_first_not_of(" (", end);
        end = command.find_first_of(")", start + 1);
        selector = command.substr(start, end - start);
        return ;
    }
    else  if(opt == Out){               //command是out[k]类型指令
        //获取k值
        start = command.find_first_not_of("[ ", end);
        end = command.find_first_of( "] ", start + 1);
        strk = command.substr(start, end - start);
        //如果strk不是数字或者为空，返回错误
        if(strk.empty() || !std::all_of(strk.begin(), strk.end(), ::isdigit)){   
            opt = Wrong;
            std::cout << "\033[32mOut[" << Counter << "]:\033[0m " <<"\033[31mKError: name'" << strk <<  "'K is not a number\033[0m" << std::endl;
            return ;
        }
        k = std::stoi(strk);     //将字符串转为数字
        if(command.find("(", start) != -1){       //如果最后一个字符是)，则是KQuery指令
            start = command.find_first_not_of(".", end + 1);
            end = command.find_first_of("(", start);
            operation2 = command.substr(start, end - start);   //获取第二个指令
            opt = get_command(operation2);
            if(opt == Query){    //如果是Query指令,获取selector
                opt = KQuery;
                start = end + 1; 
                end = command.find_first_of(")", start);
                selector = command.substr(start, end - start);
            }
        }
        else{                             //否则是其他out指令
            start = end + 2;
            operation2 = command.substr(start, size - start);   //获取第二个指令
            opt = get_command(operation2);
        }
    }
    if(opt == Wrong){           //如果操作2错误，打印错误
        std::cout << "\033[32mOut[" << Counter << "]:\033[0m " << "\033[31mNameError: name '" << operation2 <<  "' is not defined\033[0m" << std::endl;
    }
}


//输出使用说明
void PrintUsage()
{
    std::cout << std::endl;
    std::cout << "Selector Usage:" << std::endl; 
    std::cout << "\t" << "<command> (args)" << std::endl;
    std::cout << "or:" << std::endl;
    std::cout << "\t" << "Out [options]. <command> (args)" << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "[s]\t" << "exit()            Exit program" << std::endl;       
    std::cout << "[s]\t" << "read(path)        Read the html file from the path" << std::endl;
    std::cout << "[s]\t" << "fetch(url)        Get the web content from the url"<< std::endl;            
    std::cout << "[s]\t" << "query(selector)   Choose element by seletor expression, you can see more details in https://www.w3school.com.cn/cssref/css_selectors.asp"<< std::endl;
    std::cout << "[s]\t" << "list              Output a list of query result" << std::endl;
    std::cout << "[s]\t" << "out[k].command    Execute commands on No.k element of query elements list, which commands include query and the following:"<< std::endl;
    std::cout << "[s]\t" << "innertext         Output the nested text content of No.k element"<< std::endl;
    std::cout << "[s]\t" << "html              Output the whole html code of No.k element"<< std::endl;
    std::cout << "[s]\t" << "href              Output the 'href' attribute of tag<a>"<< std::endl << std::endl;
}



//交互程序
void interact()
{
    std::vector<CSTree> nodeslist;      //存放query匹配到的的结点列表
    std::vector<Tag> tags;              //存放标签顺序表
    CSTree T = nullptr, root;           //html树，子树
    std::string command;                //用户指令
    int k = -1;                         //out指令中的下标
    Operation operation;                //指令类型
    std::string path_url, html, selector;    //文件路径或url地址、html字符串，选择器表达式
    while(operation != Exit){   //exit指令退出
        Counter ++;             //控制全局命令交互行数
        std::cout << "\033[32mIn [" << Counter << "]:\033[0m " ;
        std::getline(std::cin, command);      //输入指令
        parser_command(command, k, operation, path_url, selector);     //解析指令
        switch (operation)      //根据指令执行操作
        {
        case Wrong:             //错误指令不执行
            break;
        case Help:              //打印使用说明
            PrintUsage();   
            break; 
        case Read:              //读取文件
            if(read_file(html, path_url)){    //成功读取
                std::cout<< "\033[32mOut[" << Counter << "]:\033[0m " << "Sucessfully read this file" << std::endl;
                if(T){                      //如果树已经存在，先销毁树
                    destroy_tree(T);
                    tags.clear();           //清空tags数组
                }
                split_tags(html, tags);    //切割标签
                init_tree(T);              //初始化树 
                create_tree(T, tags);      //构造html树
            }
            else{           //读取失败
                std::cout << "\033[32mOut[" << Counter << "]:\033[0m \033[31mCan't open this file" << std::endl;
                std::cout << "          Check for spelling mistakes in the path and whether the file exists\033[0m" << std::endl;
            }
            break;
        case Fetch:       //获取网页内容
            html = "";
            if(fetch_url(html, path_url)){   //获取成功
                std::cout<< "\033[32mOut[" << Counter << "]:\033[0m " << "Sucessfully fetch this url" << std::endl;
                if(T){                      //如果树已经存在，先销毁树
                    destroy_tree(T);
                    tags.clear();           //清空tags数组    
                }
                split_tags(html, tags);    //切割标签
                init_tree(T);              //初始化树 
                create_tree(T, tags);      //构造html树
            }
            else{       //获取失败
                std::cout << "\033[32mOut[" << Counter << "]:\033[0m \033[31mCan't fetch this url " << std::endl;
                std::cout << "          Check for access authority to the url and whether the url exists\033[0m" << std::endl;
            }
            break; 
        case Query:         //选择器指令
            nodeslist.clear();    //清空上一次结果
            query(T, selector, nodeslist);
            std::cout << "\033[32mOut[" << Counter << "]:\033[0m ";
            print_nodeslist(nodeslist, html);     //打印结果
            break;
        case KQuery:        //第k个node执行选择器
            if(k < 0 || k >= nodeslist.size()){    //k越界
                std::cout << "\033[32mOut[" << Counter << "]:\033[0m  \033[31mk is among wrong range \033[0m" << std::endl;
                break;
            }
            root = nodeslist[k];
            nodeslist.clear();              //清空上一次结果
            query(root, selector, nodeslist);
            std::cout << "\033[32mOut[" << Counter << "]:\033[0m ";
            print_nodeslist(nodeslist, html);     //打印结果
            break;
        case KOutHtml:      //输出html代码
            if(k < 0 || k >= nodeslist.size()){    //k越界
                std::cout << "\033[32mOut[" << Counter << "]:\033[0m  \033[31mk is among wrong range \033[0m" << std::endl;
                break;
            }
            if(html == ""){     //如果没有读取到文件或者返回url内容
                std::cout << "\033[32mOut[" << Counter << "]:\033[0m  \033[31mHaven't open file or fetched url \033[0m" << std::endl;
                break;
            }

            out_html(nodeslist[k], html);
            break;
        case KOutInnerText:   //输出标签内文本
            if(k < 0 || k >= nodeslist.size()){    //k越界
                std::cout << "\033[32mOut[" << Counter << "]:\033[0m  \033[31mk is among wrong range \033[0m" << std::endl;
                break;
            }
            out_inner_text(nodeslist[k]);
            break;
        case KOutHref:      //输出第k个tag标签的href属性
            if(k < 0 || k > nodeslist.size()){    //k越界
                std::cout << "\033[32mOut[" << Counter << "]:\033[0m  \033[31mk is among wrong range \033[0m" << std::endl;
                break;
            }
            out_href(nodeslist[k]);   
            break;
        case List:        //打印nodeslist列表
            if(html == ""){     //如果没有读取到文件或者返回url内容
                std::cout << "\033[32mOut[" << Counter << "]:\033[0m  \033[31mHaven't read file or fetch url \033[0m" << std::endl;
                break;
            }
            std::cout << "\033[32mOut[" << Counter << "]:\033[0m ";
            print_nodeslist(nodeslist, html);
            break;
        default:
            break;
        }
    }
    destroy_tree(T);   //销毁树
}


int main()
{
    PrintUsage();      //打印使用说明
    interact();         //CSS选择器交互实现
    return 0;
}