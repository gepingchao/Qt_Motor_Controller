#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "yuntai.h"
class protocol:public QObject
{
public:
    void motor_run();
    QByteArray motor_reset(int address);
    QByteArray motor1_goto_posiation(int address,int posiation);
    QByteArray motor2_goto_posiation(int address,int posiation);
    QByteArray test_motor(int address,int frequency,int angle);
private:

};


#endif // PROTOCOL_H
