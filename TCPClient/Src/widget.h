#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QFile>
#include <QTcpSocket>
#include <QFileDialog>
#include <QProgressBar>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QTextEdit *receiveInfo;
    QTextEdit *contentWnd;
    QProgressBar *progressBar;

    QFile file;
    QString fileName;
    qint64  fileSize;
    qint64  receivedSize;

    QTcpSocket *tcpSocket;

    bool isStart;

public slots:
    void onReceiveData();
    void connectToServer();


};

#endif // WIDGET_H
