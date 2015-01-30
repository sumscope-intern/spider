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
#include <vector> 
#include "html/ParserDom.h"
#include "util.h"


class spider{
public:
	spider(std::string); 
	bool crawl(); 
	std::vector<std::vector<std::string> > parse(); 
	bool write_db(const std::vector<std::vector<std::string> > & data);			

private: 

	//Read the default configuration file and initialize the url and db table names
	bool init(std::string&); 
	
	//parse a html table from an htmlcxx iterator pointing to table tree root,return a two dimensional vector corresponding to the table rows and colomuns
	std::vector<std::vector<std::string> > parse_html_table(tree<htmlcxx::HTML::Node> &,  tree<htmlcxx::HTML::Node>::iterator &table_root); 		
	
	//this is a callback function used by curl to receive web page data
	static size_t write_html(void * buffer, size_t size,size_t blocks, std::string * html_p);
		
	//actual database write 
	bool write_db(std::string &ccy, std::string &tenor, std::string &ask_rate, std::string &bid_rate, DateTime &date_time, std::string &source);

private:
	std::string url; 
	std::string db_name; 
	std::string table_name;		
	std::string html_doc;
};

#endif //end of MDSERVER_SPIDER_H_ 
