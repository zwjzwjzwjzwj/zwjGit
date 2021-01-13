#include "widget.h"
#include <QTcpSocket>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QFont>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QCoreApplication>


Widget::Widget(QWidget *parent)
    : QWidget(parent), receivedSize(0)
{
    QGridLayout *mainLayout = new QGridLayout(this);
    QFont font("宋体", 18, QFont::Bold);
    this->setFont(font);

    QPushButton *connectBtn = new QPushButton("连接服务器");

    QLabel *titleLabel = new QLabel("客户端");
    titleLabel->setAlignment(Qt::AlignCenter);
    QLabel *ipLabel   = new QLabel("IP地址");
    QLabel *portLabel = new QLabel("端口");

    ipLineEdit   = new QLineEdit;
    ipLineEdit->setText("192.168.42.42");
    portLineEdit = new QLineEdit;
    portLineEdit->setText("8888");

    receiveInfo = new QTextEdit;
    receiveInfo->setText("通信信息");
    contentWnd = new QTextEdit;
    contentWnd->setText("通信状态");

    mainLayout->addWidget(titleLabel, 0, 0, 1, 3);
    mainLayout->addWidget(ipLabel, 1, 0, 1, 1);
    mainLayout->addWidget(portLabel, 2, 0, 1, 1);
    mainLayout->addWidget(ipLineEdit, 1, 1, 1, 1);
    mainLayout->addWidget(portLineEdit, 2, 1, 1, 1);
    mainLayout->addWidget(connectBtn, 1, 2, 2, 1);
    mainLayout->addWidget(receiveInfo, 3, 0, 2, 3);
    mainLayout->addWidget(contentWnd, 4, 0, 2, 3);

    isStart = true;
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, &QTcpSocket::readyRead, this, &Widget::onReceiveData);
    connect(connectBtn, &QPushButton::clicked, this, &Widget::connectToServer);
}

void Widget::onReceiveData()
{
    //qDebug() << "收到文件";
    QByteArray buf = tcpSocket->readAll();
    if(isStart == true) //接收头部
    {
        isStart = true;
        fileName = QString(buf).section("#", 0, 0);
        fileSize = QString(buf).section("#", 1, 1).toInt();
        file.setFileName(QString("../%1").arg(fileName));
        bool isOk = file.open(QIODevice::WriteOnly);
        if(isOk == false)
        {
            qDebug() << "文件打开出错";
        }
        isStart = false;
    }
    else
    {
        qint64 len = file.write(buf);
        receivedSize += len;
        if(receivedSize == fileSize)
        {
            file.close();
            QMessageBox::information(this, "接收提示", "文件接收完毕", QMessageBox::NoButton);
            //rec
            isStart = true;
        }

    }
}

void Widget::connectToServer()
{
    //获取服务器IP和端口
    QString ip = ipLineEdit->text();
    qint64 port = portLineEdit->text().toInt();

    tcpSocket->connectToHost(QHostAddress(ip), port);

}

Widget::~Widget()
{

}
