/*
 * @Description: 定义库，包含库函数、define以及数据结构
 * @version: 1.0.0
 * @Author: yrp
 * @Date: 2022-08-21 19:18:19
 * @LastEditTime: 2022-08-29 15:14:26
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef int status;
typedef int boolean;

#define ERROR 0
#define OK 1
#define true 1
#define false 0
#define LITERAL_SIZE 10

typedef struct LiteralNode
{
    int data;
    struct LiteralNode *next;
} LiteralNode, *LiteralList;

typedef struct ClauseNode
{
    int nodeNum;
    struct LiteralNode *head;
    struct ClauseNode *next;
} ClauseNode, *ClauseList;