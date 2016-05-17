// Operator supported: +,-,*,/,%,^,(,)
// Operands supported: all single character operands
 
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
 
#define MAX 50
struct node
{
	char element;
	struct node *left,*right;
}; 
typedef struct stack
{
    int data[MAX];
    int top;
}stack;
 
int precedence(char);
void init(stack *);
int empty(stack *);
int full(stack *);
int pop(stack *);
void push(stack *,int);
int top(stack *);   //value of the top element
void infix_to_postfix(char infix[],char postfix[]);
void postfix2exptree(char* postfix, struct node **root);
void inorder(struct node *x);
void preorder(struct node *x);
void postorder(struct node *x);
 
void main()
{
    char infix[30],postfix[30];
    struct node *r;
    printf("Enter an infix expression(eg: 5+2*4): ");
    gets(infix);
    infix_to_postfix(infix,postfix);
    printf("\nPostfix expression: %s",postfix);
    postfix2exptree(postfix,&r);
    printf("\nInorder = ");
    inorder(r);
    printf("\nPreorder = ");
    preorder(r);
    printf("\nPostorder = ");
    postorder(r);
    printf("\n");
}
void postfix2exptree(char* postfix, struct node **root)
{
	struct stack X;
	struct node *newnode,*op1,*op2;
	char *p;
	p = &postfix[0];
	empty(&X);
	while(*p)
	{

		while(*p == ' ' || *p == '\t')
		{
			p++;
		}

		if(isalpha(*p) || isdigit(*p))
		{
			newnode = malloc(sizeof(struct node));
			newnode->element = *p;
			newnode->left = NULL;
			newnode->right = NULL;
			push(&X,newnode);
		}
		else
		{
			op1 = pop(&X);
			op2 = pop(&X);
			newnode = malloc(sizeof(struct node));
			newnode->element = *p;
			newnode->left = op2;
			newnode->right = op1;
			push(&X,newnode);
		}
		p++;
	}
	*root = pop(&X);
}

void inorder(struct node *x)
{
	if(x != NULL)
	{
		inorder(x->left);
		printf("%c ",x->element);
		inorder(x->right);
	}
}

void preorder(struct node *x)
{
	if(x != NULL)
	{
		printf("%c ",x->element);
		preorder(x->left);
		preorder(x->right);
	}
}

void postorder(struct node *x)
{
	if(x != NULL)
	{
		postorder(x->left);
		postorder(x->right);
		printf("%c ",x->element);
	}
} 
void infix_to_postfix(char infix[],char postfix[])
{
    stack s;
    char x,token;
    int i,j;    //i-index of infix,j-index of postfix
    init(&s);
    j=0;
 
    for(i=0;infix[i]!='\0';i++)
    {
        token=infix[i];
        if(isalnum(token))
            postfix[j++]=token;
        else
            if(token=='(')
               push(&s,'(');
        else
            if(token==')')
                while((x=pop(&s))!='(')
                      postfix[j++]=x;
                else
                {
                    while(precedence(token)<=precedence(top(&s))&&!empty(&s))
                    {
                        x=pop(&s);
                        postfix[j++]=x;
                    }
                    push(&s,token);
                }
    }
 
    while(!empty(&s))
    {
        x=pop(&s);
        postfix[j++]=x;
    }
 
    postfix[j]='\0';
}
 
int precedence(char x)
{
    if(x=='(')
        return(0);
    if(x=='+'||x=='-')
        return(1);
    if(x=='*'||x=='/'||x=='%')
        return(2);
 
    return(3);
}
 
void init(stack *s)
{
    s->top=-1;
}
 
int empty(stack *s)
{
    if(s->top==-1)
        return(1);
 
    return(0);
}
 
int full(stack *s)
{
    if(s->top==MAX-1)
        return(1);
 
    return(0);
}
 
void push(stack *s,int x)
{
    s->top=s->top+1;
    s->data[s->top]=x;
}
 
int pop(stack *s)
{
    int x;
    x=s->data[s->top];
    s->top=s->top-1;
    return(x);
}
 
int top(stack *p)
{
    return (p->data[p->top]);
}
