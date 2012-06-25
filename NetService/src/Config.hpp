#pragma once
#include <string>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
class Config
{
public:
    bool Initialize(std::string config_file);
    std::string getStringValue(std::string key);
    struct ConfigValue{
        ConfigValue(std::string value):m_value(value){}
        operator int(){return atoi(m_value.c_str());}
        operator std::string(){return m_value;}
        std::string m_value;
    };
    ConfigValue getValue(std::string key){return ConfigValue((getStringValue((key))));}
    static Config* getInstance();
private:
    po::variables_map vm_;
    static Config instance_;
};