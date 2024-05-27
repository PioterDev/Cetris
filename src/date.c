#include <time.h>
#include <string.h>

#include "deus.h"
#include "utils.h"

status_t ISO8601(char* buf, size_t bufsize) {
    if(bufsize <= 19)return BUFFERTOOSMALL;
    static char* nums[] = {
        "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", 
        "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", 
        "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", 
        "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", 
        "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", 
        "50", "51", "52", "53", "54", "55", "56", "57", "58", "59"};
    time_t t;
    time(&t);
    struct tm* now = gmtime(&t);
    char year[5] = {0};
    itos(now->tm_year + 1900, 10, year, sizeof(year)); //tm_year counts how many years have passed since 1900, hence the offset
    //For example, let the date = 2038-01-19 03:14:07 (Epoch's 32-bit limit).
    strcat(buf, year);                   //2038
    strcat(buf, "-");                    //2038-
    strcat(buf, nums[now->tm_mon + 1]);  //2038-01
    strcat(buf, "-");                    //2038-01-
    strcat(buf, nums[now->tm_mday]);     //2038-01-19
    strcat(buf, " ");                    //2038-01-19 
    strcat(buf, nums[now->tm_hour]);     //2038-01-19 03
    strcat(buf, ":");                    //2038-01-19 03:
    strcat(buf, nums[now->tm_min]);      //2038-01-19 03:14
    strcat(buf, ":");                    //2038-01-19 03:14:
    strcat(buf, nums[now->tm_sec]);      //2038-01-19 03:14:07
    return SUCCESS;
    //The code could probably be optimized, but who cares, it works.
}