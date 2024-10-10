#include"LinkList.h"
#include <limits>  

void Menu(LinkList &head, LinkList &pattern)
{
    int exit = 1;
    //操作用到的变量
    LinkList mergehead = nullptr;
    LinkList result = new LNode;
    create_node(result, "");
    int pos = 0;      //插入，删除位置  
    ElemType data;      //删除的数据,插入的单词
    ElemType over;   
    while(exit)
    {
        std::cout << "--------------请根据操作输入相应的数字--------------" << std::endl; 
        std::cout << "**********************0.退出*************************" << std::endl; 
        std::cout << "*******************1.输入字符串**********************" << std::endl; 
        std::cout << "*******************2.打印字符串**********************" << std::endl; 
        std::cout << "*******************3.插入单词************************" << std::endl; 
        std::cout << "*******************4.删除单词************************" << std::endl; 
        std::cout << "*******************5.字符串倒置**********************" << std::endl; 
        std::cout << "*******************6.回文串判断**********************" << std::endl; 
        std::cout << "*******************7.字符串单词计数******************" << std::endl; 
        std::cout << "*******************8.字符串查找**********************" << std::endl; 
        std::cout << "*******************9.合并字符串**********************" << std::endl; 
        int choose = 0;
        std::cout << "输入你的选择：" ;      //选择操作
        if (!(std::cin >> choose)) {  // 如果输入不是一个整数
        std::cin.clear();         // 清除错误标志
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // 忽略错误输入直到下一个换行符
        std::cout << "------请输入0-9数字选择操作-----" << std::endl;
        continue;            // 继续下一次循环迭代
        }
        std::cout << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //清空缓冲区

        switch (choose)
        {
        case 0:
            exit = 0;
            std::cout << "----------------已退出----------------" << std::endl;
            break;
        case 1:
            std::cout << "输入字符串, 词之间以空格或逗号隔开：" << std::endl;
            create_List_from_input(head);
            //create_List_from_file(head, "D:\\CODE_REPOSITORY\\DataStructure2\\Homework\\homewrok1\\Chinese_Large.txt");
            break;
        case 2:
            std::cout <<"字符串如下：" << std::endl;
            List_print(head);
            break;
        case 3:
            std::cout << "请输入需要插入的单词和位置,以空格隔开 "<<std::endl;
            std::cin >> data >> pos;
            if(pos <= 0 ){      //输入格式有误
                std::cout << "---输入格式错误----" << std::endl;
                break;
            }
            if(List_insert(head, pos, data)){
                std::cout << "----插入完成----" << std::endl;
            }
            else{
                std::cout << "----插入失败----" << std::endl;
            }
            break;
        case 4:
            std::cout << "请输入需要删除的单词的位置"<<std::endl;
            std::cin >> pos;

            if (List_delete(head, pos, data)){
                std::cout << "-----删除成功-----" << std::endl;
            }
            else{
                std::cout << "------删除失败-----" << std::endl;
            }
            break; 
        case 5:
            List_reverse(head);    //倒置字符串
            break; 
        case 6:
            if (is_palindrome(head)){
                std::cout << "----字符串是回文串----" << std::endl;
            }
            else{
                std::cout << "----字符串不是回文串----" << std::endl;
            }
            break;
        case 7:
            std::cout << "字符串包含的单词数为：" << List_length(head) << std::endl;
            break;
        case 8:
            std::cout << "请输入要查找的字符串" << std::endl;
            create_List_from_input(pattern);
            if(search_subList(head, pattern, result)){
                std::cout << "字符串中存在子串：" << std::endl;
                List_print(pattern);
                std::cout << "位置依次为：" << std::endl;
                List_print(result);
                clear_List(result);   //清空结果
            }
            else{
                std::cout << "字符串中不存在子串：" << std::endl;
                List_print(pattern);
            }
            std::cout << std::endl;
            break;
        case 9:
            std::cout << "请输入要合并的字符串：" << std::endl;
            create_List_from_input(pattern);
            mergehead = new_merge_List(head, pattern);
            std::cout << "合并后的字符串为：" << std::endl;
            List_print(mergehead);
            break; 
        default:
            std::cout << "------选择错误，请重新选择-----" << std::endl;
            break;
        }
        if (exit){   //避免操作完直接打印菜单，影响阅读（除退出操作外）
            std::cout << "----请按Enter继续执行操作----" << std::endl;
            std::getline(std::cin, over);
            std::cout << std::endl;
        }
    }
    destroy_List(mergehead);    //销毁合并链表
    destroy_List(result);
}


int main()
{
    LinkList head = nullptr;      //链表
    LinkList pattern = nullptr;   //查找，合并的子串或字符串
    Menu(head, pattern);
    destroy_List(head);
    destroy_List(pattern);
    return 0;
}

