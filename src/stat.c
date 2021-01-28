#include <stdio.h>
#include <stdarg.h>
#if defined(_WIN32)
# include <sysinfoapi.h>
#else
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <ctype.h>
# include <unistd.h>
# include <sys/sysinfo.h>
# include <sys/statvfs.h>
# include "strlcpy.h"
#endif
#include "../api/stat.h"

static char error[512] = {0};

static void
stat_store_error(bool print_errno, const char *fmt, ...) {
    va_list ap;
    int errno_saved, len = 0;
#if defined(_WIN32)
    LPTSTR error_w[sizeof(error)];
    TCHAR len_w;
#endif

    if (print_errno) {
#if defined(_WIN32)
        errno_saved = GetLastError();
#else
        errno_saved = errno;
#endif
    }

    if (fmt != NULL) {
        va_start(ap, fmt);
        len = vsnprintf(error, sizeof(error), fmt, ap);
        va_end(ap);

        if (print_errno && len < sizeof(error) - 2) {
            strcpy(error + len, ": ");
            len += 2;
        }
    }

#if defined(_WIN32)
# if defined(UNICODE)
    if (print_errno) {
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errno_saved, 0, error + len, sizeof(error) - len, 0, NULL);
    }
# else
    if (print_errno) {
        len_w = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errno_saved, 0, error_w, sizeof(error_w), 0, NULL);
        if (len_w == 0) {
            snprintf(error, sizeof(error), "Function failed with an error getting error message: %s", GetLastError());
            return;
        }

        wcstombs(error + len, error_w, sizeof(error) - len - 1);
    }
# endif
#else
    if (print_errno) {
        strlcpy(error + len, strerror(errno_saved), sizeof(error) - len);
    }
#endif
}

bool
stat_hostname(char *hostname) {
#if defined(_WIN32)
    if (!GetComputerName(hostname, STAT_HOSTNAME_MAX + 1)) {
        stat_store_error(true, NULL);
        return false;
    }
#else
    if (gethostname(hostname, STAT_HOSTNAME_MAX + 1) == -1) {
        stat_store_error(true, NULL);
        return false;
    }
#endif
    return true;
}

bool
stat_memory(struct stat_memory_t *memory) {
#if defined(_WIN32)
    MEMORYSTATUSEX info;

    if (!GlobalMemoryStatusEx(&info)) {
        stat_store_error(true, NULL);
        return false;
    }

    memory->mem_total = info.ullTotalPhys;
    memory->mem_free = info.ullAvailPhys;
    memory->mem_shared = 0;
    memory->mem_buffer = 0;
    memory->swap_total = info.ullTotalVirtual;
    memory->swap_free = info.ullAvailVirtual;
#else
    struct sysinfo info;

    if (sysinfo(&info) == -1) {
        stat_store_error(true, NULL);
        return false;
    }

    memory->mem_total = (info.totalram * info.mem_unit) / 1024;
    memory->mem_free = (info.freeram * info.mem_unit) / 1024;
    memory->mem_shared = (info.sharedram * info.mem_unit) / 1024;
    memory->mem_buffer = (info.bufferram * info.mem_unit) / 1024;
    memory->swap_total = (info.totalswap * info.mem_unit) / 1024;
    memory->swap_free = (info.freeswap * info.mem_unit) / 1024;
#endif
    return true;
}

bool
stat_disk(const char *path, struct stat_disk_t *disk) {
#if defined(_WIN32)
    ULARGE_INTEGER info_total, info_free;

    if (!GetDiskFreeSpaceExA(&info_total, &info_free, NULL)) {
        stat_store_error(true, NULL);
        return false;
    }

    disk->total = info_total.QuadPart;
    disk->free = info_free.QuadPart;
#else
    struct statvfs info;

    if (statvfs(path, &info) != 0) {
        stat_store_error(true, NULL);
        return false;
    }

    disk->total = info.f_blocks * info.f_frsize;
    disk->free = info.f_bavail * info.f_frsize;
#endif
    return true;
}

bool
stat_cpu(struct stat_cpu_t *cpu) {
#if defined(_WIN32)
    SYSTEM_INFO info;

    if (!GetNativeSystemInfo(&info)) {
        stat_store_error(true, NULL);
        return false;
    }

    cpu->count = dwNumberOfProcessors;

    return true;
#else
    cpu->count = get_nprocs();
    return true;
#if 0
    bool success = true;
    char *line;
    FILE *f;
    ssize_t len;
    size_t size;

    f = fopen("/proc/stat", "r");
    if (f == NULL) {
        stat_store_error(true, "Error opening /proc/stat");
        return false;
    }

    size = 1024;
    line = malloc(size);
    if (line == NULL) {
        stat_store_error(true, "Out of memory");
        success = false;
    }

    if (success) {
        cpu->count = 0;

        while (success) {
            len = getline(&line, &size, f);

            if (len == -1) {
                if (ferror(f)) {
                    stat_store_error(true, "Error reading /proc/stat");
                    success = false;
                }

                break;
            }

            if (len > 4 && strncmp(line, "cpu", 3) == 0 && isdigit(line[3])) {
                ++cpu->count;
            }
        }
    }

    if (line != NULL) {
        free(line);
    }
    fclose(f);

    return success;
#endif
#endif
}

const char *
stat_error() {
    return error;
}
