#ifndef STACK_H
#define STACK_H

typedef int stackData_t;

enum StackStatus
{
    STACK_NOT_CONSTRUCTED,
    STACK_CONSTRUCTED,
    STACK_DESTROYED
};

struct stack_t
{
    size_t capacity;
    stackData_t *data;
    size_t size;
    StackStatus status;
};

enum StackErrorCode
{
    STACK_NO_ERROR,
    STACK_NOT_EXIST,
    STACK_ALREADY_CONSTRUCTED,
    STACK_DATA_DESTROY,
    STACK_POP_FROM_EMPTY,
    STACK_DATA_CALLOC_ERROR,
    STACK_DATA_REALLOC_ERROR,
    STACK_CAPACITY_LESS_SIZE
};

StackErrorCode StackCtor(stack_t *stack, size_t capacity);

StackErrorCode StackDtor(stack_t *stack);

StackErrorCode StackPush(stack_t *stack, int element);

StackErrorCode StackPop(stack_t *stack, int *top);

size_t GetStackCapacity(stack_t *stack);

size_t GetStackSize(stack_t *stack);

#endif
