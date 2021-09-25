#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

const size_t STACK_MEMORY_EXPAND = 2;
const size_t STACK_MEMORY_SHRINK = 3;
const void* const ERR_PTR = (void*)(1000-7);
const void* const ERR_CALLOC_PTR = (void*)(300);
const void* const ERR_REALLOC_PTR = (void*)(666);

static void dump(stack_t *stack, const char *nameFunction);

FILE *foutput = nullptr;

static StackErrorCode GetStackStatus(stack_t *stack)
{
    if (stack == nullptr)
    {
        return STACK_NOT_EXIST;
    }

    if (stack->data == ERR_PTR)
    {
        return STACK_DATA_DESTROY;
    }

    if (stack->data == ERR_CALLOC_PTR)
    {
        return STACK_DATA_CALLOC_ERROR;
    }

    if (stack->data == ERR_REALLOC_PTR)
    {
        return STACK_DATA_REALLOC_ERROR;
    }

    if (stack->capacity < stack->size)
    {
        return STACK_CAPACITY_LESS_SIZE;
    }

    return STACK_NO_ERROR;
}

static const char *StackErrorToString(StackErrorCode error)
{
    switch(error)
    {
        case STACK_NO_ERROR:           {return "STACK_NO_ERROR";          }
        case STACK_NOT_EXIST:          {return "STACK_NOT_EXIST";         }
        case STACK_DATA_DESTROY:       {return "STACK_DATA_DESTROY";      }
        case STACK_POP_FROM_EMPTY:     {return "STACK_POP_FROM_EMPTY";    }
        case STACK_DATA_CALLOC_ERROR:  {return "STACK_DATA_CALLOC_ERROR"; }
        case STACK_DATA_REALLOC_ERROR: {return "STACK_DATA_REALLOC_ERROR";}
        case STACK_CAPACITY_LESS_SIZE: {return "STACK_CAPACITY_LESS_SIZE";}
        default:                       {return nullptr;                   }
    }
}

static const char *StackStatusToString(StackStatus status)
{
    switch(status)
    {
        case STACK_NOT_CONSTRUCTED: {return "STACK_NOT_CONSTRUCTED";}
        case STACK_CONSTRUCTED:     {return "STACK_CONSTRUCTED";    }
        case STACK_DESTROYED:       {return "STACK_DESTROYED";      }
        default:                    {return nullptr;                }
    }
}

StackErrorCode StackCtor(stack_t *stack, size_t capacity)
{
    StackErrorCode stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    if (stack->status != STACK_NOT_CONSTRUCTED)
    {
        printf("1\n");
        return STACK_ALREADY_CONSTRUCTED;
    }

    stack->data = (stackData_t *) calloc(capacity, sizeof(stackData_t));
    if (stack->data == nullptr)
    {
        return STACK_DATA_CALLOC_ERROR;
    }
    stack->capacity = capacity;
    stack->size = 0;
    stack->status = STACK_CONSTRUCTED;

    dump(stack, "StackCtor");
    return stackStatus;
}

static StackErrorCode reallocStack(stack_t *stack, size_t newCapacity)
{
    assert(newCapacity > 0);

    StackErrorCode stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    stackData_t *data = (stackData_t *)realloc(stack->data, newCapacity * sizeof(stackData_t));
    if (data == nullptr)
    {
        return STACK_DATA_REALLOC_ERROR;
    }

    stack->data = data;
    stack->capacity = newCapacity;
    dump(stack, "reallocStack");
    return stackStatus;
}

StackErrorCode StackDtor(stack_t *stack)
{
    StackErrorCode stackStatus = GetStackStatus(stack);
    if (stackStatus == STACK_NO_ERROR)
    {
        free(stack->data);
        stack->size = 0;
        stack->data = (stackData_t*)ERR_PTR;
        stack->capacity = 0;
        stack->status = STACK_DESTROYED;
    }
    dump(stack, "StackDtor");
    return stackStatus;
}

StackErrorCode StackPush(stack_t *stack, stackData_t element)
{
    StackErrorCode stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    if (stack->size >= stack->capacity)
    {
        size_t newCapacity = (stack->capacity) * STACK_MEMORY_EXPAND;
        stackStatus = reallocStack(stack, newCapacity);
        if (stackStatus != STACK_NO_ERROR)
        {
            return stackStatus;
        }
    }

    stack->data[stack->size] = element;
    stack->size++;

    dump(stack, "StackPush");
    return stackStatus;
}

StackErrorCode StackPop(stack_t *stack, stackData_t *top)
{
    StackErrorCode stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    if (stack->size > 0)
    {
        *top = stack->data[stack->size - 1];
        stack->size--;

        if (stack->size != 0 && (stack->capacity / STACK_MEMORY_SHRINK > stack->size))
        {
            size_t newCapacity = (stack->capacity) / STACK_MEMORY_SHRINK;
            stackStatus = reallocStack(stack, newCapacity);
            return stackStatus;
        }

        dump(stack, "StackPop");

        return STACK_NO_ERROR;
    }
    else
    {
        return STACK_POP_FROM_EMPTY;
    }
}

size_t getStackCapacity(stack_t *stack)
{
    int stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    return stack->capacity;
}

size_t getStackSize(stack_t *stack)
{
    int stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    return stack->size;
}

static void dump(stack_t *stack, const char *nameFunction)
{
    assert(stack != nullptr);

    foutput = fopen("output.txt", "a");

    StackErrorCode stackStatus = GetStackStatus(stack);

    fprintf(foutput, "================================================================================\n");
    fprintf(foutput, "dump called in %s\n", nameFunction);
    fprintf(foutput, "%s\n", StackErrorToString(stackStatus));
    fprintf(foutput, "%s\n", StackStatusToString(stack->status));
    fprintf(foutput, "Stack address: 0x%p\n", stack);
    fprintf(foutput, "Capacity address: 0x%p\n", &(stack->capacity));
    fprintf(foutput, "Capacity: %Iu\n", stack->capacity);
    fprintf(foutput, "size address: 0x%p\n", &(stack->size));
    fprintf(foutput, "size: %Iu\n", stack->size);
    fprintf(foutput, "Data address: 0x%p\nData:\n", stack->data);

    for (size_t i = 0; i < stack->capacity; i++)
    {
        fprintf(foutput, "[%Iu] - %d\n", i, stack->data[i]);
    }

    fprintf(foutput, "--------------------------------------------------------------------------------\n");

    fclose(foutput);
}
