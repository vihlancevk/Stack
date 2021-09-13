#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

const int STACK_MEMORY_EXPAND = 2;
const int STACK_MEMORY_SHRINK = 3;

FILE *foutput = nullptr;

int checkStack(Stack_t *stack)
{
    if (stack == nullptr)
    {
        return STACK_NOT_EXIST;
    }

    if (stack->capacity == 0 && stack->data == nullptr && stack->size == 0)
    {
        return STACK_NO_ERROR;
    }

    if (stack->data == nullptr)
    {
        return STACK_DATA_REALLOC_ERROR;
    }

    if (stack->capacity < stack->size)
    {
        return STACK_CAPACITY_LESS_SIZE;
    }

    return STACK_NO_ERROR;
}

static char *getStackError(int stackStatus)
{
    switch(stackStatus)
    {
        case STACK_NO_ERROR:           {return "STACK_NO_ERROR - %d\n";           break;}
        case STACK_NOT_EXIST:          {return "STACK_NOT_EXIST - %d\n";          break;}
        case POP_FROM_EMPTY_STACK:     {return "POP_FROM_EMPTY_STACK - %d\n";     break;}
        case STACK_DATA_REALLOC_ERROR: {return "STACK_DATA_REALLOC_ERROR - %d\n"; break;}
        default:                       {return "STACK_CAPACITY_LESS_size - %d\n"; break;}
    }
}

int createStack(Stack_t *stack, size_t capacity)
{
    int stackStatus = checkStack(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    stack->capacity = capacity;
    stack->data = (stackData_t *) calloc(capacity, sizeof(stackData_t));
    if (stack->data == nullptr)
    {
        dump(stack);
        return STACK_DATA_REALLOC_ERROR;
    }
    stack->size = 0;

    dump(stack);
    return stackStatus;
}

static int reallocStack(Stack_t *stack, size_t new_capacity)
{
    int stackStatus = checkStack(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    stackData_t *data = stack->data;
    stack->data = (stackData_t *)realloc(stack->data, new_capacity * sizeof(stackData_t));
    if (stack->data == nullptr)
    {
        free(data);
        return STACK_DATA_REALLOC_ERROR;
    }
    stack->capacity = new_capacity;
    return stackStatus;
}

int clearStack(Stack_t *stack)
{
    int stackStatus = checkStack(stack);
    if (stackStatus == STACK_NO_ERROR)
    {
        stack->size = 0;
    }

    dump(stack);
    return stackStatus;
}

int push(Stack_t *stack, stackData_t element)
{
    int stackStatus = checkStack(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        dump(stack);
        return stackStatus;
    }

    if (stack->size >= stack->capacity)
    {
        size_t new_capacity = (stack->capacity + 1) * STACK_MEMORY_EXPAND;
        stackStatus = reallocStack(stack, new_capacity);
        if (stackStatus != STACK_NO_ERROR)
        {
            dump(stack);
            return stackStatus;
        }
    }

    stack->data[stack->size] = element;
    stack->size++;

    dump(stack);
    return stackStatus;
}

int pop(Stack_t *stack, stackData_t *top)
{
    int stackStatus = checkStack(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        dump(stack);
        return stackStatus;
    }

    if (stack->size > 0)
    {
        *top = stack->data[stack->size - 1];
        stack->size--;
        if (stack->size != 0 && stack->capacity / STACK_MEMORY_SHRINK > stack->size)
        {
            size_t new_capacity = (stack->capacity) / STACK_MEMORY_SHRINK;
            stackStatus = reallocStack(stack, new_capacity);
            dump(stack);
            return stackStatus;
        }
        dump(stack);
        return STACK_NO_ERROR;
    }
    else
    {
        dump(stack);
        return POP_FROM_EMPTY_STACK;
    }
}

void dump(Stack_t *stack)
{
    foutput = fopen("output.txt", "a");

    int stackStatus = checkStack(stack);

    fprintf(foutput, "================================================================================\n");
    fprintf(foutput, "%s\n", getStackError(stackStatus));
    fprintf(foutput, "Stack address: %llu\n", stack);
    fprintf(foutput, "Capacity addres: %llu\n", &(stack->capacity));
    fprintf(foutput, "Capacity: %Iu\n", stack->capacity);
    fprintf(foutput, "size addres: %llu\n", &(stack->size));
    fprintf(foutput, "size: %Iu\n", stack->size);
    fprintf(foutput, "Data address: %llu\nData:\n", stack->data);


    for (size_t i = 0; i < stack->size; i++)
    {
        fprintf(foutput, "[%Iu] - %d\n", i, stack->data[i]);
    }
    fprintf(foutput, "--------------------------------------------------------------------------------\n");

    return;
}
