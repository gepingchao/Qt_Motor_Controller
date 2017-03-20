#include "yuntai.h"
#include "ui_yuntai.h"

yuntai::yuntai(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::yuntai)
{
    ui->setupUi(this);
    this->init();
}

yuntai::~yuntai()
{
    delete ui;
}
crc8 mycrc;
protocol myprotocol;

void yuntai::on_horizontal_angle_sliderReleased()
{

}


void yuntai::on_vertical_angle_sliderReleased()
{
}


void yuntai::init()
{


    myHelper::SetsysCode();


    my_timer = new QTimer(this);
    table_save_data = new QTableWidget(ui->tabe_space);
    table_save_data->setRowCount(0);
    table_save_data->setColumnCount(5);
    table_save_data->setGeometry(QRect(0,0,435,310));
    table_save_data->setHorizontalHeaderLabels(QStringList() /*<<tr("定位")*/<<tr("水平角度")<<tr("垂直角度")<<tr("启动时间(ms)")<<tr("方向")<<tr("转动时间(ms)"));
    //QHeaderView* headerView = table_save_data->verticalHeader();
    //headerView->setHidden(true);

    //table_save_data->setColumnWidth(0,40);
    table_save_data->setColumnWidth(0,80);
    table_save_data->setColumnWidth(1,80);
    table_save_data->setColumnWidth(2,85);
    table_save_data->setColumnWidth(3,65);
    table_save_data->setColumnWidth(4,85);
    //table_save_data->setDisabled(TRUE);

    //table_save_data->setItem(2,2,new QTableWidgetItem("1234"));
    //table_save_data->setItem(0,1,new QTableWidgetItem("水平角度"));
    //table_save_data->setItem(0,2,new QTableWidgetItem("垂直角度"));
    //table_save_data->setItem(0,3,new QTableWidgetItem("时间"));

    table_save_data->setSelectionBehavior(QAbstractItemView::SelectRows);
    table_save_data->setSelectionMode(QAbstractItemView::SingleSelection);
    table_save_data->setEditTriggers(QAbstractItemView::NoEditTriggers);



    connect(table_save_data,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(on_table_save_data_clicked()));



    real_time_timer = new QTimer(this);
    ui->port_set->setText("3333");
    ui->hex_rx->setChecked(TRUE);
    ui->hex_tx->setChecked(TRUE);

    ui->progressBar_download->setRange(0,49999);
    ui->progressBar_download->setValue(0);
    ui->progressBar_download->setVisible(false);

    tcpClient = new QTcpSocket(this);
    tcpClient->abort();
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ReadError(QAbstractSocket::SocketError)));

    tcpServer = new myTcpServer(this);
    connect(tcpServer,SIGNAL(ClientConnect(int,QString,int)),this,SLOT(ClientConnect(int,QString,int)));
    connect(tcpServer,SIGNAL(ClientDisConnect(int,QString,int)),this,SLOT(ClientDisConnect(int,QString,int)));
    connect(tcpServer,SIGNAL(ClientReadData(int,QString,int,QByteArray)),this,SLOT(ClientReadData(int,QString,int,QByteArray)));

    ////timer
    connect(my_timer,SIGNAL(timeout()),this,SLOT(timer_updata()));
    my_timer->start(1000);

    connect(my_timer,SIGNAL(timeout()),this,SLOT(real_time_timer_updata()));
    my_timer->start(5);
    ui->positive->setChecked(true);
}

void yuntai::on_listen_clicked()
{
    if(ui->listen->text()=="监听")
    {
        bool ok=tcpServer->listen(QHostAddress::Any,ui->port_set->text().toInt());
        if(ok)
        {
            ui->listen->setText("停止");
            ui->rx_buf->append(tr("监听成功"));
        }
    }
    else
    {
        tcpServer->CloseAllClient();
        tcpServer->close();
        ui->listen->setText("监听");
        ui->rx_buf->append(tr("停止监听成功"));
    }
}



void yuntai::on_button_add_line_clicked()
{
    table_save_data->setRowCount(table_save_data->rowCount()+1);

    QString set_start_time,set_h_angle,set_v_angle,set_runing_time;
    set_h_angle = QString("%1").arg(ui->dial_value->value());
    set_v_angle = QString("%1").arg(ui->slider_value->value());
    set_start_time = QString("%1").arg(ui->start_time_value->value());
    set_runing_time = QString("%1").arg(ui->runing_time_value->value());
    table_save_data->setItem(table_save_data->rowCount()-1,0,new QTableWidgetItem(set_h_angle));
    table_save_data->setItem(table_save_data->rowCount()-1,1,new QTableWidgetItem(set_v_angle));
    table_save_data->setItem(table_save_data->rowCount()-1,2,new QTableWidgetItem(set_start_time));
    table_save_data->setItem(table_save_data->rowCount()-1,4,new QTableWidgetItem(set_runing_time));
    if(ui->positive->isChecked())
    {
        table_save_data->setItem(table_save_data->rowCount()-1,3,new QTableWidgetItem("正转"));
    }
    else
    {
        table_save_data->setItem(table_save_data->rowCount()-1,3,new QTableWidgetItem("反转"));
    }
}

void yuntai::on_button_lock_line_clicked()
{
    if(ui->button_lock_line->text()=="锁定")
        {
        ui->button_lock_line->setText("解锁");
        table_save_data->setDisabled(true);
        ui->button_add_line->setDisabled(true);
        ui->button_dele_line->setDisabled(true);
        ui->button_add_item->setDisabled(true);
        ui->button_jump->setDisabled(true);
        }
    else
        {
        ui->button_lock_line->setText("锁定");
        table_save_data->setDisabled(false);
        ui->button_add_line->setDisabled(false);
        ui->button_dele_line->setDisabled(false);
        ui->button_add_item->setDisabled(false);
        ui->button_jump->setDisabled(false);
        }
}


void yuntai::on_button_dele_line_clicked()
{
    table_save_data->removeRow(table_save_data->currentRow());
}

void yuntai::on_button_add_item_clicked()
{
    QString set_start_time,set_h_angle,set_v_angle,set_runing_time;
    set_h_angle = QString("%1").arg(ui->dial_value->value());
    set_v_angle = QString("%1").arg(ui->slider_value->value());
    set_start_time = QString("%1").arg(ui->start_time_value->value());
    set_runing_time = QString("%1").arg(ui->runing_time_value->value());
    table_save_data->setItem(table_save_data->currentRow(),0,new QTableWidgetItem(set_h_angle));
    table_save_data->setItem(table_save_data->currentRow(),1,new QTableWidgetItem(set_v_angle));
    table_save_data->setItem(table_save_data->currentRow(),2,new QTableWidgetItem(set_start_time));
    table_save_data->setItem(table_save_data->currentRow(),4,new QTableWidgetItem(set_runing_time));
    if(ui->positive->isChecked())
    {
        table_save_data->setItem(table_save_data->currentRow(),3,new QTableWidgetItem("正转"));
    }
    else
    {
        table_save_data->setItem(table_save_data->currentRow(),3,new QTableWidgetItem("反转"));
    }

    //table_save_data->item(2,3)->values.

}


void yuntai::ReadData()
{
    QByteArray buffer = tcpClient->readAll();
    if(!buffer.isEmpty())
    {
        if(ui->hex_rx->isChecked())
        {
            ui->rx_buf->append(tr("time:%1 recv:%2").arg(QTime::currentTime().toString("hh:mm:ss")).arg(myHelper::ByteArrayToHexStr(buffer)));
        }
        else
        {
            ui->rx_buf->append(tr("time:%1 recv:%2").arg(QTime::currentTime().toString("hh:mm:ss")).arg(QString(buffer)));
        }
    }
}

void yuntai::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    ui->listen->setText("try again");
    ui->rx_buf->append(tr("error: %1").arg(tcpClient->errorString()));
}

void yuntai::ClientReadData(int clientID, QString IP, int Port, QByteArray data)
{
    if(!data.isEmpty())
    {
        deal_recv_data(data);
        if(ui->hex_rx->isChecked())
        {
            //QString target_device = ui->device->currentText();
            //if(target_device.split(":")[0].toInt() == clientID)
            //{
               // deal_with_data(data);
            //}
            ui->rx_buf->append(tr("recv: %1  from: ip:%2:%3").arg(QString(myHelper::ByteArrayToHexStr(data))).arg(IP).arg(Port));
        }
        else
        {
            ui->rx_buf->append(tr("recv: %1  from: ip:%2:%3").arg(QString(data)).arg(IP).arg(Port));
        }
    }
}

void yuntai::ClientConnect(int clientID, QString IP, int Port)
{
    ui->rx_buf->append(tr("client: %1 ip:%2 port:%3 connected!").arg(clientID).arg(IP).arg(Port));
    ui->client->addItem(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port));
    //ui->device->addItem(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port));
    ui->comboBox_device_stat->addItem(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port));
}

void yuntai::ClientDisConnect(int clientID, QString IP, int Port)
{
    ui->rx_buf->append(tr("client: %1 ip:%2 port:%3 disconnected!").arg(clientID).arg(IP).arg(Port));
    ui->client->removeItem(ui->client->findText(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port)));
    //ui->device->removeItem(ui->device->findText(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port)));
    ui->comboBox_device_stat->removeItem(ui->comboBox_device_stat->findText(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port)));
}


void yuntai::on_reset_clicked()
{
    QString target = ui->client->currentText();
    int clientID = target.split(":")[0].toInt();
    if(0 == clientID)
    {
        return;
    }

    QByteArray mesg;
    mesg = myprotocol.motor_reset(0XFF);
    tcpServer->SendData(clientID,mesg);
}

void yuntai::send_mesg(QByteArray msg)
{
    QString target = ui->client->currentText();
    int clientID = target.split(":")[0].toInt();
    if(0 == clientID)
    {
        return;
    }

    tcpServer->SendData(clientID,msg);
}

void yuntai::on_send_clicked()
{
    QString txt = ui->tx_buf->text();
    if(txt == ""){return;}

    QString target = ui->client->currentText();
    int clientID = target.split(":")[0].toInt();
    QByteArray send_data;
    if(ui->hex_tx->isChecked())
    {
        send_data = myHelper::HexStrToByteArray(txt);
        tcpServer->SendData(clientID,send_data);
    }
    else
    {
        send_data = txt.toAscii();
        tcpServer->SendData(clientID,send_data);
    }
}

void yuntai::on_clear_rx_buf_clicked()
{
    ui->rx_buf->clear();
}

void yuntai::on_clear_tx_buf_clicked()
{
    ui->tx_buf->clear();
}

void yuntai::real_time_timer_updata()
{
    static int old_value;
    if(old_value != ui->horizontal_angle->value())
    {
        old_value = ui->horizontal_angle->value();
        if(ui->real_time->isChecked())
        {
            on_horizontal_angle_sliderReleased();
        }
    }
}

void yuntai::on_pushButton_clicked()
{
    QString target = ui->client->currentText();
    int clientID = target.split(":")[0].toInt();
    if(0 == clientID)
    {
        return;
    }

    QByteArray mesg;
    mesg = myprotocol.test_motor(0XFF,ui->test_paratemer->text().toInt(),ui->test_paratemer_2->text().toInt());
    tcpServer->SendData(clientID,mesg);
}

void yuntai::on_table_save_data_clicked()
{
    if(table_save_data->item(table_save_data->currentRow(),3)->text()== "NULL")
    {
        return;
    }
    else
    {
        ui->horizontal_angle->setValue(table_save_data->item(table_save_data->currentRow(),0)->text().toInt());
        ui->vertical_angle->setValue(table_save_data->item(table_save_data->currentRow(),1)->text().toInt());
        ui->start_time_value->setValue(table_save_data->item(table_save_data->currentRow(),2)->text().toInt());
        ui->runing_time_value->setValue(table_save_data->item(table_save_data->currentRow(),4)->text().toInt());
        if(table_save_data->item(table_save_data->currentRow(),3)->text()== "正转")
        {
            ui->positive->setChecked(true);
        }
        else
        {
            ui->negative->setChecked(true);
        }
    }

}


void yuntai::on_button_commit_curdata_db_clicked()
{
    QByteArray msg;
    int int_h_angle,int_v_angle,int_start_time,int_direction,int_runing_time,int_row;
    if(table_save_data->rowCount() == 0)
    {
        return;
    }
    else
    {
        int_h_angle = table_save_data->item(table_save_data->currentRow(),0)->text().toInt();
        int_v_angle = table_save_data->item(table_save_data->currentRow(),1)->text().toInt();
        int_start_time = table_save_data->item(table_save_data->currentRow(),2)->text().toInt();
        if(table_save_data->item(table_save_data->currentRow(),3)->text()== "正转")
            {int_direction = 1;}
        if(table_save_data->item(table_save_data->currentRow(),3)->text()== "反转")
            {int_direction = 0;}
        int_runing_time = table_save_data->item(table_save_data->currentRow(),4)->text().toInt();
        int_row = table_save_data->currentRow();
        msg = myprotocol.download_posiztion_data(0XFF,int_h_angle,int_v_angle,int_start_time,int_direction,int_runing_time,int_row);
        send_mesg(msg);
    }

}




void yuntai::on_button_delete_alldata_clicked()
{
    QByteArray msg;
    msg = myprotocol.delete_all_posiation_data(0XFF);
    send_mesg(msg);
}


void yuntai::on_button_get_curdata_clicked()
{
    if(table_save_data->rowCount() == 0)
    {
        return;
    }
    QByteArray msg;
    msg = myprotocol.polling_one_posiztion_data(0XFF,table_save_data->currentRow());
    send_mesg(msg);
}


void yuntai::on_button_commit_clicked()
{
    QByteArray msg;
    msg = myprotocol.commit_data_to_flash(0XFF);
    send_mesg(msg);
}


void yuntai::display_posiation_data(QByteArray data)
{
    unsigned int int_h_angle,int_v_angle,int_time,int_speed;
    QString set_time,set_h_angle,set_v_angle,set_speed;
    if(data[3] != 27)
    {
        return;
    }
    int_h_angle = data.at(5)*0X1000000 +(data.at(6))*0X10000 +(data.at(7))*0X100 +(unsigned char)(data.at(8));
    int_v_angle = data.at(9)*0X1000000 +(data.at(10))*0X10000 +(data.at(11))*0X100 +(unsigned char)(data.at(12));
    int_time = data.at(13)*0X1000000 +(data.at(14))*0X10000 +(data.at(15))*0X100 +(unsigned char)(data.at(16));
    int_speed = data.at(18)*0X1000000 +(data.at(19))*0X10000 +(data.at(20))*0X100 +(unsigned char)(data.at(21));

    set_h_angle=QString("%1").arg(int_h_angle);
    set_v_angle=QString("%1").arg(int_v_angle);
    set_time=QString("%1").arg(int_time);
    set_speed = QString("%1").arg(int_speed);

    table_save_data->setItem(table_save_data->currentRow(),0,new QTableWidgetItem(set_h_angle));
    table_save_data->setItem(table_save_data->currentRow(),1,new QTableWidgetItem(set_v_angle));
    table_save_data->setItem(table_save_data->currentRow(),2,new QTableWidgetItem(set_time));
    table_save_data->setItem(table_save_data->currentRow(),4,new QTableWidgetItem(set_speed));
    if(data[17] == 0X1)
    {
        table_save_data->setItem(table_save_data->currentRow(),3,new QTableWidgetItem("正转"));
    }
    if(data[17] == 0X00)
    {
        table_save_data->setItem(table_save_data->currentRow(),3,new QTableWidgetItem("反转"));
    }
}

void yuntai::deal_recv_data(QByteArray data)
{
    char cmd;
    cmd = data[4];
    switch(cmd)
    {
        case 37:
        display_posiation_data(data);
        break;

        default:
        break;
    }
}


void yuntai::on_dial_value_valueChanged(int arg1)
{
    QByteArray mesg;
    mesg = myprotocol.motor1_goto_posiation(0XFF,ui->horizontal_angle->value());
    send_mesg(mesg);
}

void yuntai::on_slider_value_valueChanged(int arg1)
{
     QByteArray mesg;
     mesg = myprotocol.motor2_goto_posiation(0XFF,ui->vertical_angle->value());
     send_mesg(mesg);
}


void yuntai::on_button_jump_clicked()
{
    QByteArray mesg;
    mesg = myprotocol.jump_to_point(0XFF,table_save_data->currentRow());
    send_mesg(mesg);
}

void yuntai::on_test_Button_2_clicked()
{
    QByteArray mesg;
    mesg = myprotocol.test_motor_operat_intime(0XFF,ui->test_paratemer_time->text().toInt(),ui->test_paratemer_x_angle->text().toInt(),ui->test_paratemer_y_angle->text().toInt());
    send_mesg(mesg);
}
