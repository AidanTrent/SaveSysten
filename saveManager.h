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
	Node* tail;
} LList;

// Make a list containing one node with given params
LList* makeList(uint16_t dfBytes, uint8_t* data);

// Insert a Node of given params before the head of list
Node* insertHead(LList* list, uint16_t dfBytes, uint8_t* data);

// Insert a Node of given params after the tail of list
Node* insertTail(LList* list, uint16_t dfBytes, uint8_t* data);

void freeList(LList* list);

// Remove node from list. Returns 1 when nothing is freed, 0 on success
int freeNode(LList* list, Node* node);

// Saves an EntityList as a binary file. Returns 1 on failure, 0 on success
int saveList(LList* list, char saveName[]);

// Loads a saved EntityList from a file
LList* loadSave(char saveName[]);
#endif
