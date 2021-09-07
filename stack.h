#ifndef STACK_H
#define STACK_H

struct Stack_t
{
    int capacity;
    int *data;
    size_t sizeStack;
};

enum StackErrorCode
{
    STACK_NO_ERROR = 0,
    STACK_NOT_EXIST = -1,
    STACK_IS_EMPTY = -2,
    STACK_DATA_NOT_EMPTY = -3
};

extern const int STACK_MEMORY_RESIZE;

int checkExistenceStack(Stack_t *thou);
int createStack(Stack_t *thou, size_t capacity);
int clearStack(Stack_t *thou);
int push(Stack_t *thou, int element);
int pop(Stack_t *thou, int *top);
int dump(Stack_t *thou);

#endif
