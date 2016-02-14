#include <stdio.h>
#include <stdlib.h>
#include <iostream> // for standard I/O
#include <fstream>
#include <time.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
#define SHOW
int main(int, char**)
{

    // Preparation for the cl device
    // cl_platform_id platform;
    // cl_device_id device;
    // cl_context context;
    // cl_command_queue queue;
    // cl_int err;
    // cl_program program;
    // FILE* program_handle;
    // char* program_buffer;
    // size_t program_size;
    // cl_kernel kernel;
    // size_t global_work_size;
    // size_t status;


    // clGetPlatformIDs(1, &platform, NULL);
    // clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU,1,&device,NULL);
    // context = clCreateContext(NULL,1,&device,NULL,NULL,&err);
    // program_handle = fopen(PROGRAM_FILE,"r");
    // fseek(program_handle,0,SEEK_END);
    // program_size = ftell(program_handle);
    // rewind(program_handle);
    // program_buffer = (char*)malloc(program_size+1);
    // program_buffer[program_size] = '\0';
    // status = fread(program_buffer,sizeof(char),program_size,program_handle);
    // if(status == 0) printf("fread problem!\n");
    // fclose(program_handle);

    // program = clCreateProgramWithSource(context,1,(const char**)&program_buffer, &program_size,&err);
    // if(program == NULL) printf("Program creation failed\n");
    // free(program_buffer);
    // int success = clBuildProgram(program,0,NULL,NULL,NULL,NULL);
    // if(success!=CL_SUCCESS) printf("Program Build failed\n");
    // kernel = clCreateKernel(program, KERNEL_FUC,&err);
    // if(err!=CL_SUCCESS) printf("Kernel create failed\n");
    // queue = clCreateCommandQueue(context,device,0,&err);
    // if(err!=CL_SUCCESS) printf("Command queue create failed\n");
    // cl_mem frame_buff, filter_buff, res_buff;



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
    printf("S.height:%d\n", S.height);
	
    VideoWriter outputVideo;                                        // Open the output
        outputVideo.open(NAME, ex, 25, S, true);

    if (!outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write: " << NAME << endl;
        return -1;
    }
	time_t start,end;
	double diff,tot;
	int count=0;
	const char *windowName = "filter";   // Name shown in the GUI window.
    #ifdef SHOW
    namedWindow(windowName); // Resizable window, might not work on Windows.
    #endif
    while (true) {
        Mat cameraFrame,displayframe;
		count=count+1;
		if(count > 299) break;
        camera >> cameraFrame;
        Mat filterframe = Mat(cameraFrame.size(), CV_8UC3);
        Mat grayframe,edge_x,edge_y,edge,edge_inv;
    	cvtColor(cameraFrame, grayframe, CV_BGR2GRAY);
		time (&start);




        //Sending data for execution
//        frame_buff = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR,sizeof(unsigned int)*,mat1,&err)




  //   	GaussianBlur(grayframe, grayframe, Size(3,3),0,0);
  //   	GaussianBlur(grayframe, grayframe, Size(3,3),0,0);
  //   	GaussianBlur(grayframe, grayframe, Size(3,3),0,0);
		// Scharr(grayframe, edge_x, CV_8U, 0, 1, 1, 0, BORDER_DEFAULT );
		// Scharr(grayframe, edge_y, CV_8U, 1, 0, 1, 0, BORDER_DEFAULT );



		addWeighted( edge_x, 0.5, edge_y, 0.5, 0, edge );
        threshold(edge, edge, 80, 255, THRESH_BINARY_INV);
		time (&end);
        cvtColor(edge, edge_inv, CV_GRAY2BGR);
    	// Clear the output image to black, so that the cartoon line drawings will be black (ie: not drawn).
    	memset((char*)displayframe.data, 0, displayframe.step * displayframe.rows);
		grayframe.copyTo(displayframe,edge);
        cvtColor(displayframe, displayframe, CV_GRAY2BGR);
		outputVideo << displayframe;
	#ifdef SHOW
        imshow(windowName, displayframe);
	#endif
		diff = difftime (end,start);
		tot+=diff;
	}
	outputVideo.release();
	camera.release();
  	printf ("FPS %.2lf .\n", 299.0/tot );

    return EXIT_SUCCESS;

}
