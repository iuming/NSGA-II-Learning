/* Routines for storing population data into files */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Function to print the information of a population in a file */
/* report_pop   将种群中所有个体的   目标函数值， 限制条件值， 编码值  打印出来 */
void report_pop (population *pop, FILE *fpt)
{
    int i, j, k;
    {
        for (j=0; j<nobj; j++)
        {
            fprintf(fpt,"%e\t",pop->ind[i].obj[j]);
        }
        if (ncon!=0)
        {
            for (j=0; j<ncon; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i].constr[j]);
            }
        }
        if (nreal!=0)
        {
            for (j=0; j<nreal; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i].xreal[j]);
            }
        }
        if (nbin!=0)
        {
            for (j=0; j<nbin; j++)
            {
                for (k=0; k<nbits[j]; k++)
                {
                    fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
                }
            }
        }
        fprintf(fpt,"%e\t",pop->ind[i].constr_violation);
        fprintf(fpt,"%d\t",pop->ind[i].rank);
        fprintf(fpt,"%e\n",pop->ind[i].crowd_dist);
    }
    return;
}

/* Function to print the information of feasible and non-dominated population in a file */
void report_feasible (population *pop, FILE *fpt)
{
    int i, j, k;
    for (i=0; i<popsize; i++)
    {
        /* report_pop   种群中的非支配个体并且限制条件总和为0   （constr_violation == 0.0）的个体的   目标函数值， 限制条件值， 编码值  打印出来。 */
        if (pop->ind[i].constr_violation == 0.0 && pop->ind[i].rank==1)
        {
            for (j=0; j<nobj; j++)
            {
                fprintf(fpt,"%e\t",pop->ind[i].obj[j]);
            }
            if (ncon!=0)
            {
                for (j=0; j<ncon; j++)
                {
                    fprintf(fpt,"%e\t",pop->ind[i].constr[j]);
                }
            }
            if (nreal!=0)
            {
                for (j=0; j<nreal; j++)
                {
                    fprintf(fpt,"%e\t",pop->ind[i].xreal[j]);
                }
            }
            if (nbin!=0)
            {
                for (j=0; j<nbin; j++)
                {
                    for (k=0; k<nbits[j]; k++)
                    {
                        fprintf(fpt,"%d\t",pop->ind[i].gene[j][k]);
                    }
                }
            }
            fprintf(fpt,"%e\t",pop->ind[i].constr_violation);
            fprintf(fpt,"%d\t",pop->ind[i].rank);
            fprintf(fpt,"%e\n",pop->ind[i].crowd_dist);
        }
    }
    return;
}
