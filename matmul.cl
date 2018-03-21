__kernel void matmul(__global const float* mat1,
                    __global const float* mat2, 
                    __global float* restrict result, 
                    __global const int N)
{   
    int index = get_global_id(0);
    int i = index/N;
    int j = index%N;
    result[index] = 0;

    for(int k=0;k<N;k++)
    {
        result[index] += mat1[i*N+k] * mat2[k*N+j];
    }
}