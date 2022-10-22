#include <iostream>
#include <fstream>

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

int countFile() {
    std::ifstream file("data.txt");
    int count = 0;
    int temp;
    while (file >> temp) count++;
    file.close();
    return count;
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

bool isCacheValue (Block* cache, int blockIndex, int numSets, int memoryValue, int setNumber) {
    for (int i = 0; i < numSets; i++) {
        if (cache[blockIndex].sets[i].value == memoryValue) {
            cache[blockIndex].sets[i].timeAcess = setNumber;
            return true;
        }
    }
    return false;   
}

void printCache(Block* cache, int numBlocks, int numSets, int memorySize, int numHits) {
    std::cout << "Cache ";
    
    if (numBlocks == 1)
        std::cout << "direct mapping" << std::endl;
    else if (numSets == 1)
        std::cout << "fully associative" << std::endl;
    else
        std::cout << "set associative" << std::endl;
        
    std::cout << "\nBlocos\t\tSets" << "\n";

    for (int i = 0; i < numBlocks; i++) {
        std::cout << i << "\t\t";
        for (int j = 0; j < numSets; j++) {
            std::cout << cache[i].sets[j].value << "    ";
        }
        std::cout << "\n";
    }

    std::cout << "\nMemory size:" << memorySize << "\n";
    std::cout << "Hits: " << numHits << "\n";
    std::cout << "Misses: " << memorySize -  numHits << "\n"; // num of misses
    std::cout << "Hit Rate: " << (double)numHits / (memorySize) * 100 << "%\n";
}

void simulateCache(int numBlocks, int numSets) {
    int memorySize = countFile();
    int* memory = loadFileToMemory(memorySize);
    Block* cache = createCache(numBlocks, numSets);
    
    int numHits = 0;
    int setNumber = 0;

    for (int i = 0; i < memorySize; i++) {
        int memoryValue = memory[i];
        int blockIndex = memoryValue % numBlocks;
        int* cacheValue = &cache[blockIndex].sets[0].value;

        if (isCacheValue(cache, blockIndex, numSets, memoryValue, setNumber)  && i != 0) {
            numHits++;
        } else {
            cache[blockIndex].sets[0].value = memoryValue;
            cache[blockIndex].sets[0].addr = &memory[i];
            cache[blockIndex].sets[0].timeAcess = setNumber;
        }

        cache = sortCache(cache, numBlocks, numSets);
        setNumber++;
    }

    printCache(cache, numBlocks, numSets, memorySize, numHits);

    for (int i = 0; i < numBlocks; i++)
        delete[] cache[i].sets;
    delete[] cache;
    delete[] memory;
}

int main() {
    int numBlocks = 16;
    int numSets = 2;

    // while (true) {
    //     std::cout << "Enter the number of blocks: ";
    //     std::cin >> numBlocks;
    //     if (powerOfTwo(numBlocks)) break;
    // }

    // while (true) {
    //     std::cout << "Enter the number of sets: ";
    //     std::cin >> numSets;
    //     if (powerOfTwo(numSets)) break;
    // }

    simulateCache(numBlocks, numSets);

    return 0;
}