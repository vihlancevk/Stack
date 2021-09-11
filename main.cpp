#include <stdio.h>
#include "stack.h"

int main()
{
    FILE *foutput = fopen("output.txt", "w");

    Stack_t stack = {0, nullptr, 0};
    int stackStatus = createStack(&stack, 10, foutput);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    for (int i = 0; i < 15; i++)
    {
        if ((stackStatus = push(&stack, i, foutput)) != STACK_NO_ERROR)
        {
            return stackStatus;
        }
    }

    int top = 0;
    for (int i = 0; i < 10000; i++)
    {
        stackStatus = pop(&stack, &top, foutput);

        if (stackStatus != STACK_NO_ERROR)
        {
            return stackStatus;
        }
    }

    fclose(foutput);
    return stackStatus;
}
