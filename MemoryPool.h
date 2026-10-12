#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <cstddef>
#include "Stack.h"

class MemoryPool
{
private:
    size_t blockSize_;
    size_t blockCount_;

    unsigned char* memory_;
    bool* allocated_;

    Stack<void*> freeBlocks_;

    bool isValidBlock(void* ptr) const;
    size_t getBlockIndex(void* ptr) const;

public:
    MemoryPool(size_t blockSize, size_t blockCount);
    ~MemoryPool();

    void* allocate();
    bool deallocate(void* ptr);

    size_t availableBlocks() const;
    size_t allocatedBlocks() const;
    size_t blockSize() const;
    size_t capacity() const;
};

#endif