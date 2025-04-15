#include "struct_reminder.c"


#define NOT_EMPTY 0

typedef char * String;

typedef enum {
    BAD_USAGE,
    BAD_OPTION,
    BAD_ARGUMENT,
    MISSING_ARGUMENT,
    BAD_CONTROLLER
} E_ERROR_FLAG;
