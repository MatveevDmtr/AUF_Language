#ifndef AUF_LANG_H_INCLUDED
#define AUF_LANG_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>

#include "D:\\Programming\\C\\Ded_course_1_sem\\Processor_v4\\logging.h"

#define StructTreeInit(name)                              \
        StructureTreeInit(#name,                          \
                           __PRETTY_FUNCTION__,           \
                           __FILE__,                      \
                           __LINE__);


//#define allocate_array(type, num_elems)                             \
({                                                                  \
    type* location__ = (type*) calloc (num_elems, sizeof (type));   \
    log ("Type_calloc: %s\n", #type);                               \
    log ("Sizeof type: %d\n", sizeof (type*));                      \
    Assert(location__ == NULL);                                     \
    log("Calloc returned: %p\n", location__);                       \
    location__;                                                     \
})

#define allocate_array(type, num_elems)                             \
({                                                                  \
    type* location__ = (type*) calloc (num_elems, sizeof (type));   \
    Assert(location__ == NULL);                                     \
    location__;                                                     \
})

// START DUMP  DSL

#define dumpline(...)  fprintf(dump_file, __VA_ARGS__);
#define dumphtml(...)  fprintf(html_file, __VA_ARGS__);
// FINISH DUMP DSL

#define MAX_LEN_ELEM 100// to insert in scanf
//start DSL


#define RecursiveRead2Ops(op1, op2, next_func)                         \
{                                                                      \
    elem_s* op_node = next_func;                                       \
                                                                       \
    if (tcode->Ptr[ip].type != T_OP)    return op_node;                \
                                                                       \
    if (tcode->Ptr[ip].value.op_v == op1 ||                            \
        tcode->Ptr[ip].value.op_v == op2)                              \
    {                                                                  \
        ip++;                                                          \
                                                                       \
        elem_s* l_node = op_node;                                      \
                                                                       \
        if (tcode->Ptr[ip - 1].value.op_v == op1)                      \
        {                                                              \
            op_node = NewOp(op1);                                      \
        }                                                              \
        else                                                           \
        {                                                              \
            op_node = NewOp(op2);                                      \
        }                                                              \
                                                                       \
        elem_s* r_node = next_func;                                    \
                                                                       \
        MakeSons(op_node, l_node, r_node);                             \
    }                                                                  \
                                                                       \
    return op_node;                                                    \
}

#define RecursiveRead1Op(op, next_func)                                \
{                                                                      \
    elem_s* op_node = next_func;                                       \
                                                                       \
    if (tcode->Ptr[ip].type != T_OP)    return op_node;                \
                                                                       \
    if (tcode->Ptr[ip].value.op_v == op)                               \
    {                                                                  \
        ip++;                                                          \
                                                                       \
        elem_s* r_node = next_func;                                    \
        elem_s* l_node = op_node;                                      \
                                                                       \
        op_node = NewOp(op);                                           \
                                                                       \
        MakeSons(op_node, l_node, r_node);                             \
    }                                                                  \
                                                                       \
    return op_node;                                                    \
}

#define MakeSons(node, left_son, right_son)                       \
{                                                                 \
    ConnectNodes(node, left_son,  LEFT);                          \
    ConnectNodes(node, right_son, RIGHT);                         \
}

#define NewOp(op)       CreateNode(NODE_OP, {.op_v  = op})

#define NewVal(number)  CreateNode(NODE_VAL, {.int_v = number})

#define NewVar(line)                                                        \
({                                                                          \
    elem_s* __node = CreateNode(NODE_VAR, {.op_v = NULL});                  \
    strcpy(__node->value.str_v, line);                                       \
    __node;                                                                 \
})

#define NewFunc(line)                                                       \
({                                                                          \
    elem_s* __node = CreateNode(NODE_FUNC, {.op_v = NULL});                 \
    strcpy(__node->value.str_v, line);                                       \
    __node;                                                                 \
})

#define UpdSon(new_node)                                                                   \
{                                                                                          \
    if      (node->parent == NULL)          {UpdRoot(new_node);}                           \
    else if (node->parent->left  == node)   ConnectNodes(node->parent, new_node, LEFT);    \
    else if (node->parent->right == node)   ConnectNodes(node->parent, new_node, RIGHT);   \
    else                                    print_log(FRAMED, "Invalid node Connection");  \
}

#define UpdRoot(new_node)                                \
{                                                        \
    tree->Ptr         = new_node;                        \
    tree->Ptr->parent = NULL;                            \
}

#define TexPrint(text...)    TexPrintf(texfile, text);
#define TexLeft              NodeToTex(texfile, node->left)
#define TexRight             NodeToTex(texfile, node->right)

#define InOrdCmd(str)                                         \
{                                                             \
    TexLeft;                                                  \
    TexPrint(str);                                            \
    TexRight;                                                 \
}

#define BraceInOrdTrigOp(str)                                 \
{                                                             \
    TexPrint(" {(");                                          \
    TexPrint(str);                                            \
    if (node->right->type == NODE_OP)   TexPrint("(");        \
    TexRight;                                                 \
    if (node->right->type == NODE_OP)   TexPrint(")");        \
    TexPrint("})} ");                                         \
}

#define NoBraceInOrdTrigOp(str)                               \
{                                                             \
    TexPrint(" {");                                           \
    TexPrint(str);                                            \
    if (node->right->type == NODE_OP)   TexPrint("(");        \
    TexRight;                                                 \
    if (node->right->type == NODE_OP)   TexPrint(")");        \
    TexPrint("}} ");                                          \
}

#define BraceDeg(str)                                         \
{                                                             \
    TexPrint("({");                                           \
    if (node->left->type == NODE_OP)   TexPrint("(");         \
    TexLeft;                                                  \
    if (node->left->type == NODE_OP)   TexPrint(")");         \
    TexPrint("}");                                            \
    TexPrint(str);                                            \
    TexRight;                                                 \
    TexPrint("})");                                           \
}

#define NoBraceDeg(str)                                       \
{                                                             \
    TexPrint("{");                                            \
    if (node->left->type == NODE_OP)   TexPrint("(");         \
    TexLeft;                                                  \
    if (node->left->type == NODE_OP)   TexPrint(")");         \
    TexPrint("}");                                            \
    TexPrint(str);                                            \
    TexRight;                                                 \
    TexPrint("}");                                            \
}

//#define TexTrigOp(str)                                                    \
{                                                                           \
    if (node->parent->type == NODE_OP &&                                    \
        node->parent->value.op_val == OP_DEG) {BraceInOrdTrigOp(str);}      \
    else                                      {NoBraceInOrdTrigOp(str);}    \
}

#define TexTrigOp(str)                                                      \
{                                                                           \
    NoBraceInOrdTrigOp(str);                                                \
}

//finish DSL

const size_t MAX_LEN_VAR_NAME   = 30;
const size_t MIN_LEN_TOKEN_CODE = 10;
const size_t MAX_LEN_TOKEN_STR  = 30;

const size_t TAB = 4;

typedef int elem_t;

enum ERRCODES
{
     OK                 , // 0
     SEGFAULT           , // 1
     ZOMBIE             , // 2
     NULLPTR            , // 3
     SIZEPOISONED       , // 4
     NEGCAP             , // 5
     DEBUGINFOERROR     , // 6
     REALLOCERROR       , // 7
     STACKOVERFLOW      , // 8
     FREE_ERROR         , // 9
     CHAINERROR         , // 10
     CONSTR_ERROR         // 11
};

enum ARYTHMETIC_OPERATIONS
{
    OP_ADD    = 'DDA',
    OP_SUB    = 'BUS',
    OP_MUL    = 'LUM',
    OP_DIV    = 'VID',
    OP_DEG    = 'GED',
    OP_SIN    = 'NIS',
    OP_COS    = 'SOC',
    OP_TG     = 'GT' ,
    OP_CTG    = 'GTC',
    OP_SH     = 'HS' ,
    OP_CH     = 'HC' ,
    OP_ARCSIN = 'NSA',
    OP_ARCCOS = 'SCA',
    OP_ARCTG  = 'GTA',
    OP_ARCCTG = 'GCA',
};

enum NUM_SONS
{
    LEFT  = 1,
    RIGHT = 2
};

enum NODE_TYPES
{
    NODE_OP   = 'PO',
    NODE_VAR  = 'RAV',
    NODE_VAL  = 'LAV',
    NODE_FUNC = 'CNUF',
};

union value_t
{
    char     str_v[MAX_LEN_TOKEN_STR];
    int      int_v;
    size_t   op_v;
};

typedef struct struct_elem
{
    size_t       type;

    value_t value;

    struct struct_elem* parent;
    struct struct_elem* left;
    struct struct_elem* right;
} elem_s;


typedef struct struct_def_node
{
    elem_s* Node;
    size_t  Branch;
} def_node;

typedef struct struct_queue
{
    def_node* Ptr;
    size_t    Head;
    size_t    Tail;

} queue_t;

typedef struct tree_info
{
    elem_s*  Ptr;
    elem_s*  Tail;
    size_t   Size;

    bool     DeadInside;
} tree_t;

elem_s* CreateNode(size_t type, union value_t value);

int ConnectNodes(elem_s* parent, elem_s* son, size_t num_son);

elem_s* Diff(elem_s* node);

elem_s* DiffOperation(elem_s* node, elem_s* dest_node);

elem_s* DiffAddSub(elem_s* node, elem_s* dest_node);

elem_s* DiffMul(elem_s* node, elem_s* dest_node);

elem_s* DiffDiv(elem_s* node, elem_s* dest_node);

elem_s* DiffDeg(elem_s* node, elem_s* dest_node);

elem_s* DiffSinCos(elem_s* node, elem_s* dest_node);

elem_s* DiffTgCtg(elem_s* node, elem_s* dest_node);

elem_s* CopyNode(elem_s* node);

int OneIterationSimplify(tree_t* tree, elem_s* node, bool* if_simple);

int CalculateConsts(tree_t* tree, elem_s* node, bool* if_simple);

int CheckForConst(tree_t* tree     , elem_s* node,
                  elem_s* son_check, elem_s* other_son,
                  bool* if_simple);

int CheckForAddSubConst(elem_s* node, elem_s* son_check, elem_s* other_son);

int SimplifyAddSub(elem_s* node);

int TreeSimplify(tree_t* tree);

elem_s* NewNodeForOpWithOne(size_t op_val, elem_s* other_son);

elem_s* NewNodeForOpWithZero(size_t op_val, elem_s* other_son);

int TreeToTex(FILE* texfile, tree_t* tree);

int NodeToTex(FILE* texfile, elem_s* node);

int GetNDeriv(FILE* texfile, tree_t* tree, size_t max_deriv);

int TaylorPrint(FILE* texfile, tree_t* tree, size_t max_deriv);

FILE* TexStart();

int TexFinish(FILE* texfile);

void TexPrintf(FILE* texfile, const char* format, ...);


int FreeNode(elem_s* node);

int FreeOneNode(elem_s* node);





tree_t StructureTreeInit(const char* name,
                           const char* func,
                           const char* file,
                           int line);

int TreeCtor(tree_t* tree);

void SpeakAndPrint(const char* str...);

int ReadNewNode(tree_t* tree, elem_s* node);

int WriteTree(elem_s* root);

int WriteNode(elem_s* node, FILE* treefile, size_t num_spaces);

void WriteSpaces(int num_spaces, FILE* treefile);

int WriteOp(FILE* treefile, size_t op);

int WriteVar(FILE* treefile, char* var);

int WriteVal(FILE* treefile, int val);

char ReadPrefix(FILE* db);

int InsertNode(tree_t* tree, elem_s* parent, char* ins_elem, size_t num_son);

int ConfigNode(elem_s** son_field, elem_s* parent, char* ins_elem);

int PrintQuality(queue_t* way_down, size_t i);

int PrintDif(const char* name, queue_t* way_down, size_t i_dif);

int PrintComparison(tree_t* tree, queue_t* way_down1, queue_t* way_down2);

int FindNode(tree_t* tree, elem_s* node, const char* name, queue_t* way_down, size_t branch);

int PrintDefinition(tree_t* tree, queue_t* way_down, const char* name);

int InsertSon(tree_t* tree, elem_s* parent, char* ins_elem, const char* quest, size_t num_son);

int InsertBeforeElem(tree_t* tree, size_t i_anc, elem_t elem);

int InsertAfterElem (tree_t* tree, size_t i_anc, elem_t elem);

int TreeVerify(tree_t* tree);

int TreeAppend(tree_t* tree, elem_t elem);

elem_t TreePop(tree_t* tree);

elem_t DeleteElem(tree_t* tree, size_t i_del);

void GraphTreeDump(const tree_t* tree, const char* picname);

int HTMLDump(const tree_t* tree, const char* occasion);

int LogCritError(int errcode, const char* func, int line);

int TreeRecalloc(tree_t* tree, size_t mode);

#endif //guard

