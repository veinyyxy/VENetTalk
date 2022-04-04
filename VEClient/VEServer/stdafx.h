// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <ace/ACE.h>
#include <ace/OS_main.h>
#include <ace/SOCK_Acceptor.h>
#include <ace/Thread_Manager.h>
#include <ace/Profile_Timer.h>
#include <ace/Basic_Types.h>
#include <ace/OS_NS_sys_select.h>
#include <ace/ace_wchar.h>
#include <ace/OS_NS_stdlib.h>
#include <ace/OS_NS_unistd.h>
#include <ace/Asynch_IO.h>
#include <ace/SOCK_Dgram.h>
#include <ace/Message_Block.h>
#include <ace/Proactor.h>
#include <ace/Handle_Set.h>
#include <ace/Task.h>
#include <ace/Mutex.h>
#include <map>

#ifdef DEBUG
	#define THROW_ERROR /
	__asm{/
		int 3/
	};
#else
	#define THROW_ERROR
#endif


#define ACE_MESSAGE_BLOCK_SIZE 1024 * 1024
// TODO: reference additional headers your program requires here
