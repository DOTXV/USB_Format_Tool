#ifndef FORMAT_TOOL_H
#define FORMAT_TOOL_H

bool format_main(System::String^ disk,int MBR ,int FAT_type, int cluster_size, int reserved_offset);
extern volatile int progress;

#endif