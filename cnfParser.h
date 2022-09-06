/*
 * @Description: 处理cnf文件
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-30 10:17:28
 * @LastEditTime: 2022-09-06 09:15:02
 */

status createClause(ClauseList &clause, FILE *fp)
{
    int literal;
    char buffer[10];
    //初始化
    clause->next = (ClauseList)malloc(sizeof(ClauseNode));
    clause = clause->next;
    clause->nodeNum = 0;
    clause->head = (LiteralList)malloc(sizeof(LiteralNode));
    clause->head->next = (LiteralList)malloc(sizeof(LiteralNode));
    LiteralList literalHead = clause->head;
    literalHead->next = NULL;

    while (2233)
    {
        //读取
        fscanf(fp, "%s", buffer);
        literal = atoi(buffer);
        //赋值
        if (literal == 0)
            break;
        //链表传递
        literalHead->next = (LiteralList)malloc(sizeof(LiteralNode));
        literalHead = literalHead->next;
        literalHead->data = literal;
        clause->nodeNum++;
    }
    literalHead->next = NULL;
    return OK;
}

status fileInput(ClauseList &clause, int &literalNum)
{
    FILE *fp = NULL;
    clause = (ClauseList)malloc(sizeof(ClauseNode));
    ClauseList headp = clause;

    char buffer[500], fileName[100];
    int clauseNum;

    printf("请输入要打开的文件名: ");
    scanf("%s", fileName);
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("打开文件失败!\n");
        return ERROR;
    }

    do
    {
        fscanf(fp, "%s", buffer);
    } while (strcmp(buffer, "cnf"));
    fscanf(fp, "%s", buffer);
    literalNum = atoi(buffer); //获取布尔变元量
    fscanf(fp, "%s", buffer);
    clauseNum = atoi(buffer); //获取子句数量

    for (int i = 0; i < clauseNum; i++) //写入数据到链表
    {
        createClause(clause, fp);
    }
    clause->next = NULL;

    clause = headp;
    fclose(fp);
    return OK;
}

status generateBaseCnfFile(int data[])
{
    FILE *fp = NULL;
    if ((fp = fopen("baseCnf.cnf", "w")) == NULL)
        return ERROR;
    fprintf(fp, "p cnf %d %d\n", BASE_LITERAL_NUM, BASE_CLAUSE_NUM + data[0]);
    //提示数
    for (int i = 1; data[i] != 0; i++)
        fprintf(fp, "%d 0\n", data[i]);
    for (int t = 1; t <= 2; t++)
    {
        //行
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                for (int k = 1; k <= 9; k++)
                    fprintf(fp, "%d%d%d%d ", t, i, k, j);
                fprintf(fp, "0\n");
            }
            for (int n = 1; n <= 9; n++)
                for (int j = 1; j <= 9; j++)
                    for (int k = 1 + j; k <= 9; k++)
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, i, j, n, t, i, k, n);
        }
        //列
        for (int i = 1; i <= 9; i++)
        {
            for (int j = 1; j <= 9; j++)
            {
                for (int k = 1; k <= 9; k++)
                    fprintf(fp, "%d%d%d%d ", t, k, i, j);
                fprintf(fp, "0\n");
            }
            for (int n = 1; n <= 9; n++)
                for (int j = 1; j <= 9; j++)
                    for (int k = 1 + j; k <= 9; k++)
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, j, i, n, t, k, i, n);
        }
        //九宫格
        int a[10] = {0};
        for (int n = 0; n < 9; n++)
        {
            memset(a, 0, 10);
            for (int i = 1; i <= 9; i++)
            {
                int count = 1;
                for (int j = 1; j <= 3; j++)
                    for (int k = 1; k <= 3; k++)
                    {
                        fprintf(fp, "%d%d%d%d ", t, j + ((int)(n / 3) * 3), k + ((n % 3) * 3), i);
                        a[count++] = (j + ((int)(n / 3) * 3)) * 10 + k + ((n % 3) * 3);
                    }
                fprintf(fp, "0\n");
                for (int j = 2 + n; j <= 9; j++)
                    fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, a[n + 1] / 10, a[n + 1] % 10, i, t, a[j] / 10, a[j] % 10, i);
            }
        }
        for (int i = 1; i <= 9; i++)
            for (int j = 1; j <= 9; j++)
                for (int n = 1; n <= 9; n++)
                    for (int g = 1 + n; g <= 9; g++)
                        fprintf(fp, "-%d%d%d%d -%d%d%d%d 0\n", t, i, j, n, t, i, j, g);
    }
    //重叠部分
    for (int j = 1; j <= 3; j++)
        for (int k = 1; k <= 3; k++)
            for (int t = 1; t <= 9; t++)
            {
                fprintf(fp, "-1%d%d%d 2%d%d%d 0\n", j + 6, k + 6, t, j, k, t);
                fprintf(fp, "1%d%d%d -2%d%d%d 0\n", j + 6, k + 6, t, j, k, t);
            }
    fclose(fp);
    return OK;
}

status loadCnfFile(ClauseList &clause, int &literalNum, char fileName[])
{
    FILE *fp = NULL;
    clause = (ClauseList)malloc(sizeof(ClauseNode));
    ClauseList headp = clause;

    char buffer[500];
    int clauseNum;

    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("打开文件失败!\n");
        return ERROR;
    }

    do
    {
        fscanf(fp, "%s", buffer);
    } while (strcmp(buffer, "cnf"));
    fscanf(fp, "%s", buffer);
    literalNum = atoi(buffer); //获取布尔变元量
    fscanf(fp, "%s", buffer);
    clauseNum = atoi(buffer); //获取子句数量

    for (int i = 0; i < clauseNum; i++) //写入数据到链表
    {
        createClause(clause, fp);
    }
    clause->next = NULL;

    clause = headp;
    fclose(fp);
    return OK;
}