#pragma once
#include <string>
#include <boost/program_options.hpp>
namespace po = boost::program_options;
class Config
{
public:
    bool Initialize(std::string config_file);
    std::string getValue(std::string key);
    static Config* getInstance();
private:
    po::variables_map vm_;
    static Config instance_;
};