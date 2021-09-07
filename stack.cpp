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
        return STACK_DATA_NOT_EMPTY;
    }

    return STACK_NO_ERROR;
}

int createStack(Stack_t *thou, size_t capacity)
{
    if (thou != nullptr)
    {
        thou->capacity = capacity;
        thou->data = (int *) calloc(capacity, sizeof(int));
        if (thou->data == nullptr)
        {
            return STACK_DATA_NOT_EMPTY;
        }
        thou->sizeStack = 0;

        return STACK_NO_ERROR;
    }
    else
    {
        return STACK_NOT_EXIST;
    }
}

int clearStack(Stack_t *thou)
{
    int stackStatus = checkExistenceStack(thou);
    if (stackStatus == STACK_NO_ERROR)
    {
        thou->sizeStack = 0;

        return STACK_NO_ERROR;
    }
    else
    {
        if (stackStatus == STACK_NOT_EXIST)
        {
            return STACK_NOT_EXIST;
        }
        else
        {
            return STACK_DATA_NOT_EMPTY;
        }
    }
}

int push(Stack_t *thou, int element)
{
    int stackStatus = checkExistenceStack(thou);
    if (stackStatus == STACK_NO_ERROR)
    {
        if (thou->sizeStack >= thou->capacity)
        {
            thou->capacity = (thou->capacity + 1) * STACK_MEMORY_RESIZE;
            thou->data = (int *) realloc(thou->data, thou->capacity * sizeof(int));
            if (thou->data == nullptr)
            {
                return STACK_DATA_NOT_EMPTY;
            }
        }

        thou->data[thou->sizeStack] = element;
        thou->sizeStack++;

        return STACK_NO_ERROR;
    }
    else
    {
        if (stackStatus == STACK_NOT_EXIST)
        {
            return STACK_NOT_EXIST;
        }
        else
        {
            return STACK_DATA_NOT_EMPTY;
        }
    }
}

int pop(Stack_t *thou, int *top)
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
        if (stackStatus == STACK_NOT_EXIST)
        {
            return STACK_NOT_EXIST;
        }
        else
        {
            return STACK_DATA_NOT_EMPTY;
        }
    }
}

int dump(Stack_t *thou)
{
    int stackStatus = checkExistenceStack(thou);
    if (stackStatus == STACK_NO_ERROR)
    {
        for (int i = 0; i < thou->sizeStack; i++)
        {
            printf("%d ", thou->data[i]);
        }

        printf("\n");

        return STACK_NO_ERROR;
    }
    else
    {
        if (stackStatus == STACK_NOT_EXIST)
        {
            return STACK_NOT_EXIST;
        }
        else
        {
            return STACK_DATA_NOT_EMPTY;
        }
    }
}
