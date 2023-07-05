#include "calc.h"
#include "Arduino.h"

// https://github.com/SeBassTian23/ESP32-WeatherStation/blob/main/src/calculations.cpp

/*
 * The Heat Index Equation
 * 
 * Source: https://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
 * 
 * The computation of the heat index is a refinement of a result obtained by multiple regression analysis carried out by Lans P. Rothfusz and described in a 
 * 1990 National Weather Service (NWS) Technical Attachment (SR 90-23).  The regression equation of Rothfusz is
 * HI = -42.379 + 2.04901523*T + 10.14333127*RH - .22475541*T*RH - .00683783*T*T - .05481717*RH*RH + .00122874*T*T*RH + .00085282*T*RH*RH - .00000199*T*T*RH*RH
 * where T is temperature in degrees F and RH is relative humidity in percent.  HI is the heat index expressed as an apparent temperature in degrees F.
 * 
 * If the RH is less than 13% and the temperature is between 80 and 112 degrees F, then the following adjustment is subtracted from HI:
 * ADJUSTMENT = [(13-RH)/4]*SQRT{[17-ABS(T-95.)]/17}
 * where ABS and SQRT are the absolute value and square root functions, respectively.
 * 
 * On the other hand, if the RH is greater than 85% and the temperature is between 80 and 87 degrees F, then the following adjustment is added to HI:
 * ADJUSTMENT = [(RH-85)/10] * [(87-T)/5]
 * 
 * The Rothfusz regression is not appropriate when conditions of temperature and humidity warrant a heat index value below about 80 degrees F.
 * In those cases, a simpler formula is applied to calculate values consistent with Steadman's results:
 * HI = 0.5 * {T + 61.0 + [(T-68.0)*1.2] + (RH*0.094)}
 * 
 * In practice, the simple formula is computed first and the result averaged with the temperature. If this heat index value is 80 degrees F or higher,
 * the full regression equation along with any adjustment as described above is applied.
 * The Rothfusz regression is not valid for extreme temperature and relative humidity conditions beyond the range of data considered by Steadman.
 */

double heatIndex( double T, double RH ){
    double HI = 0.5 * (T + 61.0 + ((T-68.0)*1.2) + (RH*0.094));
    if(HI > 80.0)
    {
        HI = -42.379 + 2.04901523*T + 10.14333127*RH - .22475541*T*RH - .00683783*T*T - .05481717*RH*RH + .00122874*T*T*RH + .00085282*T*RH*RH - .00000199*T*T*RH*RH;
        if( RH < 13.0 && T >= 80.0 && T <= 112.0)
        {
            HI -= ((13.0-RH)/4)*sqrt((17.0-abs(T-95.0))/17);
            HI = HI;
        }
        else if (RH > 85.0 && T >= 80.0 && T <= 87.0 )
        {
            HI += ((RH-85)/10) * ((87-T)/5);
        }
    }
    return HI;
}
