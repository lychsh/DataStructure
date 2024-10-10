#include"LinkList.h"
#include <limits>  

void Menu(LinkList &head, LinkList &pattern)
{
    int exit = 1;
    //�����õ��ı���
    LinkList mergehead = nullptr;
    LinkList result = new LNode;
    create_node(result, "");
    int pos = 0;      //���룬ɾ��λ��  
    ElemType data;      //ɾ��������,����ĵ���
    ElemType over;   
    while(exit)
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
            exit = 0;
            std::cout << "----------------���˳�----------------" << std::endl;
            break;
        case 1:
            std::cout << "�����ַ���, ��֮���Կո�򶺺Ÿ�����" << std::endl;
            create_List_from_input(head);
            //create_List_from_file(head, "D:\\CODE_REPOSITORY\\DataStructure2\\Homework\\homewrok1\\Chinese_Large.txt");
            break;
        case 2:
            std::cout <<"�ַ������£�" << std::endl;
            List_print(head);
            break;
        case 3:
            std::cout << "��������Ҫ����ĵ��ʺ�λ��,�Կո���� "<<std::endl;
            std::cin >> data >> pos;
            if(pos <= 0 ){      //�����ʽ����
                std::cout << "---�����ʽ����----" << std::endl;
                break;
            }
            if(List_insert(head, pos, data)){
                std::cout << "----�������----" << std::endl;
            }
            else{
                std::cout << "----����ʧ��----" << std::endl;
            }
            break;
        case 4:
            std::cout << "��������Ҫɾ���ĵ��ʵ�λ��"<<std::endl;
            std::cin >> pos;

            if (List_delete(head, pos, data)){
                std::cout << "-----ɾ���ɹ�-----" << std::endl;
            }
            else{
                std::cout << "------ɾ��ʧ��-----" << std::endl;
            }
            break; 
        case 5:
            List_reverse(head);    //�����ַ���
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
            std::cout << "�ַ��������ĵ�����Ϊ��" << List_length(head) << std::endl;
            break;
        case 8:
            std::cout << "������Ҫ���ҵ��ַ���" << std::endl;
            create_List_from_input(pattern);
            if(search_subList(head, pattern, result)){
                std::cout << "�ַ����д����Ӵ���" << std::endl;
                List_print(pattern);
                std::cout << "λ������Ϊ��" << std::endl;
                List_print(result);
                clear_List(result);   //��ս��
            }
            else{
                std::cout << "�ַ����в������Ӵ���" << std::endl;
                List_print(pattern);
            }
            std::cout << std::endl;
            break;
        case 9:
            std::cout << "������Ҫ�ϲ����ַ�����" << std::endl;
            create_List_from_input(pattern);
            mergehead = new_merge_List(head, pattern);
            std::cout << "�ϲ�����ַ���Ϊ��" << std::endl;
            List_print(mergehead);
            break; 
        default:
            std::cout << "------ѡ�����������ѡ��-----" << std::endl;
            break;
        }
        if (exit){   //���������ֱ�Ӵ�ӡ�˵���Ӱ���Ķ������˳������⣩
            std::cout << "----�밴Enter����ִ�в���----" << std::endl;
            std::getline(std::cin, over);
            std::cout << std::endl;
        }
    }
    destroy_List(mergehead);    //���ٺϲ�����
    destroy_List(result);
}


int main()
{
    LinkList head = nullptr;      //����
    LinkList pattern = nullptr;   //���ң��ϲ����Ӵ����ַ���
    Menu(head, pattern);
    destroy_List(head);
    destroy_List(pattern);
    return 0;
}

