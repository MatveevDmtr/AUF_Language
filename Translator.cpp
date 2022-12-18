#include "lang_rec_des.h"
#include "auf_lang.h"
#include "translator.h"

const char*  INPUT_FILE_NAME  = "Tree.txt";

int main()
{
    tree_t* tree = CreateTree();

    log("tree made, start writing\n");

    WriteTree(tree->Ptr);

    tree_t tree_copy = StructTreeInit(tree_copy);

    tree_copy.Ptr = allocate_array(elem_s, 1);

    ScanTree(&tree_copy);

    HTMLDump(&tree_copy, "Scanned tree");

    return 0;
}


int ScanTree(tree_t* tree)
{
    log("Start scanning tree file\n");

    Assert(tree->Ptr == nullptr);

    FILE* treefile = fopen(INPUT_FILE_NAME, "r");

    if (treefile == NULL)
    {
        print_log(FRAMED, "Can't open tree file\n");

        return -1;
    }

    tree->Ptr = ScanNode(tree, NULL, treefile, LEFT);

    fclose(treefile);

    log("Finish scanning tree file\n");

    return 0;
}

elem_s* ScanNode(tree_t* tree, elem_s* parent, FILE* treefile, size_t num_son)
{
    log("\n\nstart scanning node\n");

    Assert(treefile == nullptr);

    elem_s* node = NULL;

    bool has_sons = false;

    char line[MAX_LEN_ELEM] = "";

    int num_read_sym = 0;

    fscanf(treefile, " { %s ", line);

    log("read line 1: %s\n", line);

    fscanf(treefile, " }%n ", &num_read_sym);

    if (!num_read_sym)
    {
        has_sons = true;
        log("has sons\n");
    }

    log("read line 2: %s\n", line);

    int op_v = 0;

    if(op_v = IdentifyOp(line))
    {
        node = NewOp(op_v);
        log("new op is ready\n");
    }
    else
    {
        //log("start writing var or val\n");
        int val = 0;

        log("num read sym before val = %d\n", num_read_sym);

        num_read_sym = 0;

        sscanf(line, "%d%n", &val, &num_read_sym);

        log("num read sym in val = %d\n", num_read_sym);

        node = NewVal(val);

        if (!num_read_sym)
        {
            char var[MAX_LEN_ELEM] = "";

            sscanf(line, " \"%[à-ÿÀ-ß_]\" %n", var, &num_read_sym);

            if (!num_read_sym)
            {
                LogError(READNODE_ERROR);

                log("ERROR: line: %s var: %s\n", line, var);
            }

            node = NewVar(var);
        }
    }
    Assert(node == NULL);

    node->parent = parent;

    tree->Size++;

    if (!has_sons) return node;

    node->left  = ScanNode(tree, node, treefile, LEFT);
    node->right = ScanNode(tree, node, treefile, RIGHT);

    fscanf(treefile, " }%n ", &num_read_sym);

    if (node->left->type == NODE_OP && node->left->value.op_v == OP_PARAM)
    {
        node->type = NODE_FUNC;
    }

    return node;
}

size_t IdentifyOp(char* str)
{
    size_t op_v = OP_STM;

    if (!strcmp((const char*)&op_v, str))               return OP_STM;

    CheckStrFunc(OP_ASS);
    CheckStrFunc(OP_IF);
    CheckStrFunc(OP_ELIF);
    CheckStrFunc(OP_ELSE);
    CheckStrFunc(OP_WHILE);
    CheckStrFunc(OP_VOID);
    CheckStrFunc(OP_TYPE);
    CheckStrFunc(OP_RET);
    CheckStrFunc(OP_BIGGER);
    CheckStrFunc(OP_LESS);
    CheckStrFunc(OP_NBIGGER);
    CheckStrFunc(OP_NLESS);
    CheckStrFunc(OP_EQ);
    CheckStrFunc(OP_NEQ);
    CheckStrFunc(OP_AND);
    CheckStrFunc(OP_OR);
    CheckStrFunc(OP_INPUT);
    CheckStrFunc(OP_COMMA);
    CheckStrFunc(OP_PARAM);
    CheckStrFunc(OP_FUNC);
    CheckStrFunc(OP_CALL);
    CheckStrFunc(OP_NIL);
    //op_v = OP_NIL;
    //if (!strcmp((const char*)&op_v, str))               return -1;

    CheckStrFunc(OP_ADD);
    CheckStrFunc(OP_SUB);
    CheckStrFunc(OP_MUL);
    CheckStrFunc(OP_DIV);
    CheckStrFunc(OP_DEG);
    CheckStrFunc(OP_SIN);
    CheckStrFunc(OP_COS);
    CheckStrFunc(OP_TG);
    CheckStrFunc(OP_CTG);

    return 0;
}
