/**
 * climate.c
 *
 * Performs analysis on climate data provided by the
 * National Oceanic and Atmospheric Administration (NOAA).
 *
 * Input:    Tab-delimited file(s) to analyze.
 * Output:   Summary information about the data.
 *
 * Compile:  run make
 *
 * Example Run:      ./climate data_tn.tdv data_wa.tdv
 *
 *
 * Opening file: data_tn.tdv
 * Opening file: data_wa.tdv
 * States found: TN WA
 * -- State: TN --
 * Number of Records: 17097
 * Average Humidity: 49.4%
 * Average Temperature: 58.3F
 * Max Temperature: 110.4F
 * Max Temperatuer on: Mon Aug  3 11:00:00 2015
 * Min Temperature: -11.1F
 * Min Temperature on: Fri Feb 20 04:00:00 2015
 * Lightning Strikes: 781
 * Records with Snow Cover: 107
 * Average Cloud Cover: 53.0%
 * -- State: WA --
 * Number of Records: 48357
 * Average Humidity: 61.3%
 * Average Temperature: 52.9F
 * Max Temperature: 125.7F
 * Max Temperature on: Sun Jun 28 17:00:00 2015
 * Min Temperature: -18.7F
 * Min Temperature on: Wed Dec 30 04:00:00 2015
 * Lightning Strikes: 1190
 * Records with Snow Cover: 1383
 * Average Cloud Cover: 54.5%
 *
 * TDV format:
 *
 * CA» 1428300000000»  9prcjqk3yc80»   93.0»   0.0»100.0»  0.0»95644.0»277.58716
 * CA» 1430308800000»  9prc9sgwvw80»   4.0»0.0»100.0»  0.0»99226.0»282.63037
 * CA» 1428559200000»  9prrremmdqxb»   61.0»   0.0»0.0»0.0»102112.0»   285.07513
 * CA» 1428192000000»  9prkzkcdypgz»   57.0»   0.0»100.0»  0.0»101765.0» 285.21332
 * CA» 1428170400000»  9prdd41tbzeb»   73.0»   0.0»22.0»   0.0»102074.0» 285.10425
 * CA» 1429768800000»  9pr60tz83r2p»   38.0»   0.0»0.0»0.0»101679.0»   283.9342
 * CA» 1428127200000»  9prj93myxe80»   98.0»   0.0»100.0»  0.0»102343.0» 285.75
 * CA» 1428408000000»  9pr49b49zs7z»   93.0»   0.0»100.0»  0.0»100645.0» 285.82413
 *
 * Each field is separated by a tab character \t and ends with a newline \n.
 *
 * Fields:
 *      state code (e.g., CA, TX, etc),
 *      timestamp (time of observation as a UNIX timestamp),
 *      geolocation (geohash string),
 *      humidity (0 - 100%),
 *      snow (1 = snow present, 0 = no snow),
 *      cloud cover (0 - 100%),
 *      lightning strikes (1 = lightning strike, 0 = no lightning),
 *      pressure (Pa),
 *      surface temperature (Kelvin)
 */

#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_STATES 50

struct climate_info {
    char code[3];
    unsigned long num_records;
    long double temp;
    long double humidity;
    long double max_temp;
    long int max_temp_date;
    long double min_temp;
    long int min_temp_date;
    int lightning_strikes;
    int snow;
    long double cloud_cover;
    long double pressure;
};

typedef struct climate_info info;

void analyze_file(FILE *file, struct climate_info *states[], int num_states);
void print_report(struct climate_info *states[], int num_states);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s tdv_file1 tdv_file2 ... tdv_fileN \n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Let's create an array to store our state data in. As we know, there are
     * 50 US states. */
    info *states[NUM_STATES] = { NULL };

    int i;
    for (int i = 1; i < argc; ++i) {
        // Declare file pointer and open the file for reading.
        FILE* fp;
        fp = fopen(argv[i], "r");

        /* If the file doesn't exist, print an error message and move on
         * to the next file. */
        if (fp == NULL) {
          printf("Error in opening file.\n");
          return(EXIT_FAILURE);
        }

        // Analyze the file
        printf("Opening file: %s\n", argv[i]);
        analyze_file(fp, states, NUM_STATES);
    }

    // Now that we have recorded data for each file, we'll summarize them:
    print_report(states, NUM_STATES);
    return 0;
}

/*
* analyze_file is a function that opens the specified file,
* extracts climate data, and then stores that data in states.
*/
void analyze_file(FILE *file, struct climate_info **states, int num_states) {
    const int line_sz = 100;
    char line[line_sz];
    while (fgets(line, line_sz, file) != NULL) {

      // Tokenize the line.
      char* token = strtok(line, "\t\n");

      // Create an array to hold all 9 tokens.
      char* tokenList[9];
      int index = 0;

      while (token != NULL && index < 9)
      {
          tokenList[index] = token;
          token = strtok(NULL, "\t\n");
          index++;
      }

      struct climate_info* state = malloc(sizeof(struct climate_info));

      strncpy(state->code, tokenList[0], 2);
      state->min_temp_date = (atof(tokenList[1]))/1000;
      state->max_temp_date = (atof(tokenList[1]))/1000;
      state->humidity = atof(tokenList[3]);
      state->snow = atof(tokenList[4]);
      state->cloud_cover = atof(tokenList[5]);
      state->lightning_strikes = atof(tokenList[6]);
      state->pressure = atof(tokenList[7]);
      state->temp = ((atof(tokenList[8])) * 9 / 5 - 459.67); // Convert K to F
      state->max_temp = ((atof(tokenList[8])) * 9 / 5 - 459.67); // Convert K to F
      state->min_temp = ((atof(tokenList[8])) * 9 / 5 - 459.67); // Convert K to F
      state->num_records = 1;

      char *code = state->code;

      for(int i = 0; i < num_states; i++) {

        // If the state code has not been found before, then add it to states.
        if (states[i] == NULL) {
          states[i] = state;
          break;

        // If the state code has been found before, then update the record in states.
        } else if (strcmp(states[i]->code, code) == 0) {
          states[i]->num_records++;
          states[i]->humidity += state->humidity;
          states[i]->snow += state->snow;
          states[i]->cloud_cover += state->cloud_cover;
          states[i]->lightning_strikes += state->lightning_strikes;
          states[i]->pressure += state->pressure;
          states[i]->temp += state->temp;

          // Check for new max temperature.
          if (states[i]->max_temp <= state->temp) {
            states[i]->max_temp = state->temp;
            states[i]->max_temp_date = state->max_temp_date;
          }

          // Check for new min temperature.
          if (states[i]->min_temp >= state->temp) {
            states[i]->min_temp = state->temp;
            states[i]->min_temp_date = state->min_temp_date;
          }
          break;
        }
      }
    }
  }


/*
* print_report prints a climate report for all states that are found.
*/
void print_report(struct climate_info *states[], int num_states) {
    printf("States found:\n");
    int i;
    for (i = 0; i < num_states; ++i) {
        if (states[i] != NULL) {
            info *info = states[i];
            printf("%s ", info->code);
        }
    }
    printf("\n");

    for (i = 0; i < num_states; ++i) {
        if (states[i] != NULL) {
            struct climate_info *info = states[i];
            printf("-- State: %s --\n", info->code);
            printf("Number of Records: %lu\n", info->num_records);
            printf("Average Humidity: %.1Lf%%\n", (info->humidity / info->num_records));        // Calculate average humidity.
            printf("Average Temperature: %.1LfF\n", (info->temp / info->num_records));          // Calculate average temperature.
            printf("Max Temperature: %.1LfF\n", info->max_temp);
            printf("Max Temperature on: %s", ctime(&info->max_temp_date));                      // Convert unix timestamp to string form).
            printf("Min Temperature: %.1LfF\n", info->min_temp);
            printf("Min Temperature on: %s", ctime(&info->min_temp_date));                      // Convert unix timestamp to string form.
            printf("Lightning Strikes: %d\n", info->lightning_strikes);
            printf("Records with Snow Cover: %d\n", info->snow);
            printf("Average Cloud Cover: %.1Lf%%\n", (info->cloud_cover / info->num_records));  // Calculate average cloud cover.

        }
    }
}
