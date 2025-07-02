#include <stdio.h>
#include <stdint.h>
#include "mmio.h"

// Simple memory test for OmniXtend via TSI
#define MEMORY_BASE 0x80000000
#define TEST_SIZE 1024  // 1KB test

int main(void) {
    printf("=== OmniXtend Simple Memory Test ===\n");
    printf("Testing memory at base address: 0x%lx\n", MEMORY_BASE);
    printf("Test size: %d bytes\n", TEST_SIZE);
    printf("====================================\n\n");

    // Test 1: Write test data
    printf("Step 1: Writing test data to memory...\n");
    for (int i = 0; i < TEST_SIZE; i += 8) {
        uint64_t addr = MEMORY_BASE + i;
        uint64_t data = 0x123456789ABCDEF0 + i;
        reg_write64(addr, data);
        
        if (i % 256 == 0) {
            printf("  Written %d bytes\n", i);
        }
    }
    printf("Write completed.\n\n");

    // Test 2: Read and verify data
    printf("Step 2: Reading and verifying data...\n");
    int errors = 0;
    for (int i = 0; i < TEST_SIZE; i += 8) {
        uint64_t addr = MEMORY_BASE + i;
        uint64_t expected = 0x123456789ABCDEF0 + i;
        uint64_t read_data = reg_read64(addr);
        
        if (read_data != expected) {
            printf("  ERROR at addr 0x%lx: expected 0x%lx, got 0x%lx\n", 
                   addr, expected, read_data);
            errors++;
        }
        
        if (i % 256 == 0) {
            printf("  Verified %d bytes, errors: %d\n", i, errors);
        }
    }
    
    printf("Verification completed.\n");
    printf("Total errors: %d\n", errors);
    
    if (errors == 0) {
        printf("SUCCESS: All data written and read correctly!\n");
    } else {
        printf("FAILURE: %d errors detected\n", errors);
    }

    // Test 3: Pattern test
    printf("\nStep 3: Pattern test...\n");
    uint64_t patterns[] = {0xAAAAAAAAAAAAAAAA, 0x5555555555555555, 0xFFFFFFFFFFFFFFFF};
    
    for (int p = 0; p < 3; p++) {
        printf("  Testing pattern %d: 0x%lx\n", p, patterns[p]);
        
        // Write pattern
        for (int i = 0; i < 64; i += 8) {
            reg_write64(MEMORY_BASE + i, patterns[p]);
        }
        
        // Read and verify
        int pattern_errors = 0;
        for (int i = 0; i < 64; i += 8) {
            uint64_t read_data = reg_read64(MEMORY_BASE + i);
            if (read_data != patterns[p]) {
                pattern_errors++;
            }
        }
        
        printf("    Pattern %d: %d errors\n", p, pattern_errors);
    }

    printf("\n=== Test completed ===\n");
    return 0;
} 