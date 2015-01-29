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
#include <boost/lexical_cast.hpp>
#include <html/ParserDom.h> 
#include <curl/curl.h>
#include "spider.h" 
#include "util.h"
#include "html/ParserDom.h"

using namespace htmlcxx; 
using namespace std; 

spider::spider(string cfg_file){
	init(cfg_file);
}

/*
* Read default configurations from a setting file spider.ini using boost library program_options
*/
bool spider::init(string & config_file){
	namespace po = boost::program_options; 
	//DEBUG("setup...");

	//setup options
	po::options_description desc("Options");
	desc.add_options()
		("web.url",po::value<string> (&url))
		("db.db_name",po::value<string> (&db_name))
		("db.table_name",po::value<string> (&table_name));	

	//load configuration file 
	po::variables_map vm;
	ifstream setting_file(config_file.c_str()); 
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
	////DEBUG(url);

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
size_t spider::write_html(void * buffer, size_t size, size_t blocks, string  * html_p){
	if(html_p == NULL){
		ERROR("html_p NULL");
		return 1;
	}
	html_p->append((char*)buffer,size*blocks);
	return blocks*size;
}

/*
parse a table in html document
@param1: dom, the html document 
@param2: table_root, the root iterator pointing to the table in html doc
@return: return two dimensional vector corresponding to table
*/
vector<vector<string> > spider::parse_html_table(tree<HTML::Node> & dom,  tree<HTML::Node>::iterator &table_root){
	vector<vector<string> > table; 
	tree<HTML::Node>::sibling_iterator row_it = dom.begin(table_root); 
	//iterate through each row in the table
	while(row_it != dom.end(table_root)){
		if(row_it->tagName() != "tr"){
			++row_it;
			continue;
		}
		tree<HTML::Node>::sibling_iterator col_it = dom.begin(row_it);
		vector<string> row; 
		//iterate through each column
		while(col_it != dom.end(row_it)){
			if(col_it->tagName() != "td"){
				++col_it;
				continue;
			}
			tree<HTML::Node>::sibling_iterator content = dom.child(col_it,0); 
			if(content != NULL){
				string data = content->text();
				boost::trim(data);
				row.push_back(data);	
			}
			++col_it;
		}
		table.push_back(row);
		++row_it;
	}
	return table;
}

/*
Parse the content crawled by crawl() function and store the parsed values in a two-dimensional vector 
*/
vector<vector<string> > spider::parse(){
	HTML::ParserDom parser; 
	tree<HTML::Node> dom = parser.parseTree(html_doc); 	
	tree<HTML::Node>::iterator it = dom.begin(); 
	vector<vector<string> > table;
	for(; it != dom.end(); it++){
		if(it->isTag() && it->tagName() == "table"){
			table = parse_html_table(dom,it);
			break;
		}
	}
	return table ; 
}

//convert a date format like "month-day hour:minute" into mysql date format 
string convert_date(string date){
	return date;
}

//input is the type string of IBO 
string get_tenor(string type){
	if(type.size() < 2){
		ERROR("In get_tenor: Wrong IBO type");
		return "";
	}
	if(boost::iends_with(type,"01") 
		|| boost::iends_with(type,"07")
		|| boost::iends_with(type,"14")
		|| boost::iends_with(type,"21")){
		string  tenors[] = {"O/N","1W","2W","3W"};
		int idx = boost::lexical_cast<int>(type.substr(type.size()-2,2))/7;
		if(idx > 3){
			ERROR("Invalid index");
			return "";	
		}
		return tenors[idx];
	}else{
		return type.substr(type.size()-2,2);
	}	
}

bool spider::write_db(const vector<vector<string> > & data){
	if(data.size() <= 2){
		ERROR("In write_db: HTML table size is to small!!");
		return false;
	}
	//last update time
	string date = convert_date(data[0][1]);
	string source = "cfets";
	string ccy = "CNY";
	//iterate through each row in data
	for(size_t i = 2; i < data.size(); i++){
		const vector<string> & row = data[i]; 
		string tenor = get_tenor(row[0]); 
		string ask_rate = row[1];
		string bid_rate = row[1];
		write_db(ccy,tenor,ask_rate,bid_rate,date,source);
	}	
	return true;
}

bool spider::write_db(string &ccy, string &tenor, string &ask_rate, string &bid_rate, string &date, string &source){
	cout << ccy << " " << tenor << " " << ask_rate << " " << bid_rate << " " << date << " " << source << endl;	
	return true;
}
