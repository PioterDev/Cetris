#include <time.h>
#include <string.h>

#include "deus.h"
#include "utils.h"

status_t ISO8601(char* buf, const size_t bufsize) {
    if(bufsize < 20) return BUFFERTOOSMALL;
    time_t t = time(NULL);
    struct tm* now = localtime(&t);
    /* char year[5] = {0};
    itos(now->tm_year + 1900, 10, year, sizeof(year)); //tm_year counts how many years have passed since 1900, hence the offset
    //For example, let the date = 2038-01-19 03:14:07 (Epoch's 32-bit limit).
    strcat(buf, year);                        //2038
    strcat(buf + 4, "-");                     //2038-
    strcat(buf + 5, nums[now->tm_mon + 1]);   //2038-01
    strcat(buf + 7, "-");                     //2038-01-
    strcat(buf + 8, nums[now->tm_mday]);      //2038-01-19
    strcat(buf + 10, " ");                    //2038-01-19 
    strcat(buf + 11, nums[now->tm_hour]);     //2038-01-19 03
    strcat(buf + 13, ":");                    //2038-01-19 03:
    strcat(buf + 14, nums[now->tm_min]);      //2038-01-19 03:14
    strcat(buf + 16, ":");                    //2038-01-19 03:14:
    strcat(buf + 17, nums[now->tm_sec]);      //2038-01-19 03:14:07
    //Offsets are for strcat to not check the string repeatedly searching for '\0' */

    //Update: this nonsense can be simplified to one snprintf, lol
    snprintf(
        buf, bufsize, "%04d-%02d-%02d %02d:%02d:%02d",
        now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
        now->tm_hour, now->tm_min, now->tm_sec
    );
    return SUCCESS;
}