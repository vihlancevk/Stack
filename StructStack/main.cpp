#include "stack.h"

const char *INPUT_FILE  = "input.txt";
const char *OUTPUT_FILE = "output.txt";

int main()
{
    CLEAR_LOG_FILE();

    stack_t stack = {};
    int stackError = STACKCTOR_(&stack, 10);
    IS_STACK_ERROR_(&stack, stackError);

    IS_STACK_ERROR_(&stack, StackPush(&stack, 10));

    return stackError;
}
