#include<iostream>
#include<stdlib.h>
#include"multiply.h"
#include"mpi.h"
#include<time.h>

void InitMatrix(float *mat,int m,int n)
{
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
            mat[i*n+j]=rand()%100;
}

void Check(float *A, float *B, float *C,int m,int n, int k)
{
    float mat_check[m*k];
    memset(mat_check,0,m*k*sizeof(float));
    for(int i=0;i<m;i++)
        for(int j=0;j<k;j++)
            for(int t=0;t<n;t++)
                mat_check[i*k+j]+=A[i*n+t]*B[t*k+j];
    int count=0;
    for(int i=0;i<m*k;i++)
        if(mat_check[i]!=C[i])
            count++;
    cout<<count<<" errors"<<endl;
}

void matrixAdd(float *A,float *B,int m,int k)
{
    for(int i=0;i<m;i++)
        for(int j=0;j<k;j++)
            A[i*k+j]+=B[i*k+j];
}

void Traverse(float *mat,int m,int n)
{
    float t;
    for(int i=0;i<m;i++)
        for(int j=0;j<n;j++)
        {
            t=b[i*n+j];
            b[i*n+j] = b[j*m+i];
            b[j*m+i] = t;
        }
}

int main(int argc, char *argv[])
{
    int rank,num_of_process;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&num_of_process);

    int m,n,k;

    m = atoi(argv[1]);
    n = atoi(argv[2]);
    k = atoi(argv[3]);

    float *matA = (float*)malloc(m*n*sizeof(float));
    float *matB = (float*)malloc(n*k*sizeof(float));
    float *matC = (float*)malloc(m*k*sizeof(float));

    int subA_m=m/num_of_process;
    int subA_n=n/num_of_process;
    int subB_k=k/num_of_process;
    InitMatrix(matA,m,n);
    InitMatrix(matB,m,n);

    int num_of_GPUs = 2;

    Traverse(matA,m,n);

    if(rank==0)
    {
        for(int i=0;i<num_of_process;i++)
        {
            int tag = i;
            MPI_Send(matA+i*m*subA_n,m*subA_n,MPI_FLOAT,i,tag,MPI_COMM_WORLD);
            MPI_Send(matB+i*subA_n*k,subA_n*k,MPI_FLOAT,i,tag*10,MPI_COMM_WORLD);
        }
    }

    float *subMatA = (float*)malloc(m*subA_n*sizeof(float));
    float *subMatB = (float*)malloc(subA_n*k*sizeof(float));
    float *subMatC = (float*)malloc(m*k*sizeof(float));

    MPI_Recv(subMatA,m*subA_n,MPI_FLOAT,0,rank,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    MPI_Recv(subMatB,subA_n*k,MPI_FLOAT,0,(rank+1)*10,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    Traverse(subMatA,m,subA_n);

    multiply(subMatA,subMatB,subMatC,subA_m,subA_n,subB_k,rank);

    MPI_Send(subMatC,m*k,MPI_FLOAT,0,(rank+5)*20,MPI_COMM_WORLD);

    if(rank==0)
    {
        float *recMatC = (float*)malloc(m*k*sizeof(float));
        for(int i=0;i<num_of_process;i++)
        {
            MPI_Recv(recMatC,m*k,MPI_FLOAT,(i+5)*20,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            matrixAdd(matC,recMatC,m,k);
        }

        Check(matA,matB,matC,m,n,k);
    }
    
    return 0;
}