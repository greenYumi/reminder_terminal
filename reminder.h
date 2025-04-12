#include "struct_reminder.c"

typedef char * String;

typedef enum {
    BAD_USAGE,
    BAD_OPTION,
    BAD_ARGUMENT,
    MISSING_ARGUMENT,
    BAD_CONTROLLER
} E_ERROR_FLAG;
