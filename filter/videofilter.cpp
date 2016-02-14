#include <stdio.h>
#include <stdlib.h>
#include <iostream> // for standard I/O
#include <fstream>
#include <time.h>
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
#define SHOW
void print_matrix(unsigned N, float * mat) {
    for(unsigned k = 0; k < N*N; ++k) {
        printf("%2f ", *(mat+k));
        if ((k+1)%N == 0) {
            printf("\n");
        }
    }
}

int main(int, char**)
{
    VideoCapture camera("./bourne.mp4");
    if(!camera.isOpened())  // check if we succeeded
        return -1;

    const string NAME = "./output_gpu.avi";   // Form the new name with container
    int ex = static_cast<int>(CV_FOURCC('M','J','P','G'));
    Size S = Size((int) camera.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) camera.get(CV_CAP_PROP_FRAME_HEIGHT));
	//Size S =Size(1280,720);
	
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
	
	//unsigned char* inputarray = cameraFrame.data;
	//printf("%s", *inputarray);
	int width = S.width;
	int height = S.height;	
	printf("size of the image is %d, %d\n", width, height);

    while (true) {
        Mat cameraFrame,displayframe;
		count=count+1;
		if(count > 299) break;
        camera >> cameraFrame;
		time (&start);
        Mat filterframe = Mat(cameraFrame.size(), CV_8UC3);
        Mat grayframe,edge_x,edge_y,edge;
		// create a mat with float number
		Mat floatframe = Mat(cameraFrame.size(), CV_32FC1);

//		unsigned char* inputarray = cameraFrame.data;
//    	printf("%d", *inputarray); 		
//		memcpy(cameraFrame.data, inputarray, 3*width*height );

		// cvtColor : turn cameraFrame to grayframe, defined by CV_BGR2GRAY
	   	cvtColor(cameraFrame, grayframe, CV_BGR2GRAY);
		
		// get the grayframe size by int
		int h = grayframe.size.p[0];
		int w = grayframe.size.p[1];		
		// create a matrix
		float *image = (float *) malloc(sizeof(float)*h*w);		
		for (int i=0; i < h*w; ++i) {
			*(image + i) = (float) grayframe.data[i];			
		}
		printf("the first 5 element is \n");
		for (int i =0; i< 5; ++i) {
			printf("%f  ",image[i]);	
		}

		// convert grayframe to floatframe
		grayframe.convertTo(floatframe, CV_32FC1);
		//printf("%d, %d \n", w, h );
		//printf("%f \n", *((float *)floatframe.data) );
		//cout << floatframe.data << endl;
		
//		Mat g_kernel = floatframe.getGaussianKernel(3, 3, 50, 50);
//		cout << g_kernel << endl;
		
		int k_row = 3;
        int k_col = 3;
        float g_kernel[9] = {1,2,1, 2, 4, 2, 1,2,1};
        printf("the kernel is :\n");
        print_matrix(3, g_kernel);		
		
		// blur the image with various low pass filters (LPF) or customer defined images
		// high pass filters helps in findign edges in the images
		// gaussian blur specifies the (input, output, size, deviation_x, deviation_y) 
    	GaussianBlur(grayframe, grayframe, Size(3,3),0,0);
    	GaussianBlur(grayframe, grayframe, Size(3,3),0,0);
    	GaussianBlur(grayframe, grayframe, Size(3,3),0,0);
		// edge detection (inputarray, outputarray, deepth, dx,dy, scale, delta, )
		Scharr(grayframe, edge_x, CV_8U, 0, 1, 1, 0, BORDER_DEFAULT );
		Scharr(grayframe, edge_y, CV_8U, 1, 0, 1, 0, BORDER_DEFAULT );
		addWeighted( edge_x, 0.5, edge_y, 0.5, 0, edge );
		time (&end);
        cvtColor(edge, displayframe, CV_GRAY2BGR);
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
