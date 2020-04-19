#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr,"引数が正しくありません\n");
		return 1;
	}

	//引数を文字列にいれる
	char *p = argv[1];
			
	//x86-64の宣言文記述	
	printf(".intel_syntax noprefix\n");
	printf(".global main\n");
	printf("main:\n");

	//最初の値を初期値としてレジストリに代入
	printf("	mov rax, %ld\n", strtol(p, &p, 10));

	while (*p) {
		if (*p == '+') {
			p++;
			printf("	add rax, %ld\n", strtol(p, &p, 10));
			continue;
		}
		
		if (*p == '-') {
			p++;
			printf("	sub rax, %ld\n", strtol(p, &p, 10));
			continue;
		}

		fprintf(stderr, "予期しない文字です: %c\n", *p);
	}
		
	printf(" ret\n");
	return 0;

}




