#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

const int STACK_MEMORY_RESIZE_UP = 2;
const int STACK_MEMORY_RESIZE_DOWN = 3;


int checkCorrectStack(Stack_t *stack)
{
    if (stack == nullptr)
    {
        return STACK_NOT_EXIST;
    }

    if (stack->capacity == 0 && stack->data == nullptr && stack->sizeStack == 0)
    {
        return STACK_NO_ERROR;
    }

    if (stack->data == nullptr)
    {
        return STACK_DATA_REALLOC_ERROR;
    }

    if (stack->capacity < stack->sizeStack)
    {
        return STACK_CAPACITY_LESS_SIZESTACK;
    }

    return STACK_NO_ERROR;
}

void outputStackError(int stackStatus, FILE *foutput)
{
    switch(stackStatus)
    {
        case STACK_NO_ERROR:           {fprintf(foutput, "STACK_NO_ERROR - %d\n", stackStatus);                 break;}
        case STACK_NOT_EXIST:          {fprintf(foutput, "STACK_NOT_EXIST - %d\n", stackStatus);                break;}
        case STACK_IS_EMPTY:           {fprintf(foutput, "STACK_IS_EMPTY - %d\n", stackStatus);                 break;}
        case STACK_DATA_REALLOC_ERROR: {fprintf(foutput, "STACK_DATA_REALLOC_ERROR - %d\n", stackStatus);       break;}
        default:                       {fprintf(foutput, "STACK_CAPACITY_LESS_SIZESTACK - %d\n", stackStatus);  break;}
    }
}

int createStack(Stack_t *stack, size_t capacity, FILE *foutput)
{
    int stackStatus = checkCorrectStack(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    stack->capacity = capacity;
    stack->data = (stackDataType *) calloc(capacity, sizeof(stackDataType));
    if (stack->data == nullptr)
    {
        dump(stack, foutput);
        return STACK_DATA_REALLOC_ERROR;
    }
    stack->sizeStack = 0;

    dump(stack, foutput);
    return stackStatus;
}

int reallocStack(Stack_t *stack, size_t new_capacity)
{
    int stackStatus = checkCorrectStack(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    stackDataType *data = stack->data;
    stack->data = (stackDataType *)realloc(stack->data, new_capacity * sizeof(stackDataType));
    if (stack->data == nullptr)
    {
        free(data);
        return STACK_DATA_REALLOC_ERROR;
    }
    stack->capacity = new_capacity;
    return stackStatus;
}

int clearStack(Stack_t *stack, FILE *foutput)
{
    int stackStatus = checkCorrectStack(stack);
    if (stackStatus == STACK_NO_ERROR)
    {
        stack->sizeStack = 0;
    }

    dump(stack, foutput);
    return stackStatus;
}

int push(Stack_t *stack, stackDataType element, FILE *foutput)
{
    int stackStatus = checkCorrectStack(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        dump(stack, foutput);
        return stackStatus;
    }

    if (stack->sizeStack >= stack->capacity)
    {
        size_t new_capacity = (stack->capacity + 1) * STACK_MEMORY_RESIZE_UP;
        stackStatus = reallocStack(stack, new_capacity);
        if (stackStatus != STACK_NO_ERROR)
        {
            dump(stack, foutput);
            return stackStatus;
        }
    }

    stack->data[stack->sizeStack] = element;
    stack->sizeStack++;

    dump(stack, foutput);
    return stackStatus;
}

int pop(Stack_t *stack, stackDataType *top, FILE *foutput)
{
    int stackStatus = checkCorrectStack(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        dump(stack, foutput);
        return stackStatus;
    }

    if (stack->sizeStack > 0)
    {
        *top = stack->data[stack->sizeStack - 1];
        stack->sizeStack--;
        if (stack->sizeStack != 0 && stack->capacity / STACK_MEMORY_RESIZE_DOWN > stack->sizeStack)
        {
            size_t new_capacity = (stack->capacity) / STACK_MEMORY_RESIZE_DOWN;
            stackStatus = reallocStack(stack, new_capacity);
            dump(stack, foutput);
            return stackStatus;
        }
        dump(stack, foutput);
        return STACK_NO_ERROR;
    }
    else
    {
        dump(stack, foutput);
        return STACK_IS_EMPTY;
    }
}

void dump(Stack_t *stack, FILE *foutput)
{
    int stackStatus = checkCorrectStack(stack);

    fprintf(foutput, "================================================================================\n");
    outputStackError(stackStatus, foutput);
    fprintf(foutput, "Stack address: %llu\n", stack);
    fprintf(foutput, "Capacity addres: %llu\n", &(stack->capacity));
    fprintf(foutput, "Capacity: %Iu\n", stack->capacity);
    fprintf(foutput, "SizeStack addres: %llu\n", &(stack->sizeStack));
    fprintf(foutput, "SizeStack: %Iu\n", stack->sizeStack);
    fprintf(foutput, "Data address: %llu\nData:\n", stack->data);


    for (size_t i = 0; i < stack->sizeStack; i++)
    {
        fprintf(foutput, "[%Iu] - %d\n", i, stack->data[i]);
    }
    fprintf(foutput, "--------------------------------------------------------------------------------\n");
    return;
}
