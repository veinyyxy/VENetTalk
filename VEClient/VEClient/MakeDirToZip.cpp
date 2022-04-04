#include "StdAfx.h"
#include "ace/dirent.h"
#include "zip.h"
#include "unzip.h"
#include "MakeDirToZip.h"

MakeDirToZip::MakeDirToZip(void)
{
}


MakeDirToZip::~MakeDirToZip(void)
{
}

//最终接口：从某个目录创建zip文件
//void MakeDirToZip::CreateZipFromDir(const std::string& dirName, const std::string& zipFileName);


//将文件添加到zip文件中，注意如果源文件srcFile为空则添加空目录
//fileNameInZip: 在zip文件中的文件名，包含相对路径
void MakeDirToZip::AddFileToZip(zipFile zf, const char* fileNameInZip, const char* srcFile)
{
  FILE* srcfp = NULL;

  //初始化写入zip的文件信息
  zip_fileinfo zi;
  zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
  zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
  zi.dosDate = 0;
  zi.internal_fa = 0;
  zi.external_fa = 0;

  //如果srcFile为空，加入空目录
  char new_file_name[MAX_PATH];
  memset(new_file_name, 0, sizeof(new_file_name));
  strcat_s(new_file_name, fileNameInZip);
  if (srcFile == NULL)
  {
      strcat_s(new_file_name, "/");
  }
  
  //在zip文件中创建新文件
  zipOpenNewFileInZip(zf, new_file_name, &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);

  if (srcFile != NULL)
  {
      //打开源文件
      srcfp = fopen(srcFile, "rb");
      if (srcfp == NULL)
      {
          //MessageBox(_T("无法添加文件") + std::string(srcFile) + _T("!"));
          zipCloseFileInZip(zf); //关闭zip文件
          return;
      }

      //读入源文件并写入zip文件
      char buf[100*1024]; //buffer
      int numBytes = 0;
      while( !feof(srcfp) )
      {
          numBytes = fread(buf, 1, sizeof(buf), srcfp);
          zipWriteInFileInZip(zf, buf, numBytes);
          if( ferror(srcfp) )
              break;
      }

      //关闭源文件
      fclose(srcfp);
  }

  //关闭zip文件
  zipCloseFileInZip(zf);
}

int IsDir(const char* filename)
{
	struct _stat buf;
	int r = _stat( filename, &buf );
	if( r == 0 )
	{
		if( buf.st_mode & _S_IFDIR )
			return 0;//floder
		else
			return 1;//file;
	}
	else
	{
		if( errno == ENOENT )
			return 2;//printf( "nonexistent\n" );
		else
			return -1;//printf( "error" );
	}
}

//递归添加子目录到zip文件
void MakeDirToZip::CollectFilesInDirToZip(zipFile zf, const std::string& strPath, const std::string& parentDir)
{
  std::string strRelativePath;
  
  ACE_Dirent dir;

  if(dir.open(strPath.c_str()) != -1)
  {
	  for (ACE_DIRENT *directory = 0;(directory = dir.read ()) != 0;)
	  {
		  std::string fileFullPath = strPath 
			  + std::string("\\") + std::string(directory->d_name);
		  if(strcmp(directory->d_name, ".") == 0
			  || strcmp(directory->d_name, "..") == 0)
		  {
			  continue;
		  }

		  if (strcmp(parentDir.c_str(), "") == 0)
		  {
			  strRelativePath = directory->d_name;
		  }
		  else
		  {

			  strRelativePath = parentDir + std::string("\\") + std::string(directory->d_name); //生成在zip文件中的相对路径
		  }

		  if(IsDir(fileFullPath.c_str()) == 0)
		  {
			  AddFileToZip(zf, strRelativePath.c_str(), NULL); //在zip文件中生成目录结构
			  CollectFilesInDirToZip(zf, fileFullPath.c_str(), strRelativePath); //递归收集子目录文件
			  continue;
		  }

		  AddFileToZip(zf, strRelativePath.c_str(), fileFullPath.c_str()); //将文件添加到zip文件中
	  }
  }
}


//最终接口：从某个目录创建zip文件
void MakeDirToZip::CreateZipFromDir(const std::string& dirName, const std::string& zipFileName)
{
  zipFile newZipFile = zipOpen(zipFileName.c_str(), APPEND_STATUS_CREATE); //创建zip文件
  if (newZipFile == NULL)
  {
    printf("无法创建zip文件!");
    return;
  }
  
  CollectFilesInDirToZip(newZipFile, dirName, "");
  zipClose(newZipFile, NULL); //关闭zip文件
}

void MakeDirToZip::UncompressFile( const std::string& fileName )
{
	char path_buffer[_MAX_PATH] = {0};
	char drive[_MAX_DRIVE] = {0};
	char dir[_MAX_DIR] = {0};
	char fname[_MAX_FNAME] = {0};
	char ext[_MAX_EXT] = {0};
	_splitpath(fileName.c_str(), drive, dir, fname, ext);

	std::string strCurrentPath = std::string(drive) + std::string(dir) 
		+ std::string(fname);
	
	mkdir(strCurrentPath.c_str());

	strCurrentPath += std::string("\\");

	int buffSize = 1024 * 1024 * 10;
	char* pBuffer = new char[buffSize];
	unzFile uzf =  unzOpen(fileName.c_str());

	if(unzGoToFirstFile(uzf) != UNZ_OK) return;

	
	do{
		char fileName[1024] = {0};
		unz_file_info ufi;
		unzGetCurrentFileInfo(uzf, &ufi, fileName, 1024, NULL, 0, NULL, 0);
		
		std::string strNewFilePathName = strCurrentPath + std::string(fileName);

		if(fileName[ufi.size_filename - 1] == '/' || fileName[ufi.size_filename - 1] == '\\' )
		{
			mkdir(strNewFilePathName.c_str());
			continue;
		}

		FILE* newFile = fopen(strNewFilePathName.c_str(), "wb");
		if(unzOpenCurrentFile(uzf) == UNZ_OK)
		{
			memset(pBuffer, 0, buffSize);
			int iReadLen = unzReadCurrentFile(uzf, pBuffer, buffSize);

			fwrite(pBuffer, iReadLen, 1, newFile);
		}
		fclose(newFile);
	}
	while(unzGoToNextFile(uzf) == UNZ_OK);

	delete[] pBuffer;

	unzClose(uzf);
}

