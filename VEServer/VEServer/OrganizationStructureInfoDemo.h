#pragma once
#include "organizationstructureinfo.h"
class OrganizationStructureInfoDemo :
	public OrganizationStructureInfo
{
public:
	OrganizationStructureInfoDemo(void);
	virtual ~OrganizationStructureInfoDemo(void);

	virtual int GetWholeOrganization(std::vector<ORG_NODE>* pNodes);
	virtual int GetDepatment(int iID, std::vector<ORG_NODE>* pNodes);
};

