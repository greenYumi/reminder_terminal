#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "reminder.h"


void usage(char *argv[], E_ERROR_FLAG, String err_msg);

// ### Utility

int date( char *date, time_t now_time, Reminder* new_reminder) {
    if((date[0] < 48 || date[0] > 57) && (date[0] != '-')) {
        return 0;
    }

    struct tm time_info = *localtime(&now_time);
    int ddmmyyyy[3] = {0, 0, 0};
    int i,j;
    i=j=0;

    while(date[i] != '\0') {
        if (date[i] == '-') {
            if (j > 2) {
                return 0;
            }
            j++;
            i++;
        }
        else if(date[i] >= 48 && date[i] <= 57) {
            ddmmyyyy[j] *= 10;
            ddmmyyyy[j] += date[i] - '0';
            i++;
        }
        else {
            return 0;
        }
    }

    int date_arg, month_arg, year_arg;
    date_arg = ddmmyyyy[0];
    month_arg = ddmmyyyy[1];
    year_arg = ddmmyyyy[2];

    if (date_arg < 0 || date_arg  > 31) {
        return 0;
    }
    if (month_arg < 0 || month_arg > 12) {
        return 0;
    }
    if (year_arg != 0 && year_arg < time_info.tm_year + 1900) {
        return 0;
    }

    if (year_arg > time_info.tm_year + 1900) {
        time_info.tm_year  = year_arg - 1900;
        time_info.tm_mon = (month_arg == 0) ? new_reminder->month - 1 : month_arg - 1;
        time_info.tm_mday = (month_arg == 0) ? new_reminder->date : date_arg;
        mktime(&time_info);
    }
    else if (month_arg > new_reminder->month) {
        time_info.tm_mon = month_arg - 1;
        time_info.tm_mday = date_arg;
        mktime(&time_info);
    }
    else if (month_arg < new_reminder->month && month_arg != 0) {
        return 0;
    }
    else if (date_arg < new_reminder->date && date_arg != 0){
        return 0;
    }
    else {
        time_info.tm_mday = (date_arg == 0) ? new_reminder->date : date_arg;
        mktime(&time_info);
    }

    new_reminder->date = time_info.tm_mday;
    new_reminder->month = time_info.tm_mon + 1;
    new_reminder->year = time_info.tm_year + 1900;

    return 1;
}


void saveReminder(Reminder* new_reminder) {

    // load and read file
    ReminderBox reminder_box;
    FILE* save_file = fopen("reminder.save", "r+");
    
    fseek(save_file, 0, SEEK_END);
    int size = ftell(save_file);
    rewind(save_file);

    reminder_box.size = size / sizeof(Reminder);
    reminder_box.reminders = (Reminder *)malloc(sizeof(Reminder) * reminder_box.size);

    fread(reminder_box.reminders, sizeof(Reminder), reminder_box.size, save_file);

    // write new_reminder;
    Reminder * tmp = (Reminder *)realloc(reminder_box.reminders, sizeof(Reminder)*(reminder_box.size + 1));
    reminder_box.reminders = tmp;
    reminder_box.reminders[reminder_box.size] = *new_reminder;
    reminder_box.size += 1;

    fwrite(reminder_box.reminders, sizeof(Reminder), reminder_box.size, save_file);

    fclose(save_file);
    free(reminder_box.reminders);
}


// ## main

int main(int argc, char *argv[]) {
    if (argc == 1) {
        usage(argv, BAD_USAGE, "");
    }

    time_t now_time;
    time(&now_time);
    struct tm info_time = *localtime(&now_time);

    if (argv [1] != NULL) {

        if (strcmp(argv[1], "add") == 0) {
            if (argv[2] != NULL) {
                Reminder* new_reminder = (Reminder *)malloc(sizeof(Reminder));
                new_reminder->date = info_time.tm_mday;
                new_reminder->month = info_time.tm_mon + 1;
                new_reminder->year = info_time.tm_year + 1900;

                if (date((argv[3] == NULL) ? "-" : argv[3], now_time, new_reminder) != 0) {
                    strcpy(new_reminder->note, argv[2]);

                    printf("[note saved] on %d-%d-%d\n", 
                        new_reminder->date, 
                        new_reminder->month,
                        new_reminder->year);
                    printf("\t%s\n", new_reminder->note);

                    saveReminder(new_reminder);
                    free(new_reminder);
                }
                
            } else {
                usage(argv, MISSING_ARGUMENT, "add need [note] [options time setter] argument.");
            }

        } else {
            usage(argv, BAD_OPTION, "");
        }
    }
}


void usage(char *argv[], E_ERROR_FLAG flag, String err_msg) {

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

