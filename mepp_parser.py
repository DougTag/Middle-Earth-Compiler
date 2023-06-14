import ply.yacc as yacc
from mepp_lexer import tokens

def p_program(p):
    'program : global main_statement'

def p_global(p):
    '''
    global : declaration_statement end
           | function_statement
           | global global
           | empty
    '''
    
def p_function_statement(p):
    'function_statement : FUNCTION TYPE ID OPEN_PARENTHESIS args CLOSE_PARENTHESIS block'

def p_args(p):
    '''
    args : declaration_statement
         | separator
         | empty
    '''

def p_separator(p):
    '''
    separator : separator COMMA declaration_statement
              | declaration_statement
    '''

def p_main_statement(p):
    'main_statement : THE_1 JOURNEY BEGINS HERE blocks'
    
def p_declaration_statement(p):
    '''
    declaration_statement : type ID
    '''
    
def p_block(p):
    '''
    block : OPEN_BRACE statements CLOSE_BRACE
    '''

def p_statements(p):
    '''
    statements : declaration_statement end
               | while_statement
               | continue_statement end
               | break_statement end
               | return_statement end
               | if_else_statement
    '''

def p_while_statement(p):
    'while_statement : WHILE OPEN_PARENTHESIS comparasion CLOSE_PARENTHESIS block'
    
def p_continue_statement(p):
    'continue_statement : THE_1 QUEST CONTINUES'

def p_break_statement(p):
    'break_statement : YOU SHALL NOT PASS'

def p_return_statement(p):
    '''
    return_statement : GO BACK TO THE_2 ABYSS
                     | GO BACK TO THE_2 ABYSS ID
                     | GO BACK TO THE_2 ABYSS literal
    '''

def p_if_else_statement(p):
    '''
    if_else_statement : if_statement
                      | if_statement elif_statement elif_statement
                      | if_statement else_statement else_statement
    '''

def p_if_statement(p):
    'if_statement : IF OPEN_PARENTHESIS comparasion CLOSE_PARENTHESIS block'

def p_elif_statement(p):
    '''
    elif_statement : HOWEVER WHEN OPEN_PARENTHESIS comparasion CLOSE_PARENTHESIS block
                   | empty
    '''

def p_else_statement(p):
    '''
    else_statement : ELSE OPEN_PARENTHESIS comparasion CLOSE_PARENTHESIS block
                   | empty
    '''
        
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
    
def p_unary(p):
    'expression : SUB literal %prec UNARY'
    p[0] = -p[2]

def p_literal(p):
    '''
    literal : INT_LITERAL
            | FLOAT_LITERAL
            | CHAR_LITERAL
            | STRING_LITERAL
            | bool_literal
    '''
    p[0] = p[1]

def p_bool_literal(p):
    '''
    bool_literal : BOOL_TRUE
                 | BOOL_FALSE
    '''
    p[0] = p[1]

def p_type(p):
    '''
    type : UNSIGNED
         | SHORT
         | INT
         | LONG
         | FLOAT
         | DOUBLE
         | CHAR
         | STRING
         | BOOL
         | VOID
    '''

def p_end(p):
    'end : SEMICOLON'

def p_empty(p):
    'empty :'
    pass



precedence = (
    ('left', 'ADD', 'SUB'),
    ('left', 'MULT', 'DIV'),
    ('left', 'MOD'),
    ('left', 'POW'),
    ('right', 'UNARY'), 
)
 

parser = yacc.yacc()

result = parser.parse("The journey begins here {hobbit a;}")
print(result)