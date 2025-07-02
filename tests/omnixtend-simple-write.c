#include <stdio.h>
#include <stdint.h>
#include "mmio.h"

// Simple write test for OmniXtend
#define TEST_ADDR 0x100000000
#define TEST_DATA 0x123456789ABCDEF0

int main(void) {
    printf("=== OmniXtend Simple Write Test ===\n");
    printf("Writing to address: 0x%lx\n", TEST_ADDR);
    printf("Test data: 0x%lx\n", TEST_DATA);
    printf("==================================\n\n");

    // Step 1: Write test
    printf("Step 1: Writing data...\n");
    printf("  Writing 0x%lx to address 0x%lx\n", TEST_DATA, TEST_ADDR);
    
    reg_write64(TEST_ADDR, TEST_DATA);
    printf("  Write completed\n");
    
    // Step 2: Confirmation
    printf("Step 2: Write confirmation...\n");
    printf("  Write command sent successfully\n");
    printf("  Data should be written to memory\n");
    
    printf("\n=== Simple Write test completed ===\n");
    printf("Exiting...\n");
    
    return 0;
} 