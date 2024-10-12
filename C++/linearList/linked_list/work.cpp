#include"linklist.h"
#include <limits>  

void Menu(linklist &head, linklist &pattern)
{
    int exit = 0;
    //�����õ��ı���
    linklist mergehead = nullptr;      //�ϲ�����ַ���
    linklist result;       //��Ų����Ľ��
    create_node(result, "");     
    int pos = 0;      //���룬ɾ��λ��  
    int count = 0;   //ɾ��Ԫ�ظ���
    elemtype data;      //ɾ��������,����ĵ���
    elemtype enter;      //enter�������ݣ���֤����������������
    while(!exit)
    {
        std::cout << "--------------����ݲ���������Ӧ������--------------" << std::endl; 
        std::cout << "**********************0.�˳�*************************" << std::endl; 
        std::cout << "*******************1.�����ַ���**********************" << std::endl; 
        std::cout << "*******************2.��ӡ�ַ���**********************" << std::endl; 
        std::cout << "*******************3.���뵥��************************" << std::endl; 
        std::cout << "*******************4.ɾ������************************" << std::endl; 
        std::cout << "*******************5.�ַ�������**********************" << std::endl; 
        std::cout << "*******************6.���Ĵ��ж�**********************" << std::endl; 
        std::cout << "*******************7.�ַ������ʼ���******************" << std::endl; 
        std::cout << "*******************8.�ַ�������**********************" << std::endl; 
        std::cout << "*******************9.�ϲ��ַ���**********************" << std::endl; 
        int choose = 0;
        std::cout << "�������ѡ��" ;      //ѡ�����
        if (!(std::cin >> choose)) {  // ������벻��һ������
        std::cin.clear();         // ��������־
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // ���Դ�������ֱ����һ�����з�
        std::cout << "------������0-9����ѡ�����-----" << std::endl;
        continue;            // ������һ��ѭ������
        }
        std::cout << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  //��ջ�����

        switch (choose)
        {
        case 0:
            exit = 1;
            std::cout << "----------------���˳�----------------" << std::endl;
            break;
        case 1:
            std::cout << "�����ַ���, ��֮���Կո�򶺺Ÿ�����" << std::endl;
            create_list_from_input(head);
            //create_list_from_file(head, "D:\\CODE_REPOSITORY\\DataStructure\\Homework\\ʵ��01\\Chinese_Large.txt");
            break;
        case 2:
            std::cout <<"�ַ������£�" << std::endl;
            list_print(head);
            break;
        case 3:
            std::cout << "��������Ҫ����ĵ���, �Կո���� "<<std::endl;
            create_list_from_input(pattern);
            std::cout << "��������Ҫ�����λ��" << std::endl;
            std::cin >> pos;
            if(pos <= 0 || list_empty(pattern)){      //��������
                std::cout << "---�������,������ѡ��----" << std::endl;
                break;
            }
            if(list_insert_words(head, pos, pattern)){
                std::cout << "----�������----" << std::endl;
                pattern->next = nullptr;
            }
            else{
                std::cout << "----����ʧ��----" << std::endl;
            }
            break;
        case 4:
            std::cout << "��������Ҫɾ���ĵ��ʵ�λ�ú͸���, �ո�ָ�"<<std::endl;
            if(! (std::cin >> pos >> count) ){    //��������
                std::cout << "---�������, ������ѡ��----" << std::endl;
                break;
            } 
            clear_list(result);   //�����һ�β����Ľ��
            if (list_delete_words(head, pos, count, result)){
                std::cout << "-----ɾ���ɹ�-----" << std::endl;
                std::cout << "ɾ������Ϊ: " << std::endl;
                list_print(result);
                clear_list(result);    //���ɾ�����
            }
            else{
                std::cout << "------ɾ��ʧ��-----" << std::endl;
            }
            break; 
        case 5:
            list_reverse(head);    //�����ַ���
            break; 
        case 6:
            if (is_palindrome(head)){
                std::cout << "----�ַ����ǻ��Ĵ�----" << std::endl;
            }
            else{
                std::cout << "----�ַ������ǻ��Ĵ�----" << std::endl;
            }
            break;
        case 7:
            std::cout << "�ַ��������ĵ�����Ϊ��" << list_length(head) << std::endl;
            break;
        case 8:
            std::cout << "������Ҫ���ҵ��ַ���" << std::endl;
            create_list_from_input(pattern);
            if(search_sublist(head, pattern, result)){
                std::cout << "�ַ����д����Ӵ���" << std::endl;
                list_print(pattern);
                std::cout << "λ������Ϊ��" << std::endl;
                list_print(result);
                clear_list(result);   //��ս��
            }
            else{
                std::cout << "�ַ����в������Ӵ���" << std::endl;
                list_print(pattern);
            }
            std::cout << std::endl;
            break;
        case 9:
            std::cout << "������Ҫ�ϲ����ַ�����" << std::endl;
            create_list_from_input(pattern);
            mergehead = new_merge_list(head, pattern);
            std::cout << "�ϲ�����ַ���Ϊ��" << std::endl;
            list_print(mergehead);
            destroy_list(mergehead);   //���ٺϲ�����
            break; 
        default:
            std::cout << "------ѡ�����������ѡ��-----" << std::endl;
            break;
        }
        if (!exit){   //���������ֱ�Ӵ�ӡ�˵���Ӱ���Ķ������˳������⣩
            std::cout << "----�밴Enter����ִ�в���----" << std::endl;
            std::getline(std::cin, enter);
            std::cout << std::endl;
        }
    }
    destroy_list(mergehead);    //���ٺϲ�����
    destroy_list(result);
}


int main()
{
    linklist head = nullptr;      //����
    linklist pattern = nullptr;   //���ң��ϲ����Ӵ����ַ���
    Menu(head, pattern);
    destroy_list(head);
    destroy_list(pattern);
    return 0;
}

