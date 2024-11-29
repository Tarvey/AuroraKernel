#include "aurora_utils.h"
#include "yield.h"

void create_task(void (*task_func)()) {
    Task *new_task = (Task *)simple_malloc(sizeof(Task));
    if (!new_task) {
        alk_panic("Out of memory while creating new task!");
        return;
    }
    new_task->task_func = task_func;
    new_task->stack = (char *)simple_malloc(STACK_SIZE);
    if (!new_task->stack) {
        simple_free(new_task);
        alk_panic("Out of memory while allocating stack for new task!");
        return;
    }
    new_task->next = 0;

    if (!task_list) {
        task_list = new_task;
        current_task = new_task;
    } else {
        Task *temp = task_list;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = new_task;
    }
}