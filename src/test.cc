#include <string> 
#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <html/ParserDom.h>

using namespace std;
using namespace htmlcxx;

void test_htmlcxx(){
  //Parse some html code
  string html = "<html><body>hey</body></html>";
  HTML::ParserDom parser;
  tree<HTML::Node> dom = parser.parseTree(html);
  
  //Print whole DOM tree
  cout << dom << endl;
  
  //Dump all links in the tree
  tree<HTML::Node>::iterator it = dom.begin();
  tree<HTML::Node>::iterator end = dom.end();
  for (; it != end; ++it)
  {
     if (strcasecmp(it->tagName().c_str(), "A") == 0)
     {
       it->parseAttributes();
       cout << it->attribute("href").second << endl;
     }
  }
  
  //Dump all text of the document
  it = dom.begin();
  end = dom.end();
  for (; it != end; ++it)
  {
    if ((!it->isTag()) && (!it->isComment()))
    {
      cout << it->text();
    }
  }

  cout << endl;

}

void test_curl(int argc, char ** argv){
    CURL *curl;            
    CURLcode res;          
    if(argc!=2)
    {
        printf("Usage : file <url>;\n");
        exit(1);
    }
 
    curl = curl_easy_init();       
    if(curl!=NULL)
    {
        curl_easy_setopt(curl, CURLOPT_URL, argv[1]);        
        res = curl_easy_perform(curl);
    }
	
}

int main(int argc, char *argv[])
{
    //test_htmlcxx();
	test_curl(argc,argv);
    return 0;
}


