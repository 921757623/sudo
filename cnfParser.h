/*
 * @Description: 处理cnf文件
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-30 10:17:28
 * @LastEditTime: 2022-08-30 10:36:58
 */

status createClause(ClauseList &clause, FILE *fp)
{
    int literal;
    char buffer[10];
    //初始化
    clause->next = (ClauseList)malloc(sizeof(ClauseNode));
    clause = clause->next;
    clause->next = NULL;
    clause->nodeNum = 0;
    clause->head = (LiteralList)malloc(sizeof(LiteralNode));
    clause->head->next = (LiteralList)malloc(sizeof(LiteralNode));
    LiteralList literalHead = clause->head->next;

    do
    {
        //读取
        fscanf(fp, "%s", buffer);
        literal = atoi(buffer);
        //赋值
        literalHead->data = literal;
        clause->nodeNum++;
        //链表传递
        literalHead->next = (LiteralList)malloc(sizeof(LiteralNode));
        literalHead = literalHead->next;
        literalHead->next = NULL;
    } while (literal != 0);
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
        exit(1);
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

    clause = headp;
    fclose(fp);
    return OK;
}