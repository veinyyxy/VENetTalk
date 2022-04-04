#pragma once
#include "ui_ModefiyGroupNode.h"

void addItem(QStandardItem* pSourceItem, QStandardItem* pDestItem);
void addChildItem(QStandardItem* pSourceItem, QStandardItem* pDestItem);

class ModifyGroupNodeDialog :
	public QDialog
{
	Q_OBJECT
public:
	ModifyGroupNodeDialog(QStandardItemModel* pS, QStandardItemModel* pD
		, int iNodeID, QWidget* pParent = 0);
	~ModifyGroupNodeDialog(void);
	Ui::ModifyGroupNodeDialog m_MyWidget;
	inline void GetAddID(QVector<int>& v){v = m_addID;}
public slots:
	void slot_buttonClicked(QAbstractButton* button);
	void slot_radioChange(bool);
private:
	void FillSourceTreeView(int iType = 0);
	QStandardItemModel* m_SourceModel;
	QStandardItemModel* m_DestinationModel;
	int m_iNodeID;
	QVector<int> m_addID;
	QPointer<QButtonGroup> m_ButtonGroup;
};

