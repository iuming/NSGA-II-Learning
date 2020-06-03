/* Domination checking routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Routine for usual non-domination checking
   It will return the following values
   1 if a dominates b
   -1 if b dominates a
   0 if both a and b are non-dominated */

int check_dominance (individual *a, individual *b)
{
    int i;
    int flag1;
    int flag2;
    flag1 = 0;
    flag2 = 0;
    /* 基本遵循两个原则，首先是判断两个个体是是否超出限制条件，即判断   constr_violation  的大小 */
    if (a->constr_violation<0 && b->constr_violation<0)
    {
        /* 如果个体  没有超过限制条件  则个体的  constr_violation >= 0，constr_violation 默认值  为  0 ，然后判断两个个体之间的支配关系 */
        if (a->constr_violation > b->constr_violation)
        {
            return (1);
        }
        else
        {
            if (a->constr_violation < b->constr_violation)
            {
                return (-1);
            }
            else
            {
                return (0);
            }
        }
    }
    else
    {
        /* 如果两个个体都超出限制了则选出   超出限制较小  的个体，  即  constr_violation  较大个体 */
        if (a->constr_violation < 0 && b->constr_violation == 0)
        {
            return (-1);
        }
        else
        {
            if (a->constr_violation == 0 && b->constr_violation <0)
            {
                return (1);
            }
            else
            {
                /*
                     如果两个个体都超出限制了  而  constr_violation  相等， 则判断两个个体  支配关系为  互不支配
                     如果两个个体  均没有超出限制，  则判断两个个体的支配关系。这里的具体操作是对两个个体的各个目标函数值进行大小判断
                */
                for (i=0; i<nobj; i++)
                {
                    /* 如果  a  个体的目标函数有小于 b 个体的, flag1==1 */
                    if (a->obj[i] < b->obj[i])
                    {
                        flag1 = 1;
                    }
                    else
                    /* 如果  b  个体的目标函数有小于  a 个体的, flag2==1 */
                    {
                        if (a->obj[i] > b->obj[i])
                        {
                            flag2 = 1;
                        }
                    }
                }
                /* 如果 flag1==1  flag2==0, 则  a  支配  b */
                if (flag1==1 && flag2==0)
                {
                    return (1);
                }
                else
                {
                    /* 如果 flag1==0  flag2==1, 则  b  支配  a */
                    if (flag1==0 && flag2==1)
                    {
                        return (-1);
                    }
                    else
                    /* 如果 flag1==1  flag2==1, 则  a    b  互不支配 */
                    {
                        return (0);
                    }
                }
            }
        }
    }
}
