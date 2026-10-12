#include "MemoryPool.h"

#include <cstdint>

MemoryPool::MemoryPool(size_t blockSize, size_t blockCount)
{
    blockSize_ = blockSize;
    blockCount_ = blockCount;

    memory_ = nullptr;
    allocated_ = nullptr;

    if (blockSize_ == 0 || blockCount_ == 0)
    {
        return;
    }

    memory_ = new unsigned char[blockSize_ * blockCount_];
    allocated_ = new bool[blockCount_];

    for (size_t i = 0; i < blockCount_; i++)
    {
        allocated_[i] = false;

        void* blockAddress = memory_ + (i * blockSize_);
        freeBlocks_.push(blockAddress);
    }
}

MemoryPool::~MemoryPool()
{
    delete[] allocated_;
    delete[] memory_;
}

void* MemoryPool::allocate()
{
    if (freeBlocks_.empty())
    {
        return nullptr;
    }

    void* block = freeBlocks_.pop();

    size_t index = getBlockIndex(block);
    allocated_[index] = true;

    return block;
}

bool MemoryPool::deallocate(void* ptr)
{
    if (!isValidBlock(ptr))
    {
        return false;
    }

    size_t index = getBlockIndex(ptr);

    if (!allocated_[index])
    {
        return false;
    }

    allocated_[index] = false;
    freeBlocks_.push(ptr);

    return true;
}

size_t MemoryPool::availableBlocks() const
{
    return freeBlocks_.size();
}

size_t MemoryPool::allocatedBlocks() const
{
    return blockCount_ - freeBlocks_.size();
}

size_t MemoryPool::blockSize() const
{
    return blockSize_;
}

size_t MemoryPool::capacity() const
{
    return blockSize_ * blockCount_;
}

bool MemoryPool::isValidBlock(void* ptr) const
{
    if (ptr == nullptr || memory_ == nullptr || blockSize_ == 0)
    {
        return false;
    }

    std::uintptr_t start =
        reinterpret_cast<std::uintptr_t>(memory_);

    std::uintptr_t end =
        start + (blockSize_ * blockCount_);

    std::uintptr_t address =
        reinterpret_cast<std::uintptr_t>(ptr);

    if (address < start || address >= end)
    {
        return false;
    }

    std::uintptr_t offset = address - start;

    if (offset % blockSize_ != 0)
    {
        return false;
    }

    return true;
}

size_t MemoryPool::getBlockIndex(void* ptr) const
{
    std::uintptr_t start =
        reinterpret_cast<std::uintptr_t>(memory_);

    std::uintptr_t address =
        reinterpret_cast<std::uintptr_t>(ptr);

    std::uintptr_t offset = address - start;

    return offset / blockSize_;
}