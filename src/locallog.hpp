#ifndef _locallog_h
#define _locallog_h

#include <Arduino.h>

// define in platformio.ini local log level
//
// build_flags =
//    -DLOCAL_LOG_LEVEL=4

const char * pathToFileName(const char * path);
int log_printf(const char *fmt, ...);
void log_print_buf(const uint8_t *b, size_t len);

#define LOCAL_LOG_FORMAT(letter, format)  ARDUHAL_LOG_COLOR_ ## letter "%.03f [" #letter "][%s:%u] %s(): " format ARDUHAL_LOG_RESET_COLOR "\r\n", (float)(esp_timer_get_time() / 1000.00f), pathToFileName(__FILE__), __LINE__, __FUNCTION__

#if LOCAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_DEBUG
#ifndef USE_ESP_IDF_LOG
#define llog_d(format, ...) log_printf(LOCAL_LOG_FORMAT(D, format), ##__VA_ARGS__)
#define llog_isr_d(format, ...) ets_printf(LOCAL_LOG_FORMAT(D, format), ##__VA_ARGS__)
#define llog_buf_d(b,l) do{ARDUHAL_LOG_COLOR_PRINT(D);log_print_buf(b,l);ARDUHAL_LOG_COLOR_PRINT_END;}while(0)
#else
#define llog_d(format, ...) do {ESP_LOG_LEVEL_LOCAL(ESP_LOG_DEBUG, TAG, format, ##__VA_ARGS__);}while(0)
#define llog_isr_d(format, ...) do {ets_printf(LOG_FORMAT(D, format), esp_log_timestamp(), TAG, ##__VA_ARGS__);}while(0)
#define llog_buf_d(b,l) do {ESP_LOG_BUFFER_HEXDUMP(TAG, b, l, ESP_LOG_DEBUG);}while(0)
#endif
#else
#define llog_d(format, ...)  do {} while(0)
#define llog_isr_d(format, ...) do {} while(0)
#define llog_buf_d(b,l) do {} while(0)
#endif

#if LOCAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_INFO
#ifndef USE_ESP_IDF_LOG
#define llog_i(format, ...) log_printf(LOCAL_LOG_FORMAT(I, format), ##__VA_ARGS__)
#define llog_isr_i(format, ...) ets_printf(LOCAL_LOG_FORMAT(I, format), ##__VA_ARGS__)
#define llog_buf_i(b,l) do{ARDUHAL_LOG_COLOR_PRINT(I);log_print_buf(b,l);ARDUHAL_LOG_COLOR_PRINT_END;}while(0)
#else
#define llog_i(format, ...) do {ESP_LOG_LEVEL_LOCAL(ESP_LOG_INFO, TAG, format, ##__VA_ARGS__);}while(0)
#define llog_isr_i(format, ...) do {ets_printf(LOG_FORMAT(I, format), esp_log_timestamp(), TAG, ##__VA_ARGS__);}while(0)
#define llog_buf_i(b,l) do {ESP_LOG_BUFFER_HEXDUMP(TAG, b, l, ESP_LOG_INFO);}while(0)
#endif
#else
#define llog_i(format, ...) do {} while(0)
#define llog_isr_i(format, ...) do {} while(0)
#define llog_buf_i(b,l) do {} while(0)
#endif

#if LOCAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_WARN
#ifndef USE_ESP_IDF_LOG
#define llog_w(format, ...) log_printf(LOCAL_LOG_FORMAT(W, format), ##__VA_ARGS__)
#define llog_isr_w(format, ...) ets_printf(LOCAL_LOG_FORMAT(W, format), ##__VA_ARGS__)
#define llog_buf_w(b,l) do{ARDUHAL_LOG_COLOR_PRINT(W);log_print_buf(b,l);ARDUHAL_LOG_COLOR_PRINT_END;}while(0)
#else
#define llog_w(format, ...) do {ESP_LOG_LEVEL_LOCAL(ESP_LOG_WARN, TAG, format, ##__VA_ARGS__);}while(0)
#define llog_isr_w(format, ...) do {ets_printf(LOG_FORMAT(W, format), esp_log_timestamp(), TAG, ##__VA_ARGS__);}while(0)
#define llog_buf_w(b,l) do {ESP_LOG_BUFFER_HEXDUMP(TAG, b, l, ESP_LOG_WARN);}while(0)
#endif
#else
#define llog_w(format, ...) do {} while(0)
#define llog_isr_w(format, ...) do {} while(0)
#define llog_buf_w(b,l) do {} while(0)
#endif

#if LOCAL_LOG_LEVEL >= ARDUHAL_LOG_LEVEL_ERROR
#ifndef USE_ESP_IDF_LOG
#define llog_e(format, ...) log_printf(LOCAL_LOG_FORMAT(E, format), ##__VA_ARGS__)
#define llog_isr_e(format, ...) ets_printf(LOCAL_LOG_FORMAT(E, format), ##__VA_ARGS__)
#define llog_buf_e(b,l) do{ARDUHAL_LOG_COLOR_PRINT(E);log_print_buf(b,l);ARDUHAL_LOG_COLOR_PRINT_END;}while(0)
#else
#define llog_e(format, ...) do {ESP_LOG_LEVEL_LOCAL(ESP_LOG_ERROR, TAG, format, ##__VA_ARGS__);}while(0)
#define llog_isr_e(format, ...) do {ets_printf(LOG_FORMAT(E, format), esp_log_timestamp(), TAG, ##__VA_ARGS__);}while(0)
#define llog_buf_e(b,l) do {ESP_LOG_BUFFER_HEXDUMP(TAG, b, l, ESP_LOG_ERROR);}while(0)
#endif
#else
#define llog_e(format, ...) do {} while(0)
#define llog_isr_e(format, ...) do {} while(0)
#define llog_buf_e(b,l) do {} while(0)
#endif

#endif