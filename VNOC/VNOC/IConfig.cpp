#include "stdafx.h"
#include "IConfig.h"

IConfig::IConfig( LPCSTR strRoot )
{
	m_rootName = strRoot;
}

IConfig::~IConfig()
{
}

BOOL IConfig::_ParseConfig( const TiXmlElement* root )
{
	return TRUE;
}

const TiXmlElement* IConfig::_ReadConfig( LPCSTR rootName/*=NULL*/ )
{
	return NULL;
}

TiXmlElement* IConfig::_QueryConfig( LPCSTR rootName/*=NULL*/ )
{
	return NULL;
}

void IConfig::_ReleaseConfig( BOOL writeToFileNow/*=FALSE*/ )
{

}
