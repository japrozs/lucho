#include "lucho.h"

struct mach_object_file_t* parse_file(const char* file_path)
{
    struct mach_object_file_t* object_file = (struct mach_object_file_t*)malloc(sizeof(struct mach_object_file_t));
    if (object_file == NULL) {
        printf("%serror%s: could not allocate memory necessary for parsing\n", RED_BOLD, RESET);
        exit(1);
    }

    FILE* file = fopen(file_path, "rb");
    fread(&object_file->magic, sizeof(uint32_t), 1, file);
    fread(&object_file->cpu_type, sizeof(uint32_t), 1, file);
    fread(&object_file->cpu_subtype, sizeof(uint32_t), 1, file);
    fread(&object_file->file_type, sizeof(uint32_t), 1, file);
    fread(&object_file->number_of_load_commands, sizeof(uint32_t), 1, file);
    fread(&object_file->size_of_load_commands, sizeof(uint32_t), 1, file);
    fread(&object_file->flags, sizeof(uint32_t), 1, file);
    fread(&object_file->reserved, sizeof(uint32_t), 1, file);

    object_file->commands = ALLOC(struct load_command_t, object_file->number_of_load_commands);
    for (uint32_t i = 0; i < object_file->number_of_load_commands; i++) {
        uint32_t cmd, cmd_size;
        fread(&cmd, sizeof(uint32_t), 1, file);
        fread(&cmd_size, sizeof(uint32_t), 1, file);

        switch (cmd) {
        case LC_SEGMENT_64: {
            struct segment_command_64_t segment;

            fread(segment.segname, sizeof(uint8_t), 16, file);
            fread(&segment.vmaddr, sizeof(uint64_t), 1, file);
            fread(&segment.vmsize, sizeof(uint64_t), 1, file);
            fread(&segment.fileoff, sizeof(uint64_t), 1, file);
            fread(&segment.filesize, sizeof(uint64_t), 1, file);
            fread(&segment.maxprot, sizeof(vm_prot_t), 1, file);
            fread(&segment.initprot, sizeof(vm_prot_t), 1, file);
            fread(&segment.nsects, sizeof(uint32_t), 1, file);
            fread(&segment.flags, sizeof(uint32_t), 1, file);

            struct load_command_t command = {
                .cmd = cmd,
                .cmd_size = cmd_size,
                .cmd_seg_64 = segment,
            };
            object_file->commands[i] = command;

            break;
        }
        default:
            printf("%serror%s: encountered unexpected tag with value 0x%08x\n", RED_BOLD, RESET, cmd);
            // exit(1);
        }
    }

    fclose(file);

    return object_file;
}
