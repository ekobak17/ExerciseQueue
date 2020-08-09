/*
 * Author: Ellie Kobak
 * project exercise queue
 * usage: ./exerciseQueue file.in | diff - file.out
 */

#include "ADTs/queue.h"
#include <stdio.h>
#include <string.h>

void printQueue(const Queue *ql) {
	const Queue *tmp = Queue_create(NULL);
	long var;
	while (ql->dequeue(ql, (void**)&var)) {
		printf("%ld%s", var, (0 == ql->size(ql)) ? "" : " ");
		tmp->enqueue(tmp, (void*)var);
		}
	printf("\n");
	while (tmp->dequeue(tmp, (void**)&var)) {
		ql->enqueue(ql, (void*)var);
	}	
	tmp->destroy(tmp);
} 

int main(int argc, char* argv[]) {
	char line[BUFSIZ];
	int count;
	int i;
	long variable;
	if (argc != 2) {
		fprintf(stderr, "Only 1 file can be processed"); 
		return 1;
	}
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "Unable to open file\n"); 
		return 1;
	}
	const Queue *ql = Queue_create(NULL);
	if (ql == NULL) {
		fprintf(stderr, "Queue not created"); 
		return 1;
	}
	fgets(line, BUFSIZ, fp);
	sscanf(line, "%d", &count);
	for (i=0; i < count; i++) {
		fgets(line, BUFSIZ, fp);
		if (strncmp(line,"enqueue ", 8) == 0) {
			long var;
			sscanf(line + 8, "%ld\n", &var);
			ql->enqueue(ql, (void*)var);
		}
		else if (strcmp(line,"dequeue\n") == 0) {
			if (ql->size(ql) != 0) {
				if (ql->dequeue(ql, (void**)&variable)) 
					printf("%ld\n", variable);
			}
			else
				printf("QueueError\n");
		}
		else if(strcmp(line, "print\n") == 0) {
			if (ql->size(ql) == 0)
				printf("Empty\n");
			else
				printQueue(ql);
		}	
		else {
			continue;
		}
	}
	ql->destroy(ql);
	fclose(fp);
	return 0;
}	
