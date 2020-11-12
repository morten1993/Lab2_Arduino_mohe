#include "PID.h"

int u_max = 5;
int u_min = 0;
float y_prev = 0;
float r_prev = 0;
float u_prev = 0;
float u = 0;



float y2_prev = 0;
float y2 = 0;
float a = 0;

PIDLib::PIDLib(bool displayMsg) {
  // Anything you need when instantiating your object goes here
  
};

// Function
float PIDLib::getControlSignal(float Kp, float Ts, float Ti, float r, float y) {

  u = u_prev + (Kp*(r - y - r_prev + y_prev)+((Kp * Ts)/Ti)*(r-y));

  if(u >= u_max)
    {u = u_max;}


  if(u <= u_min)
    {u = u_min;}

  y_prev = y;
  r_prev = r;
  u_prev = u;
  
  return u;
};

float PIDLib::filter(float Tf, float Ts, float u2)
{
  a = Ts/(Tf+Ts);

  y2 = (1-a)*y2_prev + (a*u2);
  
  y2_prev = y2;  

  return y2;
};
