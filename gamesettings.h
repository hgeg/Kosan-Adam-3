#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H


typedef struct{
  float gravity;
  float airFriction;
  float groundFriction;
  float ySpeedTerminal;
  float yJumpSpeed;
  float xSpeedRunning;
  float xSpeedWalking;
  float xAcceleration;
  float yClimbingSpeed;
}GameSettings;

#endif
