/**
DASHPI
Author: lyinch
Version: 0.0.2
www.lyinch.com
www.github.com/lyinch/dashpi
*/
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

//libraries
#include "../lib/lib_log.h"

//constants
const std::string VERSION = "v.0.0.2";
const short int image_size_x = 1280;
const short int image_size_y = 720;

//global values
bool DEBUGGING = false;


//structs
struct Tpoi
{
	int area_code;
};
Tpoi poi[1280][720] = {0};

struct Tcheck
{
	bool checked;
};
Tcheck check[1280][720] = {false};

void init(){
	char buffer[100];
	sprintf(buffer,"Application started");
	writeLog(buffer,2);
	if(DEBUGGING){
		writeLog("Debugging enabled",1);
	}else{
		writeLog("Debugging disabled",2);
	}
	//sprintf(buffer,"DashPi %s loaded",VERSION);
	//connect to camera and wifi
	sprintf(buffer,"Dashpi loaded successfully!");
	writeLog(buffer,2);
}

//functions
bool isInBound(short int x, short int y){
	if(x < 0 || x > image_size_x-1)
	{
		return false;
	}
	else if (y < 0 || y > image_size_y-1)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int main(int argc, char** argv)
{
	char buffer[100]; 
	std::string img_path;

//TODO: include in init function
	sprintf(buffer,"Arguments: %d",argc);
	writeLog(buffer,2);
	std::string argstr;
	for(int i = 0; i < argc; i++)
	{
		sprintf(buffer,"argv[%d] = %s",i,argv[i]);
		writeLog(buffer,2);
		argstr = argv[i];
		if(argstr == "-d")
		{
			DEBUGGING = true;
		}
		else if (argstr == "-i")
		{
			img_path = argv[i+1];
		}
	}

	init();

	cv::Mat input;
	input = cv::imread(img_path, CV_LOAD_IMAGE_COLOR);
	if(!input.data)                              
	{
		writeLog("Could not open or find the image",0);
		writeLog("Application closed!",0);
		return -1;
	}

	if(DEBUGGING)
	{
		//sprintf(buffer, "Image size: %d x %d ",input.size().width, input.size().height);
		//writeLog(buffer,2);
		sprintf(buffer, "Image columns: %d Image rows: %d Image channels: %d ", input.cols, input.rows, input.channels());
		writeLog(buffer,2);
	}

	if((input.cols < image_size_x) || (input.rows < image_size_y))
	{
		sprintf(buffer,"Image resolution smaller than %dx%d!",image_size_x,image_size_y);
		writeLog(buffer,0);
		writeLog("Application closed!",0);
		return -1;

	}
	else if ((input.cols > image_size_x) || (input.rows > image_size_y))
	{
		sprintf(buffer,"Image resolution bigger than %dx%d!",image_size_x,image_size_y);
		writeLog(buffer,1);
		writeLog("Analysis may be wrong!",1);
	}
	else
	{
		writeLog("Image loaded successfully",2);
	}
	writeLog("Press ESC or C to exit",2);

	cv::namedWindow( "Input", CV_WINDOW_NORMAL);
	cv::namedWindow( "Output", CV_WINDOW_NORMAL);

	cv::Mat output = input.clone();

//TODO: write as class
	int x,y;
	int total_area_code = 0;
	int count_biggest_area = 0;
	int biggest_area = 0;
	int count_area_code = 0;
	bool colour_found = false;
	std::cout << check[1][1].checked << std::endl;
	for (int i = 0; i <= image_size_x-1; i++)
	{
		for (int j = 0; j <= image_size_y-1; j++)
		{	
			if((isInBound(i,j)) && (check[i][j].checked == false) && ((input.at<cv::Vec3b>(j,i)[2]) >= (18+input.at<cv::Vec3b>(j,i)[1])) && ((input.at<cv::Vec3b>(j,i)[2]) >= (18+input.at<cv::Vec3b>(j,i)[0])) )
			{
				x = i;
				y = j;
				if(count_biggest_area < count_area_code)
				{
					count_biggest_area = count_area_code;
					biggest_area = total_area_code;
				}
				total_area_code++;
				count_area_code = 0;
				check[x][y].checked = true;
				poi[x][y].area_code = total_area_code;
				colour_found = true;
				while(colour_found){
					colour_found = false;
					//check the 4 nearest pixel
					for (int check_pos = 1; check_pos < 150; check_pos++)
					{
						if((isInBound(x,y+check_pos)) && (check[x][y+check_pos].checked == false) &&((input.at<cv::Vec3b>(y+check_pos,x)[2]) >= (18+input.at<cv::Vec3b>(y+check_pos,x)[1])) && ((input.at<cv::Vec3b>(y+check_pos,x)[2]) >= (18+input.at<cv::Vec3b>(y+check_pos,x)[0])) )
						{
							colour_found = true;
							check[x][y+check_pos].checked = true;
							poi[x][y+check_pos].area_code = total_area_code;
							count_area_code++;
						}
						if((isInBound(x,y-check_pos)) && (check[x][y-check_pos].checked == false) &&((input.at<cv::Vec3b>(y-check_pos,x)[2]) >= (18+input.at<cv::Vec3b>(y-check_pos,x)[1])) && ((input.at<cv::Vec3b>(y-check_pos,x)[2]) >= (18+input.at<cv::Vec3b>(y-check_pos,x)[0])) )
						{
							colour_found = true;
							check[x][y-check_pos].checked = true;
							poi[x][y-check_pos].area_code = total_area_code;
							count_area_code++;
						}
						if((isInBound(x+check_pos,y)) && (check[x+check_pos][y].checked == false) &&((input.at<cv::Vec3b>(y,x+check_pos)[2]) >= (18+input.at<cv::Vec3b>(y,x+check_pos)[1])) && ((input.at<cv::Vec3b>(y,x+check_pos)[2]) >= (18+input.at<cv::Vec3b>(y,x+check_pos)[0])))
						{
							colour_found = true;
							check[x+check_pos][y].checked = true;
							poi[x+check_pos][y].area_code = total_area_code;
							count_area_code++;
						}
						if((isInBound(x-check_pos,y)) && (check[x-check_pos][y].checked == false) &&((input.at<cv::Vec3b>(y,x-check_pos)[2]) >= (18+input.at<cv::Vec3b>(y,x-check_pos)[1])) && ((input.at<cv::Vec3b>(y,x-check_pos)[2]) >= (18+input.at<cv::Vec3b>(y,x-check_pos)[0])) )
						{
							colour_found = true;
							check[x-check_pos][y].checked = true;
							poi[x-check_pos][y].area_code = total_area_code;
							count_area_code++;
						}		
					}
					x++;
					y++;
				}
			}
		}
	}


	if(DEBUGGING){
		sprintf(buffer,"#Max entries: %d ",count_biggest_area);
		writeLog(buffer,2);
		sprintf(buffer,"Pos of biggest area: %d",biggest_area);
		writeLog(buffer,2);
		sprintf(buffer,"Areas in total: %d",total_area_code);
		writeLog(buffer,2);
		sprintf(buffer,"Highlighting POIs in \033[1;43mYELLOW\033[0m and the biggest AOI in \033[1;45mPINK\033[0m.");
		writeLog(buffer,2);
		for(int i = 0; i <= image_size_x-1; i++)
		{
			for(int j = 0; j <= image_size_y-1; j++)
			{
				if(poi[i][j].area_code == biggest_area){ //PINK -> biggest AOI
					output.at<cv::Vec3b>(j,i)[0] = 255;
					output.at<cv::Vec3b>(j,i)[1] = 0;
					output.at<cv::Vec3b>(j,i)[2] = 255;
				}else if (poi[i][j].area_code > 0) //YELLOW -> POIs
				{
					output.at<cv::Vec3b>(j,i)[0] = 0;
					output.at<cv::Vec3b>(j,i)[1] = 255;
					output.at<cv::Vec3b>(j,i)[2] = 255;
				}
			}
		}

	}

	sprintf(buffer,"Possible AOIs are highlighted in \033[1;44mBLUE\033[0m.");
	writeLog(buffer,2);
	//Higlight signs
	int *counter = new int[total_area_code+1];
	std::fill_n(counter,total_area_code+1,0);

	for(int i = 0; i <= image_size_x-1; i++)
	{
		for(int j = 0; j <= image_size_y-1; j++)
		{
			counter[poi[i][j].area_code]++;
		}
	}


//find best areas
// improve to make fewer calls
	for (int q = 1; q <= total_area_code; q++)
	{
		if (counter[q] > 600 && counter[q] < 2700)
		{
			std::cout << q << "|";
			for (int i = 0; i <= image_size_x-1; i++)
			{
				for (int j = 0; j < image_size_y-1; j++)
				{
					if (poi[i][j].area_code == q)
					{
						output.at<cv::Vec3b>(j,i)[0] = 255;
						output.at<cv::Vec3b>(j,i)[1] = 255;
						output.at<cv::Vec3b>(j,i)[2] = 0;
					}

				}
			}
		}
	}

	std::cout << std::endl;



	cv::imshow("Output",output);
	cv::imshow( "Input",input );

	delete [] counter;
	counter = 0;

	//INFO: F1 : 65470 to F12: 65481
	//ends the program on ESC(27) or 'c'(99)
	while(true){
	int c = cv::waitKey( 1000 );
    if( (char)c == 27 || (char)c == 99)
      { 
      	break;
      }
	}
	writeLog("Application closed successfully",2);
	return 0;
}