#include <stdio.h>
#include <stat.h>

int
main(int argc, char **argv) {
    struct stat_memory_t memory;
    struct stat_disk_t disk;
    struct stat_cpu_t cpu;
    char hostname[STAT_HOSTNAME_MAX + 1];
    uint64_t uptime;

    stat_hostname(hostname);
    stat_memory(&memory);
    stat_disk("/", &disk);
    stat_uptime(&uptime);

    if (!stat_cpu(&cpu)) {
        printf("Error getting CPU: %s\n", stat_error());
    }

    printf("Hostname: '%s'\n", hostname);
    printf("Total Memory: %lu\n", memory.mem_total);
    printf("Free Memory: %lu\n", memory.mem_free);
    printf("Shared Memory: %lu\n", memory.mem_shared);
    printf("Buffer/Cache Memory: %lu\n", memory.mem_buffer);
    printf("Total Swap: %lu\n", memory.swap_total);
    printf("Free Swap: %lu\n", memory.swap_free);
    printf("Total Disk [/]: %lu\n", disk.total);
    printf("Free Disk [/]: %lu\n", disk.free);
    printf("CPU Count: %u\n", cpu.count);
    printf("Uptime: %lu\n", uptime);

    return 0;
}
