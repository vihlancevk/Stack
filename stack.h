#ifndef STACK_H
#define STACK_H

typedef int stackData_t;

struct Stack_t
{
    size_t capacity;
    stackData_t *data;
    size_t size;
};

enum StackErrorCode
{
    STACK_NO_ERROR = 0,
    STACK_NOT_EXIST = -1,
    POP_FROM_EMPTY_STACK = -2,
    STACK_DATA_REALLOC_ERROR = -3,
    STACK_CAPACITY_LESS_SIZE = -4
};

extern const int STACK_MEMORY_RESIZE_UP;
extern const int STACK_MEMORY_RESIZE_DOWN;

int checkStack(Stack_t *stack);

int createStack(Stack_t *stack, size_t capacity);

int clearStack(Stack_t *stack);

int push(Stack_t *stack, int element);

int pop(Stack_t *stack, int *top);

void dump(Stack_t *stack);

#endif
