/* Aidan Trent
 * For testing saveManager
 */

#pragma pack(1)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saveManager.h"

typedef struct{
	uint16_t nodeNum;
	uint8_t state;
} TestData;

LList* populateSave(int nodes){
	LList* list = NULL;
	uint8_t* raw = malloc(sizeof(TestData));
	TestData test;

	for (int i = 1; i <= nodes; i++){
		test.nodeNum = i;
		test.state = 255;
		memcpy(raw, &test, sizeof(TestData));
		if (list == NULL){
			list = makeList(sizeof(TestData), raw);
			if (list == NULL){
				return(NULL);
			}
		}
		else if (push(list, sizeof(TestData), raw) == NULL){
			return(NULL);
		}
	}
	return(list);
}

int main(void){
	LList* list = populateSave(3);
	if (list == NULL){
		return(EXIT_FAILURE);
	}

	if (saveList(list, "entityListTest.sav") == 1){
		return(EXIT_FAILURE);
	}


	LList* list2;
	list2 = loadSave("entityListTest.sav");
	//freeList(list);

	printf("%d\n", freeNode(list2, list->cur));
	printf("%d\n", freeNode(list2, list2->cur));
	if (saveList(list2, "entityListTest2.sav") == 1){
		return(EXIT_FAILURE);
	}
	freeList(list2);


	return EXIT_SUCCESS;
}
