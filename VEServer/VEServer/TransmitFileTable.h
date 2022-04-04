#pragma once
#include <string>
class TransmitFileTable
{
public:
	virtual int CreateFileBlockTable(std::string& strNameID, std::string& strFileID) = 0;
	virtual int SetFileBlockState(std::string& strNameID, std::string& strFileID, int iBlockID, int iState) = 0;
	virtual int GetFileBlockTable(std::string& strNameID, std::string& strFileID, int iBlockID, int& iState) = 0;
	virtual int GetBreakPoint(std::string& strNameID, std::string& strFileID, int& iBreak) = 0;
	virtual int QueryState(std::string& strNameID, std::string& strFileID) = 0;
};

