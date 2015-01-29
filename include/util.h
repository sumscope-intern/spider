#ifndef MDSERVER_UTIL_H_
#define MDSERVER_UTIL_H_

#include <string> 

#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED   "\033[31m"
#define GREEN "\033[32m"

void DEBUG(const std::string &info);
void ERROR(const std::string &info);

#endif 
