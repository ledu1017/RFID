#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main() {
	char a[] = "abcd";
	char b[] = "efgh";
	char result[] = NULL;

	result = a + b;

	printf("%s\n", result);
}