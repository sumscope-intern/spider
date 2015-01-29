#include <iostream> 
#include "util.h"

void DEBUG(const std::string &info){
	std::string green("\033[33m");
	std::string deft("\033[0m");
	std::cout << green << "DEBUG: " << deft <<  info << std::endl;
}

void ERROR(const std::string &info){
	std::cout << RED << "ERROR: " << RESET << info << std::endl;
}
