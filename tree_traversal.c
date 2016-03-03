/*
 * ROUGH DRAFT FOR TREE INSERTION AND TRAVERSAL FROM GIVEN STRINGS
 *
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct node
{
	int data;
	struct node* left;
	struct node* right;
};

//int search(char arr[], int strt, int end, char value);
int search(char *arr, int strt, int end, int value);
struct node* newNode(int data);

int search(char *arr, int strt, int end, int value)
{
	int i;
	for (i = strt; i <= end; i++)
	{
		int data = arr[i] - '0';
		if (data == value)
			return i;
	}
}

struct node* newNode(int data)
{
	struct node* node = (struct node*)malloc(sizeof(struct node));
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	return(node);
}

int size(struct node* node)
{
	if (node == NULL)
		return 0;
	else
		return(size(node->left) + 1 + size(node->right));
}
void preorderTree(struct node* node,char *pr)
{
	if (node == NULL)
		return;
	sprintf(pr, "%s%d", pr, node->data);
	preorderTree(node->left,pr);
	preorderTree(node->right,pr);
}

char* treeToStringPreorder(struct node* root)
{
	int sizeTree = size(root);
	char *pre = (char *)calloc(sizeTree, sizeof(char));
	preorderTree(root,pre);
	return pre;
}

void inorderTree(struct node* node, char *pr)
{
	if (node == NULL)
		return;
	inorderTree(node->left, pr);
	sprintf(pr, "%s%d", pr, node->data);
	inorderTree(node->right, pr);
}

char* treeToStringInorder(struct node* root)
{
	int sizeTree = size(root);
	char *in = (char *)calloc(sizeTree, sizeof(char));
	inorderTree(root, in);
	return in;
}

struct node* stringToTree(char *in, char *pre, int inStrt, int inEnd)
{
	static int preIndex = 0;

	if (inStrt > inEnd)
		return NULL;
	
	int data = (pre[preIndex++])-'0';
	struct node *tNode = newNode(data);
	if (inStrt == inEnd)
		return tNode;

	int inIndex = search(in, inStrt, inEnd, tNode->data);

	
	tNode->left = stringToTree(in, pre, inStrt, inIndex - 1);
	tNode->right = stringToTree(in, pre, inIndex + 1, inEnd);

	return tNode;
}

void printInorder(struct node* node)
{
	if (node == NULL)
		return;
	printInorder(node->left);
	printf("%d ", node->data);
	printInorder(node->right);
}

int main(int argc, char* argv[])
{
	struct node *root = newNode(3);
	root->left = newNode(9);
	root->right = newNode(5);
	root->left->left = newNode(1);
	root->left->right = newNode(2);
	root->right->left = newNode(7);
	root->right->right = newNode(8);
	char *in  = treeToStringInorder(root);
	char *pre = treeToStringPreorder(root);
	int len = strlen(in);
	struct node *rootNew = stringToTree(in, pre, 0, len - 1);

	printf("\n Inorder traversal of the constructed tree is \n");
	printInorder(rootNew);
	getchar();
	return 0;
}

