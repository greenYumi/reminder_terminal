#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include "reminder.h"

#define NOT_EMPTY 0
#define FAILED -1



void usage(char *argv[], E_ERROR_FLAG, String err_msg);

// ### Utility

int date( char *date, time_t now_time, Reminder* new_reminder) {
    if((date[0] < 48 || date[0] > 57) && (date[0] != '-')) {
        return -1;
    }

    struct tm time_info = *localtime(&now_time);
    int ddmmyyyy[3] = {0, 0, 0};
    int i,j;
    i=j=0;

    while(date[i] != '\0') {
        if (date[i] == '-') {
            if (j > 2) {
                return -1;
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
            return -1;
        }
    }

    int date_arg, month_arg, year_arg;
    date_arg = ddmmyyyy[0];
    month_arg = ddmmyyyy[1];
    year_arg = ddmmyyyy[2];

    if (date_arg < 0 || date_arg  > 31) {
        return -1;
    }
    if (month_arg < 0 || month_arg > 12) {
        return -1;
    }
    // if user gave the year but past by current year, definitely error
    if (year_arg != 0 && year_arg < time_info.tm_year + 1900) {
        return -1;
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
        return -1;
    }
    else if (date_arg < new_reminder->date && date_arg != 0){
        return -1;
    }
    else {
        time_info.tm_mday = (date_arg == 0) ? new_reminder->date : date_arg;
        mktime(&time_info);
    }

    new_reminder->date = time_info.tm_mday;
    new_reminder->month = time_info.tm_mon + 1;
    new_reminder->year = time_info.tm_year + 1900;

    // sucess must be return 0;
    return 0;
}


void getLable(char *lable_frame) {

    char c;
    int max_char = 50;
    int i;

    for (i=0; i<max_char; i++) {
        lable_frame[i] = 0;
    }

    i=0;
    while ((c = getchar()) != EOF) {
        if (i == max_char) {
            printf("lable length can't more than 50 chars\n");
            exit(9);
            break;
        }
        else if (c == '\n') {
            lable_frame[i] = '\0';
            break;
        }
        else {
            lable_frame[i] = c;
            i++;
        }
    }
}


void noteTaking(String note) {
    // what's the error case?

    char c;
    int i=0;
    while((c = getchar()) != EOF) {
        if (i > 100) {
            printf("char exceed 100, bad input.\n");
            // loop or exit?
            exit(-1);
        }
        else if (c == '\n') {
            if (i==0) {
                strcpy(note, "(leaved blank)");
                printf("leave blank\n");
                i = 15;
                break;
            }
            note[i] = '\0';
            break;
        }
        else {
            note[i] = c;
            i++;
        }
    }

    // is this even necessary?
    for (; i<=100; i++) {
        note[i] = 0;
    }

} 


void saveReminder(Reminder* new_reminder, String current_path) {
    
    // load and read file
    ReminderBox reminder_box;

    char path1[999];
    sprintf(path1, "%s/reminder.save", current_path);
    FILE* save_file = fopen(path1, "r+");
    
    fseek(save_file, 0, SEEK_END);
    int size = ftell(save_file);
    rewind(save_file);
    
    reminder_box.size = size / sizeof(Reminder);
    reminder_box.reminders = (Reminder *)malloc(sizeof(Reminder) * reminder_box.size);
    
    fread(reminder_box.reminders, sizeof(Reminder), reminder_box.size, save_file);
    
    
    // label
    char label[50]; 
    int lable_duplicate = 1;
    while (lable_duplicate == 1) {
        printf("lable: ");
        // fgets(lable, 50, stdin); // contain '\n'
        getLable(label);
        if (reminder_box.size == 0) {
            break;
        }

        for (int i=0; i<reminder_box.size; i++) {
            if (strcmp(label, reminder_box.reminders[i].label) == 0) {
                printf("the same lable already saved\n");
                break;
            }
            else if (strcmp(label, reminder_box.reminders[i].label) != 0 && i == reminder_box.size - 1) {
                lable_duplicate = 0;
            }
        }
    }

    strcpy(new_reminder->label, label);
    printf("lable : %s saved\n", label);


    // taking note;
    // note max. 100 char;
    char note[101];
    printf("note: ");
    noteTaking(note);
    

    // write new_reminder;
    Reminder * tmp = (Reminder *)realloc(reminder_box.reminders, sizeof(Reminder)*(reminder_box.size + 1));
    reminder_box.reminders = tmp;
    reminder_box.reminders[reminder_box.size] = *new_reminder;
    reminder_box.size += 1;

    rewind(save_file);
    fwrite(reminder_box.reminders, sizeof(Reminder), reminder_box.size, save_file);

    fclose(save_file);
    free(reminder_box.reminders);

    // write note in .reminders with label as title
    char path[999];
    sprintf(path, "%s/.reminders/%s", current_path, label);
    FILE* note_file = fopen(path, "w");
    printf("...saving note\n");
    fwrite(note, 100, 1, note_file);
    fclose(note_file);
    printf("(saving complete)\n");
}


void lookup(Reminder* lookup_reminder, ReminderBox* lookup_reminder_box, struct tm current_time, String current_path) {
    char path[999];
    sprintf(path, "%s/reminder.save", current_path);
    FILE* file = fopen(path, "r");

    if (    lookup_reminder->date == current_time.tm_mday
        &&  lookup_reminder->month == current_time.tm_mon + 1
        &&  lookup_reminder->year == current_time.tm_year + 1900) {

                char path1[999];
                sprintf(path1, "%s/reminder_today.found", current_path);
                FILE* saved_file = fopen(path1, "r");

                if (fseek(saved_file, 0, SEEK_END) == NOT_EMPTY) {
                    size_t size = ftell(saved_file);
                    rewind(saved_file);
                    lookup_reminder_box->size = size / sizeof(Reminder);

                    lookup_reminder_box->reminders = (Reminder *)malloc(sizeof(Reminder) * lookup_reminder_box->size);
                    fread(lookup_reminder_box->reminders, sizeof(Reminder), lookup_reminder_box->size, saved_file);

                    printf("note: %d-%d-%d\n\n", 
                                lookup_reminder->date,
                                lookup_reminder->month,
                                lookup_reminder->year);
                    for(int i=0; i<lookup_reminder_box->size; i++) {
                        printf("[%d] %s\n", i+1, lookup_reminder_box->reminders[i].label);
                    }
                }

                
                fclose(saved_file);
                // writing data in file for `see` command;
                char path2[999];
                sprintf(path2, "%s/lookup.see", current_path);
                FILE* lookup_file = fopen(path2, "w");
                fwrite(lookup_reminder_box->reminders, sizeof(Reminder), lookup_reminder_box->size, lookup_file);
                fclose(lookup_file);
    }

    else if (fseek(file, 0, SEEK_END) == NOT_EMPTY) {
        size_t size = ftell(file) / sizeof(Reminder);
        rewind(file);

        Reminder* tmp_reminder = (Reminder *)malloc(sizeof(Reminder) * size);
        fread(tmp_reminder, sizeof(Reminder), size, file);

        for (int i=0; i<size; i++) {
            if (    tmp_reminder[i].date == lookup_reminder->date
                &&  tmp_reminder[i].month == lookup_reminder->month
                &&  tmp_reminder[i].year == lookup_reminder->year) {

                        lookup_reminder_box->size++;
                        Reminder* tmp = (Reminder *)realloc(lookup_reminder_box->reminders, sizeof(Reminder) * lookup_reminder_box->size);
                        lookup_reminder_box->reminders = tmp;
                        lookup_reminder_box->reminders[(lookup_reminder_box->size)-1] = tmp_reminder[i];
                }
        }

        free(tmp_reminder);

        // writing data in file for `see` command;
        sprintf(path, "%s/lookup.see", current_path);
        FILE* lookup_file = fopen(path, "w");
        fwrite(lookup_reminder_box->reminders, sizeof(Reminder), lookup_reminder_box->size, lookup_file);
        fclose(lookup_file);  

        if (lookup_reminder_box->size == 0) {
            printf("note with date %d-%d-%d not found\n", 
                lookup_reminder->date,
                lookup_reminder->month,
                lookup_reminder->year);
                return;
        }

        printf("note: %d-%d-%d\n\n", 
            lookup_reminder->date,
            lookup_reminder->month,
            lookup_reminder->year);
        for(int i=0; i<lookup_reminder_box->size; i++) {
        printf("[%d] %s\n", i+1, lookup_reminder_box->reminders[i].label);
        }
    }
    else {
        printf("Not saved anything yet\n.");
    }

    fclose(file);
    
}


void seeNote(int index, String current_path) {
    char path[999];
    sprintf(path, "%s/lookup.see", current_path);
    FILE* file = fopen(path, "r");

    ReminderBox reminder_box;
    memset(&reminder_box, 0, sizeof(ReminderBox));

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file) / sizeof(Reminder);
    rewind(file);
    if(size == 0) {
        // or bad usage
        fprintf(stderr, "file empty please lookup first :)\n");
        exit(-1);
    }

    if (index > size) {
        // or bad usage
        fprintf(stderr, "bad index, try update lookup buffer\n");
        exit(-1);
    }

    reminder_box.size = size;
    reminder_box.reminders = (Reminder *)malloc(sizeof(Reminder) * size);
    fread(reminder_box.reminders, sizeof(Reminder), size, file);
    fclose(file);

    char fileName[999];
    sprintf(fileName, "%s/.reminders/%s",current_path, reminder_box.reminders[index-1].label);
    FILE* noteFile = fopen(fileName, "r");
    char note[101];
    fread(note, 1, 101, noteFile);
    printf("%s\n", note);
    fclose(noteFile);

}


// ## main

int main(int argc, char *argv[]) {
    if (argc == 1) {
        usage(argv, BAD_USAGE, "");
    }

    time_t now_time;
    time(&now_time);
    struct tm current_time = *localtime(&now_time);
    
    char current_path[999];
    ssize_t len = readlink("/proc/self/exe", current_path, sizeof(current_path) - 1);
    if (len != -1) {
        current_path[len] = '\0';
        char *dir = dirname(current_path);
        sprintf(current_path, dir);
    }
    else {
        return -1;
    }

    if (argv [1] != NULL) {

        if (strcmp(argv[1], "add") == 0) {
            Reminder* new_reminder = (Reminder *)malloc(sizeof(Reminder));
            memset(new_reminder, 0, sizeof(int)*3 + sizeof(char)*50);
            new_reminder->date = current_time.tm_mday;
            new_reminder->month = current_time.tm_mon + 1;
            new_reminder->year = current_time.tm_year + 1900;

            if (date((argv[2] == NULL) ? "-" : argv[2], now_time, new_reminder) == 0) {

                printf("[note saved] on %d-%d-%d\n", 
                    new_reminder->date, 
                    new_reminder->month,
                    new_reminder->year);
                
                saveReminder(new_reminder, current_path);
                free(new_reminder);
            }
        }

        else if(strcmp(argv[1], "lookup") == 0) {
            // FIXME: -ME, better find a way which we avoid using Reminder to saving
            // space (char)[50] in this structure.
            Reminder* lookup_reminder = (Reminder *)malloc(sizeof(Reminder));
            ReminderBox* lookup_reminder_box = (ReminderBox *)malloc(sizeof(ReminderBox));
            memset(lookup_reminder, 0, sizeof(Reminder));
            memset(lookup_reminder_box, 0, sizeof(ReminderBox));
            lookup_reminder->date = current_time.tm_mday;
            lookup_reminder->month = current_time.tm_mon + 1;
            lookup_reminder->year = current_time.tm_year + 1900;

            if(date((argv[2] == NULL) ? "-" : argv[2], now_time, lookup_reminder) == 0) {
                lookup(lookup_reminder,lookup_reminder_box, current_time, current_path);

                //  ** some operation **

                free(lookup_reminder);
            }
        }
        else if ((strcmp(argv[1], "see") == 0)) {
            if (argv[2] == NULL) {
                usage(argv, BAD_ARGUMENT, "see need an index\n");
            }

            int index;
            if ((index = strToInt(argv[2])) != FAILED) {
                seeNote(index, current_path);
            } 
            else {
                // err
                usage(argv, BAD_CONTROLLER, "invalid index");
            }
        }

        else {
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
    
    exit(-99);
} 

