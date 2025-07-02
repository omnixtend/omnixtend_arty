#include <stdio.h>
#include <stdint.h>
#include "mmio.h"

// Basic GET/PUT test for OmniXtend
//#define TEST_ADDR 0x80000000
#define TEST_ADDR 0x100000000
#define TEST_DATA 0x123456789ABCDEF0

int main(void) {
    printf("=== OmniXtend GET/PUT Basic Test ===\n");
    printf("Testing address: 0x%lx\n", TEST_ADDR);
    printf("Test data: 0x%lx\n", TEST_DATA);
    printf("==================================\n\n");

    // Step 1: PUT (Write) test with cache flush
    printf("Step 1: PUT test (writing data with cache flush)...\n");
    printf("  Writing 0x%lx to address 0x%lx\n", TEST_DATA, TEST_ADDR);
    
    // Use cache-aware write function
    reg_write64_flush(TEST_ADDR, TEST_DATA);
    printf("  PUT completed with cache flush\n\n");
    
    // Add delay to ensure write propagates
    printf("  Waiting for write to propagate...\n");
    delay_us(1000); // 1ms delay
    printf("  Delay completed\n\n");
    
    // Step 2: GET (Read) test with cache invalidate
    printf("Step 2: GET test (reading data with cache invalidate)...\n");
    printf("  Reading from address 0x%lx\n", TEST_ADDR);
    
    // Use cache-aware read function
    uint64_t read_data = reg_read64_invalidate(TEST_ADDR);
    printf("  GET result: 0x%lx\n\n", read_data);
    
    // Step 3: Verification
    printf("Step 3: Verification...\n");
    if (read_data == TEST_DATA) {
        printf("  SUCCESS: PUT and GET working correctly!\n");
        printf("  OmniXtend basic functionality confirmed\n");
    } else {
        printf("  FAILURE: Data mismatch!\n");
        printf("  Expected: 0x%lx\n", TEST_DATA);
        printf("  Got:      0x%lx\n", read_data);
        printf("  OmniXtend may not be working properly\n");
        
        // Try reading again with regular function for comparison
        printf("  Trying regular read for comparison...\n");
        uint64_t read_data2 = reg_read64(TEST_ADDR);
        printf("  Regular read result: 0x%lx\n", read_data2);
    }
    
    printf("\n=== Basic GET/PUT test completed ===\n");
    printf("Exiting...\n");
    
    return 0;
} 