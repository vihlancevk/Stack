#include <stdio.h>
#include "stack.h"

int main()
{
    stack_t stack = {};
    int stackStatus = STACKCTOR_(&stack, 10, "stack");
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    for (int i = 0; i < 15; i++)
    {
        if (StackPush(&stack, i) != STACK_NO_ERROR)
        {
            return stackStatus;
        }
    }

    int top = 0;
    for (int i = 0; i < 10000; i++)
    {
        stackStatus = StackPop(&stack, &top);

        if (stackStatus != STACK_NO_ERROR)
        {
            return stackStatus;
        }
    }

    return stackStatus;
}
