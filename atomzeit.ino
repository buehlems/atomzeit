// #include <Arduino.h>


#include "util.h"
#include "url.h"

/****f* 
  *  NAME
  *    Atomzeit -- 
  *  SYNOPSIS
  *   Atomzeit az=Atomzeit(wifi);
  *  FUNCTION
  *    define Atomzeit
  *  INPUTS
  *    wifi      - a wifi
  *  RESULT
  *    ---
   ******
*/
Atomzeit::Atomzeit(WiFi *_w){
  url=new Url(_w);
}

Atomzeit::~Atomzeit(){
  delete url;
}

/****f* 
  *  NAME
  *    getAtomZeitFromWeb -- 
  *  SYNOPSIS
  *   int retcode=getAtomZeitFromWeb();
  *  FUNCTION
  *    get current time values from atomzeit.eu:
  *        date & time 
  *        sunrise, sunset in minutes from midnight
  *        millis0: milli counter at 0:00h
  *  INPUTS
  *    
  *  RESULT
  *    return code (see url.h)
   ******
*/
int Atomzeit::getAtomzeitFromWeb(){
  int retCode=url->ok;
  util::println("********** getAtomzeitFromWeb **********");
  // retCode=url->requestWebPage("www.uhrzeit.org","/atomuhr.php");
  retCode=url->requestWebPage("www.atomzeit.eu","/");
  util::printfln("requestWebPage retCode=%d",retCode);

  // get date and time
  url->resetRespBuf();
  retCode=url->findString("Aktuelle Zeit:",url->removeSearchString);

  WiFiSocket *socket=url->getSocket();
  util::printfln("findString retCode=%d\nBuffer=%s",retCode,url->getBuf());
    
  Date ad; // atom date
  sscanf(url->getBuf(),"%d.%d.%d%*s%d:%d",&date.d,&date.m,&date.y,&time.h,&time.m);
  util::printfln("date: %d.%d.%d time: %d:%d",date.d,date.m,date.y,time.h,time.m);

  // get sunrise
  Hm tmp;
  retCode=url->findString("<b>Sonnenaufgang:</b> ",url->removeSearchString);
  sscanf(url->getBuf(),"%d:%d",&tmp.h,&tmp.m);
  Minute sunrise(tmp); // sunrise and sunset

  util::msgln("findString retCode=%d\nBuffer=%s",retCode,url->getBuf());   
  util::msgln("sunrise Berlin: %d:%d",sunrise.geth(),sunrise.getm());

  retCode=url->findString("Sonnenuntergang:",url->closeAfterFind | url->removeSearchString);
  sscanf(url->getBuf(),"%d:%d",&tmp.h,&tmp.m);
  Minute sunset(tmp);

  util::printfln("findString retCode=%d\nBuffer=%s",retCode,url->getBuf());   
  util::printfln("sunset Berlin: %d:%d",sunset.geth(),sunset.getm());

  // now calc the minutes of sunrise and sunset from midnight
  Sun sunRiseSet(date.y);
  this->sunrise=sunrise.getMinutesDay()+sunRiseSet.adjustSunRise(date); 
  this->sunset =sunset.getMinutesDay()+sunRiseSet.adjustSunSet(date); 
    
  // now calculate the millis at last midnight
  unsigned long t=millis();
  unsigned long millis_since_midnight=(time.h*60+time.m)*60000;
  this->millis0=t-millis_since_midnight;
    
  return retCode;
}



/****f* 
  *  NAME
  *    get value of the millis counter at midnight today
  *  SYNOPSIS
  *   unsigned long m=getMillis0();
  *  FUNCTION
  *    getAtomzeit must have run before to get the actual time and calculate the millis counter at the beginning of this day
  *  INPUTS
  *    
  *  RESULT
  *    millis couner at midnight (see url.h)
   ******
*/
unsigned long Atomzeit::getMillis0() { 
  return this->millis0; 
}

/****f* 
  *  NAME
  *    calculate the milliseconds passed since midnight
  *  SYNOPSIS
  *   long m=millis();
  *  FUNCTION
  *    getAtomzeit must have run before to get the actual time and calculate the millis counter at the beginning of this day
  *  INPUTS
  *    
  *  RESULT
  *    millis seconds since midnight. -1 if invalid
   ******
*/
long Atomzeit::millis() { 
  if(getMillis0()==0 && ! isInitialized()){
    return -1;
  }
  unsigned long t=millis();
  return (long)(t-getMillis0());
}

/****f* 
  *  NAME
  *    check if Atomzeit has been initialized
  *  SYNOPSIS
  *   bool isinit=atomzeit.isInitialized();
  *  FUNCTION
  *    In order to properly set all internal structures, you must get the current time from the web by calling 
  *    getAtomzeitFromWeb. This will set sunrise and sunset. This function will check of sunrise and sunset are > =0 and then
  *    return true.
  *  INPUTS
  *    
  *  RESULT
  *    t if initialized
   ******
*/
bool Atomzeit::isInitialized() { 
  return (this->sunrise>=0 && this->sunset>=0) ? true:false;
}

/****f* 
  *  NAME
  *    calculate the millis of a point in time
  *  SYNOPSIS
  *   long md=millis(minutes);
  *  FUNCTION
  *    getAtomzeit must have run before to get the actual time and calculate the millis counter at the beginning of this day
  *  INPUTS
  *    minutes - the minutes since midnight
  *  RESULT
  *    milli seconds since midnight. -1 if invalid
   ******
*/
long Atomzeit::minutes2millis(int m) { 
  return((long)m * 60 * 1000);
}


/****f* 
  *  NAME
  *    calculate the millis of the sunrise today
  *  SYNOPSIS
  *   long m=millisSunrise();
  *  FUNCTION
  *    getAtomzeit must have run before to get the actual time and calculate the millis counter at the beginning of this day
  *  INPUTS
  *    
  *  RESULT
  *    millis seconds since midnight. -1 if invalid
   ******
*/
long Atomzeit::millisSunrise() { 
  return(minutes2millis(this->sunrise));
}

/****f* 
  *  NAME
  *    calculate the millis of the sunrise of the next day
  *  SYNOPSIS
  *   long m=millisNextSunrise();
  *  FUNCTION
  *    getAtomzeit must have run before to get the actual time and calculate the millis counter at the beginning of this day
  *  INPUTS
  *    
  *  RESULT
  *    millis seconds since midnight today. -1 if invalid
   ******
*/
long Atomzeit::millisNextSunrise() { 
  return(minutes2millis(this->sunrise+60*24));
}


/****f* 
  *  NAME
  *    calculate the millis of the sunset today
  *  SYNOPSIS
  *   long m=millisSunset();
  *  FUNCTION
  *    getAtomzeit must have run before to get the actual time and calculate the millis counter at the beginning of this day
  *  INPUTS
  *    
  *  RESULT
  *    millis seconds since midnight. -1 if invalid
   ******
*/
long Atomzeit::millisSunset() { 
  return(minutes2millis(this->sunset));
}