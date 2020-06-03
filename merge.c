/* Routine for mergeing two populations */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Routine to merge two populations into one */
void merge(population *pop1, population *pop2, population *pop3)
{
    int i, k;
    for (i=0; i<popsize; i++)
    {
        copy_ind (&(pop1->ind[i]), &(pop3->ind[i]));
    }
    for (i=0, k=popsize; i<popsize; i++, k++)
    {
        copy_ind (&(pop2->ind[i]), &(pop3->ind[k]));
    }
    return;
}

/* Routine to copy an individual 'ind1' into another individual 'ind2' */
// 具体进行两个个体 内容拷贝的函数
void copy_ind (individual *ind1, individual *ind2)
{
    int i, j;
    ind2->rank = ind1->rank; // 复制个体的  支配层  排序值
    ind2->constr_violation = ind1->constr_violation; // 复制个体的  限制条件  的超出值， 大于等于0未超出， 小于0为超出限制
    ind2->crowd_dist = ind1->crowd_dist;             // 复制个体的  拥挤距离

    // 判断个体的编码， 把个体的具体编码值 复制
    if (nreal!=0)
    {
        for (i=0; i<nreal; i++)
        {
            ind2->xreal[i] = ind1->xreal[i];
        }
    }
    if (nbin!=0)
    {
        for (i=0; i<nbin; i++)
        {
            ind2->xbin[i] = ind1->xbin[i];
            for (j=0; j<nbits[i]; j++)
            {
                ind2->gene[i][j] = ind1->gene[i][j];
            }
        }
    }

    // 复制个体的  各个  目标函数的数值
    for (i=0; i<nobj; i++)
    {
        ind2->obj[i] = ind1->obj[i];
    }

    // 复制个体的  各个限制条件的  数值， 所有限制条件的数值之和等于 constr_violation
    if (ncon!=0)
    {
        for (i=0; i<ncon; i++)
        {
            ind2->constr[i] = ind1->constr[i];
        }
    }
    return;
}
