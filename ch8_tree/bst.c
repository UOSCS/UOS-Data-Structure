#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct TreeNode
{
    int key;
    struct TreeNode *left, *right;
} TreeNode;

TreeNode *new_node(TreeNode *, int);
TreeNode *search(TreeNode *, int);
TreeNode *insert(TreeNode *, int);
TreeNode *delete(TreeNode *, int);
int find_min(TreeNode *);
void inorder(TreeNode *);
void preorder(TreeNode *);
void postorder(TreeNode *);

int main(void)
{
    TreeNode *root = NULL;
    int key;
    int select;
    char ch;

    while(1)
    {
        printf("노드의 key값을 콤마(,) 또는 공백으로 분리하여 입력하세요(종료하려면 q). : \n");
        if(scanf("%d", key) != 1)
            if((ch = getchar()) == 'q')
                return 0;
            else
            {
                printf("올바른 값을 입력하세요.\n");
                while(ch = getchar())
                    continue;
                continue;
            }
        while(scanf("%d", &key) == 1)
        {
            root = insert(root, key);

            while(scanf("%c", &ch) == 1)
                if(isdigit(ch))
                    ;
        }

        printf("순회 방법을 선택하세요.\n");
        printf("1. 전위순회     2. 중위순회\n");
        printf("3. 후위순회     4. 레벨순회\n");
        scanf("%d", &select);
    }
}

TreeNode *new_node(TreeNode *node, int key)
{
    node = (TreeNode *)malloc(sizeof(TreeNode));
    node->key = key;
    node->left = node->right = NULL;

    return node;
}

TreeNode *search(TreeNode *node, int key)
{
    if(node == NULL)
        return NULL;

    if(key == node->key)
        return node;
    else if(key < node->key)
        return search(node->left, key);
    else
        return search(node->right, key);
}

TreeNode *insert(TreeNode *node, int key)
{
    if(node == NULL)
        return new_node(node, key);

    if(key <= node->key)
        node->left = insert(node->left, key);
    else
        node->right = insert(node->right, key);
    return node;
}

TreeNode *delete(TreeNode *node, int key)
{
    if(node == NULL)
        return node;

    if(key < node->key)
        node->left = delete(node->left, key);
    else if(key > node->key)
        node->right = delete(node->right, key);
    else
    {
        if(node->left == NULL)
        {
            TreeNode *temp = node->right;
            free(node);
            return temp;
        }
        else if(node->right == NULL)
        {
            TreeNode *temp = node->left;
            free(node);
            return temp;
        }
        else
        {
            node->key = find_min(node->right);
            node->right = delete(node->right, node->key);
            return node;
        }
    }
    
    return node;
}

int find_min(TreeNode *node)
{
    for(; node->left; node = node->left)
        continue;
    return node->key;
}

void inorder(TreeNode *root)
{
    if(!root)
    {
        inorder(root->left);
        printf("[%d] ", root->key);
        inorder(root->right);
    }
}