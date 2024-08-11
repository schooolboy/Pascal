// sa3LL.h
// синтаксический анализатор

#include "syntall.h"

class syntaxan {
public:
    // конструктор
    syntaxan(lexan* lex, FILE* parse_stream, FILE* result_stream) {
        this->lex = lex;
        this->parse_stream = parse_stream;
        this->result_stream = result_stream;
    }
    // LL(1)
    int parse(void) {
        lex->parse();
        next_token();
        // итератор и длина правила
        int i = 0, n = 0;
        ttype at = TOK_EOT, bt = TOK_EOT;
        // символ на стэке
        ttype s = TOK_EOT;
        // значение, считанное из управл¤ющей таблицы
        int t = 0;
        // первоначальное проталкивание TOK_EOT в стэк (дно стэка)
        sta.push(TOK_EOT);
        //  проталкивание аксимомы грамматики в стэк
        sta.push(START);
        fprintf(parse_stream, "\n");
        // бесконченый цикл амтомата LL
        while (1) {
            s = clear_stack(); // символ на вершине стэка
            t = SYNTA[s][tok.type];
            //fprintf(parse_stream, "\nSYNTA[%d][%d] = %d", s, tok.type, t);
            if (t <= 0) {
                //ошибка
                throw(new texception("syntaxan: failure synta", 0, tok.pos));
                return 0;
            }
            else if (t == ACC) {
                // допуск
                break;
            }
            else if (t == POP) {
                // выброс
                at = sta.pop();
                clear_stack();
                next_token();
            }
            else if (t <= MAX_RULE) {
                // это правило
                at = sta.pop();
                n = RLEN[t];
                // проталкиваем правило в стэк в обратном пор¤дке
                for (i = n - 1; i > -1; i--) {
                    sta.push(bt = RULE[t][i]);
                }
                fprintf(parse_stream, "\npush rule: %d", t);
            }
            else {
                // ошибка управл¤ющей таблицы
                throw (new texception("syntaxan: failure synta", 0, tok.pos));
            }
        }
        printf("\nSYNTAXAN SUCCESS\n");
        fprintf(parse_stream, "\n\nSYNTAXAN SUCCESS\n");
        // записываем конец ленты ѕќЋ»«
        out(OUT_END);
        strip.fprint(parse_stream);
        strip.print();
        execute();
        return 1;
    }
private:
    // ссылка на лексический анализатор
    lexan* lex;
    // поток вывода (анализ)
    FILE* parse_stream;
    // поток вывода (результата)
    FILE* result_stream;
    // текущий токен
    ttoken tok;
    // стек ћѕ-автомата
    mpstack sta;
    // таблица символов
    stsyms syms;
    // исполн¤ющий стэк
    exstack exe;
    // лента ѕќЋ»«
    ststrip strip;
    // стек меток
    labelstack stl;
    // возвращает очередной токен  
    void next_token() {
        tok = lex->next_token();
        if (tok.type == TOK_LF) {
            next_token();
        }
        else if (tok.type == TOK_COMMENT) {
            next_token();
        }
    }
    // запись на ленту ѕќЋ»«
    void out(ttype tt) {
        int i = 0, j = 0;
        ttoken t = tok;
        t.type = tt;
        switch (tt) {
        case OUT_PUSH:
            j = strip.new_label();
            stl.push(j);
            t.word_val = j;
            t.type = OUT_LABEL;
            break;
        case OUT_POPL:
            j = stl.pop();
            t.word_val = j;
            t.type = OUT_LABEL;
            break;
        }
        strip.add(t);
    }
    // чистит операционные символы на стеке 
    ttype clear_stack() {
        ttype s;
        while ((s = sta.top()) > SYM_LAST + 1) {
            s = sta.pop();
            out(s);
        }
        return s;
    }
    // исполн¤юща¤ машина ѕќЋ»«
    void execute() {
        // указатель ленты ѕќЋ»«
        int strip_pointer = 1;
        // счетчик операций ѕќЋ»«
        int it_counter = 1;
        // текущий элемент ѕќЋ»«
        ttype tp;
        // переменные дл¤ вычислений
        ttoken x, y;
        int j = 0, val;
        bool m1, m2;
        // рабочий цикл выполнени¤ ленты ѕќЋ»«
        while (1) {
            // очистим переменные
            x.reset();
            y.reset();
            // считываем тип элемента ленты
            tp = strip[strip_pointer].type;
            switch (tp) {
            case OUT_END:
                printf("\n\nEXE DONE\n");
                fprintf(result_stream, "\n\nEXE_DONE\n");
                return;
            case OUT_PROGRAM:
                syms.insert(strip[strip_pointer]);
                strip_pointer++;
            case OUT_ID: case OUT_WORD: case OUT_CH: case OUT_QWORD: case OUT_QCH:
                // проталкиваем в стек
                exe.push(strip[strip_pointer]);
                strip_pointer++;
                break;
            case OUT_DIM:
                exe.pop(y);
                // при любом обь¤влении
                do {
                    exe.pop(x);
                    j = syms.insert(x);
                    
                    if (j == ST_EXISTS) throw (new texception("exe: duplicate declaration", 0, x.pos));
                    syms[j].type = y.type;

                } while (exe.size() != 0);
                strip_pointer++;
                break;
            case OUT_ASS:
                exe_pop(y);
                exe.pop(x);
                j = syms.find(x);
                if (j == ST_NOTFOUND) throw (new texception("exe: identifier not found", 0, x.pos));
                // если присваиваетс¤ char
                if (y.type == OUT_CH) val = char_asc(y.char_val);
                // если присваиваетс¤ word | bool
                else val = y.word_val;
                // если переменна¤ char
                if (syms[j].type == OUT_QCH) {
                    // приводим значение к ближайшим границам ( это word )
                    if (val > 256) val = -1;
                    else if (val > 127 && val < 256) val = val - 128 * 2;
                    else if (val < -128) val = -128;
                    syms[j].char_val = val;
                }
                // если переменна¤ word
                else { 
                    if (val < 0 || val > WORD_SIZE) throw (new texception("exe: 'word' can be only between 0 and WORD_SIZE,", WORD_SIZE, x.pos));
                    syms[j].word_val = val; 
                }
                // обозначаем что переменна¤ инициализирована
                syms[j].tok_val.set_str("INIT");
                strip_pointer++;
                break;
                // все операции двуместные (кроме QNE)
            case OUT_ADD: case OUT_SUB: case OUT_MUL: case OUT_DIV: case OUT_EQ:
            case OUT_NE: case OUT_LT: case OUT_GT: case OUT_LE: case OUT_GE:
            case OUT_QNE: case OUT_QAND: case OUT_QOR:
                exe_pop(y);
                if (tp != OUT_QNE) exe_pop(x);
                if (x.type == OUT_CH) x.word_val = char_asc(x.char_val);
                if (y.type == OUT_CH) y.word_val = char_asc(y.char_val);
                // выполнить операцию
                switch (tp) {
                case OUT_ADD:
                    if (x.type == OUT_CH || y.type == OUT_CH) throw (new texception("exe: type is not suitable for the operation", 0, x.pos));
                    x.word_val += y.word_val;
                    break;
                case OUT_SUB:
                    if (x.type == OUT_CH || y.type == OUT_CH) throw (new texception("exe: type is not suitable for the operation", 0, x.pos));
                    x.word_val -= y.word_val;
                    break;
                case OUT_MUL:
                    if (x.type == OUT_CH || y.type == OUT_CH) throw (new texception("exe: type is not suitable for the operation", 0, x.pos));
                    x.word_val *= y.word_val;
                    break;
                case OUT_DIV:
                    if (x.type == OUT_CH || y.type == OUT_CH) throw (new texception("exe: type is not suitable for the operation", 0, x.pos));
                    if (y.word_val == 0) throw (new texception("exe: division by zero", 0, x.pos));
                    x.word_val /= y.word_val;
                    break;
                case OUT_EQ:
                    if (x.word_val == y.word_val) {
                        x.word_val = 1; // true
                        x.type = OUT_BOOL;
                    }
                    else {
                        x.word_val = 0; // false
                        x.type = OUT_BOOL; // вспомогательный логический тип
                    }
                    break;
                case OUT_NE:
                    if (x.word_val != y.word_val) {
                        x.word_val = 1;
                        x.type = OUT_BOOL;
                    }
                    else {
                        x.word_val = 0;
                        x.type = OUT_BOOL;
                    }
                    break;
                case OUT_LT:
                    if (x.word_val < y.word_val) {
                        x.word_val = 1;
                        x.type = OUT_BOOL;
                    }
                    else {
                        x.word_val = 0;
                        x.type = OUT_BOOL;
                    }
                    break;
                case OUT_GT:
                    if (x.word_val > y.word_val) {
                        x.word_val = 1;
                        x.type = OUT_BOOL;
                    }
                    else {
                        x.word_val = 0;
                        x.type = OUT_BOOL;
                    }
                    break;
                case OUT_LE:
                    if (x.word_val <= y.word_val) {
                        x.word_val = 1;
                        x.type = OUT_BOOL;
                    }
                    else {
                        x.word_val = 0;
                        x.type = OUT_BOOL;
                    }
                    break;
                case OUT_GE:
                    if (x.word_val >= y.word_val) {
                        x.word_val = 1;
                        x.type = OUT_BOOL;
                    }
                    else {
                        x.word_val = 0;
                        x.type = OUT_BOOL;
                    }
                    break;
                case OUT_QNE:
                    x = y;
                    if (x.type != OUT_BOOL) { 
                        throw 
                        (new texception("exe: invalid type for operation 'not'",
                         0, y.pos)); 
                    }
                    if (x.word_val == 1) x.word_val = 0;
                    else x.word_val = 1;
                    break;
                case OUT_QAND: case OUT_QOR:
                    if (x.type != OUT_BOOL || y.type != OUT_BOOL) { 
                        throw (new texception("exe: invalid type for operations 'and', 'or'",
                        0, y.pos)); 
                    }
                    if (tp == OUT_QAND) {
                        if (x.word_val && y.word_val) x.word_val = 1;
                        else x.word_val = 0;
                    }
                    else {
                        if (x.word_val|| y.word_val) x.word_val = 1;
                        else x.word_val = 0;
                    }
                    break;
                }
                exe.push(x);
                strip_pointer++;
                break;
            case OUT_LABEL:
                // если встретили метку, то проталкиваем ее
                x.word_val = strip[strip_pointer].word_val;
                x.type = OUT_WORD;
                x.pos = strip[strip_pointer].pos;
                exe.push(x);
                strip_pointer++;
                break;
            case OUT_DEFL:
                // срабатывает тогда, как метка уже не нужна
                exe.pop(x);
                strip_pointer++;
                break;
            // переход по лжи
            case OUT_BZ:
                exe_pop(y);
                exe_pop(x);
                if (x.type != OUT_BOOL) { 
                    throw 
                    (new texception("exe: invalid type of conditional expression",
                     0, x.pos));
                }
                if (x.word_val == 0) {
                    j = strip.find_DEF(y.word_val);
                    if (j == -1) { 
                        throw 
                        (new texception("exe: lable not found", 0, x.pos)); 
                    }
                    strip_pointer = j;
                }
                else {
                    strip_pointer++;
                }
                break;
            case OUT_WRITE:
                exe_pop(y);
                if (y.type == OUT_CH) { printf("\n%c", y.char_val); fprintf(result_stream, "\n%c", y.char_val); }
                else if (y.type == OUT_BOOL && y.word_val == 1) {printf("\ntrue"); fprintf(result_stream, "\ntrue"); }
                else if (y.type == OUT_BOOL && y.word_val == 0) {printf("\nfalse"); fprintf(result_stream, "\nfalse"); }
                else if (y.type == OUT_WORD) {printf("\n%d", y.word_val); fprintf(result_stream, "\n%d", y.word_val); }
                else throw (new texception("exe: invalid type for operation 'write'", 0, strip[strip_pointer].pos));
                strip_pointer++;
                break;
            }
            if (++it_counter > MAX_IT) {
                throw (new texception("exe: deadlock", 0, strip[strip_pointer].pos));
            }
        }
    }
    // извлекает из стэка значение
    void exe_pop(ttoken& e) {
        exe.pop(e);
        // допустимые типы пропускаем
        if (e.type == OUT_WORD) {}
        else if (e.type == OUT_CH) {}
        else if (e.type == OUT_BOOL) {}
        else if (e.type == OUT_ID) {
            int j = syms.find(e);
            if (j == ST_NOTFOUND) {
                // символ не найден
                throw (new texception("exe_pop: identifier not found", 0, e.pos));
            }
            // если переменна¤ не инициализирована
            if (strcmp(syms[j].tok_val.str, "NON_INIT") == 0) {
                throw (new texception("exe_pop: uninitialized variable is used", 0, e.pos));
            }
            // значит число
            if (syms[j].type == OUT_QWORD) {
                e.word_val = syms[j].word_val;
                e.type = OUT_WORD;
            } 
            // значит символ
            else {
                e.char_val = syms[j].char_val;
                e.type = OUT_CH;
            }
        }
        else {
            // неправильна¤ лента ѕќЋ»«
            throw (new texception("exe_pop: internal error", 0, e.pos));
        }
    }
    int char_asc(int a) {
        if (a < 0) {
            a = a + 128 * 2;
        }
        return a;
    }
};