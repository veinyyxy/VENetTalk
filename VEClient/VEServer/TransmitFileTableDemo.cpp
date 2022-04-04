#include "StdAfx.h"
#include "TransmitFileTableDemo.h"


TransmitFileTableDemo::TransmitFileTableDemo(void)
{
}


TransmitFileTableDemo::~TransmitFileTableDemo(void)
{
}

int TransmitFileTableDemo::CreateFileBlockTable( std::string& strNameID, std::string& strFileID )
{
	std::map<std::string, int> fileMap;
	typedef std::pair<std::string, std::map<std::string, int> > NameFileMapPair;
	typedef std::pair<std::string, int> mapPair;

	FILE_TRANSMIT_MAP_TABLE::iterator iFinded = m_FileMapTable.find(strNameID);
	if(iFinded != m_FileMapTable.end())
	{
		std::map<std::string, int>::iterator iFileStateFinded = iFinded->second.find(strFileID);
		if(iFileStateFinded != iFinded->second.end())
		{
			return 0;
		}
		else
		{
			iFinded->second.insert(mapPair(strFileID, 0));
			return 1;
		}
	}
	fileMap.insert(mapPair(strFileID, 0));
	m_FileMapTable.insert(NameFileMapPair(strNameID, fileMap));
	return 1;
}

int TransmitFileTableDemo::SetFileBlockState( std::string& strNameID, std::string& strFileID, int iBlockID, int iState )
{
	return -1;
}

int TransmitFileTableDemo::GetFileBlockTable( std::string& strNameID, std::string& strFileID, int iBlockID, int& iState )
{
	return -1;
}

int TransmitFileTableDemo::GetBreakPoint( std::string& strNameID, std::string& strFileID, int& iBreak )
{
	return -1;
}

int TransmitFileTableDemo::QueryState( std::string& strNameID, std::string& strFileID )
{
	std::map<std::string, int> fileMap;
	typedef std::pair<std::string, std::map<std::string, int> > NameFileMapPair;
	typedef std::pair<std::string, int> mapPair;

	FILE_TRANSMIT_MAP_TABLE::iterator iFinded = m_FileMapTable.find(strNameID);
	if(iFinded != m_FileMapTable.end())
	{
		std::map<std::string, int>::iterator iFileStateFinded = iFinded->second.find(strFileID);
		if(iFileStateFinded != iFinded->second.end())
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
		return 0;
}
