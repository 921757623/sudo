/*
 * @Description: 主函数
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-08 20:54:00
 * @LastEditTime: 2022-09-06 11:21:26
 * @endcode: UTF-8
 */
#include "def.h"
#include "DPLL.h"
#include "cnfParser.h"
#include "sudo.h"

int main()
{
    void *response;
    int op = 1, literalNum, value = 1;
    int *data;
    char fileName[200] = "baseCnf.cnf";
    char *res;
    ClauseList clause;
    // clock_t start;
    // float end;
    pthread_t main_work, count;
    parameter params;

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
            printf("开始文件读取...\n");
            value = fileInput(clause, literalNum);
            if (value == OK)
                printf("文件读取成功！\n");
            else if (value == ERROR)
                printf("文件读取失败！\n");

            params.clause = clause;
            params.literalNum = literalNum;
            params.thread = &count;
            // DPLL求解线程，规定时间内求解完毕会结束全部线程
            pthread_create(&main_work, NULL, main_thread_1, (void *)&params);
            //计时线程，超时会结束全部线程
            pthread_create(&count, NULL, count_thread, (void *)&main_work);
            pthread_join(count, &response);
            if (response != (void *)0xdeadbeef)
            {
                value = atoi((char *)response);
                if (value == OK)
                {
                    pthread_cancel(main_work);
                    printf("已终止DPLL线程！\n");
                }
            }
            pthread_join(main_work, &response);
            getchar();
            getchar();
            break;
        case 2:
            data = datasource[0];
            generateBaseCnfFile(data);
            value = loadCnfFile(clause, literalNum, fileName);
            if (value == OK)
                printf("文件读取成功！\n");
            else if (value == ERROR)
                printf("文件读取失败！\n");
            params.clause = clause;
            params.literalNum = literalNum;
            params.thread = &count;
            // DPLL求解线程，规定时间内求解完毕会结束全部线程
            pthread_create(&main_work, NULL, main_thread_2, (void *)&params);
            //计时线程，超时会结束全部线程
            pthread_create(&count, NULL, count_thread, (void *)&main_work);
            pthread_join(count, &response);
            if (response != (void *)0xdeadbeef)
            {
                value = atoi((char *)response);
                if (value == OK)
                {
                    pthread_cancel(main_work);
                    printf("已终止DPLL线程！\n");
                }
            }
            pthread_join(main_work, &response);
            getchar();
            getchar();
            break;
        case 0:
            getchar();
            getchar();
        default:
            getchar();
            getchar();
            break;
        }
    }
    return 0;
}
