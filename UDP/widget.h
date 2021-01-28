#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QUdpSocket>
#include <QDebug>
#include <QByteArray>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextEdit>
#include <QGridLayout>
#include <QLabel>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
    void sendMsg();
    void sendFile();
    void onReadyRead();
    void onSelectFile();

private:
    QUdpSocket *m_pUdpSocket;
    QLineEdit *m_pIP_Edit;
    QLineEdit *m_pPort_Edit;
    QLineEdit *m_pMsg_Edit;
    QTextEdit *m_pText_Edit;
    QPushButton *m_pSendBtn;
    QPushButton *m_pFileSendBtn;

    QString m_fileName;
    qint64 m_nFileSize ;

    QString filePath;

    QByteArray fileContent;
};

#endif // WIDGET_H
