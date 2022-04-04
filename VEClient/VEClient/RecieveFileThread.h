#pragma once
#include <map>
#include "FileBlockMirror.h"
#include "MessageProtocol.h"

class WriteFileThread;
class Receiver;

class RecieveFileThread :
	public ACE_Task<ACE_MT_SYNCH>
{
public:
	RecieveFileThread(ACE_INET_Addr* pAddr);
	~RecieveFileThread(void);
	virtual int svc( void );
	inline WriteFileThread* GetWriteThread(){return m_pWriteFileThread;}

private:
	Receiver* m_pReceiver;
	WriteFileThread* m_pWriteFileThread;
};

