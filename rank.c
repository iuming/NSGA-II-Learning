/* Rank assignment routine */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to assign rank and crowding distance to a population of size pop_size*/
void assign_rank_and_crowding_distance (population *new_pop)
{
    int flag;
    int i;
    int end;
    int front_size;
    int rank=1;
    list *orig;
    list *cur;
    list *temp1, *temp2;
    orig = (list *)malloc(sizeof(list));
    cur = (list *)malloc(sizeof(list));
    front_size = 0;
    orig->index = -1;
    orig->parent = NULL;
    orig->child = NULL;
    cur->index = -1;
    cur->parent = NULL;
    cur->child = NULL;
    temp1 = orig;

    /* 对orig 链表中的个体进行初始化，元素赋值相对的个体序号 */
    for (i=0; i<popsize; i++)
    {
        insert (temp1,i);
        temp1 = temp1->child;
    }

    /* 支配关系分层的主循环函数 */
    do
    {
        /*
            如果orig链表中只有一个个体，则直接对其分层赋值，
            因为该层只有一个个体对其拥挤度直接赋值为无穷，并break主循环
        */
        if (orig->child->child == NULL)
        {
            new_pop->ind[orig->child->index].rank = rank;
            new_pop->ind[orig->child->index].crowd_dist = INF;
            break;
        }
        /*
            orig 中的元素为待分层的元素， 此时 cur 链表为空。

            取出 orig 链表中的头一个个体插入到 cur 链表中，该操作相当于对以下内循环的初始化
            此时，cur链表中只有一个元素
        */
        temp1 = orig->child;
        insert (cur, temp1->index);
        front_size = 1;
        temp2 = cur->child;
        temp1 = del (temp1);
        temp1 = temp1->child;
        do
        {
            /*temp2 指向cur链表的第一个节点*/
            temp2 = cur->child;
            do
            {
                /*结束标志位 归0 */
                end = 0;
                /*判断 orig 和 cur 链表中 temp1, temp2 指针指向的节点元素所对应的个体支配关系 */
                flag = check_dominance (&(new_pop->ind[temp1->index]), &(new_pop->ind[temp2->index]));
                /*若 a支配b ,在orig中插入a,在cur中删除b */
                if (flag == 1)
                {
                    insert (orig, temp2->index);
                    temp2 = del (temp2);
                    front_size--;
                    temp2 = temp2->child;
                }
                /*个体a b互不支配， cur链表指针下移一位*/
                if (flag == 0)
                {
                    temp2 = temp2->child;
                }
                /*个体b 支配 个体a , 结束该次循环*/
                if (flag == -1)
                {
                    end = 1;
                }
            }

            /* 
                个体b 被 个体a 支配即 flag==-1,  将该层循环结束位 end置1，结束该层循环。
                cur 链表中 所有个体均已遍历，没有b个体，结束循环。 
            */
            while (end != 1 && temp2 != NULL);

            /* 
                个体a 支配 个体b   或者  互不支配 
                将个体a 插入到 cur链表最前端，同时移除orig链表中的a个体
            */
            if (flag == 0 || flag == 1)
            {
                insert (cur, temp1->index);
                front_size++;
                temp1 = del (temp1);
            }
            /*orig链表中所指向个体的指针后移一位*/
            temp1 = temp1->child;
        }
        /*temp1指针指向NULL意味着orig链表中所有元素对应的个体均被 cur链表中对应的个体  支配*/
        while (temp1 != NULL);

        /*
            temp2重新指向 cur 列表中第一个元素，cur列表中的元素为当前已分层的元素
        */
        temp2 = cur->child;

        do
        {
            new_pop->ind[temp2->index].rank = rank;
            temp2 = temp2->child;
        }
        while (temp2 != NULL);
        /* 对当前层的个体进行拥挤度判断 */
        assign_crowding_distance_list (new_pop, cur->child, front_size);
        /* 对 cur 链表中的个体释放内存空间 */
        temp2 = cur->child;
        do
        {
            temp2 = del (temp2);
            temp2 = temp2->child;
        }
        while (cur->child !=NULL);
        /* 分层的排序值 加1 */
        rank+=1;
    }
    /* 循环判断，直到orig链表中出头节点外为空，即所有个体全部分层 */
    while (orig->child != NULL);
    /* 将链表orig cur的头结点内存空间释放掉 */
    free (orig);
    free (cur);
    return;
}

/*
    该非支配分层基本思想是设置两个双向链表（orig   cur），orig 链表里面存放所有待分层排序的个体索引，cur链表中的元素为分层结束后该层的个体索引。
    每次在orig 中取出的元素对应的个体为 a,  cur 中取出的元素对应的个体为 b 。

    若 b支配于 a  ,则取 orig 中对应的下一个个体作为 a ,
    若 a  b  互不支配  ,则依次取 cur 中对应的下一个个体作为 b ,  遍历cur 中所有个体（cur 中的个体为待分层的个体，其互不支配），若a 与  cur 中所有个体互不支配则将个体a移除orig链表并插入到cur链表的最前端。
    若a  支配于  b,  则将b 个体移除cur 链表并插入到 orig 链表的最前端，同时取cur 中的下一个个体作为 b 。

    当遍历orig 中的所有元素，此时 cur 中个体便是此时的非支配解。
*/