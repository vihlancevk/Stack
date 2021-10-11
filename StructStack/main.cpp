#include "stack.h"

const char *INPUT_FILE  = "input.txt";
const char *OUTPUT_FILE = "output.txt";

int main()
{
    CLEAR_LOG_FILE();

    stack_t stack = {};
    int stackError = STACKCTOR_(&stack, 10);
    IS_STACK_ERROR_(&stack, STACKCTOR_(&stack, 10));

    IS_STACK_ERROR_(&stack, StackPush(&stack, 10));

    FILE *foutput = fopen(OUTPUT_FILE, "w");

    fclose(foutput);

    return stackError;
}
