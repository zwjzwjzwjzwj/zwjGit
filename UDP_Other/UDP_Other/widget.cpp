#include "widget.h"
#include <QDebug>
#include <QFile>

Widget::Widget(QWidget *parent)
    : QWidget(parent), receiveSize(0)
{
    //界面布局
    setWindowTitle(QObject::tr("绑定8888端口"));
    QGridLayout *mainLayout = new QGridLayout(this);
    m_pIP_Edit = new QLineEdit;
    m_pPort_Edit = new QLineEdit;
    m_pText_Edit = new QTextEdit("接收消息内容");
    m_pMsg_Edit = new QLineEdit("发送消息内容");
    m_pSendBtn = new QPushButton("发送");
    m_pCloseBtn = new QPushButton("发送文件");

    QLabel *IPLabel = new QLabel("IP地址");
    QLabel *portLabel = new QLabel("Port号");

    mainLayout->addWidget(IPLabel, 0, 0, 1, 1);
    mainLayout->addWidget(portLabel, 1, 0, 1, 1);
    mainLayout->addWidget(m_pIP_Edit, 0, 1, 1, 1);
    mainLayout->addWidget(m_pPort_Edit, 1, 1, 1, 1);
    mainLayout->addWidget(m_pMsg_Edit, 2, 0, 1, 2);
    mainLayout->addWidget(m_pText_Edit, 3, 0, 1, 2);
    mainLayout->addWidget(m_pSendBtn, 4, 0, 1, 1);
    mainLayout->addWidget(m_pCloseBtn, 4, 1, 1, 1);

    //通信
    m_pUdpSocket = new QUdpSocket(this);
    m_pUdpSocket->bind(QHostAddress::AnyIPv4, 8888);          //  默认绑定8888端口

    m_pUdpSocket->joinMulticastGroup(QHostAddress("224.0.0.2"));
    connect(m_pUdpSocket, &QUdpSocket::readyRead, this, &Widget::onReadyRead);
    connect(m_pSendBtn, &QPushButton::clicked, this, &Widget::sendMsg);

    isStart = true;

}

void Widget::sendMsg()
{
    QByteArray msg = m_pMsg_Edit->text().toUtf8();
    m_pUdpSocket->writeDatagram(msg, QHostAddress(m_pIP_Edit->text()), m_pPort_Edit->text().toInt());
}

void Widget::onReadyRead()
{
    //先接收头部
    if(isStart)
    {
        //取出文件大小和名称
        char titleBuf[1024] = {0};
        QHostAddress ip;
        quint16 port;
        m_pUdpSocket->readDatagram(titleBuf, sizeof(titleBuf), &ip, &port);
        //头部接收完毕

        QString fileinfo = QString(titleBuf);
        m_fileName = fileinfo.section("#", 0, 0);
        m_nFileSize = fileinfo.section("#", 1, 1).toInt();

        file.setFileName(m_fileName);
        file.open(QIODevice::WriteOnly);

        isStart = false;
    }
    else
    {
        while (m_pUdpSocket->hasPendingDatagrams())
        {
            QByteArray fileByte;
            fileByte.resize(m_pUdpSocket->pendingDatagramSize());
            QHostAddress ip;
            quint16 port;
            m_pUdpSocket->readDatagram(fileByte.data(), fileByte.size(), &ip, &port);
            receiveSize = receiveSize + file.write(fileByte);
            if(receiveSize == m_nFileSize)
            {
                file.close();
            }
        }
    }

}

void Widget::onReadyFile()
{
    QString fileInfo = m_pText_Edit->toPlainText();
    m_fileName = fileInfo.section("#", 1, 1);
    m_nFileSize = fileInfo.section("#", 3, 3).toInt();

    char filebBuffer[m_nFileSize + 1]  = {0};
    QHostAddress peerIp;
    quint16 port;
    m_pUdpSocket->readDatagram(filebBuffer, sizeof(filebBuffer), &peerIp, &port);
    QString receive = QString("收到文件:%1来自%2:%3").arg(m_fileName).arg(peerIp.toString()).arg(port);
    m_pText_Edit->append(receive);

    QFile file(m_fileName);
    file.open(QIODevice::WriteOnly);
    file.write(filebBuffer);
    file.close();
}

Widget::~Widget()
{

}
