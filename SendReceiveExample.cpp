#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <string>
#include "CGMTimers.h"
#include "Comm.h"
#include "SimpleCommObject.h"
#include "GeneralUtilities.h"
#include "utils.h"

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
	int **total_matrix=new int*[matrix_size];
	for (int i=0;i<matrix_size;i++)
		total_matrix[i]=new int[matrix_size];
	for (int row=0;row<values.size();row++)
		total_matrix[row][row]=0;
	int **matrix=new int*[bloco];
	for (int i=0;i<bloco;i++)
		matrix[i]=new int[matrix_size];
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
		int **submatrix=new int*[row_size];
		for (int i=0;i<row_size;i++)
			submatrix[i]=new int[col_size];
		copyToSubMatrix(&matrix,&submatrix,row_start,row_end,col_start,col_end);
		convertMatrixToList(&submatrix,row_start, row_end, col_start, col_end, &data_to_send);
		CommObjectList data_to_receive(&sample);
		if (id!=0)
		{
			comm -> send(id-1,data_to_send,id-1);
		}
		if (id!=p-1)
		{
			comm -> receive(id+1,data_to_receive,id+1,&actualSource);
			last_data_received=&data_to_receive;
		}
	}
	if (id==0)
	{
		printf("Custo total da matriz %d",matrix[0][values.size()-1]);
	}

  comm -> dispose(); 
}

