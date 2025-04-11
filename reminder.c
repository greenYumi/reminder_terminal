#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef char * string;


// error hendling;

typedef enum {
    BAD_USAGE,
    BAD_OPTION,
    BAD_ARGUMENT,
    MISSING_ARGUMENT,
    BAD_CONTROLER
} E_ERROR_FLAG;

void usage(char *argv[], E_ERROR_FLAG);

typedef struct {
    char note[100];
    int date;
    int month;
    int year;
} Reminder;

typedef struct {
    int size;
    Reminder* reminders;
} ReminderBox;


// ### Utility

// typedef enum {Minggu, Senin, Selasa, Rabu, Kamis, Jumat, Sabtu} Eind_DAY;
// char *hari[] = 
typedef enum {Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday} Een_DAY;
char *wday[7] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
char *mon[12] = {
    "January", "February", "March", "April", "Mey", "June", 
    "July", "August", "September", "October", "November", "Decembar"
};

void add(char *argv[], int argc, time_t now_time) {
    struct tm user_time = *localtime(&now_time);
    Reminder new_reminder;
    new_reminder.date = user_time.tm_mday;
    new_reminder.month = user_time.tm_mon + 1;
    new_reminder.year = user_time.tm_year + 1900;

    char singleWord_flag = 0;

    // reminder add "teks" time_setter;
    // [0]      [1]  [2]     [3]
    
    if (argc > 2) {
        // using number
        
        // reminder add "teks" date[3] month[4] year[5];
        if (argc >= 4) {
            /* 
                argc 4 or argv 3, has 3 possibility
                - it means the date
                - it means using single word controler
                - it means using weekday word
            */
           char *date_arg = argv[3];
           char date;

           // date
           if ((date = atoi(date_arg)) != 0) {
                if (date < user_time.tm_mday) {
                    usage(argv, BAD_CONTROLER);
                }
                new_reminder.date = date;
           }
           // single word 
           else if (strcmp(date_arg, "tomorrow") == 0 || strcmp(date_arg, "dat") == 0) {
                if (strcmp(date_arg, "tomorrow") == 0) {
                    user_time.tm_mday += 1;
                    mktime(&user_time);

                    new_reminder.date = user_time.tm_mday;
                }
                else if (strcmp(date_arg, "dat") == 0) {
                    user_time.tm_mday += 2;
                    mktime(&user_time);

                    new_reminder.date = user_time.tm_mday;
                }
           } 
           
            // week day word
            else {
                int nextWeek_flag = 0;
                while(date_arg[nextWeek_flag] == '+') {
                    nextWeek_flag++;
                }
                date_arg = date_arg + nextWeek_flag;

                for(int i=0; i<7; i++) {
                    if (strcmp(date_arg, wday[i]) == 0) {
                        if (nextWeek_flag) {
                            user_time.tm_mday = user_time.tm_mday + ((7*nextWeek_flag) - (user_time.tm_wday + 1) + (i + 1));
                            mktime(&user_time);
                            new_reminder.date = user_time.tm_mday;
                            new_reminder.month = user_time.tm_mon+1;
                            new_reminder.year = user_time.tm_year + 1900;
                        }
                        else if(i < user_time.tm_wday) {
                            usage(argv, BAD_CONTROLER);
                        }
                        user_time.tm_mday = user_time.tm_mday + (i - user_time.tm_wday);
                        mktime(&user_time);

                        /*this one possibly may exceed the day of the month or year so
                        it's not wise to combine with month-year number setter time for
                        they can make collision data or unexpected behavior,
                        so we have to prevent it using singleWord_flag*/

                        singleWord_flag = 1;
                        break;
                    }
                }
        }

        }

        if (singleWord_flag && argc >= 5) {
            usage(argv, BAD_CONTROLER);
        }
        else {

            // if argc >= 5 then it spesify the month or more spesific its motnh and year
            if (argc >= 5) {
                char *month_arg = argv[4];
                char month;
                if ((month = atoi(month_arg)) != 0) {
                    if (month < user_time.tm_mon+1) {
                        usage(argv, BAD_CONTROLER);
                    }
                    new_reminder.month = month;
                }
            }
    
            if (argc >= 6) {
                char *year_arg = argv[5];
                int year;
                if ((year = atoi(year_arg)) != 0) {
                    if (year < user_time.tm_year + 1900) {
                        usage(argv, BAD_CONTROLER);
                    }
                    new_reminder.year = year;
                } 
            }
        }
    }

    printf("time %d-%d-%d\n", new_reminder.date, new_reminder.month, new_reminder.year);
}



int main(int argc, char *argv[]) {
    if (argc == 1) {
        usage(argv, BAD_USAGE);
    }

    time_t nowTime;
    time(&nowTime);

    if (argc >= 2) {
        if (strcmp(argv[1], "add") == 0) {
            if (argc > 2) {
                add(argv, argc, nowTime);
            } else {
                usage(argv, MISSING_ARGUMENT);
            }

        } else {
            usage(argv, BAD_OPTION);
        }
    }
    else {
        usage(argv, BAD_USAGE);
    }
}


void usage(char *argv[], E_ERROR_FLAG flag) {

    /* NOTE:  before exit better free the heap*/
    fprintf(stderr, "%s: ", argv[0]);
    switch (flag) {
        case BAD_USAGE: {
            printf("usage: %s options [argumen]\n", argv[0]);
            break;
        }
        case BAD_OPTION: {
            fprintf(stderr, "%s: %s: command '%s' not found\n", argv[0], argv[1], argv[1]);
            break;
        }
        case BAD_ARGUMENT: {
            // TODO: fill the case
            break;
        }
        case MISSING_ARGUMENT: {
            printf("%s : missing argument\n", argv[1]);
            exit(4);
        }
        case BAD_CONTROLER: {
            fprintf(stderr, "Bad controler :(\n");
            exit(4);
        }
        default : {
            break;
        }
    }

    // TODO: free after error;
    //
}
