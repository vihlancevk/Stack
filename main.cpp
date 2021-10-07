#include "stack.h"
#include "fileOperations.h"

const char *INPUT_FILE  = "input.txt";
const char *OUTPUT_FILE = "output.txt";

StackErrorCode CalculatingExpression(stack_t *stack, const char *nameInputFile, FILE *foutput);

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

StackErrorCode CalculatingExpression(stack_t *stack, const char *nameInputFile, FILE *foutput)
{
    ASSERT_OK(stack);

    StackErrorCode stackError = GetStackError(stack);
    IS_STACK_ERROR_(stack, stackError);
    assert(nameInputFile != nullptr);
    assert(foutput != nullptr);

    int linesCount = 0;
    char *str = nullptr;
    Line *lines = (Line *) fillStructLine(nameInputFile, &linesCount, str);

    CodeOperation codeOperation = PUSH;
    stackData_t elem = 0;
    for (int i = 0; i < linesCount; i++)
    {
        sscanf(lines[i].str, "%d %d", &codeOperation, &elem);
        if (codeOperation == PUSH)
        {
            stackError = StackPush(stack, elem);
            IS_STACK_ERROR_(stack, stackError);
        }
        else
        {
            stackError = StackElemOperation(stack, codeOperation);
            IS_STACK_ERROR_(stack, stackError);
        }
    }

    stackError = writeFile(foutput, lines, linesCount, stack);
    if (stackError != STACK_NO_ERROR)
    {
        return stackError;
    }

    return stackError;
}
