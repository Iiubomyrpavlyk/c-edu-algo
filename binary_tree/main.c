#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef enum
{
    DECIMAL_ELEM,
    REAL_ELEM,
    STRING_ELEM,
} vtype_tree_t;

typedef union
{
   int64_t decimal;
   double real;
   uint8_t *string;
} value_tree_t;

typedef struct tree_node
{
    struct 
    {
        value_tree_t key;
        value_tree_t value;
    } data;

    struct tree_node *left;
    struct tree_node *right;
    struct tree_node *parent;
} tree_node;

typedef struct Tree
{
    struct 
    {
        vtype_tree_t key;
        vtype_tree_t value;
    } type;
    
    struct tree_node *node;

} Tree;

extern Tree *new_tree(vtype_tree_t key, vtype_tree_t value);
extern void free_tree(Tree *tree);

extern value_tree_t get_tree(Tree *tree, void *key);
extern void set_tree(Tree *tree, void *key, void *value);
extern void del_tree(Tree *tree, void *key);
extern _Bool in_tree(Tree *tree, void *key);

static tree_node * _new_node(vtype_tree_t vkey, vtype_tree_t tvalue, void *key, void *value);
static void _set_value(tree_node *node, vtype_tree_t tvalue, void *value);
static void _set_key(tree_node *node, vtype_tree_t tkey, void *key);
static void _set_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value);

static tree_node * _get_tree(tree_node *node, vtype_tree_t tkey, void *key);
static tree_node * del1_tree(Tree *tree, vtype_tree_t tkey, void *key);
static void _del3_tree(tree_node *node);
static void _del2_tree(Tree *tree, tree_node *node);
static void _free_tree(tree_node *node);

extern void * decimal(int64_t x);
extern void * real(double x);
extern void * string(uint8_t *x);

extern void print_tree(Tree *tree);
extern void print_tree_as_list(Tree *tree);

static void _print_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static void _print_tree_elem(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);
static void _print_tree_as_list(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue);

int main(void)
{
    Tree *tree = new_tree(DECIMAL_ELEM, STRING_ELEM);

    set_tree(tree, decimal(50), string("A"));
    set_tree(tree, decimal(40), string("B"));
    set_tree(tree, decimal(90), string("C"));
    set_tree(tree, decimal(30), string("D"));
    set_tree(tree, decimal(70), string("E"));
        
//    value_tree_t v = get_tree(tree, decimal(50));
 //   printf("%s\n", v.string);
   
  //  printf("%d", in_tree(tree, decimal(100)));

    del_tree(tree, decimal(40));

    print_tree_as_list(tree);  

    free_tree(tree);
    return 0;
}


extern Tree *new_tree(vtype_tree_t key, vtype_tree_t value)
{
    switch (key) 
    {
        case DECIMAL_ELEM: case STRING_ELEM:
            break;
        default:
            fprintf(stderr, "%s\n", "Key type not supported");
            return NULL;
    } 

    switch (value) 
    {
        case DECIMAL_ELEM: case STRING_ELEM: case REAL_ELEM:
            break;
        default:
            fprintf(stderr, "%s\n", "value type not supported");
            return NULL;
    }
    
    Tree *tree = (Tree *) malloc(sizeof(Tree));
    tree->type.key = key;
    tree->type.value = value;
    tree->node = NULL;
    
    return tree;
}

extern void set_tree(Tree *tree, void *key, void *value)
{
    if (tree->node == NULL)
    {
        tree->node = _new_node(tree->type.key, tree->type.value, key, value);
        return;
    }

    _set_tree(tree->node, tree->type.key, tree->type.value, key, value);

}

static tree_node * _new_node(vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value)
{
    tree_node *node = (tree_node*) malloc(sizeof(tree_node));
    
    _set_key(node, tkey, key);
    _set_value(node, tvalue, value);

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;

    return node;
}

static void _set_value(tree_node *node, vtype_tree_t tvalue, void *value)
{
    switch (tvalue) {
        case DECIMAL_ELEM:
            node->data.value.decimal = (int64_t) value;
            break;
        case REAL_ELEM:
            node->data.value.real = *(double *) value;
            free((double *) value);
            break;
        case STRING_ELEM:
            node->data.value.string = (uint8_t *) value;
            break;
    }
}

static void _set_key(tree_node *node, vtype_tree_t tkey, void *key)
{
    switch (tkey)
    {
        case DECIMAL_ELEM:
            node->data.key.decimal = (int64_t) key;
            break;
        case STRING_ELEM:
            node->data.key.string = (uint8_t *) key;
            break;
    }
}

static void _set_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue, void *key, void *value)
{
    int cond;
    switch (tkey)
    {
        case DECIMAL_ELEM:
            if ((int64_t) key > node->data.key.decimal)
            {
                if (node->right == NULL)
                {
                    node->right = _new_node(tkey, tvalue, key, value);
                    node->right->parent = node;
                } else 
                {
                    _set_tree(node->right, tkey, tvalue, key, value);
                }
            } else if ((int64_t) key < node->data.key.decimal)
            {
                if (node->left == NULL)
                {
                    node->left = _new_node(tkey, tvalue, key, value);
                    node->left->parent = node;
                } else 
                    _set_tree(node->left, tkey, tvalue, key, value);
            } else
                _set_value(node, tvalue, value);
            break;
        case STRING_ELEM:
            cond = strcmp((uint8_t *) key, node->data.key.string);
            if (cond > 0)
            {
                if (node->right == NULL)
                {
                    node->right = _new_node(tkey, tvalue, key, value);
                    node->right->parent = node;
                } else 
                {
                    _set_tree(node->right, tkey, tvalue, key, value);
                }
            } else if (cond < 0)
            {
                if (node->left == NULL)
                {
                    node->left = _new_node(tkey, tvalue, key, value);
                    node->left->parent = node;
                } else 
                    _set_tree(node->left, tkey, tvalue, key, value);
            } else 
                _set_value(node, tvalue, value);
           
            break;
    }
}

extern void * decimal(int64_t x)
{
    return (void *) x;
}

extern void * real(double x)
{
    double * ptr = (double *) malloc(sizeof(double));
    *ptr = x;

    return (void *) ptr;
}

extern void * string(uint8_t *x)
{
    return (void *) x;
}

extern void print_tree_as_list(Tree *tree)
{
    printf("[");
    _print_tree_as_list(tree->node, tree->type.key, tree->type.value);
    printf("]\n");
}

extern void print_tree(Tree *tree)
{
    _print_tree(tree->node, tree->type.key, tree->type.value);
    putchar('\n');
}

static void _print_tree_elem(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue)
{
    switch (tkey)
    {
        case DECIMAL_ELEM:
            printf("[%ld => ", node->data.key.decimal);
            switch (tvalue)
            {
                case DECIMAL_ELEM:
                    printf("%ld]", node->data.value.decimal);
                    break;
                case REAL_ELEM:
                    printf("%lf]", node->data.value.real);
                    break;
                case STRING_ELEM:
                    printf("%s]", node->data.value.string);
                    break; 
            } 
            break;
        case STRING_ELEM:
            printf("[%s => ", node->data.key.string);
            switch (tvalue)
            {
                case DECIMAL_ELEM:
                    printf("%ld]", node->data.value.decimal);
                    break;
                case REAL_ELEM:
                    printf("%lf]", node->data.value.real);
                    break;
                case STRING_ELEM:
                    printf("%s]", node->data.value.string);
                    break; 
            } 
            break;
    }
}

static void _print_tree(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue)
{
    if (node == NULL)
    {
        printf("null");
        return;
    }
    putchar('(');
    _print_tree(node->left, tkey, tvalue);
    _print_tree_elem(node, tkey, tvalue);
    _print_tree(node->right, tkey, tvalue);
    putchar(')');

}

static void _print_tree_as_list(tree_node *node, vtype_tree_t tkey, vtype_tree_t tvalue)
{
    if (node == NULL)
        return;
   
    _print_tree_as_list(node->left, tkey, tvalue);
    _print_tree_elem(node, tkey, tvalue);
    _print_tree_as_list(node->right, tkey, tvalue);

}   

extern void free_tree(Tree *tree)
{
    _free_tree(tree->node);
    free(tree);
}

static void _free_tree(tree_node *node)
{
    if (node == NULL)
        return;

    _free_tree(node->left);
    _free_tree(node->right);

    free(node);
}

extern value_tree_t get_tree(Tree *tree, void *key)
{
    tree_node *node = _get_tree(tree->node, tree->type.key, key);
    if (node == NULL)
    {
        fprintf(stderr, "%s\n", "value undefined"); 
        value_tree_t none;
        none.decimal = 0;

        return none;
    }

    return node->data.value;
}

static tree_node * _get_tree(tree_node *node, vtype_tree_t tkey, void * key)
{
    if (node == NULL)
        return NULL;

    int cond;
    switch (tkey) {
        case DECIMAL_ELEM:
            if ((int64_t) key > node->data.key.decimal)
               return _get_tree(node->right, tkey, key);
            else if ((int64_t) key < node->data.key.decimal) 
               return _get_tree(node->left, tkey, key); 
            break;
        case STRING_ELEM:
            cond = strcmp((uint8_t *) key, node->data.key.string);
            if (cond > 0)
                return _get_tree(node->right, tkey, key);
            else if (cond < 0)
                return _get_tree(node->left, tkey, key);
            break;
    }

    return node;
}

extern _Bool in_tree(Tree *tree, void *key)
{
    return _get_tree(tree->node, tree->type.key, key) != NULL; 
}

extern void del_tree(Tree *tree, void *key)
{
    tree_node *node = del1_tree(tree, tree->type.key, key);
    if (node == NULL)
        return;

    if (node->left != NULL && node->right != NULL)
    {
        _del3_tree(node);
        return;
    }    

    _del2_tree(tree, node);

}   

static tree_node * del1_tree(Tree *tree, vtype_tree_t tkey, void* key)
{
    tree_node *node = tree->node;
    node = _get_tree(node, tkey, key);
    
    if (node == NULL)
        return NULL;

    if (node->left != NULL || node->right != NULL)
    {
        return node;
    }

    tree_node *parent = node->parent;
    if (parent == NULL)
        tree->node = NULL;
    else if (parent->left == node)
        parent->left = NULL;
    else 
        parent->right = NULL;

    free(node);

    return NULL;

}

static void _del2_tree(Tree *tree, tree_node *node)
{
    tree_node *parent = node->parent;
    tree_node *temp;    
    
    if (node->right != NULL)
        temp = node->right;
    else 
        temp = node->left;

    if (parent == NULL)
        tree->node = temp;
    else if (parent->left == node)
        parent->left = temp;
    else 
        parent->right = temp;

    temp->parent = parent;

    free(node);
}

static void _del3_tree(tree_node *node)
{
    tree_node *ptr = node->right;
    
    while (ptr->left != NULL)
        ptr = ptr->left;

    node->data.key = ptr->data.key;
    node->data.value = ptr->data.value;
    tree_node *parent = ptr->parent;

    if (parent->left == ptr)
        parent->left = NULL;
    else
        parent->right = NULL;
    
    free(ptr);
}







