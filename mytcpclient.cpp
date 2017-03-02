
/*���ʱ�䣺2013-10-20
 *�����Ա��������
*/

#include "mytcpclient.h"
#include <QHostAddress>
#include "myhelper.h"

myTcpClient::myTcpClient(QObject *parent,int clientID) :
    QTcpSocket(parent)
{    
    this->clientID=clientID;
    connect(this,SIGNAL(readyRead()),this,SLOT(ReadData()));//�ҽӶ�ȡ�����ź�
    connect(this,SIGNAL(disconnected()),this,SLOT(DisConnect()));//�ر�����ʱ�����ͶϿ������ź�
    //����ر������Զ�ɾ�������´β����ٴμ�������ֵ�����
    //connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));//�ر�����ʱ�������Զ�ɾ��
}

void myTcpClient::ReadData()
{
    myHelper::Sleep(100);
    //��ȡ����һ�����ݲ������ź�
    QByteArray data=this->readAll();
    emit ClientReadData(this->clientID,this->peerAddress().toString(),this->peerPort(),data);
}

void myTcpClient::DisConnect()
{
    //�Ͽ�����ʱ�����ͶϿ��ź�
    emit ClientDisConnect(this->clientID,this->peerAddress().toString(),this->peerPort());
}
