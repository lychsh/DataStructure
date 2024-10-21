#include"sqlist.h"
#include"linkstack.h"
#include<sstream>
#include <fstream>
#include <limits> 
#include<vector>
#include<regex>
#include<cmath>

//自闭合标签
const std::vector<std::string> SINGLE_TAG = {
         "base", "link", "img", "input", "br", "hr", "!DOCTYPE", "area",
        "command", "embed", "keygen",  "meta", "param", "col",
        "source", "track", "wbr" 
};

//块级元素，特殊行内元素a
const std::vector<std::string> HIGHELEM = {
    "html", "div","ul", "ol", "li", "a", "dl", "dt", "dd", "header", "footer", "tr",
    "section", "article", "nav", "aside", "figure", "figcaption", "main", "blockquote", 
    "pre", "address", "head", "body", "button", "table", "tbody", "textarea", "option","optgroup"
};
//行内元素,特殊块级元素 - 只能嵌套行内元素
const std::vector<std::string> LOWELEM = {
    "h1", "h2", "h3", "h4", "h5", "h6", "p", "dt",
    "span", "td","th" "strong", "em", "b", "i", "del", "ins",
    "kbd", "samp", "var", "code", "q", "cite", "dfn",
    "abbr", "time", "mark", "ruby", "rt", "rp", "bdi",
    "bdo", "br", "wbr", "small", "sub", "sup", "title","P", "audio"
};

enum Order{     //菜单命令
    Exit,
    LoadHTML,
    CheckHTML,
    OutHTML,
    OutText,
    URLParser,
    Wrong
};

enum HtmlElemClass{
    None,
    Single,
    Inline,
    Block  
};


bool html_tag_cmp(std::string str1, std::string str2)
{
    size_t len1 = str1.length();
    size_t len2 = str2.length();
    if(len1 != len2){
        return false;
    }
    for(int i = 0; i < len1; i++){
        if(str1[i] != str2[i] && std::abs(str1[i] - str2[i]) != 32){
            return false;
        }
    }
    return true;
}

Order get_order(std::string order)
{
    if(order == "Exit" || order == "0"){
        return Exit;
    }
    else if(order == "LoadHTML" || order == "1"){
        return LoadHTML;
    }
    else if(order == "CheckHTML" || order == "2"){
        return CheckHTML;
    }
    else if(order == "OutHTML" || order == "3"){
        return OutHTML;
    }
    else if(order == "OutText" || order == "4"){
        return OutText;
    }
    else if(order == "URLParser" || order == "5"){
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


//按文件读取html文件，切词并保存在顺序表中
bool load_html_by_file(sqlist &content, std::string path)
{
    transfer_path(path);       //获取正确路径
    std::ifstream file(path);
    if(!file.is_open()){
        std::cerr << "无法打开文件" << std::endl;
        return false;
    }

    std::stringstream buffer;  //文件字节流
    std::string html, word, tag, text = "", tmp;       //文件内容、切出的词、标签、文本、
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
    //预处理换行符
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
        end = find_first_delimiter(html, size, start, "<");   //查找tag开始位置
        if(tag_open || end == start){        //如果找到标签起始或标签未找完
            if(text != ""){  //判断之前的text是否为空，不为空入栈
                //text存入顺序表
                list_insert(content, count, text);    //插入顺序表
                count ++;
                text = "";            //置空，方便下一次查找
            }
            end = find_first_delimiter(html, size, start, ">");  //找到标签结尾
            tmp = html.substr(start, end - start + 1);
            len = end - start + 1;
            //判断是否是过滤标签并跳过(<!-- --> or <script> or <style>)
            if(len > 6){
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
                
                if(html[end1 - 1] == '/' || html[end1 - 1] == '<' || html[end1 + 1] == '>'){   //如果是在标签名称和标签括号之间有空格不保留空格
                    tag += html.substr(start, end1 - start);
                }
                else{   //否则保留一个空格
                    tag += html.substr(start, end1 - start + 1);      //加上当前部分标签
                }
                start = end1 + 1;
                continue;
            }
            else{
                if(tag_open){    //如果是多空格分隔的标签
                    tag += html.substr(start, end - start + 1);      //加上之前的标签内容构成完整标签
                }
                else{      //否则直接获取标签
                    tag = html.substr(start, end - start + 1);      
                }
                tag_open = false;
            }
            //tag存入顺序表
            list_insert(content, count, tag);
            count ++;
            tag = "";    //置空标签
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
                text += word + " ";        //词后是空白，加空格，跳过多余空格
            }
        }
        start = end + 1;
    }
    if(text != ""){      //如果未正确闭合还有一段文字未插入
        list_insert(content, count, text);
    }
    if(start >= size){    //正确保存所有html内容
        return true;
    }
    return false;
}

//按行加载html文件的tag和文本内容，存储在顺序表中
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


//返回标签tag的等级
HtmlElemClass type_of_tag(elemtype tag)
{
    //判断是否是自闭合标签
    for (const auto& elem : SINGLE_TAG) {
        if(html_tag_cmp(tag, elem)){
            return Single;
        }
    }
    //判断是否是行内元素或者特殊块级元素
    for (const auto& elem : LOWELEM) {
        if(html_tag_cmp(tag, elem)){
            return Inline;
        }
    }
    //判断是否是块级元素或特殊行内元素
    for (const auto& elem : HIGHELEM) {
        if(html_tag_cmp(tag, elem)){
            return Block;
        }    
    }
    return None;
}

//检查父标签和子标签之间的嵌套关系是否正确
bool check_nest_rule(std::string ftag, std::string tag, HtmlElemClass tag_type)
{
     //判断自闭合标签嵌套关系
    if(tag_type == Single){
        if((tag == "area" && ftag != "map") ||
            (tag == "col" && ftag != "clogroup" && ftag != "table" && ftag != "tbody") ||
            (tag == "colgroup" && ftag != "table" && ftag != "tbody") ||
            (tag == "param" && ftag != "object") ||
            (tag == "track" && ftag != "audio" && ftag != "video") ||
            ((tag == "option" || tag == "optgroup") && ftag != "select" && ftag != "datalist")){
        return false;
        }
    }
    //判断行内元素嵌套关系
    else if(tag_type == Inline){ 
        if( ((tag == "td" || tag == "th") && ftag != "table" && ftag != "thead" && ftag != "tbody" && ftag != "tfoot" && ftag != "colgroup" && ftag != "tr")){
            return false;
        }
    }//判断块级元素嵌套关系
    else if(tag_type == Block){
        if( (tag == "tr" && ftag != "thead" && ftag != "tbody" && ftag != "tfoot" && ftag != "colgroup")||
        ((tag == "caption" || tag == "colgroup" ||  tag == "thead" || tag == "tbody" || tag == "tfoot" ) && ftag != "table")){
            return false;
        }
    }
    return true;
}

//判断顺序表中的html代码是否合理，用error_info保存错误信息
bool check_html(sqlist content, std::string &error_info)
{
    stack Tags = nullptr;     //标签名称栈
    init_stack(Tags);    //初始化栈
    elemtype tag, top; //标签，标签栈顶标签
    HtmlElemClass tag_type, top_type;    //标签的级别
    size_type start, end;    //切词开始、结尾
    int index = 0;      //顺序表下标
    while(index < content.length){
        tag = content.elem[index];
        if(tag[0] != '<'){    //不是标签跳过
            index ++;
            continue;
        }
        end = find_first_delimiter(tag, tag.length(), 0, "> ");
        start = (tag[1] == '/')? 2: 1;     //前后标签名称起始，start值1、 2分别代表前后标签
        tag = content.elem[index].substr(start, end - start);   //只保留标签名称
        tag_type = type_of_tag(tag);
        if(!stack_empty(Tags)){    //取栈顶元素
            get_top(Tags, top);
            top_type = type_of_tag(top);
        }
        else{
            top = "";
        }
        //如果是错误标签
        if(tag_type == None){
            if(start == 1){
                error_info = "标签<" + tag + ">是非法的html标签";
            }
            else{
                error_info = "标签<" + tag + " />是非法的html标签";
            }
            return false;
        }
        //如果是自闭合标签
        if (tag_type == Single) {
            if(top != "" and !check_nest_rule(top, tag, tag_type)){   //判断嵌套是否正确
                error_info =  "标签<" + top + ">不应该嵌套标签<" + tag + ">"; 
                return false;
            }
            index ++;
            continue;
        } 
        //判断标签合法性
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
            else if(top == "option" || top == "textarea"){
                error_info = "标签<" + top + ">不能嵌套文本外内容";
                return false;  
            }
            else if(tag_type <= top_type){    //与栈顶元素比较，如果当前元素优先级小于等于栈顶元素，压入栈中
                push(Tags, tag);
            }
            else{                           //否则，返回错误信息
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
            if(html_tag_cmp(tag, top)){     //如果当前元素等于栈顶元素，栈顶元素出栈
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
    elemtype xtag = xpath.elem[0], tag, tag_name, top;      //xpath标签，content标签, tag标签后标签
    size_type start, end;
    stack Tags = nullptr;     //存放content中的标签
    init_stack(Tags);
    int pairs = 1;     //前后标签成对数目
    HtmlElemClass tag_type;
    bool output = false;
    //如果是根目录，输出所有标签
    if(xpath.length == 0){     
        for(int i = 0; i < content.length; i++){
            tag = content.elem[i];
            if(tag[0] != '<'){  //如果是文本直接输出
                std::cout << tag;
                continue;
            }
            end = find_first_delimiter(tag, tag.length(), 0, "> ");
            start = (tag[1] == '/')? 2: 1;
            tag_name = tag.substr(start, end - start);   //只保留标签名称
            tag_type = type_of_tag(tag_name);
            if(type_of_tag(tag) == Single){    //如果是自闭合标签输出完换行
                std::cout << tag << std::endl;
            }
            else if(tag[1] == '/'){  //如果是后标签
                if(tag_type == Inline || tag_name == "a"){
                    std::cout << tag;
                }
                else 
                {
                    std::cout << std::endl << tag;
                }
                std::cout << std::endl;
            }    //如果是前标签
            else{
                if(tag_type == Block && tag_name != "a"){
                    std::cout << tag << std::endl;
                }
                else{
                    std::cout << tag ;        //前标签输出完不换行
                }
            }
        }
        return true;
    }
    //如果不是根目录
    while(index < content.length){
        tag = content.elem[index];
        if(tag[0] != '<'){      //跳过文本内容
            index ++;
            continue;
        }
        //获得标签名称
        end = find_first_delimiter(tag, tag.length(), 0, "> ");
        start = (tag[1] == '/')? 2: 1;
        tag_name = tag.substr(start, end - start);   //只保留标签名称
        tag_type = type_of_tag(tag_name);

        if(start == 1){    //如果是左标签和自闭合标签
            if(tag_name == xtag){      //如果html当前标签tag与xpath路径当前标签相同，tag入栈
                push(Tags, tag_name);
                index ++;
                xindex ++;
                if(xindex == xpath.length){     //找到了xpath路径下，打印内容
                    output = true;
                    pop(Tags, top);
                    get_top(Tags, top);
                    xindex --;              //xpath下标回溯
                    std::cout << tag << std::endl;
                    if(tag_type == Single){
                        pairs = 0;
                    }
                    else{
                        pairs = 1;
                    }
                    while(pairs && index < content.length){   //知道找到xpath最后一个标签的后标签
                        tag = content.elem[index];
                        if(tag[0] != '<'){  //如果是文本直接输出
                            std::cout << tag << std::endl;
                            index ++;
                            continue;
                        }
                        //获得标签名称
                        end = find_first_delimiter(tag, tag.length(), 0, "> ");
                        start = (tag[1] == '/')? 2: 1;
                        tag_name = tag.substr(start, end - start);   //只保留标签名称
                        tag_type = type_of_tag(tag_name);
                        if(type_of_tag(tag) == Single){    //如果是自闭合标签输出完换行
                            std::cout << tag << std::endl;
                        }
                        else if(tag[1] == '/'){  //如果是后标签
                            pairs --;
                            std::cout << tag << std::endl;
                        }    //如果是前标签
                        else{
                            pairs ++;
                            std::cout << tag << std::endl;
                        }
                        index ++;
                    }
                }
                xtag = xpath.elem[xindex];
                continue;
            }
            else{        //否则跳过当前标签内容
                if(tag_type == Single){
                    pairs = 0;
                }
                else{
                    pairs = 1;
                }
                index ++;
                while(pairs && index < content.length){   
                    tag = content.elem[index];
                    if(tag[0] != '<'){  //如果是文本跳过
                        index ++;
                        continue;
                    }
                    //获得标签名称
                    end = find_first_delimiter(tag, tag.length(), 0, "> ");
                    start = (tag[1] == '/')? 2: 1;
                    tag_name = tag.substr(start, end - start);   //只保留标签名称
                    tag_type = type_of_tag(tag_name);
                    if(tag_type == Single || tag[0] != '<'){    //如果是自闭合标签或者文本跳过
                       index ++;
                       continue;
                    }
                    else if(tag[1] == '/'){   //如果是后标签
                        pairs --;
                    }
                    else {     //如果前标签
                        pairs ++;
                    }
                    index ++;
                }
                continue;
            }
        }
        
        else if(!stack_empty(Tags)){       //如果是后标签
            get_top(Tags, top);
            if(top == tag_name){   //后标签与栈顶元素相同，弹出栈顶
                pop(Tags, top);
                xindex --;
                xtag = xpath.elem[xindex];
            }
        }
        index ++;
    }
    if(!output){
        return false;
    }
    return true;
}


//输出页面中符合路径条件的elements中的所有文本内容
bool out_text(sqlist content, sqlist xpath)
{
    int index = 0, xindex = 0;      //content下标位置,xpath下标位置
    elemtype xtag = xpath.elem[0], tag, tag_name, top;      //xpath标签，content标签, tag标签后标签
    size_type start, end;
    stack Tags = nullptr;     //存放content中的标签
    init_stack(Tags);
    int pairs = 1;     //前后标签成对数目
    HtmlElemClass tag_type;
    bool output = false;
    //如果是根目录，输出所有文本
    if(xpath.length == 0){     
        for(int i = 0; i < content.length; i++){
            tag = content.elem[i];
            if(tag[0] != '<'){  //如果是文本直接输出
                std::cout << tag;
            }
        }
        return true;
    }
    //如果不是根目录
    while(index < content.length){
        tag = content.elem[index];
        if(tag[0] != '<'){      //跳过文本内容
            index ++;
            continue;
        }
        //获得标签名称
        end = find_first_delimiter(tag, tag.length(), 0, "> ");
        start = (tag[1] == '/')? 2: 1;
        tag_name = tag.substr(start, end - start);   //只保留标签名称
        tag_type = type_of_tag(tag_name);

        if(start == 1){    //如果是左标签和自闭合标签
            if(tag_name == xtag){      //如果html当前标签tag与xpath路径当前标签相同，tag入栈
                push(Tags, tag_name);
                index ++;
                xindex ++;
                if(xindex == xpath.length){     //找到了xpath路径下，打印内容
                    pop(Tags, top);
                    get_top(Tags, top);
                    xindex --;              //xpath下标回溯
                    if(tag[0] != '<'){
                        output = true;
                        std::cout << tag << std::endl;
                    }
                    if(tag_type == Single){
                        pairs = 0;
                    }
                    else{
                        pairs = 1;
                    }
                    while(pairs && index < content.length){   //知道找到xpath最后一个标签的后标签
                        tag = content.elem[index];
                        if(tag[0] != '<'){  //如果是文本直接输出
                            output = true;
                            std::cout << tag << std::endl;
                        }
                        else if(tag[1] == '/'){  //如果是后标签
                            pairs --;
                        }    //如果是前标签
                        else{
                            pairs ++;
                        }
                        index ++;
                    }
                }
                xtag = xpath.elem[xindex];
                continue;
            }
            else{        //否则跳过当前标签内容
                if(tag_type == Single){
                    pairs = 0;
                }
                else{
                    pairs = 1;
                }
                index ++;
                while(pairs && index < content.length){   
                    tag = content.elem[index];
                    if(tag[0] != '<'){  //如果是文本跳过
                        index ++;
                        continue;
                    }
                    //获得标签名称
                    end = find_first_delimiter(tag, tag.length(), 0, "> ");
                    start = (tag[1] == '/')? 2: 1;
                    tag_name = tag.substr(start, end - start);   //只保留标签名称
                    tag_type = type_of_tag(tag_name);
                    if(tag_type == Single || tag[0] != '<'){    //如果是自闭合标签或者文本跳过
                       index ++;
                       continue;
                    }
                    else if(tag[1] == '/'){   //如果是后标签
                        pairs --;
                    }
                    else {     //如果前标签
                        pairs ++;
                    }
                    index ++;
                }
                continue;
            }
        }
        
        else if(!stack_empty(Tags)){       //如果是后标签
            get_top(Tags, top);
            if(top == tag_name){   //后标签与栈顶元素相同，弹出栈顶
                pop(Tags, top);
                xindex --;
                xtag = xpath.elem[xindex];
            }
        }
        index ++;
    }
    if(!output){
        return false;
    }
    return true;
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
            clear_list(content);
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
            std::cout << "查找到的内容如下: "<<std::endl;
            if(!out_html(content, xpath)){
                std::cout << "该路径不存在" << std::endl;
            }
            break;
        case OutText:
            std::cout << "请输入要查找到的XPATH路径: " ;
            std::getline(std::cin, path);
            clear_list(xpath);        //清空之前的xpath表
            get_xpath(xpath, path);
            std::cout << "查找到的内容如下: "<<std::endl;
            if(!out_text(content, xpath)){
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






