#pragma once

class ReplyThread :
	public ACE_Task<ACE_MT_SYNCH>
{
public:
	ReplyThread(void);
	virtual ~ReplyThread(void);

	virtual int svc( void );
private:

};

