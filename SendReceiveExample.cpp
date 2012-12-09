#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include "CGMTimers.h"
#include "Comm.h"
#include "SimpleCommObject.h"
#include "GeneralUtilities.h"

void copyToSubMatrix(int ***matrix, int ***submatrix, int row_start, int row_end, int col_start, int col_end);
void workOnSubMatrix(int ***cost_matrix, int row_start, int row_end, int col_start, int col_end, CommObjectList **last_received);
void convertMatrixToList(int ***matrix, int row_start, int row_end, int col_start, int col_end, CommObjectList **result);


int main (int argc, char **argv)
{
  CGMTimers *timers = new CGMTimers ();
  Comm *comm = Comm::getComm (&argc, &argv, timers);
  int n = -1;
  int p = comm->getNumberOfProcessors ();
  printf("Number of processors %d\n",p);
  int id = comm->getMyId ();
  printf("ID%d\n",id);
  int sendTo = (id + 1) % p;
  int receiveFrom = (id - 1 + p) % p;
  int actualSource = -1;
  vector<int> values;
  if (argc > 1)
  {
	char *size=strtok(argv[1],",");
	while (size!=NULL)
	{
		values.push_back(atoi(size));
		size=strtok(NULL,",");
	}
  }
  else
  {
	printf("Ta faltando argumento com tamanho das matrices: ex. 1,3,4,5,6");
  }

	int bloco=values.size()-1/p;
	comm -> synchronize();
	SimpleCommObject<int> sample(0);
	int matrix_size=values.size()-1;
	int **total_matrix=new int[matrix_size][matrix_size];
	for (int row=0;row<list.size();row++)
	{
		total_matrix[row][row]=values[row];
	}
	int **matrix=new int[bloco][matrix_size];
	CommObjectList *last_data_received=0;
	for (int rodada=0;rodada<=p-id;rodada++)
	{
		CommObjectList data_to_send(&sample);
		int row_start=0;
		int row_end=bloco-1;
		int col_start=rodada*bloco;
		int col_end=((rodada+1)*bloco)-1;
		int offset=0;
		while (matrix[row_start][col_start+offset]!=0)
			offset++;
		col_start+=offset;
		col_end+=offset;
		int row_size=row_end-row_start+1;
		int col_size=col_end-col_start+1;
		workOnSubMatrix(&matrix,row_start,row_end,col_start,col_end, &last_data_received);
		int **submatrix=new int[row_size][col_size];
		copyToSubMatrix(&matrix,&submatrix,row_start,row_end,col_start,col_end);
		convertMatrixToList(&submatrix,row_start, row_end, col_start, col_end, &data_to_send);
		CommObjectList data_to_receive(&sample);
		if (id!=0)
		{
			comm -> send(id-1,data_to_send,id-1);
		}
		if (id!=p-1)
		{
			comm -> receive(id+1,data_receive,id+1,&actualSource);
			last_data_received=&data_to_receive;
		}
	}
	if (id==0)
	{
		printf("Custo total da matriz %d",matrix[0][list.size()-2]);
	}

  comm -> dispose(); 
}

void copyToSubMatrix(int ***matrix, int ***submatrix, int row_start, int row_end, int col_start, int col_end)
{
	for (int row=row_start;row<row_end;row++)
		for (int col=col_start;col<col_end;col++)
		{
			(*submatrix)[row][col-col_start]=(*matrix)[row][col];
		}
}

void workOnSubMatrix(int ***cost_matrix, int row_start, int row_end, int col_start, int col_end, CommObjectList *last_received)
{
	if (last_received!=0)
	{
		
	}
	else
	{
		int rec_row_start=0;
		SimpleCommObject<int> *row_start_obj=(*result)[0];
		rec_row_start=row_start_obj->getData();
		
		int rec_row_end=0;
		SimpleCommObject<int> *row_end_obj=(*result)[1];
		rec_row_end=row_end_obj->getData();

		int rec_col_start=0;
		SimpleCommObject<int> *col_start_obj=(*result)[2];
		rec_col_start=col_start_obj->getData();

		int rec_col_end=0;
		SimpleCommObject<int> *col_end_obj=(*result)[3];
		rec_col_end=col_end_obj->getData();

		int rows=rec_row_end-rec_row_start+1;
		int cols=rec_col_end-rec_col_start+1;

		int index=4;
		for (int i=rec_row_start;i<=rec_row_end;i++)
			for (int j=rec_col_start;j<=rec_col_end;j++)
			{
				SimpleCommObject<int>* next=(*result)[index];
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
	SimpleCommObject<int> *row_start_obj=(*result)[0];
	row_start_obj->setData(row_start);

	SimpleCommObject<int> *row_end_obj=(*result)[1];
	row_end_obj->setData(row_end);

	SimpleCommObject<int> *col_start_obj=(*result)[2];
	col_start_obj->setData(col_start);

	SimpleCommObject<int> *col_end_obj=(*result)[3];
	col_end_obj->setData(col_end);

	int elem_index=4;
	for (int row=0;row<row_end-row_start+1;row++)
	{
		for(int col=0;col<col_end-col_start+1;col++)
		{
			SimpleCommObject<int>* new_elem=dynamic_cast<SimpleCommObject<int>* >((*result)[elem_index]);
			new_elem->setData((*matrix)[row][col]);
			elem_index++;
		}
		elem_index++;
	}
}
