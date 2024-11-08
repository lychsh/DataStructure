# 大作业二 栈和线性表实现HTMl文档存储和解析

## 姓名：李应
## 学号：2023200440
## 完成日期: 2024/10/29


## 一、需求分析
### (1)输入形式：以命令行的形式运行htmlparser.exe程序，并输入html文件路径(字符串)作为参数
### (2)输出形式：loadhtml函数：输出加载信息（成功/失败）；checkhtml函数：html代码是否合法(不合法输出错误信息); outhtml函数：xpath路径下的代码（字符串）；outtext函数：输出指定xpath函数路径下文本内容（字符串）；
### (3)程序功能：loadhtml函数：将html文件以一定格式存储在线性表中；checkhtml函数：检查html代码是否合法(不合法输出错误信息); outhtml函数：输出指定xpath函数路径下内容；outtext函数：输出指定xpath函数路径下文本内容；
### (4)测试数据：错误html文件vldb_invalid.html文件，正确html文件vldb_valid.html

## 二、概要设计
### 1、抽象数据类型定义
#### （1）tag节点类型
```cpp
typedef struct elemtype{    //节点元素类型
    std::string tag;       //标签内容
    std::string tag_name;  //标签名称
    HtmlElemClass type;    //标签类型
}elemtype;
```

#### （2）顺序表类型
```cpp
typedef struct{
    elemtype *elem;   //顺序表
    int length;     //表长(现有元素个数)，初始为0
    int listsize;   //存储容量，能容纳的元素个数
}sqlist;
```

#### （2）链栈类型
```cpp
typedef struct stacknode{
    elemtype data;     // 数据域
    stacknode* next;   //指针域
}stacknode, *stack;

```

### 2、主程序流程
打印菜单，根据用户输入指令或指令对应的数字，通过switch结构跳转到相应的语句，调用对应函数，输入参数，执行功能；
若输入错误则输出提醒，并再次执行菜单功能。


### 3、模块调用关系
#### 程序分为三个模块：htmlparser、linkstack、sqlist。分别有相应的cpp文件及头文件
#### 调用关系：
##### 1、主函数调用menu函数打印菜单和接受命令，调用栈和链表的初始化及销毁函数
##### 2、menu函数调用栈的初始化函数，loadhtml, check_html, out_html, out_text函数，栈的销毁和清空函数，链表的清空和销毁函数
##### 3、load_html函数调用transfer_path，find_first_delimiter，find_first_substr，remove_extraspace， list_insert函数，分别用于path路径转义符转换，查找第一个分隔符位置，去除多余空格，和插入顺序表存储tag
##### 4、check_html函数调用init_stack初始化标签栈，stack_empty，pop,push等栈的基本操作，check_nest_rule判断嵌套规则
##### 5、out_html函数调用init_stack初始化标签栈，stack_empty，pop， push等栈的基本操作,out_tag控制标签输出
##### 6、out_text函数调用init_stack初始化标签栈，stack_empty，pop， push等栈的基本操作.

## 三、详细设计
### 1、枚举类型定义
#### （1）HTML标签分类
```cpp
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
```

#### （2）菜单命令
```cpp

enum Order{     //菜单命令
    Exit,
    LoadHTML,
    CheckHTML,
    OutHTML,
    OutText,
    XPathSearch,
    Wrong
};
```
### 主功能模块
#### 1、加载文件
```cpp
//按文件读取html文件，切词并保存在顺序表中
Status load_html_by_file(sqlist &content, std::string path)
{
    transfer_path(path);       //获取正确路径
    std::ifstream file(path);
    if(!file.is_open()){
        std::cerr << "Can't open file" << std::endl;
        return ERROR;
    }

    std::stringstream buffer;  //文件字节流
    std::string html, word, text = "", tmp;       //文件内容、切出的词、标签、文本、
    elemtype elem;      
    size_t size;     //文件内容长度
    size_t start = 0, end, len;   //切词开始和结尾、词长
    size_t end1, start1;
    int count = 1;     //标签和文本个数
    //读取文件内容
    html = file.str();
    //预处理换行符
        if(html[i...size-1] == '\n'){
            html[i] = ' ';
        }
    //遍历切词
    while(start < size){
        if(html[start] == ' '){      //遇到空格跳过
            start += 1;
            continue;
        }
        //读取标签和文本
        end = find_first_delimiter(html, size, start, "<");   //查找tag开始位置
        if(end == start){        //如果找到标签起始或标签未找完
            if(text != ""){  //判断之前的text是否为空，不为空入栈
                //text存入顺序表
                elem.tag = text;
                elem.type = Text;
                elem.tag_name = "";
                list_insert(content, count, elem);    //插入顺序表
                count ++;
                text = "";            //置空，方便下一次查找
            }
            end = find_first_delimiter(html, size, start, ">");  //找到标签结尾
            len = end - start + 1;
            tmp = html.substr(start, len);
            
            //判断是否是过滤标签并跳过(<!-- --> or <script> or <style>)
            
            end1 = find_first_substr(html, "  ", start, size);
            elem.tag = tmp;
            if(end1 < end){     //tag中间有多个空格，去除多余空格
                elem.tag = remove_extraspace(tmp, len);
            }
            //tag存入顺序表
            end1 = find_first_delimiter(elem.tag, elem.tag.length(), 0, "> ");
            start1 = (elem.tag[1] == '/')? 2: 1;     //前后标签名称起始，start值1、 2分别代表前后标签
            elem.tag_name = elem.tag.substr(start1, end1 - start1);   //只保留标签名称
            elem.type = type_of_tag(elem.tag_name);     //获取标签类别
            list_insert(content, count, elem);       //插入顺序表
            count ++;
            start = end + 1;
        }
        else{      //找到文本
            end = find_first_delimiter(html, size, start, "< ");   //词结束符位置
            len = end - start;
            word = html.substr(start, len);          //切词
            if(word == " " || word == ""){                //空词跳过
                start = end;
                continue;
            }
            text += word + " ";        //词后是空白，加空格，跳过多余空格
            start = end;
        }
    }

    if(text != ""){      //如果未正确闭合还有一段文字未插入
        elem.tag = text;
        elem.type = Text;
        list_insert(content, count, elem);
    }
    if(start >= size){    //正确保存所有html内容
        return OK;
    }
    return ERROR;
}
```

#### 2、检查html合法性
##### (1)嵌套规则判断
```cpp

//检查父标签和子标签之间的嵌套关系是否正确
Status check_nest_rule(elemtype felem, elemtype elem, std::string &error_info)
{
    if(felem.type == TextInline && elem.type != Text){     //文本行内元素不能嵌套非文本
        error_info = "标签" + elem.tag + "不能嵌套非文本内容，而标签" + felem.tag + "不是文本"; 
        return ERROR;
    }
    if(felem.type <= Block && elem.type <= Block){   //父元素和子元素都不是表格类元素
        
        if(felem.tag_name == "a" && elem.tag_name == "a"){
            error_info = "特殊行内元素<a>不能嵌套自身";
            return ERROR;
        }
        else if(felem.type == SpecialBlock && elem.type == SpecialBlock){     //如果两个都是特殊块级元素
            error_info = "特殊块级元素" + felem.tag + "只能嵌套行内元素不能嵌套块级元素" + elem.tag;
            return ERROR;
        }
        else if(felem.type < elem.type && elem.tag_name != "a"){
            error_info = "标签" + felem.tag + "只能嵌套行内元素不能嵌套块级元素" + elem.tag;
            return ERROR;
        }
    }
    else if(felem.type > Block && elem.type > Block){           //父元素和子元素都是表格类元素
        if(felem.type == Table_1 && felem.tag_name == "caption"){
            error_info = "表格元素+" +  felem.tag + "不能嵌套" + elem.tag; 
            return ERROR;
        }
        if(felem.type <= elem.type){
            error_info = "表格元素" + felem.tag + "不能嵌套表格元素" + elem.tag;
            return ERROR;
        }
    }
    else{                                            //父元素和子元素既有表格类元素也有非表格元素
        //如果父元素是表格元素且不是td、th、tr，子元素是非表格元素
        if(((felem.type != Table_3 && felem.type != Table_2) || felem.tag_name == "col") && elem.type <= Block){
            error_info = "表格元素" + felem.tag + "不能直接嵌套元素" + elem.tag;
            return ERROR;
        }
        else if ((felem.type < Block && elem.type > Block) || (felem.type == Block && elem.type < Table)){
            error_info = "元素" + felem.tag + "不能直接嵌套表格元素" + elem.tag;
            return ERROR;
        }
    }
    //判断特殊要求嵌套关系
    if((elem.tag_name == "area" && felem.tag_name != "map") || 
        (elem.tag_name == "param" && felem.tag_name != "object") ||
        (elem.tag_name == "track" && felem.tag_name != "audio" && felem.tag_name != "video") ||
        ((elem.tag_name == "option" || elem.tag_name == "optgroup") && felem.tag_name != "select" && felem.tag_name != "datalist")){
    return ERROR;
    }
    if((elem.tag_name != "area" && felem.tag_name == "map") || 
        (elem.tag_name != "param" && felem.tag_name == "object") ||
        (elem.tag_name != "track" && (felem.tag_name == "audio" || felem.tag_name == "video")) ||
        ((elem.tag_name != "option" && elem.tag_name != "optgroup") && (felem.tag_name == "select" || felem.tag_name == "datalist"))){
    return ERROR;
    }
    return OK;
}
```

```cpp
//判断顺序表中的html代码是否合理，用error_info保存错误信息
Status check_html(sqlist content, std::string &error_info)
{
    stack Tags = nullptr;     //标签名称栈
    init_stack(Tags);    //初始化栈
    elemtype elem, top; //标签，标签栈顶标签
    int index = 0;      //顺序表下标
    while(index < content.length){
        elem = content.elem[index];
        if(elem.type == Text){    //跳过文本
            index ++;
            continue;
        }
        if(!stack_empty(Tags)){    //取栈顶元素
            get_top(Tags, top);
        }
        else{
            top.tag = "";
        }
        //如果是错误标签
        if(elem.type == None){
            error_info = "标签" + elem.tag + "是非法的html标签";
            return ERROR;
        }
        //如果是自闭合标签
        else if (elem.type == Single) {
            if(top.tag != "" && !check_nest_rule(top, elem, error_info)){   //判断嵌套是否正确
                return ERROR;
            }
            index ++;
            continue;
        } 
        //判断标签合法性
        if(elem.tag[1] != '/'){    //双标签第一个标签
            if(stack_empty(Tags)){   //栈空直接入
                push(Tags, elem);
                index ++;
                continue;
            }
            if(!check_nest_rule(top, elem, error_info)){    //否则判断栈顶与当前元素嵌套关系
                return ERROR;
            }
            else{
                push(Tags, elem);
            }
        }
        else{      //后标签与栈顶元素比较
            if(stack_empty(Tags)){    //栈已空，说明缺少当前标签的前标签
                error_info = "标签: " + elem.tag + "缺少前标签"+ elem.tag + "";
                return ERROR;
            }
            //栈中有元素,当前标签与栈顶标签比较
            if(html_tag_cmp(elem.tag_name, top.tag_name)){     //如果当前元素等于栈顶元素，栈顶元素出栈
                pop(Tags, top);
            }  
            else{   //栈顶元素和当前元素不匹配，输出错误信息
                error_info = "标签" + top.tag + "缺少闭合标签</" + top.tag_name + ">, 而不是" + elem.tag; 
                return ERROR;
            }
        }   
        index ++;        //节点后移
    }
    return OK;
}

```

#### 3、输出代码
```cpp
//给定单纯由Tag构成的简化的XPATH路径path，输出所有能够匹配该路径的节点的OuterHTML 
Status out_html(sqlist content, sqlist xpath)
{
    int index = 0, xindex = 0;      //content下标位置,xpath下标位置
    elemtype xtag = xpath.elem[0], elem, top, ftag;      //xpath标签，content标签, tag标签后标签
    ftag.type = None;
    stack Tags = nullptr;     //存放content中的标签
    init_stack(Tags);
    int pairs = 0;     //前后标签成对数目
    bool output = ERROR, find = ERROR;
    //如果是根目录，输出所有标签
    if(xpath.length == 0){     
        for(int i = 0; i < content.length; i++){
            elem = content.elem[i];
            out_tag(elem, ftag);
            ftag = elem;
        }
        return OK;
    }
    //如果不是根目录
    while(index < content.length){
        elem = content.elem[index];
        if(index > 0){ftag = content.elem[index - 1];}
        if(pairs){
            if(output){
                out_tag(elem, ftag);
            }
            if(elem.tag[1] == '/'){  //如果是后标签
                pairs --;
                
            }    //如果是前标签
            else if(elem.type > Single){
                pairs ++;
            }
            index ++;
            continue;
        }
        if(elem.type == Text){      //跳过文本内容
            index ++;
            continue;
        }
        if(elem.tag[1] != '/'){    //如果是左标签和自闭合标签
            if(elem.tag_name == xtag.tag_name){      //如果html当前标签tag与xpath路径当前标签相同，tag入栈
                push(Tags, elem);
                index ++;
                xindex ++;
                if(xindex == xpath.length){     //找到了xpath路径下，打印内容
                    output = OK;
                    find = OK;
                    pop(Tags, top);
                    xindex --;              //xpath下标回溯
                    out_tag(elem, ftag);
                    pairs = (elem.type == Single)? 0: 1;
                }
                xtag = xpath.elem[xindex];
                continue;
            }
            else{        //否则跳过当前标签内容
                pairs = (elem.type == Single)? 0: 1;
                index ++;
                output = ERROR;
                continue;
            }
        }
        else if(!stack_empty(Tags)){       //如果是后标签
            get_top(Tags, top);
            if(top.tag_name == elem.tag_name){   //后标签与栈顶元素相同，弹出栈顶
                pop(Tags, top);
                xindex --;
                xtag = xpath.elem[xindex];
            }
        }
        index ++;
    }
    if(!find){
        return ERROR;
    }
    return OK;
}
```


#### 4、输出文本
实现与3基本相同，输出内容限定为文本
```cpp
Status out_text(sqlist content, sqlist xpath)
{
·······
        if(pairs){
            if(output && elem.type == Text){    //只输出文本
                out_tag(elem, ftag);
            }
            if(elem.tag[1] == '/'){  //如果是后标签
                pairs --;
                
            }    //如果是前标签
            else if(elem.type > Single){
                pairs ++;
            }
            index ++;
            continue;
        }
·······
}
```

#### 5、主代码
```cpp
void Menu(sqlist &content, stack &S, std::string filepath)
{
    int exit = 1;      //决定程序是否退出
    //初始化操作用到的变量
    stack result = nullptr;
    init_stack(result); 
    std::string enter;
    std::string input; 
    std::string path;
    sqlist xpath;
    xpath.elem = nullptr;
    init_list(xpath, 100);
    std::string error_info;

    while(exit)
    {
        //打印菜单
        ·······
        //输入指令
        std::cout << "Command/input order> " ;      //选择操作
        if (!(std::cin >> input)) {  // 如果输入错误
            std::cin.clear();         // 清除错误标志
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 忽略错误输入直到下一个换行符
            std::cout << "------Please check if the input is correct and re-enter the command-----" << std::endl;
            Sleep(1);    //暂停1秒
            continue;            // 继续下一次循环迭代
        }
        std::cout << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //清空缓冲区
        Order order = get_order(input);
        //进行操作
        switch (order)
        {
        case Exit:
            exit = 0;
            destroy_stack(result);
            destroy_list(xpath);
            std::cout << "........" << std::endl;
            Sleep(1);    //暂停1秒
            std::cout << "----------------alreadly exit----------------" << std::endl;
            break;
        case LoadHTML:
            clear_list(content);
            if(load_html_by_file(content, filepath)){
                std::cout << "Load successfully" << std::endl;
            }
            else{
                std::cout << "Loading failed, please check if the file exists or if the input is incorrect" << std::endl;
                 Sleep(1);    //暂停1秒
                continue;
            }
            break;
        case CheckHTML:
            std::cout << "......" << std::endl;
            if(check_html(content, error_info)){
                std::cout <<"Html is correct" << std::endl;
            }
            else{
                std::cout << "Wrong: The HTML code is illegal, the illegal places are as follows: " << std::endl;
                std::cout << error_info << std::endl;
            }
            break;
        case OutHTML:
            clear_list(xpath);       //清空之前的xpath表
            std::cout << "Command/input xpath> " ;
            std::getline(std::cin, path);
            get_htmlpath(xpath, path);
            std::cout << std::endl << "Find the following: "<<std::endl;
            if(!out_html(content, xpath)){
                std::cout << "The path does not exist or is entered incorrectly" << std::endl;
            }
            break;
        case OutText:
            std::cout << "Command/input xpath> " ;
            std::getline(std::cin, path);
            clear_list(xpath);        //清空之前的xpath表
            get_htmlpath(xpath, path);
            std::cout << "Find the following: "<<std::endl;
            if(!out_text(content, xpath)){
                std::cout << "The path does not exist or is entered incorrectly" << std::endl;
            }
            break;
        case XPathSearch:
            break;
        default:
            std::cout << "------Wrong selection, please select again-----" << std::endl;
             Sleep(1);    //暂停1秒
            continue;
        }
        if (exit){   //避免操作完直接打印菜单，影响阅读（除退出操作外）
            std::cout << "--------Please enter anything to continue the operation--------" << std::endl;
            std::getline(std::cin, enter);
            std::cout << std::endl;
        }
    }
}

```

## 四、调试分析
### 1、问题
编写过程中遇到的问题主要是html的存储和html的输出。刚开始直接使用原来的顺序表存储html的标签，但信息较少，所以在顺序表元素结构体中增加tag_name和type类型，以存储更多信息，方便后续功能的实现。输出时会存在逻辑上的一些问题，导致输出不全或者查找失败，通过调试，完善逻辑和代码，对标签出入栈合理判断，能够正确输出。

### 2、时空分析
#### (1)load_html:
时间复杂度：O(n)，n取决于tag标签数量
空间复杂度：O(n)，，n取决于tag标签数量

#### (2)check_html:
时间复杂度：O(n)，n取决于tag标签数量
空间复杂度：O(1)

#### (3)out_html:
时间复杂度：O(n)，n取决于tag标签数量
空间复杂度：O(1)

#### (3)out_text:
时间复杂度：O(n)，n取决于tag标签数量
空间复杂度：O(1)

## 五、使用说明
首先需要编译:命令行输入 g++ -o htmlparser.cpp sqlist.cpp linkstack.cpp -o ./htmlparser.exe
运行：在命令输入 ./htmlparser xxx.html
xxx.html为需要进行操作的html文件路径

## 六、测试结果
正确判断出三个错误文件信息，对正确文件能够完整输出xpath路径下的代码和文本

## 七、附录
程序所包含的文件如下:
node.h
sqlist.cpp
sqlist.h
linkstack.cpp
linkstack.h
htmlparser.cpp


