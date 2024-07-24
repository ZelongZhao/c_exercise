//
// @Author Leoric
// @Date 2024/07/24
// @Email leoric@vfeda.com
// @Description
//

#include "minunit.h"
#include "lcthw/ringbuffer.h"

#define MAX_LENTH 1024

RingBuffer *ring_buffer;

char *test_create()
{
    ring_buffer = RingBuffer_create(MAX_LENTH);
    mu_assert(ring_buffer != NULL, "ring_buffer creation failed");
    return NULL;
}

char *test_read_write() {
    char buffer[100];

    RingBuffer_write(ring_buffer, "hello", 5);

    RingBuffer_read(ring_buffer, buffer, 5);
    mu_assert(memcmp(buffer,"hello",5),"ring_buffer read write failed");
    return NULL;
}

char *test_destroy() {
    RingBuffer_destroy(ring_buffer);
    mu_assert(ring_buffer == NULL, "ring_buffer destroy failed");
    return NULL;
}


char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    // mu_run_test(test_send_recv);
    mu_run_test(test_read_write);
    // mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);