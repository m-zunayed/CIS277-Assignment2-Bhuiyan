#include "PacketQueue.h"

#include <cstring>

PacketQueue::PacketQueue(std::size_t blockSize, std::size_t blockCount)
    : pool_(blockSize, blockCount),
      queue_(blockCount)
{
}

bool PacketQueue::receive(std::size_t id,
                          const unsigned char* bytes,
                          std::size_t length)
{
    if (bytes == nullptr || length == 0)
    {
        return false;
    }

    if (length > pool_.blockSize())
    {
        return false;
    }

    if (queue_.full())
    {
        return false;
    }

    void* block = pool_.allocate();

    if (block == nullptr)
    {
        return false;
    }

    std::memcpy(block, bytes, length);

    PacketDescriptor packet;
    packet.id = id;
    packet.block = block;
    packet.length = length;

    if (!queue_.enqueue(packet))
    {
        pool_.deallocate(block);
        return false;
    }

    return true;
}

bool PacketQueue::processNext(std::size_t& id,
                              std::vector<unsigned char>& bytes)
{
    PacketDescriptor packet;

    if (!queue_.dequeue(packet))
    {
        return false;
    }

    unsigned char* data =
        static_cast<unsigned char*>(packet.block);

    std::vector<unsigned char> copiedBytes(
        data,
        data + packet.length
    );

    id = packet.id;
    bytes = copiedBytes;

    pool_.deallocate(packet.block);

    return true;
}

std::size_t PacketQueue::pending() const
{
    return queue_.size();
}

std::size_t PacketQueue::availableBlocks() const
{
    return pool_.availableBlocks();
}