#ifndef STACK_H
#define STACK_H

struct Stack_t
{
    int capacity;
    int *date;
    size_t sizeStack;
};

enum ErrorCode
{
    NO_ERROR = 0,
    STACK_NOT_EXIST = -1,
    STACK_IS_EMPTY = -2
};

const int MEMORY_MULTIPLIER = 2;

int CheckExistenceStack(Stack_t *Stack);
int createStack(Stack_t *Stack, size_t maxElement);
int clearStack(Stack_t *Stack);
int push(Stack_t *Stack, int element);
int pop(Stack_t *Stack);
int outputStack(Stack_t *Stack);

#endif
