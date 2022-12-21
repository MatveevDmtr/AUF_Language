#include "lang_rec_des.h"
#include "auf_lang.h"
#include "translator.h"

const char*  INPUT_FILE_NAME  = "Tree.txt";
const char*  ASM_FILE_NAME    = "factorial_auf.txt";

static FILE* asmfile = fopen(ASM_FILE_NAME, "w");

static size_t ram_free = 10;

enum RETURNS
{
    NOT_FOUND   = -1,
    NO_REG      = -1,
    RAM_ARG     =  1,
    NORAM_ARG   = -1,
    REG_FOR_RET = 0,
    FUNC        = 1,
    NO_FUNC     = 0,
};

enum PROCESSOR_CMDS
{
    CMD_PUSH = 'HSUP',
    CMD_POP  = 'POP',
    CMD_IN   = 'NI',
    CMD_OUT  = 'TUO',
    CMD_HLT  = 'TLH',
    CMD_ADD  = 'DDA',
    CMD_SUB  = 'BUS',
    CMD_MUL  = 'LUM',
    CMD_DIV  = 'VID',
    CMD_DEG  = 'GED',
    CMD_RET  = 'TER',
    CMD_JB   = 'BJ',
    CMD_JA   = 'AJ',
    CMD_JBE  = 'EBJ',
    CMD_JAE  = 'EAJ',
    CMD_JE   = 'EJ',
    CMD_JNE  = 'ENJ',
    //CMD_JB   = 'BJ',
};

int main()
{
    tree_t* tree = CreateTree();

    log("tree made, start writing\n");

    WriteTree(tree->Ptr);

    tree_t tree_copy = StructTreeInit(tree_copy);

    tree_copy.Ptr = allocate_array(elem_s, 1);

    ScanTree(&tree_copy);

    HTMLDump(&tree_copy, "Scanned tree");

    NewNameSpace(tree_copy.Ptr, NULL);

    fclose(asmfile);

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

            sscanf(line, " \"%[a-zA-Zà-ÿÀ-ß_]\" %n", var, &num_read_sym);

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

    if (node->type == NODE_VAR &&
        node->left->type == NODE_OP &&
        node->left->value.op_v == OP_PARAM)
    {
        node->type = NODE_FUNC;
    }

    return node;
}

size_t IdentifyOp(char* str)
{
    size_t op_v = OP_STM;

    if (!strcmp((const char*)&op_v, str))               return OP_STM;

    CheckStrFunc(OP_NEWVAR);
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
    CheckStrFunc(OP_OUTPUT);
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


int NewNameSpace(elem_s* node, elem_s* node_func)
{
    if (!node_func)     DumpAsm("CALL main\n");

    ram_free += MIN_LEN_TABVAR;

    tabvar_t tablevar = {.Table    = allocate_array(var_t, MIN_LEN_TABVAR),
                         .Size     = 0,
                         .Capacity = MIN_LEN_TABVAR};

    if (node_func)  RunOp(&tablevar, node_func);

    RunNode(&tablevar, node);

    ram_free -= tablevar.Capacity; // before return

    free(tablevar.Table);

    //if (node_func) log("node_func: %.4s, %.4s\n", &node_func->type, &node_func->value.op_v);

    if (node_func && !strcmp(node_func->parent->value.str_v, "main"))    WriteCmdAsm(CMD_HLT);

    if (node_func)
    {
        WriteCmdAsm(CMD_RET);
        DumpAsm("\n\n");
    }

    return 0;
}

int RunNode(tabvar_t* tablevar, elem_s* node)
{
    if (node->type == NODE_OP)
    {
        RunOp(tablevar, node);
    }
    /*else if (node->type == NODE_FUNC)
    {
        if (node->parent->type == NODE_OP &&
            node->parent->value.op_v == OP_FUNC)
            {
                DumpAsm("lbl %s\n", node->value.str_v);

                NewNameSpace(node);
            }
    }*/
    /*else
    {
        if (node->left)      RunNode(tablevar, node->left);
        if (node->right)     RunNode(tablevar, node->right);
    }*/
}

int DumpTableVar(tabvar_t* tablevar)
{
    log("\n\n++++++++++TableVar Dump++++++++++\n\n");

    log("Size     = %d\n",   tablevar->Size);
    log("Capacity = %d\n\n", tablevar->Capacity);

    for (size_t i = 0; i < tablevar->Capacity; i++)
    {
        log("name: %s, memloc: %d \n", tablevar->Table[i].name, tablevar->Table[i].memloc);
    }

    log("\n----------TableVar Dump----------\n\n");
}

int RunOp(tabvar_t* tablevar, elem_s* node)
{
    static int label_num      = 0;
    static int curr_if_lbl    = 0;
    static int curr_while_lbl = 0;

    DumpTableVar(tablevar);

    if (node->type == NODE_FUNC)
    {
        log("FUNC found\n");

        if (node->parent->type != NODE_OP ||
           node->parent->value.op_v != OP_CALL)
        {
            DumpAsm("\n\nname %s %d\n", node->value.str_v, label_num);
            DumpAsm("lbl %s\n", node->value.str_v);
            label_num++;

            NewNameSpace(node->parent->right, node->left);
        }
        return 0;
    }
    else if (node->type == NODE_VAL)
    {
        DumpAsm("push %d\n", node->value.int_v);
        return 0;
    }
    else if (node->type == NODE_VAR)
    {
        WritePushPopAsm(CMD_PUSH, RAM_ARG, SearchVar(tablevar, node->value.str_v), NO_REG);
        return 0;
    }

    switch(node->value.op_v)
    {
        case OP_STM:
            RunOp(tablevar, node->left);
            if (node->right) RunOp(tablevar, node->right);
            break;

        case OP_NEWVAR:
            AsmCreateVar(tablevar, node);
            break;

        case OP_FUNC:
            RunOp(tablevar, node->left);
            break;

        case OP_ASS:
            AsmAss(tablevar, node);
            break;

        case OP_INPUT:
            AsmInput(tablevar, node);
            break;

        case OP_OUTPUT:
            AsmOutput(tablevar, node);
            break;

        case OP_IF:
            RunSons(node);
            break;

        case OP_ELSE:
            //label_num++;

            RunOp(tablevar, node->left);

            DumpAsm("jump end_if%d\n", curr_if_lbl);

            WriteLabel("if_false", curr_if_lbl, label_num);

            RunOp(tablevar, node->right);

            WriteLabel("end_if", curr_if_lbl, label_num);

            curr_if_lbl++;

            break;

        case OP_WHILE:

            //label_num++;

            WriteLabel("while", curr_while_lbl, label_num);

            RunOp(tablevar, node->left);

            RunOp(tablevar, node->right);

            DumpAsm("jump while%d\n", curr_while_lbl);

            WriteLabel("end_while", curr_while_lbl, label_num);

            curr_while_lbl++;

            break;

        case OP_AND:
            log("op AND %.4s\n", &node->value.op_v);
            RunSons(node);
            WriteCmdAsm(CMD_ADD);
            break;

        case OP_OR:
            RunSons(node);
            WriteCmdAsm(CMD_MUL);
            break;

        case OP_BIGGER:
            RunSonsRev(node);
            JumpFromCond("jbe");
            break;

        case OP_LESS:
            RunSonsRev(node);
            JumpFromCond("jae");
            break;

        case OP_NBIGGER:
            RunSonsRev(node);
            JumpFromCond("jb");
            break;

        case OP_NLESS:
            RunSonsRev(node);
            JumpFromCond("ja");
            break;

        case OP_EQ:
            RunSonsRev(node);
            three_equals_five;
            JumpFromCond("jne");
            break;

        case OP_NEQ:
            RunSonsRev(node);
            JumpFromCond("je");
            break;

        case OP_ADD:
            log("op ADD\n");
            RunSons(node);
            WriteCmdAsm(CMD_ADD);
            break;

        case OP_SUB:
            RunSons(node);
            WriteCmdAsm(CMD_SUB);
            break;

        case OP_MUL:
            RunSons(node);
            log("try to MUL");
            WriteCmdAsm(CMD_MUL);
            break;

        case OP_DIV:
            RunSons(node);
            WriteCmdAsm(CMD_DIV);
            break;

        case OP_DEG:
            RunSons(node);
            WriteCmdAsm(CMD_DEG);
            break;

        case OP_PARAM:
            AsmParams(tablevar, node);
            break;

        case OP_CALL:
            RunOp(tablevar, node->left->left);
            DumpAsm("CALL %s\n", node->left->value.str_v);
            //RunOp(tablevar, node->left);
            break;

        case OP_RET:
            RunOp(tablevar, node->left);
            //WriteCmdAsm(CMD_RET);
            break;

        case OP_NIL:
            break;

        default:
            log("Operation: %.4s\n", &node->value.op_v);
            print_log(FRAMED, "Unexpected operation");
    }
}

int AsmParams(tabvar_t* tablevar, elem_s* node)
{
    elem_s* node_op = node;


    if(node->parent->parent->type == NODE_OP &&
       node->parent->parent->value.op_v == OP_CALL)
    {
        while (node_op &&
               node_op->type == NODE_OP &&
               node_op->value.op_v == OP_PARAM)
        {
            if (node_op->left)
            {
                if (node_op->left->type == NODE_VAR)
                {
                    int ram_loc = SearchVar(tablevar, node_op->left->value.str_v);

                    WritePushPopAsm(CMD_PUSH, RAM_ARG, ram_loc, NO_REG);
                }
                else if (node_op->left->type == NODE_OP)
                {
                    RunOp(tablevar, node->left);
                }
                else if (node_op->left->type == NODE_VAL)
                {
                    WritePushPopAsm(CMD_PUSH, NORAM_ARG, node_op->left->value.int_v, NO_REG);
                }
                else
                {
                    if (node_op->parent->type == NODE_OP &&
                        node_op->parent->value.op_v == OP_PARAM)
                        LogError(INVALID_ARG_ERROR);
                }
            }
            node_op = node_op->right;
        }
    }
    else if (node->parent->parent->type == NODE_OP &&
             node->parent->parent->value.op_v == OP_FUNC)
    {// add check!

        while (node_op &&
               node_op->type == NODE_OP &&
               node_op->value.op_v == OP_PARAM)
        {
            if (node_op->left)
            {
                if (node_op->left->type == NODE_VAR)
                {
                    int ram_loc = AddVar(tablevar, node_op->left->value.str_v);

                    WritePushPopAsm(CMD_POP, RAM_ARG, ram_loc, NO_REG);
                }
                else
                {
                    if (node_op->parent->type == NODE_OP &&
                        node_op->parent->value.op_v == OP_PARAM)
                        LogError(INVALID_ARG_ERROR);
                }
            }
            node_op = node_op->right;
        }
    }
}

int CompareToJumps(size_t op)
{
    switch(op)
    {
        case OP_BIGGER:
            return CMD_JA;

        case OP_LESS:
            return CMD_JB;

        case OP_NBIGGER:
            return CMD_JBE;

        case OP_NLESS:
            return CMD_JAE;

        case OP_EQ:
            return CMD_JE;

        case OP_NEQ:
            return CMD_JNE;

        default:
            print_log(FRAMED, "Error: not a compare operation");
    }

    return 0;
}

int CountBoolExpr(tabvar_t* tablevar, elem_s* node)
{
    //int l_node = NULL;
    //int r_node = NULL;

    size_t comp_cmd = 0;

    if      (node->type == NODE_OP &&
            (node->value.op_v == OP_AND ||
             node->value.op_v == OP_OR))
    {

        CountBoolExpr(tablevar, node->left);
        CountBoolExpr(tablevar, node->right);

        if (node->value.op_v == OP_AND)     WriteCmdAsm(CMD_MUL);
        else                                WriteCmdAsm(CMD_ADD);
    }
    else if (node->type == NODE_OP &&
             !(comp_cmd = CompareToJumps(node->value.op_v)))
    {
        CountBoolExpr(tablevar, node->left);
        CountBoolExpr(tablevar, node->right);


    }
    else if (node->type == NODE_VAR)
    {
        WritePushPopAsm(CMD_PUSH, RAM_ARG, SearchVar(tablevar, node->value.str_v), NO_REG);
    }
    else if (node->type == NODE_VAL)
    {
        WritePushPopAsm(CMD_PUSH, NORAM_ARG, node->value.int_v, NO_REG);
    }
    else
    {
        print_log(FRAMED, "Unexpected type of node in CountNode");
    }

    return 0;
}

int AddVar(tabvar_t* tablevar, const char* varname)
{
    if (tablevar->Size >= tablevar->Capacity)
    {
        ;//Recalloc
    }

    //if (SearchVar(tablevar, varname) != NOT_FOUND)      LogError(MULT_DEF_ERROR);

    tablevar->Table[tablevar->Size] = {.memloc = ram_free - tablevar->Capacity + tablevar->Size};

    strcpy(tablevar->Table[tablevar->Size].name, varname);

    tablevar->Size++;

    return tablevar->Table[tablevar->Size - 1].memloc;
}

int WriteCmdAsm(size_t cmd)
{
    DumpAsm("%.4s\n", &cmd);
}

int WritePushPopAsm(size_t func, size_t if_ram, int val, int reg)
{
    if (func == CMD_PUSH)       {DumpAsm("push ");}
    else                        {DumpAsm("pop ");}

    if (if_ram == RAM_ARG)      {DumpAsm("[");}

    if (reg != NO_REG)
    {
        char str_reg[4] = "rax";
        str_reg[1] += reg;

        DumpAsm("%s", str_reg);
    }
    else                        {DumpAsm("%d", val);}

    if (if_ram == RAM_ARG)      {DumpAsm("]");}

    DumpAsm("\n");

    if (func == CMD_POP &&
        if_ram != RAM_ARG &&
        reg == NO_REG)           print_log(FRAMED, "CANNOT POP TO A NUMBER");

    return 0;
}

int AsmAss(tabvar_t* tablevar, elem_s* node_ass)
{
    elem_s* node_dest = node_ass->left;

    int ram_loc = SearchVar(tablevar, node_dest->value.str_v);

    RunOp(tablevar, node_ass->right);

    WritePushPopAsm(CMD_POP, RAM_ARG, ram_loc, NO_REG);

    return 0;
}

int AsmCreateVar(tabvar_t* tablevar, elem_s* node_op)
{
    elem_s* node_var = node_op->left;

    int ram_loc = AddVar(tablevar, node_var->value.str_v);

    RunOp(tablevar, node_op->right);

    WritePushPopAsm(CMD_POP, RAM_ARG, ram_loc, NO_REG);

    return 0;
}

int CountNode(tabvar_t* tablevar, elem_s* node)
{
    //int l_node = NULL;
    //int r_node = NULL;

    if      (node->type == NODE_OP &&
             node->value.op_v == OP_CALL)
    {
        DumpAsm("CALL %s\n", node->left->value.str_v);

        WritePushPopAsm(CMD_PUSH, NORAM_ARG, NULL, REG_FOR_RET);
    }
    else if (node->type == NODE_OP)
    {
        CountNode(tablevar, node->left);
        CountNode(tablevar, node->right);
        //operation
    }
    else if (node->type == NODE_VAR)
    {
        WritePushPopAsm(CMD_PUSH, RAM_ARG, SearchVar(tablevar, node->value.str_v), NO_REG);
    }
    else if (node->type == NODE_VAL)
    {
        WritePushPopAsm(CMD_PUSH, NORAM_ARG, node->value.int_v, NO_REG);
    }
    else
    {
        print_log(FRAMED, "Unexpected type of node in CountNode");
    }

    return 0;
}


int AsmInput(tabvar_t* tablevar, elem_s* node_inp)
{
    elem_s* node_op = node_inp->left;

    while (node_op &&
           node_op->type == NODE_OP &&
           node_op->value.op_v == OP_PARAM)
    {
        if (node_op->left)
        {
            WriteCmdAsm(CMD_IN);

            if (node_op->left->type != NODE_VAR)   LogError(INVALID_ARG_ERROR);

            int ram_loc = SearchVar(tablevar, node_op->left->value.str_v);

            WritePushPopAsm(CMD_POP, RAM_ARG, ram_loc, NO_REG);
        }
        node_op = node_op->right;
    }

    return 0;
}

int AsmOutput(tabvar_t* tablevar, elem_s* node_out)
{
    elem_s* node_op = node_out->left;

    while (node_op &&
           node_op->type == NODE_OP &&
           node_op->value.op_v == OP_PARAM)
    {
        if (node_op->left)
        {
            CountNode(tablevar, node_op->left);

            WriteCmdAsm(CMD_OUT);
        }
        node_op = node_op->right;
    }

    return 0;
}

int SearchVar(tabvar_t* tablevar, const char* varname)
{
    for (size_t i = 0; i < tablevar->Size; i++)
    {
        if (!strcmp(tablevar->Table[i].name, varname))  return ram_free - tablevar->Capacity + i;
    }

    LogError(UNDECLARED_VAR_ERROR);

    return NOT_FOUND;
}
