#ifndef _DATA_BUFFER_H_
#define _DATA_BUFFER_H_

#define DATA_BUFFER_BASE_SIZE  4
#define DATA_BUFFER_FLAG_ALLOC 0x00000001
#define DATA_BUFFER_FLAG_FIXED 0x00000002
#define DATA_BUFFER_INIT_MODE_FIXED 0x00000001
#define DATA_BUFFER_INIT_MODE_ALLOC 0x00000002
#define DATA_BUFFER_INIT_MODE_WRAP  0x00000004

#define DATA_BUFFER_AVAIL_SIZE(size) ((size) & (~(DATA_BUFFER_BASE_SIZE - 1)))
#define DATA_BUFFER_ADJUST_SIZE(size) (((size) + DATA_BUFFER_BASE_SIZE - 1) & (~(DATA_BUFFER_BASE_SIZE - 1)))
#define DATA_BUFFER_CAPACITY(buffer) DATA_BUFFER_AVAIL_SIZE((buffer)->capacity)

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct DATA_BUFFER
{
    unsigned int capacity;
    unsigned int len;
    char *data;
} data_buffer;

int  data_buffer_init(data_buffer *buffer, char *data, unsigned int capacity, int mode);
void data_buffer_deinit(data_buffer *buffer);
void data_buffer_clear(data_buffer *buffer);
void data_buffer_move(data_buffer *dst, data_buffer *src);
int  data_buffer_adjust(data_buffer *buffer, unsigned int capacity);
int  data_buffer_put(data_buffer *buffer, const char *data, unsigned int len);

#ifdef __cplusplus
}
#endif

#endif