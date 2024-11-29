typedef struct Task {
    void (*task_func)();
    char *stack;
    struct Task *next;
} Task;

Task *task_list = 0;
Task *current_task = 0;

#define STACK_SIZE 1024
#define MEMORY_POOL_SIZE (STACK_SIZE * 100) // Adjust as needed for the number of tasks

char memory_pool[MEMORY_POOL_SIZE];
unsigned int memory_pool_index = 0;

void *simple_malloc(unsigned int size) {
    if (memory_pool_index + size > MEMORY_POOL_SIZE) {
        return 0; // Out of memory
    }
    void *mem = &memory_pool[memory_pool_index];
    memory_pool_index += size;
    return mem;
}

void simple_free(void *ptr) {
    // No-op for simplicity; real implementations would require tracking allocations
}
