#ifndef STACK_H
#define STACK_H

typedef int stackDataType;

struct Stack_t
{
    size_t capacity;
    stackDataType *data;
    size_t sizeStack;
};

enum StackErrorCode
{
    STACK_NO_ERROR = 0,
    STACK_NOT_EXIST = -1,
    STACK_IS_EMPTY = -2,
    STACK_DATA_REALLOC_ERROR = -3,
    STACK_CAPACITY_LESS_SIZESTACK = -4
};

extern const int STACK_MEMORY_RESIZE_UP;
extern const int STACK_MEMORY_RESIZE_DOWN;

int checkCorrectStack(Stack_t *stack);
void outputStackError(int stackStatus, FILE *foutput);
int createStack(Stack_t *stack, size_t capacity, FILE *foutput);
int reallocStack(Stack_t *stack, size_t new_capacity);
int clearStack(Stack_t *stack, FILE *foutput);
int push(Stack_t *stack, int element, FILE *foutput);
int pop(Stack_t *stack, int *top, FILE *foutput);
void dump(Stack_t *stack, FILE *foutput);

#endif
