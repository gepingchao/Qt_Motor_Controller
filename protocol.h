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
    QByteArray download_posiztion_data(int address, int int_h_angle,int int_v_angle,int int_time ,int direction,int int_row);
    QByteArray delete_all_posiation_data(int address);
    QByteArray polling_one_posiztion_data(int address,int int_row);
    QByteArray commit_data_to_flash(int address);



    QByteArray test_motor(int address,int frequency,int angle);
private:

};


#endif // PROTOCOL_H
