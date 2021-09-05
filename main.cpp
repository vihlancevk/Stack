#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Stack_t
{
    int maxElements;
    int *ptrStartStack;
    int numberElement;
};

const int MULTIPLIER = 2;

enum ERROR_CODES
{
    STACK_IS_EMPTY = 1
};

void creatStack(Stack_t *stack_t, int maxElement)
{
    assert(stack_t != nullptr);

    (*stack_t).maxElements = maxElement;
    (*stack_t).ptrStartStack = (int *) calloc(maxElement, sizeof(int));
    (*stack_t).numberElement = 0;
}

void clearStract(Stack_t *stack_t)
{
    assert(stack_t != nullptr);

    (*stack_t).maxElements = 0;
    (*stack_t).numberElement = 0;
    free((*stack_t).ptrStartStack);
}

void push(Stack_t *stack_t, int element)
{
    assert(stack_t != nullptr);

    if ((*stack_t).numberElement < (*stack_t).maxElements)
    {
        (*stack_t).ptrStartStack[(*stack_t).numberElement] = element;
        (*stack_t).numberElement++;
    }
    else
    {
        (*stack_t).maxElements = ((*stack_t).maxElements + 1) * MULTIPLIER;
        (*stack_t).ptrStartStack = (int *) realloc((*stack_t).ptrStartStack, (*stack_t).maxElements * sizeof(int));
        (*stack_t).ptrStartStack[(*stack_t).numberElement] = element;
        (*stack_t).numberElement++;
    }
}

int pop(Stack_t *stack_t)
{
    assert(stack_t != nullptr);

    if ((*stack_t).numberElement > 0)
    {
        int buffer = (*stack_t).ptrStartStack[(*stack_t).numberElement - 1];
        (*stack_t).numberElement--;
        return buffer;
    }
    else
    {
        printf("\nStack is empty!\n");
        return STACK_IS_EMPTY;
    }
}

void outputStack(Stack_t *stack_t)
{
    assert(stack_t != nullptr);

    for (int i = 0; i < (*stack_t).numberElement; i++)
    {
        printf("%d\n", (*stack_t).ptrStartStack[i]);
    }

    printf("\n\n");
}

int main()
{
    Stack_t stack_t;

    creatStack(&stack_t, 10);

    for (int i = 0; i < 20; i++)
    {
        push(&stack_t, i);
    }

    outputStack(&stack_t);

    for (int i = 0; i < 23; i++)
    {
        printf("%d ", pop(&stack_t));
    }

    clearStract(&stack_t);

    return 0;
}
