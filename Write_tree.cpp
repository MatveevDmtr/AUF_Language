#include "lang_rec_des.h"
#include "auf_lang.h"

int WriteTree(elem_s* root)
{
    log("Start writing tree\n");

    Assert(root == nullptr);

    FILE* treefile = fopen("Tree.txt", "w");

    if (treefile == NULL)
    {
        printf("Can't open dataBase file\n");

        return -1;
    }

    WriteNode(root, treefile, 0);

    fclose(treefile);

    log("Finish writing tree\n");

    return 0;
}

int WriteNode(elem_s* node, FILE* treefile, size_t num_spaces) //remaking
{
    bool have_children = false;

    Assert(node     == nullptr);
    Assert(treefile == nullptr);

    log("start writing node: %.4s\n", &node->type);

    WriteSpaces(num_spaces, treefile);

    fprintf(treefile, "{");
    //WriteSpaces(num_spaces, treefile);
    switch (node->type)
    {
        case NODE_OP:
            have_children = true;
            WriteOp(treefile, node->value.op_v);
            fprintf(treefile, "\n");
            break;

        case NODE_VAR:
            WriteVar(treefile, node->value.str_v);
            break;

        case NODE_FUNC:
            have_children = true;
            WriteVar(treefile, node->value.str_v);
            fprintf(treefile, "\n");
            break;

        case NODE_VAL:
            WriteVal(treefile, node->value.int_v);
            break;

        default:
            printf("TYPE ERROR: Unexpected node type: %.4s\n", &node->type);
    }

    if (node->left)
    {
        WriteNode(node->left, treefile, num_spaces + TAB);
    }
    else if (node->type == NODE_OP)
    {
        WriteSpaces(num_spaces + TAB, treefile);

        fprintf(treefile, "{ NIL }\n");
    }

    if (node->right)
    {
        WriteNode(node->right, treefile, num_spaces + TAB);
    }
    else if (node->type == NODE_OP)
    {
        WriteSpaces(num_spaces + TAB, treefile);

        fprintf(treefile, "{ NIL }\n");
    }

    if (have_children)          WriteSpaces(num_spaces, treefile);

    fprintf(treefile, "}\n");

    return 0;
}

void WriteSpaces(int num_spaces, FILE* treefile)
{
    Assert(treefile == nullptr);

    while (num_spaces-- > 0)
    {
        fprintf(treefile, " ");
    }
}

int WriteOp(FILE* treefile, size_t op)
{
    fprintf(treefile, " %.4s ", &op);
}

int WriteVar(FILE* treefile, char* var)
{
    fprintf(treefile, " \"%s\" ", var);
}

int WriteVal(FILE* treefile, int val)
{
    fprintf(treefile, " %d ", val);
}
