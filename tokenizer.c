//tokenizer.c

#include "9cc.h"

/*--グローバル変数の定義--*/
char *user_input;	//ユーザが入力したプログラム
Token *token;		//処理中のトークン

/*--関数の定義--*/
//エラーを報告するための関数
//printfと同じ引数をとる
void error(char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}

void error_at(char *loc, char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);

	int pos = loc - user_input;
	fprintf(stderr, "%s\n", user_input);
	fprintf(stderr, "%*s", pos, "");//pos個の空白を出力
	fprintf(stderr, "^");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, "\n");
	exit(1);
}


//次のトークンが期待している記号の時は，トークンの一つ読み進めて
//真を返す，それ以外の場合には偽を返す．
bool consume(char *op) {
	if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
		return false;
	token = token->next;
	return true;
}

//次のトークンが期待している記号の時には，トークンを一つ読み進める．
//それ以外の場合にはエラーを報告する，
void expect(char *op) {
	if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
		error_at(token->str, "expect '%s'", op);
	token = token->next;
}

//次のトークンが数値の場合，トークンを一つ読み進めてその数値を返す．
//それ以外の場合はエラーを報告する．
int expect_number() {
	if (token->kind != TK_NUM)
		error_at(token->str, "expected a number");
	int val = token->val;
	token = token->next;
	return val;
}

bool at_eof() {
	return token->kind == TK_EOF;
}

//新しいトークンを作成してcurに繋げる．
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
	Token *tok = calloc(1, sizeof(Token));
	tok->kind = kind;
	tok->str = str;
	tok->len = len;
	cur->next = tok;
	return tok;
}

bool startswith(char *p, char *q) {
	return memcmp(p, q, strlen(q)) == 0;
}


//入力文字列pをトークナイズしてそれを返す．
Token *tokenize() {
	char *p = user_input;
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while (*p) {
		//空白文字をスキップ
		if (isspace(*p)) {
			p++;
			continue;
		}

		//比較演算子
		if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">=")) {
			cur = new_token(TK_RESERVED, cur, p, 2);
			p += 2;
			continue;
		}

		//四則演算子
		if (strchr("+-*/()<>",*p)) {
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		//10進数
		if (isdigit(*p)) {
			cur = new_token(TK_NUM, cur, p, 0);
			char *q = p;
			cur->val = strtol(p, &p, 10);
			cur->len = p - q;
			continue;
		}

		//変数
		if ('a' <= *p && *p <= 'z') {
			cur = new_token(TK_IDENT, cur, p++);
			cur->len = 1;
			continue;
		}


		error_at(p, "invalid token");
	}

	new_token(TK_EOF, cur, p, 0);
	return head.next;
}