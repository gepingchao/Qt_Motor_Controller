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
