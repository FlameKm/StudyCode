#ifndef BlazeMiles_hyc_LOG_H_
#define BlazeMiles_hyc_LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* function of print  */
#define SEND_MSG_FUNCTION uart_printf
/*function declaration*/
void uart_printf(const char *fmt, ...);

/* log switch */
#define LOG_INFO 1
#define LOG_DEBUG 1
#define LOG_ERROR 1
#define LOG_WARING 1
#define LOG_VERBOSE 1

/* log function */
#if LOG_INFO
#define logi(msg, ...) SEND_MSG_FUNCTION( \
    "(%s) "                              \
    "info: " msg "\r\n",                 \
    __FUNCTION__, ##__VA_ARGS__)
#else
#define logi(msg, ...) \
    do {               \
    } while (0)
#endif

#if LOG_DEBUG
#define logd(msg, ...) SEND_MSG_FUNCTION( \
    "(%s) "                               \
    "debug: " msg "\r\n",            \
    __FUNCTION__, ##__VA_ARGS__)
#else
#define logd(msg, ...) \
    do {               \
    } while (0)
#endif

#if LOG_ERROR
#define loge(msg, ...) SEND_MSG_FUNCTION( \
    "(%s) "                               \
    "err: " msg "\r\n",            \
    __FUNCTION__, ##__VA_ARGS__)
#else
#define loge(msg, ...) \
    do {               \
    } while (0)
#endif

#if LOG_WARING
#define logw(msg, ...) SEND_MSG_FUNCTION( \
    "(%s) "                               \
    "war: " msg "\r\n",                \
     __FUNCTION__, ##__VA_ARGS__)
#else
#define loge(msg, ...) \
    do {               \
    } while (0)
#endif


#if LOG_INFO
#define logv(msg, ...) SEND_MSG_FUNCTION( \
    "(%s) "                              \
    "ver: " msg "\r\n",                 \
    __FUNCTION__, ##__VA_ARGS__)
#else
#define logi(msg, ...) \
    do {               \
    } while (0)
#endif

#ifdef __cplusplus
}
#endif
#endif