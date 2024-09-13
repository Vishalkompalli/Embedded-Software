#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>


#define TIMEOUT 3 
#define TASK_COUNT 3

bool watchdog_kicked = false;
pthread_mutex_t lock;


typedef struct{

	int task_id;
	int work_duration;
}Task;

void* watchdog_timer(void * arg)
{
	while(1)
	{
		sleep(TIMEOUT);
		pthread_mutex_lock(&lock);
		if (!watchdog_kicked)
		{
			printf("Watchdog: Timeout! System resetting...\n");
		}
		else
		{
			printf("Watchdog: System healthy, Watchdog kicked.\n");
			watchdog_kicked = false;
		}
		pthread_mutex_unlock(&lock);
	}
	pthread_mutex_unlock(&lock);
	return NULL;

}

void* task_function(void* arg) 
{
    Task* task = (Task*) arg;
    
    while (1) 
    {
        printf("Task %d: Working for %d seconds...\n", task->task_id, task->work_duration);
        sleep(task->work_duration); // Simulate task work

        pthread_mutex_lock(&lock);
        watchdog_kicked = true; // Kick the watchdog after completing work
        printf("Task %d: Work done, kicking the watchdog!\n", task->task_id);
        pthread_mutex_unlock(&lock);

        sleep(1); // Task waits before starting next cycle
    }
    return NULL;
}


int main() 
{
    pthread_t watchdog_thread;
    pthread_t task_threads[TASK_COUNT];
    Task tasks[TASK_COUNT];

    pthread_mutex_init(&lock, NULL);

    // Create the watchdog timer thread
    pthread_create(&watchdog_thread, NULL, watchdog_timer, NULL);

    // Initialize and create task threads
    for (int i = 0; i < TASK_COUNT; i++) 
    {
        tasks[i].task_id = i + 1;
        tasks[i].work_duration = (i + 1) * 2; // Task work durations: 2, 4, 6 seconds
        pthread_create(&task_threads[i], NULL, task_function, (void*)&tasks[i]);
    }

    // Join all task threads (not needed if they run infinitely)
    for (int i = 0; i < TASK_COUNT; i++) 
    {
        pthread_join(task_threads[i], NULL);
    }

    // Join the watchdog thread
    pthread_join(watchdog_thread, NULL);

    pthread_mutex_destroy(&lock);
    return 0;
}
