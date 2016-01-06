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
        void onPlayerComboBoxChanged(int iPlayer);

    private:
        QString GetPlayerProcessingStateColor( int iPlayer );

        QTimer m_Timer;
};

#endif // INTERPROCLOOP_H
