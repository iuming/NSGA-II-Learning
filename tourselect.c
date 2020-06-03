/* Tournamenet Selections routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Routine for tournament selection, it creates a new_pop from old_pop by performing tournament selection and the crossover */
void selection (population *old_pop, population *new_pop)
{
    int *a1, *a2;
    int temp;
    int i;
    int rand;
    individual *parent1, *parent2;

    /* 分别生成两个种群个体大小的数组 a1  a2，这两个数组里面以后会分别保存乱序的种群个体序号 */
    a1 = (int *)malloc(popsize * sizeof(int)); 
    a2 = (int *)malloc(popsize*sizeof(int));

    /* 对两个数组进行初始话，顺序存放种群个体序号 */
    for (i=0; i<popsize; i++)
    {
        a1[i] = a2[i] = i;
    }

    /* 对a1, a2  数组中存放的个体序号打乱，其中打乱的次数为  popsize  ,该操作基本保证所有个体的序号基本不在其原有位置上 */
    for (i=0; i<popsize; i++)
    {
        rand = rnd (i, popsize-1);
        temp = a1[rand];
        a1[rand] = a1[i];
        a1[i] = temp;
        rand = rnd (i, popsize-1);
        temp = a2[rand];
        a2[rand] = a2[i];
        a2[i] = temp;
    }

    /* 这部分代码完成了遗传算法中的  选择操作  和  交叉操作，其中  old_pop  new_pop  都是相同种群个体大小的种群，其种群大小均为  popsize */
    for (i=0; i<popsize; i+=4)
    {
        /* tournament   锦标赛法，这里面使用的是二元锦标赛选择法，循环体内共有4次  tournament  操作，该循环共执行  popsize/4  次，故共进行了  popsize  次二元锦标赛选择。由于每次选择出一个新个体，所以该方式选择出的新种群 new_pop  个体数   和   旧种群 old_pop  个体数一致 */
        parent1 = tournament (&old_pop->ind[a1[i]], &old_pop->ind[a1[i+1]]);
        parent2 = tournament (&old_pop->ind[a1[i+2]], &old_pop->ind[a1[i+3]]);
        /* crossover  操作进行了  popsize/2  次 ， （其中每次进行交叉操作的时候都是选择两个个体，每次判断选择的两个个体是否进行交叉都要根据给定的交叉概率进行判断），该循环体中crossover函数总共会对   popsize   个个体进行处理 */
        crossover (parent1, parent2, &new_pop->ind[i], &new_pop->ind[i+1]);
        parent1 = tournament (&old_pop->ind[a2[i]], &old_pop->ind[a2[i+1]]);
        parent2 = tournament (&old_pop->ind[a2[i+2]], &old_pop->ind[a2[i+3]]);
        crossover (parent1, parent2, &new_pop->ind[i+2], &new_pop->ind[i+3]);
        /* 注意： crossover  操作  循环调用    popsize/2  次  而不是    popsize  次 */
    }
    free (a1);
    free (a2);
    return;
}

/* Routine for binary tournament */
individual* tournament (individual *ind1, individual *ind2)
{
    int flag;
    /* 调用  check_dominance  函数判断两个个体的支配关系 */
    flag = check_dominance (ind1, ind2);
    if (flag==1)
    {
        return (ind1);
    }
    if (flag==-1)
    {
        return (ind2);
    }
    /* 如果互不支配则判断两个个体的拥挤距离 */
    if (ind1->crowd_dist > ind2->crowd_dist)
    {
        return(ind1);
    }
    if (ind2->crowd_dist > ind1->crowd_dist)
    {
        return(ind2);
    }
    /* 如果都相同这则随机选择一个个体 */
    if ((randomperc()) <= 0.5)
    {
        return(ind1);
    }
    else
    {
        return(ind2);
    }
}
