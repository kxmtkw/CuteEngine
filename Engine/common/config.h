#ifndef CUTE_CONFIG_H
#define CUTE_CONFIG_H

#define CT_CONF_FIXED_SLOT_COUNT 256
#define CT_CONF_FIXED_CON_BUCKET_SIZE 1024

#define CT_CONF_CALLSTACK_SIZE 1000

#define CT_CONF_FAIL_ON_NULL

//#define CT_CONF_DEBUG


#ifdef CT_CONF_DEBUG

#define CT_CONF_LOG_FILTER(DOMAIN) true

#define CT_LOG(DOMAIN, ...) \
    do { \
		if (CT_CONF_LOG_FILTER(DOMAIN)) {\
			printf("[LOG] (%s) ", DOMAIN); \
			printf(__VA_ARGS__); \
		} \
    } while (0)

#else

#define CT_LOG(DOMAIN, ...) do {} while (0)

#endif // CT_CONF_DEBUG


#endif // CUTE_CONFIG_H