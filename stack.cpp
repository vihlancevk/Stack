#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <typeinfo>
#include <cxxabi.h>
#include "stack.h"

#define DEBUG
//#undef DEBUG

#ifdef DEBUG
    const uint64_t STACK_LEFT_CANARY  = *(uint64_t*)"B\xA\xD\xD\xE\xD\x3\x2";
    const uint64_t STACK_RIGHT_CANARY = *(uint64_t*)"D\xE\xD\xB\xA\xD\x2\x3";
    const uint64_t DATA_LEFT_CANARY = *(uint64_t*)"1\xD\xC\xB\xA\xD\x1\x1";
    const uint64_t DATA_RIGHT_CANARY = *(uint64_t*)"2\xD\xC\xD\xA\xB\x2\x2";
#endif // DEBUG

const size_t STACK_MEMORY_EXPAND = 2;
const size_t STACK_MEMORY_SHRINK = 3;
const char *NAME_OUTPUT_FILE = "output.txt";
const void *const ERR_PTR = (void*)(1000-7);
const void *const ERR_CALLOC_PTR = (void*)(300);
const void *const ERR_REALLOC_PTR = (void*)(666);
const stackData_t RECALLOC_VALUE = 1488;

#ifdef DEBUG
    FILE *foutput = nullptr;

    void ClearOutputFile()
    {
        foutput = fopen(NAME_OUTPUT_FILE, "w");
        fclose(foutput);
    }
#endif // DEBUG

static uint64_t HashRot13(const void *elem, size_t size)
{
    assert(elem != nullptr);

    uint64_t hash = 0;

    char *buffer = (char*)elem;

    for(size_t i = 0; i < size; i++)
    {
        hash += (uint64_t)buffer[i];
        hash -= (hash << 13) | (hash >> 19);
    }

    return hash;
}

//{-------------------------------------------------------------------

#ifdef DEBUG
    static void StackAndDataHashing(stack_t *stack)
    {
        assert(stack != nullptr);

        stack->hashStack = HashRot13((void*)stack, sizeof((*stack)) - sizeof(stack->hashStack) - sizeof(stack->hashData));
        if(stack->data != nullptr)
        {
            stack->hashData = HashRot13((void*)stack->data, sizeof(*(stack->data)) * stack->capacity);
        }
    }
#endif // DEBUG

#ifdef DEBUG
    #define STACK_AND_DATA_HASHING_(stack) \
        StackAndDataHashing(stack)
#else
    #define STACK_AND_DATA_HASHING_(stack) \

#endif // DEBUG

//}-------------------------------------------------------------------

StackErrorCode GetStackError(stack_t *stack)
{
    if (stack == nullptr)
    {
        return STACK_NOT_EXIST;
    }

    #ifdef DEBUG
        if (stack->leftStackCanary != (STACK_LEFT_CANARY ^ (size_t)stack))
        {
            return STACK_LEFT_CANARY_DIED;
        }

        if (stack->rightStackCanary != (STACK_RIGHT_CANARY ^ (size_t)stack))
        {
            return STACK_RIGHT_CANARY_DIED;
        }

        if (stack->nameStack == nullptr)
        {
            return STACK_NO_NAME;
        }

        if (stack->hashStack != HashRot13(stack, sizeof((*stack)) - sizeof(stack->hashStack) - sizeof(stack->hashData)))
        {
            return STACK_HASH_DAMAGED;
        }

        if (stack->hashData != HashRot13(stack->data, sizeof(*(stack->data)) * stack->capacity))
        {
            return DATA_HASH_DAMAGED;
        }

        if (*(uint64_t*)(stack->data) != DATA_LEFT_CANARY)
        {
            return DATA_LEFT_CANARY_DIED;
        }

        if (*(uint64_t*)((char*)(stack->data) + sizeof(uint64_t) + sizeof(stackData_t) * stack->capacity) != DATA_RIGHT_CANARY)
        {
            return DATA_RIGHT_CANARY_DIED;
        }

    #endif // DEBUG

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

//{-------------------------------------------------------------------

#define GET_DESCR_(code)  case (code): { return #code; }

static const char *StackErrorToString(StackErrorCode error)
{
    switch(error)
    {
        GET_DESCR_(STACK_NO_ERROR)
        GET_DESCR_(STACK_NOT_EXIST)
        GET_DESCR_(STACK_ALREADY_CONSTRUCTED)

        #ifdef DEBUG
            GET_DESCR_(STACK_LEFT_CANARY_DIED)
            GET_DESCR_(STACK_RIGHT_CANARY_DIED)
            GET_DESCR_(STACK_NO_NAME)
            GET_DESCR_(STACK_HASH_DAMAGED)
            GET_DESCR_(DATA_HASH_DAMAGED)
            GET_DESCR_(DATA_LEFT_CANARY_DIED)
            GET_DESCR_(DATA_RIGHT_CANARY_DIED)
        #endif // DEBUG

        GET_DESCR_(STACK_DATA_DESTROY)
        GET_DESCR_(STACK_POP_FROM_EMPTY)
        GET_DESCR_(STACK_DATA_CALLOC_ERROR)
        GET_DESCR_(STACK_DATA_REALLOC_ERROR)
        GET_DESCR_(STACK_CAPACITY_LESS_SIZE)

        default:{return nullptr;}
    }
}

static const char *StackStatusToString(StackStatus status)
{
    switch(status)
    {
        GET_DESCR_(STACK_NOT_CONSTRUCTED)
        GET_DESCR_(STACK_CONSTRUCTED)
        GET_DESCR_(STACK_DESTROYED)
        default:{return nullptr;}
    }
}

//}-------------------------------------------------------------------

//{-------------------------------------------------------------------

StackErrorCode StackCtor(stack_t *stack, size_t capacity)
{
    assert(stack != nullptr);

    if (stack->status != STACK_NOT_CONSTRUCTED)
    {
        return STACK_ALREADY_CONSTRUCTED;
    }

    stack->data = (stackData_t *) calloc(1, sizeof(stackData_t) * capacity + 2 * sizeof(uint64_t));
    if (stack->data == nullptr)
    {
        return STACK_DATA_CALLOC_ERROR;
    }
    #ifdef DEBUG
        *(uint64_t*)(stack->data) = DATA_LEFT_CANARY;
        *(uint64_t*)((char*)(stack->data) + sizeof(uint64_t) + sizeof(stackData_t) * capacity) = DATA_RIGHT_CANARY;
    #endif // DEBUG

    stack->capacity = capacity;
    stack->size = 0;
    stack->status = STACK_CONSTRUCTED;
    STACK_AND_DATA_HASHING_(stack);

    ASSERT_OK(stack);
    return STACK_NO_ERROR;
}

#ifdef DEBUG
    StackErrorCode StackCtorForDebug(stack_t *stack, size_t capacity, const char *nameStack, size_t line, const char* filePath)
    {
        assert(stack != nullptr);

        stack->nameStack = nameStack;
        stack->leftStackCanary = STACK_LEFT_CANARY ^ (size_t)stack;
        stack->rightStackCanary = STACK_RIGHT_CANARY ^ (size_t)stack;
        stack->filePath = filePath;
        stack->line = line;

        return StackCtor(stack, capacity);
    }
#endif // DEBUG

//}-------------------------------------------------------------------

//{-------------------------------------------------------------------

static StackErrorCode ReallocStack(stack_t *stack, size_t newCapacity)
{
    ASSERT_OK(stack);

    assert(newCapacity > 0);

    StackErrorCode stackStatus = GetStackError(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    #ifdef DEBUG
        stackData_t *data = (stackData_t *)realloc(stack->data, newCapacity * sizeof(stackData_t) + 2 * sizeof(uint64_t));
        if (data == nullptr)
        {
            return STACK_DATA_REALLOC_ERROR;
        }
    #else
        stackData_t *data = (stackData_t *)realloc(stack->data, newCapacity * sizeof(stackData_t));
        if (data == nullptr)
        {
            return STACK_DATA_REALLOC_ERROR;
        }
    #endif // DEBUG

    stack->data = data;
    stack->capacity = newCapacity;
    #ifdef DEBUG
        *(uint64_t*)(stack->data) = DATA_LEFT_CANARY;
        *(uint64_t*)((char*)stack->data + sizeof(uint64_t) + sizeof(stackData_t) * stack->capacity) = DATA_RIGHT_CANARY;
    #endif // DEBUG
    STACK_AND_DATA_HASHING_(stack);

    ASSERT_OK(stack);
    return stackStatus;
}

//}-------------------------------------------------------------------

#ifdef DEBUG
    static StackErrorCode RecallocStack(stack_t *stack, size_t newCapacity)
    {
        ASSERT_OK(stack);

        StackErrorCode status = ReallocStack(stack, newCapacity);

        for (size_t i = stack->size; i < stack->capacity; i++)
        {
          ((stackData_t*)((char*)(stack->data) + sizeof(uint64_t)))[i] = RECALLOC_VALUE;
        }

        STACK_AND_DATA_HASHING_(stack);

        ASSERT_OK(stack);
        return status;
    }
#endif // DEBUG

#ifdef DEBUG
    #define RECALLOC_STACK_(stack, newCapacity)        \
        RecallocStack(stack, newCapacity)
#else
    #define RECALLOC_STACK_(satck, newCapacity)        \
        ReallocStack(stack, newCapacity)
#endif // DEBUG

//}-------------------------------------------------------------------

StackErrorCode StackDtor(stack_t *stack)
{
    ASSERT_OK(stack);

    StackErrorCode stackStatus = GetStackError(stack);
    if (stackStatus == STACK_NO_ERROR)
    {
        free(stack->data);
        stack->size = 0;
        stack->data = (stackData_t*)ERR_PTR;
        stack->capacity = 0;
        stack->status = STACK_DESTROYED;
    }

    STACK_AND_DATA_HASHING_(stack);

    ASSERT_OK(stack);
    return stackStatus;
}

StackErrorCode StackPush(stack_t *stack, stackData_t element)
{
    ASSERT_OK(stack);

    StackErrorCode stackStatus = GetStackError(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        ASSERT_OK(stack);
        return stackStatus;
    }

    if (stack->size >= stack->capacity)
    {
        size_t newCapacity = (stack->capacity) * STACK_MEMORY_EXPAND;
        stackStatus = RECALLOC_STACK_(stack, newCapacity);
        if (stackStatus != STACK_NO_ERROR)
        {
            ASSERT_OK(stack);
            return stackStatus;
        }
    }

    ((stackData_t*)((char*)stack->data + sizeof(uint64_t)))[stack->size] = element;
    stack->size++;
    STACK_AND_DATA_HASHING_(stack);

    ASSERT_OK(stack);
    return stackStatus;
}

StackErrorCode StackPop(stack_t *stack, stackData_t *top)
{
    ASSERT_OK(stack);

    assert(top != nullptr);

    StackErrorCode stackStatus = GetStackError(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        ASSERT_OK(stack);
        return stackStatus;
    }

    if (stack->size > 0)
    {
        *top = ((stackData_t*)((char*)stack->data + sizeof(uint64_t)))[stack->size - 1];
        stack->size--;
        STACK_AND_DATA_HASHING_(stack);

        if (stack->size != 0 && (stack->capacity / STACK_MEMORY_SHRINK >= stack->size))
        {
            size_t newCapacity = (stack->capacity) / STACK_MEMORY_SHRINK;
            stackStatus = RECALLOC_STACK_(stack, newCapacity);
            ASSERT_OK(stack);
            return stackStatus;
        }

        ASSERT_OK(stack);
        return STACK_NO_ERROR;
    }
    else
    {
        STACK_AND_DATA_HASHING_(stack);

        ASSERT_OK(stack);
        return STACK_POP_FROM_EMPTY;
    }
}

size_t GetStackCapacity(stack_t *stack)
{
    ASSERT_OK(stack);

    int stackStatus = GetStackError(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    return stack->capacity;
}

size_t GetStackSize(stack_t *stack)
{
    ASSERT_OK(stack);

    int stackStatus = GetStackError(stack);
    if (stackStatus != STACK_NO_ERROR)
    {
        return stackStatus;
    }

    return stack->size;
}

//{-------------------------------------------------------------------

#ifdef DEBUG
    void Dump(stack_t *stack, const char *nameFunction, size_t line, const char *filePath)
    {
        assert(filePath != nullptr);

        foutput = fopen(NAME_OUTPUT_FILE, "a");

        StackErrorCode stackStatus = GetStackError(stack);

        int status = 0;
        char *type = abi::__cxa_demangle(typeid(stack->data[0]).name(), 0, 0, &status);
        if (status != 0)
        {
            printf("Error with __cxa_demangle() in dump()\n");
        }

        fprintf(foutput, "================================================================================\n");
        fprintf(foutput, "Stack ");
        fprintf(foutput, "<%s> ", type);
        fprintf(foutput, "[%p] ", stack);
        fprintf(foutput, "[%s] ", stack->nameStack);
        fprintf(foutput, "at %s(%zu) ", stack->filePath, stack->line);
        fprintf(foutput, "called from %s at %s(%zu)\n{\n", nameFunction, filePath, line);
        fprintf(foutput, "\t%s\n", StackErrorToString(stackStatus));
        fprintf(foutput, "\t%s\n\t{\n", StackStatusToString(stack->status));
        fprintf(foutput, "\t\tsize = %zu\n", stack->size);
        fprintf(foutput, "\t\tcapacity = %zu\n", stack->capacity);
        fprintf(foutput, "\t\tdata[%p]\n", stack->data);
        fprintf(foutput, "\t\t{\n");
        fprintf(foutput, "\t\t\tDATA_LEFT_CANARY = %lu\n", *(uint64_t*)stack->data);
        for (size_t i = 0; i < stack->capacity; i++)
        {
            fprintf(foutput, "\t\t\t(%s) [%zu] - %d\n", type, i,
            ((stackData_t*)((char*)stack->data + sizeof(uint64_t)))[i]);
        }
        fprintf(foutput, "\t\t\tDATA_RIGHT_CANARY = %lu\n",
        *(uint64_t*)((char*)(stack->data) + sizeof(uint64_t) + sizeof(stackData_t) * stack->capacity));

        fprintf(foutput, "\t\t}\n");
        fprintf(foutput, "\t}\n");
        fprintf(foutput, "}\n");

        fprintf(foutput, "--------------------------------------------------------------------------------\n");

        fclose(foutput);
    }
#endif // DEBUG

//}-------------------------------------------------------------------
