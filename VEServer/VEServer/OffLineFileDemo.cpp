#include "StdAfx.h"
#include <map>
#include "OffLineFile.h"
#include "OffLineFileDemo.h"


OffLineFileDemo::OffLineFileDemo(void)
{

}


OffLineFileDemo::~OffLineFileDemo(void)
{
}

int OffLineFileDemo::WriteOccupyFile( int iUserID, const std::string& strAccount , const std::string& strFileUUID, const std::string strFileName , int iFileSize )
{
	return -1;
}

int OffLineFileDemo::WriteFileBlock( int iUserID, const std::string& strAccount , const std::string& strFileUUID, const std::string strFileName , char* pBuffer, int iBlockSize )
{
	return -1;
}

int OffLineFileDemo::GetFileInfo( const std::string& strID, OffLineFile::OFF_LINE_FILES& files )
{
	m_Mutex.acquire();
	std::map<std::string, OffLineFile::OFF_LINE_FILES>::iterator ifinded 
		= m_InfoMap.find(strID);
	if(ifinded != m_InfoMap.end())
	{
		files = ifinded->second;
		m_InfoMap.erase(strID);
		m_Mutex.release();
		return 1;
	}
	else
	{
		m_Mutex.release();
		return 0;
	}
}

int OffLineFileDemo::SaveFileInfo( const std::string& strID, const std::string& strUUID, const OFF_LINE_FILE_INFO& info )
{
	m_Mutex.acquire();
	std::map<std::string, OffLineFile::OFF_LINE_FILES>::iterator ifinded = m_InfoMap.find(strID);
	if(ifinded != m_InfoMap.end())
	{
		(ifinded->second)[strUUID] = info;
	}
	else
	{
		OffLineFile::OFF_LINE_FILES files;
		files[strUUID] = info;
		m_InfoMap[strID] = files;
	}
	m_Mutex.release();
	return 1;
}
