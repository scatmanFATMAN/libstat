#pragma once

#include <stdint.h>
#include <stdbool.h>
#if defined(_WIN32)
# include <winbase.h>
#else
# include <limits.h>
#endif

#if defined(_WIN32)
# define STAT_HOSTNAME_MAX MAX_COMPUTERNAME_LENGTH
#else
# define STAT_HOSTNAME_MAX HOST_NAME_MAX
#endif

struct stat_memory_t {
    uint64_t mem_total;
    uint64_t mem_free;
    uint64_t mem_shared;
    uint64_t mem_buffer;
    uint64_t swap_total;
    uint64_t swap_free;
};

struct stat_disk_t {
    uint64_t total;
    uint64_t free;
};

/**
 * Gets the system's hostname.
 *
 * Stores the system's hostname into <tt>hostname</tt>. <tt>hostname</tt> must be at least
 * STAT_HOSTNAME_MAX + 1 bytes in size.
 *
 * @param[in] hostname The buffer to store the system's hostname.
 * @return <tt>true</tt> on success, <tt>false</tt> on failure.
 */
bool stat_hostname(char *hostname);

bool stat_memory(struct stat_memory_t *memory);

bool stat_disk(const char *path, struct stat_disk_t *disk);
