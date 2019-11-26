/* Bobitan Denisa-Ioana

   Task:

   Implement correctly and efficiently linear time transformations between three different representations for a multi-way tree: parent representation, multiway-tree representation, binary tree representation.

*/

#include <stdio.h>
#include <stdlib.h>

#pragma warning(disable : 4996)

int *nodesVector, *parentVector;
int n;

struct  node1
{
	int id;
	int parent;
};

struct node2 { //for the multi-way tree representation
	int id;
	struct node2 **children; //we use a pointer to a list of all children of a node 
	struct node2 *parent;
	int size;
};

struct node3 { //for the binary tree representation
	int id;
	struct node3 *rChild; //pointer to right child
	struct node3 *lChild; //pointer to left child
};

void scanVectors() //there are given as input 2 arrays: nodesVector (contains the keys of the nodes) and parentVector(contains the parent of each node)
{
	FILE *f = fopen("input.txt", "r");
	fscanf(f, "%d", &n);
	nodesVector = (int*)malloc(n * sizeof(int));
	parentVector = (int*)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%d", &nodesVector[i]);
	}
	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%d", &parentVector[i]);
	}
	fclose(f);
}

void printVectors() //display arrays
{
	printf("Nodes vector: ");
	for (int i = 0; i < n; i++)
	{
		printf("%d ", nodesVector[i]);
	}
	printf("\n");
	printf("Parent vector: ");
	for (int i = 0; i < n; i++)
	{
		printf("%d ", parentVector[i]);
	}
	printf("\n");
}

node2 *newNode2(int id)
{
	node2 *p = (node2*)malloc(sizeof(node2));
	p->id = id;
	p->size = 0;
	p->parent = NULL;
	p->children = (node2**)malloc(10 * sizeof(node2*)); //alloc memory for an array of 10 children
	return p;
}

void insertNode(int i, node2 **created, node2 **root)
{
	if (created[i] != NULL) //if the array has already been created, do nothing
	{
		return;
	}
	//else, create new node and insert it in the tree
	created[i] = newNode2(nodesVector[i]);
	if (parentVector[i] == 0) //if i is root
	{
		*root = created[i]; //set pointer to root
		return;
	}
	if (created[parentVector[i] - 1] == NULL) //if parent was not created yet, call the function recursively to create it
	{
		insertNode(parentVector[i], created, root);
	}
	//here, the parent of the current node is set and the new node is added to the children array
	node2 *p = created[parentVector[i] - 1];
	created[i]->parent = p;
	p->children[p->size] = created[i]; //we put new node on index "size"
	p->size++; //increment size
}

node2 *createMultiWayTree()
{
	node2 **created = (node2**)malloc(n * sizeof(node2*)); //use this array to link nodes and to check whether they have been created or not
	for (int i = 0; i < n; i++)
	{
		created[i] = NULL;
	}
	node2 *root = NULL;
	for (int i = 0; i < n; i++)
	{
		insertNode(i, created, &root);
	}
	return root;
}

void printMultiWayTree(node2 *root) //print each node and its children
{
	int x = root->id;
	printf("%d: ", root->id);
	if (root->size == 0)
	{
		printf("no children");
	}
	else
	{
		for (int i = 0; i < root->size; i++)
		{
			printf("%d ", root->children[i]->id);
		}
	}
	printf("\n");
	for (int i = 0; i < root->size; i++)
	{
		printMultiWayTree(root->children[i]);
	}
}

node3 *newNode3(int id)
{
	node3 *p = (node3*)malloc(sizeof(node3));
	p->id = id;
	p->lChild = p->rChild = NULL;
	return p;
}

node3 *multiWayToBinaryTree(node2 *root, int index)
{
	node3 *nod = newNode3(root->id); //create a new node for the binary tree
	if (root->size > 0) //if the root has children
	{
		nod->lChild = multiWayToBinaryTree(root->children[0], 0); //insert first element from root's children on the left branch of the new node
	}
	else //if (root->size <= 0)
	{
		nod->lChild = NULL; //there are no children, so it gets NULL
	}
	if (root->parent != NULL && index < root->parent->size - 1)
	{
		nod->rChild = multiWayToBinaryTree(root->parent->children[index + 1], index + 1); //insert siblings of the current node on its right branch
	}
	else
	{
		nod->rChild = NULL; //there are no siblings
	}
	return nod;
}

void prettyPrint(node3 *root, int level) //inorder
{
	if (root != NULL)
	{
		prettyPrint(root->rChild, level + 1);
		for (int i = 0; i <= level; i++)
		{
			printf("    ");
		}
		printf("%d\n", root->id);
		prettyPrint(root->lChild, level + 1);
	}
}

void demo() 
{
	scanVectors();
	printVectors();

	node2 *root = createMultiWayTree();
	printf("\nMulti-way tree representation:\n");
	printMultiWayTree(root);

	printf("\nBinary tree representation:\n");
	node3 *nod = multiWayToBinaryTree(root, 0);
	prettyPrint(nod, 0);
	printf("\n");
}

int main()
{
	int x = 1;
	while (x != 0)
	{
		printf("Enter:\n1 for test\n0 for exit\nOption: ");
		scanf("%d", &x);
		printf("\n");
		if (x == 1)
		{
			demo();
			printf("\n");
		}
	}

	return 0;
}