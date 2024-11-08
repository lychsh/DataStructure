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


//�Ապϱ�ǩ
const std::vector<std::string> SINGLE_TAGs = {
        "base", "link", "img", "input", "br", "hr", "!DOCTYPE", "area",
        "command", "embed", "keygen",  "meta", "param", 
        "source", "track", "wbr" 
};

//�ı�����Ԫ��--ֻ��Ƕ���ı�
const std::vector<std::string> TEXT_INLINE_ELEMS =  {
    "option", "textarea", "optgroup"
};

//����Ԫ��--ֻ��Ƕ������Ԫ��,����Ƕ������
const std::vector<std::string> INLINE_ELEMs = {
    "span",  "strong", "em", "b", "i", "del", "ins",
    "kbd", "samp", "var", "code", "q", "cite", "dfn",
    "abbr", "time", "mark", "ruby", "rt", "rp", "bdi",
    "bdo", "wbr", "small", "sub", "sup", "title", "audio"
};

//����鼶Ԫ��--ֻ��Ƕ������Ԫ�أ�����Ƕ�׿鼶Ԫ�أ���������
const std::vector<std::string> SPECIAL_BLOCKS = {
    "h1", "h2", "h3", "h4", "h5", "h6", "p", "dt", "dd"
};

//�鼶Ԫ�أ���������Ԫ��a
const std::vector<std::string> BLOCKS = {
    "html", "div","ul", "ol", "li", "a", "dl", "header", "footer",
    "section", "article", "nav", "aside", "figure", "figcaption", "main", "blockquote", 
    "pre", "address", "head", "body", "button"
};

//table��ǩ��ֱ����Ԫ��
const std::vector<std::string> TABLE_1 = {
    "tbody", "thead", "tfoot", "caption", "colgroup"
};


enum Order{     //�˵�����
    Exit,
    LoadHTML,
    CheckHTML,
    OutHTML,
    OutText,
    XPathSearch,
    Wrong
};

//HTML��ǩ�Ƚ�--HTML��ǩ�����ִ�Сд
bool html_tag_cmp(std::string str1, std::string str2)
{
    size_t len1 = str1.length();
    size_t len2 = str2.length();
    if(len1 != len2){    //������Ȳ��ȣ���ǩһ������ͬ
        return false;
    }
    for(int i = 0; i < len1; i++){
        if(str1[i] != str2[i] && std::abs(str1[i] - str2[i]) != 32){    //�����ǩ����ͬ(�����ִ�Сд)������false
            return false;
        }
    }
    return true;    //���򷵻�true
}


//���ر�ǩԪ�ص�����
HtmlElemClass type_of_tag(std::string tag)
{
    //�ж��Ƿ����Ապϱ�ǩ
    for (const auto& elem : SINGLE_TAGs) {
        if(html_tag_cmp(tag, elem)){
            return Single;
        }
    }
    //�ж��Ƿ�������Ԫ��
    for (const auto& elem : INLINE_ELEMs) {
        if(html_tag_cmp(tag, elem)){
            return Inline;
        }
    }
    //�ж��Ƿ��ǿ鼶Ԫ�ػ���������Ԫ��
    for (const auto& elem : BLOCKS) {
        if(html_tag_cmp(tag, elem)){
            return Block;
        }    
    }
    //�ж��Ƿ�������鼶Ԫ��
    for (const auto& elem : SPECIAL_BLOCKS) {
        if(html_tag_cmp(tag, elem)){
            return SpecialBlock;
        }    
    }
    //�ж��Ƿ���table��ǩԪ��
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
    //�ж��Ƿ����ı�����Ԫ��
    for (const auto& elem : TEXT_INLINE_ELEMS) {
        if(html_tag_cmp(tag, elem)){
            return TextInline;
        }    
    }
    return None;
}

//��ȡ����ָ��
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

//���ļ�·���е�ת���ַ�/��Ϊ//
void transfer_path(std::string &path)
{   
    size_t pos = 0;
    while((pos = path.find('\\', pos)) != std::string::npos) {
        path.replace(pos, 1, "\\\\");
        pos += 2; // ��Ϊ�滻����ַ������������ˣ�������Ҫ���������ַ�
    }
}

//�Ƴ�����ո�
std::string remove_extraspace(std::string tag, size_t len)
{
    std::string subtag;
    size_t start, end, size = len - 1;
    if(tag[1] == '/'){   //����Ǻ��ǩ
        start = 2;
        subtag = "</";
    }    
    else{             //�����ǰ��ǩ
        start = 1;
        subtag = '<';
    }   
    while(start < size){
        if(tag[start] == ' '){     //��������ո�
            start ++;
            continue;
        }
        end = find_first_delimiter(tag, len, start, "> ");
        if(tag[end] == ' '){     //û�е��պϴ�>
            subtag += tag.substr(start, end - start + 1);    //���ϱ�ǩһ���ֺ�һ���ո�
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


//���ļ���ȡhtml�ļ����дʲ�������˳�����
bool load_html_by_file(sqlist &content, std::string path)
{
    transfer_path(path);       //��ȡ��ȷ·��
    std::ifstream file(path);
    if(!file.is_open()){
        std::cerr << "Can't open file" << std::endl;
        return false;
    }

    std::stringstream buffer;  //�ļ��ֽ���
    std::string html, word, text = "", tmp;       //�ļ����ݡ��г��Ĵʡ���ǩ���ı���
    elemtype elem;      
    size_t size;     //�ļ����ݳ���
    size_t start = 0, end, len;   //�дʿ�ʼ�ͽ�β���ʳ�
    size_t end1, start1;
    int count = 1;     //��ǩ���ı�����
    //��ȡ�ļ�����
    buffer << file.rdbuf();
    html = buffer.str();
    file.close();
    size = html.length();
    //Ԥ�����з�
    for(int i = 0; i < size; i++){
        if(html[i] == '\n'){
            html[i] = ' ';
        }
    }
    //�����д�
    while(start < size){
        if(html[start] == ' '){      //�����ո�����
            start += 1;
            continue;
        }
        //��ȡ��ǩ���ı�
        end = find_first_delimiter(html, size, start, "<");   //����tag��ʼλ��
        if(end == start){        //����ҵ���ǩ��ʼ���ǩδ����
            if(text != ""){  //�ж�֮ǰ��text�Ƿ�Ϊ�գ���Ϊ����ջ
                //text����˳���
                elem.tag = text;
                elem.type = Text;
                elem.tag_name = "";
                list_insert(content, count, elem);    //����˳���
                count ++;
                text = "";            //�ÿգ�������һ�β���
            }
            end = find_first_delimiter(html, size, start, ">");  //�ҵ���ǩ��β
            len = end - start + 1;
            tmp = html.substr(start, len);
            
            //�ж��Ƿ��ǹ��˱�ǩ������(<!-- --> or <script> or <style>)
            if(len > 6){
                if(tmp[1] == '!' && tmp[2] == '-'){
                    end = find_first_substr(html, "->", start, size);    //����ע��
                    start = end + 2;
                    continue;
                }
                else if ((tmp[1] == 's' && tmp[2] == 'c' )){
                    end = find_first_substr(html, "script>", end + 1, size);    //�ҹ��˿������ǩλ��
                    start = end + 7;      //�������˿�
                    continue;
                }
                else if((tmp[1] == 's' && tmp[2] == 't' && tmp[3] == 'y')){
                    end = find_first_substr(html, "style>", end + 1, size);    //�ҹ��˿������ǩλ��
                    start = end + 6;      //�������˿�
                    continue;
                }
            }
            end1 = find_first_substr(html, "  ", start, size);
            elem.tag = tmp;
            if(end1 < end){     //tag�м��ж���ո�ȥ������ո�
                elem.tag = remove_extraspace(tmp, len);
            }
            //tag����˳���
            end1 = find_first_delimiter(elem.tag, elem.tag.length(), 0, "> ");
            start1 = (elem.tag[1] == '/')? 2: 1;     //ǰ���ǩ������ʼ��startֵ1�� 2�ֱ����ǰ���ǩ
            elem.tag_name = elem.tag.substr(start1, end1 - start1);   //ֻ������ǩ����
            elem.type = type_of_tag(elem.tag_name);     //��ȡ��ǩ���
            list_insert(content, count, elem);       //����˳���
            count ++;
            start = end + 1;
        }
        else{      //�ҵ��ı�
            end = find_first_delimiter(html, size, start, "< ");   //�ʽ�����λ��
            len = end - start;
            word = html.substr(start, len);          //�д�
            if(word == " " || word == ""){                //�մ�����
                start = end;
                continue;
            }
            text += word + " ";        //�ʺ��ǿհף��ӿո���������ո�
            start = end;
        }
    }

    if(text != ""){      //���δ��ȷ�պϻ���һ������δ����
        elem.tag = text;
        elem.type = Text;
        list_insert(content, count, elem);
    }
    if(start >= size){    //��ȷ��������html����
        return true;
    }
    return false;
}


//��鸸��ǩ���ӱ�ǩ֮���Ƕ�׹�ϵ�Ƿ���ȷ
bool check_nest_rule(elemtype felem, elemtype elem, std::string &error_info)
{
    if(felem.type == TextInline && elem.type != Text){     //�ı�����Ԫ�ز���Ƕ�׷��ı�
        error_info = "��ǩ" + elem.tag + "����Ƕ�׷��ı����ݣ�����ǩ" + felem.tag + "�����ı�"; 
        return false;
    }
    if(felem.type <= Block && elem.type <= Block){   //��Ԫ�غ���Ԫ�ض����Ǳ����Ԫ��
        
        if(felem.tag_name == "a" && elem.tag_name == "a"){
            error_info = "��������Ԫ��<a>����Ƕ������";
            return false;
        }
        else if(felem.type == SpecialBlock && elem.type == SpecialBlock){     //���������������鼶Ԫ��
            error_info = "����鼶Ԫ��" + felem.tag + "ֻ��Ƕ������Ԫ�ز���Ƕ�׿鼶Ԫ��" + elem.tag;
            return false;
        }
        else if(felem.type < elem.type && elem.tag_name != "a"){
            error_info = "��ǩ" + felem.tag + "ֻ��Ƕ������Ԫ�ز���Ƕ�׿鼶Ԫ��" + elem.tag;
            return false;
        }
    }
    else if(felem.type > Block && elem.type > Block){           //��Ԫ�غ���Ԫ�ض��Ǳ����Ԫ��
        if(felem.type == Table_1 && felem.tag_name == "caption"){
            error_info = "���Ԫ��+" +  felem.tag + "����Ƕ��" + elem.tag; 
            return false;
        }
        if(felem.type <= elem.type){
            error_info = "���Ԫ��" + felem.tag + "����Ƕ�ױ��Ԫ��" + elem.tag;
            return false;
        }
    }
    else{                                            //��Ԫ�غ���Ԫ�ؼ��б����Ԫ��Ҳ�зǱ��Ԫ��
        //�����Ԫ���Ǳ��Ԫ���Ҳ���td��th��tr����Ԫ���ǷǱ��Ԫ��
        if(((felem.type != Table_3 && felem.type != Table_2) || felem.tag_name == "col") && elem.type <= Block){
            error_info = "���Ԫ��" + felem.tag + "����ֱ��Ƕ��Ԫ��" + elem.tag;
            return false;
        }
        else if ((felem.type < Block && elem.type > Block) || (felem.type == Block && elem.type < Table)){
            error_info = "Ԫ��" + felem.tag + "����ֱ��Ƕ�ױ��Ԫ��" + elem.tag;
            return false;
        }
    }
    //�ж�����Ҫ��Ƕ�׹�ϵ
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

//�ж�˳����е�html�����Ƿ������error_info���������Ϣ
bool check_html(sqlist content, std::string &error_info)
{
    stack Tags = nullptr;     //��ǩ����ջ
    init_stack(Tags);    //��ʼ��ջ
    elemtype elem, top; //��ǩ����ǩջ����ǩ
    int index = 0;      //˳����±�
    while(index < content.length){
        elem = content.elem[index];
        if(elem.type == Text){    //�����ı�
            index ++;
            continue;
        }
        if(!stack_empty(Tags)){    //ȡջ��Ԫ��
            get_top(Tags, top);
        }
        else{
            top.tag = "";
        }
        //����Ǵ����ǩ
        if(elem.type == None){
            error_info = "��ǩ" + elem.tag + "�ǷǷ���html��ǩ";
            return false;
        }
        //������Ապϱ�ǩ
        else if (elem.type == Single) {
            if(top.tag != "" && !check_nest_rule(top, elem, error_info)){   //�ж�Ƕ���Ƿ���ȷ
                return false;
            }
            index ++;
            continue;
        } 
        //�жϱ�ǩ�Ϸ���
        if(elem.tag[1] != '/'){    //˫��ǩ��һ����ǩ
            if(stack_empty(Tags)){   //ջ��ֱ����
                push(Tags, elem);
                index ++;
                continue;
            }
            if(!check_nest_rule(top, elem, error_info)){    //�����ж�ջ���뵱ǰԪ��Ƕ�׹�ϵ
                return false;
            }
            else{
                push(Tags, elem);
            }
        }
        else{      //���ǩ��ջ��Ԫ�رȽ�
            if(stack_empty(Tags)){    //ջ�ѿգ�˵��ȱ�ٵ�ǰ��ǩ��ǰ��ǩ
                error_info = "��ǩ: " + elem.tag + "ȱ��ǰ��ǩ"+ elem.tag + "";
                return false;
            }
            //ջ����Ԫ��,��ǰ��ǩ��ջ����ǩ�Ƚ�
            if(html_tag_cmp(elem.tag_name, top.tag_name)){     //�����ǰԪ�ص���ջ��Ԫ�أ�ջ��Ԫ�س�ջ
                pop(Tags, top);
            }  
            else{   //ջ��Ԫ�غ͵�ǰԪ�ز�ƥ�䣬���������Ϣ
                error_info = "��ǩ" + top.tag + "ȱ�ٱպϱ�ǩ</" + top.tag_name + ">, ������" + elem.tag; 
                return false;
            }
        }   
        index ++;        //�ڵ����
    }
    return true;
}


void get_htmlpath(sqlist &xpath, std::string path)
{
    size_t start, end;       //path�Ӵ���ʼ�ͽ���λ��
    size_t size = path.length();     //path����
    elemtype elem;
    int count = 1;    
    start = (path[0] == '/' || path[0] == '\\')? 1: 0; 
    while(start < size){
        if(path[start] == ' '){     //�����ո�
            start ++;
            continue;
        }
        end = find_first_delimiter(path, size, start, "/\\");     //xpath�ָ�λ��
        elem.tag_name = path.substr(start, end - start);              //��ǩ����
        elem.type = type_of_tag(elem.tag_name);  
        list_insert(xpath, count, elem);         //����xpath��
        count ++;
        start = end + 1;    //��������
    }
}

void out_tag(elemtype elem, elemtype ftag)
{
    if(elem.type == Text){  //������ı�ֱ�����
        std::cout << elem.tag;
    }
    else if(elem.type == Single){    //������Ապϱ�ǩ����껻��
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

//����������Tag���ɵļ򻯵�XPATH·��path����������ܹ�ƥ���·���Ľڵ��OuterHTML 
bool out_html(sqlist content, sqlist xpath)
{
    int index = 0, xindex = 0;      //content�±�λ��,xpath�±�λ��
    elemtype xtag = xpath.elem[0], elem, top, ftag;      //xpath��ǩ��content��ǩ, tag��ǩ���ǩ
    ftag.type = None;
    stack Tags = nullptr;     //���content�еı�ǩ
    init_stack(Tags);
    int pairs = 0;     //ǰ���ǩ�ɶ���Ŀ
    bool output = false, find = false;
    //����Ǹ�Ŀ¼��������б�ǩ
    if(xpath.length == 0){     
        for(int i = 0; i < content.length; i++){
            elem = content.elem[i];
            out_tag(elem, ftag);
            ftag = elem;
        }
        return true;
    }
    //������Ǹ�Ŀ¼
    while(index < content.length){
        elem = content.elem[index];
        if(index > 0){ftag = content.elem[index - 1];}
        if(pairs){
            if(output){
                out_tag(elem, ftag);
            }
            if(elem.tag[1] == '/'){  //����Ǻ��ǩ
                pairs --;
                
            }    //�����ǰ��ǩ
            else if(elem.type > Single){
                pairs ++;
            }
            index ++;
            continue;
        }
        if(elem.type == Text){      //�����ı�����
            index ++;
            continue;
        }
        if(elem.tag[1] != '/'){    //��������ǩ���Ապϱ�ǩ
            if(elem.tag_name == xtag.tag_name){      //���html��ǰ��ǩtag��xpath·����ǰ��ǩ��ͬ��tag��ջ
                push(Tags, elem);
                index ++;
                xindex ++;
                if(xindex == xpath.length){     //�ҵ���xpath·���£���ӡ����
                    output = true;
                    find = true;
                    pop(Tags, top);
                    xindex --;              //xpath�±����
                    out_tag(elem, ftag);
                    pairs = (elem.type == Single)? 0: 1;
                }
                xtag = xpath.elem[xindex];
                continue;
            }
            else{        //����������ǰ��ǩ����
                pairs = (elem.type == Single)? 0: 1;
                index ++;
                output = false;
                continue;
            }
        }
        else if(!stack_empty(Tags)){       //����Ǻ��ǩ
            get_top(Tags, top);
            if(top.tag_name == elem.tag_name){   //���ǩ��ջ��Ԫ����ͬ������ջ��
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


//���ҳ���з���·��������elements�е������ı�����
bool out_text(sqlist content, sqlist xpath)
{
    int index = 0, xindex = 0;      //content�±�λ��,xpath�±�λ��
    elemtype xtag = xpath.elem[0], elem, top;      //xpath��ǩ��content��ǩ, tag��ǩ���ǩ
    stack Tags = nullptr;     //���content�еı�ǩ
    init_stack(Tags);
    int pairs = 0;     //ǰ���ǩ�ɶ���Ŀ
    bool output = false, find = false;
    //����Ǹ�Ŀ¼����������ı�
    if(xpath.length == 0){     
        for(int i = 0; i < content.length; i++){
            elem = content.elem[i];
            if(elem.tag[0] != '<'){  //������ı�ֱ�����
                std::cout << elem.tag << std::endl << std::endl;
            }
        }
        return true;
    }
    //������Ǹ�Ŀ¼
    while(index < content.length){
        elem = content.elem[index];
        if(pairs){
            if(output && elem.type == Text){
                std::cout << elem.tag << std::endl;
            }
            if(elem.tag[1] == '/'){  //����Ǻ��ǩ
                pairs --;
                
            }    //�����ǰ��ǩ
           else if(elem.type > Single){
                pairs ++;
            }
            index ++;
            continue;
        }
        if(elem.tag[0] != '<'){      //�����ı�����
            index ++;
            continue;
        }
        if(elem.tag[1] != '/'){    //��������ǩ���Ապϱ�ǩ
            if(elem.tag_name == xtag.tag_name){      //���html��ǰ��ǩtag��xpath·����ǰ��ǩ��ͬ��tag��ջ
                push(Tags, elem);
                index ++;
                xindex ++;
                if(xindex == xpath.length){     //�ҵ���xpath·���£���ӡ����
                    pop(Tags, top);
                    xindex --;              //xpath�±����
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
            else{        //����������ǰ��ǩ����
                pairs = (elem.type == Single)? 0: 1;
                index ++;
                output = false;
                continue;
            }
        }
        
        else if(!stack_empty(Tags)){       //����Ǻ��ǩ
            get_top(Tags, top);
            if(top.tag_name == elem.tag_name){   //���ǩ��ջ��Ԫ����ͬ������ջ��
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
    int exit = 1;      //���������Ƿ��˳�
    //�����õ��ı���
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
        //����ָ��
        std::cout << "Command/input order> " ;      //ѡ�����
        if (!(std::cin >> input)) {  // ����������
            std::cin.clear();         // ��������־
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // ���Դ�������ֱ����һ�����з�
            std::cout << "------Please check if the input is correct and re-enter the command-----" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));    //��ͣ1��
            continue;            // ������һ��ѭ������
        }
        std::cout << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //��ջ�����
        Order order = get_order(input);
        //���в���
        switch (order)
        {
        case Exit:
            exit = 0;
            destroy_stack(result);
            destroy_list(xpath);
            std::cout << "........" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));    //��ͣ1��
            std::cout << "----------------alreadly exit----------------" << std::endl;
            break;
        case LoadHTML:
            clear_list(content);
            // std::cout << "Command/�������ļ�·��> " ;
            // std::getline(std::cin, path);
            
            if(load_html_by_file(content, filepath)){
                std::cout << "Load successfully" << std::endl;
                //list_print(content);
            }
            else{
                std::cout << "Loading failed, please check if the file exists or if the input is incorrect" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));    //��ͣ1��
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
            clear_list(xpath);       //���֮ǰ��xpath��
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
            clear_list(xpath);        //���֮ǰ��xpath��
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
            std::this_thread::sleep_for(std::chrono::seconds(1));    //��ͣ1��
            continue;
        }
        if (exit){   //���������ֱ�Ӵ�ӡ�˵���Ӱ���Ķ������˳������⣩
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
    init_stack(tag);      //��ʼ��tagջ��content˳���
    init_list(content, INIT_SIZE);

    Menu(content, tag, filepath);

    destroy_stack(tag);
    destroy_list(content);
    return 0;
}






