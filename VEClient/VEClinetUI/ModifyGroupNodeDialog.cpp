#include "StdAfx.h"
#include "ModifyGroupNodeDialog.h"

void addItem(QStandardItem* pSourceItem, QStandardItem* pDestItem)
{
	if(pSourceItem->hasChildren())
	{
		int rowCount = pSourceItem->rowCount();
		for(int i = 0; i < rowCount; i++)
		{
			QStandardItem* pChildItem = pSourceItem->child(i);
			QStandardItem* pCloneItem = pChildItem->clone();

			pDestItem->setChild(i, pCloneItem);

			addItem(pChildItem, pCloneItem);
		}
	}
	else
	{
		return;
	}
}

void addChildItem(QStandardItem* pSourceItem, QStandardItem* pDestItem)
{
	if(pSourceItem->hasChildren())
	{
		int rowCount = pSourceItem->rowCount();
		for(int i = 0; i < rowCount; i++)
		{
			QStandardItem* pChildItem = pSourceItem->child(i);
			addChildItem(pChildItem, pDestItem);
		}
	}
	else
	{
		NODE_PROPERTY nodeProperty;
		nodeProperty = pSourceItem->data().value<NODE_PROPERTY>();
		int iType = nodeProperty.value("type").toInt();
		if(iType == 0)
			return;

		QStandardItem* pLeaf = pSourceItem->clone();
		pDestItem->setChild(pDestItem->rowCount(), pLeaf);
		return;
	}
}

ModifyGroupNodeDialog::ModifyGroupNodeDialog(QStandardItemModel* pS
	, QStandardItemModel* pD, int iNodeID, QWidget* pParent)
	: QDialog(pParent)
	, m_SourceModel(pS)
	, m_DestinationModel(pD)
{
	m_MyWidget.setupUi(this);

	
	m_MyWidget.treeView->setModel(new QStandardItemModel);

	m_MyWidget.radioButton->setChecked(true);
	FillSourceTreeView();

	//QStandardItemModel* sim = new QStandardItemModel;
	
	m_MyWidget.treeView_2->setModel(m_DestinationModel);

	m_ButtonGroup = new QButtonGroup(this);
	m_ButtonGroup->addButton(m_MyWidget.AddButton);
	m_ButtonGroup->addButton(m_MyWidget.DelButton);
	m_ButtonGroup->addButton(m_MyWidget.FindButton);
	m_ButtonGroup->addButton(m_MyWidget.CancelButton);
	m_ButtonGroup->addButton(m_MyWidget.OkButton);

	connect(m_ButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this
		, SLOT(slot_buttonClicked(QAbstractButton*)));
	connect(m_MyWidget.radioButton, SIGNAL(clicked(bool)), this, SLOT(slot_radioChange(bool)));  
	connect(m_MyWidget.radioButton_2, SIGNAL(clicked(bool)), this, SLOT(slot_radioChange(bool)));
}


ModifyGroupNodeDialog::~ModifyGroupNodeDialog(void)
{
}

void ModifyGroupNodeDialog::slot_buttonClicked( QAbstractButton* button )
{
	if(m_MyWidget.AddButton == button)
	{
		QModelIndex ml = m_MyWidget.treeView->currentIndex();
		QStandardItemModel* pSourceStandardModel = (QStandardItemModel*)(m_MyWidget.treeView->model());
		QStandardItem* pItem = pSourceStandardModel->itemFromIndex(ml);
		if(!pItem) return;

		if(!(pItem->parent()))
			return;

		QStandardItem* pNewItem = pItem->clone();
		QStandardItemModel* pDestStandardModel = 
			(QStandardItemModel*)m_MyWidget.treeView_2->model();

		if(pItem->hasChildren())
		{
			addItem(pItem, pNewItem);
		}

		NODE_PROPERTY nodeProperty = pNewItem->data().value<NODE_PROPERTY>();
		int iNodeID = nodeProperty.value("id").toInt();

		QList<QStandardItem*> pL = pDestStandardModel->findItems(pNewItem->text());
		if(pL.length() != 0)
			return;

		pDestStandardModel->setItem(pDestStandardModel->rowCount()
			, pNewItem);
	}
	else if(m_MyWidget.DelButton == button)
	{
		QModelIndex sil = m_MyWidget.treeView_2->currentIndex();
		int iRowID = sil.row();
		m_MyWidget.treeView_2->model()->removeRow(iRowID);
	}
	else if(m_MyWidget.CancelButton == button)
	{
		this->close();
	}
	else if(m_MyWidget.FindButton == button)
	{
		QStandardItemModel* pSourceStandardModel 
			= (QStandardItemModel*)(m_MyWidget.treeView->model());

		QString strText = m_MyWidget.lineEdit->text();
		QList<QStandardItem*> findedList 
			= pSourceStandardModel->findItems(strText, Qt::MatchContains);

		QList<QStandardItem*> newList;
		for(int i = 0; i < findedList.size(); i++)
		{
			QStandardItem* pItem = findedList.at(i);
			QStandardItem* pCloneItem = pItem->clone();
			if(pItem->hasChildren())
			{
				addItem(pItem, pCloneItem);
			}

			newList.push_back(pCloneItem);
		}

		if(findedList.size() > 0)
		{
			pSourceStandardModel->clear();

			for(int i = 0; i < newList.size(); i++)
			{
				pSourceStandardModel->setItem(i, newList.at(i));
			}
		}
		else
		{
			slot_radioChange(true);
		}

	}
	else if(m_MyWidget.OkButton == button)
	{
		this->accept();
	}
}

void ModifyGroupNodeDialog::slot_radioChange(bool)
{
	if(sender() == m_MyWidget.radioButton)
	{
		FillSourceTreeView();
	}
	else if(sender() == m_MyWidget.radioButton_2)
	{
		FillSourceTreeView(1);
	}
	else
	{

	}
}

void ModifyGroupNodeDialog::FillSourceTreeView(int iType)
{
	m_MyWidget.treeView->model()->removeRows(0, m_MyWidget.treeView->model()->rowCount());
	for(int i = 0; i < m_SourceModel->rowCount(); i++)
	{
		QStandardItem * pItem = m_SourceModel->item(i);

		/*NODE_PROPERTY nodeProperty;
		nodeProperty = pItem->data().value<NODE_PROPERTY>();
		int iNodeID = nodeProperty.value("id").toInt();*/

		QStandardItem *pNewItem = pItem->clone();

		/*nodeProperty = pNewItem->data().value<NODE_PROPERTY>();
		iNodeID = nodeProperty.value("id").toInt();*/

		switch(iType)
		{
		case 0:
			addItem(pItem, pNewItem);
			break;
		case 1:
			addChildItem(pItem, pNewItem);
			break;
		}
		

		QStandardItemModel* sourceModel 
			= (QStandardItemModel*)(m_MyWidget.treeView->model());

		sourceModel->setItem(i, pNewItem);
	}
}

