#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <RTClib.h> 
#include <Time.h>


#include "util.h"

#include "WiFi.h"
#include "url.h"

/****f* 
  *  NAME
  *    atomzeit -- 
  *  SYNOPSIS
  *   atomzeit az=atomzeit(wifi);
  *  FUNCTION
  *    define atomzeit
  *  INPUTS
  *    wifi      - a wifi
  *  RESULT
  *    ---
   ******
*/
atomzeit::atomzeit(WiFi *_w){
  url=new url(_w);
}

atomzeit::~atomzeit(){
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
int atomzeit:getAtomzeitFromWeb(){
  int retCode=url->ok;
    util::println("********** getAtomzeitFromWeb **********");
    // retCode=url->requestWebPage("www.uhrzeit.org","/atomuhr.php");
    retCode=url->requestWebPage("www.atomzeit.eu","/");
    util::printfln("requestWebPage retCode=%d",retCode);

    // get date and time
    url->resetRespBuf();
    retCode=url->findString("Aktuelle Zeit:",url::removeSearchString);

    WiFiSocket *socket=url->getSocket();
    util::printfln("findString retCode=%d\nBuffer=%s",retCode,url->getBuf());
    
     date ad; // atom date
    sscanf(url->getBuf(),"%d.%d.%d%*s%d:%d",&date.d,&date.m,&date.y,&time.h,&time.m);
    util::printfln("date: %d.%d.%d time: %d:%d",date.d,date.m,date.y,time.h,time.m);

    // get sunrise
    hm tmp;
    retCode=url->findString("<b>Sonnenaufgang:</b> ",url::removeSearchString);
    sscanf(url->getBuf(),"%d:%d",&tmp.h,&tmp.m);
    minute sr(tmp);

    util::msgln("findString retCode=%d\nBuffer=%s",retCode,url->getBuf());   
    util::msgln("sunrise: %d:%d",sr.gethh(),sr.getmm());

    retCode=url->findString("Sonnenuntergang:",url::closeAfterFind | url::removeSearchString);
    sscanf(url->getBuf(),"%d:%d",&tmp.h,&tmp.m);
    minute ss(tmp)

    util::printfln("findString retCode=%d\nBuffer=%s",retCode,url->getBuf());   
    util::printfln("sunset: %d:%d",ss.gethh(),ss.getmm());

    // now calc the minutes of sunrise and sunset from midnight
    sun sunRiseSet(date.y);
    this->sunrise=sr.getm()+sunRiseSet.adjustSunrise(date); 
    this->sunset =ss.getm()+sunRiseSet.adjustSunset(date); 
    
    // now calculate the millis at last midnight
    unsigned long t=millis();
    unsigned long millis_since_midnight=(time.h*60+time.m)*60000;
    this->millis0=t-millis_since_midnight;
    
    return retCode;
  }
}


