#ifndef STATS_H
#define STATS_H

// Statistics tracking and reporting component
typedef struct {
    int total_allocations;
    int successful_allocations;
    int failed_allocations;
    long total_nodes_traversed;
    long total_fragments;
    int request_count;
} StatisticsComponent;

// Function declarations
void stats_init(StatisticsComponent* stats);
void stats_cleanup(StatisticsComponent* stats);

// Update statistics after each allocation/deallocation request
void update_allocation_stats(StatisticsComponent* stats, int nodes_traversed, int allocation_successful);
void update_fragment_stats(StatisticsComponent* stats, int fragment_count);

// Get current performance metrics
double get_average_fragments(const StatisticsComponent* stats);
double get_average_allocation_time(const StatisticsComponent* stats);
double get_allocation_denial_percentage(const StatisticsComponent* stats);

// Print final statistics
void print_statistics(const StatisticsComponent* stats, const char* technique_name);

// Reset statistics for new simulation run
void reset_stats(StatisticsComponent* stats);

#endif // STATS_H
