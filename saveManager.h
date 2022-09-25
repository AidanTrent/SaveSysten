#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H
#include <stdint.h>

typedef struct Node{
	struct Node* next;
	uint16_t dfBytes;
	uint8_t data[];
} Node;

typedef struct{
	Node* head;
	Node* cur;
} LList;

// Make a list containing one node with given params
LList* makeList(uint16_t dfBytes, uint8_t* data);

// Add a node of given params to top of list
Node* push(LList* list, uint16_t dfBytes, uint8_t* data);

void freeList(LList* list);

// Remove node from list. Returns 1 on failure, 0 success
int freeNode(LList* list, Node* node);

// Saves an EntityList as a binary file. Returns 1 on failure, 0 success
int saveList(LList* list, char saveName[]);

// Loads a saved EntityList from a file
LList* loadSave(char saveName[]);
#endif
