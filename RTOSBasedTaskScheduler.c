#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_TASKS 5

typedef struct {
    int task_id;
    int priority;
    int execution_time;
    int remaining_time;
    bool is_active;
    pthread_t thread;
} Task;


Task tasks[MAX_TASKS];

void initialize_tasks() {
    for (int i = 0; i < MAX_TASKS; i++) {
        tasks[i].task_id = i;
        tasks[i].priority = i + 1; // Priorities from 1 to MAX_TASKS
        tasks[i].execution_time = (i + 1) * 2; // Example: 2, 4, 6, 8, 10 units
        tasks[i].remaining_time = tasks[i].execution_time;
        tasks[i].is_active = true;
    }
}

void* task_function(void* arg) {
    Task* task = (Task*)arg;

    while (task->remaining_time > 0) {
        printf("Task %d (Priority %d) is running...\n", task->task_id, task->priority);
        sleep(1); // Simulating task execution time (1 second)
        task->remaining_time--;

        if (task->remaining_time == 0) {
            printf("Task %d has completed.\n", task->task_id);
            task->is_active = false;
            pthread_exit(NULL); // Exit the thread when the task is complete
        }
    }

    return NULL;
}

void schedule_tasks() {
    for (int i = 0; i < MAX_TASKS; i++) {
        for (int j = 0; j < MAX_TASKS - 1; j++) {
            if (tasks[j].priority > tasks[j + 1].priority) {
                Task temp = tasks[j];
                tasks[j] = tasks[j + 1];
                tasks[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].is_active) {
            pthread_create(&tasks[i].thread, NULL, task_function, (void*)&tasks[i]);
        }
    }

    for (int i = 0; i < MAX_TASKS; i++) {
        if (tasks[i].is_active) {
            pthread_join(tasks[i].thread, NULL);
        }
    }
}

int main() {
    initialize_tasks();
    schedule_tasks();
    return 0;
}
