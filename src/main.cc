#include <iostream> 
#include "spider.h"
#include "util.h"


using namespace std; 

int main(int argc, char ** argv){
	string config_file(argv[1]);
	spider spd(config_file);
	spd.crawl();
	vector<vector<string> > table = spd.parse();
	spd.write_db(table);
	return 0;
}
