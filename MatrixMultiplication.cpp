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
	printf("Falt argumento com os tamanhos das matrices: ex. 1,3,4,5,6\n");
  }
//	printf("size %d\n",values.size());
//	int bloco=(values.size()-1)/p;
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
	int *blocos=new int[p];
	int q=(values.size()-1)/p;
	if (q<1) q=1;
	int r=(values.size()-1)%p;
	if (r<1) r=0;
	for (int i=0;i<p;i++)
	{
		if (i<r)
			blocos[i]=q+1;
		else
			blocos[i]=q;
	}
	//printf("ID: %d Bloco: %d\n",id,blocos[id]);
	int bloco_offset=0;
	for (int i=0;i<id;i++)
		bloco_offset+=blocos[i];
	//int row_start=blocos[id]*id;
	int row_start=bloco_offset;
	int row_end=row_start+blocos[id]-1;
	//printf("ID %d, row_start %d row_end %d\n",id,row_start, row_end);
	for (int rodada=0;rodada<=p-id-1;rodada++)
	{
		//printf("ID %d RODADA %d\n",id,rodada);
		//int col_start=blocos[id]*(rodada+id);
		int col_start=bloco_offset+(blocos[id]*rodada);
		//int col_end=blocos[id]*(rodada+id+1)-1;
		int col_end=col_start+blocos[id]-1;
		if (col_start>values.size()-2)
			break;
	//	printf("ID %d, col_start %d, col_end %d RODADA: %d\n",id,col_start, col_end,rodada);
		CommObjectList data_to_send(&sample);

		workOnSubMatrix(&total_matrix, &values, row_start,row_end,col_start,col_end, rodada, blocos[id], id);
		//printf("ID: %d SAIU RODADA: %d\n",id,rodada);
		int **submatrix=new int*[row_end-row_start+1];
		for (int i=0;i<row_end-row_start+1;i++)
			submatrix[i]=new int[col_end-col_start+1];
/*if (id==1)
{		
	for (int row=0;row<matrix_size;row++)
	{
		for (int col=0;col<matrix_size;col++)
			{
				printf("R>%d %d,%d=%d ",rodada,row,col,total_matrix[row][col]);
			}
		printf("\n");
	}
}*/

		convertMatrixToList(&total_matrix,row_start, col_end, col_start, col_end, &data_to_send,id,rodada);
		//printf("ID: %d, COPIOU SUBM RODADA: %d\n",id,rodada);

		CommObjectList data_to_receive(&sample);


		if (id!=0)
		{
			//printf("ID: %d IS SENDING RODADA: %d\n",id,rodada);
			comm -> send(id-1,data_to_send,rodada);
//			printf("MANDOU DE ID: %d a ID: %d TAG_ROUND:%d\n",id,id-1,rodada);
		}
		if (id!=p-1)
		{
			if (col_end>=values.size()-2)
			{
//				printf("ID: %d SAINDO\n",id);
				continue;
			}else
			{
			
				int num_receives=blocos[id]/blocos[id+1];
//				printf("ID: %d ESPERA RECEBER %d DE ID: %d RODADA: %d\n",id,num_receives,id+1,rodada);

			for (int i=0;i<num_receives;i++)
			{
				comm -> receive(id+1,data_to_receive,rodada+i,&actualSource);
				copyFromSubMatrix(&total_matrix,&data_to_receive,id+1,id,rodada+i);

//				printf("ID: %d RECEBEU DE ID: %d TAG_ROUND: %d\n",id,id+1, rodada+i);	
			}
						}
		}
	}
//	printf("ID %d terminou\n",id);

	if (id==0)
	{
		printf("Custo total da matriz %d\n",total_matrix[0][values.size()-2]);
	}
	  comm -> dispose(); 
}

