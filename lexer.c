#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h> // Fixes implicit declaration of strlen & strncmp
#define TEST_LEXER


#ifndef LEXER_H
#define LEXER_H
#define KEYWORD_COUNT (sizeof(keywords) / sizeof(Keyword))

// * General Use
#define allocError(ptr, func) \
    do { \
        if ((ptr) == NULL) { \
            fprintf(stderr, "Malloc - Allocation Error in %s\n", func); \
            return(NULL); \
        } \
    } while (0)


#define peek() (source[lexer.currentOffset])
#define advance() (source[lexer.currentOffset++])

// * Keywords
typedef enum {
    TOKEN_EOF,
    TOKEN_INT_LITERAL,
    TOKEN_FLOAT_LITERAL,
    TOKEN_IDENTIFIER,

    // types
    TOKEN_TYPE_INT,
    TOKEN_TYPE_BOOL,
    TOKEN_TYPE_FLOAT,

    TOKEN_KEYWORD_LET,
    TOKEN_KEYWORD_FUN,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_MATCH,

    TOKEN_ARROW,         // ->
    TOKEN_MATCH_ARROW,   // =>
    TOKEN_EQUAL,         // =
    TOKEN_PLUS,          // +
    TOKEN_MINUS,         // -
    TOKEN_STAR,          // *
    TOKEN_SLASH,         // /
    TOKEN_PIPE,          // |

    TOKEN_LPAREN,        // (
    TOKEN_RPAREN,        // )
    TOKEN_LBRACE,        // {
    TOKEN_RBRACE,        // }
    TOKEN_MATCH_WILDCARD,// _

    TOKEN_COMMA,         // ,
    TOKEN_SEMICOLON,     // ;
    TOKEN_COLON,         // :

    TOKEN_UNKNOWN
} TokenType;

typedef struct keyword {
    const char* word;
    TokenType type;
} Keyword;

static const Keyword keywords[] = {
    { "let", TOKEN_KEYWORD_LET },
    { "fun", TOKEN_KEYWORD_FUN },
    { "return", TOKEN_KEYWORD_RETURN },
    { "if", TOKEN_KEYWORD_IF },
    { "else", TOKEN_KEYWORD_ELSE },
    { "while", TOKEN_KEYWORD_WHILE },
    { "match", TOKEN_KEYWORD_MATCH },
    { "int", TOKEN_TYPE_INT },
    { "bool", TOKEN_TYPE_BOOL },
    { "float", TOKEN_TYPE_FLOAT },
    { "_", TOKEN_MATCH_WILDCARD }
};

struct lexerInfo {
    int startOffset;
    int currentOffset;
    int line;
};

struct lexerInfo lexer;
const char *source;

typedef struct {
    const char *start;
    TokenType type;
    int length;
    int line;
} Token;

void init_lexer (const char* initSource) {
    source = initSource;
    lexer.startOffset = 0;
    lexer.currentOffset = 0;
    lexer.line = 1;
}

int is_end () {
    if (source != NULL)
        return(source[lexer.currentOffset] == '\0');
    printf("Source file not found!");
    return(-1); // Lexer allocation error
}

void skip_whitespace() {
    char c;

    while (1) {
        c = peek();
        switch (c) {
            case '\n':
                lexer.line++;
            case '\r':
            case '\t':
            case ' ':
                advance();
                break;
            default:
                return;
        }
    }
}

// Initialises a new token
Token *tokenizer (TokenType type) {
    Token *newToken;
    
    newToken = (Token*) malloc(sizeof(Token));
    allocError(newToken, "tokenizer");
    
    newToken->type = type;
    newToken->line = lexer.line;
    newToken->start = source + lexer.startOffset;  // Fixed to ensure it's a pointer to the string
    newToken->length = lexer.currentOffset - lexer.startOffset;

    return(newToken);
}

Token *identifierOrKeyword () {
    char current;
    TokenType type;
    int len;

    while (1) {
        current = peek();
        if (!is_end() && (isalpha(current) || isdigit(current) || current == '_'))
            advance();
        else
            break; 
    }
    
    len = lexer.currentOffset - lexer.startOffset;

    type = TOKEN_IDENTIFIER;

    for (int i = 0; i < KEYWORD_COUNT; i++) {
        if (len == strlen(keywords[i].word) && !strncmp(&source[lexer.startOffset], keywords[i].word, len)) {
            type = keywords[i].type;
            break;
        }
    }

    return tokenizer(type);
}

Token *number () {
    int isFloat = 0;
    char cur = peek();

    if (isdigit(cur)) {
        while (isdigit(peek())) advance();
        if (peek() == '.') {
            isFloat = 1;
            advance();
            while (isdigit(peek())) advance();
        }
    }
    else if (cur == '.' && isdigit(source[lexer.currentOffset + 1])) {
        isFloat = 1;
        advance();
        while (isdigit(peek())) advance();
    }
    else {
        return tokenizer(TOKEN_UNKNOWN);
    }

    return tokenizer(isFloat ? TOKEN_FLOAT_LITERAL : TOKEN_INT_LITERAL);
}

Token *consume () {
    char cur;

    skip_whitespace();
    lexer.startOffset = lexer.currentOffset;
    cur = peek();

    if (isalpha(cur) || cur == '_') {
        return identifierOrKeyword(); // handles `let`, `x`, etc.
    } else if (isdigit(cur) || cur == '.') {
        return number(); // handles numeric literals
    }

    switch (cur) {
        case '=': {
            advance();
            if (peek() == '>') {
                advance();
                return tokenizer(TOKEN_MATCH_ARROW);
            }
            return tokenizer(TOKEN_EQUAL);   
        }
        case '-': {
            advance();
            if (peek() == '>') {
                advance();
                return tokenizer(TOKEN_ARROW);
            }
            return tokenizer(TOKEN_MINUS);
        }
        case '+': advance(); return tokenizer(TOKEN_PLUS);
        case '*': advance(); return tokenizer(TOKEN_STAR);
        case '/': advance(); return tokenizer(TOKEN_SLASH);
        case '|': advance(); return tokenizer(TOKEN_PIPE);
        case '(': advance(); return tokenizer(TOKEN_LPAREN);
        case ')': advance(); return tokenizer(TOKEN_RPAREN);
        case '{': advance(); return tokenizer(TOKEN_LBRACE);
        case '}': advance(); return tokenizer(TOKEN_RBRACE);
        case ',': advance(); return tokenizer(TOKEN_COMMA);
        case ';': advance(); return tokenizer(TOKEN_SEMICOLON);
        case ':': advance(); return tokenizer(TOKEN_COLON);
        case '\0': 
            return tokenizer(TOKEN_EOF);
    }

    // Unknown token (unrecognized character)
    advance();
    return tokenizer(TOKEN_UNKNOWN);
}

#ifdef TEST_LEXER
const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_EOF: return "EOF";
        case TOKEN_INT_LITERAL: return "INT_LITERAL";
        case TOKEN_FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_KEYWORD_LET: return "KEYWORD_LET";
        case TOKEN_KEYWORD_FUN: return "KEYWORD_FUN";
        case TOKEN_KEYWORD_RETURN: return "KEYWORD_RETURN";
        case TOKEN_KEYWORD_IF: return "KEYWORD_IF";
        case TOKEN_KEYWORD_ELSE: return "KEYWORD_ELSE";
        case TOKEN_KEYWORD_WHILE: return "KEYWORD_WHILE";
        case TOKEN_KEYWORD_MATCH: return "KEYWORD_MATCH";
        case TOKEN_ARROW: return "ARROW";
        case TOKEN_MATCH_ARROW: return "MATCH_ARROW";
        case TOKEN_EQUAL: return "EQUAL";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_STAR: return "STAR";
        case TOKEN_SLASH: return "SLASH";
        case TOKEN_PIPE: return "PIPE";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COLON: return "COLON";  // Added support for colon
        case TOKEN_TYPE_INT: return "TYPE_INT";  // Added for 'int'
        case TOKEN_TYPE_BOOL: return "TYPE_BOOL";  // Added for 'bool'
        case TOKEN_TYPE_FLOAT: return "TYPE_FLOAT";  // Added for 'float'
        case TOKEN_MATCH_WILDCARD: return "TOKEN_MATCH_WILDCARD";  // Added for 'float'
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "UNDEFINED";
    }
}


int main() {
    const char *sourceCode =
        "fun is_prime(n: int) -> bool {\n"
        "    if n <= 1 {\n"
        "        return 0;\n"
        "    }\n"
        "\n"
        "    let i = 2;\n"
        "    while i * i <= n {\n"
        "        if n % i == 0 {\n"
        "            return 0;\n"
        "        }\n"
        "        i = i + 1;\n"
        "    }\n"
        "\n"
        "    return 1;\n"
        "}\n"
        "\n"
        "fun main() -> int {\n"
        "    let num = 7;\n"
        "\n"
        "    if is_prime(num) {\n"
        "        let result = 1;\n"
        "    } else {\n"
        "        let result = 0;\n"
        "    }\n"
        "\n"
        "    match num {\n"
        "        0 => 0;\n"
        "        1 | 2 => 1;\n"
        "        _ => 2;\n"
        "    }\n"
        "\n"
        "    return 0;\n"
        "}\n";

    init_lexer(sourceCode);

    Token *token;
    while ((token = consume())->type != TOKEN_EOF) {
        printf("Token: Type=%s, Line=%d, Length=%d, Text=`%.*s`\n",
            tokenTypeToString(token->type),
            token->line,
            token->length,
            token->length,
            token->start);
        free(token); // Don't forget to free each token
    }

    return 0;
}


#endif

#endif
