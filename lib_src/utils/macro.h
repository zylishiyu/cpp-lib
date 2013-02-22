#ifndef __ZYLISHIYU_MACRO_H_
#define __ZYLISHIYU_MACRO_H_
#include <stdio.h>
#include <sys/time.h>

// iterator 
#define foreach(x, y) for(typeof(x) it = (x); it != (y); ++it)


// string
typedef char* char_p;
typedef char_p* char_p_p;
typedef char* String;
typedef String* StringList;


// time
#define START_TIMER long long _stime_us = ustime()
#define END_TIMER   fprintf(stderr, "\033[32;49;1m=================\
    cost time:%.2lf ms=================\033[39;49;0m\n",\
    (ustime() - _stime_us) / 1000.0)

static long long ustime(void) {
    struct timeval tv;
    long long ust;

    gettimeofday(&tv, NULL);
    ust = ((long)tv.tv_sec)*1000000;
    ust += tv.tv_usec;
    return ust;
}   

static long long mstime(void) {
    struct timeval tv;
    long long mst; 

    gettimeofday(&tv, NULL);
    mst = ((long)tv.tv_sec)*1000;
    mst += tv.tv_usec/1000;
    return mst;
} 

#define __stringify_1(x...)     #x
#define __stringify(x...)       __stringify_1(x)
#define is_enabled(x)           (__stringify(CONFIG_##x)[0]=='1')
#define CONFIG_PERSIST 1


#define _TO_STR(x) #x
#define _STR_CONCAT(s1, s2) s1 ## s2

#define GET_RAW_FROM_MCPACK(pack, key, value, value_len) do {    \
        value = (char*)mc_pack_get_raw((pack), key, &value_len); \
        if (MC_PACK_PTR_ERR(value)) {                            \
            log_warning("request has no field[%s]", key);        \
            return DB_REQ_PARAM_ERROR;                           \
        }                                                        \
    } while (0)

#define GET_RAW_FROM_REQUEST(req, field)   \
    const char*  field;                    \
    unsigned int _STR_CONCAT(field, _len); \
    GET_RAW_FROM_MCPACK(req->data, _TO_STR(field), field, _STR_CONCAT(field, _len))


#endif  // __ZYLISHIYU_MACRO_H_

