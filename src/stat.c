#include <stdio.h>
#include <string.h>
#include <errno.h>
#if defined(_WIN32)
# include <sysinfoapi.h>
#else
# include <unistd.h>
# include <sys/sysinfo.h>
# include <sys/statvfs.h>
#endif
#include "../api/stat.h"

bool
stat_hostname(char *hostname) {
#if defined(_WIN32)
    if (!GetComputerName(hostname, STAT_HOSTNAME_MAX + 1)) {
        return false;
    }
#else
    if (gethostname(hostname, STAT_HOSTNAME_MAX + 1) == -1) {
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
        return false;
    }

    disk->total = info_total.QuadPart;
    disk->free = info_free.QuadPart;
#else
    struct statvfs info;

    if (statvfs(path, &info) != 0) {
        return false;
    }

    disk->total = info.f_blocks * info.f_frsize;
    disk->free = info.f_bavail * info.f_frsize;
#endif
    return true;
}
