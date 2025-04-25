#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Represents a block of memory
struct Block {
    string process_id; // "free" if not allocated
    int start;
    int size;

    Block(string id, int s, int sz) : process_id(id), start(s), size(sz) {}
};

class MemoryAllocator {
private:
    vector<Block> memory;
    int total_size;

public:
    MemoryAllocator(int size) {
        total_size = size;
        memory.push_back(Block("free", 0, size)); // initially, all memory is free
    }

    void printMemoryStatus() {
        cout << "\nCurrent memory layout:\n";
        for (const auto& block : memory) {
            cout << "[" << block.start << " - " << (block.start + block.size - 1) << "] "
                 << (block.process_id == "free" ? "Free" : "Process " + block.process_id)
                 << " (" << block.size << ")\n";
        }
        cout << endl;
    }

    void addProcess(const string& id, int size) {
        for (size_t i = 0; i < memory.size(); ++i) {
            if (memory[i].process_id == "free" && memory[i].size >= size) {
                int start = memory[i].start;

                // Split block if there's extra space
                if (memory[i].size > size) {
                    memory.insert(memory.begin() + i + 1,
                                  Block("free", start + size, memory[i].size - size));
                }

                memory[i] = Block(id, start, size);
                cout << "Process " << id << " added.\n";
                return;
            }
        }

        cout << "Not enough space for process " << id << " (external fragmentation may exist).\n";
    }

    void removeProcess(const string& id) {
        bool found = false;
        for (auto& block : memory) {
            if (block.process_id == id) {
                block.process_id = "free";
                found = true;
                cout << "Process " << id << " removed.\n";
            }
        }
        if (!found)
            cout << "Process " << id << " not found.\n";

        mergeFreeBlocks(); // Clean up after deletion
    }

    void compactMemory() {
        vector<Block> new_memory;
        int current_start = 0;

        // Move all processes to the front
        for (const auto& block : memory) {
            if (block.process_id != "free") {
                new_memory.push_back(Block(block.process_id, current_start, block.size));
                current_start += block.size;
            }
        }

        // Add one big free block at the end
        if (current_start < total_size) {
            new_memory.push_back(Block("free", current_start, total_size - current_start));
        }

        memory = new_memory;
        cout << "Memory compacted.\n";
    }

private:
    void mergeFreeBlocks() {
        vector<Block> new_memory;
        for (size_t i = 0; i < memory.size(); ++i) {
            if (!new_memory.empty() &&
                new_memory.back().process_id == "free" &&
                memory[i].process_id == "free") {
                new_memory.back().size += memory[i].size;
            } else {
                new_memory.push_back(memory[i]);
            }
        }
        memory = new_memory;
    }
};

int main() {
    MemoryAllocator allocator(1000); // Total memory size

    cout << "Memory Allocator CLI\n";
    cout << "Commands: add <id> <size>, remove <id>, stat, compact, exit\n";

    string command;
    while (true) {
        cout << "> ";
        cin >> command;

        if (command == "add") {
            string id;
            int size;
            cin >> id >> size;
            allocator.addProcess(id, size);
        } else if (command == "remove") {
            string id;
            cin >> id;
            allocator.removeProcess(id);
        } else if (command == "stat") {
            allocator.printMemoryStatus();
        } else if (command == "compact") {
            allocator.compactMemory();
        } else if (command == "exit") {
            break;
        } else {
            cout << "Invalid command.\n";
        }
    }

    return 0;
}
