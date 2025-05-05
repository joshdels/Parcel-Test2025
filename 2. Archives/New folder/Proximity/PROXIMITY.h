#ifndef PROXIMITY_H
#define PROXIMITY_H

class PROXIMITY {
  private:
    int trig;
    int echo;
    int ceiling_limit;

  public:
    PROXIMITY(int trigPin, int echoPin, int ceiling_limit);

    long get_distance();

    bool check_ceiling_limit(long distance);
};
#endif