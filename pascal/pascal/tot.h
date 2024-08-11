// sttot.h
// таблица перекодировки

// псевдо-символы
#define INVAL 0   // недопустимый
#define DIGIT 1   // цифра
#define ALPHA 2   // буква
#define OPERA 3   // операция
#define CHDOT 4   // точка
#define QUOTE 5   // двойная кавычка
#define SINQU 6   // одинарная кавычка
#define UNDER 7   // знак подчеркивани¤
#define SLASH 8   // слеш
#define WHITE 9   // пробельный символ
#define SYMBOL 10 // одинарная кавычка

/* character conversion table */
unsigned char TOT[256] = { /* (c) 2003 ReVoL */
    /* управл¤ющие символы */
    /* 00     01     02     03     04     05     06     07     08     TAB    LF     0B     0C     CR     0E     0F */
    INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, WHITE, INVAL, INVAL, INVAL, WHITE, INVAL, INVAL,
    /* 10     11     12     13     14     15     16     17     18     19     1A     1B     1C     1D     1E     1F */
    INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL,

    /* пробел, знаки, цифры */
    /* 20     !      "      #      $      %      &      '      (      )      *      +      ,      -      .      /  */
    WHITE, INVAL, QUOTE, INVAL, INVAL, INVAL, INVAL, SINQU, OPERA, OPERA, OPERA, OPERA, OPERA, OPERA, INVAL, OPERA,
    /*  0       1      2      3      4      5      6      7      8      9     :      ;      <      =      >      ? */
    DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, DIGIT, OPERA, OPERA, OPERA, OPERA, OPERA, INVAL,

    /* прописные буквы, знаки */
    /*  @      A      B      C      D      E      F      G      H      I      J      K      L      M      N      O */
    INVAL, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,
    /*  P      Q      R      S      T      U      V      W      X      Y      Z      [      \      ]      ^      _ */
    ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, INVAL, INVAL, INVAL, INVAL, UNDER,

    /* строчные буквы, знаки */
    /*  `      a      b      c      d      e      f      g      h      i      j      k      l      m      n      o */
    INVAL, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA,
    /*  p      q      r      s      t      u      v      w      x      y      z      {      |      }      ~        */
    ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, ALPHA, OPERA, INVAL, OPERA, INVAL, INVAL,

    /* втора¤ половина ASCII - русские или другие интернациональные символы */
    INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL,
    INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL,
    INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL,
    INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL,
    INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL,
    INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL,
    INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL,
    INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL, INVAL
};