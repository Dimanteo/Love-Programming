#ifndef PROGRAMMING_LANGUAGE_MAKSIM_H
#define PROGRAMMING_LANGUAGE_MAKSIM_H

const char LFUNC_DEF[] = "чтоб не нести";
const char LMAIN[] = "припев";
const char LBEGIN[] = "вдоль ночных дорог";
const char LEND[] = "не потеряй его и не сломай";
const char LGET[] = "мой нежный";
const char LPUT[] = "сквозь слезы прошептала";
const char LWHILE[] = "повторяю лишь";
const char LIF[] = "знаешь ли ты";
const char LELSE[] = "пульс его теперь";
const char LASSIGNMENT[] = "теперь";
const char LRETURN[] = "не потеряй";
const char LDIFFER[] = "не жалея";
const char LDIFFERVAL[] = "шла по";

#define L(tok) (char*)L##tok

char* Language_CMD[] {
    L(FUNC_DEF),
    L(MAIN),
    L(BEGIN),
    L(END),
    L(GET),
    L(PUT),
    L(WHILE),
    L(IF),
    L(ELSE),
    L(ASSIGNMENT),
    L(RETURN),
    L(DIFFER),
    L(DIFFERVAL)
};

int LCMD_COUNT = sizeof(Language_CMD) / sizeof(Language_CMD[0]);

#undef L

#endif //PROGRAMMING_LANGUAGE_MAKSIM_H
