#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "../lib/lib_log.h"

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


int main( int argc, char** argv )
{
	char buffer[100]; 
	cv::Mat input;
	input = cv::imread("input/img04.jpg", CV_LOAD_IMAGE_COLOR);
	if(!input.data)                              
	{
	  std::cout <<  "Could not open or find the image" << std::endl ;
	  return -1;
	}
	std::cout << "Press any key to exit!" << std::endl;
	  writeLog("WORKS",2);

	cv::namedWindow( "Input", CV_WINDOW_NORMAL);
	cv::namedWindow( "Output", CV_WINDOW_NORMAL);

	cv::Mat output = input.clone();
	int x,y;
	int total_area_code = 0;
	int count_biggest_area = 0;
	int biggest_area = 0;
	int count_area_code = 0;
	bool colour_found = false;
	std::cout << check[1][1].checked << std::endl;
	for (int i = 0; i <= 1279; i++)
	{
		for (int j = 0; j <= 719; j++)
		{	
			if(((input.at<cv::Vec3b>(j,i)[2]) >= (18+input.at<cv::Vec3b>(j,i)[1])) && ((input.at<cv::Vec3b>(j,i)[2]) >= (18+input.at<cv::Vec3b>(j,i)[0])) && (check[i][j].checked == false))
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
						if(((input.at<cv::Vec3b>(y+check_pos,x)[2]) >= (18+input.at<cv::Vec3b>(y+check_pos,x)[1])) && ((input.at<cv::Vec3b>(y+check_pos,x)[2]) >= (18+input.at<cv::Vec3b>(y+check_pos,x)[0])) && (check[x][y+check_pos].checked == false))
						{
							colour_found = true;
							check[x][y+check_pos].checked = true;
							poi[x][y+check_pos].area_code = total_area_code;
							count_area_code++;
						}
						if(((input.at<cv::Vec3b>(y-check_pos,x)[2]) >= (18+input.at<cv::Vec3b>(y-check_pos,x)[1])) && ((input.at<cv::Vec3b>(y-check_pos,x)[2]) >= (18+input.at<cv::Vec3b>(y-check_pos,x)[0])) && (check[x][y-check_pos].checked == false))
						{
							colour_found = true;
							check[x][y-check_pos].checked = true;
							poi[x][y-check_pos].area_code = total_area_code;
							count_area_code++;
						}
						if(((input.at<cv::Vec3b>(y,x+check_pos)[2]) >= (18+input.at<cv::Vec3b>(y,x+check_pos)[1])) && ((input.at<cv::Vec3b>(y,x+check_pos)[2]) >= (18+input.at<cv::Vec3b>(y,x+check_pos)[0])) && (check[x+check_pos][y].checked == false))
						{
							colour_found = true;
							check[x+check_pos][y].checked = true;
							poi[x+check_pos][y].area_code = total_area_code;
							count_area_code++;
						}
						if(((input.at<cv::Vec3b>(y,x-check_pos)[2]) >= (18+input.at<cv::Vec3b>(y,x-check_pos)[1])) && ((input.at<cv::Vec3b>(y,x-check_pos)[2]) >= (18+input.at<cv::Vec3b>(y,x-check_pos)[0])) && (check[x-check_pos][y].checked == false))
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



	std::cout << "#Max entries: " <<  count_biggest_area << " pos of biggest area: " << biggest_area << std::endl;
	std::cout << "Areas in total:" << total_area_code << std::endl;
//Highlight areas; pink => biggest area; yellow => other areas
	for(int i = 0; i <= 1279; i++)
	{
		for(int j = 0; j <= 719; j++)
		{
			if(poi[i][j].area_code == biggest_area){
				output.at<cv::Vec3b>(j,i)[0] = 255;
				output.at<cv::Vec3b>(j,i)[1] = 0;
				output.at<cv::Vec3b>(j,i)[2] = 255;
			}else if (poi[i][j].area_code > 0)
			{
				output.at<cv::Vec3b>(j,i)[0] = 0;
				output.at<cv::Vec3b>(j,i)[1] = 255;
				output.at<cv::Vec3b>(j,i)[2] = 255;
			}
		}
	}


//
	int *counter = new int[total_area_code+1];
	std::fill_n(counter,total_area_code+1,0);

	for(int i = 0; i <= 1279; i++)
	{
		for(int j = 0; j <= 719; j++)
		{
			counter[poi[i][j].area_code]++;
		}
	}
//find best areas
	for (int q = 1; q <= total_area_code; q++)
	{
		if (counter[q] > 200)
		{
			std::cout << q << "|";
			for (int i = 0; i <= 1279; i++)
			{
				for (int j = 0; j < 719; j++)
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

	cv::waitKey(0);

	return 0;
}

