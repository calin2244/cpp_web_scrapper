#ifndef program_settings_h
#define program_settings_h

#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Settings{
    json j;
    std::string pref_lang;
    std::string currency;

    public:
        Settings(std::ifstream& f): j(json::parse(f)), pref_lang(j["lang"]), currency(j["currency"]){}

        ~Settings(){
            delete this;
        }

        std::string getLang() const{
            return pref_lang;
        }

        std::string getCurrency() const{
            return currency;
        }

};

#endif