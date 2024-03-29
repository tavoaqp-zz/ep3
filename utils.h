#ifndef __UTILS__
#define __UTILS__

#include "CommObjectList.h"
#include "SimpleCommObject.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void copyToSubMatrix(int ***matrix, int ***submatrix, int row_start, int row_end, int col_start, int col_end);
void copyFromSubMatrix(int ***matrix, CommObjectList *result,int src_id, int dst_id,int rodada);
void workOnSubMatrix(int ***matrix, vector<int>* values, int row_start, int row_end, int col_start, int col_end, int rodada, int bloco, int id);
void convertMatrixToList(int ***matrix, int row_start, int row_end, int col_start, int col_end, CommObjectList *result, int id, int rodada);

#endif
