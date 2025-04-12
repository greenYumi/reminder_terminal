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

int positif_atoi(char *string) {
    size_t len = 0;

    while (string[len] != '\0') {
        len++;
    }

    int int_value = 0;
    for (int i=0; i<len; i++) {
        if (string[i] >= 48 && string[i] <= 57) {
            int_value *= 10;
            int_value += string[i] - '0';
        } else {
            return 0;
        }
    }

    return int_value;
}

// 1


int* date_validity(char *date, struct tm time_info) {
    int dd, mm, yyyy;
    dd = positif_atoi(date + 0);
    dd = positif_atoi(date + 1);
    yyyy = positif_atoi(date + 2);

    if (dd){
        return (int*)    
    }
     else {
        return NULL;
    }
}


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


    // reminder add "teks" time_setter;
    // [0]      [1]  [2]     [3]
    
    if (argc > 2) {
        
}
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

