#include"sqlist.h"
#include"linkstack.h"
#include<sstream>
#include <fstream>
#include <limits> 
#include<vector>
#include<cmath>
#include <chrono>
#include <thread>
#include<iostream>


//自闭合标签
const std::vector<std::string> SINGLE_TAGs = {
        "base", "link", "img", "input", "br", "hr", "!DOCTYPE", "area",
        "command", "embed", "keygen",  "meta", "param", 
        "source", "track", "wbr" 
};

//文本行内元素--只能嵌套文本
const std::vector<std::string> TEXT_INLINE_ELEMS =  {
    "option", "textarea", "optgroup"
};

//行内元素--只能嵌套行内元素,可以嵌套自身
const std::vector<std::string> INLINE_ELEMs = {
    "span",  "strong", "em", "b", "i", "del", "ins",
    "kbd", "samp", "var", "code", "q", "cite", "dfn",
    "abbr", "time", "mark", "ruby", "rt", "rp", "bdi",
    "bdo", "wbr", "small", "sub", "sup", "title", "audio"
};

//特殊块级元素--只能嵌套行内元素，不能嵌套块级元素，包括自身
const std::vector<std::string> SPECIAL_BLOCKS = {
    "h1", "h2", "h3", "h4", "h5", "h6", "p", "dt", "dd"
};

//块级元素，特殊行内元素a
const std::vector<std::string> BLOCKS = {
    "html", "div","ul", "ol", "li", "a", "dl", "header", "footer",
    "section", "article", "nav", "aside", "figure", "figcaption", "main", "blockquote", 
    "pre", "address", "head", "body", "button"
};

//table标签的直接子元素
const std::vector<std::string> TABLE_1 = {
    "tbody", "thead", "tfoot", "caption", "colgroup"
};


enum Order{     //菜单命令
    Exit,
    LoadHTML,
    CheckHTML,
    OutHTML,
    OutText,
    XPathSearch,
    Wrong
};

//HTML标签比较--HTML标签不区分大小写
bool html_tag_cmp(std::string str1, std::string str2)
{
    size_t len1 = str1.length();
    size_t len2 = str2.length();
    if(len1 != len2){    //如果长度不等，标签一定不相同
        return false;
    }
    for(int i = 0; i < len1; i++){
        if(str1[i] != str2[i] && std::abs(str1[i] - str2[i]) != 32){    //如果标签不相同(不区分大小写)，返回false
            return false;
        }
    }
    return true;    //否则返回true
}


//返回标签元素的类型
HtmlElemClass type_of_tag(std::string tag)
{
    //判断是否是自闭合标签
    for (const auto& elem : SINGLE_TAGs) {
        if(html_tag_cmp(tag, elem)){
            return Single;
        }
    }
    //判断是否是行内元素
    for (const auto& elem : INLINE_ELEMs) {
        if(html_tag_cmp(tag, elem)){
            return Inline;
        }
    }
    //判断是否是块级元素或特殊行内元素
    for (const auto& elem : BLOCKS) {
        if(html_tag_cmp(tag, elem)){
            return Block;
        }    
    }
    //判断是否是特殊块级元素
    for (const auto& elem : SPECIAL_BLOCKS) {
        if(html_tag_cmp(tag, elem)){
            return SpecialBlock;
        }    
    }
    //判断是否是table标签元素
    if(tag == "table"){
        return Table;
    }
    for(const auto& elem: TABLE_1){
        if(html_tag_cmp(tag, elem)){
            return Table_1;
        }
    }
    if(tag == "tr"){
        return Table_2;
    }
    if(tag == "td" || tag == "th" || tag == "col"){
        return Table_3;
    }
    //判断是否是文本行内元素
    for (const auto& elem : TEXT_INLINE_ELEMS) {
        if(html_tag_cmp(tag, elem)){
            return TextInline;
        }    
    }
    return None;
}

//获取输入指令
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
    else if(order == "XPathSearch" || order == "5"){
        return XPathSearch;
    }
    else {
        return Wrong;
    }
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

//移除多余空格
std::string remove_extraspace(std::string tag, size_t len)
{
    std::string subtag;
    size_t start, end, size = len - 1;
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
        end = find_first_delimiter(tag, len, start, "> ");
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
    return subtag;
}


//按文件读取html文件，切词并保存在顺序表中
bool load_html_by_file(sqlist &content, std::string path)
{
    transfer_path(path);       //获取正确路径
    std::ifstream file(path);
    if(!file.is_open()){
        std::cerr << "Can't open file" << std::endl;
        return false;
    }

    std::stringstream buffer;  //文件字节流
    std::string html, word, text = "", tmp;       //文件内容、切出的词、标签、文本、
    elemtype elem;      
    size_t size;     //文件内容长度
    size_t start = 0, end, len;   //切词开始和结尾、词长
    size_t end1, start1;
    int count = 1;     //标签和文本个数
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
            if(len > 6){
                if(tmp[1] == '!' && tmp[2] == '-'){
                    end = find_first_substr(html, "->", start, size);    //跳过注释
                    start = end + 2;
                    continue;
                }
                else if ((tmp[1] == 's' && tmp[2] == 'c' )){
                    end = find_first_substr(html, "script>", end + 1, size);    //找过滤块结束标签位置
                    start = end + 7;      //跳过过滤块
                    continue;
                }
                else if((tmp[1] == 's' && tmp[2] == 't' && tmp[3] == 'y')){
                    end = find_first_substr(html, "style>", end + 1, size);    //找过滤块结束标签位置
                    start = end + 6;      //跳过过滤块
                    continue;
                }
            }
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
        return true;
    }
    return false;
}


//检查父标签和子标签之间的嵌套关系是否正确
bool check_nest_rule(elemtype felem, elemtype elem, std::string &error_info)
{
    if(felem.type == TextInline && elem.type != Text){     //文本行内元素不能嵌套非文本
        error_info = "标签" + elem.tag + "不能嵌套非文本内容，而标签" + felem.tag + "不是文本"; 
        return false;
    }
    if(felem.type <= Block && elem.type <= Block){   //父元素和子元素都不是表格类元素
        
        if(felem.tag_name == "a" && elem.tag_name == "a"){
            error_info = "特殊行内元素<a>不能嵌套自身";
            return false;
        }
        else if(felem.type == SpecialBlock && elem.type == SpecialBlock){     //如果两个都是特殊块级元素
            error_info = "特殊块级元素" + felem.tag + "只能嵌套行内元素不能嵌套块级元素" + elem.tag;
            return false;
        }
        else if(felem.type < elem.type && elem.tag_name != "a"){
            error_info = "标签" + felem.tag + "只能嵌套行内元素不能嵌套块级元素" + elem.tag;
            return false;
        }
    }
    else if(felem.type > Block && elem.type > Block){           //父元素和子元素都是表格类元素
        if(felem.type == Table_1 && felem.tag_name == "caption"){
            error_info = "表格元素+" +  felem.tag + "不能嵌套" + elem.tag; 
            return false;
        }
        if(felem.type <= elem.type){
            error_info = "表格元素" + felem.tag + "不能嵌套表格元素" + elem.tag;
            return false;
        }
    }
    else{                                            //父元素和子元素既有表格类元素也有非表格元素
        //如果父元素是表格元素且不是td、th、tr，子元素是非表格元素
        if(((felem.type != Table_3 && felem.type != Table_2) || felem.tag_name == "col") && elem.type <= Block){
            error_info = "表格元素" + felem.tag + "不能直接嵌套元素" + elem.tag;
            return false;
        }
        else if ((felem.type < Block && elem.type > Block) || (felem.type == Block && elem.type < Table)){
            error_info = "元素" + felem.tag + "不能直接嵌套表格元素" + elem.tag;
            return false;
        }
    }
    //判断特殊要求嵌套关系
    if((elem.tag_name == "area" && felem.tag_name != "map") || 
        (elem.tag_name == "param" && felem.tag_name != "object") ||
        (elem.tag_name == "track" && felem.tag_name != "audio" && felem.tag_name != "video") ||
        ((elem.tag_name == "option" || elem.tag_name == "optgroup") && felem.tag_name != "select" && felem.tag_name != "datalist")){
    return false;
    }
    if((elem.tag_name != "area" && felem.tag_name == "map") || 
        (elem.tag_name != "param" && felem.tag_name == "object") ||
        (elem.tag_name != "track" && (felem.tag_name == "audio" || felem.tag_name == "video")) ||
        ((elem.tag_name != "option" && elem.tag_name != "optgroup") && (felem.tag_name == "select" || felem.tag_name == "datalist"))){
    return false;
    }
    return true;
}

//判断顺序表中的html代码是否合理，用error_info保存错误信息
bool check_html(sqlist content, std::string &error_info)
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
            return false;
        }
        //如果是自闭合标签
        else if (elem.type == Single) {
            if(top.tag != "" && !check_nest_rule(top, elem, error_info)){   //判断嵌套是否正确
                return false;
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
                return false;
            }
            else{
                push(Tags, elem);
            }
        }
        else{      //后标签与栈顶元素比较
            if(stack_empty(Tags)){    //栈已空，说明缺少当前标签的前标签
                error_info = "标签: " + elem.tag + "缺少前标签"+ elem.tag + "";
                return false;
            }
            //栈中有元素,当前标签与栈顶标签比较
            if(html_tag_cmp(elem.tag_name, top.tag_name)){     //如果当前元素等于栈顶元素，栈顶元素出栈
                pop(Tags, top);
            }  
            else{   //栈顶元素和当前元素不匹配，输出错误信息
                error_info = "标签" + top.tag + "缺少闭合标签</" + top.tag_name + ">, 而不是" + elem.tag; 
                return false;
            }
        }   
        index ++;        //节点后移
    }
    return true;
}


void get_htmlpath(sqlist &xpath, std::string path)
{
    size_t start, end;       //path子串开始和结束位置
    size_t size = path.length();     //path长度
    elemtype elem;
    int count = 1;    
    start = (path[0] == '/' || path[0] == '\\')? 1: 0; 
    while(start < size){
        if(path[start] == ' '){     //跳过空格
            start ++;
            continue;
        }
        end = find_first_delimiter(path, size, start, "/\\");     //xpath分隔位置
        elem.tag_name = path.substr(start, end - start);              //标签名称
        elem.type = type_of_tag(elem.tag_name);  
        list_insert(xpath, count, elem);         //插入xpath表
        count ++;
        start = end + 1;    //继续遍历
    }
}

void out_tag(elemtype elem, elemtype ftag)
{
    if(elem.type == Text){  //如果是文本直接输出
        std::cout << elem.tag;
    }
    else if(elem.type == Single){    //如果是自闭合标签输出完换行
        std::cout << elem.tag << std::endl;
    }
    else if(elem.tag[1] == '/'){
        if(ftag.type == Text && (ftag.type == Block || ftag.type == SpecialBlock) && ftag.tag_name != "a"){
            std::cout << std::endl;
        }
        std::cout << elem.tag << std::endl;
        if(((elem.type == Block && elem.tag_name != "a") || elem.type == SpecialBlock) && ftag.type == Text){
            std::cout << std::endl;
        }
    }
    else{
        if(ftag.type == Text && elem.type != Inline && elem.tag_name != "a"||
        ftag.tag[1] != '/'){
            std::cout << std::endl;
        }
        std::cout << elem.tag;
    }
}

//给定单纯由Tag构成的简化的XPATH路径path，输出所有能够匹配该路径的节点的OuterHTML 
bool out_html(sqlist content, sqlist xpath)
{
    int index = 0, xindex = 0;      //content下标位置,xpath下标位置
    elemtype xtag = xpath.elem[0], elem, top, ftag;      //xpath标签，content标签, tag标签后标签
    ftag.type = None;
    stack Tags = nullptr;     //存放content中的标签
    init_stack(Tags);
    int pairs = 0;     //前后标签成对数目
    bool output = false, find = false;
    //如果是根目录，输出所有标签
    if(xpath.length == 0){     
        for(int i = 0; i < content.length; i++){
            elem = content.elem[i];
            out_tag(elem, ftag);
            ftag = elem;
        }
        return true;
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
                    output = true;
                    find = true;
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
                output = false;
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
        return false;
    }
    return true;
}


//输出页面中符合路径条件的elements中的所有文本内容
bool out_text(sqlist content, sqlist xpath)
{
    int index = 0, xindex = 0;      //content下标位置,xpath下标位置
    elemtype xtag = xpath.elem[0], elem, top;      //xpath标签，content标签, tag标签后标签
    stack Tags = nullptr;     //存放content中的标签
    init_stack(Tags);
    int pairs = 0;     //前后标签成对数目
    bool output = false, find = false;
    //如果是根目录，输出所有文本
    if(xpath.length == 0){     
        for(int i = 0; i < content.length; i++){
            elem = content.elem[i];
            if(elem.tag[0] != '<'){  //如果是文本直接输出
                std::cout << elem.tag << std::endl << std::endl;
            }
        }
        return true;
    }
    //如果不是根目录
    while(index < content.length){
        elem = content.elem[index];
        if(pairs){
            if(output && elem.type == Text){
                std::cout << elem.tag << std::endl;
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
        if(elem.tag[0] != '<'){      //跳过文本内容
            index ++;
            continue;
        }
        if(elem.tag[1] != '/'){    //如果是左标签和自闭合标签
            if(elem.tag_name == xtag.tag_name){      //如果html当前标签tag与xpath路径当前标签相同，tag入栈
                push(Tags, elem);
                index ++;
                xindex ++;
                if(xindex == xpath.length){     //找到了xpath路径下，打印内容
                    pop(Tags, top);
                    xindex --;              //xpath下标回溯
                    output = true;
                    find = true;
                    if(elem.type == Text){
                        std::cout << elem.tag << std::endl;
                    }
                    pairs = (elem.type == Single)? 0: 1;
                }
                xtag = xpath.elem[xindex];
                continue;
            }
            else{        //否则跳过当前标签内容
                pairs = (elem.type == Single)? 0: 1;
                index ++;
                output = false;
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
        return false;
    }
    return true;
}


void Menu(sqlist &content, stack &S, std::string filepath)
{
    int exit = 1;      //决定程序是否退出
    //操作用到的变量
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
        std::cout << "################################### MENU ########################################" << std::endl;
        std::cout << "##    ------------------input order or number as follows -------------------   ##" << std::endl; 
        std::cout << "##          ***********           0.Exit                    ***********        ##" << std::endl; 
        std::cout << "##          ***********           1.LoadHTML                ***********        ##" << std::endl; 
        std::cout << "##          ***********           2.CheckHTML               ***********        ##" << std::endl; 
        std::cout << "##          ***********           3.OutHTML                 ***********        ##" << std::endl;
        std::cout << "##          ***********           4.OutText                 ***********        ##" << std::endl;
        std::cout << "#################################################################################" << std::endl;
        std::cout << std::endl;
        //输入指令
        std::cout << "Command/input order> " ;      //选择操作
        if (!(std::cin >> input)) {  // 如果输入错误
            std::cin.clear();         // 清除错误标志
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 忽略错误输入直到下一个换行符
            std::cout << "------Please check if the input is correct and re-enter the command-----" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));    //暂停1秒
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
            std::this_thread::sleep_for(std::chrono::seconds(1));    //暂停1秒
            std::cout << "----------------alreadly exit----------------" << std::endl;
            break;
        case LoadHTML:
            clear_list(content);
            // std::cout << "Command/请输入文件路径> " ;
            // std::getline(std::cin, path);
            
            if(load_html_by_file(content, filepath)){
                std::cout << "Load successfully" << std::endl;
                //list_print(content);
            }
            else{
                std::cout << "Loading failed, please check if the file exists or if the input is incorrect" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));    //暂停1秒
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
            std::this_thread::sleep_for(std::chrono::seconds(1));    //暂停1秒
            continue;
        }
        if (exit){   //避免操作完直接打印菜单，影响阅读（除退出操作外）
            std::cout << "--------Please enter anything to continue the operation--------" << std::endl;
            std::getline(std::cin, enter);
            std::cout << std::endl;
        }
    }
}


int main(int argc, char *argv[])
{
    if(argc < 2){
        return 0;
    }
    std::string filepath(argv[1]);
    stack tag = nullptr;
    sqlist content;
    content.elem = nullptr;
    init_stack(tag);      //初始化tag栈和content顺序表
    init_list(content, INIT_SIZE);

    Menu(content, tag, filepath);

    destroy_stack(tag);
    destroy_list(content);
    return 0;
}






