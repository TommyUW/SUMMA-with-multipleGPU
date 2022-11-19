# SUMMA-with-multipleGPU
This program is designed to operate SUMMA algorithm to multiply matrices on multiple GPUs.
Based on my previous work on GEMM with multiple GPUs, I added SUMMA algorithm this time.
SUMMA can multiply matrices with higher efficiency, so I thought if I run this algorithm with multiple GPUs, the performance can be even better. 
I divide matrix A and B into 8 sub matrices, and distribute these matrices into GPUs,then these matrices can be multiplied by SUMMA algorithm. 
The speed of computation was 30% faster than GEMM when running on the same number and type of GPUs.  
