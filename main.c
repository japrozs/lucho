#include "lucho.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
        printf("%serror%s: incorrect number of arguments\n", RED_BOLD, RESET);
        printf("\n%susage%s: %s <filename>\n", WHITE_BOLD, RESET, argv[0]);
        exit(0);
    }

    const char* file_path = argv[1];
    struct mach_object_file_t* file = parse_file(file_path);
    pretty_print(file);

    return 0;
}
