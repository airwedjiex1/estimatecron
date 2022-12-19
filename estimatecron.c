//  CITS2002 Project 1 2022
//  Student1:   22704037   Kent   Samuel
//  Student2:   23245207   Scaffidi   Henri

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>


//define size given in proj brief
#define MAX_COMMANDNAME_LEN 40
#define MAX_LINE_LEN 100
#define MAX_VALID_LINES 20

//enum to hold relevant ints for there corrosponding dates and days of week
enum month{jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec};
enum week{mon, tue, wed, thur, fri, sat, sun};

//structure to hold data from the crontab file, including execution time added on (from estimates-file)
struct commandData {

        char command_name[MAX_COMMANDNAME_LEN + 1];
        int minute;
        int hour;
        int day;
        int month;
        int day_of_week;
        int execution_time;

};


struct tm ct; //declares current time structure

struct tm tm; //declares crontab time structure

struct tm times[MAX_VALID_LINES]; //declares array of crontab time structures

//an additional struc to hold words from the estimates file
struct {
        char comm_name[MAX_COMMANDNAME_LEN];
        int mins;

} estimates[MAX_LINE_LEN];



//function that opens and returns number of commands in the estimates file
int getCommLinesInFile(char filename[]) {

        FILE *file = fopen(filename, "r");

        if (file == NULL) {
                printf("Could not open file.\n");
                exit(EXIT_FAILURE);
        }

        char line[MAX_LINE_LEN];
        int commCount = 0;

        while (fgets(line, sizeof(line), file) != NULL) {

                for (int i = 0; i < sizeof(line); i++) {

                        bool spaceCheck = (line[i] == ' ' || line[i] == '\t');

                        if (line[i] == '#') {
                                break;
                        }

                        else if (spaceCheck) {
                                continue;
                        }

                        else {
                                commCount++;
                                break;
                        }

                }

        }

        fclose(file);

        return commCount;

}




//converts asterisk to -1 if its an asterisk, otherwise returns integer as it was
//only use this if the input is confirmed valid, and coming in as either * or int
int asterisk_integer(char data[]) {

        if (data[0] == '*') {

                return -1;

        }

        return atoi(data);
}




//checks if a given 3 letter char or int is a valid month, if it is returns its corrosponding int
int check_month(char m[]) {

        char temp_months[12][4] = {""};

        strcpy(temp_months[0], "jan");
        strcpy(temp_months[1], "feb");
        strcpy(temp_months[2], "mar");
        strcpy(temp_months[3], "apr");
        strcpy(temp_months[4], "may");
        strcpy(temp_months[5], "jun");
        strcpy(temp_months[6], "jul");
        strcpy(temp_months[7], "aug");
        strcpy(temp_months[8], "sep");
        strcpy(temp_months[9], "oct");
        strcpy(temp_months[10], "nov");
        strcpy(temp_months[11], "dec");

        for (int i = 0; i <=11; i++) {

                char str[20];
                sprintf(str, "%d", i);

                if ((strcmp(m, temp_months[i]) == 0) || (strcmp(m, str) == 0)) {

                        return i;
                }

        }
        return -1;
}



//checks if a given 3 letter char or int is a valid day of week, if it is returns its corrosponding int
int check_dow(char dow[]) {

        char temp_days[7][4] = {""};

        strcpy(temp_days[0], "sun");
        strcpy(temp_days[1], "mon");
        strcpy(temp_days[2], "tue");
        strcpy(temp_days[3], "wed");
        strcpy(temp_days[4], "thu");
        strcpy(temp_days[5], "fri");
        strcpy(temp_days[6], "sat");

        for (int i = 0; i <= 6; i++) {

                char str[20];
                sprintf(str, "%d", i);

                if ((strcmp(dow, temp_days[i]) == 0) || (strcmp(dow, str) == 0)) {

                        int dow;
                        dow = i; //save month as an int
                        return dow;

                }

        }
        return -1;
}


//Takes a valid line from the crontab file and passes it into our tm struc,
void parse_cronline(char line[], int lpos) {

        char *words[MAX_LINE_LEN];

        //remove tabs for continuity
        for (int i = 0; i < strlen(line); i++) {

                if (line[i] == '\t') {

                        line[i] = ' ';

                }

        }

        const char s[2] = " ";
        char *token;
        token = strtok(line, s);
        int i = 0;

        //iterate through other tokens
        while (token != NULL) {

                words[i] = token;
                i++;
                token = strtok(NULL, s);

        }

        //assign min
        if (strcmp(words[0], "*") == 0) {

                times[lpos].tm_min = -1;
        }

        else {

                times[lpos].tm_min = atoi(words[0]);

        }

        //assign hour
        if (strcmp(words[1], "*") == 0) {

                times[lpos].tm_hour = -1;

        }

        else {

                times[lpos].tm_hour = atoi(words[1]);
        }

        //assign day of month
        if (strcmp(words[2], "*") == 0) {

                times[lpos].tm_mday = -1;

        }

        else {

                times[lpos].tm_mday = atoi(words[2]);
        }


        //assign month
        if (strcmp(words[3], "*") == 0) {

                times[lpos].tm_mon = -1;

        }

        else {
                int mon_int;
                mon_int = check_month(words[3]);
                times[lpos].tm_mon = mon_int;
        }

        //assign Day of week
        if (strcmp(words[4], "*") == 0) {

                times[lpos].tm_wday = -1;

        }

        else {
                int dow_int;
                dow_int = check_dow(words[4]);
                times[lpos].tm_wday = dow_int;
        }
}


//Checks that all words on the estimates line are valid and that there are exactly 2 words otherwise exits with failure
//If the words are valid parse them into our estimates struc
void check_est_line(char line[], int line_n, int vc) {

        const char *words[100];

        //remove tabs for continuity
        for (int i = 0; i < strlen(line); i++) {

                if (line[i] == '\t') {

                        line[i] = ' ';

                }
        }

        const char s[2] = " ";
        char *token;
        token = strtok(line, s);
        int i = 0;

        //iterate through other tokens
        while (token != NULL) {

                words[i] = token;
                i++;
                token = strtok(NULL, s);

        }

        if (i != 2) {

                printf("line %i unrecognized in estimates-file: '%s'", (line_n + 1), line);
                exit(EXIT_FAILURE);

        }

        int mins = atoi(words[1]);
        if (mins <= 0) {

                printf("line %i unrecognized in estimates-file: '%s'", (line_n + 1), line);
                exit(EXIT_FAILURE);

        }

        //words have been checked so parse the commname and mins into our estimates struc
        //estimates[vc].comm_name = *words[0];
        //char str[20] = words[0];
        strcpy(estimates[vc].comm_name, words[0]);
        estimates[vc].mins = atoi(words[1]);

}



//Checks that all words on the estimates line are valid and that there are 6 or more words on the line otherwise exits with failure
//If the words are valid parse them into our array of time structures at the correct index
void check_cron_line(char line[], int line_n) {


        char *words[MAX_LINE_LEN];

        //remove tabs for continuity
        for (int i = 0; i < strlen(line); i++) {

                if (line[i] == '\t') {

                        line[i] = ' ';
                }

        }

        const char s[2] = " ";
        char *token;
        token = strtok(line, s);
        int i = 0;

        //iterate through other tokens
        while (token != NULL) {

                words[i] = token;
                i++;
                token = strtok(NULL, s);

        }

        printf("%d\n", i );

        if (i < 6) {

                printf("line %i unrecognized in crontab-file", line_n + 1);
                exit(EXIT_FAILURE);

        }

        if ((strcmp(words[0], "*") == 0)) {
                ;
        }

        else {
                int mins = atoi(words[0]);
                if (mins < 0 || mins > 59) {

                        printf("line %i unrecognized in crontab-file", line_n + 1);
                        exit(EXIT_FAILURE);

                }
        }

        if ((strcmp(words[1], "*") == 0)) {
                ;
        }

        else {
                int hours = atoi(words[1]);
                if (hours < 0 || hours > 23) {

                        printf("line %i unrecognized in crontab-file", line_n + 1);
                        exit(EXIT_FAILURE);

                }
        }

        if ((strcmp(words[2], "*") == 0)) {
                ;
        }

        else {
                int dom = atoi(words[2]);
                printf("    %d    ", dom);
                if( dom < 1 || dom > 31) {

                        printf("line %i unrecognized in crontab-file", line_n + 1);
                        exit(EXIT_FAILURE);

                }
        }

        if ((strcmp(words[3], "*") == 0)) {
                ;
        }

        else {
                int month = atoi(words[3]);
                if(month < 0 || month > 11) {

                        printf("line %i unrecognized in crontab-file", line_n + 1);
                        exit(EXIT_FAILURE);

                }
        }

        if ((strcmp(words[4], "*") == 0)) {
                ;
        }

        else {
                int dow = atoi(words[4]);
                if (dow < 0 || dow > 6) {

                        printf("line %i unrecognized in crontab-file", line_n + 1); //counting lines start at 1 for error report
                        exit(EXIT_FAILURE);

                }
        }
}



//checks if line is a comment line - returns true if a comment line
bool isComment (char lines[]) {

        for (int j = 0; j < strlen(lines); j++) {

                bool spaceCheck = (lines[j] == ' ' || lines[j] == '\t');

                if (spaceCheck) {
                        continue;
                }

                else if (!spaceCheck && lines[j] != '#') {
                        return false;

                }

                else {
                        return true;
                }
        }

        return true;
}


//Opens the estimates file and passes each line to our check line function
//also, counts the number of lines in the file.
void read_estimates(char estfile[]) {

        //ATTEMPT TO OPEN THE FILE FOR READ-ACCESS
        FILE   *est;
        est = fopen(estfile, "r");

        //CHECK IF ANYTHING WENT WRONG
        if (est == NULL) {

                printf( "cannot open estimates file: '%s'\n", estfile);
                exit(EXIT_FAILURE);

        }

        int lnum = 0; //store the number of the current line, including commented lines
        int vc = 0; //counts the number of valid command lines (non-comment), max = 20
        char line[MAX_LINE_LEN]; //holds the line from the textfile as a string
        char lines[20][MAX_LINE_LEN];

        while (fgets(line, sizeof line, est) != NULL) {

                strcpy(lines[lnum], line);
                lnum++;

        }


        //int check_ret[lnum];
        for (int i = 0; i < lnum; i++) {

                if (!isComment(lines[i])) {

                        check_est_line(lines[i], i, vc);
                        vc++;
                }
        }



}



//Opens the crontab file and passes each line to our check line function
//also, counts the number of lines in the file.
void read_crontab(char cronfile[]) {


        //ATTEMPT TO OPEN THE FILE FOR READ-ACCESS
        FILE   *cron;
        cron = fopen(cronfile, "r");

        //CHECK IF ANYTHING WENT WRONG
        if (cron == NULL) {

                printf( "cannot open crontab file: '%s'\n", cronfile);
                exit(EXIT_FAILURE);

        }

        int lnum = 0; //store the number of the current line, including commented lines
        int vcount = 0; //counts the number of valid command lines (non-comment), max = 20
        char line[MAX_LINE_LEN]; //holds the line from the textfile as a string
        char lines[20][MAX_LINE_LEN];

        while (fgets(line, sizeof line, cron) != NULL) {



                if(!isComment(line)) { //ignore commented lines in the file, leave func if the line is a comment

                        //puts(line);
                        vcount++;

                }
                strcpy(lines[lnum], line);
                lnum++;
        }

        //int check_ret[lnum];
        for (int i = 0; i < lnum; i++) {

                if (!isComment(lines[i])) {

                        printf("%d", i);
                        check_cron_line(lines[i], i);

                }

        }

        rewind(cron);

        //Since the file's lines have now been checked, we can read each word into our time.h struc
        //set the fields of our tm struc to 0 to start
        memset(&tm, 0, sizeof(tm));

        //lines have been checked so send them to be placed into our tm struc
        int lpos = 0;

        while (fgets(line, sizeof line, cron) != NULL) {

                if(!isComment(line)) {

                        parse_cronline(line, lpos);
                        lpos++;

                }
        }
}



//Iterates through every min of the given month using our ct current time struc
//prints the required result
void simulate(int month, struct commandData data[]) {

        //check crontab struc


        for (int i = 0; i < 6; i++) {

                //printf("\n%s\n", cron_commname[i]);
                printf("minute: %i\n", times[i].tm_min);
                printf("hour: %i\n", times[i].tm_hour);
                printf("day: %i\n", times[i].tm_mday);
                printf("month: %i\n", times[i].tm_mon);
                printf("dow: %i\n", times[i].tm_wday);

        }

        printf("ESTIMATES:\n");
        //for (int i = 0; i < 5; i++) {

        //printf("\n%s\n", estimates[i].comm_name);
        //printf("\n%d\n", estimates[i].mins);

        //}

        //check est struc

        memset(&tm, 0, sizeof(ct)); //empty struc

        ct.tm_mon = month;
        ct.tm_year = 2022-1900;
        ct.tm_mday = 1;
        ct.tm_min = 0;
        ct.tm_hour = 0;

        mktime(&ct);

        //printf("tm_wday == %d\n", ct.tm_wday);

        //mktime(&times[0]);

        //puts(asctime(&ct));

        int max_cr = 0; //to store the maximum number of commands running at one time

        //setup array to track executions and running commands
        int trackingArrayLen = (2 * MAX_VALID_LINES) + 2;
        int trackingArray[trackingArrayLen];
        for (int i = 0; i < trackingArrayLen; i++) {

                trackingArray[i] = 0;
                printf("%i\n", trackingArray[i]);

        }

        bool first = false;

        while (ct.tm_mon == month) {


                mktime(&ct);
                puts(asctime(&ct));

                for (int i = 0; i < MAX_VALID_LINES; i++) {

                        if (times[i].tm_mon == month || times[i].tm_mon == -1) {
                                if (ct.tm_wday == times[i].tm_wday || times[i].tm_wday == -1) {
                                        if (ct.tm_hour == times[i].tm_hour || times[i].tm_hour == -1) {
                                                if (ct.tm_min == times[i].tm_min || times[i].tm_min == -1) {
                                                        if (ct.tm_mday == times[i].tm_mday || times[i].tm_mday == -1) {



                                                                trackingArray[trackingArrayLen - 2] += 1; //updates cr (commands running)
                                                                trackingArray[trackingArrayLen - 1] += 1; //updates tr (total ran/invoked)
                                                                trackingArray[i] += 1;                    //updates cj (jth commands running)
                                                                trackingArray[i + MAX_VALID_LINES] += 1; //updates tj (total jth commands ran/invoked)

                                                        }
                                                }
                                        }
                                }
                        }

                        //obtaining estimate time
                        int exeTime = 0;
                        for (int j = 0; j < MAX_VALID_LINES; j++) {
                                if (strcmp(data[i].command_name, estimates[j].comm_name) == 0) {

                                        exeTime = estimates[j].mins;

                                }
                        }

                        //moving back in time to check if command was executed, and now needs to be terminated
                        ct.tm_min -= exeTime;
                        mktime(&ct);

                        if (times[i].tm_mon == month || times[i].tm_mon == -1) {
                                if (ct.tm_wday == times[i].tm_wday || times[i].tm_wday == -1) {
                                        if (ct.tm_hour == times[i].tm_hour || times[i].tm_hour == -1) {
                                                if (ct.tm_min == times[i].tm_min || times[i].tm_min == -1) {
                                                        if (first == true && (ct.tm_mday == times[i].tm_mday || times[i].tm_mday == -1)) {

                                                                trackingArray[trackingArrayLen - 2] -= 1; //updates cr
                                                                trackingArray[i] -= 1; //updates cj

                                                        }
                                                }
                                        }
                                }
                        }

                        //correcting the current time
                        ct.tm_min += exeTime;
                        mktime(&ct);

                }

                //checks if a new max cr
                if (max_cr < trackingArray[trackingArrayLen - 2]) {

                        max_cr = trackingArray[trackingArrayLen - 2];

                }

                ct.tm_min++;
                first = true;
        }

        for (int i = 0; i < MAX_VALID_LINES; i++) {

                if (times[i].tm_mon == -1 && times[i].tm_wday == -1 && times[i].tm_hour == -1 && times[i].tm_mday == -1 && times[i].tm_min == -1) {

                        trackingArray[trackingArrayLen - 1] -= 1;
                        trackingArray[i + MAX_VALID_LINES] -= 1;

                }

        }

        //finding index of most freq executed command
        int max_tj_index = 0;
        int max_tj = 0;

        for (int tj = MAX_VALID_LINES; tj < (2 * MAX_VALID_LINES); tj++) {

                if (trackingArray[tj] > max_tj) {

                        max_tj = trackingArray[tj];
                        max_tj_index = tj;

                }

        }

        int freqInd = max_tj_index - MAX_VALID_LINES;

        //output
        printf("\n");
        printf("%s ", data[freqInd].command_name);
        printf("%i ", trackingArray[trackingArrayLen - 1]);
        printf("%i ", max_cr);

}


//writes to an additonal struct to hold crontab date without time format
void readCronFile(char crontab[], struct commandData data[]) {

        FILE *cron = fopen(crontab, "r");

        if (cron == NULL) {
                printf("Could not open crontab-file.\n");
                exit(EXIT_FAILURE);
        }

        char line[MAX_LINE_LEN];
        int commInd = 0;

        while (fgets(line, sizeof(line), cron) != NULL) {

                bool minStart = false;
                bool minDone = false;
                int minInd = 0;
                char minute[MAX_LINE_LEN] = "";

                bool hrStart = false;
                bool hrDone = false;
                int hrInd = 0;
                char hour[MAX_LINE_LEN] = "";

                bool dayStart = false;
                bool dayDone = false;
                int dayInd = 0;
                char day[MAX_LINE_LEN] = "";

                bool monthStart = false;
                bool monthDone = false;
                int monthInd = 0;
                char month[MAX_LINE_LEN] = "";

                bool dowStart = false;
                bool dowDone = false;
                int dowInd = 0;
                char dow[MAX_LINE_LEN] = "";

                bool nameStart = false;
                bool nameDone = false;
                int nameInd = 0;
                char name[MAX_COMMANDNAME_LEN + 1] = "";


                for (int i = 0; i < sizeof(line); i++) {

                        bool spaceCheck = (line[i] == ' ' || line[i] == '\t');

                        if (line[i] == '#') {
                                break;
                        }

                        else if (spaceCheck && minStart == false) {
                                continue;
                        }

                        else if (!spaceCheck && minDone == false) {
                                minStart = true;
                                minute[minInd] = line[i];
                                minInd++;
                        }

                        else if (spaceCheck && hrStart == false) {
                                minDone = true;
                                continue;
                        }

                        else if (!spaceCheck && hrDone == false) {
                                hrStart = true;
                                hour[hrInd] = line[i];
                                hrInd++;
                        }
                        else if (spaceCheck && dayStart == false) {
                                hrDone = true;
                                continue;
                        }

                        else if (!spaceCheck && dayDone == false) {
                                dayStart = true;
                                day[dayInd] = line[i];
                                dayInd++;
                        }
                        else if (spaceCheck && monthStart == false) {
                                dayDone = true;
                                continue;
                        }

                        else if (!spaceCheck && monthDone == false) {
                                monthStart = true;
                                month[monthInd] = line[i];
                                monthInd++;
                        }

                        else if (spaceCheck && dowStart == false) {
                                monthDone = true;
                                continue;
                        }

                        else if (!spaceCheck && dowDone == false) {
                                dowStart = true;
                                dow[dowInd] = line[i];
                                dowInd++;
                        }

                        else if (spaceCheck && nameStart == false) {
                                dowDone = true;
                                continue;
                        }

                        else if (!spaceCheck && nameDone == false && line[i] != '\n') {
                                nameStart = true;
                                name[nameInd] = line[i];
                                nameInd++;
                        }

                        else if ((spaceCheck || line[i] == '\n') && nameStart == true) {
                                name[nameInd] = '\0';
                                break;
                        }

                }

                if (strcmp(minute, "") != 0) {

                        data[commInd].minute = asterisk_integer(minute);

                        data[commInd].hour = asterisk_integer(hour);

                        data[commInd].day = asterisk_integer(day);

                        data[commInd].month = asterisk_integer(month);

                        data[commInd].day_of_week = asterisk_integer(dow);

                        strcpy(data[commInd].command_name, name);

                        commInd++;

                }
        }

        fclose(cron);
}







int main(int argcount, char *argvalue[])
{
        //check the number of args given is exactly 4
        if (argcount != 4) {

                fprintf(stderr, "Usage: %s month[integer 0..11 or a 3-character name] crontab-file[text file] estimates-file[text file] \n", argvalue[0]);
                exit(EXIT_FAILURE);                     // Exit indicating failure

        }

        //creating structs of crontab data (not time format)
        int numCommandsinCron = getCommLinesInFile(argvalue[2]);
        struct commandData crontab_data[numCommandsinCron];
        readCronFile(argvalue[2], crontab_data);


        //func checks appropriate 3 char string or int has been given as the month
        if (check_month(argvalue[1]) < 0) {

                fprintf(stderr, "Usage: %s month[integer 0..11 or a 3-character name] crontab-file[text file] estimates-file[text file] \n", argvalue[0]);
                fprintf(stderr, "Month invalid \n");
                exit(EXIT_FAILURE);

        }

        //saves the month given by the command line as an int
        int given_month = check_month(argvalue[1]);

        printf("GV:%d\n", given_month);
        //char line[40] = "0 2  * *      send-monthly-newsletter";
        //check_cron_line(line, 7);

        //Validate and read the crontab file into relevant time strucs
        read_crontab(argvalue[2]);

        //Validate and read the estimates file into relevant time strucs
        read_estimates(argvalue[3]);

        //Simulate
        simulate(given_month, crontab_data);


        return EXIT_SUCCESS;

}
