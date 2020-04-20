//main.c

#include "9cc.h"

/*--グローバル変数の定義--*/
//なし

/*--関数の定義--*/

int main(int argc, char **argv) {
	if (argc != 2)
		error("%s:引数が正しくありません\n", argv[0]);

	//トークナイズとパース
	user_input = argv[1];
	token = tokenize();
	Node *node = expr();
			
	//アッセンブリx86-64の宣言文記述	
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	//抽象構文木を下りながらコードを生成
	gen(node);
	
	//スタックトップに式全体の値が残っているはず
	//それをRAXにロードして関数からの返り値とする
	printf("	pop rax\n");
	printf("	ret\n");
	return 0;
}
