#pragma once

class Sender : public ACE_Handler
{
public:
	Sender (void);
	~Sender (void);

	//FUZZ: disable check_for_lack_ACE_OS
	///FUZZ: enable check_for_lack_ACE_OS
	int Open();
	int Open (const ACE_TCHAR *host, u_short port);
	int asynch_send(ACE_Message_Block* pMB);
	int asynch_send(ACE_INET_Addr& addr, ACE_Message_Block* pMB);
	int clean();
protected:
	// These methods are called by the freamwork

	/// This is called when asynchronous writes from the dgram socket
	/// complete
	virtual void handle_write_dgram (const ACE_Asynch_Write_Dgram::Result &result);

private:
	ACE_Proactor* m_pProactor;

	/// Network I/O handle
	ACE_SOCK_Dgram sock_dgram_;

	/// wd (write dgram): for writing to the socket
	ACE_Asynch_Write_Dgram wd_;

	const char* completion_key_;
	const char* act_;

	ACE_INET_Addr m_ReceiverAddr;
};


