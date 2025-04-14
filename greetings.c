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

    ReminderBox* reminder_box = (ReminderBox *)malloc(sizeof(reminder_box));
    loadData(reminder_box);
    
    int found_reminder_today = checkReminder(reminder_box, *localtime(&now_time));
    if (found_reminder_today) {
        printf("you have reminder%s for today (%d)\n", (found_reminder_today > 1) ? "s" : "", found_reminder_today);
    } 

    free(reminder_box);
    return 0;
}