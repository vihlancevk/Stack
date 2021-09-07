#include <stdio.h>
#include "stack.h"

int main()
{
    Stack_t thou = {0, nullptr, 0};
    int stackStatus = createStack(&thou, 10);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    for (int i = 0; i < 20; i++)
    {
        if ((stackStatus = push(&thou, i)) != STACK_NO_ERROR)
        {
            return stackStatus;
        }
    }

    if ((stackStatus = dump(&thou)) != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    int top = 0;
    for (int i = 0; i < 23; i++)
    {
        stackStatus = pop(&thou, &top);

        if (stackStatus != STACK_NO_ERROR)
        {
            return stackStatus;
        }

        printf("%d ", top);
    }

    printf("\n");

    if ((stackStatus = clearStack(&thou)) != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    return stackStatus;
}
