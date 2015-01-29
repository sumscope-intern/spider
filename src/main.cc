#include <iostream> 
#include "spider.h"
#include "util.h"


using namespace std; 

int main(int argc, char ** argv){
	string config_file(argv[1]);
	spider spd(config_file);
	spd.crawl();
	spd.parse();
	return 0;
}
