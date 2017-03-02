#ifndef YUNTAI_H
#define YUNTAI_H

#include <QMainWindow>
#include "myhelper.h"
#include "mytcpclient.h"
#include "mytcpserver.h"
#include <QRegExpValidator>
#include <QThread>
#include<QTime>
#include<QTimer>
#include <QDateTime>
#include <QDirModel>
#include <QFile>
#include <strstream>
#include <QDebug>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "crc.h"
#include "protocol.h"

namespace Ui {
class yuntai;
}

class yuntai : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit yuntai(QWidget *parent = 0);
    ~yuntai();

private:
    QTcpSocket *tcpClient;
    myTcpServer *tcpServer;
    QTimer *my_timer;
    QTimer *real_time_timer;
    QTableWidget *tabe_save_data;

private:
    void init();


    
private slots:
    void on_horizontal_angle_sliderReleased();
    void on_listen_clicked();

    void ReadData();
    void ReadError(QAbstractSocket::SocketError);

    void ClientReadData(int clientID,QString IP,int Port,QByteArray data);
    void ClientConnect(int clientID,QString IP,int Port);
    void ClientDisConnect(int clientID,QString IP,int Port);

    void on_send_clicked();

    void on_reset_clicked();

    void on_clear_rx_buf_clicked();

    void on_clear_tx_buf_clicked();

    void on_vertical_angle_sliderReleased();

    void real_time_timer_updata();

    void on_button_add_line_clicked();

    void on_button_lock_line_clicked();

    void on_button_dele_line_clicked();

    void on_button_add_item_clicked();

    void on_pushButton_clicked();

    void on_tabe_save_data_clicked();

private:
    Ui::yuntai *ui;
};

#endif // YUNTAI_H




