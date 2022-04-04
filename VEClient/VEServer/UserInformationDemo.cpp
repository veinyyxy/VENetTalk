#include "StdAfx.h"
#include "UserInformationDemo.h"


UserInformationDemo::UserInformationDemo(void)
{
	UserInfo user1, user2, user3, user4, user5;
	user1.id = 13;
	user1.name = "lili";
	user1.pass = "123";
	user1.heartbeat_number = 0;
	user1.user_state = 0;
	user1.heartbeat_state = 0;

	user2.id = 14;
	user2.name = "wanglei";
	user2.pass = "123";
	user2.heartbeat_number = 0;
	user2.user_state = 0;
	user2.heartbeat_state = 0;

	user3.id = 15;
	user3.name = "lihua";
	user3.pass = "123";
	user3.heartbeat_number = 0;
	user3.user_state = 0;
	user3.heartbeat_state = 0;

	user4.id = 16;
	user4.name = "lele";
	user4.pass = "123";
	user4.heartbeat_number = 0;
	user4.user_state = 0;
	user4.heartbeat_state = 0;

	user5.id = 17;
	user5.name = "jack";
	user5.pass = "123";
	user5.heartbeat_number = 0;
	user5.user_state = 0;
	user5.heartbeat_state = 0;

	UsersMap[user1.name] = user1;
	UsersMap[user2.name] = user2;
	UsersMap[user3.name] = user3;
	UsersMap[user4.name] = user4;
	UsersMap[user5.name] = user5;

	UserInformationDemo::CUSTOM_GROUPS groups;
	CustomGroup group;
	group.name = "常用联系人";
	group.groupid = 0;
	group.nodes_id.push_back(14);
	group.nodes_id.push_back(15);
	group.nodes_id.push_back(16);
	group.type = 1;
	groups.push_back(group);
	m_CustomGroups["lili"] = groups;

	groups.clear();
	group.name = "常用联系人";
	group.nodes_id.clear();
	group.nodes_id.push_back(15);
	group.type = 1;
	group.groupid = 0;
	groups.push_back(group);
	m_CustomGroups["wanglei"] = groups;

	groups.clear();
	group.name = "常用联系人";
	group.nodes_id.clear();
	group.nodes_id.push_back(16);
	group.type = 1;
	group.groupid = 0;
	groups.push_back(group);
	m_CustomGroups["lihua"] = groups;

	groups.clear();
	group.name = "常用联系人";
	group.nodes_id.clear();
	group.nodes_id.push_back(17);
	group.type = 1;
	group.groupid = 0;
	groups.push_back(group);
	m_CustomGroups["lele"] = groups;

	groups.clear();
	group.name = "常用联系人";
	group.nodes_id.clear();
	group.nodes_id.push_back(14);
	group.type = 1;
	group.groupid = 0;
	groups.push_back(group);
	m_CustomGroups["jack"] = groups;
	
}

UserInformationDemo::~UserInformationDemo( void )
{

}


int UserInformationDemo::IsRightfulUser( const std::string& strUser, const std::string& pass )
{
	m_Mutex.acquire();
	USER_LIST::iterator iFind = UsersMap.find(strUser);
	USER_LIST::iterator endIter = UsersMap.end();
	if(iFind != endIter)
	{
		if(iFind->second.pass.compare(pass) == 0)
		{
			m_Mutex.release();
			return 1;
		}
	}
	m_Mutex.release();
	return 0;
}

int UserInformationDemo::GetUserList( P_USER_LIST pUL )
{
	m_Mutex.acquire();
	*pUL = UsersMap;
	m_Mutex.release();
	return 1;
}

int UserInformationDemo::SetUserAddress( const std::string& strUser, const std::string& strAddr )
{
	m_Mutex.acquire();
	USER_LIST::iterator iFind = UsersMap.find(strUser);
	if(iFind != UsersMap.end())
	{
		iFind->second.user_address = strAddr;
		m_Mutex.release();
		return 1;
	}
	else
	{
		m_Mutex.release();
		return 0;
	}
}

int UserInformationDemo::GetUserAddress( const std::string& strUser, std::string& strAddr )
{
	m_Mutex.acquire();
	USER_LIST::iterator iFind = UsersMap.find(strUser);
	if(iFind != UsersMap.end())
	{
		strAddr = iFind->second.user_address;
		m_Mutex.release();
		return 1;
	}
	else
	{
		m_Mutex.release();
		return 0;
	}
}

int UserInformationDemo::GetUserHeartbeat( const std::string& strUser, unsigned int& beatNum )
{
	m_Mutex.acquire();
	USER_LIST::iterator iFind = UsersMap.find(strUser);
	if(iFind != UsersMap.end())
	{
		beatNum = iFind->second.heartbeat_number;
		m_Mutex.release();
		return 1;
	}
	
	m_Mutex.release();
	return 0;
}

int UserInformationDemo::SetUserHeartbeat( const std::string& strUser, const unsigned int& beatNum )
{
	m_Mutex.acquire();
	USER_LIST::iterator iFind = UsersMap.find(strUser);
	if(iFind != UsersMap.end())
	{
		iFind->second.heartbeat_number = beatNum;
		m_Mutex.release();
		return 1;
	}
	
	m_Mutex.release();
	return 0;
}

int UserInformationDemo::SetUserState( const std::string& strUser, int iS )
{
	m_Mutex.acquire();
	USER_LIST::iterator iFind = UsersMap.find(strUser);
	if(iFind != UsersMap.end())
	{
		if(iS == 0)
		{
			iFind->second.user_state = iS;
			m_Mutex.release();
			return 1;
		}

		if(iFind->second.user_address.size() != 0)
		{
			iFind->second.user_state = iS;
			m_Mutex.release();
			return 1;
		}
	}

	m_Mutex.release();
	return 0;
}

int UserInformationDemo::GetUserState( const std::string& strUser, int& iS )
{
	m_Mutex.acquire();
	USER_LIST::iterator iFind = UsersMap.find(strUser);
	if(iFind != UsersMap.end())
	{
		if(iFind->second.user_address.size() != 0)
		{
			iS = iFind->second.user_state;
			m_Mutex.release();
			return 1;
		}
	}

	m_Mutex.release();
	return 0;
}

int UserInformationDemo::SetHeartState( const std::string& strUser, int iS )
{
	m_Mutex.acquire();
	USER_LIST::iterator iFind = UsersMap.find(strUser);
	if(iFind != UsersMap.end())
	{
		iFind->second.heartbeat_state = iS;
		m_Mutex.release();
		return 1;
	}

	m_Mutex.release();
	return 0;
}

int UserInformationDemo::GetHeartState( const std::string& strUser, int& iS )
{
	m_Mutex.acquire();
	USER_LIST::iterator iFind = UsersMap.find(strUser);
	if(iFind != UsersMap.end())
	{
		if(iFind->second.user_address.size() != 0)
		{
			iS = iFind->second.heartbeat_state;
			m_Mutex.release();
			return 1;
		}
	}

	m_Mutex.release();
	return 0;
}

int UserInformationDemo::GetCustomGroup( const std::string& strUser, CUSTOM_GROUPS& groups )
{
	NAME_MAP_CUSTOM_GROUPS::iterator iFinded = m_CustomGroups.find(strUser);
	if(iFinded != m_CustomGroups.end())
	{
		groups = iFinded->second;
		return 1;
	}
	else
		return 0;
}

int UserInformationDemo::AddCustomGroup( const std::string& strUser, const CustomGroup& group, int& iGroupID )
{
	NAME_MAP_CUSTOM_GROUPS::iterator iFinded = m_CustomGroups.find(strUser);
	if(iFinded != m_CustomGroups.end())
	{
		iFinded->second.push_back(group);
		iGroupID = iFinded->second.size() - 1;
	}
	else
	{
		CUSTOM_GROUPS groups;
		groups.push_back(group);

		m_CustomGroups[strUser] = groups;

		iGroupID = groups.size() - 1;
	}
	return 1;
}

int UserInformationDemo::DelCustomGroup( const std::string& strUser, int iGroupID )
{
	NAME_MAP_CUSTOM_GROUPS::iterator iFinded = m_CustomGroups.find(strUser);

	if(iFinded != m_CustomGroups.end())
	{
		CUSTOM_GROUPS::iterator cgBE = iFinded->second.begin();
		for(cgBE; cgBE != iFinded->second.end(); cgBE++)
		{
			if(cgBE->groupid == iGroupID)
			{
				iFinded->second.erase(cgBE);
				break;
			}
		}
		return 1;
	}
	else
		return 0;
}

int UserInformationDemo::AddNodeToGroup( const std::string& strUser, int iGroupID, int iNodeID )
{
	NAME_MAP_CUSTOM_GROUPS::iterator iFinded = m_CustomGroups.find(strUser);

	if(iFinded != m_CustomGroups.end())
	{
		/*if(iGroupID < ((iFinded->second).size()))
			(iFinded->second)[iGroupID].nodes_id.push_back(iNodeID);*/
		CUSTOM_GROUPS::iterator cgBE = iFinded->second.begin();
		for(cgBE; cgBE != iFinded->second.end(); cgBE++)
		{
			if(cgBE->groupid == iGroupID)
			{
				cgBE->nodes_id.push_back(iNodeID);
				break;
			}
		}
		return 1;
	}
	else
		return 0;
}

int UserInformationDemo::DelNodeFromGroup( const std::string& strUser, int iGroupID, int iNodeID )
{
	NAME_MAP_CUSTOM_GROUPS::iterator iFinded = m_CustomGroups.find(strUser);

	if(iFinded != m_CustomGroups.end())
	{
		CUSTOM_GROUPS::iterator cgBE = iFinded->second.begin();
		for(cgBE; cgBE != iFinded->second.end(); cgBE++)
		{
			if(cgBE->groupid == iGroupID)
			{
				NODES_ID::iterator beNode = cgBE->nodes_id.begin();
				for(; beNode != cgBE->nodes_id.end(); beNode++)
				{
					if(*beNode == iNodeID)
					{
						cgBE->nodes_id.erase(beNode);
						break;
					}
					
				}
				break;
			}
		}
		/*if(iGroupID < ((iFinded->second).size()))
		{
			std::vector<int>::iterator nodBegin 
				= (iFinded->second)[iGroupID].nodes_id.begin();
			int i = 0;
			while(i <= iNodeID)
			{
				nodBegin++;
				i++;
			}

			(iFinded->second)[iGroupID].nodes_id.erase(nodBegin);
		}*/
		return 1;
	}
	else
		return 0;
}

int UserInformationDemo::UpdCustomGroup( const std::string& strUser, const CustomGroup& group, int iGroupID )
{
	NAME_MAP_CUSTOM_GROUPS::iterator iFinded = m_CustomGroups.find(strUser);

	if(iFinded != m_CustomGroups.end())
	{
		/*if(iGroupID < ((iFinded->second).size()))
			iFinded->second[iGroupID] = group;*/

		CUSTOM_GROUPS::iterator cgBE = iFinded->second.begin();
		for(cgBE; cgBE != iFinded->second.end(); cgBE++)
		{
			if(cgBE->groupid == iGroupID)
			{
				cgBE->name = group.name;
				break;
			}
		}
		return 1;
	}
	else
		return 0;
}

int UserInformationDemo::GetAccountFromNodeID( int iNodeID, std::string& strUser )
{
	USER_LIST::iterator be = UsersMap.begin();
	for(be; be != UsersMap.end(); be++)
	{
		if(be->second.id == iNodeID)
		{
			strUser = be->second.name;
		}
	}
	return 1;
}

int UserInformationDemo::GetOffLineFileDir( const std::string& strUser, std::string& strDir )
{
	strDir = "D:\\offLineFile";
	return 1;
}

int UserInformationDemo::GetUserInformation( const std::string& strAccount, P_USER_DETAIL_INFO info, int direct )
{
	m_UserInfoMutex.acquire();
	USER_INFORMATION_MAP::iterator findResult = m_pUserInfoMap.find(strAccount);
	if(findResult != m_pUserInfoMap.end())
	{
		if(direct == 1)
		{
			findResult->second.name = info->name;
			findResult->second.mobile = info->mobile;
			findResult->second.phone = info->phone;
			findResult->second.email = info->email;
			findResult->second.address = info->address;
			findResult->second.qq = info->qq;
			findResult->second.length = info->length;
			memcpy_s(findResult->second.icon, sizeof(findResult->second.icon)
				, info->icon, sizeof(info->icon));
		}
		else
		{
			info->name = findResult->second.name;
			info->mobile = findResult->second.mobile;
			info->phone = findResult->second.phone;
			info->email = findResult->second.email;
			info->address = findResult->second.address;
			info->qq = findResult->second.qq;
			info->length = findResult->second.length;
			memcpy_s(info->icon, sizeof(info->icon)
				,findResult->second.icon, sizeof(findResult->second.icon));
		}
	}
	else
	{
		if(direct == 1)
		{
			USER_DETAIL_INFO ui;
			ui.name = info->name;
			ui.mobile = info->mobile;
			ui.phone = info->phone;
			ui.email = info->email;
			ui.address = info->address;
			ui.qq = info->qq;
			memcpy_s(ui.icon, sizeof(findResult->second.icon)
				, info->icon, sizeof(info->icon));
			ui.length = info->length;
			m_pUserInfoMap[strAccount] = *info;

		}
	}
	m_UserInfoMutex.release();
	return 1;
}

//int UserInformationDemo::GetCustomGroup( const std::string& strUser, std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes )
//{
//	USER_NODE_INFO::iterator iFinded = m_CustomGroup.find(strUser);
//	if(iFinded != m_CustomGroup.end())
//	{
//		*pNodes = iFinded->second;
//	}
//	return 1;
//}
//
//int UserInformationDemo::SetCustomGroup( const std::string& strUser, const std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes )
//{
//	m_CustomGroup.erase(strUser);
//	m_CustomGroup[strUser] = *pNodes;
//	return 1;
//}
//
//int UserInformationDemo::GetFrequentContact( const std::string& strUser, std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes )
//{
//	USER_NODE_INFO::iterator iFinded = m_GrequentContact.find(strUser);
//	if(iFinded != m_GrequentContact.end())
//	{
//		*pNodes = iFinded->second;
//	}
//	return 1;
//}
//
//int UserInformationDemo::GetFrequentContact( int iID, std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes )
//{
//	return 0;
//}
//
//int UserInformationDemo::SaveFrequentContact( const std::string& strUser, const std::vector<OrganizationStructureInfo::ORG_NODE>* pNodes )
//{
//	m_GrequentContact.erase(strUser);
//	m_GrequentContact[strUser] = *pNodes;
//	return 1;
//}
