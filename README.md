# stupidos-uefi-loader

## 功能

- 基于edk2工程，加载elf64格式的内核文件

## 使用

- 将该目录放在edk2目录中
- build -p ./stupidos-uefi-loader/stupidos-uefi-loader.dsc -a X64 -t GCC
- 目标EFI文件:edk2/Build/stupidos-uefi-loader/DEBUG_GCC/X64/stupidos-uefi-loader.efi