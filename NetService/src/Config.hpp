#pragma once
#include <string>
class Config
{
public:
    void Initialize();
    std::string getValue(std::string key);
    static Config* getInstance();
};