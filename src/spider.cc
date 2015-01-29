/***************************************************************************************
* @author: Jiang Zhihui(zhihui.jiang@sumscope.com) 
* @file: spider.cc 
* The file implements the interfaces of class spider
***************************************************************************************/
#include <iostream> 
#include <fstream> 
#include <string> 
#include <map> 
#include <vector> 
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <html/ParserDom.h> 
#include <curl/curl.h>
#include "spider.h" 
#include "util.h"
#include "html/ParserDom.h"

spider::spider(std::string cfg_file){
	init(cfg_file);
}

/*
* Read default configurations from a setting file spider.ini using boost library program_options
*/
bool spider::init(std::string & config_file){
	namespace po = boost::program_options; 
	DEBUG("setup...");

	//setup options
	po::options_description desc("Options");
	desc.add_options()
		("web.url",po::value<std::string> (&url))
		("db.db_name",po::value<std::string> (&db_name))
		("db.table_name",po::value<std::string> (&table_name));	

	//load configuration file 
	po::variables_map vm;
	std::ifstream setting_file(config_file.c_str()); 
	if(!setting_file)
		return false;
	po::store(po::parse_config_file(setting_file,desc),vm);
	po::notify(vm); 
	setting_file.close();  

	return true;
}

/*
Crawl a web page refered by url, and save the page content to html_doc
*/
bool spider::crawl(){
	//init curl 	
	CURLcode ret = curl_global_init(CURL_GLOBAL_ALL); 
	if(CURLE_OK != ret) 
		return false;	
	CURL *curl = curl_easy_init();
	if(NULL == curl)
		return false;
	//DEBUG(url);

	//setup curl options
	curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,spider::write_html);		
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&html_doc);
	ret = curl_easy_perform(curl);
	if(CURLE_OK != ret) {
		ERROR("perform error");
		return false;	
	}
	
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	return true;
}

/*
Callback function used by curl to write data to html_doc
*/
size_t spider::write_html(void * buffer, size_t size, size_t blocks, std::string  * html_p){
	if(html_p == NULL){
		ERROR("html_p NULL");
		return 1;
	}
	html_p->append((char*)buffer,size*blocks);
	return blocks*size;
}

/*
Parse the content crawled by crawl() function and store the parsed values in a map
*/
bool spider::parse(){
	using namespace htmlcxx; 
	HTML::ParserDom parser; 
	tree<HTML::Node> dom = parser.parseTree(html_doc); 	
	tree<HTML::Node>::iterator it = dom.begin(); 
	std::map<std::string,std::string> values;
	for(; it != dom.end(); it++){
		if(!it->isTag() && !it->isComment()){
			std::string txt = it->text();
			boost::trim(txt);		
			if(txt.size() == 0) continue;	
			if(boost::contains(txt,"CNY")){
				++it; 
				std::cout << it->text();
				values[txt] = it->text();	
			}
		}
	}
	std::cout << std::endl;
	for(std::map<std::string,std::string>::iterator it = values.begin(); it != values.end(); ++it){
		std::cout << it->first << ":" << it->second << std::endl;
	}
	return true; 
}
