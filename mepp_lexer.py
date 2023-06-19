import ply.lex as lex

reserved = {
   'Quest': 'WHILE',
   'The': 'THE_1',
   'quest': 'QUEST',
   'continues': 'CONTINUES',
   'You': 'YOU',
   'shall': 'SHALL',
   'not': 'NOT',
   'pass': 'PASS',
   'journey': 'JOURNEY',
   'begins': 'BEGINS',
   'here': 'HERE',
   'Go': 'GO',
   'back': 'BACK',
   'to': 'TO',
   'the': 'THE_2',
   'abyss': 'ABYSS',
   'Given' : 'IF',
   'Otherwise': 'ELSE',
   'However': 'HOWEVER',
   'when': 'WHEN',
   'Prologue': 'FUNCTION',
   'draugr': 'UNSIGNED',
   'hobbit': 'SHORT',
   'elf': 'INT',
   'troll': 'LONG',
   'wyvern': 'FLOAT',
   'dragon': 'DOUBLE',
   'tengwarr': 'CHAR',
   'sindarin': 'STRING',
   'istari': 'BOOL',
   'darkness': 'VOID',
   'xor': 'XOR',
   'and': 'AND',
   'or': 'OR',
   'good': 'BOOL_TRUE',
   'evil': 'BOOL_FALSE',
}

tokens = [
   'OPEN_BRACE',
   'CLOSE_BRACE',
   'SEMICOLON',
   'COMMA',
   'OPEN_PARENTHESIS',
   'CLOSE_PARENTHESIS',
   'ADD',
   'SUB',
   'MULT',
   'POW',
   'DIV',
   'MOD',
   'ASSIGN',
   'EQUAL',
   'DIFF',
   'LESS',
   'LESS_EQUAL',
   'GREATER',
   'GREATER_EQUAL',
   'BITWISE_NOT',
   'BITWISE_OR',
   'BITWISE_AND',
   'BITWISE_XOR',
   'ID',
   'INT_LITERAL',
   'FLOAT_LITERAL',
   'STRING_LITERAL',
   'CHAR_LITERAL',
   'EXCLAMATION',
   'NEWLINE'
] + list(reserved.values())

t_OPEN_BRACE = r'{'
t_CLOSE_BRACE = r'}'
t_SEMICOLON = r';'
t_COMMA = r','
t_OPEN_PARENTHESIS = r'\('
t_CLOSE_PARENTHESIS = r'\)'
t_ADD = r'\+'
t_SUB = r'-'
t_MULT = r'\*'
t_POW = r'\*\*'
t_DIV = r'/'
t_MOD = r'%' 
t_ASSIGN = r'='
t_EQUAL = r'=='
t_DIFF = r'<>'
t_LESS = r'<' 
t_LESS_EQUAL = r'<='
t_GREATER = r'>' 
t_GREATER_EQUAL = r'>='
t_BITWISE_NOT = r'~'
t_BITWISE_OR = r'\|'
t_BITWISE_AND = r'&'
t_BITWISE_XOR = r'\^'
t_EXCLAMATION = r'!'

t_STRING_LITERAL = r'"[\w]+"'
t_CHAR_LITERAL = r'\'[\w]\''

t_ignore  = ' \t'

def t_FLOAT_LITERAL(t):
    r'[0-9]+\.[0-9]+'
    t.value = float(t.value)
    return t

def t_INT_LITERAL(t):
    r'[0-9]+'
    t.value = int(t.value)
    return t

def t_COMMENT(t):
    r'\#.*'
    pass

def t_error(t):
    print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)

def t_ID(t):
    r'[a-zA-Z_][a-zA-Z_0-9]*'
    t.type = reserved.get(t.value,'ID')    # Checa se é palavra reservada
    return t

def t_NEWLINE(t):
    r'\n'
    t.lexer.lineno += len(t.value)
    return t

lexer = lex.lex()


code = '''
The journey begins here
{
    elf a;
    
    
    
    
    Go back to the abyss;
}
'''

lexer.input("")

while True:
    tok = lexer.token()
    if not tok: 
        break
    print(tok)

