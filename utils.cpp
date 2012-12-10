#include "utils.h"

void copyToSubMatrix(int ***matrix, int ***submatrix, int row_start, int row_end, int col_start, int col_end)
{
	for (int row=row_start;row<row_end;row++)
		for (int col=col_start;col<col_end;col++)
		{
			(*submatrix)[row][col-col_start]=(*matrix)[row][col];
		}
}

void copyFromSubMatrix(int ***matrix, CommObjectList *result, int src_id, int dst_id, int rodada)
{
	int rec_row_start=0;
	SimpleCommObject<int> *row_start_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[0]);
	rec_row_start=row_start_obj->getData();

	int rec_row_end=0;
	SimpleCommObject<int> *row_end_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[1]);
	rec_row_end=row_end_obj->getData();

	int rec_col_start=0;
	SimpleCommObject<int> *col_start_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[2]);
	rec_col_start=col_start_obj->getData();

	int rec_col_end=0;
	SimpleCommObject<int> *col_end_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[3]);
	rec_col_end=col_end_obj->getData();

	int rows=rec_row_end-rec_row_start+1;
	int cols=rec_col_end-rec_col_start+1;

	int index=4;
	//printf("SRC_ID: %d DST_ID: %d ROW_START: %d ROW_END:%d COL_START: %d COL_END: %d - RODADA %d\n",src_id,dst_id,rec_row_start,rec_row_end,rec_col_start,rec_col_end,rodada);
	for (int i=rec_row_start;i<=rec_row_end;i++)
		for (int j=rec_col_start;j<=rec_col_end;j++)
		{
			SimpleCommObject<int>* next=dynamic_cast<SimpleCommObject<int>*>((*result)[index]);
			(*matrix)[i][j]=next->getData();
			index++;
		}

}

void workOnSubMatrix(int ***matrix, vector<int>* values, int row_start, int row_end, int col_start, int col_end, int rodada, int bloco, int id)
{
	int size=values->size()-1;
	int rows=row_end-row_start+1;
	int cols=col_end-col_start+1;
	int start_diag=0;
	int end_diag=0;
	int row_index=0;
	if (cols==1 && rows==1 && (*matrix)[row_start][col_start]==0)
	{
		return;
	}else
	{
		if (rodada==0)
		{
			start_diag=1;
			end_diag=start_diag+bloco-2;
			row_index=id*bloco;
		}else
		{
			start_diag=bloco*(rodada-1)+1;
			end_diag=start_diag+2*(bloco-1);
			row_index=row_end;
		}
//		printf("ID %d, start_diag %d end_diag %d row_index %d\n",id,start_diag,end_diag,row_index);
		for (int diag=start_diag;diag<=end_diag;diag++)
		{
			for (;row_index<=size-diag;row_index++)
			{
				int col_index=row_index+diag;

				if ((*matrix)[row_index][col_index]!=-1) continue;
//				printf("ID %d, calc %d,%d\n",id,row_index,col_index);
				int min=1000000;
				
				for (int k=row_index;k<col_index;k++)
				{
					
					int cur=(*matrix)[row_index][k]+(*matrix)[k+1][col_index]+(*values)[row_index]*(*values)[k+1]*(*values)[col_index+1];
					if (cur<min)
						min=cur;
				}
		//		printf("ID %d, Valor %d,%d=%d - RODADA %d\n",id,row_index,col_index,min, rodada);
				(*matrix)[row_index][col_index]=min;
				if (row_index==row_end || col_index==col_end)
				{
					row_index=row_start;
					break;
				}
			}
		}
	}
}

void convertMatrixToList(int ***matrix, int row_start, int row_end, int col_start, int col_end, CommObjectList *result)
{
	int rows=row_end-row_start+1;
	int cols=col_end-col_start+1;
	result->setSize(4+rows*cols);
	SimpleCommObject<int> *row_start_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[0]);
	row_start_obj->setData(row_start);

	SimpleCommObject<int> *row_end_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[1]);
	row_end_obj->setData(row_end);

	SimpleCommObject<int> *col_start_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[2]);
	col_start_obj->setData(col_start);

	SimpleCommObject<int> *col_end_obj=dynamic_cast<SimpleCommObject<int>*>((*result)[3]);
	col_end_obj->setData(col_end);

	int elem_index=4;
	for (;row_start<=row_end;row_start++)
	{
		for(;col_start<=col_end;col_start++)
		{
			SimpleCommObject<int>* new_elem=dynamic_cast<SimpleCommObject<int>*>((*result)[elem_index]);
			new_elem->setData((*matrix)[row_start][col_start]);
			elem_index++;
		}
		elem_index++;
	}
}
