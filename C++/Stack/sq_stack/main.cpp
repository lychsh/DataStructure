#include"stack.h"
#include <limits>  

void Menu(stack &S)
{
    int exit = 0;
    //操作用到的变量
    stack result;
    init_stack(result); 
    std::string enter;

    while(!exit)
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
            
            break;
        case 2:
            std::cout <<"字符串如下：" << std::endl;
            
            break;
        case 3:
            std::cout << "请输入需要插入的单词和位置,以空格隔开 "<<std::endl;
           
            
        case 4:
            
        case 5:
           
            break; 
        case 6:
            
            break;
        case 7:
            std::cout << "字符串包含的单词数为：" << stack_length(S) << std::endl;
            break;
        case 8:
            
           
            break;
        case 9:
            
            break; 
        default:
            std::cout << "------选择错误，请重新选择-----" << std::endl;
            break;
        }
        if (exit){   //避免操作完直接打印菜单，影响阅读（除退出操作外）
            std::cout << "----请按Enter继续执行操作----" << std::endl;
            std::getline(std::cin, enter);
            std::cout << std::endl;
        }
    }
    destroy_stack(result);
}


int main()
{
    stack S;      
    Menu(S);
    destroy_stack(S);
    return 0;
}

