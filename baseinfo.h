struct EFI_GRAPHICS_OUTPUT_INFORMATION
{
    unsigned int HorizontalResolution;
    unsigned int VerticalResolution;
    unsigned int PixelsPerScanLine;

    unsigned long FrameBufferBase;
    unsigned long FrameBufferSize;
};

struct EFI_E820_MEMORY_DESCRIPTOR
{
    unsigned long address;
    unsigned long length;
    unsigned int type;
} __attribute__((packed));

struct EFI_E820_MEMORY_DESCRIPTOR_INFORMATION
{
    unsigned int e820_entrycount;
    struct EFI_E820_MEMORY_DESCRIPTOR e820_entry[0];
};

struct KERNEL_BOOT_PARAMETER_INFORMATION
{
    struct EFI_GRAPHICS_OUTPUT_INFORMATION graphics_info;
    struct EFI_E820_MEMORY_DESCRIPTOR_INFORMATION e820_info;
};