#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mem_component.h"

// Function to print memory layout
void print_memory_layout(MemoryComponent* mem, const char* name) {
    printf("\n%s Memory Layout:\n", name);
    printf("==================\n");
    
    MemoryBlock* current = get_head(mem);
    int block_num = 0;
    
    while (current != NULL) {
        printf("Block %d: ", block_num);
        if (current->process_id == -1) {
            printf("FREE  - Address: %d, Size: %d units\n", 
                   current->start_address, current->size);
        } else {
            printf("PROC %d - Address: %d, Size: %d units\n", 
                   current->process_id, current->start_address, current->size);
        }
        current = current->next;
        block_num++;
    }
    printf("\n");
}

int main() {
    printf("Memory Allocation Simulation Program - Memory Component Test\n");
    printf("==========================================================\n\n");
    
    // Create memory components for both techniques
    FirstFitMemory firstFit;
    BestFitMemory bestFit;
    
    // Initialize the memory components
    first_fit_init(&firstFit);
    best_fit_init(&bestFit);
    
    printf("Memory components created successfully!\n");
    printf("Initial memory state: 128 units (256 KB) all free\n\n");
    
    // Test 1: Basic allocation
    printf("=== Test 1: Basic Allocation ===\n");
    int result1 = first_fit_allocate_mem(&firstFit, 1, 5);
    int result2 = best_fit_allocate_mem(&bestFit, 1, 5);
    
    printf("First Fit: Process 1 allocated 5 units, nodes traversed: %d\n", result1);
    printf("Best Fit:  Process 1 allocated 5 units, nodes traversed: %d\n", result2);
    
    print_memory_layout(&firstFit.base, "First Fit");
    print_memory_layout(&bestFit.base, "Best Fit");
    
    // Test 2: Multiple allocations
    printf("=== Test 2: Multiple Allocations ===\n");
    int result3 = first_fit_allocate_mem(&firstFit, 2, 8);
    int result4 = best_fit_allocate_mem(&bestFit, 2, 8);
    
    printf("First Fit: Process 2 allocated 8 units, nodes traversed: %d\n", result3);
    printf("Best Fit:  Process 2 allocated 8 units, nodes traversed: %d\n", result4);
    
    print_memory_layout(&firstFit.base, "First Fit");
    print_memory_layout(&bestFit.base, "Best Fit");
    
    // Test 3: Fragment counting
    printf("=== Test 3: Fragment Counting ===\n");
    int frags1 = first_fit_fragment_count(&firstFit);
    int frags2 = best_fit_fragment_count(&bestFit);
    
    printf("First Fit fragments: %d\n", frags1);
    printf("Best Fit fragments: %d\n", frags2);
    
    // Test 4: Deallocation
    printf("\n=== Test 4: Deallocation ===\n");
    int dealloc1 = first_fit_deallocate_mem(&firstFit, 1);
    int dealloc2 = best_fit_deallocate_mem(&bestFit, 1);
    
    printf("First Fit: Process 1 deallocated, result: %d\n", dealloc1);
    printf("Best Fit:  Process 1 deallocated, result: %d\n", dealloc2);
    
    print_memory_layout(&firstFit.base, "First Fit (after deallocation)");
    print_memory_layout(&bestFit.base, "Best Fit (after deallocation)");
    
    // Test 5: Fragment counting after deallocation
    printf("=== Test 5: Fragment Counting After Deallocation ===\n");
    frags1 = first_fit_fragment_count(&firstFit);
    frags2 = best_fit_fragment_count(&bestFit);
    
    printf("First Fit fragments: %d\n", frags1);
    printf("Best Fit fragments: %d\n", frags2);
    
    // Test 6: Edge cases
    printf("\n=== Test 6: Edge Cases ===\n");
    
    // Try to allocate more than available
    int result5 = first_fit_allocate_mem(&firstFit, 3, 200);
    printf("First Fit: Process 3 tried to allocate 200 units, result: %d\n", result5);
    
    // Try invalid size
    int result6 = first_fit_allocate_mem(&firstFit, 4, 2);
    printf("First Fit: Process 4 tried to allocate 2 units (invalid), result: %d\n", result6);
    
    // Try to deallocate non-existent process
    int dealloc3 = first_fit_deallocate_mem(&firstFit, 999);
    printf("First Fit: Tried to deallocate process 999, result: %d\n", dealloc3);
    
    // Test 7: Fill memory and test fragmentation
    printf("\n=== Test 7: Memory Filling and Fragmentation ===\n");
    
    // Initialize random seed
    srand((unsigned int)time(NULL));
    
    // Allocate remaining memory in small chunks
    int process_id = 10;
    int total_allocated = 0;
    
    while (total_allocated < 128) {
        int size = 3 + (rand() % 8);  // Random size 3-10
        if (total_allocated + size <= 128) {
            int result = first_fit_allocate_mem(&firstFit, process_id, size);
            if (result > 0) {
                printf("Process %d allocated %d units, nodes traversed: %d\n", 
                       process_id, size, result);
                total_allocated += size;
                process_id++;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    
    printf("Total memory allocated: %d units\n", total_allocated);
    print_memory_layout(&firstFit.base, "First Fit (Memory Filled)");
    
    // Final fragment count
    frags1 = first_fit_fragment_count(&firstFit);
    printf("Final fragment count: %d\n", frags1);
    
    // Cleanup
    first_fit_cleanup(&firstFit);
    best_fit_cleanup(&bestFit);
    
    printf("\nMemory component testing completed!\n");
    return 0;
}
