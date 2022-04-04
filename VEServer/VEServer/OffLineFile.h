#pragma once
#include <string>
#include <map>

class OffLineFile
{
public:
	typedef struct ____off_line_file_information__
	{
		std::string strUUID;
		std::string strSrcName;
		std::string strDstName;
		std::string strFileDir;
		int iFileType;
	} OFF_LINE_FILE_INFO, P_OFF_LINE_FILE_INFO;

	typedef std::map<std::string, OFF_LINE_FILE_INFO> OFF_LINE_FILES;

	virtual int WriteOccupyFile(int iUserID, const std::string& strAccount
		, const std::string& strFileUUID, const std::string strFileName
		, int iFileSize) = 0;

	virtual int WriteFileBlock(int iUserID, const std::string& strAccount
		, const std::string& strFileUUID, const std::string strFileName
		, char* pBuffer, int iBlockSize) = 0;


	virtual int GetFileInfo(const std::string& strID, OffLineFile::OFF_LINE_FILES& files) = 0;
	virtual int SaveFileInfo(const std::string& strID, const std::string& strUUID, 
		const OFF_LINE_FILE_INFO& info) = 0;


};

