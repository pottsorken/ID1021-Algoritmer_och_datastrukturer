#include <stdlib.h>
#include <stdio.h>

typedef struct stack
{
    int top;
    int size;
    int *array;
} stack;

stack *new_stack(int size)
{
    int *array = (int *)malloc(size * sizeof(int));
    stack *stk = (stack *)malloc(sizeof(stack));

    stk->top = 0;
    stk->size = size;
    stk->array = array;

    return stk;
}

void push(stack *stk, int val)
{
    if (stk->top == stk->size)
    {
        int *copy = (int *)malloc(stk->size * 2 * sizeof(int));
        for (int i = 0; i < stk->size; i++)
        {
            int *from = stk->array - stk->top + i;
            copy[i] = *from;
        }
        free(stk->array);
        stk->array = copy + stk->top;
        stk->size *= 2;
    }

    *(stk->array) = val;
    stk->top++;
    stk->array++;
}

int pop(stack *stk)
{
    if (stk->top == 0)
    {
        printf("This stack is empty, no pop operation allowed!");
        return 0;
    }

    if (stk->top*2 == stk->size)
    {
        int *copy = (int *)malloc(stk->top * sizeof(int));

        for (int i = 0; i < stk->top; i++)
        {
            int *from = stk->array - stk->top + i;
            copy[i] = *from;
        }
        
        free(stk->array);
        stk->array = copy + stk->top;
        stk->size = stk->top;
    }
    stk->array--;
    stk->top--;
    int val = *(stk->array);
    return val;
}

void addition(stack *stk) {
    int val1 = pop(stk);
    int val2 = pop(stk);
    int result = val1 + val2;
    printf("ADDITION: %d + %d = %d\n", val1, val2, result);
    push(stk, result);
}

void subtraction(stack *stk) {
    int val1 = pop(stk);
    int val2 = pop(stk);
    int result = val2 - val1;
    printf("SUBTRACTION: %d - %d = %d\n", val2, val1, result);
    push(stk, result);
}

void multiplication(stack *stk) {
    int val1 = pop(stk);
    int val2 = pop(stk);
    int result = val1 * val2;
    printf("MULTIPLICATION: %d * %d = %d\n", val1, val2, result);
    push(stk, result);
}

void division(stack *stk) {
    int val1 = pop(stk);
    int val2 = pop(stk);
    int result = val2 / val1;
    printf("DIVISION: %d / %d = %d\n", val2, val1, result);
    push(stk, result);
}

int main()
{
    printf("-------- CALCULATOR --------\n");
    stack *stk = new_stack(4);
    char user_input[] = "";
    while (1) {
        scanf("%s", &user_input);

        int user_int = atoi(user_input);

        if (user_int != 0 || !strcmp("0", user_input)) {
            push(stk, user_int);
        } else { 
            if (!strcmp("+", user_input)) {
                addition(stk);
            }
            if (!strcmp(user_input, "-")) {
                subtraction(stk);
            }
            if (!strcmp(user_input, "*")) {
                multiplication(stk);
            }
            if (!strcmp(user_input, "/")) {
                division(stk);
            }
        }
    }
}