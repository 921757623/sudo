/*
 * @Description: 函数库
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-21 19:18:59
 * @LastEditTime: 2022-09-06 11:15:43
 */

void *main_thread(void *arg);
void *count_thread(void *arg);
status addClause(ClauseList &clause, int data);
status recordValue(ClauseList clause, boolean value[]);
status deleteLiteral(ClauseList &clause, int var);
status deleteClause(ClauseList &clause);
status deleteVar(ClauseList &clause, int var);
status check(ClauseList clause, boolean value[]);
boolean isUnitClause(ClauseList clause);
boolean isEmptyClause(ClauseList clause);
boolean evaluateClause(ClauseList clause, boolean value[]);
ClauseList getUnitClause(ClauseList clause);
ClauseList copySingleClause(ClauseList clause);
ClauseList copyClauses(ClauseList origin);

status drawFinalPanel(int data[])
{
    printf("-------****** Answer ******-------\n");
    printf("----------------------------------\n");
    // 打印上半部分
    for (int y = 1; y <= 6; y++)
    {
        for (int x = 1 + (y - 1) * 9; x <= y * 9; x++)
        {
            printf("%d ", data[x] % 10);
            if (x % 3 == 0)
                printf(" ");
        }
        printf("\n");
        if (y % 3 == 0)
            printf("\n");
    }
    //打印中间重叠部分
    for (int y = 7; y <= 9; y++)
    {
        for (int x = 55 + (y - 7) * 9; x <= y * 9 - 3; x++)
        {
            printf("%d ", data[x] % 10);
            if (x % 3 == 0)
                printf(" ");
        }
        for (int x = 82 + (y - 7) * 9; x <= 90 + (y - 7) * 9; x++)
        {
            printf("%d ", data[x] % 10);
            if (x % 3 == 0)
                printf(" ");
        }
        printf("\n");
        if (y % 3 == 0)
            printf("\n");
    }
    //打印下半部分
    for (int y = 4; y <= 9; y++)
    {
        printf("              ");
        for (int x = 109 + (y - 4) * 9; x <= (y + 9) * 9; x++)
        {
            printf("%d ", data[x] % 10);
            if (x % 3 == 0)
                printf(" ");
        }
        printf("\n");
        if (y % 3 == 0)
            printf("\n");
    }
    printf("----------------------------------\n");
    printf("--------****** End ******---------\n");
    return OK;
}

/**
 * @brief 在原子句组中增添一个单子句
 * @param {ClauseList} &clause
 * @param {int} data
 * @return {status}
 */
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

/**
 * @brief 记录单子句中变元的真值
 * @param {ClauseList} clause
 * @param {boolean} value
 * @return {status}
 */
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
 * @brief 删除子句中的特定文字
 * @param {ClauseList} &clause
 * @param {int} var
 * @return {*}
 */
status deleteLiteral(ClauseList &clause, int var)
{
    LiteralList literal = clause->head, temp;

    while (literal != NULL && literal->next != NULL)
    {
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

/**
 * @brief 删除与正变元相同的子句
 * @param {ClauseList} &clause
 * @return {status}
 */
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
 * @brief 在子句组中删除单子句的变元
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
 * @brief 判断是否为单子句
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
 * @brief 获取单子句，若找不到则返回NULL
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
 * @brief 判断子句是否为空
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
 * @brief 判断子句的真假情况
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

/**
 * @brief 复制单个子句
 * @param {ClauseList} clause
 * @return {*}
 */
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

/**
 * @brief 复制子句组
 * @param {ClauseList} origin
 * @return {*}
 */
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
 * @brief DPLL处理器
 * @param {ClauseList} &clause
 * @param {int} value
 * @return {*}
 */
boolean DPLL_1(ClauseList clause, boolean value[])
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
    if (DPLL_1(copy, value))
        return true;
    addClause(clause, -var);
    return DPLL_1(clause, value);
}

boolean DPLL_2(ClauseList clause, boolean value[])
{
    ClauseList stack[100000];
    node sign[100000] = {0, 0};
    int top = -1;
    stack[++top] = clause;
    int val;
    while (top != -1)
    {
    sign:
        pthread_testcancel();
        ClauseList c = stack[top];
        ClauseList headp = c->next;
        ClauseList unitClause = getUnitClause(headp);
        while (unitClause != NULL)
        {
            recordValue(unitClause, value);             //记录该单子句变量的真值
            deleteVar(c, unitClause->head->next->data); //在所有的字句中删除该相同变量

            if (c->next == NULL) // S为空
                return true;
            else if (isEmptyClause(c->next)) // clause中存在空子句
            {
                // while (sign[top] != 0 && top != -1)
                // {
                //     val = sign[top];
                //     sign[top] = 0;
                //     --top;
                // }
                // if (top != -1)
                // {
                //     addClause(stack[top], -val);
                //     sign[top] = val;
                //     goto sign;
                // }
                // return false;

                if (sign[top - 1].isAssign == false)
                {
                    addClause(stack[top - 1], -sign[top].value);
                    sign[--top].isAssign = true;
                    sign[top].isAssign = false;
                    goto sign;
                }
                else
                {
                    --top;
                    while (sign[top].isAssign != false && top != -1)
                    {
                        --top;
                    }
                    if (top != -1)
                    {
                        addClause(stack[top], -sign[top + 1].value);
                        goto sign;
                    }
                }
                return false;
            }

            headp = c->next;
            unitClause = getUnitClause(headp);
        }
        //分裂
        val = headp->head->next->data;
        ClauseList copy = copyClauses(headp);
        addClause(copy, val);
        stack[++top] = copy;
        sign[top].value = val;
        sign[top].isAssign = false;
    }
    return false;
}

// boolean DPLL_3(ClauseList clause, boolean value[])
// {
//     int level = 0;
//     IGraph IG;
//     IG.arcnum = 0;
//     IG.vexnum = 0;

//     while (true)
//     {
//     }
// }

/**
 * @brief 校验运算结果的正确性，是否满足全部子句
 * @param {ClauseList} clause
 * @param {boolean} value
 * @return {*}
 */
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

void *main_thread_1(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    param params = (param)arg;
    clock_t start;
    float end;
    char string[5];
    int choice;
    printf("DPLL递归请输入1,，非递归请输入2.\n");
    scanf("%d", &choice);
    printf("DPLL开始运行...\n限时%d秒\n", MAX_EXECUTE_TIME);
    start = clock();
    if (choice == 1)
        DPLL_1(params->clause, params->value);
    else if (choice == 2)
        DPLL_2(params->clause, params->value);
    end = ((float)(clock() - start)) / CLOCKS_PER_SEC;
    printf("DPLL运行完毕!\n");
    printf("用时：%g 秒\n", end);

    int val = pthread_cancel(*params->thread);
    if (val != 0)
    {
        printf("中止计时线程失败！\n");
        printf("错误码为：%d\n", val);
    }

    printf("开始校验结果...\n");
    check(params->clause, params->value);
    printf("校验成功！\n");

    printf("是否输出结果？[Yes/No]\n");
    scanf("%s", string);
    if (strcmp(string, "Yes") == 0 || strcmp(string, "yes") == 0 || strcmp(string, "y") == 0)
    {
        printf("结果为：\n");
        for (int i = 1; i <= params->literalNum; i++)
        {
            if (params->value[i - 1] == true)
                printf("%4d ", i);
            else
                printf("%4d ", -i);
            if (i % 8 == 0)
                printf("\n");
        }
    }
    return (void *)"2";
}

void *main_thread_2(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    param params = (param)arg;
    clock_t start;
    float end;
    char string[5];
    int choice;
    int ans[165];
    for (int i = 1; i <= params->literalNum; i++)
    {
        params->value[number[i]] = false;
    }
    printf("DPLL递归请输入1,，非递归请输入2.\n");
    scanf("%d", &choice);
    printf("DPLL开始运行...\n限时%d秒\n", MAX_EXECUTE_TIME);
    start = clock();
    if (choice == 1)
        DPLL_1(params->clause, params->value);
    else if (choice == 2)
        DPLL_2(params->clause, params->value);
    end = ((float)(clock() - start)) / CLOCKS_PER_SEC;
    printf("DPLL运行完毕!\n");
    printf("用时：%g 秒\n", end);

    int val = pthread_cancel(*params->thread);
    if (val != 0)
    {
        printf("中止计时线程失败！\n");
        printf("错误码为：%d\n", val);
    }

    printf("开始校验结果...\n");
    check(params->clause, params->value);
    printf("校验成功！\n");

    printf("是否输出结果？[Yes/No]\n");
    scanf("%s", string);
    if (strcmp(string, "Yes") == 0 || strcmp(string, "yes") == 0 || strcmp(string, "y") == 0)
    {
        printf("结果为：\n");
        int count = 1, j = 1;
        for (int i = 1; i <= params->literalNum; i++)
        {
            if (params->value[number[i] - 1] == true)
            {
                printf("%4d ", number[i]);
                ans[j++] = number[i];
                count++;
            }

            if (count % 10 == 0)
            {
                printf("\n");
                count = 1;
            }
        }
        drawFinalPanel(ans);
    }
    return (void *)"2";
}

void *count_thread(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    pthread_t thread = (pthread_t)arg;
    clock_t start, end;

    start = clock();
    while (end < MAX_EXECUTE_TIME)
    {
        pthread_testcancel();
        end = (clock() - start) / CLOCKS_PER_SEC;
    }

    // int val = pthread_cancel(thread);
    // if (val != 0)
    // {
    //     printf("终止DPLL线程失败！\n");
    //     printf("错误码为：%d", val);
    // }
    // else
    //     printf("程序运行超时！已终止运行！\n");
    return (void *)"1";
}