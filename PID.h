#ifndef PILib
#define PILib

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class PIDLib {
  public:
    // Constructor
    PIDLib(bool displayMsg = false);

    //Methods
    float getControlSignal(float Kp, float Ts, float Ti, float r, float y); 
    float filter(float Tf, float Ts, float u);
    
  private:

};
#endif
