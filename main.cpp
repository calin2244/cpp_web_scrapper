#include <curl/curl.h>
#include "utils.h"
#include "json_utlis.h"
#include <map>
#include <sstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;
using std::cin;
using std::map;

//CurlObj is used to get the html from the given webpage
class CurlObj {
public:

    CurlObj (std::string url) {
        curl = curl_easy_init();
        if (!curl) {
            throw std::string ("Curl did not initialize.");
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CurlObj::curlWriter);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &curlBuffer);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_perform(curl);
    };
    
    static int curlWriter(char *data, int size, int nmemb, std::string *buffer) {
        int result = 0;
        if (buffer != NULL) {
            buffer->append(data, size * nmemb);
            result = size * nmemb;
        }
        
        return result;
    }
    
    std::string retrieveData() {
        return curlBuffer;
    }
    
protected:
    CURL * curl;
    std::string curlBuffer;
};
//credit to stack overflow for help on the CurlObj class

static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

int main() {
    
    //get number of companies to research
    std::cout << "Introdu stock-urile(Separate prin spatiu)\n\n";
    std::string input, company_stock, delimiter=" ", token;

    //initalizing the json
    std::ifstream f("pref.json");
    myJson* myJ = new myJson(f);

    //myJ.showJsonContent();

    Methods::readInput(&company_stock);

    // //Split the content of company stock into a vector<string>
    std::stringstream ss(company_stock);
    std::istream_iterator<std::string> begin(ss);
    std::istream_iterator<std::string> end;
    std::vector<std::string> stock_names(begin, end);

    std::cout << "Se proceseaza datele...\n\n\n";
    // //std::vector<Data> dataObjs;
    
    // ////get all the data and print it out
    for (size_t i = 0; i < stock_names.size(); i++) {
        std::string name = stock_names[i];

        

        std::string address = "https://finance.yahoo.com/quotes/" + name + "/view/v1";
        CurlObj addr(address); 
        try{
            Data *data = new Data(name, addr.retrieveData());
            bool valid = data->isValid();
            if(valid){
                data->show();
                // if(json_map.find(name) == json_map.end()){
                //     json_map.insert({name, 1});
                // }
                // else json_map[name]++;
                // myJson::populateMap(pref_stocks);
                myJ->insertInMap(name);
            }
                
            else throw(valid);
        }catch(bool isValid){
            std::cout<<"\n\nStock-ul "<<Methods::toUpperCase(name)<<" nu exista.\n";
        }
    }

    // for(auto &el: json_map){
    //     preffered_stocks[el.first] = el.second;
    // }

    std::ofstream g("pref.json");
    myJ->populateJsonFile(g);

    return 0;
}