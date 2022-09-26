/* Aidan Trent
 * For testing saveManager
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "saveManager.h"

#pragma pack(1)
typedef struct{
	char rep;
	int32_t y;
	int32_t x;
	uint8_t state;
} TestData;

LList* populateSave(int nodes){
	LList* list = NULL;
	uint8_t* raw = malloc(sizeof(TestData));
	TestData test;

	for (int i = -20; i <= nodes; i++){
		test.rep = '#';
		test.y = i;
		test.x = i;
		test.state = 255;

		memcpy(raw, &test, sizeof(TestData));
		if (list == NULL){
			list = makeList(sizeof(TestData), raw);
			if (list == NULL){
				return(NULL);
			}
		}
		else if (insertTail(list, sizeof(TestData), raw) == NULL){
			return(NULL);
		}
	}
	return(list);
}

int main(void){
	LList* list = populateSave(100);
	if (list == NULL){
		return(EXIT_FAILURE);
	}

	if (saveList(list, "entityListTest.sav") == 1){
		return(EXIT_FAILURE);
	}

	LList* list2;
	list2 = loadSave("entityListTest.sav");

	TestData loadedStruct;
	memcpy(&loadedStruct, list2->head->data, sizeof(TestData));
	printf("list2 head data...\n");
	printf("rep = %u\n", loadedStruct.rep);
	printf("y = %" PRIu32 "\n", loadedStruct.y);
	printf("x = %" PRIu32 "\n", loadedStruct.x);
	printf("state = %" PRIu8 "\n\n", loadedStruct.state);


	if (saveList(list2, "entityListTest2.sav") == 1){
		return(EXIT_FAILURE);
	}
	printf("freeNode that DNE failure = %d\n\n", freeNode(list2, list->head));
	printf("freeNode that does exist failure = %d\n\n", freeNode(list2, list2->head));
	freeList(list);
	freeList(list2);


	return EXIT_SUCCESS;
}

