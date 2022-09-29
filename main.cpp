#include <curl/curl.h>
#include "header_files/Data.h"
#include "header_files/functions.h"
#include "header_files/json_utlis.h"
#include "header_files/program_settings.h"
#include <map>
#include <sstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <fstream>
#include <time.h>
#include <iomanip>

using json = nlohmann::json;
using std::cin;
using std::map;

#define YELLOW_STOCK_NAME(name) "\u001b[33m" + Methods::toUpperCase(name) + "\033[0m"

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

static void printLogo();
inline void printStockData(const vector<string>* stock_names, myJson* myJ, Settings* program_settings, size_t size);
std::ifstream create_file_if_not_exists(const string s, const string content);

int main() {
    time_t start, end;
    
    //FILE STREAMS
    std::ifstream f_settings("json_files/program_settings.json", std::ios::in);
    if(!f_settings) 
        f_settings = Methods::create_file_if_not_exists("program_settings.json", "{\"lang\":\"EN\", \"currency\":\"USD\"}");
    std::ifstream f_stocks("json_files/pref.json", std::ios::in);
    if(!f_stocks) 
        f_stocks = Methods::create_file_if_not_exists("pref.json", "null");
    
    Settings* program_settings = new Settings(f_settings);
    myJson* myJ = new myJson(f_stocks);

    if(program_settings->getLang() == "RO")
        std::cout<<"Ce doresti sa cauti? 1 - Stocks || 2 - Crypto\n\nIntrodu raspunsul: ";
    else if(program_settings->getLang() == "EN")
        std::cout<<"What do you want to search for? 1 - Stocks || 2 - Crypto\n\nEnter your answer: ";
    
    unsigned int choice;
    cin>>choice;
    //clear the unwanted buffer
    cin.ignore();
    //STOCKS
    if(choice == 1){
        //We will be using this CSV file to extract the translation in the respective language based on the program_settings' {lang} json value
        auto csvRows = Methods::read_csv("csv.txt");

        /*
            IN THE program_settings.json
            lang - the language the program will run in
            using this map we'll know what language we should use when producing output
        */
        map<string, int> language_index={
            {"RO", 0}, {"EN", 1}
        };
        unsigned int lang_indx = language_index[program_settings->getLang()];
        
        vector<string> responses{};
        for(unsigned int i = 1; i < csvRows.size(); ++i){
            responses.push_back(csvRows[i][lang_indx]);
        }
    
        //This counter will go through every line/sentence stored in the csv file accordingly to the program's lang
        unsigned int counter = 0;
        
        //Introdu stock-urile(prin spatiu)
        std::cout<<'\n'<<responses[counter++]<<"\n\n";
        std::string input, company_stock, delimiter=" ", token;
        
        Methods::readInput(&company_stock);

        time(&start);
        
        if(company_stock.size()){
            //Split the content of company stock in words and add them into a vector<string>
            std::stringstream ss(company_stock);
            std::istream_iterator<string> begin(ss);
            std::istream_iterator<string> end;
            std::vector<std::string> stock_names(begin, end);

            //Se proceseaza datele
            std::cout << responses[counter++]<<"\n\n\n";
            
            //get all the data and print it
            printStockData(&stock_names, myJ, program_settings, stock_names.size());

            std::ofstream g("json_files/pref.json", std::ios::out | std::ios::trunc);
            myJ->populateJsonFile(g);
        }

    time(&end);
    double time_taken = double(end - start);
    std::cout<<"\n\n\t\tTIMPUL EXECUTIEI: "<< std::fixed << time_taken << std::setprecision(5)<<"\n\n";

    //In case you skipped looking for stocks
    if(company_stock.size() == 0)
        counter++;

    //Doresti sa aflii cele mai cautate stock-uri si de cate ori le-ai cautat?
    std::cout<<'\n'<<responses[counter++]<<"\n";
                                                //DA || NU
    std::cout<<"\u001b[33m\t\t\t------------------\n\t\t\t"<<responses[counter++]<<"\n\t\t\t------------------\n\n\n\033[0m";

    //Introdu raspunsul: 
    std::cout<<responses[counter++];
    int answer;
    cin>>answer;
    if(answer == 1){
        unsigned int num_of_stocks;
        //Introdu numarul de stock-uri: 
        std::cout<<"\n"<<responses[counter++];
        cin>>num_of_stocks;
        std::cout<<"\n\n";
        if(num_of_stocks != 0)
            myJ->showMostSearchedStocks(num_of_stocks);
    }

    }
    //CRYPTO
    else{
        string crypto;

        std::cout<<"\nIntrodu Criptomonedele: ";
        Methods::readInput(&crypto);

        if(crypto.size()){
        //Split the content of company stock in words and add them into a vector<string>
            std::stringstream ss(crypto);
            std::istream_iterator<std::string> begin(ss);
            std::istream_iterator<std::string> end;
            std::vector<std::string> crypto_names(begin, end);

            std::cout<<"Processing data... \n";

            for(unsigned int i = 0; i < crypto_names.size(); ++i){
                string name = crypto_names[i];
                std::string address = "https://www.coingecko.com/en/coins/" + name;
                CurlObj addr(address); 
                try{
                    Crypto_Data *crypto_data = new Crypto_Data(name, addr.retrieveData());
                    bool valid = crypto_data->isValid();

                    if(valid){
                        crypto_data->show();
                    }
                    else throw(valid);
                }catch(bool isValid){
                    //Stock-ul nu exista
                    if(program_settings->getLang() == "RO")
                        std::cout<<"\n\nCryptomoneda "<< YELLOW_STOCK_NAME(name) <<" nu exista.\n";
                    else if(program_settings->getLang() == "EN")
                        std::cout<<"\n\nThe crypto currency "<< YELLOW_STOCK_NAME(name) <<" doesn't exist.\n";
            }
        }
    }
    }
    f_stocks.close();
    f_settings.close();
    return 0;    
}

static void printLogo(){
    std::ifstream logo("my_logo.txt");
    string line{};
    while(std::getline(logo, line)){
        std::cout<<"\t\t"<<line<<'\n';
    }
    std::cout<<'\n';
}

inline void printStockData(const vector<string>* stock_names, myJson* myJ, Settings* program_settings, size_t size){
    map<string, unsigned int> searched_stock{};
    for (unsigned int i = 0; i < size; i++) {
        std::string name = (*stock_names)[i];
        if(searched_stock.find(name) == searched_stock.end()){
            searched_stock.insert({name, 1});
        }
        else searched_stock[name]++;    
        
        //if the stock repeats, we just want to continue
        if(searched_stock[name] > 1)
            continue;

        std::string address = "https://finance.yahoo.com/quotes/" + name;
        CurlObj addr(address); 
        try{
            Stock_Data *stock_data = new Stock_Data(name, addr.retrieveData());
            bool valid = stock_data->isValid();

            if(valid){
                stock_data->show();
                myJ->insertInMap(name);
            }
            else throw(valid);
        }catch(bool isValid){
            //Stock-ul nu exista
            if(program_settings->getLang() == "RO")
                std::cout<<"\n\nStock-ul "<< YELLOW_STOCK_NAME(name) <<" nu exista.\n";
            else if(program_settings->getLang() == "EN")
                std::cout<<"\n\nThe stock "<< YELLOW_STOCK_NAME(name) <<" doesn't exist.\n";
        }
    }
}