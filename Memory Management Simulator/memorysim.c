#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BLOCKS  10
#define MAX_PROCESSES  5

//struct for memory block
typedef struct 
{
	int size;
	bool is_free;
	int process_id;
}MemoryBlock;

//struct for process
typedef struct 
{
	int size;
	int allocated_block_index;
}Process;

MemoryBlock memory_blocks[MAX_BLOCKS];
Process processes[MAX_PROCESSES];

//func to initialize memory blocks with random sizes

void initialize_memory_blocks()
{
	for (int i=0;i<MAX_BLOCKS;i++)
	{
		memory_blocks[i].size = (rand()%100)+50;
		memory_blocks[i].is_free = true;
		memory_blocks[i].process_id = -1;
	}
}


//func to display mem blocks
void display_memory_blocks()
{
	printf("\nMemory Blocks:\n");
	for (int i=0;i<MAX_BLOCKS;i++)
	{
		printf("Block %d: Size = %d, Free = %s, Process ID = %d\n", i, memory_blocks[i].size, memory_blocks[i].is_free ? "Yes" : "No", memory_blocks[i].process_id);

	}
}

//First Fit Allocation
void first_fit(Process *p, int process_id)
{
	for (int i=0;i<MAX_BLOCKS;i++)
	{
		if (memory_blocks[i].is_free && memory_blocks[i].size >= p->size)
		{
			p->allocated_block_index = i;
			memory_blocks[i].is_free = false;
			memory_blocks[i].process_id = process_id;
			break;
		}
	}
}

//Best fit Allocation
void best_fit(Process *p, int process_id)
{
	int best_index = -1;
	int best_size = __INT_MAX__;

	for(int i=0;i<MAX_BLOCKS;i++)
	{
		if(memory_blocks[i].is_free && memory_blocks[i].size >= p->size && memory_blocks[i].size < best_size)
		{
			best_size = memory_blocks[i].size;
			best_index = i;
		}
	}

	if(best_index!=-1)
	{
		p->allocated_block_index = best_index;
		memory_blocks[best_index].is_free = false;
		memory_blocks[best_index].process_id = process_id;
	}
}

//Worst fit Allocation

void worst_fit(Process *p, int process_id)
{
	int worst_index = -1;
	int worst_size = 0;

	for(int i=0; i<MAX_BLOCKS;i++)
	{
		if(memory_blocks[i].is_free && memory_blocks[i].size >= p->size && memory_blocks[i].size > worst_size)
		{
			worst_size = memory_blocks[i].size;
			worst_index = i;
		}
	}
	if(worst_index != -1)
	{
		p->allocated_block_index = worst_index;
		memory_blocks[worst_index].is_free = false;
		memory_blocks[worst_index].process_id = process_id;
	}
}

// Function to allocate memory using the specified algorithm
void allocate_memory(Process *p, int process_id, char algorithm) 
{
    switch(algorithm) 
    {
        case 'F':
            first_fit(p, process_id);
            break;
        case 'B':
            best_fit(p, process_id);
            break;
        case 'W':
            worst_fit(p, process_id);
            break;
        default:
            printf("Invalid allocation algorithm!\n");
            break;
    }
}

int main() 
{
    srand(time(0));

    // Initialize memory blocks
    initialize_memory_blocks();
    display_memory_blocks();

    // Example processes
    processes[0].size = 100;
    processes[1].size = 120;
    processes[2].size = 130;
    processes[3].size = 90;
    processes[4].size = 110;

    // Allocate memory using First Fit
    for (int i = 0; i < MAX_PROCESSES; i++) 
    {
        allocate_memory(&processes[i], i, 'F');
    }

    printf("\nAfter First Fit Allocation:\n");
    display_memory_blocks();

    // Reset memory blocks for another allocation strategy
    initialize_memory_blocks();

    // Allocate memory using Best Fit
    for (int i = 0; i < MAX_PROCESSES; i++) 
    {
        allocate_memory(&processes[i], i, 'B');
    }

    printf("\nAfter Best Fit Allocation:\n");
    display_memory_blocks();

    // Reset memory blocks for another allocation strategy
    initialize_memory_blocks();

    // Allocate memory using Worst Fit
    for (int i = 0; i < MAX_PROCESSES; i++) 
    {
        allocate_memory(&processes[i], i, 'W');
    }

    printf("\nAfter Worst Fit Allocation:\n");
    display_memory_blocks();

    return 0;
}