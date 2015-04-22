#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("Usage: dumpy [rom bin] [/dev/ttyUSB0, etc] [mega]\n");
		return 0;
	}
	FILE *f = fopen(argv[1],"r");
	if (!f)
	{
		printf("Failed to open %s for reading\n",argv[1]);
		return 0;
	};
	FILE *w = fopen(argv[2],"r+");
	if (!w)
	{
		printf("Failed to open %s for writing\n",argv[2]);
		fclose(f);
		return 0;
	}
	if (argc == 4)
	{
		fputs("*T",w);
		fgetc(w);
		fputs("*g",w);
		fgetc(w);
		fputc(32,w);
		printf("Gave size...\n");
	}
	int duh = 0;
	sleep(2);
	printf("Writing...\n");
	while (!feof(f) && duh < 128)
	{
		duh++;
		fputc(fgetc(f),w);
		if (duh % 16 == 0)
		{
			printf(".");
		}
	}
	sleep(5);
	fgetc(w);
	printf("\nGiving reboot command\n");
	if (argc == 4)
	{
		fputs("*rm",w);
	}
	fclose(f);
	return 0;
}
