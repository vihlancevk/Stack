#include "stack.h"
#include "fileOperations.h"

const char *INPUT_FILE = "input.txt";
const char *OUTPUT_FILE = "output.txt";

int main()
{
    CLEAR_LOG_FILE();

    stack_t stack = {};
    int stackError = STACKCTOR_(&stack, 1);
    IS_STACK_ERROR_(&stack, stackError);

    FILE *foutput = fopen(OUTPUT_FILE, "w");

    stackError = CalculatingExpression(&stack, INPUT_FILE, foutput);

    fclose(foutput);

    return stackError;
}
