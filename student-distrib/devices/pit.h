#include "../types.h"

int pit_handler();

int32_t pit_open(const uint8_t* filename);
int32_t pit_close(int32_t fd);
int32_t pit_read(int32_t fd, void* buf, int32_t nbytes);
int32_t pit_write(int32_t fd, const void* buf, int32_t nbytes);