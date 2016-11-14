#ifndef HMS_H
#define HMS_H

typedef struct {
  int hh;
  int mm;
} hm;

typedef struct {
  int hh;
  int mm;
  int ss;
} hms;

class minute {
 private:
  int hh;
  int mm;
 public:
  const static int day=24*60;
  minute(void) { hh=0; mm=0; }
  minute(hm hm) { hh=hm.hh; mm=hm.mm; }
  minute(int m) { minutes2time(m); }
  ~minute(void) { };
  int time2minutes(int h, int m);
  int time2minutes(hm &t) { return time2minutes(t.hh,t.mm); } ;
  void minutes2time(int m);
  int gethh() { return hh; };
  int getmm() { return mm; };
  int getm() { return(60*hh+mm); };
  void gethm(hm &t) {t.hh=gethh(); t.mm=getmm(); };
};

class second : public minute {
 private:
  int ss;
 public:
  const static long day=24l*60l*60l;
  second(void) { ss=0;};
  ~second(void) { };
  long time2seconds(int h, int m, int s);
  int time2seconds(hms &t) { return time2seconds(t.hh,t.mm,t.ss); } ;
  void seconds2time(long s);
  int getss() { return ss; };
  long gets() { return ((long)60*getm()+ss); };
  void gethms(hms &t) {t.hh=gethh(); t.mm=getmm(); t.ss=getss(); };

};
  

#endif
