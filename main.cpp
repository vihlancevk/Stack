#include <stdio.h>
#include "stack.h"

int main()
{
    Stack_t Stack = {0, nullptr, 0};
    if (createStack(&Stack, 10) == STACK_NOT_EXIST)
    {
        return 0;
    }

    createStack(&Stack, 10);

    for (int i = 0; i < 20; i++)
    {
        if (push(&Stack, i) == STACK_NOT_EXIST)
        {
            return 0;
        }
    }

    if (outputStack(&Stack) == STACK_NOT_EXIST)
    {
        return 0;
    }

    for (int i = 0; i < 23; i++)
    {
        int top = pop(&Stack);

        if ((top == STACK_NOT_EXIST && &Stack == nullptr) || (top == STACK_IS_EMPTY && Stack.sizeStack == 0))
        {
            return 0;
        }

        printf("%d ", top);
    }

    printf("\n");

    if (clearStack(&Stack) == STACK_NOT_EXIST)
    {
        return 0;
    }

    return 0;
}
