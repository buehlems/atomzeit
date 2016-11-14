#ifndef ATOMZEIT_H
#define ATOMZEIT_H

#include "sun.h" // date
#include "hms.h" // hm

class atomzeit {
 public:
  atomzeit(WiFi *_w);
  int getAtomzeitFromWeb();
  int getSunrise() { return sunrise; }
  int getSunset()  { return sunset; }
  date &getDate() { return date; }
  hm &getTime() { return time; }
  unsigned long getMillis0() { return millis0; }
  
 private:
  url *url;
  int sunrise=-1; // sunrise today in minutes from midnight
  int sunset=-1;  // sunset today in minutes from midnight
  date date;
  hm time;
  unsigned long millis0; // milli counter at 0:00h

};

#endif
