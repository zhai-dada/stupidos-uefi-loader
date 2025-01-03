[Defines]
	PLATFORM_NAME				= stupidos-uefi-loader
	PLATFORM_GUID				= B02121B6-14CB-0F99-613E-B201995892BF
	PLATFORM_VERSION 			= 0.01
	DSC_SPECIFICATION			= 0x00010005
	OUTPUT_DIRECTORY 			= Build/stupidos-uefi-loader
	SUPPORTED_ARCHITECTURES		= X64
	BUILD_TARGETS				= DEBUG|RELEASE|NOOPT
	SKUID_IDENTIFIER 			= DEFAULT

[LibraryClasses]
	#
	# Entry point
	#
	PeiCoreEntryPoint|MdePkg/Library/PeiCoreEntryPoint/PeiCoreEntryPoint.inf
	PeimEntryPoint|MdePkg/Library/PeimEntryPoint/PeimEntryPoint.inf
	DxeCoreEntryPoint|MdePkg/Library/DxeCoreEntryPoint/DxeCoreEntryPoint.inf
	UefiDriverEntryPoint|MdePkg/Library/UefiDriverEntryPoint/UefiDriverEntryPoint.inf
	UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
	#
	# Basic
	#
	BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
	BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
	SynchronizationLib|MdePkg/Library/BaseSynchronizationLib/BaseSynchronizationLib.inf
	PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
	IoLib|MdePkg/Library/BaseIoLibIntrinsic/BaseIoLibIntrinsic.inf
	PciLib|MdePkg/Library/BasePciLibCf8/BasePciLibCf8.inf
	PciCf8Lib|MdePkg/Library/BasePciCf8Lib/BasePciCf8Lib.inf
	PciSegmentLib|MdePkg/Library/BasePciSegmentLibPci/BasePciSegmentLibPci.inf
	CacheMaintenanceLib|MdePkg/Library/BaseCacheMaintenanceLib/BaseCacheMaintenanceLib.inf
	PeCoffLib|MdePkg/Library/BasePeCoffLib/BasePeCoffLib.inf
	PeCoffGetEntryPointLib|MdePkg/Library/BasePeCoffGetEntryPointLib/BasePeCoffGetEntryPointLib.inf
	SortLib|MdeModulePkg/Library/BaseSortLib/BaseSortLib.inf
	#
	# UEFI & PI
	#
	UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
	UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
	UefiRuntimeLib|MdePkg/Library/UefiRuntimeLib/UefiRuntimeLib.inf
	UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
	UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
	HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
	DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf
	UefiDecompressLib|MdePkg/Library/BaseUefiDecompressLib/BaseUefiDecompressLib.inf
	PeiServicesTablePointerLib|MdePkg/Library/PeiServicesTablePointerLib/PeiServicesTablePointerLib.inf
	PeiServicesLib|MdePkg/Library/PeiServicesLib/PeiServicesLib.inf
	DxeServicesLib|MdePkg/Library/DxeServicesLib/DxeServicesLib.inf
	DxeServicesTableLib|MdePkg/Library/DxeServicesTableLib/DxeServicesTableLib.inf
	UefiBootManagerLib|MdeModulePkg/Library/UefiBootManagerLib/UefiBootManagerLib.inf
	VariablePolicyLib|MdeModulePkg/Library/VariablePolicyLib/VariablePolicyLib.inf
	#
	# Generic Modules
	#
	UefiUsbLib|MdePkg/Library/UefiUsbLib/UefiUsbLib.inf
	EfiLib|UefiPayloadPkg/PayloadLoaderPeim/PayloadLoaderPeim.inf
	UefiScsiLib|MdePkg/Library/UefiScsiLib/UefiScsiLib.inf
	SecurityManagementLib|MdeModulePkg/Library/DxeSecurityManagementLib/DxeSecurityManagementLib.inf
	TimerLib|MdePkg/Library/BaseTimerLibNullTemplate/BaseTimerLibNullTemplate.inf
	SerialPortLib|MdePkg/Library/BaseSerialPortLibNull/BaseSerialPortLibNull.inf
	CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
	PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
	CustomizedDisplayLib|MdeModulePkg/Library/CustomizedDisplayLib/CustomizedDisplayLib.inf
	FrameBufferBltLib|MdeModulePkg/Library/FrameBufferBltLib/FrameBufferBltLib.inf
	#
	# Misc
	#
	DebugLib|MdePkg/Library/BaseDebugLibNull/BaseDebugLibNull.inf
	DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf
	ReportStatusCodeLib|MdePkg/Library/BaseReportStatusCodeLibNull/BaseReportStatusCodeLibNull.inf
	PeCoffExtraActionLib|MdePkg/Library/BasePeCoffExtraActionLibNull/BasePeCoffExtraActionLibNull.inf
	PerformanceLib|MdePkg/Library/BasePerformanceLibNull/BasePerformanceLibNull.inf
	DebugAgentLib|MdeModulePkg/Library/DebugAgentLibNull/DebugAgentLibNull.inf
	PlatformHookLib|MdeModulePkg/Library/BasePlatformHookLibNull/BasePlatformHookLibNull.inf
	ResetSystemLib|MdeModulePkg/Library/BaseResetSystemLibNull/BaseResetSystemLibNull.inf
	SmbusLib|MdePkg/Library/DxeSmbusLib/DxeSmbusLib.inf
	S3BootScriptLib|MdeModulePkg/Library/PiDxeS3BootScriptLib/DxeS3BootScriptLib.inf
	CpuExceptionHandlerLib|MdeModulePkg/Library/CpuExceptionHandlerLibNull/CpuExceptionHandlerLibNull.inf
	PlatformBootManagerLib|MdeModulePkg/Library/PlatformBootManagerLibNull/PlatformBootManagerLibNull.inf
	PciHostBridgeLib|MdeModulePkg/Library/PciHostBridgeLibNull/PciHostBridgeLibNull.inf
	TpmMeasurementLib|MdeModulePkg/Library/TpmMeasurementLibNull/TpmMeasurementLibNull.inf
	AuthVariableLib|MdeModulePkg/Library/AuthVariableLibNull/AuthVariableLibNull.inf
	VarCheckLib|MdeModulePkg/Library/VarCheckLib/VarCheckLib.inf
	FileExplorerLib|MdeModulePkg/Library/FileExplorerLib/FileExplorerLib.inf
	NonDiscoverableDeviceRegistrationLib|MdeModulePkg/Library/NonDiscoverableDeviceRegistrationLib/NonDiscoverableDeviceRegistrationLib.inf
	ImagePropertiesRecordLib|MdeModulePkg/Library/ImagePropertiesRecordLib/ImagePropertiesRecordLib.inf
	RegisterFilterLib|MdePkg/Library/RegisterFilterLibNull/RegisterFilterLibNull.inf
	FmpAuthenticationLib|MdeModulePkg/Library/FmpAuthenticationLibNull/FmpAuthenticationLibNull.inf
	CapsuleLib|MdeModulePkg/Library/DxeCapsuleLibNull/DxeCapsuleLibNull.inf
	BmpSupportLib|MdeModulePkg/Library/BaseBmpSupportLib/BaseBmpSupportLib.inf
	SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
	MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
	DisplayUpdateProgressLib|MdeModulePkg/Library/DisplayUpdateProgressLibGraphics/DisplayUpdateProgressLibGraphics.inf
	VariablePolicyHelperLib|MdeModulePkg/Library/VariablePolicyHelperLib/VariablePolicyHelperLib.inf
	MmUnblockMemoryLib|MdePkg/Library/MmUnblockMemoryLib/MmUnblockMemoryLibNull.inf
	VariableFlashInfoLib|MdeModulePkg/Library/BaseVariableFlashInfoLib/BaseVariableFlashInfoLib.inf
	IpmiCommandLib|MdeModulePkg/Library/BaseIpmiCommandLibNull/BaseIpmiCommandLibNull.inf
	SpiHcPlatformLib|MdeModulePkg/Library/BaseSpiHcPlatformLibNull/BaseSpiHcPlatformLibNull.inf
	SafeIntLib|MdePkg/Library/BaseSafeIntLib/BaseSafeIntLib.inf
[Components]
	stupidos-uefi-loader/stupidos-uefi-loader.inf
