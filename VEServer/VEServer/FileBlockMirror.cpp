#include "StdAfx.h"
#include "FileBlockMirror.h"
#include <fstream>

FileBlockMirror::FileBlockMirror(void)
{
	memset(m_UUID, 0, sizeof(m_UUID));
	m_FileMapTable = NULL;
	FileMapTableSize = 0;
}


FileBlockMirror::~FileBlockMirror(void)
{
	//if(m_FileMapTable)
		//delete[] m_FileMapTable;
}

void FileBlockMirror::Create( int blockNum )
{
	FileMapTableSize = blockNum;
	m_FileMapTable = new char[blockNum];
	memset(m_FileMapTable, 0, blockNum);
}

void FileBlockMirror::SetBlockState( int blockID, int iState )
{
	if(blockID < 0 || blockID > FileMapTableSize -1) return;
	m_FileMapTable[blockID] = (unsigned char)iState;
}

int FileBlockMirror::GetBlockState(int blockID)
{
	return m_FileMapTable[blockID];
}

void FileBlockMirror::Save( const std::string& strFileName, const std::string& strUUID )
{
	ofstream outfile;
	outfile.open(strFileName);

	size_t len = strUUID.length();
	outfile.write((char*)&len, sizeof(size_t));
	outfile.write(strUUID.c_str(), strUUID.length());
	outfile.write((char*)&FileMapTableSize, sizeof(FileMapTableSize));
	outfile.write(m_FileMapTable, FileMapTableSize);
	outfile.close();
}

int FileBlockMirror::Read( const std::string& strFileName)
{
	ifstream infile;
	infile.open(strFileName);

	size_t len = 0;
	infile.read((char*)&len, sizeof(size_t));
	infile.read(m_UUID, len);
	infile.read((char*)&FileMapTableSize, sizeof(FileMapTableSize));

	m_FileMapTable = new char[FileMapTableSize];
	memset(m_FileMapTable, 0, FileMapTableSize);

	infile.read(m_FileMapTable, FileMapTableSize);
	infile.close();

	return 1;
}

int FileBlockMirror::GetUUID( std::string* strUUID )
{
	strUUID->clear();
	*strUUID = m_UUID;
	return 1;
}

int FileBlockMirror::GetBreakPoint()
{
	int iIndex = 0;
	
	while(iIndex < FileMapTableSize)
	{
		int iState = m_FileMapTable[iIndex];

		if(iState == 0) return iIndex;

		iIndex++;
	}

	return -1;
}
