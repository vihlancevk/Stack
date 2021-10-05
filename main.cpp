#include "stack.h"
#include "fileOperations.h"

const char *INPUT_FILE = "input.txt";
const char *OUTPUT_FILE = "output.txt";

int main()
{
    CLEAR_LOG_FILE();

    int linesCount = 0;
    char *str = nullptr;
    Line *lines = (Line *) fillStructLine(INPUT_FILE, &linesCount, str);

    stack_t stack = {};
    int stackError = STACKCTOR_(&stack, 10);
    IS_STACK_ERROR_(&stack, stackError);

    char stackOperation[20] = {};
    stackData_t elem = 0;
    for (int i = 0; i < linesCount; i++)
    {
        sscanf(lines[i].str, "%s %d", stackOperation, &elem);
        if (strcmp(stackOperation, "push") == 0)
        {
            stackError = StackPush(&stack, elem);
            IS_STACK_ERROR_(&stack, stackError);
        }
        else
        {
            stackError = StackElemOperation(&stack, (const char*)stackOperation);
            IS_STACK_ERROR_(&stack, stackError);
        }
    }

    FILE *foutput = fopen(OUTPUT_FILE, "w");
    assert(foutput != nullptr);

    stackError = StackPop(&stack, &elem);
    IS_STACK_ERROR_(&stack, stackError);
    printf("%d\n", elem);

    fclose(foutput);

    return stackError;
}
