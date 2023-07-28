#include <stdio.h>
#include <stdlib.h> // Add this header for malloc and exit functions
#define HASH_SIZE 100
// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
};

// Fill out this structure
struct HashType
{
    struct RecordType* record;
};

// Compute the hash function
int hash(int x)
{
    // Using a simple modulo operation for hashing
    // You can use a more sophisticated hash function for better distribution
    return x % HASH_SIZE; // HASH_SIZE is the size of the hash table
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}


// Your hash implementation
 // Choose an appropriate size for the hash table

void insertRecordIntoHash(struct HashType* hashTable, int hashSize, struct RecordType record)
{
    int index = hash(record.id);

    // Separate Chaining - Handle collisions by using linked lists at each index
    struct RecordType* newRecord = (struct RecordType*)malloc(sizeof(struct RecordType));
    if (newRecord == NULL)
    {
        printf("Cannot allocate memory for the new record\n");
        exit(-1);
    }
    *newRecord = record;
    hashTable[index].record = newRecord;
}

void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    int i;

    for (i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].record != NULL)
        {
            printf("index %d -> %d, %c, %d\n", i, pHashArray[i].record->id, pHashArray[i].record->name, pHashArray[i].record->order);
        }
    }
}

int main(void)
{
    struct RecordType* pRecords;
    int recordSz = 0;
    int i;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Your hash implementation
    struct HashType* pHashTable = (struct HashType*)malloc(sizeof(struct HashType) * HASH_SIZE);
    if (pHashTable == NULL)
    {
        printf("Cannot allocate memory for the hash table\n");
        exit(-1);
    }

    // Initialize the hash table
    for (i = 0; i < HASH_SIZE; ++i)
    {
        pHashTable[i].record = NULL;
    }

    // Insert records into the hash table
    for (i = 0; i < recordSz; ++i)
    {
        insertRecordIntoHash(pHashTable, HASH_SIZE, pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(pHashTable, HASH_SIZE);

    // Free allocated memory
    free(pRecords);

    for (i = 0; i < HASH_SIZE; ++i)
    {
        if (pHashTable[i].record != NULL)
        {
            free(pHashTable[i].record);
        }
    }
    free(pHashTable);

    return 0;
}