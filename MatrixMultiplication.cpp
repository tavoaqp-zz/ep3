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
  int tag = 0;
  int p = comm->getNumberOfProcessors ();
  //printf("Number of processors %d\n",p);
  int id = comm->getMyId ();
  //printf("ID%d\n",id);
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
	printf("Ta faltando argumento com tamanho das matrices: ex. 1,3,4,5,6\n");
  }
	//printf("size %d\n",values.size());
	int bloco=(values.size()-1)/p;
	//printf("Bloco %d\n",bloco);
		SimpleCommObject<int> sample(0);
	int matrix_size=values.size()-1;
	int **total_matrix=new int*[matrix_size];
	for (int i=0;i<matrix_size;i++)
		total_matrix[i]=new int[matrix_size];
	for (int row=0;row<matrix_size;row++)
		for (int col=0;col<matrix_size;col++)
			{
				if (row==col)
					total_matrix[row][row]=0;
				else
					total_matrix[row][col]=-1;
			}

	int row_start=bloco*id;
	int row_end=bloco*(id+1)-1;
	//printf("ID %d, row_start %d row_end %d\n",id,row_start, row_end);
	for (int rodada=0;rodada<=p-id-1;rodada++)
	{
		//printf("ID %d RODADA %d\n",id,rodada);
		int col_start=bloco*(rodada+id);
		int col_end=bloco*(rodada+id+1)-1;
		//printf("ID %d, col_start %d, col_end %d RODADA: %d\n",id,col_start, col_end,rodada);
		CommObjectList data_to_send(&sample);

		workOnSubMatrix(&total_matrix, &values, row_start,row_end,col_start,col_end, rodada, bloco, id);
		//printf("ID: %d SAIU RODADA: %d\n",id,rodada);
		int **submatrix=new int*[row_end-row_start+1];
		for (int i=0;i<row_end-row_start+1;i++)
			submatrix[i]=new int[col_end-col_start+1];
		convertMatrixToList(&total_matrix,row_start, col_end, col_start, col_end, &data_to_send);
		//printf("ID: %d, COPIOU SUBM RODADA: %d\n",id,rodada);

		CommObjectList data_to_receive(&sample);
		if (id!=0)
		{
			//printf("ID: %d IS SENDING RODADA: %d\n",id,rodada);
			comm -> send(id-1,data_to_send,rodada);
			printf("MANDOU DE ID: %d a ID: %d RODADA:%d\n",id,id-1,rodada);
		}
		if (id!=p-1)
		{
			if (rodada==p-id-1)
			{
				printf("SAINDO ID %d LOOP",id);
				continue;
			}else
			{
			
			printf("ID: %d ESPERA RECEBER DE ID: %d RODADA: %d\n",id,id+1,rodada);
			comm -> receive(id+1,data_to_receive,rodada,&actualSource);
			printf("ID: %d RECEBEU DE ID: %d RODADA: %d\n",id,id+1, rodada);
			copyFromSubMatrix(&total_matrix,&data_to_receive,id+1,id,rodada);
			}
		}

	}
	printf("ID %d terminou\n",id);

	if (id==0)
	{
		printf("Custo total da matriz %d\n",total_matrix[0][values.size()-2]);
	for (int row=0;row<matrix_size;row++)
	{
		for (int col=0;col<matrix_size;col++)
			{
				printf("%d ",total_matrix[row][col]);
			}
		printf("\n");
	}


	}
	  comm -> dispose(); 
}

