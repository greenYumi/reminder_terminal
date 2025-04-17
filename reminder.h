#include "struct_reminder.c"


#define NOT_EMPTY 0

typedef char * String;

const typedef enum {
    BAD_USAGE,
    BAD_OPTION,
    BAD_ARGUMENT,
    MISSING_ARGUMENT,
    BAD_CONTROLLER
} E_ERROR_FLAG;

const String wday[7] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
const String mon[12] = {
    "Januari", "Februari", "Maret",
    "April", "Mei", "Juni",
    "Juli", "Agustus", "September",
    "Oktober", "November", "Desember"};
