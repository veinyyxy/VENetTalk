#pragma once
class FileBlockMirror
{
public:
	FileBlockMirror(void);
	~FileBlockMirror(void);
	void Create(int blockNum);
	void SetBlockState(int blockID, int iState);
	int GetBlockState(int blockID);
	void Save(const std::string& strFileName, const std::string& strUUID);
	int Read(const std::string& strFileName);
	int GetUUID(std::string* strUUID);
	int GetBreakPoint();

private:
	char m_UUID[50];
	char* m_FileMapTable;
	int FileMapTableSize;
};

