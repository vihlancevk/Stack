#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <typeinfo>
#include "stack.h"

const size_t STACK_MEMORY_EXPAND = 2;
const size_t STACK_MEMORY_SHRINK = 3;
const void* const ERR_PTR = (void*)(1000-7);
const void* const ERR_CALLOC_PTR = (void*)(300);
const void* const ERR_REALLOC_PTR = (void*)(666);

FILE *foutput = nullptr;

StackErrorCode GetStackStatus(stack_t *stack)
{
    if (stack == nullptr)
    {
        return STACK_NOT_EXIST;
    }

    if (stack->nameStack == nullptr)
    {
        return STACK_NO_NAME;
    }

    if (stack->data == ERR_PTR)
    {
        return STACK_DATA_DESTROY;
    }

    if (stack->data == ERR_CALLOC_PTR)
    {
        return STACK_DATA_CALLOC_ERROR;
    }

    if (stack->data == ERR_REALLOC_PTR)
    {
        return STACK_DATA_REALLOC_ERROR;
    }

    if (stack->capacity < stack->size)
    {
        return STACK_CAPACITY_LESS_SIZE;
    }

    return STACK_NO_ERROR;
}

static const char *StackErrorToString(StackErrorCode error)
{
    switch(error)
    {
        case STACK_NO_ERROR:           {return "STACK_NO_ERROR";          }
        case STACK_NOT_EXIST:          {return "STACK_NOT_EXIST";         }
        case STACK_DATA_DESTROY:       {return "STACK_DATA_DESTROY";      }
        case STACK_POP_FROM_EMPTY:     {return "STACK_POP_FROM_EMPTY";    }
        case STACK_DATA_CALLOC_ERROR:  {return "STACK_DATA_CALLOC_ERROR"; }
        case STACK_DATA_REALLOC_ERROR: {return "STACK_DATA_REALLOC_ERROR";}
        case STACK_CAPACITY_LESS_SIZE: {return "STACK_CAPACITY_LESS_SIZE";}
        default:                       {return nullptr;                   }
    }
}

static const char *StackStatusToString(StackStatus status)
{
    switch(status)
    {
        case STACK_NOT_CONSTRUCTED: {return "STACK_NOT_CONSTRUCTED";}
        case STACK_CONSTRUCTED:     {return "STACK_CONSTRUCTED";    }
        case STACK_DESTROYED:       {return "STACK_DESTROYED";      }
        default:                    {return nullptr;                }
    }
}

StackErrorCode StackCtor(stack_t *stack, size_t capacity, const char *nameStack = nullptr)
{
    ASSERT_OK(stack, "StackCtor");

    StackErrorCode stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    if (stack->status != STACK_NOT_CONSTRUCTED)
    {
        return STACK_ALREADY_CONSTRUCTED;
    }

    stack->data = (stackData_t *) calloc(capacity, sizeof(stackData_t));
    if (stack->data == nullptr)
    {
        return STACK_DATA_CALLOC_ERROR;
    }

    stack->capacity = capacity;
    stack->nameStack = nameStack;
    stack->size = 0;
    stack->status = STACK_CONSTRUCTED;

    ASSERT_OK(stack, "StackCtor");
    return stackStatus;
}

#ifdef DEBUG
    StackErrorCode StackCtorForDebug(stack_t *stack, size_t capacity, const char *nameStack, size_t line, const char* filePath)
    {
        assert(filePath != 0);

        stack->line = line;
        stack->filePath = filePath;

        return StackCtor(stack, capacity, nameStack);
    }
#endif // DEBUG

static StackErrorCode reallocStack(stack_t *stack, size_t newCapacity)
{
    ASSERT_OK(stack, "reallocStack");

    assert(newCapacity > 0);

    StackErrorCode stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    stackData_t *data = (stackData_t *)realloc(stack->data, newCapacity * sizeof(stackData_t));
    if (data == nullptr)
    {
        return STACK_DATA_REALLOC_ERROR;
    }

    stack->data = data;
    stack->capacity = newCapacity;

    ASSERT_OK(stack, "reallocStack");
    return stackStatus;
}

StackErrorCode StackDtor(stack_t *stack)
{
    ASSERT_OK(stack, "StackDtor");

    StackErrorCode stackStatus = GetStackStatus(stack);
    if (stackStatus == STACK_NO_ERROR)
    {
        free(stack->data);
        stack->size = 0;
        stack->data = (stackData_t*)ERR_PTR;
        stack->capacity = 0;
        stack->status = STACK_DESTROYED;
    }

    ASSERT_OK(stack, "StackDtor");
    return stackStatus;
}

StackErrorCode StackPush(stack_t *stack, stackData_t element)
{
    ASSERT_OK(stack, "StackPush");

    StackErrorCode stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        ASSERT_OK(stack, "StackPush");
        return stackStatus;
    }

    if (stack->size >= stack->capacity)
    {
        size_t newCapacity = (stack->capacity) * STACK_MEMORY_EXPAND;
        stackStatus = reallocStack(stack, newCapacity);
        if (stackStatus != STACK_NO_ERROR)
        {
            ASSERT_OK(stack, "StackPush");
            return stackStatus;
        }
    }

    stack->data[stack->size] = element;
    stack->size++;

    ASSERT_OK(stack, "StackPush");
    return stackStatus;
}

StackErrorCode StackPop(stack_t *stack, stackData_t *top)
{
    ASSERT_OK(stack, "StackPop");

    assert(top != nullptr);

    StackErrorCode stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        ASSERT_OK(stack, "StackPop");
        return stackStatus;
    }

    if (stack->size > 0)
    {
        *top = stack->data[stack->size - 1];
        stack->size--;

        if (stack->size != 0 && (stack->capacity / STACK_MEMORY_SHRINK > stack->size))
        {
            size_t newCapacity = (stack->capacity) / STACK_MEMORY_SHRINK;
            stackStatus = reallocStack(stack, newCapacity);
            ASSERT_OK(stack, "StackPop");
            return stackStatus;
        }

        ASSERT_OK(stack, "StackPop");
        return STACK_NO_ERROR;
    }
    else
    {
        ASSERT_OK(stack, "StackPop");
        return STACK_POP_FROM_EMPTY;
    }
}

size_t getStackCapacity(stack_t *stack)
{
    int stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    return stack->capacity;
}

size_t getStackSize(stack_t *stack)
{
    int stackStatus = GetStackStatus(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    return stack->size;
}

#ifdef DEBUG
    void dump(stack_t *stack, const char *nameFunction, size_t line, const char *filePath)
    {
        assert(stack != nullptr);

        foutput = fopen("output.txt", "a");

        StackErrorCode stackStatus = GetStackStatus(stack);

        const char *type = typeid(stack->data[0]).name();

        fprintf(foutput, "================================================================================\n");
        fprintf(foutput, "Stack ");
        fprintf(foutput, "<%s> ", type);
        fprintf(foutput, "[%p] ", stack);
        fprintf(foutput, "called from %s() at %s(%Iu) ", nameFunction, filePath, line);
        fprintf(foutput, "at %s(%Iu)\n{\n", stack->filePath, stack->line);
        fprintf(foutput, "\t%s\n", StackErrorToString(stackStatus));
        fprintf(foutput, "\t%s\n\t{\n", StackStatusToString(stack->status));
        fprintf(foutput, "\t\tsize = %Iu\n", stack->size);
        fprintf(foutput, "\t\tcapacity = %Iu\n", stack->capacity);
        fprintf(foutput, "\t\tdata[%p]\n", stack->data);
        fprintf(foutput, "\t\t{\n");

        for (size_t i = 0; i < stack->capacity; i++)
        {
            fprintf(foutput, "\t\t\t(%s) [%Iu] - %d\n", type, i, stack->data[i]);
        }

        fprintf(foutput, "\t\t}\n");

        fprintf(foutput, "--------------------------------------------------------------------------------\n");

        fclose(foutput);
    }
#endif // DEBUG
