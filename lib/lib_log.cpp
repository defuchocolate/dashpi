#include <iostream>
#include <fstream>
#include <cstdlib>
#include "lib_log.h"

using namespace std;
/**
   Writes a message to the log, with a timestamp and a prefix
   Outputs the same message into the std::cout stream without timestamps
   @version 0.4
   @param input_str input message
   @param m_type message type
   @return void
 */
void writeLog(string input_str, short int m_type){
  time_t now = time(0);
  struct tm* tm = localtime(&now);
  ofstream out("dashpi.log", ios::out | ios::app);
  if(!out){ 
    std::cout << "You do not have the permission to use this application!" << std::endl; //Check if program is used with sudo rights
    exit(0);
  }
  string prefix;
  string color;
  switch(m_type){
  case 0:
    color = "\033[1;91m";
    prefix = "Error";
    break;
  case 1:
    color = "\033[1;93m";
    prefix = "Warning";
    break; 
  case 2:
    color = "\033[1;94m";
    prefix = "Info";
    break;
  default:
    color = "\033[1;91m";
    prefix = "ERROR";
    break;  
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
  std::cout << "Log Message: " << color << "[" << prefix << "] " <<"\033[0m" << input_str <<std::endl;
}

