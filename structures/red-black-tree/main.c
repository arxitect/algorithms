#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_ELEMS 250

struct node
{
    int data;
    char color;
    struct node *left, *right, *parent;
};

void LeftRotate(struct node **root,struct node *x);
void rightRotate(struct node **root,struct node *y);
void insertFixUp(struct node **root,struct node *z);
void insertFixUp(struct node **root,struct node *z);
void insert(struct node **root, int data);
void inorder(struct node *root);

int main()
{
    srandom(time(NULL));
    struct node *root = NULL;

    clock_t t0 = clock();
    for (int i = 0; i < NB_ELEMS; ++i)
        insert(&root, random());
    clock_t t1 = clock();
    printf("inorder Traversal Is :\n");
    inorder(root);
    printf("\n");
    float time_taken = (float)(t1 - t0) / CLOCKS_PER_SEC * 1000;
    printf("insertion took %fms -> %fus/elem\n",
        time_taken,
        time_taken / NB_ELEMS * 1000);
    
    return 0;
}

void LeftRotate(struct node **root,struct node *x)
{
    if (!x || !x->right)
        return ;
    struct node *y = x->right;

    x->right = y->left;

    if (x->right != NULL)
        x->right->parent = x;

    y->parent = x->parent;

    if (x->parent == NULL)
        (*root) = y;

    else if (x == x->parent->left)
        x->parent->left = y;
    else    x->parent->right = y;

    y->left = x;

    x->parent = y;
}


void rightRotate(struct node **root,struct node *y)
{
    if (!y || !y->left)
        return ;
    struct node *x = y->left;
    y->left = x->right;
    if (x->right != NULL)
        x->right->parent = y;
    x->parent =y->parent;
    if (x->parent == NULL)
        (*root) = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

void insertFixUp(struct node **root,struct node *z)
{
    while (z != *root && z != (*root)->left && z != (*root)->right && z->parent->color == 'R'){
        struct node *y;

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
        }

        else
        {
            if (z->parent == z->parent->parent->left &&
                z == z->parent->left)
            {
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent->parent);
            }

            if (z->parent && z->parent->parent && z->parent == z->parent->parent->left &&
                z == z->parent->right){
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root,z->parent);
                rightRotate(root,z->parent->parent);
            }

            if (z->parent && z->parent->parent &&
                z->parent == z->parent->parent->right &&
                z == z->parent->right){
                char ch = z->parent->color ;
                z->parent->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                LeftRotate(root,z->parent->parent);
            }

            if (z->parent && z->parent->parent && z->parent == z->parent->parent->right &&
                z == z->parent->left){
                char ch = z->color ;
                z->color = z->parent->parent->color;
                z->parent->parent->color = ch;
                rightRotate(root,z->parent);
                LeftRotate(root,z->parent->parent);
            }
        }
    }
    (*root)->color = 'B'; 
}

void insert(struct node **root, int data)
{
    struct node *z = (struct node*)malloc(sizeof(struct node));
    z->data = data;
    z->left = z->right = z->parent = NULL;

    if (*root == NULL){
        z->color = 'B';
        (*root) = z;
    }else{
        struct node *y = NULL;
        struct node *x = (*root);

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

void inorder(struct node *root)
{
    static int last = 0;
    if (root == NULL)
        return;
    inorder(root->left);
    printf("%d ", root->data);
    if (root->data < last)
        printf("\nPUTE\n");
    last = root->data;
    inorder(root->right);
}