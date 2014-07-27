#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

String version = "0.1";

void WriteLog(const char*input_str,int m_type)
{
  time_t now = time(0);
  struct tm* tm = localtime(&now);
  ofstream out("camlog.txt",ios::out | ios::app);
  if(!out){
    cout << "You do not have the permission to use this application!" << endl;
    exit(0);
  }else{
    String prefix;
    switch(m_type){
    case 0 : prefix = "Error"; break;
    case 1: prefix = "Warning"; break;
    case 2: prefix = "Info"; break; 
    default : prefix = "undefined"; break;}
  out << (tm->tm_year + 1900) << '/' << (tm->tm_mon + 1) << '/' << tm->tm_mday << ' ' << tm->tm_hour << ':' << tm->tm_min << ':' << tm-> tm_sec << ": " << "[" << prefix << "] ";
  out << input_str << "\n";
  out.close();
  }
 }

int main (int argc, char* argv[]){
  WriteLog("Application starded successfully version: ",2);

  //rite("-------------------------------------------------");
  cout << "Application started successfully" << endl;
  cout << "---------------------------------------" << endl;
  /*
  // String window_name = "DashPi Playback";
  VideoCapture capture("vid.MP4");
  if(!capture.isOpened()){
    cout << "Error,video file is not opened" << endl;
    return 0;
  }
  double fps = capture.get(CV_CAP_PROP_FPS);
  cout << "fps: " << fps << endl;
  namedWindow("Fucking Video", CV_WINDOW_AUTOSIZE);
  while(true){
    Mat frame;
    bool bSuccess = capture.readframe(frame);
    
    if(!bSuccess){
      cout << "Error can't read the frame from the file" << endl;
      break;
    }

    Imshow("video", frame);
    if(waitKey(30) == 27){
      cout << "esc" << endl;
      break;
    }
  }
*/
  //Streaming 
   VideoCapture cap("http://10.5.5.9:8080/live/amba.m3u8");
   
  //VideoCapture cap("svid.mp4");
  if(!cap.isOpened()){
    WriteLog("Could not open capture!",0);
    WriteLog("Application closed!",0);
    cout << "----------------------------------------" << endl;
    cout << "Could not open capture!" << endl;
    cout << "----------------------------------------" << endl;                                         
    return 0;
  }
  Mat frame;
  namedWindow("CameraStream", 1);
  

  while(true){
    cap.grab();
    cap.retrieve(frame);
    imshow("CameraStream",frame);
    //    WriteLog("Video should be showing?!",1);
 }
  
  destroyAllWindows();
  return 0;

}
