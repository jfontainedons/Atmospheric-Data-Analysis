# Atmospheric-Data-Analysis

Analysis of data from the National Oceanic and Atmospheric Administration (NOAA) North American Mesoscale Forecast System.

Each file contains one record per line with fields separated by tab characters (‘\t’). The columns are organized as
follows:
TN 1424325600000 dn20t1kz0xrz 67.0 0.0 0.0 0.0 101872.0 262.5665
TN 1422770400000 dn2dcstxsf5b 23.0 0.0 100.0 0.0 100576.0 277.8087
TN 1422792000000 dn2sdp6pbb5b 96.0 0.0 100.0 0.0 100117.0 278.49207
TN 1422748800000 dn2fjteh8e80 6.0 0.0 100.0 0.0 100661.0 278.28485
TN 1423396800000 dn2k0y7ffcup 14.0 0.0 100.0 0.0 100176.0 282.02142

#### Output:
./climate data_tn.tdv data_wa.tdv
Opening file: data_tn.tdv
Opening file: data_wa.tdv
States found: TN WA
-- State: TN --
Number of Records: 17097
Average Humidity: 49.4%
Average Temperature: 58.3F
Max Temperature: 110.4F
Max Temperature on: Mon Aug 3 11:00:00 2015
Min Temperature: -11.1F
Min Temperature on: Fri Feb 20 04:00:00 2015
Lightning Strikes: 781
Records with Snow Cover: 107
Average Cloud Cover: 53.0%
-- State: WA --
Number of Records: 48357
Average Humidity: 61.3%
Average Temperature: 52.9F
Max Temperature: 125.7F
Max Temperature on Sun Jun 28 17:00:00 2015
Min Temperature: -18.7F
Min Temperature on Wed Dec 30 04:00:00 2015
Lightning Strikes: 1190
Records with Snow Cover: 1383
Average Cloud Cover: 54.5%
