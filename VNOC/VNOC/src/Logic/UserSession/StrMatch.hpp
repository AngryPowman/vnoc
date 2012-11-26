/**
 * file: StrMatch.hpp - For Checking the legitimacy of a string
 *
 * Date: 2012/11/24
 *
 * Author: rookie2
 */

#pragma once

#include<regex>
#include<string>

const unsigned short MATCH_TYPE_NAME = 0xfe;
const unsigned short MATCH_TYPE_PWD  = 0xff;

/**
 * 功能: 用于验证字符串是否匹配正则表达式
 * @param str: 待验证字符串
 * @param matchType: 指定默认正则式的种类,使用自定义正则式时值为NULL
 * @param regEex: regular expression, 自定义的正则表达式
 * @return: 字符串若匹配正则表达式返回true, 否则返回false
 */
bool IsStringMatch(std::wstring str,
                   const unsigned short matchType = MATCH_TYPE_NAME,
                   std::wstring regExp = L"^[a-zA-Z0-9_]{5,20}$")
{
    std::wregex exp(regExp);
    if (MATCH_TYPE_PWD == matchType)
    {
        exp.assign(L"^[\\@A-Za-z0-9_\\!\\#\\$\\%\\^\\&\\*\\.\\~]{6,20}$");
    }

    return std::regex_match(str, exp);
}