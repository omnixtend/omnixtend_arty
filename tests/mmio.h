#ifndef __MMIO_H__
#define __MMIO_H__

#include <stdint.h>
#include <stdio.h>

// Memory barrier macros for cache coherency
#ifdef __x86_64__
#include <x86intrin.h>
#define MEMORY_BARRIER() _mm_mfence()
#define WRITE_BARRIER() _mm_sfence()
#define READ_BARRIER() _mm_lfence()
#elif defined(__aarch64__)
#define MEMORY_BARRIER() __asm__ __volatile__("dmb ish" : : : "memory")
#define WRITE_BARRIER() __asm__ __volatile__("dmb ishst" : : : "memory")
#define READ_BARRIER() __asm__ __volatile__("dmb ishld" : : : "memory")
#else
// Generic memory barriers
#define MEMORY_BARRIER() __asm__ __volatile__("" : : : "memory")
#define WRITE_BARRIER() __asm__ __volatile__("" : : : "memory")
#define READ_BARRIER() __asm__ __volatile__("" : : : "memory")
#endif

// Simple delay function for RISC-V compatibility
static inline void delay_cycles(int cycles)
{
    for (volatile int i = 0; i < cycles; i++) {
        __asm__ __volatile__("nop" : : : "memory");
    }
}

// Delay function that works on RISC-V
static inline void delay_us(int microseconds)
{
    // Approximate delay - adjust based on your CPU frequency
    // For a 1GHz CPU, roughly 1000 cycles = 1 microsecond
    int cycles = microseconds * 1000;
    delay_cycles(cycles);
}

// Cache flush functions
static inline void cache_flush_range(void *addr, size_t size)
{
#ifdef __x86_64__
	// Flush cache lines containing the address range
	char *ptr = (char *)addr;
	char *end = ptr + size;
	
	// Align to cache line boundary (64 bytes)
	ptr = (char *)((uintptr_t)ptr & ~63);
	
	while (ptr < end) {
		_mm_clflush(ptr);
		ptr += 64;
	}
	_mm_mfence();
#elif defined(__aarch64__)
	// ARM64 cache flush
	__asm__ __volatile__(
		"dc civac, %0\n"
		"dsb ish\n"
		: : "r" (addr) : "memory"
	);
#else
	// Generic cache flush - use memory barrier
	MEMORY_BARRIER();
#endif
}

static inline void cache_invalidate_range(void *addr, size_t size)
{
#ifdef __x86_64__
	// Invalidate cache lines
	char *ptr = (char *)addr;
	char *end = ptr + size;
	
	// Align to cache line boundary (64 bytes)
	ptr = (char *)((uintptr_t)ptr & ~63);
	
	while (ptr < end) {
		_mm_clflush(ptr);
		ptr += 64;
	}
	_mm_mfence();
#elif defined(__aarch64__)
	// ARM64 cache invalidate
	__asm__ __volatile__(
		"dc ivac, %0\n"
		"dsb ish\n"
		: : "r" (addr) : "memory"
	);
#else
	// Generic cache invalidate - use memory barrier
	MEMORY_BARRIER();
#endif
}

// Force cache flush for entire cache
static inline void cache_flush_all(void)
{
#ifdef __x86_64__
	// Flush entire cache
	__asm__ __volatile__("wbinvd" : : : "memory");
#elif defined(__aarch64__)
	// ARM64 flush all caches
	__asm__ __volatile__(
		"dc civac, xzr\n"
		"dsb ish\n"
		: : : "memory"
	);
#else
	// Generic flush all - use memory barrier
	MEMORY_BARRIER();
#endif
}

static inline void reg_write8(uintptr_t addr, uint8_t data)
{
	volatile uint8_t *ptr = (volatile uint8_t *) addr;
	*ptr = data;
	WRITE_BARRIER();
}

static inline uint8_t reg_read8(uintptr_t addr)
{
	volatile uint8_t *ptr = (volatile uint8_t *) addr;
	READ_BARRIER();
	return *ptr;
}

static inline void reg_write16(uintptr_t addr, uint16_t data)
{
	volatile uint16_t *ptr = (volatile uint16_t *) addr;
	*ptr = data;
	WRITE_BARRIER();
}

static inline uint16_t reg_read16(uintptr_t addr)
{
	volatile uint16_t *ptr = (volatile uint16_t *) addr;
	READ_BARRIER();
	return *ptr;
}

static inline void reg_write32(uintptr_t addr, uint32_t data)
{
	volatile uint32_t *ptr = (volatile uint32_t *) addr;
	*ptr = data;
	WRITE_BARRIER();
}

static inline uint32_t reg_read32(uintptr_t addr)
{
	volatile uint32_t *ptr = (volatile uint32_t *) addr;
	READ_BARRIER();
	return *ptr;
}

static inline void reg_write64(unsigned long addr, uint64_t data)
{
	volatile uint64_t *ptr = (volatile uint64_t *) addr;
	*ptr = data;
	WRITE_BARRIER();
}

static inline uint64_t reg_read64(unsigned long addr)
{
	volatile uint64_t *ptr = (volatile uint64_t *) addr;
	READ_BARRIER();
	return *ptr;
}

// Enhanced MMIO functions with cache management
static inline void reg_write64_flush(unsigned long addr, uint64_t data)
{
	reg_write64(addr, data);
	cache_flush_range((void *)addr, sizeof(uint64_t));
}

static inline uint64_t reg_read64_invalidate(unsigned long addr)
{
	cache_invalidate_range((void *)addr, sizeof(uint64_t));
	return reg_read64(addr);
}

// Debug helper functions
static inline void reg_write64_debug(unsigned long addr, uint64_t data, const char *desc)
{
	printf("DEBUG: Writing 0x%lx to 0x%lx (%s)\n", data, addr, desc);
	reg_write64_flush(addr, data);
	printf("DEBUG: Write completed\n");
}

static inline uint64_t reg_read64_debug(unsigned long addr, const char *desc)
{
	printf("DEBUG: Reading from 0x%lx (%s)\n", addr, desc);
	uint64_t result = reg_read64_invalidate(addr);
	printf("DEBUG: Read result: 0x%lx\n", result);
	return result;
}

// Force memory ordering for critical operations
static inline void reg_write64_ordered(unsigned long addr, uint64_t data)
{
	MEMORY_BARRIER();
	reg_write64(addr, data);
	cache_flush_range((void *)addr, sizeof(uint64_t));
	MEMORY_BARRIER();
}

static inline uint64_t reg_read64_ordered(unsigned long addr)
{
	MEMORY_BARRIER();
	cache_invalidate_range((void *)addr, sizeof(uint64_t));
	uint64_t result = reg_read64(addr);
	MEMORY_BARRIER();
	return result;
}

#endif
