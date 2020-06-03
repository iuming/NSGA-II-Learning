/* Routines to decode the population */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to decode a population to find out the binary variable values based on its bit pattern */
/*种群解码函数  decode_pop  为包装函数， 核心调用函数为  decode_ind  ， 对每个个体进行解码*/
void decode_pop (population *pop)
{
    int i;
    if (nbin!=0)
    {
        for (i=0; i<popsize; i++)
        {
            decode_ind (&(pop->ind[i]));
        }
    }
    return;
}

/* Function to decode an individual to find out the binary variable values based on its bit pattern */
/*核心解码代码*/
void decode_ind (individual *ind)
{
    int j, k;
    int sum;
    if (nbin!=0)
    {
        for (j=0; j<nbin; j++)
        {
            sum=0;
            for (k=0; k<nbits[j]; k++)
            {
                if (ind->gene[j][k]==1)
                {
                    sum += pow(2, nbits[j] - 1 - k); /*将个体某变量的比特编码  转换为  实数*/
                }
            }
            /*因为需要考虑精度问题，所以二进制编码的个体长度表示的范围空间(double)(pow(2,nbits[j])-1)在考虑精度的情况下   要大于 该变量的 范围空间 （ max_binvar[j] - min_binvar[j] ）*/
            ind->xbin[j] = min_binvar[j] + (double)sum*(max_binvar[j] - min_binvar[j])/(double)(pow(2,nbits[j])-1);
        }
    }
    return;
}
