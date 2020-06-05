/* Nond-domination based selection routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Routine to perform non-dominated sorting */
void fill_nondominated_sort (population *mixed_pop, population *new_pop)
{
    int flag;
    int i, j;
    int end;
    int front_size;
    int archieve_size;
    int rank=1;
    list *pool;
    list *elite;
    list *temp1, *temp2;
    pool = (list *)malloc(sizeof(list));
    elite = (list *)malloc(sizeof(list));
    front_size = 0;
    archieve_size=0;
    pool->index = -1;
    pool->parent = NULL;
    pool->child = NULL;
    elite->index = -1;
    elite->parent = NULL;
    elite->child = NULL;
    temp1 = pool;
    for (i=0; i<2*popsize; i++)
    {
        insert (temp1,i);
        temp1 = temp1->child;
    }
    i=0;
    do
    {
        temp1 = pool->child;
        insert (elite, temp1->index);
        front_size = 1;
        temp2 = elite->child;
        temp1 = del (temp1);
        temp1 = temp1->child;
        do
        {
            temp2 = elite->child;
            if (temp1==NULL)
            {
                break;
            }
            do
            {
                end = 0;
                flag = check_dominance (&(mixed_pop->ind[temp1->index]), &(mixed_pop->ind[temp2->index]));
                if (flag == 1)
                {
                    insert (pool, temp2->index);
                    temp2 = del (temp2);
                    front_size--;
                    temp2 = temp2->child;
                }
                if (flag == 0)
                {
                    temp2 = temp2->child;
                }
                if (flag == -1)
                {
                    end = 1;
                }
            }
            while (end!=1 && temp2!=NULL);
            if (flag == 0 || flag == 1)
            {
                insert (elite, temp1->index);
                front_size++;
                temp1 = del (temp1);
            }
            temp1 = temp1->child;
        }
        /*以上和 rank.c 中代码基本一致，其功能就是选出当前种群的非支配解*/

        while (temp1 != NULL);
        temp2 = elite->child;
        j=i;
        if ( (archieve_size+front_size) <= popsize)
        {
            /*如果该层个体加入到新种群中后个体总数不超过设定的种群个体数则直接加入*/
            do
            {
                copy_ind (&mixed_pop->ind[temp2->index], &new_pop->ind[i]);
                new_pop->ind[i].rank = rank;
                archieve_size+=1;
                temp2 = temp2->child;
                i+=1;
            }
            while (temp2 != NULL);
            assign_crowding_distance_indices(new_pop, j, i - 1); /* 调用  assign_crowding_distance_indices ， 计算加入个体的拥挤距离 */
            rank+=1;
        }
        else /*如果超出总体数量则对该层个体进行选择，并对选择出的个体计算拥挤距离*/
        {
            crowding_fill (mixed_pop, new_pop, i, front_size, elite);
            archieve_size = popsize;
            for (j=i; j<popsize; j++)
            {
                new_pop->ind[j].rank = rank;
            }
        }
        temp2 = elite->child;
        do
        {
            temp2 = del (temp2);
            temp2 = temp2->child;
        }
        while (elite->child !=NULL);
    }
    while (archieve_size < popsize);
    while (pool!=NULL)
    {
        temp1 = pool;
        pool = pool->child;
        free (temp1);
    }
    while (elite!=NULL)
    {
        temp1 = elite;
        elite = elite->child;
        free (temp1);
    }
    return;
}

/* Routine to fill a population with individuals in the decreasing order of crowding distance */
void crowding_fill (population *mixed_pop, population *new_pop, int count, int front_size, list *elite)
{
    int *dist;
    list *temp;
    int i, j;
    /*对该层个体进行拥挤距离判断，由于这些个体还没有加入到新种群中（矩阵），仍然使用链表保持所以调用函数*/
    assign_crowding_distance_list(mixed_pop, elite->child, front_size); /*对该层个体进行拥挤距离计算，计算后的距离信息保存在临时种群中  mixed_pop*/
    dist = (int *)malloc(front_size*sizeof(int));
    temp = elite->child;

    /*用  数组  dist 保存  链表中对应个体的序号（mixed_pop中的序号）*/
    for (j=0; j<front_size; j++)
    {
        dist[j] = temp->index;
        temp = temp->child;
    }

    /* 对 mixed_pop  中的  dist对应的个体进行拥挤距离排序， 之后dist数组对应个个体序号便是根据拥挤距离排序的*/
    quicksort_dist (mixed_pop, dist, front_size);

    /*对排序后的个体索引 dist， 从最大拥挤距离开始选择个体填入到新种群中直到填满为止*/
    for (i=count, j=front_size-1; i<popsize; i++, j--)
    {
        copy_ind(&mixed_pop->ind[dist[j]], &new_pop->ind[i]);
    }
    
    free (dist);
    return;
}
