//
// @Author ZhaoZelong
// @Date 2024/06/16
// @Email zhaozelong@leagsoft.com
// @Copyright (c) 2024 LeagSoft Inc.
// @Description
//

#ifndef C_EXERCISE_STACK_H
#define C_EXERCISE_STACK_H
#include <lcthw/list.h>

typedef struct Stack {
    List *l;
} Stack;

Stack *Stack_create(){
    Stack * ret = calloc(1,sizeof (Stack));
    ret->l = List_create();
    return ret;
}

void Stack_destroy(Stack* s){
    List_destroy(s->l);
    free(s);
}

void Stack_push(Stack *s,void* item){
    List_push(s->l,item);
}

void* Stack_peek(Stack *s){
    if(List_last(s->l) != NULL)
        return s->l->last->value;
    return NULL;
}

void* Stack_pop(Stack *s){
    if(List_last(s->l) != NULL){
        return List_pop(s->l);
    }
    return NULL;
}

#define Stack_count(A) ((A)->l->count)

#define STACK_FOREACH(S, V) ListNode *_node = NULL;\
    ListNode *V = NULL;\
    for(V = _node = S->l->last; _node != NULL; V = _node = _node->prev)
#endif //C_EXERCISE_STACK_H
