#include <iostream>
#include <fstream>

struct Block {
    int nset = 0;
};

struct CacheEntry {
    int* value = nullptr;
};

void printStatus(Block* cache, int memorySize, int numBlocks, int numSets, int numHits, int numMisses){
    std::cout << "Memory Size: " << memorySize << std::endl;
    std::cout << "Number of Blocks: " << numBlocks << std::endl;
    std::cout << "Number of Sets: " << numSets << std::endl;
    std::cout << "Number of Hits: " << numHits << std::endl;
    std::cout << "Number of Misses: " << numMisses << std::endl;
    std::cout << "Hit Ratio: " << (float)numHits / (float)(numHits + numMisses) << std::endl;
    std::cout << "Miss Ratio: " << (float)numMisses / (float)(numHits + numMisses) << std::endl;
    std::cout << "Cache: " << std::endl;
    for (int i = 0; i < numBlocks; i++) {
        std::cout << cache[i].nset << " ";
    }
    std::cout << std::endl;
}

bool powerOfTwo(int x){
	return x * (!(x & (x - 1)));
}

int* freshMemory(int size){
	int* array = new int[size];
	for(int i = 0; i < size; i++) array[i] = 0;
	return array;
}

int* loadFileToMemory(int memorySize){
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

    return cache;
}

void simulateCacheDirectMapped(int memorySize, int numBlocks, int numSets) {
    int* memory = loadFileToMemory(memorySize);
    Block* cache = createCache(numBlocks, numSets);

    int numHits = 0;
    int numMisses = 0;

    printStatus(cache, memorySize, numBlocks, numSets, numHits, numMisses);
    
    delete[] cache;
    delete[] memory;
}

int main() {
     simulateCacheDirectMapped(5, 4, 1);
                                                                                                                                                                                                                                                                                                                             
    return 0;
}
