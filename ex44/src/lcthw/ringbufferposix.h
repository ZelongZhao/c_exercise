//
// @Author ZhaoZelong
// @Date 2024/07/26
// @Email zhaozelong@leagsoft.com
// @Copyright (c) 2024 LeagSoft Inc.
// @Description
//

#ifndef RINGBUFFERPOSIX_H
#define RINGBUFFERPOSIX_H

#include <lcthw/bstrlib.h>

typedef struct {
    char *buffer;
    unsigned long length;
    unsigned long start;
    unsigned long end;
} RingBuffer;

RingBuffer *RingBuffer_create(unsigned long order);

void RingBuffer_destroy(RingBuffer *buffer);

unsigned long RingBuffer_read(RingBuffer *buffer, char *target, unsigned long amount);

unsigned long RingBuffer_write(RingBuffer *buffer, char *data, unsigned long length);

void RingBuffer_commit_read(RingBuffer *buffer,unsigned long count);

void RingBuffer_commit_write(RingBuffer *buffer,unsigned long count);

void RingBuffer_clear(RingBuffer *buffer);

unsigned long RingBuffer_count_bytes(RingBuffer *buffer);

unsigned long RingBuffer_count_free_bytes(RingBuffer *buffer);

#define RingBuffer_starts_at(B) ((B)->buffer + (B)->start)

#define RingBuffer_ends_at(B) ((B)->buffer + (B)->end)



#endif //RINGBUFFERPOSIX_H
