#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

const int STACK_MEMORY_RESIZE = 2;

int checkExistenceStack(Stack_t *thou)
{
    if (thou == nullptr)
    {
        return STACK_NOT_EXIST;
    }

    if (thou->data == nullptr)
    {
        return STACK_DATA_REALLOC_ERROR;
    }

    return STACK_NO_ERROR;
}

int createStack(Stack_t *thou, size_t capacity)
{
    assert(thou != nullptr);

    thou->capacity = capacity;
    thou->data = (stackDataType *) calloc(capacity, sizeof(stackDataType));
    if (thou->data == nullptr)
    {
        return STACK_DATA_REALLOC_ERROR;
    }
    thou->sizeStack = 0;

    return STACK_NO_ERROR;
}

int clearStack(Stack_t *thou)
{
    int stackStatus = checkExistenceStack(thou);
    if (stackStatus == STACK_NO_ERROR)
    {
        thou->sizeStack = 0;
    }

    return stackStatus;
}

int push(Stack_t *thou, stackDataType element)
{
    int stackStatus = checkExistenceStack(thou);
    if (stackStatus == STACK_NO_ERROR)
    {
        if (thou->sizeStack >= thou->capacity)
        {
            thou->capacity = (thou->capacity + 1) * STACK_MEMORY_RESIZE;
            thou->data = (stackDataType *) realloc(thou->data, (thou->capacity) * sizeof(stackDataType));
            if (thou->data == nullptr)
            {
                return STACK_DATA_REALLOC_ERROR;
            }
        }

        thou->data[thou->sizeStack] = element;
        thou->sizeStack++;
    }

    return stackStatus;
}

int pop(Stack_t *thou, stackDataType *top)
{
    int stackStatus = checkExistenceStack(thou);
    if (stackStatus == STACK_NO_ERROR)
    {
        if (thou->sizeStack > 0)
        {
            *top = thou->data[thou->sizeStack - 1];
            thou->sizeStack--;
            return STACK_NO_ERROR;
        }
        else
        {
            return STACK_IS_EMPTY;
        }
    }
    else
    {
        return stackStatus;
    }
}

int dump(Stack_t *thou)
{
    int stackStatus = checkExistenceStack(thou);
    if (stackStatus == STACK_NO_ERROR)
    {
        for (size_t i = 0; i < thou->sizeStack; i++)
        {
            printf("%d ", thou->data[i]);
        }

        printf("\n");
    }

    return stackStatus;
}
