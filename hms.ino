#include "hms.h"

/****f* 
  *  NAME
  *    time2minutes -- converts hh:mm into minutes only
  *  SYNOPSIS
  *   time2minutes(10,20);
  *  FUNCTION
  *    calculates the number of minutes and sets the internal values;
  *  INPUTS
  *    hh      - number of hours
  *    mm      - number of minutes
  *  RESULT
  *    total number of minutes
   ******
*/
int minute::time2minutes(int h, int m) {
  hh=h;
  mm=m;
  return(hh*60+mm);
}

/****f* 
  *  NAME
  *    minutes2time -- converts minutes to hh:mm
  *  SYNOPSIS
  *   minutes2time(1034);
  *  FUNCTION
  *    converts minutes into hh:mm, sets the internal values;
  *  INPUTS
  *    m       - number of minutes
  *  RESULT
  *    ---
   ******
*/
void minute::minutes2time(int m) {
  hh=m/60;
  mm=m%60;
}


/****f* 
  *  NAME
  *    time2seconds -- converts hh:mm:22 into seconds only
  *  SYNOPSIS
  *   time2seconds(10,20,5);
  *  FUNCTION
  *    calculates the number of seconds and sets the internal values;
  *  INPUTS
  *    hh      - number of hours
  *    mm      - number of minutes
  *    ss      - number of seconds
  *  RESULT
  *    total number of seconds
   ******
*/
long second::time2seconds(int h, int m, int s) {
  ss=s;
  return((long) 60*time2minutes(h,m)+ss);
}

/****f* 
  *  NAME
  *    seconds2time -- converts minutes to hh:mm:ss
  *  SYNOPSIS
  *   seconds2time(78304);
  *  FUNCTION
  *    converts seconds into hh:mm:ss, sets the internal values;
  *  INPUTS
  *    s       - number of seconds
  *  RESULT
  *    ---
   ******
*/
void second::seconds2time(long s) {
  ss=s%60;
  minutes2time(s/60);
}

