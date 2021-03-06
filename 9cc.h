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
	TK_IDENT,		//識別子
	TK_NUM, 	//変数トークン
	TK_EOF,		//入力の終わりを表すトークン
	TK_RETURN,	//リターントークン
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

//出現した変数の連結リスト
typedef struct LVar LVar;
//ローカル変数の型
struct LVar {
	LVar *next;
	char *name;
	int len;
	int offset;
};

/*--関数の宣言-*/
void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
Token *consume_ident();			//変数を判定して，文字列を返す関数
bool consume_keytoken(TokenKind kind);
void expect(char *op);
int expect_number();
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
bool startswith(char *p, char *q);
int is_alnum(char c);
LVar *find_Lvar(Token *tok);
Token *tokenize();

/*--グローバル変数の宣言-*/
extern char *user_input;
extern Token *token;




//
//パーサ
//


/*--構造体，共用体の定義--*/

typedef enum {
	ND_ASSIGN,	// 代入記号
	ND_LVAR,	// ローカル変数
	ND_ADD,
	ND_SUB,
	ND_MUL,
	ND_DIV,
	ND_EQ,
	ND_NE,
	ND_LT,
	ND_LE,
	ND_NUM,
	ND_RETURN,
} NodeKind;

typedef struct Node Node;
struct Node {
	NodeKind kind;	//ノードの型
	Node *lhs;		//左辺
	Node *rhs;		//右辺
	int val;		//kindがND_NUMのみ使用
	int offset;		//kindがND_LVARのみ使用
};

/*--関数の宣言--*/
Node *new_node(NodeKind kind);
Node *new_binary(NodeKind kind, Node *lhs, Node *rhs);
Node *new_num(int val);
//変数ノードを追加する関数
LVar *new_Lvar();
void init_Lvar(LVar *var, char *name, int len, int offset);
LVar *add_Lvar(char *name, int len, int offset);

void program();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();


/*--グローバル変数の宣言-*/
extern Node *code[];


//
//ジェネレータ
//

/*--関数の宣言--*/
void gen_lval(Node *node);
void gen(Node *node);


























