#include <stdio.h>
#include <stdlib.h>
#include <iostream> // for standard I/O
#include <fstream>
#include <time.h>
#include "opencv2/opencv.hpp"
#include <sys/types.h>

#include <CL/cl.h>


using namespace cv;
using namespace std;
#define PROGRAM_FILE "filter.cl"
#define KERNEL_FUNC "filter"
#define SHOW
int main(int, char**)
{

    // Preparation for the cl device
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue queue;
    cl_int err;
    cl_program program;
    FILE* program_handle;
    char* program_buffer;
    size_t program_size;
    cl_kernel kernel;
    size_t global_work_size;
    size_t status;
    int status_p;


    clGetPlatformIDs(1, &platform, NULL);
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU,1,&device,NULL);
    context = clCreateContext(NULL,1,&device,NULL,NULL,&err);
    program_handle = fopen(PROGRAM_FILE,"r");
    fseek(program_handle,0,SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char*)malloc(program_size+1);
    program_buffer[program_size] = '\0';
    status = fread(program_buffer,sizeof(char),program_size,program_handle);
    if(status == 0) printf("fread problem!\n");
    fclose(program_handle);

    program = clCreateProgramWithSource(context,1,(const char**)&program_buffer, &program_size,&err);
    if(program == NULL) printf("Program creation failed\n");
    free(program_buffer);
    int success = clBuildProgram(program,0,NULL,NULL,NULL,NULL);
    if(success!=CL_SUCCESS) printf("Program Build failed\n");
    kernel = clCreateKernel(program, KERNEL_FUNC,&err);
    if(err!=CL_SUCCESS) printf("Kernel create failed\n");
    queue = clCreateCommandQueue(context,device,0,&err);
    if(err!=CL_SUCCESS) printf("Command queue create failed\n");
    cl_mem frame_buff, xfilter_buff, res_buff;
    cl_event write_event[1],kernel_event[1];



    // Preparing the data

    VideoCapture camera("./bourne.mp4");
    if(!camera.isOpened())  // check if we succeeded
        return -1;

    const string NAME = "./output.avi";   // Form the new name with container
    int ex = static_cast<int>(CV_FOURCC('M','J','P','G'));
    Size S = Size((int) camera.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) camera.get(CV_CAP_PROP_FRAME_HEIGHT));
	//Size S =Size(1280,720);
	cout << "SIZE:" << S << endl;
    // printf("S.height:%d\n", S.height);
    global_work_size = S.area();
    frame_buff = clCreateBuffer(context, CL_MEM_READ_ONLY,
       sizeof(float)*S.area(), NULL, &status_p);
    if(status_p) printf("Failed to create buffer for result");    
    res_buff = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
       sizeof(float)*S.area(), NULL, &status_p);
    if(status_p) printf("Failed to create buffer for result");
    float output[S.area()];
    float input[S.area()];
    float xedgeFilter[3][3] = {{-1,0,1},{-1,0,1},{-1,0,1}};
    // int yedgeFilter[3][3] = {{-1,-1,-1},{0,0,0},{1,1,1}};
    float GauFilter[3][3] = {{0.95,1.18,0.95},{1.18,1.48,1.18},{0.95,1.18,0.95}};

    xfilter_buff = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,sizeof(float)*9,xedgeFilter,&err);
    // yfilter_buff = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,sizeof(int)*9,yedgeFilter,&err);


	
    VideoWriter outputVideo;                                        // Open the output
        outputVideo.open(NAME, ex, 25, S, true);

    if (!outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write: " << NAME << endl;
        return -1;
    }
	// time_t start,end;
	// double diff,tot = 0;
	int count=0;
	const char *windowName = "filter";   // Name shown in the GUI window.
    #ifdef SHOW
    namedWindow(windowName); // Resizable window, might not work on Windows.
    #endif
    while (true) 
    {
        Mat cameraFrame,displayframe;
		count=count+1;
		if(count > 299) break;
        camera >> cameraFrame;
        // Mat filterframe = Mat(cameraFrame.size(), CV_8UC3);
        Mat grayframe;
    	cvtColor(cameraFrame, grayframe, CV_BGR2GRAY);
		// time (&start);
        Mat newframe = Mat(cameraFrame.size(),CV_32FC1);


        // Sending data for execution
        // printf("S.area: %d\n", S.area());

        for (int i = 0; i < S.area(); ++i)
        {
            input[i] = float(grayframe.data[i]);
        }

        status_p = clEnqueueWriteBuffer(queue, frame_buff, CL_FALSE,
            0, sizeof(float)*S.area(),input, 0, NULL, write_event);
        if(status_p) printf("Failed to write input");


        clSetKernelArg(kernel,0,sizeof(int),&S.width);
        clSetKernelArg(kernel,1,sizeof(int),&S.height);
        clSetKernelArg(kernel,2,sizeof(cl_mem),&frame_buff);
        clSetKernelArg(kernel,3,sizeof(cl_mem),&xfilter_buff);
        clSetKernelArg(kernel,4,sizeof(cl_mem),&res_buff);

        clEnqueueNDRangeKernel(queue,kernel,1,NULL, &global_work_size,NULL,1,&write_event[0],kernel_event);
        clEnqueueReadBuffer(queue,res_buff,CL_TRUE,0,sizeof(int)*S.area(),output,1,kernel_event,NULL);

        for (int i = 0; i < S.area(); ++i)
        {
            input[i] = output[i];
        }

        status_p = clEnqueueWriteBuffer(queue, frame_buff, CL_FALSE,
            0, sizeof(float)*S.area(),input, 0, NULL, write_event);
        if(status_p) printf("Failed to write input");


        clSetKernelArg(kernel,0,sizeof(int),&S.width);
        clSetKernelArg(kernel,1,sizeof(int),&S.height);
        clSetKernelArg(kernel,2,sizeof(cl_mem),&frame_buff);
        clSetKernelArg(kernel,3,sizeof(cl_mem),&Gaufilter_buff);
        clSetKernelArg(kernel,4,sizeof(cl_mem),&res_buff);
        clEnqueueNDRangeKernel(queue,kernel,1,NULL, &global_work_size,NULL,1,&write_event[0],kernel_event);
        clEnqueueReadBuffer(queue,res_buff,CL_TRUE,0,sizeof(int)*S.area(),output,1,kernel_event,NULL);


        memcpy(newframe.data, output, 3*S.area());


  //   	GaussianBlur(grayframe, grayframe, Size(3,3),0,0);
  //   	GaussianBlur(grayframe, grayframe, Size(3,3),0,0);
  //   	GaussianBlur(grayframe, grayframe, Size(3,3),0,0);
		// Scharr(grayframe, edge_x, CV_8U, 0, 1, 1, 0, BORDER_DEFAULT );
		// Scharr(grayframe, edge_y, CV_8U, 1, 0, 1, 0, BORDER_DEFAULT );



		// addWeighted( edge_x, 0.5, edge_y, 0.5, 0, edge );
  //       threshold(edge, edge, 80, 255, THRESH_BINARY_INV);
		// time (&end);
  //       cvtColor(edge, edge_inv, CV_GRAY2BGR);
  //   	// Clear the output image to black, so that the cartoon line drawings will be black (ie: not drawn).
  //   	memset((char*)displayframe.data, 0, displayframe.step * displayframe.rows);


		newframe.convertTo(grayframe,CV_8U);
        cvtColor(grayframe, displayframe, CV_GRAY2BGR);
		outputVideo << displayframe;
	#ifdef SHOW
        imshow(windowName, displayframe);
	#endif
		// diff = difftime (end,start);
		// tot+=diff;
	}
    clReleaseEvent(write_event[0]);
    clReleaseEvent(kernel_event[0]);
    clReleaseCommandQueue(queue);
    clReleaseMemObject(frame_buff);
    clReleaseMemObject(res_buff);
    clReleaseMemObject(xfilter_buff);
    clReleaseProgram(program);
    clReleaseContext(context);
	outputVideo.release();
	camera.release();
  	// printf ("FPS %.2lf .\n", 299.0/tot );

    return EXIT_SUCCESS;

}
