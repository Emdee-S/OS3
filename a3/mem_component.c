#include "mem_component.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ============================================================================
// HELPER FUNCTIONS
// ============================================================================

void cleanup_memory(MemoryComponent* mem) {
    MemoryBlock* current = mem->head;
    while (current != NULL) {
        MemoryBlock* next = current->next;
        free(current);
        current = next;
    }
    mem->head = NULL;
    mem->process_count = 0;
    memset(mem->process_map, -1, sizeof(mem->process_map));
}

void merge_adjacent_free_blocks(MemoryComponent* mem) {
    MemoryBlock* current = mem->head;
    
    while (current != NULL && current->next != NULL) {
        if (current->process_id == -1 && current->next->process_id == -1) {
            // Merge two adjacent free blocks
            current->size += current->next->size;
            MemoryBlock* to_delete = current->next;
            current->next = to_delete->next;
            free(to_delete);
        } else {
            current = current->next;
        }
    }
}

MemoryBlock* get_head(const MemoryComponent* mem) {
    return mem->head;
}

// ============================================================================
// FIRST FIT MEMORY IMPLEMENTATION
// ============================================================================

void first_fit_init(FirstFitMemory* mem) {
    // Initialize with one free block covering entire memory
    mem->base.head = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    mem->base.head->start_address = 0;
    mem->base.head->size = 128; // TOTAL_MEMORY_UNITS
    mem->base.head->process_id = -1;
    mem->base.head->next = NULL;
    mem->base.process_count = 0;
    mem->base.TOTAL_MEMORY_UNITS = 128;
    memset(mem->base.process_map, -1, sizeof(mem->base.process_map));
}

void first_fit_cleanup(FirstFitMemory* mem) {
    cleanup_memory(&mem->base);
}

int first_fit_allocate_mem(FirstFitMemory* mem, int process_id, int num_units) {
    if (num_units < 3 || num_units > 10) {
        return -1;  // Invalid request size
    }
    
    MemoryBlock* current = mem->base.head;
    int nodes_traversed = 0;
    
    // Search for first block that can accommodate the request
    while (current != NULL) {
        nodes_traversed++;
        
        if (current->process_id == -1 && current->size >= num_units) {
            // Found a free block that can fit the request
            
            if (current->size == num_units) {
                // Exact fit - allocate entire block
                current->process_id = process_id;
                mem->base.process_map[process_id] = nodes_traversed - 1;  // Store block index
                mem->base.process_count++;
            } else {
                // Split the block
                MemoryBlock* new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
                new_block->start_address = current->start_address + num_units;
                new_block->size = current->size - num_units;
                new_block->process_id = -1;
                new_block->next = current->next;
                
                current->size = num_units;
                current->process_id = process_id;
                current->next = new_block;
                
                mem->base.process_map[process_id] = nodes_traversed - 1;
                mem->base.process_count++;
            }
            
            return nodes_traversed;
        }
        
        current = current->next;
    }
    
    return -1;  // No suitable block found
}

int first_fit_deallocate_mem(FirstFitMemory* mem, int process_id) {
    if (mem->base.process_map[process_id] == -1) {
        return -1;  // Process not found
    }
    
    // Find the block for this process
    MemoryBlock* current = mem->base.head;
    int block_index = mem->base.process_map[process_id];
    
    for (int i = 0; i < block_index && current != NULL; i++) {
        current = current->next;
    }
    
    if (current == NULL) {
        return -1;  // Block not found
    }
    
    current->process_id = -1;  // Mark as free
    mem->base.process_map[process_id] = -1;
    mem->base.process_count--;
    
    // Merge adjacent free blocks
    merge_adjacent_free_blocks(&mem->base);
    
    return 1;  // Success
}

int first_fit_fragment_count(FirstFitMemory* mem) {
    int fragments = 0;
    MemoryBlock* current = mem->base.head;
    
    while (current != NULL) {
        if (current->process_id == -1 && (current->size == 1 || current->size == 2)) {
            fragments++;
        }
        current = current->next;
    }
    
    return fragments;
}

// ============================================================================
// BEST FIT MEMORY IMPLEMENTATION
// ============================================================================

void best_fit_init(BestFitMemory* mem) {
    // Initialize with one free block covering entire memory
    mem->base.head = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    mem->base.head->start_address = 0;
    mem->base.head->size = 128; // TOTAL_MEMORY_UNITS
    mem->base.head->process_id = -1;
    mem->base.head->next = NULL;
    mem->base.process_count = 0;
    mem->base.TOTAL_MEMORY_UNITS = 128;
    memset(mem->base.process_map, -1, sizeof(mem->base.process_map));
}

void best_fit_cleanup(BestFitMemory* mem) {
    cleanup_memory(&mem->base);
}

int best_fit_allocate_mem(BestFitMemory* mem, int process_id, int num_units) {
    if (num_units < 3 || num_units > 10) {
        return -1;  // Invalid request size
    }
    
    MemoryBlock* current = mem->base.head;
    MemoryBlock* best_fit = NULL;
    int best_fit_size = 129;  // Initialize to larger than possible
    int nodes_traversed = 0;
    
    // Find the smallest free block that can accommodate the request
    while (current != NULL) {
        nodes_traversed++;
        
        if (current->process_id == -1 && current->size >= num_units) {
            if (current->size < best_fit_size) {
                best_fit = current;
                best_fit_size = current->size;
            }
        }
        
        current = current->next;
    }
    
    if (best_fit == NULL) {
        return -1;  // No suitable block found
    }
    
    // Allocate to the best fit block
    if (best_fit->size == num_units) {
        // Exact fit - allocate entire block
        best_fit->process_id = process_id;
        mem->base.process_map[process_id] = nodes_traversed - 1;
        mem->base.process_count++;
    } else {
        // Split the block
        MemoryBlock* new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
        new_block->start_address = best_fit->start_address + num_units;
        new_block->size = best_fit->size - num_units;
        new_block->process_id = -1;
        new_block->next = best_fit->next;
        
        best_fit->size = num_units;
        best_fit->process_id = process_id;
        best_fit->next = new_block;
        
        mem->base.process_map[process_id] = nodes_traversed - 1;
        mem->base.process_count++;
    }
    
    return nodes_traversed;
}

int best_fit_deallocate_mem(BestFitMemory* mem, int process_id) {
    if (mem->base.process_map[process_id] == -1) {
        return -1;  // Process not found
    }
    
    // Find the block for this process
    MemoryBlock* current = mem->base.head;
    int block_index = mem->base.process_map[process_id];
    
    for (int i = 0; i < block_index && current != NULL; i++) {
        current = current->next;
    }
    
    if (current == NULL) {
        return -1;  // Block not found
    }
    
    current->process_id = -1;  // Mark as free
    mem->base.process_map[process_id] = -1;
    mem->base.process_count--;
    
    // Merge adjacent free blocks
    merge_adjacent_free_blocks(&mem->base);
    
    return 1;  // Success
}

int best_fit_fragment_count(BestFitMemory* mem) {
    int fragments = 0;
    MemoryBlock* current = mem->base.head;
    
    while (current != NULL) {
        if (current->process_id == -1 && (current->size == 1 || current->size == 2)) {
            fragments++;
        }
        current = current->next;
    }
    
    return fragments;
}
