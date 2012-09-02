#include "Config.hpp"
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <ezlogger_headers.hpp>

using namespace std;
Config Config::instance_;

bool Config::Initialize(std::string config_file)
{
    po::options_description config("Configuration");
    config.add_options()
        ("port", po::value<string>(), "server port")
        ("sql_host", po::value<string>(), "sql host")
        ("sql_port", po::value<string>(), "sql port")
        ("sql_username", po::value<string>(), "sql db username")
        ("sql_password", po::value<string>(), "sql password")
        ("user_db", po::value<string>(), "sql database name for user information")
    ;

    po::options_description config_file_options;
    config_file_options.add(config);
    ifstream ifs(config_file.c_str());
    if (!ifs)    {
        EZLOGGERVLSTREAM(axter::log_often) << "can not open config file: " << config_file << "\n";
        return 0;
    } else {
        store(po::parse_config_file(ifs, config_file_options, true), vm_);
        notify(vm_);
    }
    return true;
}

std::string Config::getStringValue(std::string key)
{
    if (vm_.count(key)){
        return vm_[key].as<string>();
    }
    return string("");
}

Config* Config::getInstance()
{
    return &instance_;
}
