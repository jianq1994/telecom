__kernel void filter(const int width, const int height, __global const unsigned int* frame, __global const unsigned int* filter, __global unsigned int* restrict result)
{
    int index = get_global_id(0);
    int i = index / width;
    int j = index % width;

    result[index] = 0;

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