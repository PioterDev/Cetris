#ifndef DATE_H
#define DATE_H

#include "utils.h"

/**
 * @brief Puts the current date in ISO8601 format into the provided buffer.
 * 
 * @param buf buffer
 * @param bufsize size of the buffer
 * @return SUCCESS on success, BUFFERTOOSMALL if the buffer is too small
 */
status_t ISO8601(char* buf, size_t bufsize);


#endif