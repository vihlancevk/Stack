#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stack.h"

int CheckExistenceStack(Stack_t *Stack)
{
    if (Stack == nullptr)
    {
        printf("STACK_NOT_EXIST!\n");
        return STACK_NOT_EXIST;
    }

    return NO_ERROR;
}

int createStack(Stack_t *Stack, size_t maxElement)
{
    if (CheckExistenceStack(Stack) == STACK_NOT_EXIST)
    {
        return STACK_NOT_EXIST;
    }
    else
    {
        Stack->capacity = maxElement;
        Stack->date = (int *) calloc(maxElement, sizeof(int));
        Stack->sizeStack = 0;

        return NO_ERROR;
    }
}
int clearStack(Stack_t *Stack)
{
    if (CheckExistenceStack(Stack) == STACK_NOT_EXIST)
    {
        return STACK_NOT_EXIST;
    }

    Stack->sizeStack = 0;

    return NO_ERROR;
}

int push(Stack_t *Stack, int element)
{
    if (CheckExistenceStack(Stack) == STACK_NOT_EXIST)
    {
        return STACK_NOT_EXIST;
    }

    if (Stack->sizeStack < Stack->capacity)
    {
        Stack->date[Stack->sizeStack] = element;
        Stack->sizeStack++;
    }
    else
    {
        Stack->capacity = (Stack->capacity + 1) * MEMORY_MULTIPLIER;
        Stack->date = (int *) realloc(Stack->date, Stack->capacity * sizeof(int));
        Stack->date[Stack->sizeStack] = element;
        Stack->sizeStack++;
    }

    return NO_ERROR;
}

int pop(Stack_t *Stack)
{
    if (CheckExistenceStack(Stack) == STACK_NOT_EXIST)
    {
        return STACK_NOT_EXIST;
    }

    if (Stack->sizeStack > 0)
    {
        int top = Stack->date[Stack->sizeStack - 1];
        Stack->sizeStack--;
        return top;
    }
    else
    {
        printf("\nSTACK_IS_EMPTY!\n");
        return STACK_IS_EMPTY;
    }
}

int outputStack(Stack_t *Stack)
{
    if (CheckExistenceStack(Stack) == STACK_NOT_EXIST)
    {
        return STACK_NOT_EXIST;
    }

    for (int i = 0; i < Stack->sizeStack; i++)
    {
        printf("%d ", Stack->date[i]);
    }

    printf("\n");

    return NO_ERROR;
}
