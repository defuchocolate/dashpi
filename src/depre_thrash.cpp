/**
thrash.cpp
Author: lyinch
Version: 0.0.4
www.lyinch.com
www.github.com/lyinch/dashpi
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

const string VERSION = "v.0.0.4";
const bool DEBUGGING = true;
const short int WINDOW_WIDTH = 800;
const short int WINDOW_HEIGHT = 800;
const short int LINE_THICKNESS = 2;
const short int THRESHOLD_R = 100;
const short int THRESHOLD_G = 80;
const short int THRESHOLD_B = 80;


class stack
{

  private:
    int arr[10];   // Contains all the Data
    int top;        //Contains location of Topmost Data pushed onto Stack

  public:
   stack()         //Constructor
   {
      top=-1;      //Sets the Top Location to -1 indicating an empty stack
   }

   void push(int a)  // Push ie. Add Value Function
   {
    top++;        // increment to by 1
    if(top<10)
     {
      arr[top]=a;  //If Stack is Vacant store Value in Array
     }
     else
     {
      cout<<"STACK FULL!!"<<endl;
      top--;
     }
   }

  int pop()                  // Delete Item. Returns the deleted item
  {
    if(top==-1)
    {
      cout<<"STACK IS EMPTY!!!"<<endl;
      return false;
    }
    else
    {
      int data=arr[top];     //Set Topmost Value in data
      arr[top]=0;       //Set Original Location to NULL
      top--;               // Decrement top by 1
      return data;         // Return deleted item
    }
   }
};



struct TPOI //points of interest
{
  int x;
  int y;
};

struct TAOI{
int x;
int y;
bool not_checked;
};

TAOI AOI[50][921600];


/**
   Writes a message to the log, with a timestamp and a prefix
   Outputs the same message into the cout stream without timestamps
   @version 0.3
   @param input_str input message
   @param m_type message type
   @return void
   ToDo:
   * Create library for this function
 */
void writeLog(string input_str, short int m_type){
  time_t now = time(0);
  struct tm* tm = localtime(&now);
  ofstream out("thrash.log", ios::out | ios::app);
  if(!out){ 
    cout << "You do not have the permission to use this application!" << endl; //Check if program is used with sudo rights
    exit(0);
  }
  string prefix;
  switch(m_type){
  case 0: prefix = "Error";break;
  case 1: prefix = "Warning"; break;
  case 2: prefix = "Info"; break;
  default: prefix = "Undefined"; break;
  }
  //precede 0, if output < 9
  out << (tm->tm_year + 1900) << '/';
  tm->tm_mon+1 > 9 ? out << tm->tm_mon+1 : out << '0' << tm->tm_mon+1;
  out << '/';
  tm->tm_mday > 9 ? out << tm->tm_mday : out << '0' << tm->tm_mday;
  out << ' ';
  tm->tm_hour > 9 ? out << tm->tm_hour : out << '0' << tm->tm_hour;
  out << ':';
  tm->tm_min > 9 ? out << tm->tm_min : out << '0' << tm->tm_min;
  out << ':';
  tm->tm_sec > 9 ? out << tm->tm_sec : out << '0' << tm->tm_sec;
  out << ":" << "[" << prefix << "] ";

  out << input_str << "\n";
  out.close();
  cout << "Log Message: [" << prefix << "] " <<input_str << endl;
}

/**
   Sorts the coordinates from the POI list starting from the lowest for either x or y coordinates 
   @version 0.3
   @param array struct TPOI as type for the POIs
   @param length intervall to sort
   @param coordinate to sort x or y
   @return void
   ToDo
   Improve loop
 */
void sort_coord(TPOI * array,int length, char coord){
  short int tmp_a, tmp_b;
  if(coord == 'x'){
    for(int i = 0; i < length; ++i){
      for(int j = 0; j < length -i; ++j){
	if(array[j].x > array[j + 1].x){
	  tmp_a = array[j].x;
	  tmp_b = array[j].y;
	  array[j].y = array[j+1].y;
	  array[j+1].y = tmp_b;
	  array[j].x = array[j+1].x;
	  array[j+1].x = tmp_a;
	}
      }
    }
  }else{
      for(int i = 0; i < length; ++i){
      for(int j = 0; j < length -i; ++j){
	if(array[j].y > array[j + 1].y){
	  tmp_a = array[j].y;
	  tmp_b = array[j].x;
	  array[j].x = array[j+1].x;
	  array[j+1].x = tmp_b;
	  array[j].y = array[j+1].y;
	  array[j+1].y = tmp_a;
	}
      }
    }
  }
}

/**
   Initialize the programm, write logs and load necessery informations
   @param none
   @return void
   ToDo:
   Add version Number
*/
void init(){
  char buffer[100];
  sprintf(buffer,"Application started");
  writeLog(buffer,2);
  if(DEBUGGING){
    writeLog("Debugging enabled",2);
  }else{
    writeLog("Debugging disabled",2);
  }
}

/**
   Outputs the number of POI and saves the POIs in the given Array
   Note, that rows = y and cols = x. Axis are changed in the image container
   @param input MAT object
   @param output MAT object
   @param array to modify
   ToDo:
   use file information in the loop
   change variable names of the loop
 */
int getPOI(Mat image_in, Mat image_out,TPOI * poi){
  int POI_number = 0;
    cout << "getPOI called "<< endl;
   for(int i = 0; i <= 1279; i++){
     for(int j = 0; j <= 719; j++){
       //if((image_in.at<Vec3b>(j,i)[2] < THRESHOLD_R) || (((image_in.at<Vec3b>(j,i)[1])+(image_in.at<Vec3b>(j,i)[0])) >= 150)){
      if(((image_in.at<Vec3b>(j,i)[2]) >= (18+image_in.at<Vec3b>(j,i)[1])) && ((image_in.at<Vec3b>(j,i)[2]) >= (18+image_in.at<Vec3b>(j,i)[0]))){
         poi[POI_number].x = i;
         poi[POI_number].y = j; 
         POI_number++;
       }else {

         image_out.at<Vec3b>(j,i)[0] = 255;
         image_out.at<Vec3b>(j,i)[1] = 255;
         image_out.at<Vec3b>(j,i)[2] = 255; 
        }
      }
    }   
      cout << POI_number << " : getPOI end" << endl;
      return POI_number;
   }



int main( int argc, char** argv )
{
  char buffer[100]; //only used to build messages for the log
  init();

  Mat input;
  input = imread("/home/lyinch/dashpi/cpp/input/img05.jpg", CV_LOAD_IMAGE_COLOR); //ToDo: use arguments to load image

  if(!input.data)                              
    {
      writeLog("Could not open or find the image",0);
      cout <<  "Could not open or find the image" << endl ;
      return -1;
    }

  writeLog("Image loaded successfully",2);
  cout << "Press any key to exit!" << endl;

  //MAIN PROGRAMM START, IF FILE FOUND

  sprintf(buffer,"Arguments: %d",argc);
  writeLog(buffer,2);
  string argstr;
  for(int i = 0; i < argc; i++){
    sprintf(buffer,"argv[%d] = %s",i,argv[i]);
    writeLog(buffer,2);
    argstr = argv[i];
    if((argstr == "-d")){
      cout << "-d found" << endl;
    }
  }
  
  TPOI POI[921600] = {-1}; //1280x720 possible pixels
  int POI_found = 0;
  Point ul,dl,ur,dr; //up left, down left, up right, down right -> points of the highlight box
  namedWindow( "Input", WINDOW_NORMAL);
  namedWindow( "Output", WINDOW_NORMAL);
  resizeWindow("Output", WINDOW_WIDTH,WINDOW_HEIGHT);
  resizeWindow("Input", WINDOW_WIDTH,WINDOW_HEIGHT);
  moveWindow("Output",WINDOW_WIDTH+5,0);
  Mat output = input.clone();
  
  if(DEBUGGING){
  sprintf(buffer, "Image size: %d x %d ",input.size().width, input.size().height);
  writeLog(buffer,2);
  sprintf(buffer, "Image columns: %d Image rows: %d Image channels: %d Image type: ", input.cols, input.rows, input.channels());
  writeLog(buffer,2);
  }
  
  POI_found = getPOI(input,output,POI);
  
   //TODO change debugging
  if(DEBUGGING){
  sprintf(buffer,"POI found: %d", POI_found);
  writeLog(buffer,2);
  }
  /************************************************************************/
  //copy elements to AOI[0][n].x/.y
  //Floodcheck AOI[0][1].x/.y and save to AOI[1][n].x/.y
  //sort AOI[1][n].x/.y
  //highlight AOI[1][n].x/.y
  int tmp_aoi = 0;
  for(int n=0; n < POI_found; n++){
  AOI[0][n].x = POI[n].x;
  AOI[0][n].y = POI[n].y;
  AOI[0][n].not_checked = true; 
  }

  /***************************************************/





  /************************************************************************/
  //Highlight the sign
  //TODO function  
  sort_coord(POI,POI_found-1,'x');
  cout << "sorting ended" << endl;
  ul.x = POI[POI_found-1].x;
  ur.x = POI[0].x; 
  dl.x = POI[POI_found-1].x;
  dr.x = POI[0].x;
  sort_coord(POI,POI_found-1,'y');
  dr.y = POI[0].y;
  dl.y = POI[0].y;
  ur.y = POI[POI_found-1].y;
  ul.y = POI[POI_found-1].y;
  line(output,ul,ur,Scalar(255,0,0),LINE_THICKNESS,CV_AA);
  line(output,ul,dl,Scalar(255,0,0),LINE_THICKNESS,CV_AA);
  line(output,ur,dr,Scalar(255,0,0),LINE_THICKNESS,CV_AA);
  line(output,dr,dl,Scalar(255,0,0),LINE_THICKNESS,CV_AA);
  line(input,ul,ur,Scalar(255,0,0),LINE_THICKNESS,CV_AA);
  line(input,ul,dl,Scalar(255,0,0),LINE_THICKNESS,CV_AA);
  line(input,ur,dr,Scalar(255,0,0),LINE_THICKNESS,CV_AA);
  line(input,dr,dl,Scalar(255,0,0),LINE_THICKNESS,CV_AA);
  imshow("Output",output);
  imshow( "Input",input );
  waitKey(0);
  writeLog("Application closed",2);
  return 0;
}
