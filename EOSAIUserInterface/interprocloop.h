#ifndef INTERPROCLOOP_H
#define INTERPROCLOOP_H

#include <QTimer>

class InterprocLoop : public QObject
{
    Q_OBJECT

    public:
        InterprocLoop(){}
        void BeginListening();

    public slots:
        void ProcessTimer();

        void onProcessSingleTurn();
        void onAutoprocessTurns(bool bChecked);
        //void onPlayerComboBoxChanged(int iPlayer);

    private:
        QString GetPlayerProcessingStateColor( int iPlayer );

        QTimer m_Timer;
        bool   m_bReadAllData = true; // On startup, read all the data
};

#endif // INTERPROCLOOP_H
