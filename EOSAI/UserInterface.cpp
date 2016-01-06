
#include "stdafx.h"
//#include <QApplication>
#include <QtWidgets/QApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <qdebug.h>
#include "UserInterface.h"
UserInterface* UserInterface::s_pUserInterface = NULL;

UserInterface* UserInterface::Instance()
{
	if (s_pUserInterface == NULL)
	{
		s_pUserInterface = new UserInterface();
		s_pUserInterface->Initialize();
	}
	return s_pUserInterface;
}


void UserInterface::Initialize()
{
	//return;

	if (m_pGuiApplication) return;

	int argc = 0;
	char** argv = NULL;

	/*
		QGuiApplication app(argc, argv);
		QQmlApplicationEngine engine(QUrl("qrc:/EOSAIUserInterface/main.qml"));
		app.exec();
		*/
	//
	//QtQuickControlsApplication app(argc, argv);
	//QApplication app(argc, argv); // (argc, argv);
	//m_pApplication = new QApplication(argc, argv);
	//m_pApplication = new QApplication(argc, argv);
	m_pGuiApplication = new QGuiApplication(argc, argv);

	//QQmlApplicationEngine* engine = new QQmlApplicationEngine(this);
	//QQmlApplicationEngine engine(QUrl("qrc:/main.qml"));
	//QQmlApplicationEngine engine;
	m_pQmlApplicationEngine = new QQmlApplicationEngine;

	
	m_pQmlApplicationEngine->rootContext()->setContextProperty("MyController", "12abc");
	//m_pQmlApplicationEngine->load(QUrl(QStringLiteral("qrc:/main.qml")));
	m_pQmlApplicationEngine->load(QUrl(QStringLiteral("qrc:/EOSAIUserInterface/main.qml")));
	
}
/*
void UserInterface::DisplayUI()
{
	ASSERT(m_pGuiApplication);
	ASSERT(m_pQmlApplicationEngine);
	if (m_pGuiApplication && m_pQmlApplicationEngine)
	{
		m_pQmlApplicationEngine->rootContext()->setContextProperty("MyController", "12abc");
		//m_pQmlApplicationEngine->load(QUrl(QStringLiteral("qrc:/main.qml")));
		m_pQmlApplicationEngine->load(QUrl(QStringLiteral("qrc:/EOSAIUserInterface/main.qml")));

		//I don't think I have a qrc or main.qml file in this project.

		//m_pGuiApplication->processEvents();
		//	m_pGuiApplication->exec();
		//	app.exec();
	}
	int g = 0;
}
*/
