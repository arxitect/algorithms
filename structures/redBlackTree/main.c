#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_ELEMS 250

// node structure
struct node
{
    int data; // data of node
    char color; // color of node: red or black
    struct node *left, *right, *parent; // children and parent
};
typedef struct node Node;

void leftRotate(Node **root,Node *x);
void rightRotate(Node **root,Node *y);
void insertFixUp(Node **root,Node *z);
void insert(Node **root, int data);
void inOrder(Node *root);

int main()
{
    srandom(time(NULL));
    Node *root = NULL;

    clock_t t0 = clock();
    for (int i = 0; i < NB_ELEMS; ++i)
        insert(&root, random());
    clock_t t1 = clock();

    printf("inOrder Traversal Is :\n");
    inOrder(root);
    printf("\n");
    float time_taken = (float)(t1 - t0) / CLOCKS_PER_SEC * 1000;
    printf("insertion took %fms -> %fus/elem\n",
        time_taken,
        time_taken / NB_ELEMS * 1000);
    
    return 0;
}

/* leftRotate: left rotation */
void leftRotate(Node **root,Node *x)
{
    if (!x || !x->right)
        return ;

    //y stored pointer of right child of x
    Node *y = x->right;

    x->right = y->left;

    //update parent pointer 
    if (x->right != NULL)
        x->right->parent = x;

    y->parent = x->parent;

    //if x is NULL make y as root
    if (x->parent == NULL)
        (*root) = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;

    x->parent = y;
}

/* rightRotate: similar leftRotate */
void rightRotate(Node **root, Node *y)
{
    if (!y || !y->left)
        return ;

    Node *x = y->left;
    y->left = x->right;

    if (x->right != NULL)
        x->right->parent = y;

    x->parent =y->parent;

    if (x->parent == NULL)
        (*root) = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

/* insertFixUp: fix tree after insertion */
void insertFixUp(Node **root,Node *z)
{
    while (z != *root && z != (*root)->left && z != (*root)->right && z->parent->color == 'R'){
        Node *y;

        if (z->parent && z->parent->parent && z->parent == z->parent->parent->left)
            y = z->parent->parent->right;
        else
            y = z->parent->parent->left;

        if (!y)
            z = z->parent->parent;
        else if (y->color == 'R'){
            y->color = 'B';
            z->parent->color = 'B';
            z->parent->parent->color = 'R';
            z = z->parent->parent;
        }else{
            if (z->parent == z->parent->parent->left &&
                z == z->parent->left) {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent->parent);
            }

            if (z->parent && z->parent->parent && z->parent == z->parent->parent->left &&
                z == z->parent->right) {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                leftRotate(root,z->parent);
                rightRotate(root,z->parent->parent);
            }

            if (z->parent && z->parent->parent &&
                z->parent == z->parent->parent->right &&
                z == z->parent->right) {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                leftRotate(root,z->parent->parent);
            }

            if (z->parent && z->parent->parent && z->parent == z->parent->parent->right &&
                z == z->parent->left) {
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent);
                leftRotate(root,z->parent->parent);
            }
        }
    }
    (*root)->color = 'B'; 
}

/* insert: insert new node of tree */
void insert(Node **root, int data)
{
    Node *z = (Node*)malloc(sizeof(Node));

    z->data = data;
    z->left = z->right = z->parent = NULL;

    if (*root == NULL){
        z->color = 'B';
        (*root) = z;
    }else {
        Node *y = NULL;
        Node *x = (*root);

        while (x != NULL){
            y = x;
            if (z->data < x->data)
                x = x->left;
            else
                x = x->right;
        }

        z->parent = y;
        if (z->data > y->data)
            y->right = z;
        else
            y->left = z;

        z->color = 'R';
        insertFixUp(root,z);
    }
}
/* inOrder: traverse tree in order */
void inOrder(Node *root)
{
    static int last = 0;
    if (root == NULL)
        return;

    inOrder(root->left);
    printf("%d \n", root->data);

    if (root->data < last)
        printf("\nPUTE\n");

    last = root->data;
    inOrder(root->right);
}