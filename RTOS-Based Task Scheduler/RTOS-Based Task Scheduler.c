#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h> // For seeding rand()

#define MAX_TASKS 5
#define TIME_SLICE 1 // Time slice for round-robin
#define CONTEXT_SWITCH_OVERHEAD 1 // Units of time for context switch

typedef struct 
{
    int task_id;
    int priority;
    int execution_time;
    int remaining_time;
    int arrival_time;   // New: When the task becomes available
    int start_time;     // New: When the task first starts execution
    int completion_time;// New: When the task finishes execution
    int waiting_time;   // New: Total time spent waiting in ready queue
    int turnaround_time;// New: Total time from arrival to completion
    bool is_active;     // Indicates if the task is still running/waiting
    bool has_started;   // New: To track if task has started execution
} Task;

Task tasks[MAX_TASKS];
int current_time = 0; // Global time counter for the simulation

void initialize_tasks() 
{
    // Seed the random number generator
    srand(time(NULL));

    int RandomExecutionTime;
    int RandomArrivalTime;

    printf("Initializing Tasks:\n");
    for (int i = 0; i < MAX_TASKS; i++) 
    {
        tasks[i].task_id = i;
        tasks[i].priority = i + 1;

        RandomExecutionTime = (rand() % 10) + 1; // Execution time between 1 and 10
        tasks[i].execution_time = RandomExecutionTime;
        tasks[i].remaining_time = tasks[i].execution_time;

        // Arrival time random, ensuring some tasks might arrive later
        // Max arrival time is limited to prevent excessively long simulations
        RandomArrivalTime = (rand() % (MAX_TASKS * 2)); // Arrival between 0 and 2*MAX_TASKS-1
        tasks[i].arrival_time = RandomArrivalTime;

        tasks[i].start_time = -1; // Not started yet
        tasks[i].completion_time = -1; // Not completed yet
        tasks[i].waiting_time = 0;
        tasks[i].turnaround_time = 0;
        tasks[i].is_active = true;
        tasks[i].has_started = false;

        printf("  Task %d: Prio=%d, ExecTime=%d, ArrivalTime=%d\n",
               tasks[i].task_id, tasks[i].priority, tasks[i].execution_time, tasks[i].arrival_time);
    }
    printf("\n");
}

void schedule_tasks() 
{
    bool all_tasks_completed = false;
    int tasks_remaining = MAX_TASKS;

    printf("Starting Simulation (Round Robin with Context Switching):\n");

    // Loop until all tasks are completed
    while (tasks_remaining > 0) 
    {
        bool task_executed_in_this_cycle = false;

        for (int i = 0; i < MAX_TASKS; i++) 
        {
            // Only consider active tasks that have arrived and still have work
            if (tasks[i].is_active && tasks[i].remaining_time > 0 && current_time >= tasks[i].arrival_time) 
            {
                // If this is the first time the task is starting, record its start_time
                if (!tasks[i].has_started) 
                {
                    tasks[i].start_time = current_time;
                    tasks[i].has_started = true;
                }

                printf("Time %d: Executing Task %d (Priority: %d, Remaining: %d)\n",
                       current_time, tasks[i].task_id, tasks[i].priority, tasks[i].remaining_time);

                // Determine actual execution time for this slice (min of remaining or time_slice)
                int actual_execution = (tasks[i].remaining_time < TIME_SLICE) ? tasks[i].remaining_time : TIME_SLICE;
                
                tasks[i].remaining_time -= actual_execution;
                current_time += actual_execution; // Advance time by actual execution

                task_executed_in_this_cycle = true; // A task was executed

                if (tasks[i].remaining_time == 0) 
                {
                    tasks[i].is_active = false;
                    tasks[i].completion_time = current_time;
                    tasks[i].turnaround_time = tasks[i].completion_time - tasks[i].arrival_time;
                    tasks[i].waiting_time = tasks[i].turnaround_time - tasks[i].execution_time;
                    tasks_remaining--;
                    printf("Time %d: Task %d completed.\n", current_time, tasks[i].task_id);
                }
                
                // If there are still active tasks and we just executed one, apply context switch overhead
                // This applies if the task just completed, or if it still has remaining time and will yield
                if (tasks_remaining > 0 && tasks[i].is_active) 
                { // Only add overhead if CPU won't idle after this
                     printf("Time %d: Applying Context Switch Overhead (%d units).\n", current_time, CONTEXT_SWITCH_OVERHEAD);
                     current_time += CONTEXT_SWITCH_OVERHEAD;
                } 
                else if (tasks_remaining > 0 && !tasks[i].is_active) 
                {
                     // If task completed, and there are still tasks left, assume a context switch will occur
                     printf("Time %d: Applying Context Switch Overhead (%d units) after Task %d completion.\n", current_time, CONTEXT_SWITCH_OVERHEAD, tasks[i].task_id);
                     current_time += CONTEXT_SWITCH_OVERHEAD;
                }
            }
        }
        
        // If no tasks were executed in this full cycle (e.g., waiting for tasks to arrive), advance time
        // This prevents an infinite loop if all tasks haven't arrived yet
        if (!task_executed_in_this_cycle && tasks_remaining > 0) 
        {
            printf("Time %d: CPU Idle (waiting for tasks to arrive or become ready).\n", current_time);
            current_time++; // Advance time by 1 unit to allow new tasks to arrive
        }
    }

    printf("\nSimulation Completed at Time: %d\n", current_time);

    // Print summary of task metrics
    printf("\n--- Task Metrics ---\n");
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
        printf("Task %d:\n", tasks[i].task_id);
        printf("  Arrival Time: %d\n", tasks[i].arrival_time);
        printf("  Execution Time: %d\n", tasks[i].execution_time);
        printf("  Start Time: %d\n", tasks[i].start_time);
        printf("  Completion Time: %d\n", tasks[i].completion_time);
        printf("  Turnaround Time: %d\n", tasks[i].turnaround_time);
        printf("  Waiting Time: %d\n", tasks[i].waiting_time);
        total_turnaround_time += tasks[i].turnaround_time;
        total_waiting_time += tasks[i].waiting_time;
    }
    printf("\nAverage Turnaround Time: %.2f\n", total_turnaround_time / MAX_TASKS);
    printf("Average Waiting Time: %.2f\n", total_waiting_time / MAX_TASKS);
}

int main() {
    initialize_tasks();
    schedule_tasks();
    return 0;
}