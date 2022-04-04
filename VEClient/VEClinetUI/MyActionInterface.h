#pragma once

#include <QtGui/QAction>

#define ADD_MYACTION_CONTSTRUCTED_DEFINITION(class_name) \
	class_name( QObject * parent ); \
	class_name( const QString & text, QObject * parent );\
	class_name( const QIcon & icon, const QString & text, QObject * parent );

#define ADD_MYACTION_CONTSTRUCTED_DECLARATION(class_name)\
	class_name::class_name( QObject * parent = 0 ) \
: MyActionInterface(parent){InitMyAction();}\
class_name::class_name( const QString & text, QObject * parent  = 0) \
	: MyActionInterface(text, parent) {InitMyAction();}\
class_name::class_name( const QIcon & icon, const QString & text, QObject * parent  = 0)\
	: MyActionInterface(icon, text, parent){InitMyAction();}

class MyActionInterface : public QAction
{
public:
	MyActionInterface( QObject * parent = 0);
	MyActionInterface( const QString & text, QObject * parent  = 0 );
	MyActionInterface( const QIcon & icon, const QString & text, QObject * parent  = 0);
	virtual void MyAction() = 0;
	virtual void InitMyAction() = 0;
};

