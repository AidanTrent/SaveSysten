/* Aidan Trent
 * For a save management system. Linked list capable of being saved and loaded
 * off of storage.
 * Limited to 65,535 Kb nodes, easy as changing some data types if an increase
 * is needed though.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// TODO : Eventually plan on removing pragma pack(1)
#pragma pack(1)
typedef struct Node{
	struct Node* next;
	uint16_t dfBytes; // Number of bytes in data field
	uint8_t data[];
} Node;

typedef struct{
	Node* head;
	Node* tail;
} LList;

// Do not define in header, only used by other saveManager functions
Node* makeNode(uint16_t dfBytes, uint8_t* data){
	uint8_t nodeHeaderSize = (sizeof(struct Node*) + sizeof(uint16_t)); // Size of next + dfBytes
	Node* newNode = malloc(nodeHeaderSize + dfBytes);
	if (newNode == NULL){
		fprintf(stderr, "ERROR: malloc fail for newNode @ makeNode\n");
		return(NULL);
	}

	newNode->next = NULL;
	newNode->dfBytes = dfBytes;
	memcpy(&newNode->data, data, dfBytes);

	return newNode;
}

// Make a list containing one node with given params
LList* makeList(uint16_t dfBytes, uint8_t* data){
	LList* newList;
	newList = malloc(sizeof(struct Node*) * 2);
	if (newList == NULL){
		fprintf(stderr, "ERROR: malloc fail for newList @ makeList\n");
		return(NULL);
	}
	Node* newNode = makeNode(dfBytes, data);
	if (newNode == NULL){
		return NULL;
	}

	newList->head = newNode;
	newList->tail = newNode;

	return newList;
}

// Insert a Node of given params before the head of list
Node* insertHead(LList* list, uint16_t dfBytes, uint8_t* data){
	Node* oldHead = list->head;
	Node* newNode = makeNode(dfBytes, data);
	if (newNode == NULL){
		return(NULL);
	}

	list->head = newNode;
	newNode->next = oldHead;
	return newNode;
}

// Insert a Node of given params after the tail of list
Node* insertTail(LList* list, uint16_t dfBytes, uint8_t* data){
	Node* newNode = makeNode(dfBytes, data);
	if (newNode == NULL){
		return(NULL);
	}

	list->tail->next = newNode;
	list->tail = newNode;
	return newNode;
}

void freeList(LList* list){
	Node* temp;

	while (list->head != NULL){
		temp = list->head;
		list->head = list->head->next;
		free(temp);
	}
}

// Remove node from list. Returns 1 when nothing is freed, 0 on success
int freeNode(LList* list, Node* node){
	Node* cur = list->head; // Go to start of list

	// If removing head
	if (list->head == node){
		if (list->head->next == NULL){
			fprintf(stderr, "ERROR: Cannot free node when list has 1 node (do freeList) @ freeNode\n");
			return(1);
		}
		else{
			list->head = list->head->next;
			free(node);
			return(0);
		}
	}

	do{
		if (cur->next == node){
			// Check if patching is necessary
			if (cur->next->next != NULL){
				cur->next = cur->next->next; // Patch
			}
			else{
				cur->next = NULL;
			}
			free(node);
			return(0);
		}
		else {
			cur = cur->next; // Continue to next node
		}
	} while (cur->next != NULL);
	return(1); // node does not exist in list
}

// Saves an EntityList as a binary file. Returns 1 on failure, 0 on success
int saveList(LList* list, char saveName[]){
	FILE* saveFile = fopen(saveName, "wb");
	if (saveFile == NULL){
		fprintf(stderr, "ERROR: failed to save EntityList file @ saveList\n");
		return(1);
	}
	Node* cur = list->head; // Go to start of list

	uint8_t reading = 1;
	while (reading){
		fwrite(&cur->dfBytes, sizeof(cur->dfBytes), 1, saveFile);
		if (fwrite(cur->data, cur->dfBytes, 1, saveFile) != 1){
			fprintf(stderr, "ERROR: failed to write to saveFile file @ saveList\n");
			fclose(saveFile);
			return(1);
		}

		if (cur->next != NULL){
			cur = cur->next; // Continue to next node
		}
		else{
			reading = 0;
		}
	}

	fclose(saveFile);
	return(0);
}

// Loads a saved EntityList from a file
LList* loadSave(char saveName[]){
	LList* newList = NULL;

	FILE* saveFile = fopen(saveName, "rb");
	if (saveFile == NULL){
		fprintf(stderr, "ERROR: failed to load EntityList file @ loadSave\n");
		return(NULL);
	}

	uint16_t curDFBytes;
	uint8_t* curDataField = NULL;

	uint8_t reading = 1;
	while (reading){
		// Get number of bytes in this node
		if (fread(&curDFBytes, sizeof(curDFBytes), 1, saveFile) != 1){
			// Possibly at end of file
			reading = 0;
			if (newList == NULL){ // Check if file failed to open on first run
				fprintf(stderr, "ERROR: empty saveFile @ loadSave");
				return(NULL);
			}
		}
		// Read this node into memory
		else {
			curDataField = malloc(curDFBytes);
			if (curDataField == NULL){
				fprintf(stderr, "ERROR: malloc fail for curDataField @ loadSave\n");
				return(NULL);
			}
			if (fread(curDataField, 1, curDFBytes, saveFile) != curDFBytes){
				fprintf(stderr, "ERROR: bytesRead is not equal to curDFBytes @ loadSave\n");
				return(NULL);
			}

			if (newList == NULL){
				newList = makeList(curDFBytes, curDataField);
			}
			else{
				insertTail(newList, curDFBytes, curDataField);
			}
		}
	}

	return newList;
}
