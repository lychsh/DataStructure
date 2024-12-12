#include <iostream>
#include <unordered_map>
#include<fstream>
#include<sstream>
#include <locale>
#include <codecvt>
#include <algorithm> // std::sor
#include<string>
#include<vector>
#include <climits> // For INT_MAX
#include<string>
#include<stack>
#include <cctype>



/*
通过Dijkstra’s algorithm算法及其扩展实现N最短路径中文切词
节点表示词的开始和结束位置（不包含词本身）
边表示词的信息
图为有向无环图DAG，使用邻接表存储
*/

#define MAX_VERTEX_NUM 1000         //图最大结点个数（句子最大长度+1）

//词典中的每个条目, 弧的信息
typedef struct WordEntry {
    std::wstring word;               // 词
    int frequency;                  // 词频
    std::wstring part_of_speech;     // 词性
    WordEntry() : frequency(0), word(L""), part_of_speech(L"") {}
} WordEntry, Info;


typedef std::unordered_map<std::wstring, WordEntry> WordDict;     //词典

//表格中的每条记录
typedef struct TableRecord {
    int path;         //N种路径编号
    int len;        //起点到当前结点的路径长度
    int pre;        //前驱
    int pre_path;     //前驱结点的记录表中对应路径的编号
    TableRecord(int n, int l, int p, int pn) : path(n), len(l), pre(p), pre_path(pn) {}
}TableRecord;

//每个节点处的表格
typedef struct Table {
    int rownum;                           //表格行数
    std::vector<TableRecord> records;   //表格的行记录
}Table;

//弧的定义
typedef struct ArcNode {  
    int adjvex;       // 该弧所指向的顶点的位置
    WordEntry info;         // 该弧相关信息
} ArcNode;

//邻接矩阵
typedef std::unordered_map<int, std::unordered_map<int, Info>> AdjMatrix; 

//有向无环图
typedef struct  DAGraph{  
    AdjMatrix  matrix;       //邻接表
    int vexnum, arcnum;      //结点，弧的数目
} DAGraph;


const std::string dict_path = "Experiments\\exp04\\dict.txt";        // 词典文件路径


//将宽字符转换为窄字符
std::wstring s2ws(const std::string& str) {
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.from_bytes(str);
}

//将窄字符转换为宽字符
std::string ws2s(const std::wstring& wstr) {
    using convert_typeX = std::codecvt_utf8<wchar_t>;
    std::wstring_convert<convert_typeX, wchar_t> converterX;
    return converterX.to_bytes(wstr);
}

// 构建词典
void create_dict(const std::string dict_file, WordDict& word_dict)
{
    std::ifstream file(dict_file);

    if (!file.is_open()) {       //文件打开失败
        std::wcerr << L"Unable to open file" << std::endl;
        return;
    }

    std::wstring wline;
    std::string line;
    while (std::getline(file, line)) {     // 依次读取每行，构建词典
        wline = s2ws(line);                //将窄字符转换为宽字符
        std::wistringstream iss(wline);
        WordEntry entry;
        if (!(iss >> entry.word >> entry.frequency >> entry.part_of_speech)) {    // 输入词、词频、词性
            std::wcerr << L"Error parsing line: " << wline << std::endl;
            continue;
        }
        word_dict[entry.word] = entry;       //加入到词典
    }

    file.close();
}

// 构建基于字典的邻接矩阵
void build_graph(std::wstring str, DAGraph &G, WordDict& word_dict)
{
    size_t len = str.length();     // 语句长度
    size_t i, start, end;
    G.vexnum = len + 1;            //结点个数为字数+1
    G.arcnum = len;                //初始边个数为字数
    for (i = 0; i < len; i++) {     // 初始每个字之间的权重为1
        G.matrix[i][i + 1].frequency = 1;
    }
    std::wstring s;
    for (start = 0; start < len; start++) {     // 遍历所有子串
        for (end = start + 1; end <= len; end++) {
            s = str.substr(start, end - start);
            if (word_dict.find(s) != word_dict.end()) {    //切词，在字典中查询，存在则在词的头尾结点连接一条边
                G.vexnum ++;                               //边的数目+1
                G.matrix[start][end] = word_dict[s];       //边的信息
            }
        }
    }
}


//表格中两条记录的比较
bool WordRecord_equal(TableRecord tr1, TableRecord tr2)
{
    if (tr1.pre == tr2.pre && tr1.path == tr2.path 
        && tr1.pre_path == tr2.pre_path && tr1.len == tr2.len) {
        return true;
    }
    return false;
}

//保留N种最短路径
void keep_n_minpath(Table& table, int N)
{
    //按路径长度进行排序
    std::sort(table.records.begin(), table.records.end(), [](TableRecord a, TableRecord b) {
        return a.len < b.len;
        });
    int path_count = 0;       //路径种数
    int last = -1, last_len = -1;  //前N种路径最后一个路径的行下标，及路径长度
    int i = 0;
    for (; i < table.rownum; i++) {    //统计table里的路径种数
        if (table.records[i].len != last_len) {
            last_len = table.records[i].len;
            path_count++;
            if (path_count > N) {    //保留前N种
                last = i;
                break;
            }
        }
    }
    //去除多余的路径
    if (last != -1) {
        table.records.erase(table.records.begin() + last + 1, table.records.end());
    }
    //更新路径编号
    int index = 0;
    last_len = -1;
    i = 0;
    for (TableRecord& tr : table.records) {
        //检查是否有重复路径，有则删除
        if (i > 0 && WordRecord_equal(table.records[i], table.records[i - 1])) {
            table.records.erase(table.records.begin() + i);
        }
        if (tr.len != last_len) {
            last_len = tr.len;
            index++;
        }
        table.records[i].path = index;    //路径编号
        i++;
    }
}


void get_tables(std::vector<Table>& tables, DAGraph& G, int N)
{
    int num = G.vexnum;

    TableRecord record(1, 0, 0, 0);    //第0个节点表格占位
    Table table;
    table.records.push_back(record);
    table.rownum = 1;
    tables.push_back(table);
    int i;
    for (int cur = 1; cur <= num; cur++) {       //当前节点
        i = 0;
        Table table;
        for (int pre = 0; pre < cur; pre++) {    //当前节点前面的所有结点
            if (G.matrix.find(pre) != G.matrix.end() 
                && G.matrix[pre].find(cur) != G.matrix[pre].end()) {    //如果前面的结点是当前节点的前驱
                for (TableRecord& tr : tables[pre].records) {
                    TableRecord record(i, tr.len + G.matrix[pre][cur].frequency, pre, tr.path);  //记录到达当前结点的路径
                    table.rownum ++;
                    table.records.push_back(record);
                }
            }
        }
        keep_n_minpath(table, N);     //保留前N种最短路径
        tables.push_back(table);
    }
}


//回溯函数
void core_retro(std::wstring sentence, int cur, TableRecord record, 
                std::vector<Table> &tables, std::stack<std::wstring> &res, std::vector<std::stack<std::wstring>>& out)
{
    res.push(sentence.substr(record.pre, cur));
    if(record.pre == 0){      //记录回溯路径
        out.push_back(res);
        return ;
    }
    Table table = tables[record.pre];
    for(int i = 0; i < table.rownum; i++){      //往前回溯
        if(table.records[i].path == record.pre_path){        
            core_retro(sentence, record.pre, table.records[i], tables, res, out);
        }
        res.pop();
    } 
    return;
}

//对记录表进行回溯
void retro_back(std::wstring sentence, std::vector<Table> &tables, int N)
{
    int count = 0;
    int last_len = -1;
    int slen = sentence.length();    //句子长度        
    int cur;
    for (TableRecord& tr : tables[slen].records) {
        if(tr.len != last_len){
            last_len = tr.len;
            std::cout << "长度为: " << tr.len << std::endl;
            count ++;
            if(count > N){
                break;
            }
        }
        //开始回溯
        std::stack<std::wstring> res;     //用栈，回溯后方便输出
        cur = slen;
        std::vector<std::stack<std::wstring>> out;
        core_retro(sentence, cur, tr, tables, res, out);
        for(auto& o: out){       //依次输出路径
            std::cout << "[ " << ws2s(o.top()) ;   
            o.pop();
            while(!o.empty()){
                std::cout <<  " / " << ws2s(o.top()) ; 
                o.pop();
            }
            std::cout << "]" << std::endl;
        }
    }
}


//判断邻接表中两个结点间是否有弧
int get_weight(int v, int w, DAGraph &G)
{
    if( G.matrix.find(v) != G.matrix.end()    //v0到vi存在弧
            && G.matrix[v].find(w) != G.matrix[v].end()){
        return G.matrix[v][w].frequency;
    }
    return INT_MAX;
}

void Dijkstra(DAGraph &G, std::wstring sentence)
{
    int len = sentence.length();
    std::vector<int> D(len, INT_MAX);    //D[i]表示从v0到vi的最短路径,初值为INT_MAX
    std::vector<int> Path(len);  //Path[i]表示vi的前驱结点
    std::vector<int> visited(len, 0);   //已访问过的结点,初始状态为0

    //D[i]初始值为v0到vi的弧的权值
    D[0] = 0;       //v0到自身的距离为0
    for(int i = 1; i < len; i++){
        D[i] = get_weight(0, i, G);
    }
    int nextnode;  //下一条最短路径达到的结点
    int nextedge;  //下一条最短边长度
    visited[0] = 1;
    int i, j;
    for(i = 1; i < len; i++){
        //找出下一条最短边对对应到达的顶点
        nextedge = INT_MAX;
        for(j = 0; j < len; j++){
            if(!visited[j] && D[j] < nextedge){
                nextnode = j;
                nextedge = D[j];
            }
        }
        visited[nextnode] = 1;    //标记访问状态
        //更新路径
        for(j = 0; j < len; j++){
            //如果nextnode结点到原点的值加上nextnode到下一个未访问结点vj的值小于v0到vj的值
            if(!visited[j] + get_weight(nextnode, j, G) + D[nextnode] < D[j]){
                D[j] = get_weight(nextnode, j, G) + D[nextnode];
                Path[j] = nextnode;   //更新前驱
            }
        }
    }
    int pre = Path[len - 1];
    int cur = len - 1;
    std::cout << "最短路径长度为: " << D[len - 1] << std::endl;
    while(cur){
        std::cout << ws2s(sentence.substr(pre, cur - pre)) << " / ";
        pre = Path[pre];
        cur = pre;
    }
}


//菜单函数
void Menu()
{
    bool exit = false;
    std::string input;
    std::wstring sentence;
    int N;
    DAGraph G;
    std::vector<Table> tables;
    std::string strN;
    //创建字典
    WordDict word_dict;
    create_dict(dict_path, word_dict);
    std::cout << "中文切词器, 加载成功..." << std::endl;
    std::cout << "You can input 'exit' to terminate the program" << std::endl << std::endl;
    while(true)
    {   
        std::cout << "1.Dijkstra" << std::endl;
        std::cout << "2.N最短路径" << std::endl;
        std::cout << "选择切词方式(1/2): ";
        
        std::getline(std::cin, input);
        if(input == "exit"){     //输入退出指令，结束程序
            break;
        }
        else if(input == "1"){
            std::cout << "请输入句子: ";
            std::getline(std::cin, input);
            std::cout << "请输入N: " ;
            std::getline(std::cin, strN);
            //如果strk不是数字或者为空，返回错误
            while(strN.empty() || !std::all_of(strN.begin(), strN.end(), ::isdigit)){
                std::cout <<"\033[31mDigit Error: str'" << strN <<  "' is not a number\033[0m" << std::endl;
                std::cout << "请重新输入N: " ;
            }
            N = std::stoi(strN);     //将字符串转为数字
            sentence = s2ws(input);    //将输入转换为宽字符串
            build_graph(sentence, G, word_dict);      //构造有向无环图DAG
            get_tables(tables, G, N);                   //获得信息记录表
            retro_back(sentence, tables, N);            //回溯输出N种最短路径
        }
        else if(input == "2"){
            std::cout << "请输入句子: ";
            std::getline(std::cin, input);
            build_graph(sentence, G, word_dict);
            Dijkstra(G, sentence);
        }
        else {
            std::cout << "输入错误, 请重新输入..." << std::endl;
        }
        
    }
}

int main() {
    Menu();
    return 0;
}
