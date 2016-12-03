//
//  config.h
//  CarControl
//
//  Created by Daniel Fitzgerald on 12/3/16.
//
//

#ifndef config_h
#define config_h

#define BAUDRATE 9600

#define DEFAULT_SCREEN_WIDTH 500
#define STOP_CLAMP_RADIUS_RATIO 0.1
#define FULLSPEED_CLAMP_RADIUS_RATIO 0.8

typedef int16_t DriveSpeed;
typedef int16_t SteerAngle;
const size_t CMD_MESSAGE_SIZE = sizeof(SteerAngle) + sizeof(DriveSpeed);
#define MAX_DRIVE_SPEED 1000
#define MAX_STEER_ANGLE 1000

#endif /* config_h */
