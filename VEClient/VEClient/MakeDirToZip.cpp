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

//���սӿڣ���ĳ��Ŀ¼����zip�ļ�
//void MakeDirToZip::CreateZipFromDir(const std::string& dirName, const std::string& zipFileName);


//���ļ���ӵ�zip�ļ��У�ע�����Դ�ļ�srcFileΪ������ӿ�Ŀ¼
//fileNameInZip: ��zip�ļ��е��ļ������������·��
void MakeDirToZip::AddFileToZip(zipFile zf, const char* fileNameInZip, const char* srcFile)
{
  FILE* srcfp = NULL;

  //��ʼ��д��zip���ļ���Ϣ
  zip_fileinfo zi;
  zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =
  zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
  zi.dosDate = 0;
  zi.internal_fa = 0;
  zi.external_fa = 0;

  //���srcFileΪ�գ������Ŀ¼
  char new_file_name[MAX_PATH];
  memset(new_file_name, 0, sizeof(new_file_name));
  strcat_s(new_file_name, fileNameInZip);
  if (srcFile == NULL)
  {
      strcat_s(new_file_name, "/");
  }
  
  //��zip�ļ��д������ļ�
  zipOpenNewFileInZip(zf, new_file_name, &zi, NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);

  if (srcFile != NULL)
  {
      //��Դ�ļ�
      srcfp = fopen(srcFile, "rb");
      if (srcfp == NULL)
      {
          //MessageBox(_T("�޷�����ļ�") + std::string(srcFile) + _T("!"));
          zipCloseFileInZip(zf); //�ر�zip�ļ�
          return;
      }

      //����Դ�ļ���д��zip�ļ�
      char buf[100*1024]; //buffer
      int numBytes = 0;
      while( !feof(srcfp) )
      {
          numBytes = fread(buf, 1, sizeof(buf), srcfp);
          zipWriteInFileInZip(zf, buf, numBytes);
          if( ferror(srcfp) )
              break;
      }

      //�ر�Դ�ļ�
      fclose(srcfp);
  }

  //�ر�zip�ļ�
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

//�ݹ������Ŀ¼��zip�ļ�
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

			  strRelativePath = parentDir + std::string("\\") + std::string(directory->d_name); //������zip�ļ��е����·��
		  }

		  if(IsDir(fileFullPath.c_str()) == 0)
		  {
			  AddFileToZip(zf, strRelativePath.c_str(), NULL); //��zip�ļ�������Ŀ¼�ṹ
			  CollectFilesInDirToZip(zf, fileFullPath.c_str(), strRelativePath); //�ݹ��ռ���Ŀ¼�ļ�
			  continue;
		  }

		  AddFileToZip(zf, strRelativePath.c_str(), fileFullPath.c_str()); //���ļ���ӵ�zip�ļ���
	  }
  }
}


//���սӿڣ���ĳ��Ŀ¼����zip�ļ�
void MakeDirToZip::CreateZipFromDir(const std::string& dirName, const std::string& zipFileName)
{
  zipFile newZipFile = zipOpen(zipFileName.c_str(), APPEND_STATUS_CREATE); //����zip�ļ�
  if (newZipFile == NULL)
  {
    printf("�޷�����zip�ļ�!");
    return;
  }
  
  CollectFilesInDirToZip(newZipFile, dirName, "");
  zipClose(newZipFile, NULL); //�ر�zip�ļ�
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

