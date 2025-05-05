#ifndef SOLENOID_H
#define SOLENOID_H

class SOLENOID {
  private:
    int pin;

  public:
    SOLENOID(int solenoidPin);

    void on();

    void off();
};
#endif



