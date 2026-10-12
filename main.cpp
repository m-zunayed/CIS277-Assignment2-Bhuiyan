#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

#include "Queue.h"
#include "PacketQueue.h"

void printBytes(const std::vector<unsigned char>& bytes)
{
    for (unsigned char byte : bytes)
    {
        std::cout << "0x"
                  << std::hex
                  << std::uppercase
                  << std::setw(2)
                  << std::setfill('0')
                  << static_cast<int>(byte)
                  << " ";
    }

    std::cout << std::dec << "\n";
}

int main()
{
    std::cout << "Queue Tests\n";
    std::cout << "===========\n\n";

    Queue<int> numbers(3);

    std::cout << "Enqueue 10: "
              << (numbers.enqueue(10) ? "success" : "failed") << "\n";

    std::cout << "Enqueue 20: "
              << (numbers.enqueue(20) ? "success" : "failed") << "\n";

    std::cout << "Enqueue 30: "
              << (numbers.enqueue(30) ? "success" : "failed") << "\n";

    std::cout << "Queue full: "
              << (numbers.full() ? "yes" : "no") << "\n";

    int value = -1;

    if (numbers.front(value))
    {
        std::cout << "Front value: " << value << "\n";
    }

    std::cout << "Size after front: "
              << numbers.size() << "\n";

    if (numbers.dequeue(value))
    {
        std::cout << "Dequeued: " << value << "\n";
    }

    std::cout << "Enqueue 40 after dequeue: "
              << (numbers.enqueue(40) ? "success" : "failed") << "\n";

    std::cout << "Remaining order: ";

    while (numbers.dequeue(value))
    {
        std::cout << value << " ";
    }

    std::cout << "\n";

    std::cout << "Queue empty: "
              << (numbers.empty() ? "yes" : "no") << "\n\n";

    Queue<std::string> words(2);

    words.enqueue("alpha");
    words.enqueue("beta");

    std::string word;
    words.dequeue(word);
    words.enqueue("gamma");

    std::cout << "String queue order: ";

    while (words.dequeue(word))
    {
        std::cout << word << " ";
    }

    std::cout << "\n\n";

    std::cout << "PacketQueue Tests\n";
    std::cout << "=================\n\n";

    PacketQueue packets(4, 3);

    unsigned char packet1[] = {0x00, 0xFF, 0x12, 0x34};
    unsigned char packet2[] = {0x55};
    unsigned char packet3[] = {0x00, 0x80};
    unsigned char packet4[] = {0xAA, 0xBB, 0xCC, 0xDD};
    unsigned char tooLarge[] = {1, 2, 3, 4, 5};

    std::cout << "Initial pending: "
              << packets.pending() << "\n";

    std::cout << "Initial available blocks: "
              << packets.availableBlocks() << "\n\n";

    std::cout << "Receive packet 1: "
              << (packets.receive(1, packet1, sizeof(packet1))
                      ? "success"
                      : "failed")
              << "\n";

    std::cout << "Receive packet 2: "
              << (packets.receive(2, packet2, sizeof(packet2))
                      ? "success"
                      : "failed")
              << "\n";

    std::cout << "Receive packet 3: "
              << (packets.receive(3, packet3, sizeof(packet3))
                      ? "success"
                      : "failed")
              << "\n";

    std::cout << "Pending after three receives: "
              << packets.pending() << "\n";

    std::cout << "Available blocks: "
              << packets.availableBlocks() << "\n\n";

    std::size_t beforePending = packets.pending();
    std::size_t beforeAvailable = packets.availableBlocks();

    bool fullReceive =
        packets.receive(99, packet2, sizeof(packet2));

    std::cout << "Receive while full: "
              << (fullReceive ? "success" : "failed") << "\n";

    std::cout << "State unchanged: "
              << ((packets.pending() == beforePending &&
                   packets.availableBlocks() == beforeAvailable)
                      ? "yes"
                      : "no")
              << "\n\n";

    std::size_t id = 0;
    std::vector<unsigned char> bytes;

    if (packets.processNext(id, bytes))
    {
        std::cout << "Processed packet ID: "
                  << id << "\n";

        std::cout << "Packet bytes: ";
        printBytes(bytes);
    }

    std::cout << "Pending after processing one: "
              << packets.pending() << "\n";

    std::cout << "Available blocks after processing one: "
              << packets.availableBlocks() << "\n\n";

    std::cout << "Receive packet 4 after block release: "
              << (packets.receive(4, packet4, sizeof(packet4))
                      ? "success"
                      : "failed")
              << "\n";

    std::cout << "Pending: "
              << packets.pending() << "\n";

    std::cout << "Available blocks: "
              << packets.availableBlocks() << "\n\n";

    std::cout << "Remaining service order:\n";

    while (packets.processNext(id, bytes))
    {
        std::cout << "Packet " << id << ": ";
        printBytes(bytes);
    }

    std::cout << "\nFinal pending: "
              << packets.pending() << "\n";

    std::cout << "Final available blocks: "
              << packets.availableBlocks() << "\n\n";

    id = 888;
    bytes = {0x77};

    bool emptyProcess = packets.processNext(id, bytes);

    std::cout << "Process empty queue: "
              << (emptyProcess ? "success" : "failed") << "\n";

    std::cout << "ID unchanged: "
              << (id == 888 ? "yes" : "no") << "\n";

    std::cout << "Bytes unchanged: "
              << ((bytes.size() == 1 && bytes[0] == 0x77)
                      ? "yes"
                      : "no")
              << "\n\n";

    PacketQueue boundaryTest(4, 2);

    unsigned char oneByte[] = {0xAB};
    unsigned char fullBlock[] = {0x10, 0x20, 0x30, 0x40};

    std::cout << "Length 1 packet: "
              << (boundaryTest.receive(
                      10, oneByte, sizeof(oneByte))
                      ? "success"
                      : "failed")
              << "\n";

    boundaryTest.processNext(id, bytes);

    std::cout << "Exact block-size packet: "
              << (boundaryTest.receive(
                      11, fullBlock, sizeof(fullBlock))
                      ? "success"
                      : "failed")
              << "\n";

    boundaryTest.processNext(id, bytes);

    std::cout << "Oversize packet: "
              << (boundaryTest.receive(
                      12, tooLarge, sizeof(tooLarge))
                      ? "success"
                      : "failed")
              << "\n";

    std::cout << "Null pointer packet: "
              << (boundaryTest.receive(
                      13, nullptr, 1)
                      ? "success"
                      : "failed")
              << "\n";

    std::cout << "Zero length packet: "
              << (boundaryTest.receive(
                      14, oneByte, 0)
                      ? "success"
                      : "failed")
              << "\n";

    return 0;
}