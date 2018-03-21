__kernel void matmul(__global const float* mat1, __global const float* mat2, __global float* restrict result, __global const int* pN)
{   
    int N = *pN;
    int gpid = get_group_id(0);
    int llid = get_local_id(0);
    int gsize = get_group_size(0);
    int iNj = gpid*gsize+llid;
    int i = iNj/N;
    int j = iNj%N;
    result[iNj] = 0;

    for(k=0;k<N;k++)
    {
        result[iNj] += mat1[i*N+k] * mat2[k*N+j];
    }
}