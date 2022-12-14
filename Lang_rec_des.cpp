//#include <TXLib.h>
#include <stdio.h>
#include <string.h>


#include "auf_lang.h"
#include "lang_rec_des.h"

#include "D:\\Programming\\C\\Ded_course_1_sem\\Processor_v4\\GetPoison.h"

#include "D:\\Programming\\C\\Ded_course_1_sem\\Stack\\specificators.h"

const char*  DUMP_FILE_NAME      = "dump.dot";
const char*  PICNAME             = "graphCode";
const char*  FILE_EXTENSION      = ".png";
const char*  HTML_FILE_NAME      = "htmldump.html";
const size_t MAX_LEN_PICNAME     = 20;
const size_t MAX_LEN_CONSOLE_CMD = 70;
const size_t MAX_LEN_NUM_DUMP    = 10;

static size_t ip = 0; //instruction pointer

int SearchVar(char* var);

token_code_t* tcode = NULL;

static int curr_level = -1;

int main()
{
    printf("start\n");

    tcode = Tokenize();

    tree_t tree = StructTreeInit(tree);

    TreeCtor(&tree);

    MakeSyntaxTree(&tree);

    HTMLDump(&tree, "EXAMPLE");
}

int MakeSyntaxTree(tree_t* tree)
{
    log("start making syntax tree\n");

    elem_s* curr_branch = GetCodeBlock(); //!

    tree->Ptr  = curr_branch;
    tree->Tail = curr_branch;

    /*while (ip < tcode->Size)
    {
        curr_branch = GetTrunkBranch();

        ConnectNodes(tree->Tail, curr_branch, RIGHT);

        tree->Tail = curr_branch;
    }*/

    return 0;
}

tree_t StructureTreeInit(const char* name,
                         const char* func,
                         const char* file,
                         int line)
{
    void*              Ptr             = NULL;
    size_t             Size            = 0;

    tree_t temp_tree =
    {
        .Ptr               = (elem_s*) getPoison(Ptr),
        .Tail              = NULL,
        .Size              = getPoison(Size),
        .DeadInside        = 1
    };

    return temp_tree;
}

int TreeCtor(tree_t* tree)
{
    static size_t num_dump = 0;

    if (tree == NULL || tree == getPoison(tree))
    {
        LogError(SEGFAULT);

        return SEGFAULT;
    }

    if (!(tree->DeadInside))
    {
        LogError(CONSTR_ERROR);

        return CONSTR_ERROR;
    }

    void* temp_ptr = (elem_s*) calloc(1, sizeof(elem_s));

    if (temp_ptr == NULL)
    {
        LogError(REALLOCERROR);

        return REALLOCERROR;
    }

    tree->Ptr                = (elem_s*)(temp_ptr);
    tree->Tail               = (elem_s*)(temp_ptr);

    tree->DeadInside         = 0;
    tree->Size               = 0;

    return 0;
}

elem_s* GetTrunkBranch()
{
    elem_s* new_node = GetIf();

    if (new_node == NULL)
    {
        log("NULL in GetTrunkBranch()\n");
        return NULL;
    }

    elem_s* node_stm = NewOp(OP_STM);

    ConnectNodes(node_stm, new_node, LEFT); //GetAss is temporary

                                            //the highest func in rec des
    log("finish GetTrunkBranch()\n");

    return node_stm;
}

int GetNewLine()
{
    log("start NewLine: ip: %zd, tcode->Size: %zd\n", ip, tcode->Size);

    if (ip >= tcode->Size)                  return -1;
    if (tcode->Ptr[ip].type != T_NEWLINE)   return -1;

    ip++;
    log("ip++: %d, curr level: %d\n", tcode->Ptr[ip - 1].value.int_v, curr_level);
    return tcode->Ptr[ip - 1].value.int_v;
}

elem_s* GetCodeBlock()
{
    log("start CodeBlock\n");
    curr_level++;

    elem_s* action     = NULL;
    elem_s* block_root = GetTrunkBranch();

    log("block root: %p\n", block_root);

    if (!block_root)    return NewOp(OP_STM);

    elem_s* block_tail = block_root;

    log("before cycle in CodeBlock\n");

    while (ip < tcode->Size && GetNewLine() >= (int)curr_level)
    {
        log("iteration in CodeBlock\n");

        action = GetTrunkBranch();

        log("found action: %p, type: %.4s\n", action, &action->value.op_v);
        log("block tail: %p", block_tail);

        if (action == NULL) continue;

        ConnectNodes(block_tail, action, RIGHT);

        block_tail = action;

        log("end iteration\n");
        log("curr_level: %d\n", curr_level);
    }

    curr_level--;

    log("level-: %d\n", curr_level);
    log("NLN: %d\n", tcode->Ptr[ip - 1].value.int_v);
    log("block root: %p\n", block_root);

    ip--; //because newline was already scanned

    return block_root;
}

elem_s* GetAss()
{
    log("start GetAss\n");

    if (tcode->Ptr[ip].type       != T_OP ||
        tcode->Ptr[ip].value.op_v != OP_ASS)
    {
        log("found %.4s (%s) instead of ASS\n", &tcode->Ptr[ip].type, &tcode->Ptr[ip].value.str_v);

        return NULL;
    }

    ip++;

    elem_s* node_ass = NewOp(OP_ASS);

    elem_s* l_node = GetVariable();

    log("got variable in GetAss()\n");

    elem_s* r_node = GetE();

    log("content of l_node: %s\n", l_node->value.str_v);

    MakeSons(node_ass, l_node, r_node);

    log("finish GetAss()\n");

    return node_ass;
}

elem_s* GetBoolExpr()
{
    RecursiveRead1Op(OP_OR, GetAnd());
}

elem_s* GetCompare()
{
    RecursiveRead2Ops(OP_BIGGER, OP_LESS, GetE());
}

elem_s* GetAnd()
{
    RecursiveRead1Op(OP_AND, GetBoolP());
}

elem_s* GetBoolP()
{
    elem_s* node = NULL;

    if (tcode->Ptr[ip].type == T_BRACE &&
        tcode->Ptr[ip].value.int_v == BR_OPEN)
    {
        node = GetBoolExpr();

        if (tcode->Ptr[ip].type == T_BRACE &&
        tcode->Ptr[ip].value.int_v == BR_CLOSE)
        {
            printf("found closing bracket\n");
        }
        else
        {
            printf("syntax error: closing bracket not found\n");
        }

        ip++;
    }
    else
    {
        node = GetCompare();
    }

    return node;
}

elem_s* GetIf()
{
    if (tcode->Ptr[ip].type       != T_OP ||
        tcode->Ptr[ip].value.op_v != OP_IF)
    {
        log("returning GetAss in GetIf\n");

        return GetAss();
    }

    ip++;

    elem_s* node_ass = NewOp(OP_IF);

    elem_s* l_node = GetBoolExpr();

    GetNewLine();

    elem_s* r_node = GetCodeBlock();

    log("content of l_node in GetIf: %s\n", l_node->value.str_v);

    MakeSons(node_ass, l_node, r_node);

    return node_ass;
}


elem_s* GetVariable()
{
    log("start GetVariable\n");

    if (tcode->Ptr[ip].type != T_STR)   return NULL;

    ip++;

    return NewVar(tcode->Ptr[ip - 1].value.str_v);
}

int SearchVar(char* var)
{
    /*for (size_t i = 0; i < table_var->free; i++)
    {
        if(!strcmp(var, table_var->table[i]))           return i;
    }*/

    return -1;
}

elem_s* CreateNode(size_t type, union value_t value)
{
    elem_s* temp_ptr = allocate_array(elem_s, 1);

    *temp_ptr = {.type  = type,
                .value  = value,
                .parent = NULL,
                .left   = NULL,
                .right  = NULL};

    return temp_ptr;
}

int ConnectNodes(elem_s* parent, elem_s* son, size_t num_son)
{
    switch(num_son)
    {
        case LEFT:
            parent->left  = son;
            break;

        case RIGHT:
            parent->right = son;
            break;

        default:
            print_log(FRAMED, "Invalid number of son");
    }

    son->parent = parent;

    return 0;
}
//Differ
/*int GetG(tree_t* tree, const char* str)
{
    s = str;

    elem_s* root = GetE();

    if (ip == tcode->Size)
    {
        printf("The end\n");
    }
    else
    {
        printf("Syntax error: %c\n", *s);
    }

    tree->Ptr = root;

    return 0;
}*/

elem_s* GetN()
{
    if (tcode->Ptr[ip].type == T_VAL)
    {
        ip++;
        return NewVal(tcode->Ptr[ip - 1].value.int_v);
    }

    return GetVarsAndFuncs();
}

/*elem_s* GetTrig()
{
    elem_s* op_node = GetDeg();

    size_t op_val = 0;

    while (op_val = TypeOfTrigF(&s))
    {
        elem_s* r_node = GetDeg();
        elem_s* l_node = NewNum(0);

        op_node = NewOp(op_val);

        ConnectNodes(op_node, l_node, LEFT);
        ConnectNodes(op_node, r_node, RIGHT);
    }

    return op_node;
}

size_t TypeOfTrigF(const char** s) //make macros for each
{
    if      (!strncmp(*s, "sin", 3))
    {
        *s += 3;
        return OP_SIN;
    }
    else if (!strncmp(*s, "cos", 3))
    {
        *s += 3;
        return OP_COS;
    }
    else if (!strncmp(*s, "tg", 2))
    {
        *s += 2;
        return OP_TG;
    }
    else if (!strncmp(*s, "ctg", 3))
    {
        *s += 3;
        return OP_CTG;
    }
    else if (!strncmp(*s, "sh", 2))
    {
        *s += 2;
        return OP_SH;
    }
    else if (!strncmp(*s, "ch", 2))
    {
        *s += 2;
        return OP_CH;
    }
    else if (!strncmp(*s, "arcsin", 6))
    {
        *s += 6;
        return OP_ARCSIN;
    }
    else if (!strncmp(*s, "arccos", 6))
    {
        *s += 6;
        return OP_ARCCOS;
    }
    else if (!strncmp(*s, "arctg", 5))
    {
        *s += 5;
        return OP_ARCTG;
    }
    else if (!strncmp(*s, "arcctg", 6))
    {
        *s += 6;
        return OP_ARCCTG;
    }
    else    return 0;
}
*/
elem_s* GetVarsAndFuncs()
{
    if (tcode->Ptr[ip].type != T_STR)
    {
        printf("Syntax Error: Can't read var of func name\n");//SYNTAX ERROR
        //return CreateNode(NODE_VAR, {.var_val = 'x'});
        return NULL;
    }
    if (tcode->Ptr[ip + 1].type == T_BRACE && tcode->Ptr[ip + 1].value.int_v == BR_OPEN)
    {
        ip++;

        return NewFunc(tcode->Ptr[ip - 1].value.str_v); //MUST CONNECT ARGS
    }

    ip++;
    return NewVar(tcode->Ptr[ip - 1].value.str_v);
}

elem_s* GetDeg()
{
    log("start GetDeg()\n");

    elem_s* op_node = GetP();

    if (tcode->Ptr[ip].type == T_OP && tcode->Ptr[ip].value.op_v == OP_DEG)
    {
        elem_s* r_node = GetP();
        elem_s* l_node = op_node;

        op_node = NewOp(OP_DEG);

        MakeSons(op_node, l_node, r_node);
    }

    return op_node;
}

elem_s* GetE()//not remade
{
    log("start GetE()\n");

    elem_s* op_node = GetT();

    if (tcode->Ptr[ip].type != T_OP)    return op_node;

    if (tcode->Ptr[ip].value.op_v == OP_ADD ||
        tcode->Ptr[ip].value.op_v == OP_SUB)
    {
        ip++;

        elem_s* r_node = GetT();
        elem_s* l_node = op_node;

        if (tcode->Ptr[ip - 1].value.op_v == OP_ADD)
        {
            op_node = NewOp(OP_ADD);
        }
        else
        {
            op_node = NewOp(OP_SUB);
        }

        MakeSons(op_node, l_node, r_node);
    }

    return op_node;
}

elem_s* GetT()
{
    log("start GetT()\n");

    elem_s* op_node = GetDeg();

    if (tcode->Ptr[ip].type != T_OP)    return op_node;

    if (tcode->Ptr[ip].value.op_v == OP_MUL ||
        tcode->Ptr[ip].value.op_v == OP_DIV)
    {
        ip++;

        elem_s* r_node = GetDeg();
        elem_s* l_node = op_node;

        if (tcode->Ptr[ip - 1].value.op_v == OP_MUL)
        {
            op_node = NewOp(OP_MUL);
        }
        else
        {
            op_node = NewOp(OP_DIV);
        }

        MakeSons(op_node, l_node, r_node);
    }

    return op_node;
}

elem_s* GetP()
{
    log("start GetP()\n");

    elem_s* node = NULL;

    if (tcode->Ptr[ip].type == T_BRACE &&
        tcode->Ptr[ip].value.int_v == BR_OPEN)
    {
        node = GetE();

        if (tcode->Ptr[ip].type == T_BRACE &&
        tcode->Ptr[ip].value.int_v == BR_OPEN)
        {
            printf("found closing bracket\n");
        }
        else
        {
            printf("syntax error: closing bracket not found\n");
        }

        ip++;
    }
    else
    {
        node = GetN();
    }

    return node;
}

int HTMLDump(const tree_t* tree, const char* occasion)
{
    Assert(tree == NULL);

    log("tree pointer in HTMLDump: %p\n", tree);

    static size_t num_dump;  //init in TreeCtor

    char curr_picname[MAX_LEN_PICNAME] = "";

    char str_num_dump[MAX_LEN_NUM_DUMP] = "";

    itoa(num_dump, str_num_dump, 10);

    strcpy(curr_picname, PICNAME);
    strcat(curr_picname, str_num_dump);
    strcat(curr_picname, FILE_EXTENSION);

    log("picture name: %s\n", curr_picname);
    //picname made

    GraphTreeDump(tree, curr_picname);

    char html_piccmd[MAX_LEN_CONSOLE_CMD] = "<img src=\"";

    strcat(html_piccmd, curr_picname);
    strcat(html_piccmd, "\" alt=\"ERROR: Picture Not Found\">");

    char* r_mode = "a";
    if (num_dump == 0)          r_mode = "w";

    FILE* html_file = fopen(HTML_FILE_NAME, r_mode);

    if (html_file == NULL)
    {
        print_log(FRAMED, "FILE ERROR: can't open HTML file");
    }

    dumphtml("<pre>\n");
    dumphtml("<b>\n");
    dumphtml("<font size=6>");
    dumphtml("Tree dump (");
    dumphtml("%d", num_dump);
    dumphtml(")\n");
    dumphtml("<i>");
    dumphtml("<font size=4>");
    dumphtml("\nOcassion for DUMP: %s\n\n", occasion);
    dumphtml("%s\n\n", html_piccmd);
    dumphtml("</pre>\n");

    num_dump++;

    fclose(html_file);
}



void DrawNode(elem_s* node, FILE* dump_file, const char* branch_label)
{
    dumpline("struct%p [\nlabel = \"{<data>elem: ", node);

    char* color = "";

    switch(node->type)
    {
        case NODE_OP:
            color = "chartreuse2";
            dumpline("%.4s", &node->value.op_v);
            break;

        case NODE_VAL:
            color = "aquamarine";
            dumpline("%d", node->value.int_v);
            break;

        case NODE_VAR:
            color = "darkgoldenrod1";
            dumpline("%s", &node->value.str_v);
            break;

        case NODE_FUNC:
            color = "plum1";
            dumpline("%s", &node->value.str_v);
            break;

        default:
            print_log(FRAMED, "Invalid type of node");
    }

    extern elem_s* tree_tail;

    //if (tree_tail == node)  color = "webgreen";

    dumpline("|<type>type: %.4s|<parent>parent: %p|<left>left: %p|<right>right: %p}\", style = \"filled\", "
             "color = \"black\", fillcolor = \"%s\" \n];\n", &node->type, node->parent, node->left, node->right, color);
    if (node->parent != NULL)
    {
        dumpline("struct%p -> struct%p [weight=900 constraint=true color=red];\n", node->parent, node);
    }
    if (node->left != NULL)
    {
        DrawNode(node->left,  dump_file, "yes");
    }
    if (node->right != NULL)
    {
        DrawNode(node->right, dump_file, "no");
    }
}

void GraphTreeDump(const tree_t* tree, const char* picname)
{
    log("tree pointer: %p\n", tree);

    FILE* dump_file = fopen(DUMP_FILE_NAME, "w");

    if (dump_file == NULL)
    {
        print_log(FRAMED, "FILE ERROR: Cant't open file\n");
    }
    dumpline("digraph {\n");
    dumpline("graph [dpi = 100]");
    dumpline("rankdir = TB;\n");
    //dumpline("splines = \"ortho\";\n");
    dumpline("node [ shape=record ];\n");

    dumpline("Tail [label = \"Tail: %p\", style = \"filled\", rankdir = TB, fillcolor = \"indigo\", fontcolor = \"yellow\"];\n", tree->Tail);

    //elem_s* tree_tail = tree->Tail;

    DrawNode(tree->Ptr, dump_file, "root");

    //dumpline("Tail -> struct%p [weight=0 constraint=false color=green];\n", tree->Tail);

    dumpline("}");

    fclose(dump_file);

    static size_t num_dump;  //init in TreeCtor

    log("start making console cmd\n");

    char console_cmd[MAX_LEN_CONSOLE_CMD] = "";
    strcpy(console_cmd, "dot -T png -o ");
    strcat(console_cmd, picname);
    strcat(console_cmd, " ");
    strcat(console_cmd, DUMP_FILE_NAME);

    log("console cmd: %s\n", console_cmd);

    printf("%d\n", system("cd D:"));
    printf("%d\n", system("cd D:\\Programming\\C\\Ded_course_1_sem\\AUF Language"));
    printf("%d\n", system(console_cmd));
}

int LogCritError(int errcode, const char* func, int line)
{
    switch (errcode)
    {
        case OK:
            //print_log(FRAMED, "Everything OK: Stack is all right and can be used");
            break;

        case SEGFAULT:
            print_crit_errors("SEGMENTATION FAULT: Invalid Pointer to Structure of Stack", func, line);
            break;

        case ZOMBIE:
            print_crit_errors("DEADINSIDE ERROR: Stack doesn't exist", func, line);
            break;

        case NULLPTR:
            print_crit_errors("POINTER ERROR: Stack Pointer (pointer to buffer) is NULL", func, line);
            break;

        case SIZEPOISONED:
            print_crit_errors("SIZE ERROR: Stack Size is poisoned", func, line);
            break;

        case NEGCAP:
            print_crit_errors("CAPACITY ERROR: Stack Capacity has a Negative Value", func, line);
            break;

        case STACKOVERFLOW:
            print_crit_errors("STACK OVERFLOW ERROR: Size of Stack is bigger than its Capacity", func, line);
            break;

        case FREE_ERROR:
            print_crit_errors("FREE ERROR: Index of free unit is invalid", func, line);
            break;

        case CHAINERROR:
            print_crit_errors("CHAIN ERROR: Invalid Next-Prev Connection between elements", func, line);
            break;

        case CONSTR_ERROR:
            print_crit_errors("CONSTRUCTION ERROR: Trying to construct an existing Tree", func, line);
            break;

        default:
            print_crit_errors("DECODE ERROR: Unexpected Error Code", func, line);
            return 1;
    }

    return 0;
}
