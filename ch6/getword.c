#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFSIZE 100
#define MAXWORD 100

char buf[BUFSIZE]; 
int bufp = 0; // next free position in buf

int getch()
{
	return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
	if(bufp >= BUFSIZE)
		printf("ungetch : too many characters\n");
	else
		buf[bufp++] = c;
}

int getword(char *word, int lim)
{
	int c;
	char *w = word;

	while(isspace(c = getch()))
		;

	if(c != EOF)
		*w++ = c;
	if(!isalpha(c)){
		*w = '\0';
		return c;
	}
	for( ; --lim > 0; w++){
		if(!isalnum(*w = getch())){
			ungetch(*w);
			break;
		}
	}	
	*w = '\0';
	return word[0];
}

typedef struct key
{
	char *word;
	int count;
}key_t;

key_t keytab[] = {
	{"auto", 0},
	{"break", 0},
	{"case", 0},
	{"char", 0},
	{"const", 0},
	{"continue", 0},
	{"default", 0},
	{"unsigned", 0},
	{"void", 0},
	{"volatile", 0},
	{"while", 0},
};

int binsearch(char *, key_t *, int);

int main()
{
	const int NKEYS = sizeof(keytab) / sizeof(keytab[0]);
	freopen("csapp.h", "r",  stdin);
	int n;
	char word[MAXWORD];

	while(getword(word, MAXWORD) != EOF)
		if(isalpha(word[0]))
			if((n = binsearch(word, keytab, NKEYS)) >= 0){
				keytab[n].count++;
				n = -1;
			}

	for(n = 0; n < NKEYS; n++)
		if(keytab[n].count > 0)
			printf("%4d %s\n", keytab[n].count, keytab[n].word);

	return 0;
}

int binsearch(char *word, key_t tab[], int n)
{
	int cond = 0;
	int low, high, mid;

	low = 0;
	high = n -1;
	while(low <= high){
		mid = low + (high - low) / 2;

		if((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if(cond > 0)
			low = mid + 1;
		else 
			return mid;
	}

	return -1;
}