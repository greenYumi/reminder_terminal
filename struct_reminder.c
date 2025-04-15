typedef struct {
    char label[50];
    int date;
    int month;
    int year;
} Reminder;

typedef struct {
    int size;
    Reminder* reminders;
} ReminderBox;