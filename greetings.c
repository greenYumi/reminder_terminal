#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "reminder.h"


void loadData(ReminderBox* reminder_box) {
    FILE* file = fopen("reminder.save", "r");

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    reminder_box->size = size/sizeof(Reminder);
    Reminder* tmp = (Reminder *)malloc(sizeof(Reminder) * reminder_box->size);
    // TODO: if tmp == NULL
    reminder_box->reminders = tmp;

    fread(reminder_box->reminders, sizeof(Reminder), reminder_box->size, file);
    fclose(file);
}


int checkReminder(ReminderBox* reminder_box, struct tm time_info) {
    int found = 0;
    
    FILE* file = fopen("reminder_today.found", "w+");
    
    for (int i=0; i<reminder_box->size; i++) {
        if (    reminder_box->reminders[i].date == time_info.tm_mday
            &&  reminder_box->reminders[i].month == time_info.tm_mon + 1
            &&  reminder_box->reminders[i].year == time_info.tm_year + 1900) {

                    found++;
                    fwrite(reminder_box->reminders+i, sizeof(Reminder), 1, file);
            }
    }

    fclose(file);
    return found;

}


int main() {
    time_t now_time = time(NULL);
    struct tm time_info = *(localtime(&now_time));

    ReminderBox* reminder_box = (ReminderBox *)malloc(sizeof(ReminderBox));
    loadData(reminder_box);
    
    int found_reminder_today = checkReminder(reminder_box, *localtime(&now_time));
    free(reminder_box->reminders);
    free(reminder_box);

    String user_name = (getenv("USER") == NULL) ? "Anon" : getenv("USER");
    String day_time;
    int now_hour = time_info.tm_hour;
    int now_min = time_info.tm_min;

    int dini_hari_f = 0;
    if (now_hour <= 2 && now_min <= 59) {
        dini_hari_f = 1;
    }
    else if (now_hour <= 9 && now_min <= 59) {
        day_time = "pagi";
    }
    else if (now_hour <= 13 && now_min <= 59) {
        day_time = "siang";
    }
    else if (now_hour <= 17 && now_min <= 59) {
        day_time = "sore";
    }
    else {
        day_time = "malam";
    }

    if (dini_hari_f) {
        printf("Halo, %s.\n", user_name);
        printf("Sekarang adalah Dini hari.\n");
    }
    else {
        printf("Selamat %s, %s\n", day_time, user_name);
    }
    printf("(%s) %d %s\n", wday[time_info.tm_wday], time_info.tm_mday, mon[time_info.tm_mon]);

    if (found_reminder_today != 0) {
        printf("[*%d], gunakan `reminder lookup`.\n", found_reminder_today);
    }

    return 0;
}