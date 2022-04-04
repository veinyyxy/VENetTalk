#pragma once
#include "ui_MainUI.h"
#include "LoginUI.h"
#include "MessageUI.h"
#include "MessageCallBack.h"
#include "MainTab.h"
#include "MyActionInterface.h"

class LogoutAction;
class SendFileAction;
class SendMessageAction;

class ClientMainUI :
	public QMainWindow, public MessageCallback
{
	Q_OBJECT
public:
	typedef struct _____nodeidtoitem
	{
		int nodeid;
		QStandardItem* item;
	} NODE_ID_TO_ITEM;
	typedef struct ReceivedMessageData
	{
		int iUIType;
		int iType;//服务器标示的类型
		int iState;//消息的状态 0 已读， 1 未读
		QString m_struser;//谁发送的消息
		QString m_strName;//姓名
		char* pReceiveBuffer;//从服务器接收到的信息
		int iLen;
	} MESSSAGE_DATA_UI, *P_MESSSAGE_DATA_UI;

	typedef struct ___state____
	{
		int iPreState;
		int iCurState;
	} MY_STATE;
	typedef QVector<MESSSAGE_DATA_UI> MESSAGE_VECTOR, *P_MESSAGE_VECTOR;
	typedef QMap<QString, MESSAGE_VECTOR> USER_MAPTO_MESSAGE, *P_USER_MAPTO_MESSAGE;
	typedef QMap<QString, MessageUI*> USER_MAPTO_DIALOG, *P_USER_MAPTO_DIALOG;

	ClientMainUI(void);
	virtual ~ClientMainUI(void);
	void Show();
	void closeEvent(QCloseEvent *event);
	P_USER_MAPTO_MESSAGE GetMessagePool(){return &m_MessagePool;}
	void InserMessageToPool(QString& strname, MESSSAGE_DATA_UI& pMessageData);
	void DeleteDialogFromPool(QString& strname);
	void CreateDialogFromName(QString& strAccount, QString& strName);
	void setLoginUI();
	void ClearPool();
	void ToSendMessage();
signals:
	void signal_Login(int iState);
	//void signal_ReceiveMessage(int iType, unsigned char* pBuffer, int len);
	void signal_ReceiveMessage(P_MESSSAGE_DATA_UI);
	void signal_LogoutReply(int iS);
	void signal_FlushAllState(P_USER_INFO_LIST pUIL);
	void signal_heartbeat(int iBeat);
	void signal_setStateResult(int iState);
	MessageCallback::REQUEST_RESULT signal_TransmitFileRequest(int iState
		, std::string& srcName
		, std::string& fileName, std::string& strOutPath, int iType);
	int signal_SendFileProgress(const std::string& strSendFileName
		, int iBlockCount, int iBlockID);
	int signal_ReceiveFileProgress(const std::string& strReceiveFileName
		, int iBlockCount, int iBlockID);
	void signal_OrganizationStructure( std::vector<ORG_NODE>* pNodes );
	void signal_MyCustomGroup( P_CUSTOM_GROUPS pGroups );
	void signal_QueryAccountResult( int iNodeId, std::string& username );
	void signal_UserDetailInformation(P_USER_DETAIL_INFO info);

public slots:
	void slot_Login(int iState);
	//void slot_ReceiveMessage(int iType, unsigned char* pBuffer, int len);
	void slot_ReceiveMessage(P_MESSSAGE_DATA_UI);
	void slot_LogoutReply(int iS);
	void slot_itemDoubleClicked ( QTreeWidgetItem * item, int column );
	void slot_PatchMessage();
	void slot_activated(QSystemTrayIcon::ActivationReason);
	void slot_lookMessage();
	void slot_FlushAllState();
	void slot_UserStateHandle(P_USER_INFO_LIST PUIL);
	void slot_heartbeat(int iBeat);
	void slot_setStateResult(int iState);
	void slot_setUserStateMenu(bool);
	void slot_ContactPeopleTreeView_CustomContextMenuRequested(const QPoint& pp);
	MessageCallback::REQUEST_RESULT slot_TransmitFileRequest(int iState
		, std::string& srcName
		, std::string& fileName, std::string& strOutPath, int iType);
	int slot_SendFileProgress(const std::string& strSendFileName
		, int iBlockCount, int iBlockID);
	int slot_ReceiveFileProgress(const std::string& strReceiveFileName
		, int iBlockCount, int iBlockID);
	void slot_QueryAccountResult( int iNodeId, std::string& username );

	void slot_MainMenuToggled ( QAction* pQA );
	void slot_OrganizationStructure( std::vector<ORG_NODE>* pNodes );
	void slot_MyCustomGroup( P_CUSTOM_GROUPS pGroups );
	void slot_ModifyGroup(QAction* pAction);
	void slot_CustomGroupNameItemChanged(QStandardItem * item);
	void slot_UserStateMenuTriggered(QAction*);
	void slot_doubleClicked(const QModelIndex &);
	void slot_clicked(const QModelIndex & index);
	void slot_UserDetailInformation(P_USER_DETAIL_INFO info);
	void slot_autoLogin();

private:
	void CreateUserStateMenu();
	void CreateModifyCustomGroupMenu();
	void InitMainUI();
	void InitLoginUI();

	void Login( int iState, P_USER_INFO_LIST pUserInfo );

	void ReceiveMessage( int iType, std::string& strUser
		, unsigned char* pBuffer, int len );
	void LogoutReply( int iState );

	virtual void UserState( P_USER_INFO_LIST pUserState );
	virtual void Heartbeat( int iBeat );
	virtual void SetStateResult( int iState );

	virtual MessageCallback::REQUEST_RESULT TransmitFileRequest( int iState
		, std::string& srcName, std::string& fileName, std::string& strOutPath
		, int iType);
	virtual int SendFileProgress( const std::string& strSendFileName
		, int iBlockCount, int iBlockID );
	virtual int ReceiveFileProgress( const std::string& strReceiveFileName
		, int iBlockCount, int iBlockID );
	virtual void OrganizationStructure( std::vector<ORG_NODE>* pNodes );
	virtual int MyCustomGroup( P_CUSTOM_GROUPS pGroups );

	virtual int ModifyCustomGroupResult( int* iGroupID, int* iResult );
	virtual int QueryAccountResult( int iNodeId, std::string& username );

	virtual int UserDetailInformation( P_USER_DETAIL_INFO info );

	MessageCallback::USER_INFO_LIST m_UserInfoList;
	QString m_strUserName, m_strSendedUserName;
	Ui::ClientMainUI m_ClientUI;
	QSharedPointer<LoginUI> m_LoginUI;
	QSharedPointer<MainTab> m_MainTab;
	QPointer<QSystemTrayIcon> m_TrayIcon;

	QString m_qstrServerIP, m_qstrServerPort, m_qstrClientIP
		, m_qstrClientPort;
	MessageCallback* m_pMessageCallback;
	
	USER_MAPTO_MESSAGE m_MessagePool;
	USER_MAPTO_DIALOG m_DialogPool;
	QStack<QString> m_NameStack;
	QTimer m_PatchMessageTimer, m_LookMessageTimer, m_FlushUserStateTimer
		, m_heartbeatTimer;
	QPointer<QMenu> m_UserStateMenu;
	QPointer<QMenu> m_ModifyCustomGroupsMenu;
	QPointer<QActionGroup> m_pActionGroup;
	MY_STATE m_MySate;

	QProgressDialog m_SaveProgressDialog;
	QProgressDialog m_SendProgressDialog;

	QActionGroup m_pMainActionGroup;

	QStandardItemModel* m_OrganizationModel;
	QStandardItemModel* m_CustomGroupsModel;

	QAction* pAdd;
	QAction* pAddPeople;
	QAction* pDel;
	QAction* pMgc;
	LogoutAction* m_LogoutAction;
	SendFileAction* m_pSendFileAction;
	SendFileAction* m_pSendOfflineFileAction;
	SendMessageAction* m_pSendMessageAction;

	QMap<QAction*, QAction*> m_ActionMap;
	QMap<int, QStandardItem*> m_AccountNode;
	QVector<int> m_AccountVector;

	QModelIndex m_CurrentIndex;
	QTreeView* m_pSender;
	QTimer m_loginTimer;
};

class LogoutAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(LogoutAction)
		virtual void MyAction();
	virtual void InitMyAction();
	ClientMainUI* m_pClientUainUI;
};