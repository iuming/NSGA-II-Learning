/* Crowding distance computation routines */

# include <stdio.h>
# include <stdlib.h>
# include <math.h>

# include "global.h"
# include "rand.h"

/* Routine to compute crowding distance based on ojbective function values when the population in in the form of a list */
void assign_crowding_distance_list (population *pop, list *lst, int front_size)
{
    int **obj_array;
    int *dist;
    int i, j;
    list *temp;
    temp = lst;
    if (front_size==1)
    {
        pop->ind[lst->index].crowd_dist = INF;
        return;
    }
    if (front_size==2)
    {
        pop->ind[lst->index].crowd_dist = INF;
        pop->ind[lst->child->index].crowd_dist = INF;
        return;
    }
    obj_array = (int **)malloc(nobj*sizeof(int*));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nobj; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = temp->index;
        temp = temp->child;
    }
    assign_crowding_distance (pop, dist, obj_array, front_size);
    free (dist);
    for (i=0; i<nobj; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}

/* Routine to compute crowding distance based on objective function values when the population in in the form of an array */
void assign_crowding_distance_indices (population *pop, int c1, int c2)
{
    int **obj_array;
    int *dist;
    int i, j;
    int front_size;
    front_size = c2-c1+1;
    if (front_size==1)
    {
        pop->ind[c1].crowd_dist = INF;
        return;
    }
    if (front_size==2)
    {
        pop->ind[c1].crowd_dist = INF;
        pop->ind[c2].crowd_dist = INF;
        return;
    }
    obj_array = (int **)malloc(nobj*sizeof(int*));
    dist = (int *)malloc(front_size*sizeof(int));
    for (i=0; i<nobj; i++)
    {
        obj_array[i] = (int *)malloc(front_size*sizeof(int));
    }
    for (j=0; j<front_size; j++)
    {
        dist[j] = c1++;
    }
    assign_crowding_distance (pop, dist, obj_array, front_size);
    free (dist);
    for (i=0; i<nobj; i++)
    {
        free (obj_array[i]);
    }
    free (obj_array);
    return;
}

/* Routine to compute crowding distances */
/*距离计算的核心代码*/
void assign_crowding_distance (population *pop, int *dist, int **obj_array, int front_size)
{
    int i, j;

    /* 初始化  多目标索引矩阵，并对其不同目标列进行索引排序（按照目标函数值的大小）*/
    for (i=0; i<nobj; i++)
    {
        for (j=0; j<front_size; j++)
        {
            obj_array[i][j] = dist[j];
        }
        quicksort_front_obj (pop, i, obj_array[i], front_size);
    }

    /* 个体的拥挤距离初始化*/
    for (j=0; j<front_size; j++)
    {
        pop->ind[dist[j]].crowd_dist = 0.0;
    }

    /* 对按照某目标函数排序后最小的个体的拥挤距离赋值为无穷（并没有对两端赋值无穷，而是最小端的个体）*/
    for (i=0; i<nobj; i++)
    {
        pop->ind[obj_array[i][0]].crowd_dist = INF;
    }

    for (i=0; i<nobj; i++)
    {
        for (j=1; j<front_size-1; j++)
        {
            if (pop->ind[obj_array[i][j]].crowd_dist != INF) /*如果一个个体的拥挤距离已经被赋值为无穷，则对其不再计算*/
            {
                if (pop->ind[obj_array[i][front_size-1]].obj[i] == pop->ind[obj_array[i][0]].obj[i])
                /*如果某目标函数的排序后所有个体的该目标函数值相同，则该目标函数贡献的距离为 0*/
                {
                    pop->ind[obj_array[i][j]].crowd_dist += 0.0;
                }
                else
                {
                    /*某个个体在某一个目标函数上的拥挤距离  为 该个体在该目标函数上前后个体距离之差除以该层该目标的最大最小值之差*/
                    pop->ind[obj_array[i][j]].crowd_dist += (pop->ind[obj_array[i][j+1]].obj[i] - pop->ind[obj_array[i][j-1]].obj[i])/(pop->ind[obj_array[i][front_size-1]].obj[i] - pop->ind[obj_array[i][0]].obj[i]);
                }
            }
        }
    }

    /*将拥挤距离不为无穷的所有个体的拥挤距离除以目标函数值个数（归一化操作）*/
    for (j=0; j<front_size; j++)
    {
        if (pop->ind[dist[j]].crowd_dist != INF)
        {
            pop->ind[dist[j]].crowd_dist = (pop->ind[dist[j]].crowd_dist)/nobj;
        }
    }
    return;
}
