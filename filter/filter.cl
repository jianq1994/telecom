__kernel void filter(const int width, const int height, __global const unsigned float* frame, __global const float* filter, __global float* restrict result)
{
    int index = get_global_id(0);
    // int i = index / width;
    // int j = index % width;

    // result[index] = 0;

    int col = index % width;
    int row = index / width;
    int pivol = col + row*width;

    result[pivol] = 0;
    for(int i = 0; i < 3; ++i) 
    {
        for (int j =0; j < 3; ++j) 
        {
            int k = pivol + (i - 1) * width + (j -1);
            int k_row = (i-1) + row;
            int k_col = (j-1) + col; 
            if ( k_row >= 0 && k_row < height && k_col >= 0 && k_col < width)
            {
                result[pivol] += frame[k] * filter[3*i+j];
            }
            else
            {
                result[pivol] += 0;
            }
        }
    }

    // if(i == 0)
    // {
    //     if(j == 0)
    //     {
    //         result[index] += filter[1][1] * frame[index];
    //         result[index] += filter[1][2] * frame[index+1];
    //         result[index] += filter[2][1] * frame[index + width];
    //         result[index] += filter[2][2] * frame[index + width + 1];
    //         return 0;
    //     }
    //     elif(j == width - 1)
    //     {
    //         result[index] += filter[1][1] * frame[index];
    //         result[index] += filter[1][0] * frame[index-1];
    //         result[index] += filter[2][0] * frame[index + width - 1];
    //         result[index] += filter[2][1] * frame[index + width];
    //         return 0;
    //     }
    //     else
    //     {
    //         result[index] += filter[1][1] * frame[index];
    //         result[index] += filter[1][2] * frame[index+1];
    //         result[index] += filter[2][1] * frame[index + width];
    //         result[index] += filter[2][2] * frame[index + width + 1];
    //         result[index] += filter[1][0] * frame[index-1];
    //         result[index] += filter[2][0] * frame[index + width - 1]; 
    //         return 0;                       
    //     }
    // }
    // elif(i == height -1)
    // {
    //     if(j == 0)
    //     {
    //         result[index] += filter[1][1] * frame[index];
    //         result[index] += filter[1][2] * frame[index+1];
    //         result[index] += filter[0][1] * frame[index - width];
    //         result[index] += filter[0][2] * frame[index - width + 1];
    //         return 0;
    //     }
    //     elif(j == width - 1)
    //     {
    //         result[index] += filter[1][1] * frame[index];
    //         result[index] += filter[1][0] * frame[index-1];
    //         result[index] += filter[0][0] * frame[index - width - 1];
    //         result[index] += filter[0][1] * frame[index - width];
    //         return 0;
    //     }
    //     else
    //     {
    //         result[index] += filter[1][1] * frame[index];
    //         result[index] += filter[1][2] * frame[index+1];
    //         result[index] += filter[0][1] * frame[index - width];
    //         result[index] += filter[0][2] * frame[index - width + 1];
    //         result[index] += filter[1][0] * frame[index-1];
    //         result[index] += filter[0][0] * frame[index - width - 1];  
    //         return 0;                      
    //     }        
    // }
    // elif(j == 0)
    // {
    //     result[index] += filter[0][1] * frame[index - width];
    //     result[index] += filter[1][1] * frame[index];
    //     result[index] += filter[2][1] * frame[index +width];
    //     result[index] += filter[0][2] * frame[index - width + 1];
    //     result[index] += filter[1][2] * frame[index+1];
    //     result[index] += filter[2][2] * frame[index + width + 1];  
    //     return 0;         
    // }
    // elif(j == width - 1)
    // {
    //     result[index] += filter[0][1] * frame[index - width];
    //     result[index] += filter[1][1] * frame[index];
    //     result[index] += filter[2][1] * frame[index +width];
    //     result[index] += filter[0][0] * frame[index-width-1];
    //     result[index] += filter[1][0] * frame[index-1];
    //     result[index] += filter[2][0] * frame[index+width-1];  
    //     return 0;          
    // }
    // else
    // {
    //     result[index] += filter[0][2] * frame[index - width + 1];
    //     result[index] += filter[1][2] * frame[index + 1];
    //     result[index] += filter[2][2] * frame[index +width + 1];        
    //     result[index] += filter[0][1] * frame[index - width];
    //     result[index] += filter[1][1] * frame[index];
    //     result[index] += filter[2][1] * frame[index +width];
    //     result[index] += filter[0][0] * frame[index-width-1];
    //     result[index] += filter[1][0] * frame[index-1];
    //     result[index] += filter[2][0] * frame[index+width-1];  
    //     return 0;                  
    // }
}