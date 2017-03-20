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
    QTableWidget *table_save_data;

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

    void deal_recv_data(QByteArray data);
    void display_posiation_data(QByteArray data);

    void send_mesg(QByteArray msg);

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

    void on_table_save_data_clicked();

    void on_button_commit_curdata_db_clicked();

    void on_button_delete_alldata_clicked();

    void on_button_get_curdata_clicked();

    void on_button_commit_clicked();

    void on_dial_value_valueChanged(int arg1);

    void on_slider_value_valueChanged(int arg1);

    void on_button_jump_clicked();

    void on_test_Button_2_clicked();

private:
    Ui::yuntai *ui;
};

#endif // YUNTAI_H




