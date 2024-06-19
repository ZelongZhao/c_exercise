//
// @Author ZhaoZelong
// @Date 2024/06/16
// @Email zhaozelong@leagsoft.com
// @Copyright (c) 2024 LeagSoft Inc.
// @Description
//

#ifndef C_EXERCISE_QUEUE_H
#define C_EXERCISE_QUEUE_H

#include "list.h"

typedef struct Queue {
    List *l;
} Queue;

Queue *Queue_create(){
    Queue * q = calloc(1,sizeof (Queue));
    q->l = List_create();
    return q;
}

void Queue_destroy(Queue* q){
    List_destroy(q->l);
    free(q);
}

void Queue_send(Queue *q,void* item){
    List_unshift(q->l,item);
}

void* Queue_recv(Queue *q){
    return List_pop(q->l);
}

void* Queue_peek(Queue *q){
    if(List_last(q->l) != NULL)
        return q->l->last->value;
    return NULL;
}

#define Queue_count(A) ((A)->l->count)

#define QUEUE_FOREACH(S, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = S->l->first; _node != NULL; V = _node = _node->next)

#endif //C_EXERCISE_QUEUE_H
