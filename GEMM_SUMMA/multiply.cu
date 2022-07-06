#include"cuda_runtime.h"
#include<iostream>
#include<stdlib.h>
using namespace std;

__global__ void matrixMul(float *a, float *b, float *c, int m,int n,int k)
{
    int row = blockIdx.y*blockDim.y+threadIdx.y;
    int column = blockIdx.x*blockDim.x+threadIdx.x;
    int sum=0;
    for(int i=0;i<n;i++)
        sum+=a[row*n+i]*b[i*k+column];
    c[row*k+column]=sum;
}

void multiply(float *h_subA,float *h_subB,float *h_subC,int subA_m, int subA_n,int subB_k, int id)
{
    float *d_subA, *d_subB, *d_subC;
    cudaSetDevice(id);
    cudaDeviceProp deviceProp;
    cudaGetDeviceProperties(&deviceProp,id);
    cout<<"Device["<<id<<"]"<<deviceProp.name<<endl;
    cudaMalloc(&d_subA,subA_m*subA_n*sizeof(float));
    cudaMalloc(&d_subB,subA_n*subB_k*sizeof(float));
    cudaMalloc(&d_subC,subA_m*subB_k*sizeof(float));

    cudaMemcpy(d_subA,h_subA,subA_m*subA_n*sizeof(float),cudaMemcpyHostToDevice);
    cudaMemcpy(d_subB,h_subB,subA_n*subB_k*sizeof(float),cudaMemcpyHostToDevice);

    int threads_per_block = 16;
    dim3 block_size(threads_per_block,threads_per_block);
    dim3 grid_size(subB_k/block_size.x,subA_m/block_size.y);
    matrixMul<<<grid_size,block_size>>>(d_subA,d_subB,d_subC,int subA_m, int subA_n,int subB_k);
    cudaMemcpy(h_subC,d_subC,subA_m*subB_k*sizeof(float),cudaMemcpyDeviceToHost);
    cudaFree(d_subA);
    cudaFree(d_subB);
    cudaFree(d_subC);
}