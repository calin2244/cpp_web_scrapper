#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <json/json.h>
#include <fstream>
#include <set>
#include <nlohmann/json.hpp>
#include "header_files/program_settings.h"
#include "header_files/Data.h"
#include <ios>
#include <sstream>
#include <curl/curl.h>
#include <iomanip>
#include <thread>
#include  <chrono>
#include <stdlib.h>

using std::map;
using std::string;

using json = nlohmann::json;

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

int main() {
    
    while(true){
        CurlObj addr("https://www.coingecko.com/en/coins/bitcoin");
        try{
            Crypto_Data *crp = new Crypto_Data("BITCOIN", addr.retrieveData());
            crp->show();
            std::this_thread::sleep_for(std::chrono::seconds(6));
            std::system("clear");
        }catch(std::exception& e){
            std::cout<<"BAD URL";
        }
    }

    return 0;
}
