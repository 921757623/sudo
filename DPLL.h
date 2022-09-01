/*
 * @Description: 函数库
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-21 19:18:59
 * @LastEditTime: 2022-09-01 21:11:36
 */
void *main_thread(void *arg);
void *count_thread(void *arg);

status addClause(ClauseList &clause, int data)
{
    ClauseList temp = (ClauseList)malloc(sizeof(ClauseNode));
    temp->head = (LiteralList)malloc(sizeof(LiteralNode));
    temp->head->next = (LiteralList)malloc(sizeof(LiteralNode));
    temp->head->next->next = NULL;

    temp->head->next->data = data;
    temp->nodeNum = 1;
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

status recordValue(ClauseList clause, boolean value[])
{
    LiteralList head = clause->head->next;

    if (head->data > 0)
        value[head->data - 1] = true;
    else
        value[abs(head->data) - 1] = false;

    return OK;
}

/**
 * @description: 删除子句中的特定文字
 * @param {ClauseList} &clause
 * @param {int} var
 * @return {*}
 */
status deleteLiteral(ClauseList &clause, int var)
{
    LiteralList literal = clause->head, temp;

    while (literal != NULL && literal->next != NULL)
    {
        pthread_testcancel();
        if (literal->next->data == -var)
        {
            temp = literal->next;
            literal->next = literal->next->next;
            clause->nodeNum--;
            free(temp);
        }
        else if (literal->next->data == var)
            return false;
        literal = literal->next;
    }
    return true;
}

status deleteClause(ClauseList &clause)
{
    ClauseList temp = clause->next;
    LiteralList literal;
    do
    {
        literal = temp->head->next;
        temp->head->next = temp->head->next->next;
        free(literal);
    } while (temp->head->next != NULL); // 删除子句中所有文字
    clause->next = clause->next->next;
    free(temp);
    return OK;
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
        pthread_testcancel();
        value = deleteLiteral(temp, var); // 删除子句里的负变元，如果遇到了正变元则返回false
        if (value == false)               // 遇到了正变元
        {
            deleteClause(pre); // 与正变元相同则删除子句
            temp = pre->next;
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
 * @return {boolean}
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
        clause = clause->next;
    }
    return NULL;
}

/**
 * @description: 判断子句是否为空
 * @param {ClauseList} clause
 * @return {boolean}
 */
boolean isEmptyClause(ClauseList clause)
{
    while (clause != NULL)
    {
        if (clause->nodeNum == 0)
            return true;
        clause = clause->next;
    }
    return false;
}

/**
 * @description: 判断子句的真假情况
 * @param {ClauseList} clause
 * @param {int} value
 * @return {int}
 */
boolean evaluateClause(ClauseList clause, boolean value[])
{
    LiteralList head = clause->head->next;
    while (head)
    {
        if (head->data > 0)
            if (value[head->data - 1] == true)
                return true;
            else
                ;
        else if (value[abs(head->data) - 1] == false)
            return true;
        head = head->next;
    }
    return false;
}

ClauseList copySingleClause(ClauseList clause)
{
    ClauseList temp = (ClauseList)malloc(sizeof(ClauseNode));
    temp->nodeNum = 0;
    temp->head = (LiteralList)malloc(sizeof(LiteralNode));
    temp->next = NULL;
    temp->head->next = NULL;
    LiteralList literal = clause->head->next, headp = temp->head;
    while (literal)
    {
        headp->next = (LiteralList)malloc(sizeof(LiteralNode));
        headp->next->data = literal->data;
        literal = literal->next;
        headp = headp->next;
        headp->next = NULL;
        temp->nodeNum++;
    }
    return temp;
}

ClauseList copyClauses(ClauseList origin)
{
    ClauseList clause = (ClauseList)malloc(sizeof(ClauseNode));
    clause->next = NULL;
    ClauseList headp = clause;
    while (origin)
    {
        clause->next = copySingleClause(origin);
        clause = clause->next;
        origin = origin->next;
    }
    return headp;
}

/**
 * @description: DPLL处理器
 * @param {ClauseList} &clause
 * @param {int} value
 * @return {*}
 */
boolean DPLL(ClauseList &clause, boolean value[])
{
    ClauseList headp = clause->next;
    pthread_testcancel();
    //单子句规则
    ClauseList unitClause = getUnitClause(headp);
    while (unitClause != NULL)
    {
        recordValue(unitClause, value);                  //记录该单子句变量的真值
        deleteVar(clause, unitClause->head->next->data); //在所有的字句中删除该相同变量

        if (clause->next == NULL) // S为空
            return true;
        else if (isEmptyClause(clause->next)) // clause中存在空子句
            return false;

        headp = clause->next;
        unitClause = getUnitClause(headp);
    }
    //分裂
    int var = headp->head->next->data; // 选变元
    ClauseList copy = copyClauses(headp);
    addClause(copy, var);
    if (DPLL(copy, value))
        return true;
    addClause(clause, -var);
    return DPLL(clause, value);
}

status check(ClauseList clause, boolean value[])
{
    int count = 1;
    clause = clause->next;
    while (clause)
    {
        if (!evaluateClause(clause, value))
        {
            printf("%d false!\n", count);
        }
        count++;
        clause = clause->next;
    }
    return OK;
}

void *main_thread(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    param params = (param)arg;
    clock_t start;
    float end;
    start = clock();
    DPLL(params->clause, params->value);
    end = ((float)(clock() - start)) / CLOCKS_PER_SEC;
    printf("DPLL运行完毕!\n");
    printf("用时：%g 秒\n", end);
    if (pthread_cancel(1) != 0)
    {
        printf("中止计时线程失败！\n");
    }
    printf("结果为：");
    for (int i = 1; i <= params->literalNum; i++)
    {
        if (params->value[i - 1] == true)
            printf("%d ", i);
        else
            printf("%d ", -i);
        if (i % 5 == 0)
            printf("\n");
    }

    printf("开始校验结果...\n");
    check(params->clause, params->value);
    printf("校验成功！\n");
}

void *count_thread(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    clock_t start, end;
    start = clock();
    while (end < 10)
    {
        end = (clock() - start) / CLOCKS_PER_SEC;
        pthread_testcancel();
    }
    if (pthread_cancel(2) != 0)
        printf("终止DPLL线程失败！\n");
    else
        printf("程序运行超时！\n");
}