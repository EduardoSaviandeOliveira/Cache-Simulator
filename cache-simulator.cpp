#include <iostream>
#include <fstream>

struct Set
{
    int value = 0;
    int *addr = nullptr;
    int timeAcess = 0;
    bool dirtyBit = false;
};

struct Block
{
    Set *sets = nullptr;
};

void printCache(Block *cache, int numBlocks, int numSets, int memorySize, int numHits)
{
    std::cout << "Cache ";

    if (numBlocks == 1)
        std::cout << "direct mapping" << std::endl;
    else if (numSets == 1)
        std::cout << "fully associative" << std::endl;
    else
        std::cout << "set associative" << std::endl;

    std::cout << "\nBlocos\t\tSets\n";

    int numDirtyBit = 0;

    for (int i = 0; i < numBlocks; i++)
    {
        std::cout << i << "\t\t";
        for (int j = 0; j < numSets; j++)
        {
            std::cout << cache[i].sets[j].value << "    ";
            if (cache[i].sets[j].dirtyBit)
                numDirtyBit++;
        }
        std::cout << "\n";
    }

    std::cout << "\nMemory size:" << memorySize << "\n";
    std::cout << "Hits: " << numHits << "\n";
    std::cout << "Misses: " << memorySize - numHits << "\n"; // num of misses
    std::cout << "Hit Rate: " << (double)numHits / (memorySize)*100 << "%\n";
    std::cout << "Dirty Bits: " << numDirtyBit << "\n\n";
}

bool powerOfTwo(int x)
{
    return x * (!(x & (x - 1)));
}

int countFile()
{
    std::ifstream file("data.txt");
    int count = 0;
    int temp;
    while (file >> temp)
        count++;
    file.close();
    return count;
}

int *freshMemory(int size)
{
    int *array = new int[size];
    for (int i = 0; i < size; i++)
        array[i] = 0;
    return array;
}

int *loadFileToMemory(int memorySize)
{
    int *memory = freshMemory(memorySize);
    FILE *file = fopen("data.txt", "r");

    if (file == NULL)
    {
        std::cerr << "Unable to open the file" << std::endl;
        return memory;
    }

    for (int i = 0; i < memorySize; i++)
        fscanf(file, "%d", &memory[i]);

    fclose(file);
    return memory;
}

Block *createCache(int numBlocks, int numSets)
{
    Block *cache = new Block[numBlocks];
    for (int i = 0; i < numBlocks; i++)
        cache[i].sets = new Set[numSets];
    return cache;
}

Block *sortCache(Block *cache, int numBlocks, int numSets)
{
    for (int i = 0; i < numBlocks; i++)
    {
        for (int j = 0; j < numSets; j++)
        {
            for (int k = 0; k < numSets - 1; k++)
            {
                if (cache[i].sets[k].timeAcess > cache[i].sets[k + 1].timeAcess)
                {
                    Set temp = cache[i].sets[k];
                    cache[i].sets[k] = cache[i].sets[k + 1];
                    cache[i].sets[k + 1] = temp;
                }
            }
        }
    }
    return cache;
}

bool isCacheValue(Block *cache, int blockIndex, int numSets, int memoryValue, int timeAcess)
{
    for (int i = 0; i < numSets; i++)
    {
        if (cache[blockIndex].sets[i].value == memoryValue)
        {
            cache[blockIndex].sets[i].timeAcess = timeAcess;
            return true;
        }
    }
    return false;
}

void simulateCache(int numBlocks, int numSets)
{
    int memorySize = countFile();
    int *memory = loadFileToMemory(memorySize);
    Block *cache = createCache(numBlocks, numSets);

    int numHits = 0;
    int timeAcess = 0;

    for (int i = 0; i < memorySize; i++)
    {
        int memoryValue = memory[i];
        int blockIndex = memoryValue % numBlocks;
        int *cacheValue = &cache[blockIndex].sets[0].value;

        if (isCacheValue(cache, blockIndex, numSets, memoryValue, timeAcess) && i != 0)
        {
            numHits++;
        }
        else
        {
            cache[blockIndex].sets[0].value = memoryValue;
            cache[blockIndex].sets[0].addr = &memory[i];
            cache[blockIndex].sets[0].timeAcess = timeAcess;
            cache[blockIndex].sets[0].dirtyBit = true;
        }

        cache = sortCache(cache, numBlocks, numSets);
        timeAcess++;
    }

    printCache(cache, numBlocks, numSets, memorySize, numHits);

    for (int i = 0; i < numBlocks; i++)
        delete[] cache[i].sets;
    delete[] cache;
    delete[] memory;
}

void Simulator()
{
    int numBlocks = 0;
    int numSets = 0;

    while (true)
    {
        std::cout << "Enter the number of blocks: ";
        std::cin >> numBlocks;
        if (powerOfTwo(numBlocks))
            break;
        std::cout << "The number of sets must be a power of two\n";
    }
    while (true)
    {
        std::cout << "Enter the number of sets: ";
        std::cin >> numSets;
        if (powerOfTwo(numSets))
            break;
        std::cout << "The number of sets must be a power of two\n";
    }
    simulateCache(numBlocks, numSets);
}

int main()
{
    int menu = 0;
    while (menu != 2)
    {
        std::cout << "1 - Simulate Cache\n2 - Exit\nOption: ";
        std::cin >> menu;
        switch (menu)
        {
        case 1:
            Simulator();
            break;
        default:
            if (menu != 2)
                std::cout << "Invalid option\n";
            break;
        }
    }

    return 0;
}