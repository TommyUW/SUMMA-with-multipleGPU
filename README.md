# SUMMA-with-multipleGPU
This program is designed to operate SUMMA algorithm to multiply matrices on multiple GPUs.
Based on my previous work on matrix-matrix multiplication with multiple GPUs, I added SUMMA algorithm this time.
SUMMA can multiply matrices with higher efficiency, so I thought if I run this algorithm with multiple GPUs, the performance can be even better. 
I divide matrix A and B into sub matrices, and distribute these matrices into GPUs,then these matrices can be multiplied by SUMMA algorithm. 
