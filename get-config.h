//***************
//	Date: 2017-03-09
//	Description: Read info from config.ini, and store it as map format;
//	Requirements: Regard '#' as comment char; The config info presence as key = value. Either space exits or not is ok;
//***************

#ifndef _GET_CONFIG_H_  
#define _GET_CONFIG_H_  
#define COMMENT_CHAR '#'  
#include <string>  
#include <map>  
  
using namespace std;  
  
bool ReadConfig(const string & filename, map<string, string> & m);  
void PrintConfig(const map<string, string> & m);  
  
#endif  // _GET_CONFIG_H_
