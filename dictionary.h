#ifndef DICTIONARY_H_INCLUDED
#define DICTIONARY_H_INCLUDED

//start language commands

const cmd_t CMD_ASSIGN   = {.Text = "Запомните твари:",
                            .Len  = strlen(CMD_ASSIGN.Text)};
const cmd_t CMD_IF       = {.Text = "Я может и не может:",
                            .Len  = strlen(CMD_IF.Text)};
const cmd_t CMD_ELSEIF   = {.Text = "А может я:",
                            .Len  = strlen(CMD_ELSEIF.Text)};
const cmd_t CMD_ELSE     = {.Text = "Но хотя бы не я:",
                            .Len  = strlen(CMD_ELSE.Text)};
const cmd_t CMD_WHILE    = {.Text = "Безумно можно быть первым:",
                            .Len  = strlen(CMD_WHILE.Text)};
const cmd_t CMD_NEWVAR   = {.Text = "Лучше иметь друга, чем друг друга:",
                            .Len  = strlen(CMD_NEWVAR.Text)};
const cmd_t CMD_VOID     = {.Text = "Обид не держу - держу пиво.",
                            .Len  = strlen(CMD_VOID.Text)};
const cmd_t CMD_TYPE     = {.Text = "Любят тихо. Громко только пердят.",
                            .Len  = strlen(CMD_TYPE.Text)};
const cmd_t CMD_INPUT    = {.Text = "Дед - это как волк, только не волк:",
                            .Len  = strlen(CMD_INPUT.Text)};
const cmd_t CMD_OUTPUT   = {.Text = "Когда волк молчит, лучше его не перебивать:",
                            .Len  = strlen(CMD_OUTPUT.Text)};
const cmd_t CMD_RETURN   = {.Text = "Сделал дело - дело сделано.",
                            .Len  = strlen(CMD_RETURN.Text)};
const cmd_t CMD_OPENBR   = {.Text = "АУФ[",
                            .Len  = strlen(CMD_OPENBR.Text)};
const cmd_t CMD_CLOSEBR  = {.Text = "]АУФ",
                            .Len  = strlen(CMD_CLOSEBR.Text)};
const cmd_t CMD_OPENBRS  = {.Text = "(",
                            .Len  = strlen(CMD_OPENBRS.Text)};
const cmd_t CMD_CLOSEBRS = {.Text = ")",
                            .Len  = strlen(CMD_CLOSEBRS.Text)};
const cmd_t CMD_BIGGER   = {.Text = "сильнее",
                            .Len  = strlen(CMD_BIGGER.Text)};
const cmd_t CMD_LESS     = {.Text = "слабее",
                            .Len  = strlen(CMD_LESS.Text)};
const cmd_t CMD_NBIGGER  = {.Text = "несильнее",
                            .Len  = strlen(CMD_NBIGGER.Text)};
const cmd_t CMD_NLESS    = {.Text = "неслабее",
                            .Len  = strlen(CMD_NLESS.Text)};
const cmd_t CMD_EQ       = {.Text = "равный",
                            .Len  = strlen(CMD_EQ.Text)};
const cmd_t CMD_NEQ      = {.Text = "неравный",
                            .Len  = strlen(CMD_NEQ.Text)};
const cmd_t CMD_AND      = {.Text = "важно",
                            .Len  = strlen(CMD_AND.Text)};
const cmd_t CMD_OR       = {.Text = "неважно",
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
const cmd_t CMD_SQRT  = {.Text = "Кореш",
                         .Len  = 5};

//finish language commands


#endif //guard
