#include "lucho.h"

void pretty_print(struct mach_object_file_t* file)
{
    printf("magic                       : 0x%08x\n"
           "cpu_type                    : 0x%08x\n"
           "cpu_subtype                 : 0x%08x\n"
           "file_type                   : 0x%08x\n"
           "number_of_load_commands     : 0x%08x\n"
           "size_of_load_commands       : 0x%08x\n"
           "flags                       : 0x%08x\n"
           "reserved                    : 0x%08x\n",
        file->magic, file->cpu_type, file->cpu_subtype, file->file_type, file->number_of_load_commands, file->size_of_load_commands, file->flags, file->reserved);
    printf("---------------------------------------\n");
    for (uint32_t i = 0; i < file->number_of_load_commands; i++) {
        struct load_command_t cmd = file->commands[i];

        switch (cmd.cmd) {
        case LC_SEGMENT_64: {
            printf("LC_SYMTAB (0x%08x)\n", cmd.cmd);
            printf("\tsegname       : \"%s\"\n", cmd.cmd_seg_64.segname);
            printf("\tvmaddr        : 0x%016llx\n", cmd.cmd_seg_64.vmaddr);
            printf("\tvmsize        : 0x%016llx\n", cmd.cmd_seg_64.vmaddr);
            printf("\tfileoff       : 0x%016llx\n", cmd.cmd_seg_64.fileoff);
            printf("\tfilesize      : 0x%016llx\n", cmd.cmd_seg_64.filesize);
            printf("\tmaxprot       : 0x%08x\n", cmd.cmd_seg_64.maxprot);
            printf("\tinitprot      : 0x%08x\n", cmd.cmd_seg_64.initprot);
            printf("\tnsects        : 0x%08x\n", cmd.cmd_seg_64.nsects);
            printf("\tflags         : 0x%08x\n", cmd.cmd_seg_64.flags);
            printf("\n");

            // print the sections
            break;
        }
        default:
            printf("unknown opcode encountered : 0x%08x\n", cmd.cmd);
            break;
        }
    }
}
