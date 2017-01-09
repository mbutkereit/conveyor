#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include <stdint.h>

#define MESSAGE_VERSION 0x01

#define MESSAGE_TYPE_INFO 0x01
#define MESSAGE_TYPE_WORKPIECE 0x02
#define MESSAGE_TYPE_CRITICAL_ERROR 0x03

struct common_header {
	uint8_t typ;
	uint8_t version;
};


struct info_package_without_ch {
	uint8_t info_a;
	uint8_t info_b;
	uint8_t info_c;
};

struct critical_error_package_without_ch {
	uint8_t event;
	uint8_t reserved;
};

struct workpiece_package_without_ch {
	uint16_t id;
	uint8_t workpiece_type;// Mit bohrung, Metall usw.
	uint16_t alimetry_value_one;
	uint16_t alimetry_value_two;
	uint8_t reserved;
};

struct info_package {
	uint8_t typ;
	uint8_t version;
	uint8_t info_a;
	uint8_t info_b;
	uint8_t info_c;
};

typedef struct info_package info_package;

struct critical_error_package {
	uint8_t typ;
	uint8_t version;
	uint8_t event;
	uint8_t reserved;
};

struct workpiece_package {
	uint8_t typ;
	uint8_t version;
	uint16_t id;
	uint8_t workpiece_type;// Mit bohrung, Metall usw.
	uint16_t alimetry_value_one;
	uint16_t alimetry_value_two;
	uint8_t reserved;
};

#endif /* SERIALIZER_H_ */

