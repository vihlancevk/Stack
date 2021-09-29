#ifndef STACK_H
#define STACK_H

#define DEBUG
#undef DEBUG

typedef int stackData_t;

enum StackStatus
{
    STACK_NOT_CONSTRUCTED,
    STACK_CONSTRUCTED,
    STACK_DESTROYED
};

struct stack_t
{
    size_t capacity;
    stackData_t *data;
    size_t size;
    const char *nameStack;
    StackStatus status;
    #ifdef DEBUG
        size_t line;
        const char *filePath;
    #endif // DEBUG
};

enum StackErrorCode
{
    STACK_NO_ERROR,
    STACK_NOT_EXIST,
    STACK_ALREADY_CONSTRUCTED,
    STACK_NO_NAME,
    STACK_DATA_DESTROY,
    STACK_POP_FROM_EMPTY,
    STACK_DATA_CALLOC_ERROR,
    STACK_DATA_REALLOC_ERROR,
    STACK_CAPACITY_LESS_SIZE
};

StackErrorCode GetStackStatus(stack_t *stack);

#ifdef DEBUG
void dump(stack_t *stack, const char *nameFunction, size_t line, const char *filePath);
#endif // DEBUG

#ifdef DEBUG
    #define ASSERT_OK(stack, nameFunction)                        \
        do{                                                       \
        dump(stack, nameFunction, __LINE__, __FILE__);            \
        assert(GetStackStatus(stack) == STACK_NO_ERROR);          \
        }while(false);
#else
    #define ASSERT_OK(stack, nameFunction)                        \

#endif //DEBUG

StackErrorCode StackCtor(stack_t *stack, size_t capacity, const char *nameStack);

#ifdef DEBUG
    StackErrorCode StackCtorForDebug(stack_t *stack, size_t capacity, const char *nameStack, size_t line, const char* filePath);
#endif // DEBUG

#ifdef DEBUG
    #define STACKCTOR_(stack, capacity, nameStack)                             \
        StackCtorForDebug(stack, capacity, nameStack, __LINE__, __FILE__)
#else
    #define STACKCTOR_(stack, capacity, nameStack)                             \
        StackCtor(stack, capacity, nameStack)
#endif // DEBUG

StackErrorCode StackDtor(stack_t *stack);

StackErrorCode StackPush(stack_t *stack, int element);

StackErrorCode StackPop(stack_t *stack, int *top);

size_t GetStackCapacity(stack_t *stack);

size_t GetStackSize(stack_t *stack);

#endif
