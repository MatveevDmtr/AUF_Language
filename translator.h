int ScanTree(tree_t* tree);

elem_s* ScanNode(tree_t* tree, elem_s* parent, FILE* treefile, size_t num_son);

size_t IdentifyOp(char* str);

const size_t MIN_LEN_TABVAR = 5;

typedef struct __variable
{
    char   name[MAX_LEN_ELEM];
    size_t memloc;
} var_t;

typedef struct __table_var
{
    var_t*  Table;
    size_t  Size;
    size_t  Capacity;
} tabvar_t;

int SearchVar(tabvar_t* tablevar, const char* varname);

int CountNode(tabvar_t* tablevar, elem_s* node);

int NewNameSpace(elem_s* node, elem_s* node_func);

int RunNode(tabvar_t* tablevar, elem_s* node);

int RunOp(tabvar_t* tablevar, elem_s* node);

int AsmCreateVar(tabvar_t* tablevar, elem_s* node_op);

int AsmAss(tabvar_t* tablevar, elem_s* node_ass);

int WritePushPopAsm(size_t func, size_t if_ram, int val, int reg);

int WriteCmdAsm(size_t cmd);

int AddVar(tabvar_t* tablevar, const char* varname);

int AsmInput(tabvar_t* tablevar, elem_s* node_inp);

int AsmOutput(tabvar_t* tablevar, elem_s* node_inp);

int AsmParams(tabvar_t* tablevar, elem_s* node);

int DumpTableVar(tabvar_t* tablevar);
