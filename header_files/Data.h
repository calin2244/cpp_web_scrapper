#ifndef utils_h
#define utils_h

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <fstream>
#include "functions.h"
using std::vector;
using std::string;

class Stock_Data{
    string html;
    string name;
    string price;
    string change;
    string change_percent;
    string day_low, day_high;
    bool valid_stock = true;
    
public:
    Stock_Data(string name_, string html_) {
        name = name_;
        Methods::Methods::toUpperCase(name);
        //all the html of the website
        std::stringstream ss;
        // html_.erase(html_.begin(), html_.begin() + 200000);
        ss<<html_;
        html = ss.str();
        improved_populate();
    }
    
    bool isValid() const{
        // std::string html_exists = "Last Price</th>";
        // int pos = html.find(html_exists);
        // return pos >= 0 ? true : false;
        return valid_stock;
    }
    
    ~Stock_Data(){
        delete this;
    }

    /**
     * @brief Picks through the html to find the needed data.
     */
    void populate(){
        // regularMarketChange":{"
        if(this->isValid()){
            /*  
                Checks if the stock page exists and returns the actual value from the html <th> element
                The value stored in the JSON is residual
            */
            std::string lastPrice_html = "\"Last Price\">";
            unsigned int pos = html.find(lastPrice_html) + lastPrice_html.length();
            
            if(html[pos] == '-'){
                price = "-";
                change = "-";
            }
            else{
                /*
                    Sadly, pretty inefficient.
                    Storing a 700k characters html page into a variable and then operating on it it's not really optimal.
                    Will be looking for api's to make requests on / Somehow make this more efficient
                */
                html.erase(html.begin() + 700000, html.end());
                string market_price_json = "regularMarketPrice\":{\"raw\":";
                string change_json = "regularMarketChange\":{\"raw\":";
                unsigned int marketPrice = html.find(market_price_json) + market_price_json.size();
                unsigned int change_ = html.find(change_json) + change_json.size();
                string curr_price = html.substr(marketPrice, 10);
                string curr_change = html.substr(change_, 26);
                price = Methods::Methods::refactorOutput(curr_price) + " USD";
                change = Methods::Methods::getFmtDataFromJson(curr_change);
            }
            
        }
    }

    void improved_populate(){
        //Finding the Day Range
        string html_copy = html;
        string day_low_high_html = "\"regularMarketDayRange\":{\"raw\":\"";
        size_t pos = html_copy.find(day_low_high_html);
        if(pos != std::string::npos){
            html_copy.erase(html_copy.begin(), html_copy.begin() + pos + day_low_high_html.length());
            day_low = html_copy.substr(0, html_copy.find("\""));
            std::stringstream ss(day_low);
            string word;
            day_low = "";
            while(!ss.eof()){
                std::getline(ss, word, '-');
                if(day_low == "")
                    day_low = word;
                else{
                    day_high = word;
                    break;
                }
            }
        }
        html_copy.clear();

        html.erase(html.begin() + 97000, html.end());

        //Finding the Last Price
        string price_html = "data-field=\"regularMarketPrice\" data-trend=\"none\" data-pricehint=\"2\" value=\"";
        pos = html.find(price_html);
        if(pos != std::string::npos){
            //eraes everything until the first digit inside the value="" attribute
            html.erase(html.begin(), html.begin() + pos + price_html.length());
            price = html.substr(0, html.find("\"")) + " USD";

            //finding the change, this will work for a positive change
            std::string change_html = "data-field=\"regularMarketChange\" data-trend=\"txt\" data-pricehint=\"2\" value=\"";
            pos = html.find(change_html);
            html.erase(html.begin(), html.begin() + pos + change_html.length());
            change_html = "C($positiveColor)\">";
            pos = html.find(change_html);
            html.erase(html.begin(), html.begin() + pos + change_html.length());
            change = html.substr(0, html.find("<"));

            //check if the change is negative
            auto is_numeric = [](char c) -> bool{return c >= '0' && c <= '9';};
            //TO BE MORE EXACT: [0] - +, [1] - NUMERIC
            if(!is_numeric(change[1])){
                change_html = "C($negativeColor)\">";
                pos = html.find(change_html);
                html.erase(html.begin(), html.begin() + pos + change_html.length());
                change = html.substr(0, html.find("<"));
            }

            std::string change_percent_html = "<span class=\"C($positiveColor)\">";
            pos = html.find(change_percent_html);
            html.erase(html.begin(), html.begin() + pos + change_percent_html.length());
            change_percent = html.substr(0, html.find("<"));

            //Change percent is negative
            if(!is_numeric(change_percent[1])){
                change_percent_html = "C($negativeColor)\">";
                pos = html.find(change_percent_html);
                html.erase(html.begin(), html.begin() + pos + change_percent_html.length());
                change_percent = html.substr(0, html.find("<"));
            }
        }
        else{
            valid_stock = false;
        }
    }
    
    std::string getPrice() const{
        return price;
    }

    /*
        Prints information about each company's stock
    */
    void show(){
        size_t pos = day_high.find(".");
        day_high.erase(day_high.begin() + pos + 2, day_high.end());
        std::cout<<"\n"<<name<<":\n"<<"\tLast Price: \t\t"<<price <<'\n'<<"\tChange: \t\t"<<change<<'\n';
        std::cout<<"\tChg%: \t\t\t"<<change_percent<<'\n';
        std::cout<<"\tLowest-Highest(Day): \t";
        printf("\033[1;31m%s\033[0m ---- \u001b[1;32m%s\033[0m", day_low.c_str(), day_high.c_str());
    }
};

class Crypto_Data{
    string name;
    string price;
    string html;
    public:
        Crypto_Data(string name_, string html_): name(name_), html(html_){}
};

#endif