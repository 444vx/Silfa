#include "shell.h"
#include "sfs.h"

static unsigned short *vga = (unsigned short *)0xB8000;
static char input[256];
static int input_len = 0;
static int shell_row = 3;
static int shell_col = 0;

static void vga_putchar(char c) {
    if (c == '\n') {
        shell_row++;
        shell_col = 0;
    } else {
        vga[shell_row * 80 + shell_col] = (0x07 << 8) | (unsigned char)c;
        shell_col++;
        if (shell_col >= 80) {
            shell_col = 0;
            shell_row++;
        }
    }
    if (shell_row >= 25) {
        for (int row = 0; row < 24; row++) {
            for (int col = 0; col < 80; col++) {
                vga[row * 80 + col] = vga[(row + 1) * 80 + col];
            }
        }
        for (int col = 0; col < 80; col++) {
            vga[24 * 80 + col] = (0x07 << 8) | ' ';
        }
        shell_row = 24;
    }
}

static void vga_print(const char *msg) {
    while (*msg) {
        vga_putchar(*msg++);
    }
}

static void vga_clear(void) {
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = (0x07 << 8) | ' ';
    }
    shell_row = 3;
    shell_col = 0;
}

static int strcmp(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a - *b;
}

static int strncmp(const char *a, const char *b, int n) {
    while (n-- && *a && *b && *a == *b) { a++; b++; }
    if (n < 0) return 0;
    return *a - *b;
}

static int strlen(const char *s) {
    int n = 0;
    while (s[n]) n++;
    return n;
}

static void shell_prompt(void) {
    vga_print("silfa> ");
}

static void shell_execute(void) {
    vga_putchar('\n');

    if (strcmp(input, "help") == 0) {
        vga_print("Commands:\n");
        vga_print("  help               - show commands\n");
        vga_print("  clear              - clear screen\n");
        vga_print("  ls                 - list files\n");
        vga_print("  create <name>      - create file\n");
        vga_print("  write <name> <txt> - write to file\n");
        vga_print("  read <name>        - read file\n");
        vga_print("  delete <name>      - delete file\n");
        vga_print("  echo <text>        - print text\n");
        vga_print("  version            - show version\n");

    } else if (strcmp(input, "clear") == 0) {
        vga_clear();
        vga_print("Silfa Kernel 0.1\n");
        vga_print("Type 'help' for commands\n\n");

    } else if (strcmp(input, "ls") == 0) {
        int found = 0;
        for (int i = 0; i < SFS_MAX_FILES; i++) {
            char name[SFS_NAME_LEN];
            if (sfs_getname(i, name)) {
                vga_print("  ");
                vga_print(name);
                vga_putchar('\n');
                found = 1;
            }
        }
        if (!found) vga_print("  no files\n");

    } else if (strncmp(input, "create ", 7) == 0) {
        const char *name = input + 7;
        if (sfs_create(name) >= 0) {
            vga_print("Created: ");
            vga_print(name);
            vga_putchar('\n');
        } else {
            vga_print("Error: file exists or no space\n");
        }

    } else if (strncmp(input, "write ", 6) == 0) {
        char name[SFS_NAME_LEN];
        int i = 6;
        int j = 0;
        while (input[i] && input[i] != ' ' && j < SFS_NAME_LEN - 1) {
            name[j++] = input[i++];
        }
        name[j] = 0;
        if (input[i] == ' ') i++;
        const char *data = input + i;
        int len = strlen(data);
        if (sfs_write(name, data, len) == 0) {
            vga_print("Written to: ");
            vga_print(name);
            vga_putchar('\n');
        } else {
            sfs_create(name);
            if (sfs_write(name, data, len) == 0) {
                vga_print("Written to: ");
                vga_print(name);
                vga_putchar('\n');
            } else {
                vga_print("Error: could not write\n");
            }
        }

    } else if (strncmp(input, "read ", 5) == 0) {
        const char *name = input + 5;
        char buf[4096];
        int n = sfs_read(name, buf, 4095);
        if (n >= 0) {
            buf[n] = 0;
            vga_print(buf);
            vga_putchar('\n');
        } else {
            vga_print("Error: file not found\n");
        }

    } else if (strncmp(input, "delete ", 7) == 0) {
        const char *name = input + 7;
        if (sfs_delete(name) == 0) {
            vga_print("Deleted: ");
            vga_print(name);
            vga_putchar('\n');
        } else {
            vga_print("Error: file not found\n");
        }

    } else if (strncmp(input, "echo ", 5) == 0) {
        vga_print(input + 5);
        vga_putchar('\n');

    } else if (strcmp(input, "version") == 0) {
        vga_print("Silfa Kernel 0.1\n");

    } else if (input_len > 0) {
        vga_print("Unknown command: ");
        vga_print(input);
        vga_putchar('\n');
    }

    input_len = 0;
    input[0]  = 0;
    shell_prompt();
}

void shell_input(char c) {
    if (c == '\n') {
        shell_execute();
        return;
    }
    if (c == '\b') {
        if (input_len > 0) {
            input_len--;
            input[input_len] = 0;
            if (shell_col > 0) {
                shell_col--;
                vga[shell_row * 80 + shell_col] = (0x07 << 8) | ' ';
            }
        }
        return;
    }
    if (input_len < 255) {
        input[input_len++] = c;
        input[input_len]   = 0;
        vga_putchar(c);
    }
}

void shell_init(void) {
    shell_row = 3;
    shell_col = 0;
    shell_prompt();
}