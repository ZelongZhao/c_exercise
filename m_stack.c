#include <stddef.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct stack{
    void **buff;
    int top;
    size_t cap;
} m_stack;

bool stack_init(m_stack *s,size_t len){
    s->buff = malloc(len * sizeof (void *));
    if(s->buff == NULL){
        perror("stack init malloc error");
        return false;
    }
    s->top = -1;
    s->cap = len;
    return true;
}

bool stack_top(m_stack *s, void **item) {
    if (s == NULL || item == NULL) {
        return false;
    }
    if (s->top == -1) {
        return false;
    }
    if (s->buff == NULL) {
        return false;
    }
    *item = s->buff[s->top];
    return true;
}

bool stack_is_empty(m_stack *s){
    return s->top == -1;
}

bool stack_push(m_stack *s,void *item){
    if(s ==NULL || s->buff == NULL)
        return false;
    if(s->top == s->cap - 1){
        void** new_buff = realloc(s->buff, s->cap * 2 * sizeof (void *));
        if(new_buff == NULL){
            return false;
        }
        s->buff = new_buff;
        s->cap *= 2;
    }


    s->buff[++s->top] = item;
    return true;
}

bool stack_pop(m_stack *s,void **item){
    if(s == NULL || item == NULL)
        return false;

    if(stack_is_empty(s))
        return false;

    *item = s->buff[s->top--];
    return true;
}

void stack_destroy(m_stack *s){
    if(s){
        if(s->buff){
            free(s->buff);
        }
    }
}

int main() {
    m_stack s;
    size_t len = 10; // 初始化栈的大小

    // 初始化栈
    if (!stack_init(&s, len)) {
        fprintf(stderr, "Stack initialization failed.\n");
        return 1;
    }

    // 测试入栈
    for (int i = 0; i < len; ++i) {
        void *data = malloc(sizeof(int)); // 假设我们存储的是整数
        *(int*)data = i;
        if (!stack_push(&s, data)) {
            fprintf(stderr, "Stack push failed at index %d.\n", i);
            // 清理之前成功入栈的元素
            while (!stack_is_empty(&s)) {
                void *temp;
                stack_pop(&s, &temp);
                free(temp);
            }
            return 1;
        }
    }

    // 测试查看栈顶元素
    void *topItem;
    if (stack_top(&s, &topItem)) {
        printf("Top item: %d\n", *(int*)topItem);
    } else {
        fprintf(stderr, "Failed to get top item.\n");
    }

    // 测试出栈
    for (int i = len - 1; i >= 0; --i) {
        if (stack_pop(&s, &topItem)) {
            printf("Popped item: %d\n", *(int*)topItem);
            free(topItem); // 释放被弹出元素的内存
        } else {
            fprintf(stderr, "Stack pop failed at index %d.\n", i);
            break;
        }
    }

    // 测试栈是否为空
    if (stack_is_empty(&s)) {
        printf("Stack is empty after all items are popped.\n");
    } else {
        fprintf(stderr, "Stack is not empty.\n");
    }

    // 销毁栈
    stack_destroy(&s);

    return 0;
}