#pragma once
#include "transmitfiletable.h"
class TransmitFileTableDemo :
	public TransmitFileTable
{
public:
	typedef struct ____file_block_____
	{
		//int direct;
		int block_count;
		int break_point;
		char* block_state;
	} FILE_BLOCK_MAP, *P_FILE_BLOCK_MAP;
	typedef std::map<std::string, std::map<std::string, int> > FILE_TRANSMIT_MAP_TABLE
		, P_FILE_TRANSMIT_MAP_TABLE;

	TransmitFileTableDemo(void);
	virtual ~TransmitFileTableDemo(void);

	virtual int CreateFileBlockTable( std::string& strNameID, std::string& strFileID );

	virtual int SetFileBlockState( std::string& strNameID, std::string& strFileID, int iBlockID, int iState );

	virtual int GetFileBlockTable( std::string& strNameID, std::string& strFileID, int iBlockID, int& iState );

	virtual int GetBreakPoint( std::string& strNameID, std::string& strFileID, int& iBreak );

	virtual int QueryState( std::string& strNameID, std::string& strFileID );

private:
	FILE_TRANSMIT_MAP_TABLE m_FileMapTable;
};

