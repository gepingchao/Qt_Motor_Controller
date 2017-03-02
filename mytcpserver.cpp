/*���ʱ�䣺2013-10-20
 *�����Ա��������
*/

#include "mytcpserver.h"
#include <QHostAddress>

myTcpServer::myTcpServer(QObject *parent) :
    QTcpServer(parent)
{
    this->clientCount=0;
}

void myTcpServer::incomingConnection(int handle)
{
    myTcpClient *client=new myTcpClient(this,handle);
    client->setSocketDescriptor(handle);

    connect(client,SIGNAL(ClientReadData(int,QString,int,QByteArray)),this,SIGNAL(ClientReadData(int,QString,int,QByteArray)));
    connect(client,SIGNAL(ClientDisConnect(int,QString,int)),this,SLOT(DisConnect(int,QString,int)));

    emit ClientConnect(handle, client->peerAddress().toString(),client->peerPort());

    ClientList.append(client);//���µ�������ӵ��ͻ����б�
    ClientID.append(handle);//���µ����ӵ�ID��ӵ��ͻ���ID�б�
    clientCount++;

    //�洢��ǰ����
    CurrentClient=client;
}

void myTcpServer::DisConnect(int clientID,QString IP,int Port)
{    
    for (int i=0;i<clientCount;i++)
    {
        if (ClientID[i]==clientID)
        {
            ClientList.removeAt(i);//���б����Ƴ�������
            ClientID.removeAt(i);
            clientCount--;
            i--;//��Ȼ�Ļ�����Զֻ���Ƴ���һ������
            emit ClientDisConnect(clientID,IP,Port);
            break;
        }
    }
}

//ָ���ͻ������ӷ���Ϣ
void myTcpServer::SendData(int clientID, QByteArray data)
{
    for (int i=0;i<clientCount;i++)
    {
        if (ClientID[i]==clientID)
        {
            ClientList[i]->write(data);
            break;
        }
    }
}

//�Ե�ǰ���ӷ�������
void myTcpServer::SendDataCurrent(QByteArray data)
{
    //���û��һ�����ڵ����ӣ��򲻴���
    if (clientCount<1){return;}
    CurrentClient->write(data);
}

//���������ӷ�������
void myTcpServer::SendDataAll(QByteArray data)
{
    for (int i=0;i<clientCount;i++)
    {
        ClientList[i]->write(data);
    }
}

void myTcpServer::CloseAllClient()
{
    //int tmp = clientCount;
    for (int i=0;i<clientCount;i++)
    {
        //tmp = i-1;
        ClientList[i]->close();
        i--;//��Ȼ�Ļ�����Զֻ��Ͽ���һ������
       //tmp --;
        //if(tmp ==0)
        //{
           // return;
        //}
    }
}
