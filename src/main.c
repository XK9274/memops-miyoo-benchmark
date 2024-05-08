#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "neon.h"
#include "mi_sys.h"

#define ALIGN_UP(x, a) ((((x) + ((a) - 1)) / (a)) * (a))

void neon_memcpy(void *dest, const void *src, size_t n);

struct CopyParams {
    void *dest;
    const void *src;
    size_t size;
};

double measure_time(void (*func)(void *), void *arg) {
    clock_t start, end;
    start = clock();
    func(arg);
    end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double measure_time_mi_sys(MI_S32 (*func)(MI_PHY, MI_PHY, MI_U32), MI_PHY src, MI_PHY dst, MI_U32 size) {
    clock_t start, end;
    start = clock();
    func(src, dst, size);
    end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double measure_time_memset(void* (*func)(void*, int, size_t), void* ptr, int value, size_t size) {
    clock_t start, end;
    start = clock();
    func(ptr, value, size);
    end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

double measure_time_mi_sys_memset(MI_S32 (*func)(MI_PHY, MI_U32, MI_U32), MI_PHY ptr, MI_U32 value, MI_U32 size) {
    clock_t start, end;
    start = clock();
    func(ptr, value, size);
    end = clock();
    return (double)(end - start) / CLOCKS_PER_SEC;
}

void memcpy_standard_wrap(void *arg) {
    struct CopyParams *params = (struct CopyParams *)arg;
    memcpy(params->dest, params->src, params->size);
}

void memcpy_neon_wrap(void *arg) {
    struct CopyParams *params = (struct CopyParams *)arg;
    neon_memcpy(params->dest, params->src, params->size);
}

int main() {
    size_t sizes[] = { 1 * 1024 * 1024, 2 * 1024 * 1024, 3 * 1024 * 1024, 4 * 1024 * 1024, 5 * 1024 * 1024 };
    size_t num_sizes = sizeof(sizes) / sizeof(sizes[0]);

    for (size_t i = 0; i < num_sizes; i++) {
        size_t size = sizes[i];
        void *buffer = malloc(size * 2);
        if (!buffer) {
            perror("Failed to allocate memory");
            return 1;
        }

        struct CopyParams params_standard = { buffer, buffer + size, size };
        struct CopyParams params_neon = { buffer, buffer + size, size };

        memset(buffer + size, 0xAA, size);

        double standard_copy_time = measure_time(memcpy_standard_wrap, &params_standard);
        printf("\nStandard memcpy for %zu MB took: %f seconds\n", size / (1024 * 1024), standard_copy_time);

        double neon_copy_time = measure_time(memcpy_neon_wrap, &params_neon);
        printf("NEON memcpy for %zu MB took: %f seconds\n", size / (1024 * 1024), neon_copy_time);

        double standard_memset_time = measure_time_memset(memset, buffer, 0xBB, size);
        printf("Standard memset for %zu MB took: %f seconds\n", size / (1024 * 1024), standard_memset_time);

        free(buffer);
        
        // mi_sys related memops

        size = ALIGN_UP(size, 4096);

        MI_PHY phySrcBufAddr = 0;
        MI_PHY phyDstBufAddr = 0;

        MI_S32 ret = MI_SYS_MMA_Alloc(NULL, size, &phySrcBufAddr);
        if (ret != MI_SUCCESS) {
            printf("Failed to allocate source buffer\n");
            return 1;
        }

        ret = MI_SYS_MMA_Alloc(NULL, size, &phyDstBufAddr);
        if (ret != MI_SUCCESS) {
            printf("Failed to allocate destination buffer\n");
            MI_SYS_MMA_Free(phySrcBufAddr);
            return 1;
        }

        MI_SYS_MemsetPa(phySrcBufAddr, 0xAA, size);

        double mi_sys_copy_time = measure_time_mi_sys(MI_SYS_MemcpyPa, phySrcBufAddr, phyDstBufAddr, size);
        printf("\nMI_SYS_MemcpyPa for %zu MB took: %f seconds\n", size / (1024 * 1024), mi_sys_copy_time);

        double mi_sys_memset_time = measure_time_mi_sys_memset(MI_SYS_MemsetPa, phySrcBufAddr, 0xBB, size);
        printf("MI_SYS_MemsetPa for %zu MB took: %f seconds\n", size / (1024 * 1024), mi_sys_memset_time);

        MI_SYS_MMA_Free(phySrcBufAddr);
        MI_SYS_MMA_Free(phyDstBufAddr);
    }

    return 0;
}