#include"stack.h"

// int main()
// {
//     stack S;
//     init_stack(S);
//     elemtype node;
//     int choose;
//     int exit = 1;
//     while(exit){
//         std::cout << "--------------请根据操作输入相应的数字--------------" << std::endl; 
//         std::cout << "**********************1.输入************************" << std::endl; 
//         std::cout << "**********************2.弹出************************" << std::endl; 
//         std::cout << "**********************3.压入************************" << std::endl; 
//         std::cout << "**********************4.获取最小值*******************" << std::endl; 
//         std::cout << "**********************5.退出*************************" << std::endl; 
//         std::cout << "请输入：" << std::endl;
//         std::cin >> choose;
//         switch(choose){
//             case 1:
//                 std::cout << "输入插入的值, 空格隔开" << std::endl;
//                 while(std::cin >> node && node != EOF){
//                     push(S, node);
//                 }
//                 break;
//             case 2:
//                 std::cout << "栈顶元素为" << std::endl;
//                 pop(S, node);
//                 std::cout << node << std::endl;
//                 break;
//             case 3:
//                 std::cout << "输入插入的值：" << std::endl;
//                 std::cin >> node;
//                 push(S, node);
//                 break;
//             case 4:
//                 std::cout << "最小元素为：" << std::endl;
//                 get_min(S, node);
//                 std::cout << node << std::endl;
//                 break;
//             case 5:
//             destroy_stack(S);
//             exit = 0;
//             break;
//         }
//     }
    
// }