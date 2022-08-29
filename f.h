/*
 * @Description: 函数库
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-21 19:18:59
 * @LastEditTime: 2022-08-29 15:14:24
 */
status fileInput(ClauseList &head)
{
    FILE *fp = NULL;
    head = (ClauseList)malloc(sizeof(ClauseNode));
    head->next = (ClauseList)malloc(sizeof(ClauseNode));
    ClauseList headp = head->next;
    LiteralList literalHead = NULL;

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

status DeleteLiteral(ClauseList &clause, int var)
{
    LiteralList literal = clause->head->next, temp;

    // 处理第一个文字
    if (literal->data == -var) // 负变元
    {
        temp = clause->head->next->next;
        free(literal);
        clause->head->next = temp;
        return OK;
    }
    else if (literal->data == var)
    {
        clause == NULL;
        return OK;
    } // 正变元

    // 循环处理接下来的文字
    while (literal != NULL)
    {
        if (literal->next && literal->next->data == -var)
        {
            temp = literal->next;
            literal->next = literal->next->next;
            free(temp);
        }
        else if (literal->next && literal->next->data == var)
        {
            clause == NULL;
            return OK;
        }
        literal = literal->next;
    }
    return OK;
}

/**
 * @description:
 * @param {ClauseList} clause
 * @param {int} var
 * @return {*}
 */
status removeVar(ClauseList clause, int var)
{
    ClauseList temp = clause->next, pre = clause;
    while (temp) // 循环读取每一个子句
    {
        DeleteLiteral(temp, var); // 删除子句里所有的负变元，如果遇到了正变元则返回NULL
        if (temp == NULL)         // 遇到了正变元
        {
            DeleteClause(pre->next); // 与正变元相同则删除子句
            pre->next = temp;
            continue;
        }
        if (temp == NULL)
            break;
        pre = temp; // 下一个子句
        temp = temp->next;
    }
    return OK;
}

ClauseList DeleteClause(ClauseList &clause)
{
    ClauseList temp = clause;
    LiteralList literal;
    while (clause->head->next) // 先删除子句中所有文字
    {
        literal = clause->head->next;
        clause->head->next = clause->head->next->next;
        free(literal);
    }
    clause = clause->next;
    free(temp); // 再删除空子句
    return clause;
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

/**
 * @description: DPLL处理器
 * @param {ClauseList} &clause
 * @param {int} value
 * @return {*}
 */
status DPLL(ClauseList &clause, int value[])
{
    ClauseList headp = clause;
    ClauseList unitClause = getUnitClause(headp);

    if (unitClause != NULL)
    {
        recordValue(unitClause, value);
        int var = unitClause->head->next->data;
        removeVar(clause, var);

        if (clause->next == NULL)
            return true; // S为空
        else if (IsEmptyClause(clause))
            return false; // S中存在空子句

        headp = clause;
        unitClause = getUnitClause(headp);
    }

    int var = clause->next->head->next->data;
    ; // 选变元
    if (DPLL(AddClause(CopyS(clause), var), value))
        return true;                             // 变元的真值选对了,此处传入S的拷贝
    return DPLL(AddClause(clause, -var), value); // 变元的真值选错了
}
