#ifndef __MOTORDRIVER_H
#define __MOTORDRIVER_H
#include <stdint.h>

typedef struct motor{
    uint8_t ID;
    int16_t max_voltage;
    int16_t current_voltage;
    int16_t current_power;
} motor_t;


uint8_t motor_get_id(motor_t* aMotor);

uint16_t motor_get_max_voltage(motor_t* aMotor);

int16_t motor_get_voltage(motor_t* aMotor);

int16_t motor_get_power(motor_t* aMotor);

void motor_set_voltage(motor_t* aMotor, int16_t aVoltage);


#endif //__MOTORDRIVER_H