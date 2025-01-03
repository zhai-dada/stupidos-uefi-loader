#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/SimpleFileSystem.h>
#include <Protocol/LoadedImage.h>
#include <Guid/FileInfo.h>
#include "ElfLib/ElfCommon.h"
#include "ElfLib/Elf64.h"
#include "ElfLib.h"

#include "baseinfo.h"

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable)
{
    EFI_LOADED_IMAGE *loadimage;
    EFI_FILE_IO_INTERFACE *volumn;
    EFI_FILE_HANDLE rootfs;
    EFI_FILE_HANDLE filehandle;
    ELF_IMAGE_CONTEXT elf_context;
    INT32 i = 0;
    VOID(*kernel)
    (VOID);

    EFI_STATUS status = EFI_SUCCESS;
    struct KERNEL_BOOT_PARAMETER_INFORMATION *kernel_boot_para_info = NULL;

    // read file to memory
    gBS->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (VOID **)&loadimage);
    gBS->HandleProtocol(loadimage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (VOID **)&volumn);
    volumn->OpenVolume(volumn, &rootfs);
    status = rootfs->Open(rootfs, &filehandle, (CHAR16 *)L"osImage", EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status))
    {
        Print(L"Open osImage Failed.\n");
        return status;
    }
    EFI_FILE_INFO *fileinfo;
    UINT64 buffersize = 0;

    buffersize = sizeof(EFI_FILE_INFO) + sizeof(CHAR16) * 100;
    gBS->AllocatePool(EfiRuntimeServicesData, buffersize, (VOID **)&fileinfo);
    filehandle->GetInfo(filehandle, &gEfiFileInfoGuid, &buffersize, fileinfo);
    Print(L"FileName:%s\t Size:%d\t FileSize:%d\t Physical Size:%d\n",
          fileinfo->FileName, fileinfo->Size, fileinfo->FileSize, fileinfo->PhysicalSize);
    // alloc runtime memory
    UINT64 readfile_memory = 0;
    gBS->AllocatePool(EfiRuntimeServicesData, fileinfo->FileSize + 0x1000, (VOID **)&readfile_memory);

    buffersize = fileinfo->FileSize;
    filehandle->Read(filehandle, &buffersize, (VOID *)readfile_memory);
    gBS->FreePool(fileinfo); // free fileinfo
    filehandle->Close(filehandle);
    rootfs->Close(rootfs);

    EFI_PHYSICAL_ADDRESS elf_pages = 0x100000; // 1MB
    status = ParseElfImage((VOID *)readfile_memory, &elf_context);
    if (EFI_ERROR(status))
    {
        Print(L"Parse osImage Failed.\n");
        return status;
    }

    gBS->AllocatePages(AllocateAddress, EfiLoaderData, (elf_context.ImageSize + 0x1000 - 1) / 0x1000, &elf_pages);

    elf_context.ImageAddress = (UINT8 *)elf_pages;

    status = LoadElfImage(&elf_context);
    if (EFI_ERROR(status))
    {
        Print(L"load osImage Failed. %x \n", status);
        return status;
    }

    gBS->FreePool((VOID *)readfile_memory);

    // set display mode
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graphics_output = 0;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *graphics_output_info = 0;
    UINT64 InfoSize = 0;

    readfile_memory = 0x60000;
    kernel_boot_para_info = (struct KERNEL_BOOT_PARAMETER_INFORMATION *)0x60000;
    gBS->AllocatePages(AllocateAddress, EfiLoaderData, 1, &readfile_memory);
    gBS->SetMem((VOID *)kernel_boot_para_info, 0x1000, 0);

    gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&graphics_output);

    INT32 MaxResolutionMode = 7;

    for (i = 0; i < graphics_output->Mode->MaxMode; i++)
    {
        graphics_output->QueryMode(graphics_output, i, &InfoSize, &graphics_output_info);
        if ((graphics_output_info->PixelFormat == 1) &&
            (graphics_output_info->HorizontalResolution == 1024 && graphics_output_info->VerticalResolution == 768))
        {
            MaxResolutionMode = i;
        }
        gBS->FreePool(graphics_output_info);
    }

    graphics_output->SetMode(graphics_output, MaxResolutionMode);

    kernel_boot_para_info->graphics_info.HorizontalResolution = graphics_output->Mode->Info->HorizontalResolution;
    kernel_boot_para_info->graphics_info.VerticalResolution = graphics_output->Mode->Info->VerticalResolution;
    kernel_boot_para_info->graphics_info.PixelsPerScanLine = graphics_output->Mode->Info->PixelsPerScanLine;
    kernel_boot_para_info->graphics_info.FrameBufferBase = graphics_output->Mode->FrameBufferBase;
    kernel_boot_para_info->graphics_info.FrameBufferSize = graphics_output->Mode->FrameBufferSize;

    // map frameaddress to virtual address 0xffff800003000000
    INT64 *PageTableEntry = (INT64 *)0x103000;
    UINT32 display_buffer_count = (graphics_output->Mode->FrameBufferSize + 0x200000 - 1) >> 21;
    UINT32 display_buffer_start = (graphics_output->Mode->FrameBufferBase) >> 21;
    for (i = 0; i < display_buffer_count; i++)
    {
        *(PageTableEntry + 0x18 + i) = 0x200000 * (i + display_buffer_start) | 0x87;
    }

    // get e820 info
    struct EFI_E820_MEMORY_DESCRIPTOR *e820_pointer = kernel_boot_para_info->e820_info.e820_entry;
    struct EFI_E820_MEMORY_DESCRIPTOR *last_e820 = NULL;
    UINT64 last_end_addr = 0;
    INT32 e820_count = 0;

    UINT64 mem_map_size = 0;
    EFI_MEMORY_DESCRIPTOR *mem_map = NULL;
    UINT64 mem_key = 0;
    UINT64 mem_descriptor_size = 0;
    UINT32 mem_descriptor_version = 0;

    gBS->GetMemoryMap(&mem_map_size, mem_map, &mem_key, &mem_descriptor_size, &mem_descriptor_version);
    // Print(L"Get mem_map_size:%d,DescriptorSize:%d,count:%d\n", mem_map_size, mem_descriptor_size, mem_map_size / mem_descriptor_size);
    mem_map_size += 0x5 * mem_descriptor_size; // safer
    gBS->AllocatePool(EfiRuntimeServicesData, mem_map_size, (VOID **)&mem_map);
    gBS->SetMem((VOID *)mem_map, mem_map_size, 0);
    gBS->GetMemoryMap(&mem_map_size, mem_map, &mem_key, &mem_descriptor_size, &mem_descriptor_version);
    // Print(L"Get mem_map_size:%d,DescriptorSize:%d,count:%d\n", mem_map_size, mem_descriptor_size, mem_map_size / mem_descriptor_size);

    // Print(L"Get EFI_MEMORY_DESCRIPTOR Structure:%018lx\n", mem_map);
    for (i = 0; i < mem_map_size / mem_descriptor_size; i++)
    {
        INT32 MemType = 0;
        EFI_MEMORY_DESCRIPTOR *MMap = (EFI_MEMORY_DESCRIPTOR *)((CHAR8 *)mem_map + i * mem_descriptor_size);
        if (MMap->NumberOfPages == 0)
        {
            continue;
        }
        // Print(L"MemoryMap %4d %10d (%16lx<->%16lx) %016lx\n", MMap->Type, MMap->NumberOfPages, MMap->PhysicalStart, MMap->PhysicalStart + (MMap->NumberOfPages << 12), MMap->Attribute);
        switch (MMap->Type)
        {
            case EfiReservedMemoryType:
            case EfiMemoryMappedIO:
            case EfiMemoryMappedIOPortSpace:
            case EfiPalCode:
            {
                MemType = 2; // 2:ROM or Reserved
                break;
            }
            case EfiUnusableMemory:
            {
                MemType = 5; // 5:Unusable
                break;
            }
            case EfiACPIReclaimMemory:
            {
                MemType = 3; // 3:ACPI Reclaim Memory
                break;
            }
            case EfiLoaderCode:
            case EfiLoaderData:
            case EfiBootServicesCode:
            case EfiBootServicesData:
            case EfiRuntimeServicesCode:
            case EfiRuntimeServicesData:
            case EfiConventionalMemory:
            case EfiPersistentMemory:
            {
                MemType = 1; // 1:RAM
                break;
            }
            case EfiACPIMemoryNVS:
            {

                MemType = 4; // 4:ACPI NVS Memory
                break;
            }
            default:
            {
                Print(L"Invalid UEFI Memory Type:%4d\n", MMap->Type);
                continue;
            }
        }

        if ((last_e820 != NULL) && (last_e820->type == MemType) && (MMap->PhysicalStart == last_end_addr))
        {
            last_e820->length += MMap->NumberOfPages << 12;
            last_end_addr += MMap->NumberOfPages << 12;
        }
        else
        {
            e820_pointer->address = MMap->PhysicalStart;
            e820_pointer->length = MMap->NumberOfPages << 12;
            e820_pointer->type = MemType;
            last_end_addr = MMap->PhysicalStart + (MMap->NumberOfPages << 12);
            last_e820 = e820_pointer;
            e820_pointer++;
            e820_count++;
        }
    }

    // sort
    kernel_boot_para_info->e820_info.e820_entrycount = e820_count;
    last_e820 = kernel_boot_para_info->e820_info.e820_entry;
    INT32 j = 0;
    for (i = 0; i < e820_count; i++)
    {
        struct EFI_E820_MEMORY_DESCRIPTOR *e820i = last_e820 + i;
        struct EFI_E820_MEMORY_DESCRIPTOR tmp;
        for (j = i + 1; j < e820_count; j++)
        {
            struct EFI_E820_MEMORY_DESCRIPTOR *e820j = last_e820 + j;
            if (e820i->address > e820j->address)
            {
                tmp = *e820i;
                *e820i = *e820j;
                *e820j = tmp;
            }
        }
    }

    last_e820 = kernel_boot_para_info->e820_info.e820_entry;
    // for (i = 0; i < e820_count; i++)
    // {
    //     // Print(L"MemoryMap (%10lx<->%10lx) %4d\n", last_e820->address, last_e820->address + last_e820->length, last_e820->type);
    //     // last_e820++;
    // }
    gBS->FreePool(mem_map);

    // Print(L"Call ExitBootServices And Jmp to Kernel.\n");
    gBS->GetMemoryMap(&mem_map_size, mem_map, &mem_key, &mem_descriptor_size, &mem_descriptor_version);

    kernel = (VOID *)elf_context.EntryPoint;
    gBS->CloseProtocol(loadimage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, ImageHandle, NULL);
    gBS->CloseProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, ImageHandle, NULL);

    gBS->CloseProtocol(graphics_output, &gEfiGraphicsOutputProtocolGuid, ImageHandle, NULL);

    status = gBS->ExitBootServices(ImageHandle, mem_key);
    if (EFI_ERROR(status))
    {
        Print(L"ExitBootServices: Failed, Memory Map has Changed.\n");
        return EFI_INVALID_PARAMETER;
    }

    // start kernel
    kernel();

    return status;
}