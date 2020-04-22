//parse.c

#include "9cc.h"

/*
メモ：
パーサでローカル変数リストを更新するため，new_Lvar,init_Lvar,add_Lvarを実装した．
 add_Lvarでリストの最後尾に新しいノードを追加するために，リストの末尾を常に記憶するlocallastを
グローバル変数として追加した．
u-
*/


/*--グローバル変数の定義--*/
Node *code[100];
LVar locals;	//ローカル変数のヘッド
LVar *localhead, *locallast;	//ローカル変数のヘッドとラスト(どんどん更新していく)

/*
//ここで代入とかしたらコンパイル通らなかったから，program()内で初期化した．
locals.next = NULL;
localhead = locallast = &locals;	
*/

/*--関数の定義--*/
Node *new_node(NodeKind kind) {
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs) {
	Node *node = new_node(kind);
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

Node *new_num(int val) {
	Node *node = new_node(ND_NUM);
	node->val = val;
	return node;
}

Node *new_ident(int offset){
	Node *node = new_node(ND_LVAR);
	node->offset = offset;
	return node;
}


//変数を名前で検索する．見つからなかったときはNULLを返す．
LVar *find_Lvar(Token *tok) {
	for (LVar *var = localhead->next; var; var = var->next)
		if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
			return var;
	return NULL;
}
//変数ノードを追加する関数
LVar *new_Lvar() {
	LVar *var = calloc(1, sizeof(LVar));
	return var;
}
void init_Lvar(LVar *var, char *name, int len, int offset) {
	var->name = name;
	var->len = len;
	var->offset = offset;
}
LVar *add_Lvar(char *name, int len, int offset) {
	LVar *var = new_Lvar();
	locallast->next = var;
	locallast = var; //変数リストの最後を更新
	init_Lvar(var, name, len, offset);
	return var;
}


//
//再帰降下構文解析の処理
//


void program(){
	locals.next = NULL;
	localhead = locallast = &locals;
	int i = 0;
	for(;!at_eof();){
		code[i++] = stmt();
	}
	code[i] = NULL;
}

Node *stmt(){
	Node *node = expr();
	expect(";");
	return node;
}

//expr = equality
Node *expr(){
	return assign();
}

Node *assign(){
	Node *node = equality();
	if (consume("="))
		node = new_binary(ND_ASSIGN, node, assign());
	return node;
}

//equality = relational ("==" relational | "!=" relational)*
Node *equality() {
	Node *node = relational();

	for (;;) {
		if (consume("=="))
			node = new_binary(ND_EQ, node, relational());
		else if (consume("!="))
			node = new_binary(ND_NE, node, relational());
		else
			return node;
	}
}

// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
Node *relational() {
	Node *node = add();

	for (;;) {
		if (consume("<"))
			node = new_binary(ND_LT, node, add());
		else if (consume("<="))
			node = new_binary(ND_LE, node, add());
		else if (consume(">"))
			node = new_binary(ND_LT, add(), node);
		else if (consume(">="))
			node = new_binary(ND_LE, add(), node);
		else
			return node;
	}
}

//add = mul("+" mul | "-" mul)*
Node *add() {
	Node *node = mul();

	for (;;) {
		if (consume("+"))
			node = new_binary(ND_ADD, node, mul());
		else if (consume("-"))
			node = new_binary(ND_SUB, node, mul());
		else
			return node;
	}
}
// mul = unary ("*" unary | "/" unary)*
Node *mul() {
	Node *node = unary();

	for (;;) {
		if (consume("*"))
			node = new_binary(ND_MUL, node, unary());
		else if (consume("/"))
			node = new_binary(ND_DIV, node, unary());
		else
			return node;
	}
}

// unary = ("+" | "-")? unary
Node *unary() {
	if (consume("+"))
		return unary();
	if (consume("-"))
		return new_binary(ND_SUB, new_num(0), unary());
	return primary();
}


Node *primary() {
	if (consume("(")) {
		Node *node = expr();
		expect(")");
		return node;
	}

	//ここに変数の分岐を追加する
	//必要なのが，トークン文字列が変数かどうか判定する関数が必要
	//あと，strを参照するためのトークン型が必要
	Token *tok = consume_ident();
	if (tok) {

		//変数リストから一致する変数を検索
		LVar *lvar = find_Lvar(tok);

		if (lvar==NULL) {	//もしlvalがNULLなら，変数リストに追加する
			//変数リストを追加する
			lvar = add_Lvar(tok->str, tok->len, locallast->offset + 8);
		}

		//オフセットを取得
		int offset = lvar->offset;
		//オフセットをnodeに格納する
		Node *node = new_ident(offset);
		token = token->next;
		return node;
	}
	
	return new_num(expect_number());
}
























