#pragma once
#include <vector>

class OrganizationStructureInfo
{
public:
	typedef struct node_info
	{
		int iLevel;
		int iNodeId;
		int iParentID;
		int iNodeType;
		std::string strName;
	} ORG_NODE, *P_ORG_NODE;

	virtual int GetWholeOrganization(std::vector<ORG_NODE>* pNodes) = 0;
	virtual int GetDepatment(int iID, std::vector<ORG_NODE>* pNodes) = 0;
	//virtual int GetStaff(int iID) = 0;
	//virtual int GetStaff(const std::string& account) = 0;
};

