#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
//トークン
//

/*--構造体，共用体の定義--*/
//トークンの種類
typedef enum {
	TK_RESERVED,	//記号
	TK_NUM, 	//変数トークン
	TK_EOF,		//入力の終わりを表すトークン
}TokenKind;

//トークン型
typedef struct Token Token;
struct Token {
	TokenKind kind;	//トークンの型
	Token *next;	//次のトークン
	int val;	//kindがTK_NUMの場合，その数値
	char *str;	//トークン文字列
	int len;	//トークンの文字数
};

/*--関数の宣言-*/
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
bool startswith(char *p, char *q);
Token *tokenize();

/*--グローバル変数の宣言-*/
extern char *user_input;
extern Token *token;




//
//パーサ
//


/*--構造体，共用体の定義--*/

typedef enum {
	ND_ADD,
	ND_SUB,
	ND_MUL,
	ND_DIV,
	ND_EQ,
	ND_NE,
	ND_LT,
	ND_LE,
	ND_NUM,
} NodeKind;

typedef struct Node Node;
struct Node {
	NodeKind kind;
	Node *lhs;
	Node *rhs;
	int val;
};

/*--関数の宣言--*/
Node *new_node(NodeKind kind);
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);
Node *new_num(int val);

Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();




//
//ジェネレータ
//

/*--関数の宣言--*/
void gen(Node *node);


























