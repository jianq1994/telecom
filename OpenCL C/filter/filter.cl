__kernel void filter(const int width, const int height, __global const float* frame, __global const float* filter, __global float* restrict result)
{
    int index = get_global_id(0);
    int i = index / width;
    int j = index % width;

    result[index] = 0;

    for (int k = -1; k < 2; ++k)
    {
        for (int l = -1; l < 2; ++l)
        {
            if(i+k<0 || i+k>width-1 || j+k<0 || j+k>height-1) break;

            result[index] += filter[(k+1)*3+l+1]*frame[index+k+l*width];
        }
    }
}