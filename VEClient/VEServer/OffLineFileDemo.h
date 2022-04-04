#pragma once

class OffLineFileDemo :
	public OffLineFile
{
public:
	OffLineFileDemo(void);
	~OffLineFileDemo(void);

	virtual int WriteOccupyFile( int iUserID, const std::string& strAccount 
		, const std::string& strFileUUID, const std::string strFileName , int iFileSize );

	virtual int WriteFileBlock( int iUserID, const std::string& strAccount 
		, const std::string& strFileUUID, const std::string strFileName , char* pBuffer, int iBlockSize );

	virtual int GetFileInfo( const std::string& strID, OffLineFile::OFF_LINE_FILES& files);

	virtual int SaveFileInfo( const std::string& strID, const std::string& strUUID, const 
		OFF_LINE_FILE_INFO& info );
private:
	std::map<std::string, OffLineFile::OFF_LINE_FILES> m_InfoMap;
	ACE_Thread_Mutex m_Mutex;
};

