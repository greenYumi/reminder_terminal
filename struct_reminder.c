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