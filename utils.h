#ifndef __UTILS__
#define __UTILS__

#include "CommObjectList.h"
#include "SimpleCommObject.h"

void copyToSubMatrix(int ***matrix, int ***submatrix, int row_start, int row_end, int col_start, int col_end);
void workOnSubMatrix(int ***cost_matrix, int row_start, int row_end, int col_start, int col_end, CommObjectList **last_received);
void convertMatrixToList(int ***matrix, int row_start, int row_end, int col_start, int col_end, CommObjectList *result);

#endif