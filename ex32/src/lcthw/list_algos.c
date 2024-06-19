#include <stdio.h>
#include "list_algos.h"
void List_swap_nodes(ListNode *first, ListNode *second)
{
    void *tmp = first->value;
    first->value = second->value;
    second->value = tmp;
}

int List_bubble_sort(List *list, List_compare cmp){
    int n = list->count;
    while (n--){
        LIST_FOREACH(list,first,next,cur){
            if(!cur->next) continue;
            if(cmp(cur->value,cur->next->value) > 0){
                List_swap_nodes(cur,cur->next);
            }
        }
    }
    return 0;
}

List *List_merge_sort(List *list, List_compare cmp){
    int len = List_count(list);
    if(len <= 1)return list;

    List *first = List_create();
    List *second = List_create();
    List *result = List_create();

    int i = 0;
    LIST_FOREACH(list,first,next,cur){
        if(i < len / 2){
            List_push(first,cur->value);
        } else{
            List_push(second,cur->value);
        }
        i++;
    }

    first= List_merge_sort(first,cmp);
    second = List_merge_sort(second,cmp);

    ListNode *a = first->first;
    ListNode *b = second->first;

    while (a != NULL && b != NULL){
         if(cmp(a->value,b->value) < 0){
            List_push(result,a->value);
            a = a->next;
        } else{
            List_push(result,b->value);
            b = b->next;
        }
    }
    while (a != NULL){
        List_push(result,a->value);
        a = a->next;
    }
    while (b != NULL){
        List_push(result,b->value);
        b = b->next;
    }

    List_destroy(first);
    List_destroy(second);

    return result;
}