#include"SqList.h"
#include <iostream>
#include <limits>  
using namespace std;

void Menu(SqList &L, SqList &pattern)
{
    int exit = 1;
    //操作用到的变量
    int pos = 0;      //插入，删除位置  
    ElemType data;      //删除的数据
    data.str = NULL;
    char str[BUFFER_SIZE]; //插入的单词
    int len = 0;        //字符串长度
    bool res;   //操作结果
    while(exit)
    {
        cout << "--------------请根据操作输入相应的数字--------------" << endl; 
        cout << "**********************0.退出***********************" << endl; 
        cout << "*******************1.输入字符串*********************" << endl; 
        cout << "*******************2.打印字符串*********************" << endl; 
        cout << "*******************3.插入单词***********************" << endl; 
        cout << "*******************4.删除单词***********************" << endl; 
        cout << "*******************5.字符串倒置**********************" << endl; 
        cout << "*******************6.回文串判断**********************" << endl; 
        cout << "*******************7.字符串单词计数******************" << endl; 
        cout << "*******************8.字符串查找**********************" << endl; 
        cout << "*******************9.合并字符串**********************" << endl; 
        int choose = 0;
        cout << "输入你的选择：" ;      //选择操作
        cin >> choose;
        cout << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  //清空缓冲区

        switch (choose)
        {
        case 0:
            exit = 0;
            if(data.str){
                free(data.str);
                data.str = NULL;
            }
            cout << "----------------已退出----------------" << endl;
            break;
        case 1:
            cout << "输入字符串, 词之间以空格或逗号隔开：" << endl;
            ListInput(L);
            cout << "输入成功" << endl;
            break;
        case 2:
            cout <<"字符串如下：" << endl;
            ListPrint(L);
            cout << endl;
            break;
        case 3:
            cout << "请输入需要插入的单词和位置,以空格隔开 "<<endl;
            
            cin >> str >> pos;
            data.str = (char*)malloc(strlen(str) + 1);
            strcpy(data.str, str);
            if(ListInsert(L, pos, data)){
                cout << "----插入完成----" << endl;
            }
            else{
                cout << "----插入失败----" << endl;
            }
            free(data.str);   //释放暂存节点
            data.str = NULL;
            break;
        case 4:
            cout << "请输入需要删除的单词的位置"<<endl;
            cin >> pos;

            if (ListDelete(L, pos, data)){
                cout << "-----删除成功-----" << endl;
            }
            else{
                 cout << "------删除失败-----" << endl;
            }
            break; 
        case 5:
            ListReverse(L);
            break; 
        case 6:
            if (IsPalindrome(L)){
                cout << "----字符串是回文串----" << endl;
            }
            else{
                cout << "----字符串不是回文串----" << endl;
            }
            break;
        case 7:
            len = ListLength(L);
            cout << "字符串包含的单词数为：" << len << endl;
            break;
        case 8:
            ListInput(pattern);
            if(SearchSubList(L, pattern)){
                cout << "字符串中存在子串：" << endl;
            }
            else{
                cout << "字符串中不存在子串："<< endl;
            }
            break;
        case 9:
            if(pattern.elem != NULL){
                DestroyList(pattern);
            }
            ListInput(pattern);
            MergeList(L, pattern);
            break; 
        default:
            cout << "------选择错误，请重新选择-----" << endl;
            break;
        }
    }
}


int main()
{
    SqList L;      //链表
    SqList pattern;   //查找，合并的子串或字符串
    InitList(L);
    InitList(pattern);
    Menu(L, pattern);
    //销毁
    DestroyList(L);
    DestroyList(pattern);
    return 0;
}