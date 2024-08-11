// lexan.h
// лексический анализатор

#include <cstdio>
#include "tot.h"
#include "types.h"

class lexan {
public:
    // входной поток
    FILE* input_stream = 0;
    // выходной поток
    FILE* output_stream = 0;
    // выходной массив токенов
    tflow flow;
    // текущий символа входа
    char c;
    // счетчик считывани¤ массива токенов
    int f_count = 0;
    // конструктор
	lexan(FILE* input_stream, FILE* output_stream) {
        this->input_stream = input_stream;
        this->output_stream = output_stream;
        next_char();
    }

    // следующий символ
    int next_char() {
        c = getc(input_stream);
        return (int)c;
    }

    // комментарий
    void is_comment() {
        int count = 0;
        while (c != '}') {
            next_char();
            count++;
            if (c == 10) {
                flow.line++;
            }
            if (count > MAX_COMSIZE) {
                throw (new texception("lexan: unable to find comment or its size more than MAX_COMSIZE,", MAX_COMSIZE, flow.get_pos()));
            }
        }
        flow.push(TOK_COMMENT, 0, NULL, NULL, "TOK_COMMENT");
        next_char();
    }

    // символ
    void is_char() {
        char b = next_char();
        if (c == 39) {
            throw (new texception("lexan: empty char,", 0, flow.get_pos()));
        }
        b = c;
        next_char();
        if (c != 39) {
            throw (new texception("lexan: char may have only 1 symbol,", 0, flow.get_pos()));
        }
        flow.push(TOK_CH, 0, b, NULL, "TOK_CH");
        next_char();
    }

    // ключевое слово
    int get_keyword(char str[]) {
        if (!strcmp(str, "if")) {
            flow.push(TOK_IF, 0, NULL, NULL, "TOK_IF");
            return 1;
        }
        else if (!strcmp(str, "then")) {
            flow.push(TOK_THEN, 0, NULL, NULL, "TOK_THEN");
            return 1;
        }
        else if (!strcmp(str, "var")) {
            flow.push(TOK_VAR, 0, NULL, NULL, "TOK_VAR");
            return 1;
        }
        else if (!strcmp(str, "program")) {
            flow.push(TOK_PROGRAM, 0, NULL, NULL, "TOK_PROGRAM");
            return 1;
        }
        else if (!strcmp(str, "begin")) {
            flow.push(TOK_BEGIN, 0, NULL, NULL, "TOK_BEG");
            return 1;
        }
        else if (!strcmp(str, "or")) {
            flow.push(TOK_QOR, 0, NULL, NULL, "TOK_QOR");
            return 1;
        }
        else if (!strcmp(str, "and")) {
            flow.push(TOK_QAND, 0, NULL, NULL, "TOK_QAND");
            return 1;
        }
        else if (!strcmp(str, "end.")) {
            flow.push(TOK_END, 0, NULL, NULL, "TOK_END");
            return 1;
        }
        else if (!strcmp(str, "not")) {
            flow.push(TOK_QNE, 0, NULL, NULL, "TOK_QNE");
            return 1;
        }
        else if (!strcmp(str, "write")) {
            flow.push(TOK_WRITE, 0, NULL, NULL, "TOK_WRITE");
            return 1;
        }
        else if (!strcmp(str, "word")) {
            flow.push(TOK_QWORD, 0, NULL, NULL, "TOK_QWORD");
            return 1;
        }
        else if (!strcmp(str, "char")) {
            flow.push(TOK_QCH, 0, NULL, NULL, "TOK_QCH");
            return 1;
        }
        else return 0;
    }

    // идентификатор
    void is_id() {
        // считываемый идентификатор
        char str[ID_SIZE] = {'\0'};
        // счетчик символа
        int count = 0;
        while (true) {
            if (count == ID_SIZE) {
                throw (new texception("lexan: id length must be less then ID_SIZE,", ID_SIZE, flow.get_pos()));
            }
            c = tolower(c);
            switch (TOT[c]) {
            case UNDER:
                break;
            case ALPHA:
                break;
            case DIGIT:
                break;
            default:
                // точка
                if (c == 46) {
                    str[count] = c;
                    count++;
                    next_char();
                }
                if (get_keyword(str)) { return; }
                else {
                    flow.push(TOK_ID, 0, NULL, str, "TOK_ID");
                    return; 
                }
            }
            str[count] = c;
            count++;
            next_char();
        }
    }

    // целое число
    void is_number() {
        int value = 0;
        while (TOT[c] == DIGIT) {
            value *= 10;
            value += c;
            value -= '0';
            if (value > WORD_SIZE) {
                throw (new texception("lexan: 'word' can be only between 0 and WORD_SIZE," , WORD_SIZE, flow.get_pos()));
            }
            next_char();
            // после числа не может быть букв
            if (TOT[c] == ALPHA) {
                throw (new texception("lexan: number read error,", 0, flow.get_pos()));
            }
            else if (TOT[c] != DIGIT) {
                // конец разбора, формируем токен целого числа
                flow.push(TOK_WORD, value, NULL, NULL, "TOK_WORD");
            }
        }
    }

    // операци¤
    void is_opera() {
        switch (c) {
        case '+':
            flow.push(TOK_ADD, 0, NULL, NULL, "TOK_ADD");
            next_char();
            break;
        case '-':
            flow.push(TOK_SUB, 0, NULL, NULL, "TOK_SUB");
            next_char();
            break;
        case '*':
            flow.push(TOK_MUL, 0, NULL, NULL, "TOK_MUL");
            next_char();
            break;
        case '/':
            flow.push(TOK_DIV, 0, NULL, NULL, "TOK_DIV");
            next_char();
            break;
        case '(':
            flow.push(TOK_LP, 0, NULL, NULL, "TOK_LP");
            next_char();
            break;
        case ')':
            flow.push(TOK_RP, 0, NULL, NULL, "TOK_RP");
            next_char();
            break;
        case '{':
            is_comment();
            break;
        case '>':
            next_char();
            if (c == '=') {
                flow.push(TOK_GE, 0, NULL, NULL, "TOK_GE");
                next_char();
                return;
            }
            flow.push(TOK_GT, 0, NULL, NULL, "TOK_GT");
            break;
        case '<':
            next_char();
            if (c == '=') {
                flow.push(TOK_LE, 0, NULL, NULL, "TOK_LE");
                next_char();
                return;
            }
            else if (c == '>') {
                flow.push(TOK_NE, 0, NULL, NULL, "TOK_NE");
                next_char();
                return;
            }
            flow.push(TOK_LT, 0, NULL, NULL, "TOK_LT");
            break;
        case '=':
            flow.push(TOK_EQ, 0, NULL, NULL, "TOK_EQ");
            next_char();
            break;
        case ':':
            next_char();
            if (c == '=') {
                flow.push(TOK_ASS, 0, NULL, NULL, "TOK_ASS");
                next_char();
                break;
            }
            flow.push(TOK_COLON, 0, NULL, NULL, "TOK_COLON");
            break;
        case ';':
            flow.push(TOK_SEMI, 0, NULL, NULL, "TOK_SEMI");
            next_char();
            break;
        case ',':
            flow.push(TOK_COMMA, 0, NULL, NULL, "TOK_COMMA");
            next_char();
            break;
        default:
            throw (new texception("lexan: unknown operation,", 0, flow.get_pos()));
        }
    }

    // анализ входа
    int parse() {
        flow.reset();
        while (true) {
            // пропускаем управл¤ющие символы
            if (c < 33) {
                if (c == 10) {
                    flow.push(TOK_LF, 0, NULL, NULL, "TOK_LF");
                }
                else if (c == -1) {
                    flow.push(TOK_EOT, 0, NULL, NULL, "TOK_EOT");
                    break;
                }
                if (c < -2) {
                    throw (new texception("lexan: unknown symbol,", 0, flow.get_pos()));
                }
                next_char();
            }
            else {
                switch (TOT[c]) {
                case ALPHA:
                    is_id();
                    break;
                case UNDER:
                    is_id();
                    break;
                case DIGIT:
                    is_number();
                    break;
                case OPERA:
                    is_opera();
                    break;
                case SINQU:
                    is_char();
                    break;
                default:
                    throw (new texception("lexan: unknown token,", 0, flow.get_pos()));
                }
            }
        }
        flow.show(output_stream);
        printf("\nLEXAN SUCCESS\n");
        fprintf(output_stream, "\n\nLEXAN SUCCESS");
        return 1;
    }
    ttoken next_token() {
        if (flow.count == 0) {
            throw(new texception("read lexan: flow of tokens is empty", 0, flow.get_pos()));
        }
        return flow.st[f_count++];
    }
};