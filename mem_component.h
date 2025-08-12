// This file defines the interface for mem allocation, deallocation, and fragment count
#ifndef MEM_COMPONENT_H
#define MEM_COMPONENT_H

// Memory block structure for linked list
typedef struct MemoryBlock {
    int start_address;      // Starting address of the block
    int size;              // Size of the block in units
    int process_id;        // Process ID if allocated, -1 if free
    struct MemoryBlock* next;     // Pointer to next block
} MemoryBlock;

// Memory allocation/deallocation technique interface
typedef struct MemoryComponent {
    MemoryBlock* head;                    // Head of the linked list
    int process_map[1000];               // Simple array to map process_id to block index
    int process_count;                   // Number of active processes
    int TOTAL_MEMORY_UNITS;              // 256 KB / 2 KB = 128 units
} MemoryComponent;

// Function declarations for memory component
void cleanup_memory(MemoryComponent* mem);
void merge_adjacent_free_blocks(MemoryComponent* mem);
MemoryBlock* get_head(const MemoryComponent* mem);

// First Fit memory allocation technique
typedef struct {
    MemoryComponent base;
} FirstFitMemory;

// First Fit function declarations
void first_fit_init(FirstFitMemory* mem);
void first_fit_cleanup(FirstFitMemory* mem);
int first_fit_allocate_mem(FirstFitMemory* mem, int process_id, int num_units);
int first_fit_deallocate_mem(FirstFitMemory* mem, int process_id);
int first_fit_fragment_count(FirstFitMemory* mem);

// Best Fit memory allocation technique
typedef struct {
    MemoryComponent base;
} BestFitMemory;

// Best Fit function declarations
void best_fit_init(BestFitMemory* mem);
void best_fit_cleanup(BestFitMemory* mem);
int best_fit_allocate_mem(BestFitMemory* mem, int process_id, int num_units);
int best_fit_deallocate_mem(BestFitMemory* mem, int process_id);
int best_fit_fragment_count(BestFitMemory* mem);

#endif // MEM_COMPONENT_H
