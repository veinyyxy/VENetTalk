#pragma once
#include "stdafx.h"

#pragma pack(push, 1)

struct message_header
{
	int _id;
	int _totalnum;
	int _ordernum;
	int _type;
	unsigned _user_id;
	char _address[100];
	char _username[255];
	char _time[20];
	unsigned _body_size;
};

struct heartbeat
{
	int _state;
	unsigned long long _beat_num;
};

///////////////////////////////user list//////////////////////////////////
struct user_list
{
	int id;
	int orgid;//��֯�ṹ�е�λ��
	int groupid;//�Զ������
	char _username[255];
};
///////////////////////organization structure////////////////////////////
struct node_data
{
	int levelid;//�㼶
	int nodenum;//�ڵ�ı��
	int parentnum;//���ڵ���
	char nodename[255];
	char nodetype;
};

struct org_struct
{
	int nodecount;
	node_data* nodes;
};
///////////////////////////login////////////////////////////
struct login
{
	char _username[255];
	char _password[20];
};

struct login_reply
{
	int state;

	int user_num;
	user_list* pUserBuff;
};
////////////////////////////////////////////////////////////
struct send_message
{
	char username[255];
	int type;
	int length;
	char pBuffer[1024];
};

struct send_message_reply
{
	int state;
};
//////////////////////////////////////////////////////////////////////////
struct logout
{
	int id;
	char username[255];
};

struct logout_reply
{
	int state;
};
//////////////////////////////////////////////////////////////////////////
struct user_state
{
	int user_id;
	char _username[255];
	int state;
};

struct user_state_list
{
	int num;
	unsigned long long list_start;
};

struct user_state_request
{
	int _requestid;
	char _requestname[255];
};
//////////////////////////////////////////////////////////////////////////
struct set_user_state
{
	int user_id;
	char _username[255];
	int state;
};

struct set_user_state_reply
{
	int iType;
	int iState;
};
//////////////////////////////////////////////////////////////////////////
struct send_file_block//�ļ�����飬ֱ�ӷ��͸��ͻ���
{
	char uuid[37];
	int userid;
	char username[255];//Դ�ļ������û���ID
	char src_file_path[2048];//�ļ�Դ��λ��
	int block_id;//�������
	int file_block_size;//�ļ���С
	char type;
};

struct send_file_block_addition//����鸽����Ϣ�����͸���������
{
	char uuid[37];
	char username[255];
	int block_id;
	int send_state;
	int reciver_state;
	char type;
};

struct send_file_reqest
{
	char uuid[37];
	int userid;
	char username[255];//Դ�ļ������û���ID
	int file_size;//�ļ���С
	int block_count;//�ļ�������
	char src_file_path[2048];//�ļ�Դ��λ��
	int file_block_size;
	int state;
	char type;
	//char src_file_name[1024];//�ļ���
};

struct send_file_request_reply
{
	char uuid[37];
	int userid;
	char username[255];
	char useraddr[100];//Ŀ���ַ
	char dst_file_path[2048];//�ļ�Դ��λ��
	int reply_state;
	char type;
};
/////////////////////////////�Զ������//////////////////////////////////
/************************************************************************/
/*                    ���������ͻ��˵���������Ϣ���������ڽڵ�                */
/************************************************************************/
struct custom_group_info
{
	char type;
	int groupid;
	char groupname[255];
	int nodecout;
	int* nodeid;
};

struct custom_groups
{
	int cout;
	custom_group_info* groupinfo;
};


/************************************************************************/
/*                         �������Ϣ��������ɾ���Ĳ���                     */
/************************************************************************/
struct modify_custom_group
{
	char operate;// 0��ӣ�1ɾ�� 2����
	custom_group_info groupinfo;
};

struct modify_custom_group_node
{
	char operate;// 0��ӣ�1ɾ����2����
	int groupid;
	int nodecout;
	int* nodeid;
};

struct custom_group_reply
{
	char state;
	int groupid;
};
///////////////////////////�ڵ�ת�˺�////////////////////////////////
struct nodeid_to_account
{
	int count;
	int nodeid[50];
	char username[255 * 50];
};
////////////////////////////������Ϣ//////////////////////////////
struct user_information
{
	char direct;
	char name[24];
	char mobile[19];
	char phone[15];
	char email[100];
	char address[501];
	char qq[18];
	char icon[1 + 1024 * 32];
	int length;
};
////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)