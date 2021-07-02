#ifndef _LOG_H_
#define _LOG_H_  

#include <stdio.h>

#define LOGD(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#define LOGI(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#define LOGE(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#endif /* ifndef _LOG_H_  */
