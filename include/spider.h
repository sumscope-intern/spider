/**********************************************************************************************************************************************************************
* @author: Jiang Zhihui (zhihui.jiang@sumscope.com)
* @file: spider.h  
* The file defines interfaces for class spider. It has three main functions: crawl a web page, parse the crawled html file, write the parsed result into database, each 
* corresponding to a public method. 
* The class is designed to be reusable and configurable so that a new requirement can be easily implemented using this skeleton.
***********************************************************************************************************************************************************************/
#ifndef MDSERVER_SPIDER_H_
#define MDSERVER_SPIDER_H_

#include <string> 

class spider{
public:
	spider(std::string); 
	bool crawl(); 
	bool parse(); 
	bool write_db();			

private: 

	//Read the default configuration file and initialize the url and db table names
	bool init(std::string&); 
	
	static size_t write_html(void * buffer, size_t size,size_t blocks, std::string * html_p);
		
private:
	std::string url; 
	std::string db_name; 
	std::string table_name;		
	std::string html_doc;
};

#endif //end of MDSERVER_SPIDER_H_ 
