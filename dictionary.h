#ifndef DICTIONARY_H_INCLUDED
#define DICTIONARY_H_INCLUDED

//start language commands

const cmd_t CMD_ASSIGN   = {.Text = "��������� �����:",
                            .Len  = strlen(CMD_ASSIGN.Text)};
const cmd_t CMD_IF       = {.Text = "� ����� � �� �����:",
                            .Len  = strlen(CMD_IF.Text)};
const cmd_t CMD_ELSEIF   = {.Text = "� ����� �:",
                            .Len  = strlen(CMD_ELSEIF.Text)};
const cmd_t CMD_ELSE     = {.Text = "�� ���� �� �� �:",
                            .Len  = strlen(CMD_ELSE.Text)};
const cmd_t CMD_WHILE    = {.Text = "������� ����� ���� ������:",
                            .Len  = strlen(CMD_WHILE.Text)};
const cmd_t CMD_NEWVAR   = {.Text = "����� ����� �����, ��� ���� �����:",
                            .Len  = strlen(CMD_NEWVAR.Text)};
const cmd_t CMD_VOID     = {.Text = "���� �� ����� - ����� ����.",
                            .Len  = strlen(CMD_VOID.Text)};
const cmd_t CMD_TYPE     = {.Text = "����� ����. ������ ������ ������.",
                            .Len  = strlen(CMD_TYPE.Text)};
const cmd_t CMD_INPUT    = {.Text = "��� - ��� ��� ����, ������ �� ����:",
                            .Len  = strlen(CMD_INPUT.Text)};
const cmd_t CMD_OUTPUT   = {.Text = "����� ���� ������, ����� ��� �� ����������:",
                            .Len  = strlen(CMD_OUTPUT.Text)};
const cmd_t CMD_RETURN   = {.Text = "������ ���� - ���� �������.",
                            .Len  = strlen(CMD_RETURN.Text)};
const cmd_t CMD_OPENBR   = {.Text = "���[",
                            .Len  = strlen(CMD_OPENBR.Text)};
const cmd_t CMD_CLOSEBR  = {.Text = "]���",
                            .Len  = strlen(CMD_CLOSEBR.Text)};
const cmd_t CMD_OPENBRS  = {.Text = "(",
                            .Len  = strlen(CMD_OPENBRS.Text)};
const cmd_t CMD_CLOSEBRS = {.Text = ")",
                            .Len  = strlen(CMD_CLOSEBRS.Text)};
const cmd_t CMD_BIGGER   = {.Text = "�������",
                            .Len  = strlen(CMD_BIGGER.Text)};
const cmd_t CMD_LESS     = {.Text = "������",
                            .Len  = strlen(CMD_LESS.Text)};
const cmd_t CMD_NBIGGER  = {.Text = "���������",
                            .Len  = strlen(CMD_NBIGGER.Text)};
const cmd_t CMD_NLESS    = {.Text = "��������",
                            .Len  = strlen(CMD_NLESS.Text)};
const cmd_t CMD_EQ       = {.Text = "������",
                            .Len  = strlen(CMD_EQ.Text)};
const cmd_t CMD_NEQ      = {.Text = "��������",
                            .Len  = strlen(CMD_NEQ.Text)};
const cmd_t CMD_AND      = {.Text = "�����",
                            .Len  = strlen(CMD_AND.Text)};
const cmd_t CMD_OR       = {.Text = "�������",
                            .Len  = strlen(CMD_OR.Text)};
const cmd_t CMD_COMMA    = {.Text = ",",
                            .Len  = strlen(CMD_COMMA.Text)};

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
const cmd_t CMD_SQRT  = {.Text = "�����",
                         .Len  = 5};

//finish language commands


#endif //guard
