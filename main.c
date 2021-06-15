#define _CRT_SECURE_NO_WARNINGS

#include "dialog.h"


char* error;
const char* MSGS[] = { "0.End","1.Add", "2.Find",  "3.Remove", "4.Print" };
const int MSGS_SIZE = sizeof(MSGS) / sizeof(MSGS[0]);

int MSIZE = 5;

int main()
{
	int c;
	char* filename = GetStr("filename");
	FILE* input = NULL;
	if (_access(filename, 0) == -1)
	{
		input = fopen(filename, "wbx");
		if (!input)
			return 2;

		char buf[500] = "";
		fwrite(buf, sizeof(char), 500, input);

		if (input)
			fclose(input);
	}

	input = fopen(filename, "r+b");
	if (!input)
	{
		printf("cannot open input file\n");

		return 1;
	}

	key_space_t ks = { NULL, input };
	KSLoad(&ks);

    do {
        c = dialog(MSGS, MSGS_SIZE);
        switch (c) {
        case 0:
            break;
        case 1:
            _add(&ks);
            break;
        case 2:
            _find(&ks);
		break;
        case 3:
            _remove(&ks);
		break;
        case 4:
			_print(&ks);
			break;
        }
    } while (c != 0);


	KSSave(&ks);
	KSDelete(&ks);

	if (ks.file)
		fclose(ks.file);
	free(filename);
	return 0;
}
