#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <thread>
#include <tuple>

struct Set {
    int value = 0;
    int* addr = nullptr;
    int timeAcess = 0;
};

struct Block {
    Set* sets = nullptr;
};

bool powerOfTwo(int x){
    return x * (!(x & (x - 1)));
}

int* freshMemory(int size) {
	int* array = new int[size];
	for (int i = 0; i < size; i++) array[i] = 0;
	return array;
}

int* loadFileToMemory(int memorySize) {
    int* memory = freshMemory(memorySize);
	FILE* file = fopen("data.txt", "r");

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

    for (int i = 0; i < numBlocks; i++) {
        for (int j = 0; j < numSets; j++) {
            cache[i].sets[j].timeAcess = time(NULL);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }
    return cache;
}

Block* sortCache (Block* cache, int numBlocks, int numSets) {
    for (int i = 0; i < numBlocks; i++) {
        for (int j = 0; j < numSets; j++) {
            for (int k = 0; k < numSets - 1; k++) {
                if (cache[i].sets[k].timeAcess > cache[i].sets[k + 1].timeAcess) {
                    Set temp = cache[i].sets[k];
                    cache[i].sets[k] = cache[i].sets[k + 1];
                    cache[i].sets[k + 1] = temp;
                }
            }
        }
    }
    return cache;
}

bool isCacheValue (Block* cache, int blockIndex, int numSets, int memoryValue) {
    for (int i = 0; i < numSets; i++) {
        if (cache[blockIndex].sets[i].value == memoryValue) {
            cache[blockIndex].sets[i].timeAcess = time(NULL);
            return true;
        }
    }
    return false;   
}

void simulateCache(int memorySize, int numBlocks, int numSets) {
    int* memory = loadFileToMemory(memorySize);
    Block* cache = createCache(numBlocks, numSets);

    if (numBlocks == 1)
        std::cout << "Direct mapping" << std::endl;
    else if (numSets == 1)
        std::cout << "Fully associative" << std::endl;
    else
        std::cout << "Set associative" << std::endl;
    
    int numHits = 0;
    int numMisses = 0;

    for (int i = 0; i < memorySize; i++) {
        int memoryValue = memory[i];
        int blockIndex = memoryValue % numBlocks;
        int* cacheValue = &cache[blockIndex].sets[0].value;

        std::cout << memoryValue << " ";

        if (isCacheValue(cache, blockIndex, numSets, memoryValue)  && i != 0) {
            numHits++;
            std::cout << "H ";
        } else {
            cache[blockIndex].sets[0].value = memoryValue;
            cache[blockIndex].sets[0].addr = &memory[i];
            cache[blockIndex].sets[0].timeAcess = time(NULL);
            numMisses++;
            std::cout << "M ";
        }

        cache = sortCache(cache, numBlocks, numSets);

        for (int i = 0; i < numBlocks; i++) {
            for (int j = 0; j < numSets; j++) {
                std::cout << cache[i].sets[j].value << " ";
            }
        }

        std::cout << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    std::cout << "Hits: " << numHits << "\n";
    std::cout << "Misses: " << numMisses << "\n";;

    for (int i = 0; i < numBlocks; i++)
        delete[] cache[i].sets;
    delete[] cache;
    delete[] memory;
}

int main() {
    int memorySize = 0;
    int numBlocks = 0;
    int numSets = 0;

    while (true) {
        std::cout << "Enter the size of memory: ";
        std::cin >> memorySize;
        if (memorySize > 0) break;
    }
    while (true) {
        std::cout << "Enter the number of blocks: ";
        std::cin >> numBlocks;
        if (powerOfTwo(numBlocks)) break;
    }

    while (true) {
        std::cout << "Enter the number of sets: ";
        std::cin >> numSets;
        if (powerOfTwo(numSets)) break;
    }

    simulateCache(memorySize, numBlocks, numSets);

    return 0;
}