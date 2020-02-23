#include "midimonster.h"

MM_PLUGIN_API int init();
static int openpixel_configure(char* option, char* value);
static int openpixel_configure_instance(instance* inst, char* option, char* value);
static int openpixel_instance(instance* inst);
static channel* openpixel_channel(instance* inst, char* spec, uint8_t flags);
static int openpixel_set(instance* inst, size_t num, channel** c, channel_value* v);
static int openpixel_handle(size_t num, managed_fd* fds);
static int openpixel_start(size_t n, instance** inst);
static int openpixel_shutdown(size_t n, instance** inst);

#define OPENPIXEL_INPUT 1
#define OPENPIXEL_MARK 2

typedef struct /*_data_buffer*/ {
	uint8_t strip;
	uint8_t flags;
	uint16_t bytes;
	union {
		uint16_t* u16;
		uint8_t* u8;
	} data;
} openpixel_buffer;

#pragma pack(push, 1)
typedef struct /*_openpixel_hdr*/ {
	uint8_t strip;
	uint8_t mode;
	uint16_t length;
} openpixel_header;
#pragma pack(pop)

typedef struct {
	enum {
		rgb8 = 0,
		rgb16 = 2
	} mode;

	size_t buffers;
	openpixel_buffer* buffer;

	int dest_fd;
	int listen_fd;
	size_t clients;
	int* client_fd;
	size_t* bytes_left;
} openpixel_instance_data;
