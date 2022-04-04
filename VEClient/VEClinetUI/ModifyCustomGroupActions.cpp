#include "StdAfx.h"
#include "ModifyCustomGroupActions.h"
#include "ModifyGroupNodeDialog.h"
#include "Client.h"

extern Client* g_pClient;
extern void addItem(QStandardItem* pSourceItem, QStandardItem* pDestItem);
extern void addChildItem(QStandardItem* pSourceItem, QStandardItem* pDestItem);

ADD_MYACTION_CONTSTRUCTED_DECLARATION(AddGroupAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(UptGroupAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DelGroupAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(AddPeopleGroupAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(ModifyGroupContentAction)

void AddPeopleGroupAction::MyAction()
{
	QString strGroupName  = QInputDialog::getText(
		m_pTreeView, QString::fromLocal8Bit("添加自定义联系人组")
		, QString::fromLocal8Bit("组名称:"));

	QStandardItemModel* pSIM = (QStandardItemModel*)(m_pTreeView->model());
	QStandardItem* pItem = new QStandardItem(QIcon("./image/onlin.ico"), strGroupName);
	
	int iRowcount = pSIM->rowCount();

	NODE_PROPERTY nodeProperty;
	nodeProperty.insert("type", 1);
	nodeProperty.insert("id", iRowcount);
	nodeProperty.insert("not change", 1);

	pItem->setData(nodeProperty);

	pSIM->setItem(iRowcount, pItem);

	g_pClient->ModifyGroup(0, iRowcount, 1, strGroupName.toStdString());
}

void AddPeopleGroupAction::InitMyAction()
{
	m_pTreeView = 0;
}

void AddGroupAction::MyAction()
{
	QString strGroupName  = QInputDialog::getText(m_pTreeView, QString::fromLocal8Bit("添加自定义部门组")
		, QString::fromLocal8Bit("名称:"));

	if(strGroupName.length() == 0) return;
	QStandardItemModel* pSIM = (QStandardItemModel*)(m_pTreeView->model());
	QStandardItem* pItem = new QStandardItem(QIcon("./image/department.ico"), strGroupName);

	int iRowcount = pSIM->rowCount();
	NODE_PROPERTY nodeProperty;
	nodeProperty.insert("type", 0);
	nodeProperty.insert("id", iRowcount);
	nodeProperty.insert("not change", 1);

	pItem->setData(nodeProperty);

	pSIM->setItem(iRowcount, pItem);

	g_pClient->ModifyGroup(0, iRowcount, 0, strGroupName.toStdString());
}

void AddGroupAction::InitMyAction()
{
	m_pTreeView = 0;
}
//////////////////////////////////////////////////////////////////////////
void UptGroupAction::MyAction()
{

}

void UptGroupAction::InitMyAction()
{
	m_pTreeView = 0;
}
//////////////////////////////////////////////////////////////////////////
void DelGroupAction::MyAction()
{
	QModelIndex sil = m_pTreeView->currentIndex();
	int iRowID = sil.row();

	QStandardItemModel* pModel = dynamic_cast<QStandardItemModel*>
		(m_pTreeView->model());

	QStandardItem* pItem = pModel->itemFromIndex(sil);

	NODE_PROPERTY nodeProperty = pItem->data().value<NODE_PROPERTY>();
	int iNodeID = nodeProperty.value("id").toInt();
	int iType = nodeProperty.value("type").toInt();

	g_pClient->ModifyGroup(1, iNodeID, iType, pItem->text().toStdString());
	pModel->removeRow(iRowID);

	
	//sil = m_pTreeView->model()->data(sil);
}

void DelGroupAction::InitMyAction()
{
	m_pTreeView = 0;
}


void ModifyGroupContentAction::MyAction()
{
	QModelIndex sil = m_pTreeView->currentIndex();
	QStandardItemModel* peopleModel 
		= dynamic_cast<QStandardItemModel*>(m_pTreeView->model());
	
	NODE_PROPERTY nodeProperty;
	int iNodeID = -1;
	int iGroupID = -1;
	if(sil.isValid())
	{
		QStandardItem* pSelectItem = peopleModel->itemFromIndex(sil);

		QStandardItemModel* pMyDestinationModel = new QStandardItemModel;
		//将选择的自定义组内容添加到对话框，进行编辑
		for(int i = 0; i < pSelectItem->rowCount(); i++)
		{
			QStandardItem* pChild = pSelectItem->child(i);
			if(!pChild) continue;

			QStandardItem* pNewItem = pChild->clone();

			addItem(pSelectItem->child(i), pNewItem);

			pMyDestinationModel->setItem(i, pNewItem);
		}

		ModifyGroupNodeDialog nodeModify(m_SourceModel, pMyDestinationModel
			, 0, m_pTreeView);
		nodeModify.exec();
		int iResult = nodeModify.result();
		if(iResult == 1)
		{
			int rowCount = pSelectItem->rowCount();
			if(rowCount)
			{
				nodeProperty = pSelectItem->data().value<NODE_PROPERTY>();
				int iGroupID = nodeProperty.value("id").toInt();
				int iType = nodeProperty.value("type").toInt();

				int* pNodeBuffer = new int[rowCount];
				memset(pNodeBuffer, 0, sizeof(int) * rowCount);

				for(int i = 0; i < rowCount; i++)
				{
					QStandardItem* pChildItem = pSelectItem->child(i);
					nodeProperty = pChildItem->data().value<NODE_PROPERTY>();
					iNodeID = nodeProperty.value("id").toInt();
					
					*(pNodeBuffer+i) = iNodeID;
				}

				g_pClient->ModifyGroupNodes(1, iGroupID, rowCount, pNodeBuffer);

				delete[] pNodeBuffer;
				pSelectItem->removeRows(0, pSelectItem->rowCount());
			}
			
			//删除原来所有的节点
			/*QVariant varValue = pSelectItem->data();
			if(!varValue.isValid()) return;

			int iGroupID = varValue.toInt();*/
			int* pGroupIDBuffer = new int[pSelectItem->rowCount()];
			memset(pGroupIDBuffer, 0, sizeof(int) * pSelectItem->rowCount());

			for(int i = 0; i < pMyDestinationModel->rowCount(); i++)
			{
				QStandardItem* pSourceItem = pMyDestinationModel->item(i);

				nodeProperty = pSourceItem->data().value<NODE_PROPERTY>();
				iNodeID = nodeProperty.value("id").toInt();

				*(pGroupIDBuffer + i) = iNodeID;
				QStandardItem* pNewItem = pSourceItem->clone();

				addItem(pSourceItem, pNewItem);

				pSelectItem->setChild(pSelectItem->rowCount(), pNewItem);
			}

			int iGroupID = sil.row();

			g_pClient->ModifyGroupNodes(0, iGroupID
				, pSelectItem->rowCount(), pGroupIDBuffer);
		}
	}
}

void ModifyGroupContentAction::InitMyAction()
{
	m_pTreeView = 0;
}
