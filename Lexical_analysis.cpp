#include <stdio.h>
#include <sys\stat.h>


#include "lang_rec_des.h"
#include "D:\\Programming\\C\\Ded_course_1_sem\\Processor_v4\\logging.h"

//start DSL
#define NewStr(line)                                                        \
{                                                                           \
    CreateToken(T_STR, {.op_v = NULL});                                     \
    strcpy(tcode->Ptr[tcode->Size - 1].value.str_v, line);                  \
}

#define NewOp(cmd)                                                          \
{                                                                           \
    CreateToken(T_OP, {.op_v = cmd});                                       \
}

#define NewVal(num)                                                         \
{                                                                           \
    CreateToken(T_VAL, {.int_v = num});                                     \
}

#define NewNewLine(num_tabs)                                                \
{                                                                           \
    CreateToken(T_NEWLINE, {.int_v = num_tabs});                            \
}
//finish DSL


//constants

const char* FILECODE = "AUF.txt";



//start language commands

const cmd_t CMD_ASSIGN  = {.Text = "Запомните твари:",
                           .Len  = 16};
const cmd_t CMD_IF      = {.Text = "Я может и не может:",
                           .Len  = 19};
const cmd_t CMD_ELSEIF  = {.Text = "А может я:",
                           .Len  = 10};
const cmd_t CMD_ELSE    = {.Text = "Но хотя бы не я:",
                           .Len  = 16};
const cmd_t CMD_WHILE   = {.Text = "Безумно можно быть первым",
                           .Len  = 25};
const cmd_t CMD_NEWFUNC = {.Text = "Обид не держу - держу пиво.",
                           .Len  = 27};
const cmd_t CMD_RETURN  = {.Text = "Сделал дело - дело сделано.",
                           .Len  = 27};
const cmd_t CMD_BRACE   = {.Text = "АУФ",
                           .Len  = 3};
const cmd_t CMD_BIGGER  = {.Text = "Сильнее",
                           .Len  = 7};
const cmd_t CMD_LESS    = {.Text = "Слабее",
                           .Len  = 6};
const cmd_t CMD_AND     = {.Text = "важно",
                           .Len  = 5};
const cmd_t CMD_OR      = {.Text = "неважно",
                           .Len  = 7};

//Arithmetic
const cmd_t CMD_ADD   = {.Text = "+",
                         .Len  = 1};
const cmd_t CMD_SUB   = {.Text = "-",
                         .Len  = 1};
const cmd_t CMD_MUL   = {.Text = "*",
                         .Len  = 1};
const cmd_t CMD_DIV   = {.Text = "/",
                         .Len  = 1};
const cmd_t CMD_DEG   = {.Text = "^",
                         .Len  = 1};

//finish language commands

//GLOBAL VARIABLES
static token_code_t __tcode = {NULL, 0, 0};
static token_code_t*  tcode = &__tcode;

token_code_t* Tokenize()
{
    static FILE* filecode = OpenRFile(FILECODE);

    charbuf_t* buf = TextToBuffer(filecode);

    return LexTrans(buf->Ptr);
}


int TcodeCtor()
{
    tcode->Ptr = allocate_array(token_t, MIN_LEN_TOKEN_CODE);

    tcode->Size     = 0;
    tcode->Capacity = MIN_LEN_TOKEN_CODE;
}

int TcodeDump()
{
    log("\n\n++++++++++Tcode Dump++++++++++\n\n");

    log("Size     = %d\n",   tcode->Size);
    log("Capacity = %d\n\n", tcode->Capacity);

    for (size_t i = 0; i < tcode->Capacity; i++)
    {
        log("[%0*d]:  ", 4, i);

        switch (tcode->Ptr[i].type)
        {
            case T_STR:
                log("%.4s    %s\n", &tcode->Ptr[i].type,    tcode->Ptr[i].value.str_v);
                break;

            case T_OP:
                log("%.4s    %.4s\n", &tcode->Ptr[i].type, &tcode->Ptr[i].value.op_v);
                break;

            case T_VAL:
                log("%.4s    %d\n", &tcode->Ptr[i].type,    tcode->Ptr[i].value.int_v);
                break;

            case T_NEWLINE:
                log("%.4s    %d\n", &tcode->Ptr[i].type,    tcode->Ptr[i].value.int_v);
                break;

            default:
                log("FREE\n");
        }
    }

    log("\n----------Tcode Dump----------\n\n");
}

int TcodeRecalloc()
{
    token_t* temp_ptr = (token_t*) realloc(tcode->Ptr, tcode->Capacity * 2 * sizeof(token_t));

    if (temp_ptr == NULL)
    {
        print_log(FRAMED, "RECALLOC ERROR: Can't find memory for token code");
        return -1;
    }
    tcode->Ptr      = temp_ptr;

    tcode->Capacity *= 2;

    log("finish Tcode Recalloc\n");

    return 0;
}

token_code_t* LexTrans(const char* line)
{
    TcodeCtor();

    log("tcode created\n");

    while (*line != '\0')
    {
        if (!ReadOp(&line, &CMD_ASSIGN,  OP_ASS))    continue;
        if (!ReadOp(&line, &CMD_IF,      OP_IF ))    continue;
        if (!ReadOp(&line, &CMD_ELSEIF,  OP_EIF))    continue;
        if (!ReadOp(&line, &CMD_ELSE,    OP_ELS))    continue;
        if (!ReadOp(&line, &CMD_WHILE,   OP_WHI))    continue;
        if (!ReadOp(&line, &CMD_NEWFUNC, OP_NFU))    continue;
        if (!ReadOp(&line, &CMD_RETURN,  OP_RET))    continue;

        if (!ReadOp(&line, &CMD_BIGGER,  OP_BIGGER)) continue;
        if (!ReadOp(&line, &CMD_LESS,    OP_LESS))   continue;

        if (!ReadOp(&line, &CMD_ADD,     OP_ADD))    continue;
        if (!ReadOp(&line, &CMD_SUB,     OP_SUB))    continue;
        if (!ReadOp(&line, &CMD_MUL,     OP_MUL))    continue;
        if (!ReadOp(&line, &CMD_DIV,     OP_DIV))    continue;
        if (!ReadOp(&line, &CMD_DEG,     OP_DEG))    continue;

        if (!ReadVal(&line))                         continue;
        if (!ReadVar(&line))                         continue;

        printf("Nothing can be read. Exit\n");

        break;
    }

    TcodeDump();

    return tcode;
}

int ReadNewLine(const char** ptr_line)
{
    size_t num_spaces = 0;

    (*ptr_line)++;

    while (**ptr_line == ' ' || **ptr_line == 9)
    {
        if (**ptr_line == ' ')          num_spaces++;
        else                            num_spaces += 4;

        (*ptr_line)++;
    }

    if (num_spaces % 4 != 0)
    {
        printf("Syntax Error: invalid number of spaces at the beginning of the line\n");

        print_log(FRAMED, "Invalid number of spaces");
        log("Spaces at the beginning of the line: %d\n", num_spaces);

        NewNewLine(num_spaces / 4);
        return -1;
    }

    NewNewLine(num_spaces / 4);
    return 0;
}

int ReadVar(const char** ptr_line)
{
    SkipSpaces(ptr_line);

    log("line in ReadVar: %s end.\n", *ptr_line);

    size_t num_read_sym = 0;

    char var[MAX_LEN_VAR_NAME] = "";

    sscanf(*ptr_line, "%s%n", var, &num_read_sym);

    if (!num_read_sym)
    {
        log("num read sym = 0 for %s\n", *ptr_line);

        return -1;
    }
    NewStr(var);

    *ptr_line += strlen(var);

    return 0;
}

int ReadVal(const char** ptr_line)
{
    SkipSpaces(ptr_line);

    if (**ptr_line == '\0')     return -1;

    log("line in readVal: %s end.\n", *ptr_line);

    int val = 0;

    size_t num_read_sym = 0;

    sscanf(*ptr_line, "%d%n", &val, &num_read_sym);

    log("num read sym: %d\n", num_read_sym);

    if (!num_read_sym)          return -1;

    NewVal(val);

    *ptr_line += num_read_sym;

    return 0;
}

int ReadOp(const char** ptr_line, const cmd_t* cmd_text, size_t cmd_code)
{
    SkipSpaces(ptr_line);

    if (strncmp(*ptr_line, cmd_text->Text, cmd_text->Len))        return -1;

    NewOp(cmd_code);

    *ptr_line += cmd_text->Len;

    return 0;
}

int SkipSpaces(const char** ptr_line)
{
    while (**ptr_line == ' ' || **ptr_line == '\n' || **ptr_line == '\0')
    {
        if (**ptr_line == '\n')
        {
            ReadNewLine(ptr_line);
            return 1;
        }
        else if (**ptr_line == '\0')
        {
            printf("Reached end of file\n");
            return 1;
        }

        (*ptr_line)++;
    }

    return 0;
}


int CreateToken(size_t type, union tokenv_t value) //special macros for strs
{
    if (tcode->Size >= tcode->Capacity - 1)
    {
        TcodeDump();
        TcodeRecalloc();
        TcodeDump();
    }

    tcode->Size++;

    tcode->Ptr[tcode->Size - 1] = {.type = type, .value = value};

    return 0;
}

charbuf_t* TextToBuffer(FILE* file)
{
    static charbuf_t buf = {.Ptr = NULL, .Size = GetFileSize(file)};

    buf.Ptr = allocate_array(char, buf.Size);

    size_t num_read_sym = fread(buf.Ptr, sizeof(char), buf.Size, file);

    Assert(num_read_sym == 0);

    log("buffer size: %d\n"
        "fread number of symbols: %d\n", buf.Size, num_read_sym);

    /*if (buf.Size - 1 != num_read_sym)
    {
        print_log(FRAMED, "Program dies from cringe encoding");
        return NULL;
    }*/

    return &buf;
}

FILE* OpenRFile(const char* filename)
{
    FILE* r_file = fopen(filename, "r");

    if(r_file == NULL)
    {
        print_log(FRAMED, "FILE ERROR: No such file found");
        log("File %s not found\n", filename);
        return NULL;
    }

    return r_file;
}

size_t GetFileSize(FILE* file)
{
    Assert(file == NULL);

    struct stat buf;

    int errcode = fstat(fileno(file), &buf);

    Assert(errcode != NULL);

    log("File size = %d\n", buf.st_size);

    return buf.st_size;
}
