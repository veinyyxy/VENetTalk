#include "StdAfx.h"
#include "Receiver.h"

Receiver::Receiver (void)
	: completion_key_ ("Receiver Completion Key")
	, act_ ("Receiver ACT")
	, m_pMessageBlock(0)
{
	m_pProactor = new ACE_Proactor(new ACE_WIN32_Proactor);
	m_pAdditionalMessage = new ACE_Message_Block(512);
}

Receiver::~Receiver (void)
{
	sock_dgram_.close ();
}

int Receiver::open_addr (const ACE_INET_Addr &localAddr)
{
	int iPort = localAddr.get_port_number();
	std::string strAddr = localAddr.get_host_addr();
	ACE_DEBUG ((LM_DEBUG,
		"%N:%l:Receiver::open_addr called\n"));

	// Create a local UDP socket to receive datagrams.
	if (this->sock_dgram_.open (localAddr) == -1)
		ACE_ERROR_RETURN ((LM_ERROR,
		"%p\n",
		"ACE_SOCK_Dgram::open"), -1);

	// Initialize the asynchronous read.
	if (this->rd_.open (*this,
		this->sock_dgram_.get_handle (),
		this->completion_key_,
		/*ACE_Proactor::instance ()*/m_pProactor) == -1)
		ACE_ERROR_RETURN ((LM_ERROR,
		"%p\n",
		"ACE_Asynch_Read_Dgram::open"), -1);

	return 1;
}

int Receiver::asynch_receive(ACE_Message_Block* pAMB)
{
	// ok lets do the asynch read
	size_t number_of_bytes_recvd = 0;

	int res = rd_.recv (pAMB,
		number_of_bytes_recvd,
		0,
		PF_INET,
		this->act_);


	switch (res)
	{
	case 0:
		m_pMessageBlock = pAMB;
		// this is a good error.  The proactor will call our handler when the
		// read has completed.
		break;
	case 1:
		// actually read something, we will handle it in the handler callback
		ACE_DEBUG ((LM_DEBUG, "********************\n"));
		ACE_DEBUG ((LM_DEBUG,
			"%s = %d\n",
			"bytes recieved immediately",
			number_of_bytes_recvd));
		ACE_DEBUG ((LM_DEBUG, "********************\n"));
		m_pMessageBlock = pAMB;
		res = 0;
		break;
	case -1:
		// Something else went wrong.
		ACE_ERROR ((LM_ERROR,
			"%p\n",
			"ACE_Asynch_Read_Dgram::recv"));
		// the handler will not get called in this case so lets clean up our msg
		//msg->release ();
		break;
	default:
		// Something undocumented really went wrong.
		ACE_ERROR ((LM_ERROR,
			"%p\n",
			"ACE_Asynch_Read_Dgram::recv"));
		//msg->release ();
		break;
	}

	m_pProactor->handle_events();
	return res;
}

int Receiver::asynch_receive( ACE_Message_Block* pAMB, int blockTime )
{
	// ok lets do the asynch read
	size_t number_of_bytes_recvd = 0;

	m_bReceived = false;

	int res = rd_.recv (pAMB,
		number_of_bytes_recvd,
		0,
		PF_INET,
		this->act_);


	switch (res)
	{
	case 0:
		m_pMessageBlock = pAMB;
		// this is a good error.  The proactor will call our handler when the
		// read has completed.
		break;
	case 1:
		// actually read something, we will handle it in the handler callback
		ACE_DEBUG ((LM_DEBUG, "********************\n"));
		ACE_DEBUG ((LM_DEBUG,
			"%s = %d\n",
			"bytes recieved immediately",
			number_of_bytes_recvd));
		ACE_DEBUG ((LM_DEBUG, "********************\n"));
		m_pMessageBlock = pAMB;
		res = 0;
		break;
	case -1:
		// Something else went wrong.
		ACE_ERROR ((LM_ERROR,
			"%p\n",
			"ACE_Asynch_Read_Dgram::recv"));
		// the handler will not get called in this case so lets clean up our msg
		//msg->release ();
		break;
	default:
		// Something undocumented really went wrong.
		ACE_ERROR ((LM_ERROR,
			"%p\n",
			"ACE_Asynch_Read_Dgram::recv"));
		//msg->release ();
		break;
	}
	ACE_Time_Value timer(0, blockTime);
	m_pProactor->handle_events(timer);
	return res;
}

void Receiver::handle_read_dgram (const ACE_Asynch_Read_Dgram::Result &result)
{
	ACE_Message_Block* pMB = result.message_block();
	if(pMB == m_pMessageBlock && result.success() && pMB->size() > 0)
	{
		m_bReceived = true;
		ACE_DEBUG((LM_ERROR,
			"%p\n",
			"ACE_Asynch_Read_Dgram::recv OK!!!!!"));
		//pMB->cont(m_pAdditionalMessage);
	}
	// No need for this message block anymore.
	//result.message_block ()->release ();
}

int Receiver::close_recever()
{
	rd_.cancel();
	sock_dgram_.close();
	m_pProactor->close();
	return 1;
}
