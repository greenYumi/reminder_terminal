#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef char * String;


// error hendling;

typedef enum {
    BAD_USAGE,
    BAD_OPTION,
    BAD_ARGUMENT,
    MISSING_ARGUMENT,
    BAD_CONTROLLER
} E_ERROR_FLAG;

void usage(char *argv[], E_ERROR_FLAG, String err_msg);

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
    "January", "February", "March", "April", "May", "June", 
    "July", "August", "September", "October", "November", "December"
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
           int date;

           // date
           if ((date = atoi(date_arg)) != 0 && date_arg[0] != '+') {
                if (date < 1 || date > 31) {
                    usage(argv, BAD_CONTROLLER, "date range 1-31");
                }

                if (argv[4] != NULL ) {

                    int month = atoi(argv[4]);
                    if (month == 0) {
                        usage(argv, BAD_CONTROLLER,  "month should be a number");
                    }
                    if (month < 1 || month > 12) {
                        usage(argv, BAD_CONTROLLER,  "month range 1-12");
                    }


                    if (argv[5] != NULL) {

                        int year = atoi(argv[5]);
                        if (year == 0) {
                            usage(argv, BAD_CONTROLLER,  "year should be a number");
                        }
                        if (year < user_time.tm_year + 1900) {
                            usage(argv, BAD_CONTROLLER,  "year can't below the current year");
                        }

                        if (year > user_time.tm_year + 1900) {

                            user_time.tm_year = year - 1900;
                            user_time.tm_mon = month - 1;
                            user_time.tm_mday = date;
                            mktime(&user_time);
                        }
                        
                        if (month < user_time.tm_mon + 1) {
                            usage(argv, BAD_CONTROLLER, "month can't below the current month");
                        }

                        new_reminder.year = user_time.tm_year + 1900;
                        new_reminder.month = user_time.tm_mon + 1;
                        new_reminder.date = user_time.tm_mday;

                    } 
                    else {
                        if (month < user_time.tm_mon + 1) {
                            usage(argv, BAD_CONTROLLER, "month can't below the current month");
                        }

                        if (month >= user_time.tm_mon +1){
                            user_time.tm_mon = month - 1;
                            user_time.tm_mday = date;
                            mktime(&user_time);
                            
                            new_reminder.year = user_time.tm_year + 1900;
                            new_reminder.month = user_time.tm_mon + 1;
                            new_reminder.date = user_time.tm_mday;
                        }
                    }
                    }

                else {
                    if (date < user_time.tm_mday) {
                        usage(argv, BAD_CONTROLLER, "can't set date below the current date");
                    }

                    user_time.tm_mday = date;
                    mktime(&user_time);

                    new_reminder.year = user_time.tm_year + 1900;
                    new_reminder.month = user_time.tm_mon + 1;
                    new_reminder.date = user_time.tm_mday;
                    
                }
                

                if (date >= user_time.tm_mday) {
                    user_time.tm_mday = date;
                    mktime(&user_time);

                    new_reminder.date = user_time.tm_mday;
                } else {
                    usage(argv, BAD_CONTROLLER, "Can't set date below current date");
            }
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
            int i = 0;
            if (date_arg[0] == '+') {
                while(1) {
                    if (date_arg[i] == '+') {
                        nextWeek_flag++;
                        i++;
                        continue;
                    }
                    // FIXME: potentialy i become min value if +-wday setter given
                    else if ( atoi(date_arg+i) != 0) {
                        int weekStep = atoi(date_arg+i);
                        nextWeek_flag += weekStep;
                        i ++;
                        
                        // if there is more than 1 digit;
                        while(weekStep / 10 != 0) {
                            weekStep /= 10;
                            i++;
                        }
                        continue;
                    }
                    break;
                }
            }
            date_arg = date_arg + i;
            if (strcmp(date_arg, "") == 0) {
                usage(argv, BAD_CONTROLLER, "expected weekday name, given nothing");
            }

            for(int i=0; i<7; i++) {
                if (strcmp(date_arg, wday[i]) == 0) {
                    if (nextWeek_flag) {
                        user_time.tm_mday = user_time.tm_mday + ((7*nextWeek_flag) - (user_time.tm_wday + 1) + (i + 1));
                        mktime(&user_time);
                        new_reminder.date = user_time.tm_mday;
                        new_reminder.month = user_time.tm_mon+1;
                        new_reminder.year = user_time.tm_year + 1900;
                        
                        singleWord_flag = 1;
                        break;
                    }
                    else if(i < user_time.tm_wday) {
                        usage(argv, BAD_CONTROLLER, "can't set the day before current day in current week");
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
                else if (i == 6) {
                    usage(argv, BAD_CONTROLLER, "weekday name not found");
                }
            }
        }

    }    
}
printf("time %d-%d-%d\n", new_reminder.date, new_reminder.month, new_reminder.year);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        usage(argv, BAD_USAGE, "");
    }

    time_t nowTime;
    time(&nowTime);

    if (argc >= 2) {
        if (strcmp(argv[1], "add") == 0) {
            if (argc > 2) {
                add(argv, argc, nowTime);
            } else {
                usage(argv, MISSING_ARGUMENT, "add need [note] [options time setter] argument.");
            }

        } else {
            usage(argv, BAD_OPTION, "");
        }
    }
}


void usage(char *argv[], E_ERROR_FLAG flag, String err_msg) { // TODO: tambah parameter (string err_msg);

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
            printf("%s\n", err_msg);
            break;
        }
        case MISSING_ARGUMENT: {
            printf("%s\n", err_msg);
            exit(4);
        }
        case BAD_CONTROLLER: {
            printf("%s\n", err_msg);
            exit(4);
        }
        default : {
            break;
        }
    }
    
} 