#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QUdpSocket>
#include <QDebug>
#include <QByteArray>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextEdit>
#include <QLabel>
#include <QFile>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void sendMsg();
    void onReadyRead();
    void onReadyFile();

private:
    QUdpSocket *m_pUdpSocket;
    QLineEdit *m_pIP_Edit;
    QLineEdit *m_pPort_Edit;
    QLineEdit *m_pMsg_Edit;
    QTextEdit *m_pText_Edit;
    QPushButton *m_pSendBtn;
    QPushButton *m_pCloseBtn;

    QString m_fileName;
    qint64 m_nFileSize ;

    QFile file;

    bool isStart;

    quint64 receiveSize;
};

#endif // WIDGET_H
