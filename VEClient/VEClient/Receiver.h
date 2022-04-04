#pragma once

class ReceiverCallback
{
public:
	virtual void ReciverComplete(ACE_Message_Block* pMB) = 0;
};

class Receiver : public ACE_Service_Handler
{
public:
	// = Initialization and termination.
	Receiver (void);
	~Receiver (void);

	int open_addr (const ACE_INET_Addr &localAddr);
	int asynch_receive(ACE_Message_Block* pAMB);
	int asynch_receive(ACE_Message_Block* pAMB, int blockTime);
	int close_recever();
	inline bool getResult(){return m_bReceived;}
	inline ACE_Message_Block* getReceivedMessage(){return m_pMessageBlock;}
protected:
	// These methods are called by the framework

	/// This method will be called when an asynchronous read completes on
	/// a UDP socket.
	virtual void handle_read_dgram (const ACE_Asynch_Read_Dgram::Result &result);

private:
	ACE_Message_Block* m_pMessageBlock;
	ACE_Message_Block* m_pAdditionalMessage;
	ACE_Proactor* m_pProactor;
	ACE_SOCK_Dgram sock_dgram_;

	/// rd (read dgram): for reading from a UDP socket.
	ACE_Asynch_Read_Dgram rd_;
	const char* completion_key_;
	const char* act_;
	bool m_bReceived;
};


