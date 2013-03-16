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
 * ����: ������֤�ַ����Ƿ�ƥ��������ʽ
 * @param str: ����֤�ַ���
 * @param matchType: ָ��Ĭ������ʽ������,ʹ���Զ�������ʽʱֵΪNULL
 * @param regEex: regular expression, �Զ����������ʽ
 * @return: �ַ�����ƥ��������ʽ����true, ���򷵻�false
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