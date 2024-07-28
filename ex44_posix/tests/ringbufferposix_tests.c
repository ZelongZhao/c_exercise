//
// @Author Leoric
// @Date 2024/07/24
// @Email leoric@vfeda.com
// @Description
//

#include "minunit.h"
#include "lcthw/ringbufferposix.h"

#define MAX_ORDER 14

RingBuffer *ring_buffer = NULL;

char *test_create()
{
    ring_buffer = RingBuffer_create(MAX_ORDER);
    mu_assert(ring_buffer != NULL, "ring_buffer creation failed");
    return NULL;
}

char *test_read_write() {
    char buffer[100] ={'h','e','l','l','o','\0'};

    RingBuffer_write(ring_buffer, buffer, 6);

    RingBuffer_read(ring_buffer, buffer, 6);
    mu_assert(memcmp(buffer,"hello",5) == 0,"ring_buffer read write failed");
    return NULL;
}

char *test_destroy() {
    mu_assert(ring_buffer != NULL, "ring_buffer destroy failed");
    RingBuffer_destroy(ring_buffer);
    return NULL;
}


char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    mu_run_test(test_read_write);
    mu_run_test(test_destroy);

    return NULL;
}

RUN_TESTS(all_tests);