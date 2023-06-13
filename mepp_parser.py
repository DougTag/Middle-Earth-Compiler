import ply.yacc as yacc
from mepp_lexer import tokens

''''
hobbit var = "string";

hobbit var;
string s;

var = s;

ID = ID


Prologue hobbit func()
FUNCTION ID OPEN_P CLOSE_P

hobbit var = func();
SHORT ID ASSING ID

1-RETORNO IGUAL
2-NUM DE PARAM IGUAL
3- TIPOS DOS PARAMETROS

Prologue hobbit f(troll a, troll b)
{}

troll x = 1;
sindarin s = "string";
 
hobbit a = f(x, s);

SHORT ID ASSIGN ID OPEN ID COMMA ID CLOSE SEMICOLON

TYPE ID ASSING ID OPEN_P CLOSE_P

'''

def p_unary(p):
    'expression : SUB number %prec UNARY'
    p[0] = -p[2]

def p_expression(p):
    '''
    expression : expression ADD expression
               | expression SUB expression
               | expression DIV expression
               | expression MULT expression
               | expression MOD expression
               | expression POW expression
    '''
    if p[2] == '**':
        p[0] = p[1] ** p[3]
    elif p[2] == '+':
        p[0] = p[1] + p[3]
    elif p[2] == '/':
        p[0] = p[1] / p[3]
    elif p[2] == '-':
        p[0] = p[1] - p[3]
    elif p[2] == '*':
        p[0] = p[1] * p[3]
    elif p[2] == '%':
        p[0] = p[1] % p[3]
    

def p_expression_number(p):
    'expression : literal'
    p[0] = p[1]

def p_literal(p):
    '''
    literal : INT_LITERAL
            | FLOAT_LITERAL
            | CHAR_LITERAL
            | STRING_LITERAL
            | bool_literal
    '''
    p[0] = p[1]

def p_literal(p):
    '''
    bool_literal : BOOL_TRUE
            | BOOL_FALSE
    '''
    p[0] = p[1]


precedence = (
    ('left', 'ADD', 'SUB'),
    ('left', 'MULT', 'DIV'),
    ('left', 'MOD'),
    ('left', 'POW'),
    ('right', 'UNARY'), 
)


parser = yacc.yacc()

result = parser.parse("2 ** -2.3")
print(result)