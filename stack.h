#ifndef STACK_H
#define STACK_H

#define DEBUG
//#undef DEBUG

typedef int stackData_t;

enum StackStatus
{
    STACK_NOT_CONSTRUCTED,
    STACK_CONSTRUCTED,
    STACK_DESTROYED
};

struct stack_t
{
    #ifdef DEBUG
        unsigned long long int leftStackCanary;
    #endif // DEBUG

    size_t capacity;
    stackData_t *data;
    size_t size;
    StackStatus status;

    #ifdef DEBUG
        const char *nameStack;
        const char *filePath;
        size_t line;
        unsigned long long int rightStackCanary;
        unsigned long long int hashStack;
        unsigned long long int hashData;
    #endif // DEBUG
};

enum StackErrorCode
{
    STACK_NO_ERROR,
    STACK_NOT_EXIST,
    STACK_ALREADY_CONSTRUCTED,
    STACK_DATA_DESTROY,
    STACK_POP_FROM_EMPTY,
    STACK_DATA_CALLOC_ERROR,
    STACK_DATA_REALLOC_ERROR,
    STACK_CAPACITY_LESS_SIZE,

    #ifdef DEBUG
        LEFT_STACK_CANARY_DIED,
        RIGHT_STACK_CANARY_DIED,
        STACK_NO_NAME,
        HASH_STACK_DAMAGED,
        HASH_DATA_DAMAGED,
        LEFT_DATA_CANARY_DIED,
        RIGHT_DATA_CANARY_DIED
    #endif // DEBUG
};

void clearOutputFile();

StackErrorCode GetStackError(stack_t *stack);

#ifdef DEBUG
void dump(stack_t *stack, const char *nameFunction, size_t line, const char *filePath);
#endif // DEBUG

#ifdef DEBUG
    #define ASSERT_OK(stack)                                      \
        do{                                                       \
        dump(stack, __PRETTY_FUNCTION__, __LINE__, __FILE__);     \
        assert(GetStackError(stack) == STACK_NO_ERROR);           \
        }while(false);
#else
    #define ASSERT_OK(stack)                                      \

#endif //DEBUG

//{-------------------------------------------------------------------

StackErrorCode StackCtor(stack_t *stack, size_t capacity);

#ifdef DEBUG
    StackErrorCode StackCtorForDebug(stack_t *stack, size_t capacity, const char *nameStack, size_t line, const char* filePath);
#endif // DEBUG

#ifdef DEBUG
    #define STACKCTOR_(stack, capacity)                                         \
        StackCtorForDebug(stack, capacity, #stack, __LINE__, __FILE__)
#else
    #define STACKCTOR_(stack, capacity)                                         \
        StackCtor(stack, capacity)
#endif // DEBUG

//}-------------------------------------------------------------------

StackErrorCode StackDtor(stack_t *stack);

StackErrorCode StackPush(stack_t *stack, int element);

StackErrorCode StackPop(stack_t *stack, int *top);

size_t GetStackCapacity(stack_t *stack);

size_t GetStackSize(stack_t *stack);

#endif
