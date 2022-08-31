/*
 * @Description: 主函数
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-08 20:54:00
 * @LastEditTime: 2022-08-31 09:42:50
 * @endcode: UTF-8
 */
#include "def.h"
#include "DPLL.h"
#include "cnfParser.h"

int main()
{
    int op = 1, literalNum;
    boolean value[1000] = {false};
    ClauseList clause;
    clock_t start, end;

    while (op)
    {
        // system("cls");
        // printf("\n\n");
        // printf("         基于SAT的二进制数独游戏求解程序\n");
        // printf("-------------------------------------------------\n");
        // printf("     1. SAT算例求解       2. 数独\n");
        // printf("     0. Exit\n");
        // printf("-------------------------------------------------\n");
        // printf("    请选择你的操作[0-2]: ");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            fileInput(clause, literalNum);
            printf("DPLL开始运行...\n");
            start = clock();
            DPLL(clause, value);
            end = (clock() - start);
            printf("DPLL运行完毕!\n");
            printf("用时：%ld\n", end);
            for (int i = 1; i <= literalNum; i++)
            {
                if (value[i - 1] == true)
                    printf("%d ", i);
                else
                    printf("%d ", -i);
                if (i % 5 == 0)
                    printf("\n");
            }
            break;
        default:
            break;
        }
    }
    return 0;
}
