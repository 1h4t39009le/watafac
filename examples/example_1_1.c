extrn printf;
extrn exit;

int x = 42;
char *msg = "Hello, World!\n";
printf("%d", x);

int y = 10;
if (y > 5) {
    char *inner = "y is big\n";
    printf(inner, 12);
    aboba(1);
}

exit(0);
