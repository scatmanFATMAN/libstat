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

struct stat_cpu_t {
    uint16_t count;
};

/**
 * Gets the system's hostname.
 *
 * Stores the system's hostname into <tt>hostname</tt>. <tt>hostname</tt> must be at least
 * STAT_HOSTNAME_MAX + 1 bytes in size.
 *
 * @param[out] hostname The buffer to store the system's hostname.
 * @return <tt>true</tt> on success, <tt>false</tt> on failure.
 */
bool stat_hostname(char *hostname);

/**
 * Gets the system's memory stats.
 *
 * Stores total/free RAM and total/free swap into the <tt>memory</tt> structure. On Linux, two additional
 * properties are available to find the shared memory and buffered memory.
 *
 * @param[out] memory The structure to fill.
 * @return <tt>true</tt> on success, <tt>false</tt> on failure.
 */
bool stat_memory(struct stat_memory_t *memory);

/**
 * Gets a path's disk stats.
 *
 * Store the total/free disk space at path.
 *
 * @parm[in] path A directory path to get disk usage for.
 * @param[out] disk The structure to fill.
 * @return <tt>true</tt> on success, <tt>false</tt> on failure.
 */
bool stat_disk(const char *path, struct stat_disk_t *disk);

/**
 * Gets the number of CPUs the system has.
 *
 * @param[out] cpu The structure to fill.
 * @return <tt>true</tt> on success, <tt>false</tt> on failure.
 */
bool stat_cpu(struct stat_cpu_t *cpu);

/**
 * Gets the system's uptime in seconds.
 *
 * @param[out] seconds The variable to fill.
 * @return <tt>true</tt> on success, <tt>false</tt> on failure.
 */
bool stat_uptime(uint64_t *seconds);

/**
 * Returns an error message from the library.
 *
 * If any of the library functions return <tt>false</tt>, use this function
 * to get the associated error message. If no error occurred and this function is called,
 * an empty string will be returned.
 *
 * The error messages do not get reset between library calls so do not use this function to check
 * for sucess for any reason (also don't use it to check for failure).
 *
 * @return The error message.
 */
const char * stat_error();
