#include <Arduino.h>

#include <SPI.h>
#include <Wire.h>
#include <RTClib.h> 
#include <Time.h>


// #include "sma_web.h"
#include "util.h"

#include "WiFi.h"
#include "url.h"
WiFi wifi(5000); // 5 sec is needed to find out that URL is not valid
url url(&wifi); 

 
int atomzeit:getAtomzeitFromWeb(){
  int retCode=0;
    util::println("********** getAtomzeitFromWeb **********");
    // retCode=url.requestWebPage("www.uhrzeit.org","/atomuhr.php");
    retCode=url.requestWebPage("www.atomzeit.eu","/");
    util::printfln("requestWebPage retCode=%d",retCode);

    // get date and time
    url.resetRespBuf();
    retCode=url.findString("Aktuelle Zeit:",url::removeSearchString);

    WiFiSocket *socket=url.getSocket();
    util::printfln("findString retCode=%d\nBuffer=%s",retCode,url.getBuf());
    
    int day,month,year,h,m;
    hm at; // atom time
    date ad; // atom date
    sscanf(url.getBuf(),"%d.%d.%d%*s%d:%d",&ad.d,&ad.m,&ad.y,&at.h,&at.m);
    util::printfln("date: %d.%d.%d time: %d:%d",ad.d,ad.m,ad.y,at.h,at.m);

    // get sunrise
    date tmp;
    retCode=url.findString("<b>Sonnenaufgang:</b> ",url::removeSearchString);
    sscanf(url.getBuf(),"%d:%d",&tmp.h,&tmp.m);
    minute sr(tmp);

    util::printfln("findString retCode=%d\nBuffer=%s",retCode,url.getBuf());   
    util::printfln("sunrise: %d:%d",sr.gethh(),sr.getmm());

    retCode=url.findString("Sonnenuntergang:",url::closeAfterFind | url::removeSearchString);
    sscanf(url.getBuf(),"%d:%d",&tmp.h,&tmp.m);
    minute ss(tmp)

    util::printfln("findString retCode=%d\nBuffer=%s",retCode,url.getBuf());   
    util::printfln("sunset: %d:%d",ss.gethh(),ss.getmm());

    // now calc the minutes of sunrise and sunset from midnight
    sun sunRiseSet(ad.y);
    this->sunrise=sr.getm()+ad.adjustSunrise(ad); // need to write adjustSunrise
    this->sunset =ss.getm()+ad.adjustSunset(ad); // need to write adjustSunrise
    return 
  }
}


