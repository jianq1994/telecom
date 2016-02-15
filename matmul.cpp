#define PROGRAM_FILE "matmul.cl"
#define KERNEL_FUNC "matmul"
#define MATRIX_SIZE 2

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <CL/cl.h>


void print_mat(float* mat)
{
    for (int i = 0; i < MATRIX_SIZE* MATRIX_SIZE; ++i)
    {
        printf("%.2f\t", mat[i]);
        if ((i+1)%2 == 0)
        {
            printf("\n");
        }
    }
}

int main()
{
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_int err;

    cl_program program;
    FILE *program_handle;
    char *program_buffer;
    size_t program_size;
    cl_kernel kernel;
    size_t global_work_size;
    size_t status;


    int N = MATRIX_SIZE;
    float mat1[N*N], mat2[N*N], result[N*N];
    cl_mem mat1_buff, mat2_buff, res_buff;
    // cl_mem N_buff;

    for (int i = 0; i < N*N; ++i)
    {
        mat1[i] = float(rand());
        mat2[i] = float(rand());
    }

    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU,1,&device,NULL);
    context = clCreateContext(NULL,1,&device,NULL,NULL,&err);

    program_handle = fopen(PROGRAM_FILE, "r");
    fseek(program_handle,0,SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char*)malloc(program_size+1);
    program_buffer[program_size] = '\0';
    status = fread(program_buffer,sizeof(char),program_size,program_handle);
	if(status == 0) printf("fread problem!\n");
//    printf("bits read %d\n", status);
    fclose(program_handle);

    program = clCreateProgramWithSource(context,1,(const char**)&program_buffer, &program_size, &err);
    if (program == NULL)
    {
         printf("Program creation failed\n");
         return 1;
    }   
    free(program_buffer);
    int success = clBuildProgram(program,0,NULL,NULL,NULL,NULL);
    if(success!=CL_SUCCESS) 
    {
         printf("Program Build failed\n");
         return 1;
    }   

    kernel = clCreateKernel(program,KERNEL_FUNC,&err);
//	printf("err:%d invalid:%d", err, CL_INVALID_PROGRAM);
//	printf("kernel created");
    queue = clCreateCommandQueue(context,device,0,&err);
//	printf("queue created");	

    mat1_buff = clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,sizeof(float)*N*N,mat1,&err);
    mat2_buff = clCreateBuffer(context,CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,sizeof(float)*N*N,mat2,&err);
    res_buff = clCreateBuffer(context,CL_MEM_WRITE_ONLY,sizeof(float)*N*N, NULL, &err);
//	printf("buffer created");

    clSetKernelArg(kernel,1,sizeof(cl_mem),&mat1_buff);
    clSetKernelArg(kernel,2,sizeof(cl_mem),&mat2_buff);
    clSetKernelArg(kernel,3,sizeof(cl_mem),&res_buff);
    clSetKernelArg(kernel,0,sizeof(int),&N);

    global_work_size = N*N;
    clEnqueueNDRangeKernel(queue,kernel,1,NULL, &global_work_size,NULL,0,NULL,NULL);

    clEnqueueReadBuffer(queue,res_buff,CL_TRUE,0,sizeof(float)*N*N,result,0,NULL,NULL);

    print_mat(mat1);
    print_mat(mat2);
    print_mat(result);

    clReleaseCommandQueue(queue);
    clReleaseMemObject(mat1_buff);
    clReleaseMemObject(mat2_buff);
    clReleaseMemObject(res_buff);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseContext(context);


    return 0;
}


