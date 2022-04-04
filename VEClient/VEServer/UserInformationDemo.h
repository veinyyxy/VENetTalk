#pragma once
#include "userinformation.h"
class UserInformationDemo :
	public UserInformation
{
public:
	typedef std::vector<OrganizationStructureInfo::ORG_NODE> USER_NODE;
	typedef std::map<std::string, USER_NODE> USER_NODE_INFO;
	typedef std::map<std::string, USER_DETAIL_INFO> USER_INFORMATION_MAP;
	UserInformationDemo(void);
	virtual ~UserInformationDemo(void);

	virtual int IsRightfulUser( const std::string& strUser, const std::string& pass );

	virtual int GetUserList( P_USER_LIST pUL );

	virtual int SetUserAddress( const std::string& strUser, const std::string& strAddr );
	virtual int GetUserAddress( const std::string& strUser, std::string& strAddr );

	virtual int SetUserHeartbeat( const std::string& strUser, const unsigned int& beatNum );
	virtual int GetUserHeartbeat( const std::string& strUser, unsigned int& beatNum );

	virtual int SetUserState( const std::string& strUser, int iS );
	virtual int GetUserState( const std::string& strUser, int& iS );

	virtual int SetHeartState( const std::string& strUser, int iS );
	virtual int GetHeartState( const std::string& strUser, int& iS );

	virtual int GetCustomGroup( const std::string& strUser, CUSTOM_GROUPS& groups );

	virtual int AddCustomGroup( const std::string& strUser, const CustomGroup& group, int& iGroupID );

	virtual int DelCustomGroup( const std::string& strUser, int iGroupID );

	virtual int AddNodeToGroup( const std::string& strUser, int iGroupID, int iNodeID );

	virtual int DelNodeFromGroup( const std::string& strUser, int iGroupID, int iNodeID );

	virtual int UpdCustomGroup( const std::string& strUser, const CustomGroup& group, int iGroupID );

	virtual int GetAccountFromNodeID( int iNodeID, std::string& strUser );

	virtual int GetOffLineFileDir( const std::string& strUser, std::string& strDir );

	virtual int GetUserInformation( const std::string& strAccount, P_USER_DETAIL_INFO info, int direct );


	/*virtual int GetCustomGroup( const std::string& strUser, std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes );
	virtual int SetCustomGroup(const std::string& strUser, const std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes);

	virtual int GetFrequentContact( const std::string& strUser, std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes );

	virtual int GetFrequentContact( int iID, std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes );

	virtual int SaveFrequentContact( const std::string& strUser, const std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes );
*/
private:
	USER_LIST UsersMap;
	//USER_NODE_INFO m_GrequentContact;
	//USER_NODE_INFO m_CustomGroup;
	ACE_Thread_Mutex m_Mutex;
	ACE_Thread_Mutex m_UserInfoMutex;
	UserInformationDemo::NAME_MAP_CUSTOM_GROUPS m_CustomGroups;
	USER_INFORMATION_MAP m_pUserInfoMap;

};

