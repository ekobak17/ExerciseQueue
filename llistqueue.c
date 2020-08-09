/*
 * implementation for linked-list-based generic FIFO queue
 */

#include "ADTs/llistqueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#define DEFAULT_CAPACITY 50L;

typedef struct node Node;
struct node {
	Node *next;
	void *variable;
};

typedef struct q_data {
	long size;
	void (*freeValue)(void *e);
	Node *head;
	Node *tail;
    /* flesh out the instance specific data structure */
} QData;

static void purge(QData *qd, void (freeValue)(void *element)) {
	if (freeValue != NULL) {
		Node *p;
		for (p = qd->head; p != NULL; p = p->next)
			(*freeValue)(p->variable);
	}
}
static void freeList(QData *qd) {
	if (qd != NULL) {
		Node *p = qd->head;
		while (p != NULL) {
			Node *q = p->next;
			free(p);
			p = q;
		}
	}
}

static void q_destroy(const Queue *q) {
	QData *qd = (QData *)q->self;
	purge(qd, qd->freeValue);
	freeList(qd);
	free(qd);
	free((void *)q);

}

static void q_clear(const Queue *q) {
	QData *qd = (QData *)q->self;
	purge(qd, qd->freeValue);
	qd->size = 0L;
	qd->head = NULL;
	qd->tail = NULL;
}

static int q_enqueue(const Queue *q, void *element) {
	QData *qd = (QData *)q->self;
	Node *n = (Node *)malloc(sizeof(Node));

	if (n == NULL) {
		return 0;
	}
	n->variable = element;
	n->next = NULL;
	qd->size++;
	if (qd->head == NULL) {
		qd->head = n;
	}
	else {
		Node *tmp = qd->head;
		while (tmp->next != NULL) {
			tmp = tmp->next;
		}
		tmp->next = n;
	}
	qd->tail = n;
	return 1;
}

static int q_front(const Queue *q, void **element) {
	QData *qd = (QData *)q->self;
	int status = 0;
	Node *p = (Node*)malloc(sizeof(Node));
	if (qd != NULL) {
		status = 1;
		*element = p->variable;
	}
	return status;
}

static int q_dequeue(const Queue *q, void **element) {
	QData *qd = (QData *)q->self;
	if (qd->head == NULL)
		return 0;
	Node *tmp = qd->head;
	if (tmp != NULL) {
		*element = tmp->variable;
		qd->head = tmp->next;
	}
	free(tmp);
	qd->size--;
	return 1;
}

static long q_size(const Queue *q) {
	QData *qd = (QData *)q->self;
	return qd->size;
}

static int q_isEmpty(const Queue *q) {
	QData *qd = (QData *)q->self;
	return (qd->size == 0L);
}

static void **genArray(QData *qd) {
	void **tmp = NULL;
	if (qd->size > 0L) {
		size_t nbytes = qd->size *sizeof(void *);
		tmp = (void **)malloc(nbytes);
		if (tmp != NULL) {
			long i;
			Node *p;
			for (i = 0, p = qd->head; p != NULL; i++, p = p->next)
				tmp[i] = p->variable;
		}
	}
	return tmp;
}

static void **q_toArray(const Queue *q, long *len) {
	QData *qd = (QData *)q->self;
	void **tmp = genArray(qd);
	if (tmp != NULL) 
		*len = qd->size;
   return tmp;
}

static const Iterator *q_itCreate(const Queue *q) {
	QData *qd = (QData *)q->self;
	const Iterator *it = NULL;
	void **tmp = genArray(qd);
	if (tmp != NULL) {
		it = Iterator_create(qd->size, tmp);
		if (it == NULL)
			free(tmp);
	}
	return it;
}

static const Queue *q_create(const Queue *q);
/* this is just declaring the signature for the create() method; it's
   implementation is provided below */

static Queue template = {
    NULL, q_create, q_destroy, q_clear, q_enqueue, q_front, q_dequeue, q_size,
    q_isEmpty, q_toArray, q_itCreate
};

static const Queue *newQueue(void (*freeValue)(void *e)) {
	Queue *q = (Queue *)malloc(sizeof(Queue));

	if (q != NULL) {
		QData *qd = (QData *)malloc(sizeof(QData));
		if (qd != NULL) {
			qd->size = 0L;
			qd->head = NULL;
			qd->tail = NULL;	
			qd->freeValue = freeValue;
			*q = template;
			q->self = qd;
		}
		else {
			free(q);
			free(qd);
			q = NULL;
		}
	}
	else {
		free(q);
		q = NULL;
	}
	return q;
}

static const Queue *q_create(const Queue *q) {
	QData *qd = (QData *)q->self;
	return newQueue(qd->freeValue);
    /* implementation of the create() method */
}

const Queue *LListQueue(void (*freeValue)(void *e)) {
	return newQueue(freeValue);
    /* implementation of the structure-specific constructor */
}

const Queue *Queue_create(void (*freeValue)(void *e)) {
	return newQueue(freeValue);
    /* implementation of the generic constructor */
}
