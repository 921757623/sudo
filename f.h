/*
 * @Description: 函数库
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-21 19:18:59
 * @LastEditTime: 2022-08-30 09:07:27
 */
status fileInput(ClauseList &head, int &literalNum)
{
    FILE *fp = NULL;
    head = (ClauseList)malloc(sizeof(ClauseNode));
    head->next = (ClauseList)malloc(sizeof(ClauseNode));
    ClauseList headp = head->next;
    LiteralList literalHead = NULL;

    char stringGot[500], fileName[100];
    int clauseNum, literal;

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
    } while (strcmp(stringGot, "cnf"));
    fscanf(fp, "%s", stringGot);
    literalNum = atoi(stringGot); //获取布尔变元量
    fscanf(fp, "%s", stringGot);
    clauseNum = atoi(stringGot); //获取子句数量

    for (int i = 0; i < clauseNum; i++) //写入数据到链表
    {
        //初始化
        headp->nodeNum = 0;
        headp->head = (LiteralList)malloc(sizeof(LiteralNode));
        headp->head->next = (LiteralList)malloc(sizeof(LiteralNode));
        literalHead = headp->head->next;

        do
        {
            //读取
            fscanf(fp, "%s", stringGot);
            literal = atoi(stringGot);
            //赋值
            literalHead->data = literal;
            headp->nodeNum++;
            //链表传递
            literalHead->next = (LiteralList)malloc(sizeof(LiteralNode));
            literalHead = literalHead->next;
        } while (literal != 0);
        headp->next = (ClauseList)malloc(sizeof(ClauseNode));
        headp = headp->next;
    }

    fclose(fp);
    return OK;
}

status addClause(ClauseList &clause, int data)
{
    if (clause == NULL)
        return false;

    ClauseList temp = (ClauseList)malloc(sizeof(ClauseNode));
    temp->head = (LiteralList)malloc(sizeof(LiteralNode));
    temp->head->next = (LiteralList)malloc(sizeof(LiteralNode));
    LiteralList head = clause->head->next;

    head->data = data;
    clause->nodeNum = 1;
    temp->next = clause->next;
    clause->next = temp;

    return OK;
}

status removeClause(ClauseList &clause)
{
    if (clause == NULL)
        return false;
    ClauseList clausep = clause->next;
    LiteralList temp;
    while (clausep->head != NULL)
    {
        temp = clausep->head->next;
        free(clausep->head);
        clausep->head = clausep->head->next;
    }
    free(clausep);
    clause->next = clause->next->next;
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

status deleteLiteral(ClauseList &clause, int var)
{
    LiteralList literal = clause->head, temp;

    while (literal != NULL)
    {
        if (literal->next && literal->next->data == -var)
        {
            temp = literal->next;
            literal->next = temp->next;
            clause->nodeNum--;
            free(temp);
        }
        else if (literal->next && literal->next->data == var)
            return false;
        literal = literal->next;
    }
    return true;
}

ClauseList deleteClause(ClauseList &clause)
{
    ClauseList temp = clause->next;
    LiteralList literal;
    while (temp->head->next != NULL) // 先删除子句中所有文字
    {
        literal = clause->head->next;
        clause->head->next = clause->head->next->next;
        free(literal);
    }
    clause->next = clause->next->next;
    free(temp);
}

/**
 * @description:
 * @param {ClauseList} clause
 * @param {int} var
 * @return {*}
 */
status deleteVar(ClauseList &clause, int var)
{
    ClauseList temp = clause->next, pre = clause;
    int value;
    while (temp) // 循环读取每一个子句
    {
        value = deleteLiteral(temp, var); // 删除子句里的负变元，如果遇到了正变元则返回false
        if (value == false)               // 遇到了正变元
        {
            deleteClause(pre); // 与正变元相同则删除子句
            continue;
        }
        if (temp == NULL)
            break;
        pre = temp; // 下一个子句
        temp = temp->next;
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
 * @description: 获取单子句，若找不到则返回NULL
 * @param {ClauseList} clause
 * @return {ClauseList}
 */
ClauseList getUnitClause(ClauseList clause)
{
    while (clause != NULL)
    {
        if (isUnitClause(clause))
            return clause;
        else
            clause = clause->next;
    }
    return NULL;
}

boolean isEmptyClause(ClauseList clause)
{
    if (clause->nodeNum == 0)
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

/**
 * @description: DPLL处理器
 * @param {ClauseList} &clause
 * @param {int} value
 * @return {*}
 */
boolean DPLL(ClauseList &clause, int value[])
{
    ClauseList headp = clause;
    ClauseList unitClause = getUnitClause(headp);
    //单子句规则
    while (unitClause != NULL)
    {
        recordValue(unitClause, value);                  //记录该单子句变量的真值
        deleteVar(clause, unitClause->head->next->data); //在所有的字句中删除该相同变量

        if (clause->next == NULL) // S为空
            return true;
        else if (isEmptyClause(clause)) // clause中存在空子句
            return false;

        headp = clause;
        unitClause = getUnitClause(headp);
    }
    //分裂
    int var = clause->next->head->next->data; // 选变元
    addClause(clause, var);
    if (DPLL(clause, value))
        return true;
    deleteClause(clause);
    addClause(clause, -var);
    return DPLL(clause, value); // 变元的真值选错了
}
