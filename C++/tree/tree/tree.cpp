#include<stdio.h>
#include"tree.h"
#include<sstream>
#include <fstream>
#include<queue>
#include<stack>

//初始化空树
void init_tree(CSTree &T)
{
    if(T){   //如果树已经存在,销毁树
        destroy_tree(T);
    }
    CSTree NT = new CSNode;
    if(!NT){   //如果申请新根节点失败
        std::cerr << "Init Error: 初始化失败" << std::endl;
        return ;
    }
    NT->firstchild = nullptr;
    NT->nextsibling = nullptr;
    T = NT;     //申请成功
}

//根据tag标签字符串创建树结点
void create_node(CSTree &node, Tag tag)
{
    if(!node){    //如果节点不存在，创建新结点
        init_tree(node);
    }
    if(tag.content[0] != '<'){     //如果是文本，tag_name就是tag
        node->data.tag_name = tag.content;
        node->data.type = Text;
        return ;
    }
    size_t size = tag.content.length(), start, end;
    std::string tmp;
    TagAttr attr;
    node->data.type = tag.type;
    //先获取标签名s
    start = tag.content[1] == '/' ? 2: 1;
    end = tag.content.find_first_of(" >", start);
    tmp = tag.content.substr(start, end - start);
    node->data.tag_name = tmp;
    start = end + 1;
    //依次获取标签属性
    while(start < size && end < size){
        if(tag.content[start] == ' '){     //跳过空格
            start++;
            continue;
        }
        end = tag.content.find_first_of("=>", start);
        if(end != start){
            attr.name = tag.content.substr(start, end - start);   //获取属性名
            start = end + 2;
            end = tag.content.find_first_of("\">", start);
            if(end > size) break;
            attr.value = tag.content.substr(start, end - start);   //获取属性值
            node->data.attrs.push_back(attr);
        } 
        start = end + 1;
    }
}

//后序遍历销毁树
void destroy_tree(CSTree &T)
{
    if(!T){     //树不存在直接返回
        return ;
    }
    destroy_tree(T->firstchild);    //销毁左子树
    destroy_tree(T->nextsibling);   //销毁右子树
    delete T;                       //销毁根节点
    T = nullptr;
}

//判断是否是自闭合标签
bool is_single_tag(std::string tag_name)
{
    for (const auto& elem : SINGLE_TAGs) {
        if(tag_name == elem){    //是自闭合标签
            return true;
        }
    }
    return false;   //不是自闭合标签
}

//将文件路径中的转义字符/改为//
void transfer_path(std::string &path)
{   
    size_t pos = 0;
    while((pos = path.find('\\', pos)) != std::string::npos) {
        path.replace(pos, 1, "\\\\");
        pos += 2; // 因为替换后的字符串长度增加了，所以需要跳过两个字符
    }
}

//读取文件
bool read_file(std::string &html, std::string path)
{
    transfer_path(path);       //获取正确路径
    std::ifstream file(path);
    if(!file.is_open()){     //无法打开文件返回假
        return false;
    }
    std::stringstream buffer;  //文件字节流
    //读取文件内容
    buffer << file.rdbuf();
    html = buffer.str();
    file.close();
    //预处理换行符
    int size = html.length();
    for(int i = 0; i < size; i++){
        if(html[i] == '\n' || html[i] == '\t'){
            html[i] = ' ';
        }
    }
    return true;
}

//移除多余空格
std::string remove_extraspace(std::string tag, size_t len)
{
    std::string subtag;
    size_t start = 0, end = 0, size = len - 1;
    if(tag[0] == '<'){   //标签去除空格
        if(tag[1] == '/'){   //如果是后标签
            start = 2;
            subtag = "</";
        }    
        else{             //如果是前标签
            start = 1;
            subtag = '<';
        }   
        while(start < size){
            if(tag[start] == ' '){     //跳过多余空格
                start ++;
                continue;
            }
            end = tag.find_first_of("> ", start);
            if(tag[end] == ' '){     //没有到闭合处>
                subtag += tag.substr(start, end - start + 1);    //加上标签一部分和一个空格
                start = end + 1;
            }
            else{             
                subtag += tag.substr(start, end - start);
                break;
            }
        }
        if(tag[len - 2] == ' '){
            len = subtag.length();
            subtag[len - 1] = '>';
        }
        else{
            subtag += ">";
        }
    }
    else{    //如果是文本
        while(start < size && end < size){
            if(tag[start] == ' '){     //跳过多余空格
                start ++;
                continue;
            }
            end = tag.find_first_of("> ", start);
            subtag += tag.substr(start, end - start + 1);    //加上标签一部分和一个空格
            start = end + 1;
        }
    }
    return subtag;
}

//从html字符串切割出tag标签数组
void split_tags(std::string html, std::vector<Tag> &tags)
{
    size_t size = html.length();
    size_t start = 0, end, len, start1, end1;
    Tag tag;
    std::string tag_name;
    TagType type;
    //遍历切词
    while(start < size){
        if(html[start] == ' '){      //遇到空格跳过
            start += 1;
            continue;
        }
        //读取标签和文本
        end = html.find_first_of("<", start);   //查找tag开始位置
        if(end == start){        //如果找到标签起始
            end = html.find_first_of(">", start);  //找到标签结尾
            len = end - start + 1;
            tag.content = html.substr(start, len);   //标签内容
            start1 = html[start + 1] == '/'? start + 2: start + 1;
            tag_name = html.substr(start1, html.find_first_of(" >", start1) - start1);
            tag.start = start;
            tag.end = end;
            //判断是否是过滤标签并跳过(<!-- --> or <script> or <style>)
            if(len > 6){
                if(tag.content[1] == '!' && tag.content[2] == '-'){
                    end = html.find("->", start);    //跳过注释
                    start = end + 2;
                    continue;
                }
                else if ((tag.content[1] == 's' && tag.content[2] == 'c' )){
                    end = html.find("script>", end + 1);  //找过滤块结束标签位置
                    start = end + 7;      //跳过过滤块
                    continue;
                }
                else if((tag.content[1] == 's' && tag.content[2] == 't' && tag.content[3] == 'y')){
                    end = html.find("style>", end + 1); //找过滤块结束标签位置
                    start = end + 6;      //跳过过滤块
                    continue;
                }
            }
            end1 = html.find("  ", start);
            if(end1 < end){     //tag中间有多个空格，去除多余空格
                tag.content = remove_extraspace(tag.content, len);
            }
            if(is_single_tag(tag_name)){
                tag.type = Single;
            }
            else if(tag.content[1] == '/'){
                tag.type = EndTag;
            }
            else{
                tag.type = StartTag;
            }
            tags.push_back(tag);
            start = end + 1;
        }
        else{      //找到文本
            end = html.find_first_of("<", start);   //词结束符位置
            len = end - start;
            tag.content = html.substr(start, len);
            end1 = html.find("  ", start);
            if(end1 < end){     //tag中间有多个空格，去除多余空格
                tag.content = remove_extraspace(tag.content, len);
            }
            tag.type = Text;
            tag.start = start;
            tag.end = end - 1;
            start = end;
            if(tag.content != ""){
                tags.push_back(tag);
            }
        }
    }
}

//构造树
void create_tree(CSTree &T, std::vector<Tag> &tags)
{
    int n = tags.size();
    Tag tag;
    TagType top_type;                //栈顶结点的类型
    CSTree top_node = nullptr;               //nodes栈的栈顶结点
    std::stack<CSTree> nodes;          //树的结点辅助栈
    std::stack<TagType> types;      //树的结点类型辅助栈
    bool lastend =false;      //标示上一个标签是否是结束标签
    if(tags[0].content[1] == '!' && tags[0].content[2] == 'D' || tags[0].content[2] == 'd'){    //<!DOCTYPE html>标签作为根节点
        T->data.type = WholeTag;
        T->data.end = tags[0].end;
        T->data.tag_name = tags[0].content.substr(1, tags[0].content.length() - 2);
        types.push(StartTag);
    }
    else{
        create_node(T, tags[0]);    //创建树的根
        types.push(tags[0].type);
    }
    T->data.start = tags[0].start;
    nodes.push(T);             //压入根结点
    int size = tags.size();
    for (int i = 1; i < size; i++){     //遍历剩余tag
        tag = tags[i];
        top_type = types.top();          //栈顶结点标签类型
        top_node = nodes.top();
        //如果当前结点是开始标签或自闭合标签或者标签块或者文本
        if(tag.type == StartTag || tag.type == Single || tag.type == WholeTag || tag.type == Text){    
            CSTree cur_node = nullptr;     //当前结点
            create_node(cur_node, tag);   //创建新结点 
            cur_node->data.start = tag.start;     
            if(nodes.empty()){     //如果未遍历完nodes栈已空，说明html有错误标签或未正确闭合
                std::cerr << "html Error: html存在错误" << std::endl;
                return ;
            }
            if(top_type == StartTag){   //栈顶标签是开始标签
                top_node->firstchild = cur_node;    //当前标签结点是栈顶标签结点的左孩子
            }
            else{ //如果栈顶标签是自闭合、标签块或者文本
                top_node->nextsibling = cur_node;   //当前标签是栈顶标签的右孩子(nextnextsibling)
            } 
            if(tag.type == Single || tag.type == Text){      //自闭合标签结束位置
                cur_node->data.end = tag.end;
            } 
            lastend = false;  
            types.push(tag.type);             //当前标签类型入栈
            nodes.push(cur_node);             //当前标签入栈
        }
        else if(tag.type == EndTag){    //如果当前结点是结束标签
            while(top_type != StartTag){
                nodes.pop();    
                types.pop();
                top_type = types.top();     
            }
            top_node = nodes.top();
            top_node->data.end = tag.end;
            lastend = true;
            //前一个标签的类型改为WholeTag
            types.pop();
            types.push(WholeTag);    
        }
    }
}

//清空树
void clear_tree(CSTree &T)
{
    destroy_tree(T);    //释放树的空间
    T = nullptr;
}

//判断树是否为空树
bool tree_empty(CSTree T)
{
    if(!T){   //如果树根为空，返回真
        return true;
    }
    return false;
}

//返回树的深度
int tree_depth(CSTree T)
{
    int dep1, dep2;
    if(!T){
        return 0;
    }
    dep1 = tree_depth(T->firstchild);    //左子树最大深度
    dep2 = tree_depth(T->nextsibling);    //右子树最大深度

    return (dep1 > dep2)? dep1 + 1: dep2 + 1;    //返回左右子树最大深度+1

    /*
    //层次遍历实现
    std::queue<CSTree> queue1, queue2;     //队列1存放上一层遍历结点，队列2存放当前层结点
    CSTree cur;
    int depth = 1;
    queue1.push(T);     //根结点入栈
    while(!queue1.empty()){
        while(!queue1.empty()){    //队列1非空
            cur = queue1.front();
            queue1.pop();
            if(!cur->firstchild){    //左孩子非空，入队
                queue2.push(cur->firstchild);
            }
            if(!cur->nextsibling){    //右孩子非空，入栈
                queue2.push(cur->nextsibling);
            }
        }
        depth ++;
        swap(queue1, queue2);
    }
    return depth;
    */
}

//判断某个结点是否在树中
bool in_tree(CSTree T, CSTree cur_e)
{
    if(!T){   //如果树空，返回假
        return false;
    }
    if(T->firstchild == cur_e || T->nextsibling == cur_e){    //结点在树中，返回真
        return true;
    }
    return in_tree(T->firstchild, cur_e) || in_tree(T->nextsibling, cur_e);   //递归查找左右子树
}


//结点cur_e赋值为value
void assign(CSTree T, CSTree &cur_e, ElemType value)
{
    if(!cur_e){
        std::cerr << "Assign Error: 赋值节点不存在" << std::endl;
        return ;
    }
    if(!in_tree(T, cur_e)){    //如果结点cur_e 不在树中，返回
        return ;
    }
    cur_e->data = value;
}
 
//返回非根结点的双亲
CSTree parent(CSTree T, CSTree cur_e)
{
    if(!T){      //空结点没有孩子
        return nullptr;
    }
    if(T->firstchild == cur_e || T->nextsibling == cur_e){   //找到双亲
        return T;
    }
    else{
        CSTree p1 = parent(T->firstchild, cur_e);    //查找左子树
        CSTree p2 = parent(T->nextsibling, cur_e);    //查找右子树
        if(!p1){
            return p1;
        }
        if(!p2){
            return p2;
        }
    }
    return nullptr;
}

//返回结点cur_e的最左孩子
CSTree left_child(CSTree T, CSTree cur_e)
{
    if(!in_tree(T, cur_e) || !cur_e || !cur_e->firstchild){   //如果结点不在树中或者cur_e不存在或者cur_e没有左子树,返回空
        return nullptr;
    }
    CSTree leftchild = cur_e->firstchild;
    while(leftchild->firstchild){    //查找最左孩子
        leftchild = leftchild->firstchild;
    }
    return leftchild;
}

//返回结点的右兄弟
CSTree right_sibling(CSTree T, CSTree cur_e)
{
    if(!in_tree(T, cur_e) || !cur_e || !cur_e->nextsibling){   //如果结点不在树中或者cur_e不存在或者cur_e没有右子树,返回空
        return nullptr;
    }

    return cur_e->nextsibling;     //返回右兄弟
}

//插入c为T中p指向结点的第i棵子树
void insert_child(CSTree &T, CSNode p, int i, CSTree C);

//删除T中p指向结点的第i棵子树
void delete_child(CSTree &T, CSNode p, int i);

//按先序对T中的每个结点调用函数visit一次
void PreOrderTraverse(CSTree T, void (*visit)(CSTree node))
{
    /*  递归实现
    if(!T){
        return ;
    }
    visit(T);
    PreOrderTraverse(T->firstchild, visit);
    PreOrderTraverse(T->nextsibling, visit);
    */
    if(!T){     //树空直接返回
        return ;
    }
    CSTree node = T;
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){   //栈非空或者node存在
        while(node){      //向左访问结点
            visit(node);   //访问节点   
            nodes.push(node);    //访问过的结点入栈 
            node = node->firstchild;   //向左继续走
        }
        node = nodes.top();     //最左孩子出栈
        nodes.pop();
        node = node->nextsibling;  //向右一步
    }
    
}

//按中序对T中的每个结点调用函数visit一次
void InOrderTraverse(CSTree T, void (*visit)(CSTree node))
{
    /*   递归实现
    if(!T){
        return ;
    }
    InOrderTraverse(T->firstchild, visit);
    visit(T);
    InOrderTraverse(T->nextsibling, visit);
    */
    if(!T){    //树空直接返回
        return ;
    }
    CSTree node = T;
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){    //栈非空或者node存在
        while(node){     //找到node子树的最左孩子
            nodes.push(node);
            node = node->firstchild;
        }
       
        node = nodes.top();     //最左结点出栈
        nodes.pop();
        visit(node);      //访问结点
        node = node->nextsibling;    //向右一步
    }
}

//按后序对T中的每个结点调用函数visit一次
void PostOrderTraverse(CSTree T, void (*visit)(CSTree node))
{
    /*   递归实现
     if(!T){
        return ;
    }
    PostOrderTraverse(T->firstchild, visit);
    PostOrderTraverse(T->nextsibling, visit);
    visit(T);
    */
    if(!T){      //树空直接返回
        return ;
    }
    CSTree node = T, front;
    std::stack<CSTree> nodes;
    while(!nodes.empty() || node){
        while(node){     //找到node子树的最左孩子
            nodes.push(node);
            node = node->firstchild;
        }
        node = nodes.top();      //最左结点
        //如果最左孩子没有右孩子或者上一个访问的结点是node结点的右孩子(即node右子树遍历完)
        if(!node->nextsibling || node->nextsibling == front){    
            nodes.pop();
            visit(node);    //访问结点
            front = node;   //更新上一次访问结点
        }
        else{
            node = node->nextsibling;    //向右一步
        }
    }
   
}
