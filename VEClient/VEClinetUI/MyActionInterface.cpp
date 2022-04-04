#include "stdafx.h"
#include "MyActionInterface.h"

MyActionInterface::MyActionInterface( QObject * parent)
	: QAction(parent){}
MyActionInterface::MyActionInterface( const QString & text, QObject * parent)
	: QAction(text, parent){}
MyActionInterface::MyActionInterface( const QIcon & icon, const QString & text, QObject * parent)
	: QAction(icon, text, parent){}