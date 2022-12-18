int ScanTree(tree_t* tree);

elem_s* ScanNode(tree_t* tree, elem_s* parent, FILE* treefile, size_t num_son);

size_t IdentifyOp(char* str);

typedef struct __table_var
{
    char**  Table;
    size_t  Size;
} tvar_t;
