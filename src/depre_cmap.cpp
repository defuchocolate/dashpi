//DEPRECATED DO NOT USE THIS FILE
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>

#include "../lib/writeLog.h"

using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
  writeLog("Application started",2);
	for(q = 0; q < 16; q++){
	cout << "DEPRECATED DO NOT USE THIS FILE" << endl;
}
  Mat input;
  Mat output;
  String filename;
  if(argc > 1){
    filename = argv[1];
  }
  char buffer[100];
  sprintf(buffer,"Arguments: %d",argc);
  writeLog(buffer,2);
  for(int i = 0; i < argc; i++){
    sprintf(buffer,"argv[%d] = %s",i,argv[i]);
    writeLog(buffer,2);
  }
  input = imread("../input/" + filename, CV_LOAD_IMAGE_COLOR); 
  cout << input << endl;
  if(!input.data)                              
    {
      writeLog("Could not open or find the image",0);
      cout <<  "Could not open or find the image" << std::endl ;
      return -1;
    }
  cout << "Press any key to exit!" << endl;

  namedWindow( "Input", WINDOW_NORMAL);
  resizeWindow("Input", 400,400);
  imshow( "Input",input );                  
  sprintf(buffer, "Image size: %d x %d ",input.size().width, input.size().height);
  writeLog(buffer,2);
  sprintf(buffer, "Image columns: %d Image rows: %d Image channels: %d", input.cols, input.rows, input.channels());
  writeLog(buffer,2);
 
  
  namedWindow("Output",WINDOW_NORMAL);
  resizeWindow("Output",400,400);
  moveWindow("Output",405,0);
  Mat M(144,256,CV_8UC3,Scalar(255,0,0));
  int color[3] = {0,0,0};
    cout << M.cols << " " << M.rows << " " << M.channels() << endl;
   for(int i = 0; i <= 1274; i+=5){
     for(int j = 0; j <= 714; j+=5){
       for(int x = i; x <= i+4; x++){
	 for(int y = j; y <= j+4; y++){
	   color[0] += input.at<Vec3b>(y,x)[0];
	   color[1] += input.at<Vec3b>(y,x)[1];
	   color[2] += input.at<Vec3b>(y,x)[2];
	 }
       }
       cout << color[0]/25 << endl;
       M.at<Vec3b>(j/5,i/5)[0] = color[0]/25;
       M.at<Vec3b>(ceil(j/5),ceil(i/5))[1] = color[1]/25;
       M.at<Vec3b>(j/5,i/5)[2] = color[2]/25;
       color[0] = 0;
       color[1] = 0;
       color[2] = 0;
     }
    }


  imshow("Output",M);
  cout << M.at<Vec3b>(0,0) << endl;
  waitKey(0);
  writeLog("Application closed",2);
  return 0;
}
