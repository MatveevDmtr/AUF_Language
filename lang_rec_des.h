#ifndef LANG_REC_DES_H_INCLUDED
#define LANG_REC_DES_H_INCLUDED

#include "auf_lang.h"

union tokenv_t
{
    char     str_v[MAX_LEN_TOKEN_STR];
    int      int_v;
    size_t   op_v;
};

typedef struct __token
{
    size_t      type;
    tokenv_t    value;
} token_t;

typedef struct __token_code
{
    token_t*    Ptr;
    size_t      Size;
    size_t      Capacity;
} token_code_t;

enum TOKEN_TYPES
{
    T_OP      = 'RPO',
    T_STR     = 'RTS',
    T_VAL     = 'LAV',
    T_NEWLINE = 'NLN',
    T_BRACE   = 'CRB',
};

enum COMMANDS
{
    OP_STM     = 'MTS',
    OP_ASS     = 'SSA',
    OP_IF      = 'FI',
    OP_ELIF    = 'FILE',
    OP_ELSE    = 'ESLE',
    OP_WHILE   = 'LIHW',
    OP_VOID    = 'DIOV',
    OP_TYPE    = 'EPYT',
    OP_RET     = 'TER',
    OP_BIGGER  = 'GIB',
    OP_LESS    = 'SSEL',
    OP_NBIGGER = 'GIBN',
    OP_NLESS   = 'SELN',
    OP_EQ      = 'QE',
    OP_NEQ     = 'QEN',
    OP_AND     = 'DNA',
    OP_OR      = 'RO' ,
    OP_INPUT   = 'PNI',
    OP_COMMA   = 'MOC',
    OP_PARAM   = 'RAP',
    OP_FUNC    = 'CNUF',
    OP_CALL    = 'LLAC',
    OP_NIL    = 'LIN',
};

enum BRACE
{
    BR_OPEN    = 'NEPO',
    BR_CLOSE   = 'SOLC',
};

typedef struct buffer_char
{
    char*   Ptr;
    size_t  Size;
}
charbuf_t;

typedef struct __command
{
    const char* Text;
    size_t      Len;
} cmd_t;


//start prototypes

token_code_t* Tokenize();

token_code_t* LexTrans(const char* line);

int TcodeCtor();

int TcodeRecalloc();

int TcodeDump();

int ReadVar(const char** ptr_line);

int ReadVal(const char** ptr_line);

int ReadBraces(const char** ptr_line);

int ReadOp(const char** ptr_line, const cmd_t* cmd_text, size_t cmd_code);

int SkipSpaces(const char** ptr_line);

int CreateToken(size_t type, union tokenv_t value);

FILE* OpenRFile(const char* filename);

charbuf_t* TextToBuffer(FILE* file);

size_t GetFileSize(FILE* file);

//finish prototypes

tree_t* CreateTree();

int MakeSyntaxTree(tree_t* tree);

elem_s* GetTrunkBranch();

elem_s* GetCodeBlock();

int GetG(tree_t* tree, const char* str);

elem_s* GetOpFunc();

elem_s* GetCall();

elem_s* GetRet();

elem_s* GetFuncDef(size_t op_ret, elem_s* (FuncParam)());

elem_s* GetDefParams(elem_s* (FuncParam)());

elem_s* GetWhile();

elem_s* GetIf();

elem_s* GetElse();

elem_s* GetAss();

elem_s* GetBoolExpr();

elem_s* GetCompare();

elem_s* GetAnd();

elem_s* GetBoolP();

elem_s* GetVariable();

size_t TypeOfTrigF(const char** s);

elem_s* GetDeg();

elem_s* GetVarsAndFuncs();

elem_s* GetN();

elem_s* GetE();

elem_s* GetT();

elem_s* GetP();

#endif   //guard
