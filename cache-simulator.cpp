#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <ctime>

struct Set {
    int value = 0;
    int* addr = nullptr;
    int timeAcess = 0;
};

struct Block {
    int numSet = 0;
    Set* sets = nullptr;
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

        if (*cacheValue == memoryValue && i != 0) {
            numHits++;
        } else {
            *cacheValue = memoryValue;
            numMisses++;
        }
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

Block* sortCache (Block* cache, int numBlocks, int numSets) {
    for (int i = 0; i < numBlocks; i++) {
        for (int j = 0; j < numSets; j++) {
            for (int k = 0; k < numSets - 1; k++) {
                if (cache[i].sets[k].timeAcess < cache[i].sets[k + 1].timeAcess) {
                    Set aux = cache[i].sets[k];
                    cache[i].sets[k] = cache[i].sets[k + 1];
                    cache[i].sets[k + 1] = aux;
                }
            }
        }
    }
    return cache;
}

bool isCacheValue (Block* cache, int blockIndex, int numSets, int memoryValue) {
    for (int i = 0; i < numSets; i++) {
        if (cache[blockIndex].sets[i].value == memoryValue) {
            cache[blockIndex].sets[i].timeAcess = time(0);
            return true;
        }
    }
    return false;
}

void simulateCacheFullyAssociative(int memorySize, int numBlocks, int numSets) {
    int* memory = loadFileToMemory(memorySize);
    Block* cache = createCache(numBlocks, numSets);

    for (int i = 0; i < numBlocks; i++) 
        for (int j = 0; j < numSets; j++) {
            cache[i].sets[j].timeAcess = time(0);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

    int numHits = 0;
    int numMisses = 0;

    for (int i = 0; i < memorySize; i++) {
        int memoryValue = memory[i];
        int blockIndex = memoryValue % numBlocks;
        std::cout << memoryValue     << " ";
        
        if (isCacheValue(cache, blockIndex, numSets, memoryValue) && i != 0) {
            std::cout << "H ";
            numHits++;
        } else {
            cache[blockIndex].sets[numSets - 1].value = memoryValue;
            cache[blockIndex].sets[numSets - 1].timeAcess = time(NULL);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "M ";
            numMisses++;
        }

        for (int i = 0; i < numBlocks; i++) {
            for (int j = 0; j < numSets; j++) {
                std::cout << cache[i].sets[j].value << " ";
            }
        }

        std::cout << "\n";
        
        cache = sortCache(cache, numBlocks, numSets);
    }
    
    std::cout << "Fully Associative Cache" << "\n";
    std::cout << "Hits: " << numHits << "\n";
    std::cout << "Misses: " << numMisses << "\n";;

    for (int i = 0; i < numBlocks; i++)
        delete[] cache[i].sets;
    delete[] cache;
    delete[] memory;
}

void simulateCacheNWay (int memorySize, int numBlocks, int numSets) {
    int* memory = loadFileToMemory(memorySize);
    Block* cache = createCache(numBlocks, numSets);

    for (int i = 0; i < numBlocks; i++) 
        for (int j = 0; j < numSets; j++) {
            cache[i].sets[j].timeAcess = time(0);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }

    int numHits = 0;
    int numMisses = 0;

    for (int i = 0; i < memorySize; i++) {
        int memoryValue = memory[i];
        int blockIndex = memoryValue % numBlocks;
        std::cout << memoryValue     << " ";
        
        if (isCacheValue(cache, blockIndex, numSets, memoryValue) && i != 0) {
            std::cout << "H ";
            numHits++;
        } else {
            cache[blockIndex].sets[numSets - 1].value = memoryValue;
            cache[blockIndex].sets[numSets - 1].timeAcess = time(NULL);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            std::cout << "M ";
            numMisses++;
        }

        for (int i = 0; i < numBlocks; i++) {
            for (int j = 0; j < numSets; j++) {
                std::cout << cache[i].sets[j].value << " ";
            }
        }

        std::cout << "\n";
        
        cache = sortCache(cache, numBlocks, numSets);
    }
    
    std::cout << "Fully Associative Cache" << "\n";
    std::cout << "Hits: " << numHits << "\n";
    std::cout << "Misses: " << numMisses << "\n";;

    for (int i = 0; i < numBlocks; i++)
        delete[] cache[i].sets;
    delete[] cache;
    delete[] memory;
}

int main() {
    simulateCacheFullyAssociative(13, 1, 4);

    return 0;
}