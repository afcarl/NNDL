grammar NNDL;

prog:               (import_stat)* network_stat EOF
                    ;

import_stat:        IMPORT ID
                    ;

network_stat:       NETWORK_OPEN neuron_stat connection_stat NETWORK_END
                    ;

neuron_stat:        NEURON_OPEN (layer_stat)+ NEURON_END
                    ;

connection_stat:    CONNECT_OPEN (con_stat)* CONNECT_END
                    ;

//TODO: Make layer_stat allow multiple types of neurons in a single layer
layer_stat:         LAYER ':' LAYER_ID MAT_DECL '=' ID
                    ;

con_stat:           neuron_selection ':' CONNECT neuron_selection (',' neuron_selection)* '=' ID
                    ;

neuron_selection:   '(' logical_expr ',' logical_expr ')'
                    ;

logical_expr:       log_pred_expr | class_expr | math_expr
                    ;

log_pred_expr:      '{' predicate '}'
                    ;

class_expr:         '[' CLASS ']'
                    ;

predicate:          math_expr ('<'|'<='|'>'|'>='|'==') math_expr
                    | predicate ('&&'|'||') predicate
                    ;

math_expr:          math_expr ('*'|'/'|'%') math_expr
                    | math_expr ('+'|'-') math_expr
                    | NEUR_VAR
                    | NUM
                    ;

IMPORT:             'import'
                    ;

NETWORK_OPEN:       'network'
                    ;

NETWORK_END:        'end network'
                    ;

NEURON_OPEN:        'neurons'
                    ;

NEURON_END:         'end neurons'
                    ;

CONNECT_OPEN:       'connections'
                    ;

CONNECT_END:        'end connections'
                    ;

LAYER:              'layer'
                    ;

LAYER_ID:           ID
                    ;

MAT_DECL:           NUM 'x' NUM
                    ;

CONNECT:            'connect'
                    ;

CLASS:              '*'
                    | NUM '-' NUM
                    ;

NEUR_VAR:           'i'
                    | 'j'
                    ;

ID:                 LETTER (LETTER|DIGIT|'_')*
                    ;

NUM:                (DIGIT)+
                    ;

fragment
LETTER:             [a-zA-Z]
                    ;

fragment
DIGIT:              '0'..'9'
                    ;


WS:                 [ \t\u000C]+ -> skip
                    ;
