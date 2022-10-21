#include <iostream>
#include <fstream>
#include <ctime>

struct Set {
    int numSet = 0;
    int value = 0;
    int* addr = nullptr;
    int timeAcess = 0;
};

struct Block {
    Set* sets = nullptr;
};

void printCache(Block* cache, int numSets, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) 
        for (int j = 0; j < numSets; j++) 
            std::cout << cache[i].sets[j].value << " ";
    std::cout << std::endl;
}

int* freshMemory(int size) {
	int* array = new int[size];
	for (int i = 0; i < size; i++) array[i] = 0;
	return array;
}

int* loadFileToMemory(int memorySize) {
    int* memory = freshMemory(memorySize);
	FILE* file = fopen("memory.txt", "r");

	if (file == NULL) {
		std::cerr << "Unable to open the file" << std::endl;
		return memory;
	}

	for (int i = 0; i < memorySize; i++) 
        fscanf(file, "%d", &memory[i]);
	
	fclose(file);
    return memory;
}

Block* createCache(int numBlocks, int numSets) {
    Block* cache = new Block[numBlocks];
    for (int i = 0; i < numBlocks; i++)
        cache[i].sets = new Set[numSets];
    return cache;
}

void simulateCacheDirectMapped(int memorySize, int numBlocks, int numSets) {
    int* memory = loadFileToMemory(memorySize);
    Block* cache = createCache(numBlocks, numSets);

    int numHits = 0;
    int numMisses = 0;

    for (int i = 0; i < numBlocks; i++) 
        std::cout << i << " ";
    std::cout << std::endl << std::endl;
    
    for (int i = 0; i < memorySize; i++) {
        int memoryValue = memory[i];
        int blockIndex = memoryValue % numBlocks;
        int setIndex = numSets - 1;
        int* cacheValue = &cache[blockIndex].sets[setIndex].value;

        cache[blockIndex].sets[setIndex].timeAcess = time(NULL);

        if (*cacheValue == memoryValue && i != 0) {
            numHits++;
        } else {
            *cacheValue = memoryValue;
            numMisses++;
        }

        printCache(cache, numSets, numBlocks);
    }

    std::cout << std::endl;
    std::cout << "Direct Mapped Cache" << std::endl;
    std::cout << "Hits: " << numHits << std::endl;
    std::cout << "Misses: " << numMisses << std::endl;

    for (int i = 0; i < numBlocks; i++)
        delete[] cache[i].sets;
    delete[] cache;
    delete[] memory;
}

void simulateCacheFullyAssociative(int memorySize, int numBlocks, int numSets) {
    int* memory = loadFileToMemory(memorySize);
    Block* cache = createCache(numBlocks, numSets);

    int numHits = 0;
    int numMisses = 0;

    for (int i = 0; i < numBlocks; i++) 
        std::cout << i << " ";
    std::cout << std::endl << std::endl;
    
    for (int i = 0; i < memorySize; i++) {
        int memoryValue = memory[i];
        int blockIndex = memoryValue % numBlocks;
        int setIndex = numSets - 1;
        int* cacheValue = &cache[blockIndex].sets[setIndex].value;

        cache[blockIndex].sets[setIndex].timeAcess = time(NULL);

        if (*cacheValue == memoryValue && i != 0) {
            numHits++;
        } else {
            *cacheValue = memoryValue;
            numMisses++;
        }

        printCache(cache, numSets, numBlocks);
    }

    std::cout << std::endl;
    std::cout << "Direct Mapped Cache" << std::endl;
    std::cout << "Hits: " << numHits << std::endl;
    std::cout << "Misses: " << numMisses << std::endl;

    for (int i = 0; i < numBlocks; i++)
        delete[] cache[i].sets;
    delete[] cache;
    delete[] memory;

}

int main() {
    simulateCacheFullyAssociative(5, 1, 4);

    return 0;
}