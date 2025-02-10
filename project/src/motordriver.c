#include "motordriver.h"


uint8_t motor_get_id(motor_t* aMotor)
{
    return aMotor->ID;
}

uint16_t motor_get_max_voltage(motor_t* aMotor)
{
    return aMotor->max_voltage;
}

int16_t motor_get_voltage(motor_t* aMotor)
{
    return aMotor->current_voltage;
}

int16_t motor_get_power(motor_t* aMotor)
{
    return aMotor->current_power;
}

void motor_set_voltage(motor_t* aMotor, int16_t aVoltage)
{
    aMotor->current_voltage = aVoltage;
}
