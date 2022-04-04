#pragma once
#include <map>
#include <string>
#include "OrganizationStructureInfo.h"

class UserInformation
{
public:
	struct UserInfo
	{
		int id;
		std::string name;
		std::string pass;
		std::string user_address;
		unsigned int heartbeat_number;
		int user_state;
		int heartbeat_state;
	};

	typedef std::map< std::string, UserInfo> USER_LIST, *P_USER_LIST;
	//自定义组节点
	typedef std::vector<int> NODES_ID;
	struct CustomGroup
	{
		int type;
		int groupid;
		std::string name;
		NODES_ID nodes_id;
	};

	typedef struct __UserInformation________________
	{
		std::string name;
		std::string  mobile;
		std::string phone;
		std::string email;
		std::string address;
		std::string qq;
		char icon[1 + 1024 * 32];
		int length;
	} USER_DETAIL_INFO, *P_USER_DETAIL_INFO;

	typedef std::vector<CustomGroup> CUSTOM_GROUPS, *P_CUSTOM_GROUPS;
	typedef std::map<std::string, CUSTOM_GROUPS> NAME_MAP_CUSTOM_GROUPS
		, *P_NAME_MAP_CUSTOM_GROUPS;

public:
	virtual int IsRightfulUser(const std::string& strUser, const std::string& pass) = 0;
	virtual int GetUserList(P_USER_LIST pUL) = 0;
	virtual int SetUserAddress(const std::string& strUser, const std::string& strAddr) = 0;
	virtual int GetUserAddress(const std::string& strUser, std::string& strAddr) = 0;

	virtual int SetUserHeartbeat( const std::string& strUser, const unsigned int& beatNum) = 0;
	virtual int GetUserHeartbeat(const std::string& strUser, unsigned int& beatNum) = 0;

	virtual int SetUserState(const std::string& strUser, int iS) = 0;
	virtual int GetUserState(const std::string& strUser, int& iS) = 0;

	virtual int SetHeartState(const std::string& strUser, int iS) = 0;
	virtual int GetHeartState(const std::string& strUser, int& iS) = 0;

	//virtual int GetCustomGroup(const std::string& strUser, std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes) = 0;
	//virtual int SetCustomGroup(const std::string& strUser, const std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes) = 0;
	
	//virtual int GetFrequentContact(const std::string& strUser, std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes) = 0;
	//virtual int GetFrequentContact(int iID, std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes) = 0;
	//virtual int SaveFrequentContact(const std::string& strUser, const std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes) = 0;

	virtual int GetCustomGroup(const std::string& strUser, CUSTOM_GROUPS& groups) = 0;
	virtual int AddCustomGroup(const std::string& strUser, const CustomGroup& group, int& iGroupID) = 0;
	virtual int UpdCustomGroup(const std::string& strUser, const CustomGroup& group, int iGroupID) = 0;
	virtual int DelCustomGroup(const std::string& strUser, int iGroupID) = 0;
	
	virtual int AddNodeToGroup(const std::string& strUser, int iGroupID, int iNodeID) = 0;
	virtual int DelNodeFromGroup(const std::string& strUser, int iGroupID, int iNodeID) = 0;

	virtual int GetAccountFromNodeID(int iNodeID, std::string& strUser) = 0;
	virtual int GetOffLineFileDir(const std::string& strUser, std::string& strDir) = 0;

	virtual int GetUserInformation(const std::string& strAccount, P_USER_DETAIL_INFO info, int direct) = 0;
};

