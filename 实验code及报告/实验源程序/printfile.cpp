#include"printfile.h"

status PrintFile(FILE* fp) {
	int indentnum=0, line = 1;
	int i; char c;
	FILE* print_fp;
	char filename[30] = "C_print_file.txt";
	print_fp = fopen(filename, "w");
	while (!printList.empty())
	{
		indentnum = printList.front().indent;
		printList.pop();
		while (!printList.empty() && line == printList.front().linenum) printList.pop();
		while (!printList.empty() && line < printList.front().linenum)
		{
			for (i = 0; i < indentnum; i++)
				fputc('\t', print_fp);
			while ((c = fgetc(fp)) != '\n')
				fputc(c, print_fp);
			fputc(c, print_fp);
			line++;
		}
	}
	while ((c = fgetc(fp)) != EOF)
		fputc(c, print_fp);
	fclose(print_fp);
	return OK;
}