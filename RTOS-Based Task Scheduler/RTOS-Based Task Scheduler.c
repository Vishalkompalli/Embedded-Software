#include <stdio.h>
#include <stdbool.h>
#include<stdlib.h>



// #define MAX_TASKS 5

typedef struct {
    int task_id;
    int priority;
    int execution_time;
    int remaining_time;
    bool is_active;
} Task;


Task tasks[MAX_TASKS];

void initialize_tasks() {
    int RandomVal;
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].task_id = i;
        tasks[i].priority = i + 1; // Assign priorities from 1 to MAX_TASKS
        // tasks[i].execution_time = (i + 1) * 2; // Example: Task 1 has 2 units, Task 2 has 4 units, etc.
        RandomVal = (rand() % (10 - 1)) + 1;
        tasks[i].execution_time = RandomVal;
        tasks[i].remaining_time = tasks[i].execution_time;
        tasks[i].is_active = true;
    }
}


void schedule_tasks() {
    int time_slice = 1; // Time slice for round-robin
    bool all_tasks_completed = false;

    while (!all_tasks_completed) {
        all_tasks_completed = true;

        for (int i = 0; i < MAX_TASKS; i++) {
            if (tasks[i].is_active && tasks[i].remaining_time > 0) {
                printf("Executing Task %d (Priority: %d)\n", tasks[i].task_id, tasks[i].priority);

                if (tasks[i].remaining_time <= time_slice) {
                    tasks[i].remaining_time = 0;
                    tasks[i].is_active = false;
                    printf("Task %d completed.\n", tasks[i].task_id);
                } else {
                    tasks[i].remaining_time -= time_slice;
                }
                
                // Check if all tasks are completed
                if (tasks[i].remaining_time > 0) {
                    all_tasks_completed = false;
                }
            }
        }
    }
}

int main() {
    initialize_tasks();
    schedule_tasks();
    return 0;
}
