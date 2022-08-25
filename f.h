/*
 * @Description: 函数库
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-21 19:18:59
 * @LastEditTime: 2022-08-25 21:17:00
 */
status fileInput(ClauseList &head)
{
    FILE *fp = NULL;
    head = (ClauseList)malloc(sizeof(ClauseNode));
    ClauseList headp = head;

    char stringGot[500], fileName[100];
    int literalNum, clauseNum, literal;

    printf("请输入要打开的文件名: ");
    scanf("%s", fileName);
    if ((fp = fopen(fileName, "r")) == NULL)
    {
        printf("打开文件失败!\n");
        exit(1);
    }

    do
    {
        fscanf(fp, "%s", stringGot);
    } while (!strcmp(stringGot, "p"));
    fscanf(fp, "%s", stringGot);
    literalNum = atoi(stringGot); //获取布尔变元量
    fscanf(fp, "%s", stringGot);
    clauseNum = atoi(stringGot); //获取子句数量

    for (int i = 0; i < clauseNum; i++) //写入数据到链表
    {
        int count = 0;
        headp->next = (ClauseList)malloc(sizeof(ClauseNode));
        headp->next->prior = headp;
        headp = headp->next;
        headp->nodeNum = 0;
        headp->head = (LiteralList)malloc(sizeof(LiteralNode));
        headp->head->next = (LiteralList)malloc(sizeof(LiteralNode));
        LiteralList literalHead = headp->head->next;
        do
        {
            fscanf(fp, "%s", stringGot);
            literal = atoi(stringGot);
            literalHead->data = literal;
            headp->nodeNum++;
            literalHead->next = (LiteralList)malloc(sizeof(LiteralNode));
            literalHead = literalHead->next;
        } while (literal != 0);
    }

    fclose(fp);
    return OK;
}

status addClause(ClauseList &clause, int data[])
{
    if (clause == NULL)
        return false;
    clause->next = (ClauseList)malloc(sizeof(ClauseNode));
    clause = clause->next;
    clause->nodeNum = 0;
    clause->head = (LiteralList)malloc(sizeof(LiteralNode));
    clause->head->next = (LiteralList)malloc(sizeof(LiteralNode));
    LiteralList head = clause->head->next;
    for (int i = 0; data[i] != 0; i++)
    {
        head->data = data[i];
        head->next = (LiteralList)malloc(sizeof(LiteralNode));
        head = head->next;
        clause->nodeNum++;
    }
    return OK;
}

status removeClause(ClauseList &clause)
{
    if (clause == NULL)
        return false;
    clause->prior->next = clause->next;
    clause->next->prior = clause->prior;
    LiteralList temp;
    while (clause->head != NULL)
    {
        temp = clause->head->next;
        free(clause->head);
        clause->head = clause->head->next;
    }
    free(clause);
    return OK;
}

status recordValue(ClauseList clause, int value[])
{
    if (clause == NULL)
        return false;
    LiteralList head = clause->head->next;
    while (head != NULL)
    {
        if (head->data > 0)
            value[head->data - 1] = true;
        else
            value[abs(head->data) - 1] = false;
        head = head->next;
    }
    return OK;
}

/**
 * @description: 判断是否为单子句
 * @param {ClauseList} clause
 * @return {int}
 */
boolean isUnitClause(ClauseList clause)
{
    if (clause->nodeNum == 1)
        return true;
    else
        return false;
}

/**
 * @description: 判断子句的真假情况
 * @param {ClauseList} clause
 * @param {int} value
 * @return {int}
 */
boolean evaluateClause(ClauseList clause, int value[])
{
    LiteralList head = clause->head->next;
    for (int i = 0; head->data != 0; i++)
    {
        if (value[head->data] == true)
            return true;
        head = head->next;
    }
    return false;
}

status DPLL()
{
}
