#pragma once

class MakeDirToZip
{
public:
	MakeDirToZip(void);
	~MakeDirToZip(void);
	static void CreateZipFromDir(const std::string& dirName, const std::string& zipFileName);
	static void UncompressFile(const std::string& fileName);
private:
	static void AddFileToZip(zipFile zf, const char* fileNameInZip, const char* srcFile);
	static void CollectFilesInDirToZip(zipFile zf, const std::string& strPath, const std::string& parentDir);
};

