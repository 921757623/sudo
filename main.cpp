/*
 * @Description: 主函数
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-08 20:54:00
 * @LastEditTime: 2022-09-01 21:01:47
 * @endcode: UTF-8
 */
#include "def.h"
#include "DPLL.h"
#include "cnfParser.h"

int main()
{
    int op = 1, literalNum;
    ClauseList clause;
    // clock_t start;
    // float end;
    pthread_t main_work, count;
    parameter params;

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
            params.clause = clause;
            params.literalNum = literalNum;
            pthread_create(&count, NULL, count_thread, NULL);
            pthread_create(&main_work, NULL, main_thread, (void *)&params);
            pthread_join(count, NULL);
            pthread_join(main_work, NULL);
            // start = clock();
            // DPLL(clause, value);
            // end = ((float)(clock() - start)) / CLOCKS_PER_SEC;
            // printf("DPLL运行完毕!\n");
            // printf("用时：%g 秒\n", end);

            break;
        default:
            break;
        }
    }
    return 0;
}
