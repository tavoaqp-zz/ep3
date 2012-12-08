#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <list>
#include <string>
#include "CGMTimers.h"
#include "Comm.h"
#include "SimpleCommObject.h"
#include "GeneralUtilities.h"

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
  list<int> values;
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
	
	bloco=list.size()-1/p;
	comm -> synchronize();
	SimpleCommObject<int> sample(0);
	int **matrix=new int[bloco][list.size()-1];
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
		convertMatrixToList(&submatrix,&data_to_send);
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

/*  SimpleCommObject<int> sample(0); 
  CommObjectList data1(&sample);
  data1.setSize(values.size());
  list<int>::iterator it;
  SimpleCommObject<int> *curObj;
  int index=0;
  for(it=values.begin();it!=values.end();it++)
  {
	int val=*it;
	curObj=dynamic_cast<SimpleCommObject<int>*>(data1[index]);
 	curObj->setData(val);
	index++;
  }
  SimpleCommObject<int> sample2(0);
  CommObjectList data2(&sample2);
  data2.setSize(2);
  SimpleCommObject<int> *curObj2;
  for (int i=0;i<2;i++)
  {
	curObj2=dynamic_cast<SimpleCommObject<int>*>(data2[i]);
	curObj2->setData(i);
  }
  CommObjectList results(&sample2);
  comm -> synchronize();
  if (id == Comm::PROC_ZERO)
  {
	comm->send(1,data1,id);
        comm->receive(1,results,1,&actualSource);	
  }
  else
  {
	comm->receive(0,results,0,&actualSource);
	comm->send(0,data2,id);
  }
  printf("Process %d Received data, printing...\n",id);
  SimpleCommObject<int> *curObj3;
  for (int i=0;i<results.getSize();i++)
  {
	curObj3=dynamic_cast<SimpleCommObject<int>*>(results[i]);
	printf("Process %d received Val %d\n",id,curObj3->getData());
  }*/
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
