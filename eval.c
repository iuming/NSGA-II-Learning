/* Routine for evaluating population members  */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Routine to evaluate objective function values and constraints for a population */
void evaluate_pop (population *pop)
{
    int i;
    for (i=0; i<popsize; i++)
    {
        evaluate_ind (&(pop->ind[i]));
    }
    return;
}

/* Routine to evaluate objective function values and constraints for an individual */
void evaluate_ind (individual *ind)
{
    int j;
    test_problem (ind->xreal, ind->xbin, ind->gene, ind->obj, ind->constr);

    /*根据标识位判断是否需要进行  超限  处理*/
    if (ncon==0)
    {
        ind->constr_violation = 0.0;
    }
    else
    {
        ind->constr_violation = 0.0;

        /*对个体的所有限制条件遍历， 如果  某限制条件 constr[j]>=0 ,  证明该个体在该条件上没有超限*/
        for (j=0; j<ncon; j++)
        {
            if (ind->constr[j]<0.0)
            {
                /*将  个体  所有超出限制条件的数值相加（所有小于0的 条件数值  constr[j]<0）,  其和便是该个体的超限的非法数值 constr_violation*/
                ind->constr_violation += ind->constr[j];
                /*constr_violation  小于0  说明该个体有超出限制条件的情况，  等于0  说明没有超出限制，   不存在大于0的情况*/
            }
        }
    }
    return;
}
