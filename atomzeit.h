#ifndef ATOMZEIT_H
#define ATOMZEIT_H

#include "sun.h" // date
#include "hms.h" // hm
#include "WiFi.h" // WiFi
#include "url.h" // url

class Atomzeit {
 public:
  Atomzeit(WiFi *_w);
  ~Atomzeit();
  int getAtomzeitFromWeb();
  int getSunrise() { return sunrise; }
  int getSunset()  { return sunset; }
  Date &getDate() { return date; }
  Hm &getTime() { return time; }
  unsigned long getMillis0();
  long millis();
  bool isInitialized();
  long minutes2millis(int m);
  long millisSunrise();
  long millisNextSunrise();
  long millisSunset();
  
 private:
  Url *url;
  int sunrise=-1; // sunrise today in minutes from midnight
  int sunset=-1;  // sunset today in minutes from midnight
  Date date;
  Hm time;
  unsigned long millis0=0; // milli counter at 0:00h

};

#endif
