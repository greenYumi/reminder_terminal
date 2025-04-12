#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char note[100];
    int date;
    int month;
    int year;
} Reminder;


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
    else if (month_arg < new_reminder->date){
        return 0;
    }
    else {
        time_info.tm_mday = (month_arg == 0) ? new_reminder->date : date_arg;
        mktime(&time_info);
    }

    new_reminder->date = time_info.tm_mday;
    new_reminder->month = time_info.tm_mon + 1;
    new_reminder->year = time_info.tm_year + 1900;

    return 1;
}


int main(int argc, char *argv[]) {
    Reminder* new_reminder = (Reminder *)malloc(sizeof(Reminder));
    time_t nowTime = time(NULL);
    struct tm infoTime = *localtime(&nowTime);

    new_reminder->date = infoTime.tm_mday;
    new_reminder->month = infoTime.tm_mon + 1;
    new_reminder->year = infoTime.tm_year + 1900;


    
    if (date(argv[1], nowTime, new_reminder) != 0) {
        printf("%d-%d-%d\n", new_reminder->date, new_reminder->month, new_reminder->year);
    } else {
        printf("err\n");
    }

    return 0;
}