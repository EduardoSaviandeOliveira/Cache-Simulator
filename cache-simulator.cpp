#include <iostream>
#include <fstream>
#include <ctime>

struct CacheEntry {
    int numSet = 0;
    int timeAcess = 0;
    int* value = 0;
};

struct Block {
    CacheEntry* sets = nullptr;
};

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
        cache[i].sets = new CacheEntry[numSets];
    return cache;
}

void simulateCacheDirectMapped(int memorySize, int numBlocks, int numSets) {
    int* memory = loadFileToMemory(memorySize);
    Block* cache = createCache(numBlocks, numSets);

    int numHits = 0;
    int numMisses = 0;

    

    

    std::cout << "Direct Mapped Cache" << std::endl;
    std::cout << "Hits: " << numHits << std::endl;
    std::cout << "Misses: " << numMisses << std::endl;

    for (int i = 0; i < numBlocks; i++)
        delete[] cache[i].sets;
    delete[] cache;
    delete[] memory;
}

int main() {
    simulateCacheDirectMapped(5, 4, 1);    

    return 0;
}