/*
 * @Description: 主函数
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-08 20:54:00
 * @LastEditTime: 2022-08-25 18:07:31
 * @endcode: UTF-8
 */
#include "def.h"
#include "f.h"

int main()
{
    int op = 1;
    char fileName[500];
    while (op)
    {
        system("cls");
        printf("\n\n");
        printf("         基于SAT的二进制数独游戏求解程序\n");
        printf("-------------------------------------------------\n");
        printf("     1. SAT算例求解       2. 数独\n");
        printf("     0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0-2]: ");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            printf("请输入要读取数据文件的文件名:");
            scanf("%s", fileName);
            break;
        
        default:
            break;
        }
    }
    return 0;
}
