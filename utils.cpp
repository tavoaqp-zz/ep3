#include "utils.h"

void copyToSubMatrix(int ***matrix, int ***submatrix, int row_start, int row_end, int col_start, int col_end)
{
	for (int row=row_start;row<row_end;row++)
		for (int col=col_start;col<col_end;col++)
		{
			(*submatrix)[row][col-col_start]=(*matrix)[row][col];
		}
}

void workOnSubMatrix(int ***matrix, int row_start, int row_end, int col_start, int col_end, CommObjectList **result)
{
	if (result!=0)
	{

	}
	else
	{
		int rec_row_start=0;
		SimpleCommObject<int> *row_start_obj=dynamic_cast<SimpleCommObject<int>*>((**result)[0]);
		rec_row_start=row_start_obj->getData();

		int rec_row_end=0;
		SimpleCommObject<int> *row_end_obj=dynamic_cast<SimpleCommObject<int>*>((**result)[1]);
		rec_row_end=row_end_obj->getData();

		int rec_col_start=0;
		SimpleCommObject<int> *col_start_obj=dynamic_cast<SimpleCommObject<int>*>((**result)[2]);
		rec_col_start=col_start_obj->getData();

		int rec_col_end=0;
		SimpleCommObject<int> *col_end_obj=dynamic_cast<SimpleCommObject<int>*>((**result)[3]);
		rec_col_end=col_end_obj->getData();

		int rows=rec_row_end-rec_row_start+1;
		int cols=rec_col_end-rec_col_start+1;

		int index=4;
		for (int i=rec_row_start;i<=rec_row_end;i++)
			for (int j=rec_col_start;j<=rec_col_end;j++)
			{
				SimpleCommObject<int>* next=dynamic_cast<SimpleCommObject<int>*>((**result)[index]);
				(*matrix)[i][j]=next->getData();
				index++;
			}
	}
}

void convertMatrixToList(int ***matrix, int row_start, int row_end, int col_start, int col_end, CommObjectList *result)
{
	int rows=row_end-row_start+1;
	int cols=col_end-col_start+1;
	result->setSize(4+rows*cols+(rows-1));
	SimpleCommObject<int> *row_start_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[0]);
	row_start_obj->setData(row_start);

	SimpleCommObject<int> *row_end_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[1]);
	row_end_obj->setData(row_end);

	SimpleCommObject<int> *col_start_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[2]);
	col_start_obj->setData(col_start);

	SimpleCommObject<int> *col_end_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[3]);
	col_end_obj->setData(col_end);

	int elem_index=4;
	for (int row=0;row<row_end-row_start+1;row++)
	{
		for(int col=0;col<col_end-col_start+1;col++)
		{
			SimpleCommObject<int>* new_elem=dynamic_cast<SimpleCommObject<int>*>((*result)[elem_index]);
			new_elem->setData((*matrix)[row][col]);
			elem_index++;
		}
		elem_index++;
	}
}
