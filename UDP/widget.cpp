#include "widget.h"
#include <QHostAddress>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //界面布局
    setWindowTitle(QObject::tr("绑定9999端口"));
    QGridLayout *mainLayout = new QGridLayout(this);
    m_pIP_Edit = new QLineEdit("255.255.255.255");
    m_pPort_Edit = new QLineEdit("8888");
    m_pText_Edit = new QTextEdit("接收消息内容");
    m_pMsg_Edit = new QLineEdit("发送消息内容");
    m_pSendBtn = new QPushButton("发送");
    m_pFileSendBtn = new QPushButton("选取发送文件");

    QLabel *IPLabel = new QLabel("IP地址");
    QLabel *portLabel = new QLabel("Port号");

    mainLayout->addWidget(IPLabel, 0, 0, 1, 1);
    mainLayout->addWidget(portLabel, 1, 0, 1, 1);
    mainLayout->addWidget(m_pIP_Edit, 0, 1, 1, 1);
    mainLayout->addWidget(m_pPort_Edit, 1, 1, 1, 1);
    mainLayout->addWidget(m_pMsg_Edit, 2, 0, 1, 2);
    mainLayout->addWidget(m_pText_Edit, 3, 0, 1, 2);
    mainLayout->addWidget(m_pSendBtn, 4, 0, 1, 1);
    mainLayout->addWidget(m_pFileSendBtn, 4, 1, 1, 1);

    //通信
    m_pUdpSocket = new QUdpSocket(this);
    m_pUdpSocket->bind(QHostAddress::AnyIPv4, 9999);          //  默认绑定8888端口

    m_pUdpSocket->joinMulticastGroup(QHostAddress("224.0.0.2"));
    connect(m_pUdpSocket, &QUdpSocket::readyRead, this, &Widget::onReadyRead);
    connect(m_pSendBtn, &QPushButton::clicked, this, &Widget::sendMsg);
    connect(m_pFileSendBtn, &QPushButton::clicked, this, &Widget::onSelectFile);
}

void Widget::sendMsg()
{
    //首先发送文件信息 名称和大小
    //拼接头部
    QString fileInfomation = QString("%1#%2").arg(m_fileName).arg(m_nFileSize);
    //发送头
    qDebug() << fileInfomation;
    m_pUdpSocket->writeDatagram(fileInfomation.toUtf8(), QHostAddress(m_pIP_Edit->text()), m_pPort_Edit->text().toInt());

    qDebug() << fileInfomation.toUtf8();
    char buf[4000] = {0};

    //打开并读取文件
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件打开失败";
        return;
    }
    while (!file.atEnd())
    {
        quint64 length;
        length = file.read(buf, sizeof(buf));
        if(length < 4000)
        {
            char lastBuf[length] = {0};
            for(int i = 0; i < length; i++)
            {
                lastBuf[i] = buf[i];
            }
            m_pUdpSocket->writeDatagram(lastBuf, sizeof(lastBuf), QHostAddress(m_pIP_Edit->text()), m_pPort_Edit->text().toInt());
            qDebug() << lastBuf;
            file.close();
            break;
        }
        m_pUdpSocket->writeDatagram(buf, sizeof(buf), QHostAddress(m_pIP_Edit->text()), m_pPort_Edit->text().toInt());

    }

}

void Widget::onReadyRead()
{
    char buf[102400] = {0};
    QHostAddress peerIp;
    quint16 port;
    m_pUdpSocket->readDatagram(buf, sizeof(buf), &peerIp, &port);
    QString receive = QString("收到消息:%1来自%2:%3").arg(QString(buf)).arg(peerIp.toString()).arg(port);
    m_pText_Edit->append(receive);
}

void Widget::onSelectFile()
{
    //选择文件获取文件 大小和名称
    filePath = QFileDialog::getOpenFileName(this, "选择发送文件", "../");
    QFileInfo fileInfo(filePath);
    m_fileName = fileInfo.fileName();
    m_nFileSize = fileInfo.size();
    m_pMsg_Edit->setText(QString("发送文件:#%1#,文件大小:#%2#字节").arg(m_fileName).arg(m_nFileSize));

}

void Widget::sendFile()
{
    m_pUdpSocket->writeDatagram(fileContent, QHostAddress(m_pIP_Edit->text()), m_pPort_Edit->text().toInt());
}

Widget::~Widget()
{

}
