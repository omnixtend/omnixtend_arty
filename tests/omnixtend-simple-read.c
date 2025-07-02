#include <stdio.h>
#include <stdint.h>
#include "mmio.h"

// Simple read test for OmniXtend
#define BASE_ADDR 0x100000000  // Back to 4GB - commonly used in RISC-V
#define BLOCK_SIZE 0x1000  // 4KB
#define NUM_READS 10

int main(void) {
    printf("=== OmniXtend Simple Read Test ===\n");
    printf("Reading from 0x%lx in 4KB increments (%d times)\n", BASE_ADDR, NUM_READS);
    printf("==============================================\n\n");

    // Test a single read first to see if the address is accessible
    printf("Testing single read first...\n");
    uint64_t test_read = reg_read64(BASE_ADDR);
    printf("Initial read from 0x%lx: 0x%lx\n\n", BASE_ADDR, test_read);

    for (int i = 0; i < NUM_READS; i++) {
        uint64_t addr = BASE_ADDR + (i * BLOCK_SIZE);
        
        printf("Read %d: Reading from address 0x%lx\n", i + 1, addr);
        
        // Step 1: Read test with basic read first
        uint64_t read_data = reg_read64(addr);
        printf("  Read data: 0x%lx\n", read_data);
        
        // Add a small delay between reads
        delay_us(100); // 100 microseconds delay
        
        printf("\n");
    }
    
    printf("=== Simple Read test completed ===\n");
    printf("Read %d blocks from 0x%lx to 0x%lx\n", 
           NUM_READS, BASE_ADDR, BASE_ADDR + (NUM_READS - 1) * BLOCK_SIZE);
    printf("Exiting...\n");
    
    return 0;
} 