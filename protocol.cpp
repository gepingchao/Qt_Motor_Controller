#include "protocol.h"

crc8 protocol_crc;
void protocol::motor_run()
{

}
QByteArray protocol::motor_reset(int address)
{
    QByteArray value;
    value.resize(6);
    value[0] = 0XAA;
    value[1] = 0X90;
    value[2] = address;
    value[3] = 0X06;
    value[4] = 0X00;
    //value[5] = crc8::SetCRC8Sub()
    protocol_crc.SetCRC8Sub(value.data(),5);
    return value;
}

QByteArray protocol::motor1_goto_posiation(int address, int posiation)
{
    QByteArray value;
    QByteArray angle;
    value.resize(8);
    angle.resize(2);
    angle[1] = posiation&0X000000FF;
    angle[0] = (posiation >> 8)&0X000000FF;
    value[0] = 0XAA;
    value[1] = 0X90;
    value[2] = address;
    value[3] = 0X08;
    value[4] = 0X15;
    value[5] = angle[0];
    value[6] = angle[1];
    protocol_crc.SetCRC8Sub(value.data(),7);
    return value;
}

QByteArray protocol::motor2_goto_posiation(int address, int posiation)
{
    QByteArray value;
    QByteArray angle;
    value.resize(8);
    angle.resize(2);
    angle[1] = posiation&0X000000FF;
    angle[0] = (posiation >> 8)&0X000000FF;


    value[0] = 0XAA;
    value[1] = 0X90;
    value[2] = address;
    value[3] = 0X08;
    value[4] = 0X16;
    value[5] = angle[0];
    value[6] = angle[1];
    protocol_crc.SetCRC8Sub(value.data(),7);
    return value;
}

QByteArray protocol::test_motor(int address, int frequency,int angle)
{
    QByteArray value;
    QByteArray freq;
    QByteArray ang;
    value.resize(11);
    freq.resize(3);
    ang.resize(2);
    freq[2] = frequency&0X000000FF;
    freq[1] = (frequency >> 8)&0X000000FF;
    freq[0] = (frequency >> 16)&0X000000FF;

    ang[1] = angle&0X000000FF;
    ang[0] = (angle >> 8)&0X000000FF;


    value[0] = 0XAA;
    value[1] = 0X90;
    value[2] = address;
    value[3] = 11;
    value[4] = 0X88;
    value[5] = freq[0];
    value[6] = freq[1];
    value[7] = freq[2];
    value[8] = ang[0];
    value[9] = ang[1];
    protocol_crc.SetCRC8Sub(value.data(),10);
    return value;
}

QByteArray protocol::test_motor_operat_intime(int address,int int_time,int int_x_angle,int int_y_angle)
{
    QByteArray value;
    QByteArray time;
    QByteArray x_angle,y_angle;
    value.resize(12);
    time.resize(2);
    x_angle.resize(2);
    y_angle.resize(2);

    time[1] = (int_time >> 0)&0X000000FF;
    time[0] = (int_time >> 8)&0X000000FF;

    x_angle[1] = (int_x_angle >> 0)&0X000000FF;
    x_angle[0] = (int_x_angle >> 8)&0X000000FF;

    y_angle[1] = (int_y_angle >> 0)&0X000000FF;
    y_angle[0] = (int_y_angle >> 8)&0X000000FF;


    value[0] = 0XAA;
    value[1] = 0X90;
    value[2] = address;
    value[3] = 12;
    value[4] = 0X89;
    value[5] = time[0];
    value[6] = time[1];
    value[7] = x_angle[0];
    value[8] = x_angle[1];
    value[9] = y_angle[0];
    value[10] = y_angle[1];
    protocol_crc.SetCRC8Sub(value.data(),11);
    return value;
}

 QByteArray protocol::download_posiztion_data(int address, int int_h_angle,int int_v_angle,int int_time ,int direction,int int_speed,int int_row)
 {
    QByteArray value;
    QByteArray h_angle;
    QByteArray v_angle;
    QByteArray time;
    QByteArray speed;
    QByteArray row;
    value.resize(27);
    h_angle.resize(4);
    v_angle.resize(4);
    time.resize(4);
    row.resize(4);
    speed.resize(4);

    h_angle[0]= (int_h_angle >> 24)&0XFF;
    h_angle[1]= (int_h_angle >> 16)&0XFF;
    h_angle[2]= (int_h_angle >> 8)&0XFF;
    h_angle[3]= (int_h_angle >> 0)&0XFF;

    v_angle[0]= (int_v_angle >> 24)&0XFF;
    v_angle[1]= (int_v_angle >> 16)&0XFF;
    v_angle[2]= (int_v_angle >> 8)&0XFF;
    v_angle[3]= (int_v_angle >> 0)&0XFF;

    time[0]= (int_time >> 24)&0XFF;
    time[1]= (int_time >> 16)&0XFF;
    time[2]= (int_time >> 8)&0XFF;
    time[3]= (int_time >> 0)&0XFF;

    row[0]= (int_row >> 24)&0XFF;
    row[1]= (int_row >> 16)&0XFF;
    row[2]= (int_row >> 8)&0XFF;
    row[3]= (int_row >> 0)&0XFF;

    speed[0]= (int_speed >> 24)&0XFF;
    speed[1]= (int_speed >> 16)&0XFF;
    speed[2]= (int_speed >> 8)&0XFF;
    speed[3]= (int_speed >> 0)&0XFF;


    value[0] = 0XAA;
    value[1] = 0X90;
    value[2] = address;
    value[3] = 27;
    value[4] = 0X21;
    value[5] = h_angle[0];
    value[6] = h_angle[1];
    value[7] = h_angle[2];
    value[8] = h_angle[3];
    value[9] = v_angle[0];
    value[10] = v_angle[1];
    value[11] = v_angle[2];
    value[12] = v_angle[3];
    value[13] = time[0];
    value[14] = time[1];
    value[15] = time[2];
    value[16] = time[3];
    if(1 == direction){
        value[17] = 0XFF;
    }else if(0 == direction){
        value[17] = 0X00;}

    value[18] = speed[0];
    value[19] = speed[1];
    value[20] = speed[2];
    value[21] = speed[3];

    value[22] = row[0];
    value[23] = row[1];
    value[24] = row[2];
    value[25] = row[3];

    protocol_crc.SetCRC8Sub(value.data(),26);
    return value;
 }


 QByteArray protocol::delete_all_posiation_data(int address)
 {
     QByteArray value;
     value.resize(6);
     value[0] = 0XAA;
     value[1] = 0X90;
     value[2] = address;
     value[3] = 0X06;
     value[4] = 0X23;
     //value[5] = crc8::SetCRC8Sub()
     protocol_crc.SetCRC8Sub(value.data(),5);
     return value;
 }

 QByteArray protocol::polling_one_posiztion_data(int address, int int_row)
 {
     QByteArray value;
     value.resize(7);
     value[0] = 0XAA;
     value[1] = 0X90;
     value[2] = address;
     value[3] = 0X07;
     value[4] = 0X25;
     value[5] = (int_row & 0XFF);
     //value[5] = crc8::SetCRC8Sub()
     protocol_crc.SetCRC8Sub(value.data(),6);
     return value;
 }
QByteArray protocol::commit_data_to_flash(int address)
{
    QByteArray value;
    value.resize(6);
    value[0] = 0XAA;
    value[1] = 0X90;
    value[2] = address;
    value[3] = 0X06;
    value[4] = 0X26;
    //value[5] = crc8::SetCRC8Sub()
    protocol_crc.SetCRC8Sub(value.data(),5);
    return value;
}

QByteArray protocol::jump_to_point(int address,int int_point)
{
    QByteArray value;
    QByteArray point;
    value.resize(8);
    point.resize(2);

    point[0]= (int_point >> 8)&0XFF;
    point[1]= (int_point >> 0)&0XFF;

    value[0] = 0XAA;
    value[1] = 0X90;
    value[2] = address;
    value[3] = 0X08;
    value[4] = 0X20;
    value[5] = point[0];
    value[6] = point[1];

    //value[5] = crc8::SetCRC8Sub()
    protocol_crc.SetCRC8Sub(value.data(),7);
    return value;
}
