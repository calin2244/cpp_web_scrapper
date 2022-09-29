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

#define POSITIVE_CHGPC(name) "\u001b[1;32m" + Methods::toUpperCase(name) + "\033[0m"
#define NEGATIVE_CHGPC(name) "\033[1;31m" + Methods::toUpperCase(name) + "\033[0m"

//For Crypto
static void findDayLow_High(string* s, string* html){
    string day_range = "}\" data-target=\"price.price\">";
    size_t pos = (*html).find(day_range);
    (*html).erase((*html).begin(), (*html).begin() + pos + day_range.length());
    *s = (*html).substr(0, (*html).find("<"));
}

class data{
    protected:
        string name;
        string html;
    public:
        data(string name_, string html_){
            name = name_;
            Methods::toUpperCase(name);
            std::stringstream ss;
            ss<<html_;
            html = ss.str();
        }

        ~data(){
            delete this;
        }

        void muie() {}
};

class Stock_Data: public data{
    string price;
    string change;
    string change_percent;
    string day_low, day_high;
    bool valid_stock = true;
    
public:
    
    Stock_Data(string name_, string html_) : data(name_, html_){
        improved_populate();
    }
    
    ~Stock_Data(){
        delete this;
    }

    bool isValid() const{
        // std::string html_exists = "Last Price</th>";
        // int pos = html.find(html_exists);
        // return pos >= 0 ? true : false;
        return valid_stock;
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
                html_copy.clear();
            }
        }
        else{
            valid_stock = false;
        }

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
            if(pos != std::string::npos){
                html.erase(html.begin(), html.begin() + pos + change_html.length());
            change = html.substr(0, html.find("<"));
            }
            //check if the change is negative
            else{
                
                change_html = "C($negativeColor)\">";
                pos = html.find(change_html);
                html.erase(html.begin(), html.begin() + pos + change_html.length());
                change = html.substr(0, html.find("<"));
            }

            std::string change_percent_html = "<span class=\"C($positiveColor)\">";
            pos = html.find(change_percent_html);
            if(pos != std::string::npos){
                html.erase(html.begin(), html.begin() + pos + change_percent_html.length());
                change_percent = html.substr(0, html.find("<"));
            }
            //Change percent is negative
            else{
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
        std::ios_base::sync_with_stdio(true);
        size_t pos = day_high.find(".");
        day_high.erase(day_high.begin() + pos + 2, day_high.end());

        std::cout<<"\n\u001b[33m"<<name<<":\n\033[0m";
        std::cout<<"\tLast Price: \t\t"<<price <<'\n';
        std::cout<<"\tChange: \t\t"<<change<<'\n';
        std::cout<<"\tChg%: \t\t\t"; 
        if(change_percent[0] == '+')
            std::cout<<POSITIVE_CHGPC(change_percent)<<'\n';
        else std::cout<<NEGATIVE_CHGPC(change_percent)<<'\n';
        std::cout<<'\t'<<"Highest-Lowest(Day): \t"<<NEGATIVE_CHGPC(day_low)<<" ---- "<<POSITIVE_CHGPC(day_high)<<'\n';
    }
};

class Crypto_Data: public data{
    string current_price;
    string day_low;
    string day_high;
    string week_low;
    string week_high;
    string trading_volume;
    string market_cap_rank;
    bool valid_crypto = true;

    public:
        Crypto_Data(string name_, string html_): data(name_, html_){
            populate();
        }

        ~Crypto_Data(){
            delete this;
        }

        bool isValid() const{
            return valid_crypto;
        }

        void populate(){
            string current_price_html = "data-target=\"price.price\">";
            size_t pos = html.find(current_price_html);

            if(pos == string::npos){
                valid_crypto = false;
            }
            else{
                html.erase(html.begin(), html.begin() + pos + current_price_html.length());
                current_price = html.substr(0, html.find("<"));

                while(current_price.length() > 12){
                    current_price.pop_back();
                }

                findDayLow_High(&day_low, &html);
                findDayLow_High(&day_high, &html);

                while(day_low.length() > 12){
                    day_low.pop_back();
                }

                while(day_high.length() > 12){
                    day_high.pop_back();
                }

                string trading_volume_html = "data-no-decimal=\"false\" data-target=\"price.price\">";
                pos = html.find(trading_volume_html);
                html.erase(html.begin(), html.begin() + pos + trading_volume_html.length());
                pos = html.find(trading_volume_html);
                html.erase(html.begin(), html.begin() + pos + trading_volume_html.length());
                trading_volume = html.substr(0, html.find("<"));
            }
        }
        
        void show(){
            std::cout<<"\n\u001b[33m"<<name<<":\n\t\033[0m"<<"Current Price: \t\t\t"<<current_price<<'\n';
            std::cout<<'\t'<<"Highest-Lowest(Day): \t\t"<<NEGATIVE_CHGPC(day_low)<<" ---- "<<POSITIVE_CHGPC(day_high)<<'\n';
            std::cout<<'\t'<<"Trading volume: \t\t"<<trading_volume<<'\n';
        }

};

#endif