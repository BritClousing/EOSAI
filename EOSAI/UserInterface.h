
#pragma once

class QGuiApplication;
class QQmlApplicationEngine;

class UserInterface
{
	//Q_OBJECT

	public:
		static UserInterface* Instance();

		void Initialize();
		//void DisplayUI();

	private:
		static UserInterface* s_pUserInterface;

		QGuiApplication* m_pGuiApplication = NULL;
		QQmlApplicationEngine* m_pQmlApplicationEngine = NULL;
};


