import ply.yacc as yacc
from mepp_lexer import tokens

def p_program(p):
    'program : global main_statement'

def p_global(p):
    '''
    global : declaration_statement end global
           | function_statement global
           | empty
    '''
    
def p_function_statement(p):
    'function_statement : FUNCTION type ID OPEN_PARENTHESIS params CLOSE_PARENTHESIS block'

def p_params(p):
    '''
    params : params_list
           | empty
    '''

def p_params_list(p):
    '''
    params_list : declaration_statement
                | params_list COMMA declaration_statement
    '''


def p_main_statement(p):
    'main_statement : THE_1 JOURNEY BEGINS HERE block'

def p_declaration_statement(p):
    '''
    declaration_statement : type ID
    '''

def p_declaration_statement_list(p):
    '''
    declaration_statement_list : declaration_statement
                               | declaration_statement_list COMMA ID 
    '''

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
    
    # adicionar exclamacao
def p_end(p):
    '''
    end : SEMICOLON
        | EXCLAMATION
    '''

def p_empty(p):
    'empty :'
    pass

def p_block(p):
    '''
    block : OPEN_BRACE statements_list CLOSE_BRACE
    '''

def p_statements_list(p):
    '''
    statements_list : statements_list statements
                    | empty
    '''

def p_statements(p):
    '''
    statements : declaration_statement_list end
               | while_statement
               | continue_statement end
               | break_statement end
               | if_else_statement
               | return_statement end
               | assign_statement end
    '''

def p_while_statement(p):
    'while_statement : WHILE OPEN_PARENTHESIS comparasion_list CLOSE_PARENTHESIS block'
    
def p_continue_statement(p):
    'continue_statement : THE_1 QUEST CONTINUES'

def p_break_statement(p):
    'break_statement : YOU SHALL NOT PASS'

def p_if_else_statement(p):
    'if_else_statement : if_statement elif_statement else_statement'

def p_if_statement(p):
    'if_statement : IF OPEN_PARENTHESIS comparasion_list CLOSE_PARENTHESIS block'

def p_else_statement(p):
    '''
    else_statement : ELSE block
                   | empty
    '''

def p_elif_statement(p):
    '''
    elif_statement : elif_statement HOWEVER WHEN OPEN_PARENTHESIS comparasion_list CLOSE_PARENTHESIS block 
                   | empty
    '''

def p_return_statement(p):
    '''
    return_statement : GO BACK TO THE_2 ABYSS
                     | GO BACK TO THE_2 ABYSS expression
    '''

def p_comparasion_list(p):
    '''
    comparasion_list : comparasion
                     | comparasion_list logic_operator comparasion
    '''

def p_comparasion(p):
    '''
    comparasion : expression
                | expression value_operators expression
    '''

def p_operators(p):
    '''
    value_operators : EQUAL
                    | LESS
                    | LESS_EQUAL
                    | GREATER
                    | GREATER_EQUAL
    '''

def p_logic_operator(p):
    '''
    logic_operator : AND
                   | OR
                   | XOR
    '''

def p_literal(p):
    '''
    literal : INT_LITERAL
            | FLOAT_LITERAL
            | CHAR_LITERAL
            | STRING_LITERAL
            | BOOL_FALSE
            | BOOL_TRUE
    '''
    p[0] = p[1]

def p_assign_statement(p):
    '''
    assign_statement : ID ASSIGN call_function
                     | ID ASSIGN expression
    '''

def p_function(p):
    'call_function : ID OPEN_PARENTHESIS args CLOSE_PARENTHESIS'

def p_args(p):
    '''
    args : args_list
         | empty
    '''

def p_args_list(p):
    '''
    args_list : expression
              | args_list COMMA expression
    '''


def p_expression(p):
    '''
    expression : expression ADD values
               | expression SUB values
               | expression DIV values
               | expression MULT values
               | expression MOD values
               | expression POW values
               | expression BITWISE_OR values
               | expression BITWISE_AND values
               | expression BITWISE_XOR values
               | OPEN_PARENTHESIS expression CLOSE_PARENTHESIS
               | values     
    '''

def p_values(p):
    '''
    values : literal
           | ID
    '''

def p_error(p):
    print("Syntax error in input!")

parser = yacc.yacc()

code = '''
elf i;
troll j;

Prologue elf fatorial(elf n){
    Given(n == 1)
    {
        Go back to the abyss 1;
    }

    b = fatorial(n-1);

    Go back to the abyss n * b;
}

sindarin string;

The journey begins here
{
    hobbit a;
    sindarin string;
    string = "eeoooo";
    
    a = (5 + 10) * 423423;
    Go back to the abyss 0!
}

'''

result = parser.parse(code)
print(result)