#include"sqlist.h"
#include"linkstack.h"
#include<sstream>
#include <fstream>
#include <limits> 
#include<vector>

//自闭合标签
const std::vector<std::string> SINGLE_TAG = {
        "img", "input", "br", "hr", "area", "base", "col",
        "command", "embed", "keygen", "link", "meta", "param",
        "source", "track", "wbr", "!DOCTYPE" 
};

//块级元素，特殊行内元素
const std::vector<std::string> HIGHELEM = {
    "html", "div","ul", "ol", "li", "a", "dl", "dt", "dd", "header", "footer",
    "section", "article", "nav", "aside", "figure", "figcaption",
    "main", "blockquote", "pre", "address", "head", "body"
};
//行内元素,特殊块级元素 - 只能嵌套行内元素
const std::vector<std::string> LOWELEM = {
    "h1", "h2", "h3", "h4", "h5", "h6", "p", "dt"
    "span", "strong", "em", "b", "i", "del", "ins",
    "kbd", "samp", "var", "code", "q", "cite", "dfn",
    "abbr", "time", "mark", "ruby", "rt", "rp", "bdi",
    "bdo", "br", "wbr", "small", "sub", "sup", "title"
};

enum Order{     //菜单命令
    Exit,
    LoadHTML,
    CheckHTML,
    OutHTML,
    Text,
    URLParser,
    Wrong
};

enum HtmlElemClass{
    None,
    Single,
    Low,
    High  
};

Order get_order(std::string order)
{
    if(order == "Exit"){
        return Exit;
    }
    else if(order == "LoadHTML"){
        return LoadHTML;
    }
    else if(order == "CheckHTML"){
        return CheckHTML;
    }
    else if(order == "OutHTML"){
        return OutHTML;
    }
    else if(order == "Text"){
        return Text;
    }
    else if(order == "URLParser"){
        return URLParser;
    }
    else {
        return Wrong;
    }
}

//将转义字符/改为//
void transfer_path(std::string &path)
{   
    size_t pos = 0;
    while((pos = path.find('\\', pos)) != std::string::npos) {
        path.replace(pos, 1, "\\\\");
        pos += 2; // 因为替换后的字符串长度增加了，所以需要跳过两个字符
    }
}

bool load_html_by_file(sqlist &content, std::string path)
{
    transfer_path(path);       //获取正确路径
    std::ifstream file(path);
    if(!file.is_open()){
        std::cerr << "无法打开文件" << std::endl;
        return false;
    }

    std::stringstream buffer;  //文件字节流
    std::string html, word, tag, text = "", tmp;       //文件内容、切出的词、连续文本、标签
    std::string::size_type size;     //文件内容长度
    std::string::size_type start = 0, end, len;   //切词开始和结尾、词长
    size_type end1;
    int count = 1;     //标签和文本个数
    bool tag_open = false;
    //读取文件内容
    buffer << file.rdbuf();
    html = buffer.str();
    file.close();
    size = html.length();
    //预处理
    for(int i = 0; i < size; i++){
        if(html[i] == '\n'){
            html[i] = ' ';
        }
    }
    //遍历切词
    while(start < size){
        if(html[start] == ' '){      //遇到空格跳过
            start += 1;
            continue;
        }
        //读取标签和文本
        end = find_first_delimiter(html, size, start, "<");   //查找tag开始
        if(tag_open || end == start){        //如果找到标签或标签未找完
            if(text != " " && text != ""){  //判断之前的text是否为空，不为空入栈
                //text存入顺序表
                list_insert(content, count, text);    //插入顺序表
                count ++;
                text = "";            //置空，下一次查找
            }
            end = find_first_delimiter(html, size, start, ">");  //找到标签结尾
            tmp = html.substr(start, end - start + 1);
            len = end - start + 1;
            //判断是否是过滤标签并跳过
            if(len > 5){
                if(tmp[1] == '!' && tmp[2] == '-'){
                    end = find_first_substr(html, "->", start, size);    //跳过注释
                    start = end + 2;
                    continue;
                }
                else if ((tmp[1] == 's' && tmp[2] == 'c' ) ||(tmp[1] == 's' && tmp[2] == 't' && tmp[3] == 'y')){
                    end = find_first_delimiter(html, size, end + 1, ">");    //找过滤块结束标签位置
                    start = end + 1;      //跳过过滤块
                    continue;
                }
            }
            end1 = find_first_substr(html, "  ", start, size);
            if(end1 < end){     //tag中间有多个空格
                tag_open = true;
                tag += html.substr(start, end1 - start + 1);      //获取标签内容
                start = end1 + 1;
                continue;
            }
            else{
                if(tag_open){
                    tag += html.substr(start, end - start + 1);      //获取标签内容
                }
                else{
                    tag = html.substr(start, end - start + 1);      //获取标签内容
                }
                tag_open = false;
            }
            //tag存入顺序表
            list_insert(content, count, tag);
            count ++;
            tag = "";
        }
        else{      //找到文本
            end = find_first_delimiter(html, size, start, "< ");   //词结束符位置
            word = html.substr(start, end - start);          //切词
            if(word == " " || word == ""){                //空词跳过
                start += 1;
                continue;
            }
            len = end - start;
            if(end < size && html[end] == '<'){             //词后是标签，不跳过标签开头，词后不加空格
                end -= 1;
                text += word;
            }
            else{
                text += word + " ";        //词后是空白，加空格，去除多余空格
            }
        }
        start = end + 1;
    }
    if(text != ""){      //如果未正确闭合还有一段文字未插入
        list_insert(content, count, text);
    }
    if(start >= size){
        return true;
    }
    return false;
}

//加载html文件的tag和文本内容，存储在顺序表中
bool load_html_by_line(sqlist &content, std::string path)
{
    transfer_path(path);       //获取正确路径
    std::ifstream file(path);
    if(!file.is_open()){      //文件打开失败
        return false;
    }
    std::string line, word, tag, text = "";       //文件一行、切出的词、连续文本、标签
    std::string::size_type size;     //行长
    std::string::size_type start = 0, end, len;   //切词开始和结尾、词长
    int count = 1; 
    bool filter = false;    //决定是否过滤(注释, css和javacript需过滤)
    bool tag_over = true;
    bool note = false;
    //一行一行读取
    while(std::getline(file, line)){      //读取失败或者到文件末尾时结束
        start = 0;
        size = line.length();
        while(start < size){
            if(line[start] == ' '){      //遇到空格跳过
                start += 1;
                continue;
            }
            if(filter){     //需要过滤的块 
                end = find_first_delimiter(line, size, start, ">");    //过滤块的结尾
                if(note){               //如果是注释，需要判断>前一个
                    if(line[end - 1] == '-'){ 
                        note = false;
                    }
                    else {
                        start = end + 1;
                        continue;
                    }
                }
                if(end < size && line[end] == '>'){         //如果找到过滤块结尾，跳过过滤块
                    filter = false;                           //过滤状态设为false
                    start = end + 1;
                    continue;
                }
                break;                                   //当前行过滤块未结束，从下一行继续读
            }
            //正常读取标签和文本
            end = find_first_delimiter(line, size, start, "<");   //查找tag
            if(!tag_over || end == start){        //如果找到标签
                if(text != " " && text != ""){  //判断之前的text是否为空，不为空入栈
                    //text存入顺序表
                    list_insert(content, count, text);   //插入顺序表
                    count ++;
                    text = "";            //置空，下一次查找
                }
                end = find_first_delimiter(line, size, start, ">");  //找到标签结尾
                len = end - start + 1;
                if(!tag_over){      //上一行标签未闭合，继续加上上一行标签
                    tag += " " + line.substr(start, len);
                }
                else{               //否则获取新标签
                    tag = line.substr(start, len);      //获取标签内容
                }
                if(end < size && line[end] == '>'){     //标签已闭合
                        tag_over = true;
                }
                else{
                    tag_over = false;               //标签未闭合
                    break;
                }
                //判断是否是过滤标签
                if(len > 3){
                    if(tag[1] == '!' && tag[2] == '-'){
                        if (end < size && line[end] == '>' && line[end-1] == '-'){     //注释块只有一行
                            start = end + 1;
                            continue;
                        }
                        note = true;
                        start = end + 1;       //注释有多行，继续读取下一行
                        filter = true;     
                        break;       
                    }   //如果是css块或JavaScript块
                    else if((tag[1] == 's' && tag[2] == 'c' ) || (tag[1] == 's' && tag[2] == 't' && tag[3] == 'y')){
                        start = end + 1;         //有多行，继续读取下一行
                        filter = true;
                        continue;
                    }
                }
                
                if(end < size && line[end] == '>'){
                    //tag存入顺序表
                    list_insert(content, count, tag);
                    tag = "";
                }
            }
            else{      //找到文本
                end = find_first_delimiter(line, size, start, "< ");   //词结束符位置
                word = line.substr(start, end - start);          //切词
                if(word == " " || word == ""){                //空词跳过
                    start += 1;
                    continue;
                }
                if(end < size && line[end] == '<'){             //词后是标签，不跳过标签开头，词后不加空格
                    end -= 1;
                    text += word;
                }
                else{
                    text += word + " ";        //词后是空白，加空格，去除多余空格
                }
            }
            start = end + 1;
        }
    }
    if(text != ""){      //如果未正确闭合还有一段文字未插入
        list_insert(content, count, text);
    }
    if(file.eof()){     //读取结束
        return true;
    }
    //某一行读取失败
    std::cout << "文件未全部读取" << std::endl;
    return false;

}


//判断前后标签是否合法
HtmlElemClass type_of_tag(elemtype tag)
{
    //判断是否是自闭合标签
    for (const auto& elem : SINGLE_TAG) {
        if(tag == elem){
            return Single;
        }
    }
    //判断是否是行内元素或者特殊块级元素
    for (const auto& elem : LOWELEM) {
        if(tag == elem){
            return Low;
        }
    }
    //判断是否是块级元素或特殊行内元素
    for (const auto& elem : HIGHELEM) {
        if(tag == elem){
            return High;
        }    
    }
    return None;
}

//判断顺序表中的html代码是否合理
bool check_html(sqlist content, std::string &error_info)
{
    stack Tags = nullptr;     //标签栈
    init_stack(Tags);    //初始化栈
    elemtype tag, top;
    HtmlElemClass tag_type, top_type;
    size_type start, end;
    int index = 0;
    while(index < content.length){
        tag = content.elem[index];
        if(tag[0] != '<'){    //不是标签跳过
            index ++;
            continue;
        }
        end = find_first_delimiter(tag, tag.length(), 0, "> ");
        start = (tag[1] == '/')? 2: 1;
        tag = content.elem[index].substr(start, end - start);   //只保留标签名称
        tag_type = type_of_tag(tag);
        //如果是自闭合标签就跳过
        if (tag_type == Single) {
            index ++;
            continue;
        }
        if(!stack_empty(Tags)) {
            get_top(Tags, top);    //取栈顶元素
            top_type = type_of_tag(top);
        }  
        //判断双标签合法性
        if(start == 1){    //双标签第一个标签
            if(stack_empty(Tags)){   //栈空直接入
                push(Tags, tag);
                index ++;
                continue;
            }
            //特殊块级元素<a>单独判断
            if(tag == "a"){ 
                if(top == "a"){
                    error_info = "特殊行内元素<a>不能嵌套<a>";
                    return false;
                }
                push(Tags, tag);     //栈顶不是<a>，入栈
            }
            else if(tag_type <= top_type){    //与栈顶元素比较，如果当前元素优先级小于等于栈顶元素，压入栈中
                push(Tags, tag);
            }
            else{                           //否则，打印错误
                error_info =  "标签<" + top + ">不应该嵌套标签<" + tag + ">"; 
                return false;
            }
        }
        else{      //双标签第二个标签与栈顶元素比较
            if(stack_empty(Tags)){    //栈已空，说明缺少当前标签的前标签
                error_info = "标签: </" + tag + ">缺少前标签<"+ tag +">";
                return false;
            }
            //栈中有元素,当前标签与栈顶标签比较
            if(tag == top){     //如果当前元素等于栈顶元素，栈顶元素出栈
                pop(Tags, top);
            }  
            else{   //栈顶元素和当前元素不匹配，输出错误信息
                error_info = "标签<" + top + ">与标签</" + tag + ">不匹配"; 
                return false;
            }
        }   
        index ++;        //节点后移
    }
    return true;
}


void get_xpath(sqlist &xpath, std::string path)
{
    size_t start = 1, end;       //path子串开始和结束位置
    size_t size = path.length();     //path长度
    elemtype tag;
    int count = 1;     
    while(start < size){
        if(path[start] == ' '){     //跳过空格
            start ++;
            continue;
        }
        end = find_first_delimiter(path, size, start, "/");     //xpath分隔位置
        tag = path.substr(start, end - start);              //标签名称  
        list_insert(xpath, count, tag);         //插入xpath表
        count ++;
        start = end + 1;    //继续遍历
    }
}

//给定单纯由Tag构成的简化的XPATH路径path，输出所有能够匹配该路径的节点的OuterHTML 
bool out_html(sqlist content, sqlist xpath)
{
    int index = 0, xindex = 0;      //content下标位置,xpath下标位置
    elemtype xtag = xpath.elem[0], tag, tag_name, fronttag, backtag, top;      //xpath标签，content标签, tag标签后标签
    size_type start, end;
    stack Tags = nullptr;     //存放content中的标签
    init_stack(Tags);
    int num = 0;
    bool ignore = false;
    bool out = false;
    while(index < content.length){
        tag = content.elem[index];
        if(tag[0] != '<'){      //跳过文本内容
            index ++;
            continue;
        }
        if(tag[1] != '/'){    //前标签，num+1
            num += 1;
        }
        else{
            num -= 1;
        }
        //获得标签名称
        end = find_first_delimiter(tag, tag.length(), 0, "> ");
        start = (tag[1] == '/')? 2: 1;
        tag_name = content.elem[index].substr(start, end - start);   //只保留标签名称
        if(ignore){     //如果当前标签需要忽略
            if (tag_name == fronttag && num == 0){    //如果当前标签是忽略标签的后标签，忽略状态设为false
                ignore = false;
            }
            index ++;
            continue;
        }
        if(out){      //如果当前标签需要输出
            if(tag_name == fronttag && num == 0){
                out = false;
            }
            std::cout << tag << std::endl;
            continue;
        }
        if(xindex == xpath.length){     //找到了xpath路径下，打印内容
            pop(Tags, fronttag);        //弹出前标签
            xindex --;              //xpath指针回溯
            out = true;
            continue;
        }
        if(start == 1){    //如果是左标签
            if(tag_name == xtag){      //如果html当前标签tag与xpath路径当前标签相同，tag入栈
                push(Tags, tag_name);
                index ++;
                xindex ++;
                xtag = xpath.elem[xindex];
                continue;
            }
            else{        //否则跳过当前标签内容
                ignore = true; 
                fronttag = tag_name;  
                index ++; 
            }
        }
        
        else{       //如果是后标签
            get_top(Tags, top);
            if(top == tag_name){
                pop(Tags, top);
            }
            else{
                return false;
            }
        }
    }
    return true;
}

//输出页面中符合路径条件的elements中的所有文本内容
void out_text(sqlist content, sqlist xpath)
{
    int index = 0, xindex = 0;      //content下标位置,xpath下标位置
    elemtype xtag = xpath.elem[0], tag, tag_name, fronttag, backtag, top;      //xpath标签，content标签, tag标签后标签
    size_type start, end;
    stack Tags = nullptr;     //存放content中的标签
    init_stack(Tags);
    int num = 0;
    bool ignore = false;
    bool out = false;
    while(index < content.length){
        tag = content.elem[index];
        if(tag[1] != '/'){    //前标签，num+1
            num += 1;
        }
        else{
            num -= 1;
        }
        //获得标签名称
        end = find_first_delimiter(tag, tag.length(), 0, "> ");
        start = (tag[1] == '/')? 2: 1;
        tag_name = content.elem[index].substr(start, end - start);   //只保留标签名称
        if(ignore){     //如果当前标签需要忽略
            if (tag_name == fronttag && num == 0){    //如果当前标签是忽略标签的后标签，忽略状态设为false
                ignore = false;
            }
            index ++;
            continue;
        }
        if(out){      //如果当前标签需要输出
            if(tag_name == fronttag && num == 0){
                out = false;
            }
            if(tag[0] == '<'){      //跳过标签内容
                index ++;
                continue;
            }
            std::cout << tag << std::endl;
            continue;
        }
        if(xindex == xpath.length){     //找到了xpath路径下，打印内容
            pop(Tags, fronttag);        //弹出前标签
            xindex --;              //xpath指针回溯
            out = true;
            continue;
        }
        if(start == 1){    //如果是左标签
            if(tag_name == xtag){      //如果html当前标签tag与xpath路径当前标签相同，tag入栈
                push(Tags, tag_name);
                index ++;
                xindex ++;
                xtag = xpath.elem[xindex];
                continue;
            }
            else{        //否则跳过当前标签内容
                ignore = true; 
                fronttag = tag_name;  
                index ++; 
            }
        }
        
        else{       //如果是后标签
            get_top(Tags, top);
            if(top == tag_name){
                pop(Tags, top);
            }
            else{
                return ;
            }
        }
    }
}
 

void Menu(sqlist &content, stack &S)
{
    int exit = 1;      //决定程序是否退出
    //操作用到的变量
    stack result = nullptr;
    init_stack(result); 
    std::string enter;
    std::string path;  
    sqlist xpath;
    xpath.elem = nullptr;
    init_list(xpath, 100);
    std::string error_info;

    while(exit)
    {
        std::cout << "------------------------请根据操作输入相应的命令-----------------------" << std::endl; 
        std::cout << "************************0.Exit(退出)***********************************" << std::endl; 
        std::cout << "************************1.LoadHTML(加载HTML文件)***********************" << std::endl; 
        std::cout << "************************2.CheckHTML(检查HTML代码合法性)****************" << std::endl; 
        std::cout << "************************3.OutHTML(输出XPATH路径下的代码)***************" << std::endl;
        std::cout << "************************4.OutText(输出XPATH路径下的文本)***************" << std::endl;
        std::cout << "************************5.URLParser(解析URL)***************************" << std::endl;
        //输入指令
        std::string input;
        std::cout << "输入你的选择：" ;      //选择操作
        if (!(std::cin >> input)) {  // 如果输入错误
            std::cin.clear();         // 清除错误标志
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 忽略错误输入直到下一个换行符
            std::cout << "------请检查输入是否正确, 并重新输入命令-----" << std::endl;
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
            std::cout << "----------------已退出----------------" << std::endl;
            break;
        case LoadHTML:
            std::cout << "请输入文件路径: " ;
            std::getline(std::cin, path);
            
            if(load_html_by_file(content, path)){
                std::cout << "加载成功" << std::endl;
                list_print(content);
            }
            else{
                std::cout << "Warning: 加载失败, 请检查文件是否存在或输入是否有误" << std::endl;
            }
            break;
        case CheckHTML:
            if(check_html(content, error_info)){
                std::cout <<"Correct: HTML代码合法" << std::endl;
            }
            else{
                std::cout << "Wrong: HTML代码不合法, 不合法的地方如下: " << std::endl;
                std::cout << error_info << std::endl;
            }
            break;
        case OutHTML:
            std::cout << "请输入要查找到的XPATH路径: " ;
            std::getline(std::cin, path);
            clear_list(xpath);       //清空之前的xpath表
            get_xpath(xpath, path);
            if(out_html(content, xpath)){
                std::cout << "查找到的内容如下: "<<std::endl;
            }
            else{
                std::cout << "该路径不存在" << std::endl;
            }
            break;
        case Text:
            std::cout << "请输入要查找到的XPATH路径: " ;
            std::getline(std::cin, path);
            clear_list(xpath);        //清空之前的xpath表
            get_xpath(xpath, path);
            if(out_html(content, xpath)){
                std::cout << "查找到的内容如下 "<<std::endl;
            }
            else{
                std::cout << "该路径不存在" << std::endl;
            }
            break;
        case URLParser:
            break;
        default:
            std::cout << "------选择错误，请重新选择-----" << std::endl;
            break;
        }
        if (exit){   //避免操作完直接打印菜单，影响阅读（除退出操作外）
            std::cout << "--------请输入任意内容继续执行操作--------" << std::endl;
            std::getline(std::cin, enter);
            std::cout << std::endl;
        }
    }
}


int main()
{
    stack tag = nullptr;
    sqlist content;
    content.elem = nullptr;
    init_stack(tag);      //初始化tag栈和content顺序表
    init_list(content, INIT_SIZE);

    Menu(content, tag);

    destroy_stack(tag);
    destroy_list(content);
    return 0;
}






