#include"LinkList.h"
#include<limits>  
using namespace std;

void Menu(LinkList &Head, LinkList &pattern)
{
    int exit = 1;
    //操作用到的变量
    LinkList MergeHead = NULL;
    int pos = 0;      //插入，删除位置  
    ElemType data;      //删除的数据
    data = NULL;
    char str[BUFFER_SIZE]; //插入的单词
    int len = 0;        //字符串长度
    string over;
    while(exit)
    {
        cout << "--------------请根据操作输入相应的数字--------------" << endl; 
        cout << "**********************0.退出*************************" << endl; 
        cout << "*******************1.输入字符串**********************" << endl; 
        cout << "*******************2.打印字符串**********************" << endl; 
        cout << "*******************3.插入单词************************" << endl; 
        cout << "*******************4.删除单词************************" << endl; 
        cout << "*******************5.字符串倒置**********************" << endl; 
        cout << "*******************6.回文串判断**********************" << endl; 
        cout << "*******************7.字符串单词计数******************" << endl; 
        cout << "*******************8.字符串查找**********************" << endl; 
        cout << "*******************9.合并字符串**********************" << endl; 
        int choose = 0;
        cout << "输入你的选择：" ;      //选择操作
        if (!(cin >> choose)) {  // 如果输入不是一个整数
        cin.clear();         // 清除错误标志
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // 忽略错误输入直到下一个换行符
        cout << "------请输入0-9数字选择操作-----" << endl;
        continue;            // 继续下一次循环迭代
        }
        cout << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  //清空缓冲区

        switch (choose)
        {
        case 0:
            exit = 0;
            if(data){
                free(data);
            }
            cout << "----------------已退出----------------" << endl;
            break;
        case 1:
        cout << "输入字符串, 词之间以空格或逗号隔开：" << endl;
            CreateListFromInput(Head);
            break;
        case 2:
            cout <<"字符串如下：" << endl;
            ListPrint(Head);
            break;
        case 3:
            cout << "请输入需要插入的单词和位置,以空格隔开 "<<endl;
            cin >> str >> pos;
            len = datalen(str);
            data = (ElemType)malloc(len);
            if(pos > 0){
                datacpy(data, str);
            }
            else{      //输入格式有误
                cout << "---输入格式错误----" << endl;
                free(data);
                data = NULL;
                break;
            }
            if(ListInsert(Head, pos, data)){
                cout << "----插入完成----" << endl;
            }
            else{
                cout << "----插入失败----" << endl;
            }
            free(data);
            data = NULL;
            break;
        case 4:
            cout << "请输入需要删除的单词的位置"<<endl;
            cin >> pos;

            if (ListDelete(Head, pos, data)){
                cout << "-----删除成功-----" << endl;
            }
            else{
                 cout << "------删除失败-----" << endl;
            }
            free(data);
            data = NULL;
            break; 
        case 5:
            ListReverse(Head);    //倒置字符串
            break; 
        case 6:
            if (IsPalindrome(Head)){
                cout << "----字符串是回文串----" << endl;
            }
            else{
                cout << "----字符串不是回文串----" << endl;
            }
            break;
        case 7:
            len = ListLength(Head);
            cout << "字符串包含的单词数为：" << len << endl;
            break;
        case 8:
            cout << "请输入要查找的字符串" << endl;
            CreateListFromInput(pattern);
            if(SearchSubList(Head, pattern)){
                cout << "字符串中存在子串：" << endl;
                ListPrint(pattern);
            }
            else{
                cout << "字符串中不存在子串：" << endl;
                ListPrint(pattern);
            }
            //DestroyList(pattern);   //销毁匹配串
            cout << endl;
            break;
        case 9:
            cout << "请输入要合并的字符串：" << endl;
            CreateListFromInput(pattern);
            MergeHead = NewMergeList(Head, pattern);
            cout << "合并后的字符串为：" << endl;
            ListPrint(MergeHead);
            break; 
        default:
            cout << "------选择错误，请重新选择-----" << endl;
            break;
        }
        if (exit){   //避免操作完直接打印菜单，影响阅读（除退出操作外）
            cout << "----请按Enter继续执行操作----" << endl;
            getline(cin, over);
            cout << endl;
        }
    }
    DestroyList(MergeHead);    //销毁合并链表
}


int main()
{
    //setlocale(LC_ALL, "zh_CN");
    LinkList Head = NULL;      //链表
    LinkList pattern = NULL;   //查找，合并的子串或字符串
    Menu(Head, pattern);
    DestroyList(Head);
    DestroyList(pattern);

    return 0;
}