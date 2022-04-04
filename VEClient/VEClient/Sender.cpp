#include "StdAfx.h"
#include "Sender.h"

Sender::Sender (void)
	: completion_key_ ("Sender completion key")
	, act_ ("Sender ACT")

{
	m_pProactor = new ACE_Proactor(new ACE_WIN32_Proactor, true);
}

Sender::~Sender (void)
{
	this->sock_dgram_.close ();
}

int Sender::Open (const ACE_TCHAR *host,
	u_short port)
{
	// Initialize stuff
	m_ReceiverAddr.set(port, host);

	if (this->sock_dgram_.open (ACE_INET_Addr::sap_any) == -1)
		ACE_ERROR_RETURN ((LM_ERROR,
		"%p\n",
		"ACE_SOCK_Dgram::open"), -1);

	// Initialize the asynchronous read.
	if (this->wd_.open (*this, this->sock_dgram_.get_handle ()
		, this->completion_key_
		, m_pProactor/*ACE_Proactor::instance ()*/) == -1)
	{

		ACE_ERROR_RETURN ((LM_ERROR,
			"%p\n",
			"ACE_Asynch_Write_Dgram::open"), -1);
	}

	return 1;
}

int Sender::asynch_send(ACE_Message_Block* msg)
{
	// do the asynch send
	size_t number_of_bytes_sent = 0;

	int res = this->wd_.send (msg, number_of_bytes_sent, 0, m_ReceiverAddr, this->act_);

	switch (res)
	{
	case 0:
		// this is a good error.  The proactor will call our handler when the
		// send has completed.
		break;
	case 1:
		// actually sent something, we will handle it in the handler callback
		/*ACE_DEBUG ((LM_DEBUG, "********************\n"));
		ACE_DEBUG ((LM_DEBUG,
		"%s = %d\n",
		"bytes sent immediately",
		number_of_bytes_sent));
		ACE_DEBUG ((LM_DEBUG, "********************\n"));*/
		res = 0;
		break;
	case -1:
		// Something else went wrong.
		ACE_ERROR ((LM_ERROR,
			"%p\n",
			"ACE_Asynch_Write_Dgram::recv"));
		// the handler will not get called in this case so lets clean up our msg
		msg->release ();
		break;
	default:
		// Something undocumented really went wrong.
		ACE_ERROR ((LM_ERROR,
			"%p\n",
			"ACE_Asynch_Write_Dgram::recv"));
		msg->release ();
		break;
	}

	m_pProactor->handle_events();
	return res;
}

int Sender::asynch_send(ACE_Message_Block* msg, ACE_INET_Addr& addr)
{
	// do the asynch send
	size_t number_of_bytes_sent = 0;

	int res = this->wd_.send (msg, number_of_bytes_sent, 0, addr, this->act_);

	switch (res)
	{
	case 0:
		// this is a good error.  The proactor will call our handler when the
		// send has completed.
		break;
	case 1:
		// actually sent something, we will handle it in the handler callback
		/*ACE_DEBUG ((LM_DEBUG, "********************\n"));
		ACE_DEBUG ((LM_DEBUG,
		"%s = %d\n",
		"bytes sent immediately",
		number_of_bytes_sent));
		ACE_DEBUG ((LM_DEBUG, "********************\n"));*/
		res = 0;
		break;
	case -1:
		// Something else went wrong.
		ACE_ERROR ((LM_ERROR,
			"%p\n",
			"ACE_Asynch_Write_Dgram::recv"));
		// the handler will not get called in this case so lets clean up our msg
		msg->release ();
		break;
	default:
		// Something undocumented really went wrong.
		ACE_ERROR ((LM_ERROR,
			"%p\n",
			"ACE_Asynch_Write_Dgram::recv"));
		msg->release ();
		break;
	}

	m_pProactor->handle_events();
	return res;
}

int Sender::asynch_send( ACE_INET_Addr* pAddr, ACE_Message_Block* pMB )
{
	m_ReceiverAddr = *pAddr;

	asynch_send(pMB);

	return 1;
}

void Sender::handle_write_dgram (const ACE_Asynch_Write_Dgram::Result &result)
{
	result.message_block ()->release ();
}

int Sender::clean()
{
	wd_.cancel();
	sock_dgram_.close();
	m_pProactor->close();
	return 1;
}
