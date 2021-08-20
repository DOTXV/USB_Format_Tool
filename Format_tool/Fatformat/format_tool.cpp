#pragma once

#include "stdafx.h"           //Visual C++ will not compile anything before #include "stdafx.h"
#include <Windows.h>
#include <WinIoCtl.h>
#include <devioctl.h>
#include <ntddscsi.h>
#include <stdlib.h>
#include <string.h>
#include <iostream> 
#include <fstream>
#include <msclr\marshal.h>

#include "upcase.h"
#include "format_tool.h"

using namespace msclr::interop;

volatile int progress = 0;
int MBR_start_sector = 0;
DWORD FatSize;
int resvd_count;
int cluster_offset = 10240;

//For exfat 
int exfat_fatlen;
int exfat_fatoffset;
int exfat_clustercount;
int exfat_clusterheapoffset;
int exfat_rootdict;
int exfat_upcasedict;

int upcasesize;
int bitallcsize;
BYTE clearbuff[51200] = { 0 };

int fat32_fatoffset;

#pragma pack(push, 1)
struct _stCDB
{
	BYTE bCDB[16];
};
struct FAT32_BOOTSECTOR
{
	BYTE sJmpBoot[3];
	CHAR sOEMName[8];
	BYTE wBytsPerSec[2];
	BYTE bSecPerClus;
	WORD wRsvdSecCnt;            //WORD == 2 BYTE
	BYTE bNumFATs;
	BYTE wRootEntCnt[2];
	BYTE wTotSec16[2];
	BYTE bMedia;
	WORD wFATSz16;
	BYTE wSecPerTrk[2];
	BYTE wNumHeads[2];
	DWORD dHiddSec;              //DWORD == 4 BYTE
	DWORD dTotSec32;
	DWORD dFATSz32;
	WORD wExtFlags;
	WORD wFSVer;
	DWORD dRootClus;
	WORD wFSInfo;
	WORD wBkBootSec;
	BYTE Reserved[12];
	BYTE bDrvNum;
	BYTE Reserved1;
	BYTE bBootSig;
	DWORD dBS_VolID;
	BYTE sVolLab[11];
	BYTE sBS_FilSysType[8];
};
struct exFAT_BOOTSECTOR
{
	// Common fields.
	BYTE sJmpBoot[3];
	CHAR FileSystemName[8];
	BYTE MustBeZero[53];
	LONGLONG PartitionOffset;
	LONGLONG VolumeLength;
	DWORD dFatOffset;
	DWORD dFatLength;
	DWORD dClusterHeapOffset;
	DWORD dClusterCount;
	DWORD dFirstClusterOfRootDirectory;
	DWORD dVolumeSerialNumberl;
	WORD wFileSystemRevision;
	WORD wVolumeFlags;
	BYTE BytesPerSectorShift;
	BYTE SectorsPerClusterShift;
	BYTE NumberOfFats;
	BYTE DriveSelect;
	BYTE PercentInUse;                         //Volume usage in unit of percent. 0xFF means this field is not used and invalid.
	BYTE Reserved[7];
	BYTE BootCode[390];
};
struct FAT32_FSINFO
{
	DWORD dLeadSig;              //0x41615252
	BYTE sReserved1[480];        //zeros
	DWORD dStrucSig;             //0x61417272
	DWORD dFree_Count;           //0xFFFFFFFF
	DWORD dNxt_Free;             //0xFFFFFFFF
	BYTE sReserved2[12];         //zeros
	DWORD dTrailSig;             //0xAA550000
}; // FSInfo structure in order to avoid to read over an entire FAT to look for free clusters or get count of free clusters.
struct exFAT_RESERVED
{
	BYTE Reservedsec1[512];
	BYTE Reservedsec2[512];
	BYTE Reservedsec3[512];
	BYTE Reservedsec4[512];
	BYTE Reservedsec5[512];
	BYTE Reservedsec6[512];
	BYTE Reservedsec7[512];
	BYTE Reservedsec8[512];
	BYTE Reservedsec9[512];      //Sector 9 contains OEM parameter
	BYTE Reservedsec10[512];     //Sector 10 is reserved for future use. The content of these sectors is depends on the system and filled with zero when not used.
}; //Extend Boot Sectors
struct MBR_SECTOR 
{
	BYTE CodeArea[446];
	BYTE Boot_Indicator1;            ///
	BYTE startCHS1[3];
	BYTE type1;                      //  First part 16 BYTES
	BYTE endCHS1[3];
	DWORD StartSector1;
	DWORD PartitionSize1;            
	BYTE Boot_Indicator2;
	BYTE startCHS2[3];
	BYTE type2;
	BYTE endCHS2[3];
	DWORD StartSector2;
	DWORD PartitionSize2;
	BYTE Boot_Indicator3;
	BYTE startCHS3[3];
	BYTE type3;
	BYTE endCHS3[3];
	DWORD StartSector3;
	DWORD PartitionSize3;
	BYTE Boot_Indicator4;
	BYTE startCHS4[3];
	BYTE type4;
	BYTE endCHS4[3];
	DWORD StartSector4;
	DWORD PartitionSize4;
};
struct ZERO_SECTOR
{
	DWORD RES1;
	DWORD RES2;
	DWORD RES3;
	DWORD RES4;
	DWORD RES5;
	DWORD RES6;
	DWORD RES7;
	DWORD RES8;
	DWORD RES9;
	DWORD RES10;
	DWORD RES11;
	DWORD RES12;
	DWORD RES13;
	DWORD RES14;
	DWORD RES15;
	DWORD RES16;
	DWORD RES17;
	DWORD RES18;
	DWORD RES19;
	DWORD RES20;
	BYTE ZERO[432];           //432 + 4 * 20 = 512
};

struct exfatFile          //File Entry 32 Byte each
{        
	DWORD file1_1;
	DWORD file1_2;
	DWORD file1_3;
	DWORD file1_4;
	DWORD file1_5;
	DWORD file1_6;
	DWORD file1_7;
	DWORD file1_8;        //For volume label
	DWORD file2_1;
	DWORD file2_2;
	DWORD file2_3;
	DWORD file2_4;
	DWORD file2_5;
	DWORD file2_6;
	DWORD file2_7;
	DWORD file2_8;        //For up-case
	DWORD file3_1;
	DWORD file3_2;
	DWORD file3_3;
	DWORD file3_4;
	DWORD file3_5;
	DWORD file3_6;
	DWORD file3_7;
	DWORD file3_8;       //For bitmap
};                                 

typedef struct _SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE
{
	SCSI_PASS_THROUGH_DIRECT sptd;
	DWORD filler;	                 // align abRequestSense to DWORD boundary
	BYTE abRequestSense[24];

}SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE, *PSCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE;
#pragma pack(pop)

//define
DWORD ScsiCmdSend(HANDLE hDev, _stCDB stCDB, BYTE bDirection, BYTE bCdbLen, void *pData, DWORD dwDataXferLen, DWORD dwTimeout = 60);

bool write_value(HANDLE Dev, BYTE *write_data, int offset_sector, int write_size);
bool read_value(HANDLE Dev, BYTE *read_buffer, int offset_sector, int read_size);
bool format_main(System::String^ disk, int MBR, int FAT_type, int cluster_size, int reserved_offset);
int read_disk_size(HANDLE dsk);

DWORD get_volume_id();
BYTE* FAT32_boot_sector_setting(HANDLE dsk, int cluster_size, int reserved_offset);
BYTE* FAT32_fsinfo_sector_setting(HANDLE dsk, int usable_size, int free_count);
BYTE* exFAT_boot_sector_setting(HANDLE dsk, int cluster_size, int reserved_offset, int MBR);
BYTE* exFAT_reserved_sector_setting();
BYTE* MBR_sector_setting(HANDLE dsk, int type);
BYTE* exFAT_checksum(BYTE* boot_sec, BYTE* res_send, BYTE* res_zero);

bool DeviceLock(HANDLE hDevice);
bool DeviceUnLock(HANDLE hDevice);
int init_disk(HANDLE dsk);

DWORD GetFATSizeSectors(DWORD DskSize, DWORD ReservedSecCnt, DWORD SecPerClus, DWORD NumFATs, DWORD BytesPerSect);

//Initialize File Entry
BYTE* initFile(int c_size);
BYTE* initFATfile(int bitallc_size, int upcase_size);
BYTE* initBitfile(int bitallc_size, int upcase_size);


DWORD ScsiCmdSend(HANDLE hDev, _stCDB stCDB, BYTE bDirection, BYTE bCdbLen, void* pData, DWORD dwDataXferLen, DWORD dwTimeout)
{
	SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE sptd;
	BOOL xAPIStatus = FALSE;
	BYTE abRequestSense[24] = { 0 };
	DWORD dwByteReturn;
	ZeroMemory(&sptd, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE));          //Use on MS Windows, implement by "void *memset(void *s,int ch,size_t n);"
	sptd.sptd.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);                        //Contains the value of sizeof(SCSI_PASS_THROUGH_DIRECT).
	sptd.sptd.PathId = 0;                                                       //Indicates the SCSI port or bus for the request.
	sptd.sptd.TargetId = 1;                                                     //Indicates the target controller or device on the bus.
	sptd.sptd.Lun = 0;                                                          //Indicates the logical unit number of the device.
	sptd.sptd.CdbLength = 10;                                                   //Indicates the size in bytes of the SCSI command descriptor block.
	sptd.sptd.DataIn = (BYTE)bDirection;                                        //Indicates whether the SCSI command will read or write data.
	sptd.sptd.SenseInfoLength = sizeof(sptd.abRequestSense);                    //Indicates the size in bytes of the request-sense buffer.
	sptd.sptd.DataTransferLength = 512;                                         //Indicates the size in bytes of the data buffer.
	sptd.sptd.TimeOutValue = 2;
	sptd.sptd.DataBuffer = (pData == NULL) ? abRequestSense : pData;            //Pointer to the data buffer.
	//sptd.sptd.DataBuffer = write_test;
	sptd.sptd.SenseInfoOffset = offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE, abRequestSense); //Indicates the size in bytes of the request-sense buffer.
	int offset_sector = 2000;
	int write_size = 512;
	memcpy(sptd.sptd.Cdb, &stCDB, sizeof(sptd.sptd.Cdb));
	xAPIStatus = DeviceIoControl(hDev,
		IOCTL_SCSI_PASS_THROUGH_DIRECT,
		&sptd,
		sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE),
		&sptd,
		sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE),
		&dwByteReturn,
		FALSE);
	if ((sptd.sptd.ScsiStatus == 0) && (xAPIStatus != 0))
		return 0;
	return GetLastError();
}

/*
* SCSI_IOCTL_DATA_OUT: for write command
* SCSI_IOCTL_DATA_IN : for read command
* SCSI_IOCTL_DATA_UNSPECIFIED : for no data command
*/

bool DeviceLock(HANDLE hDevice)
{
	DWORD dwBytesReturned;
	if (0 == DeviceIoControl(hDevice, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL))
		return false;
	return true;
}
bool DeviceUnLock(HANDLE hDevice)
{
	DWORD dwBytesReturned;
	if (0 == DeviceIoControl(hDevice, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, &dwBytesReturned, NULL))
		return false;
	return true;
}
bool write_value(HANDLE Dev, BYTE *write_data, int offset_sector, int write_size) {
	int iRet;
	ULONG length = 0;
	DWORD bytesReturn;
	SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE sptdwb;
	sptdwb.sptd.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
	sptdwb.sptd.PathId = 0;
	sptdwb.sptd.TargetId = 1;
	sptdwb.sptd.Lun = 0;
	sptdwb.sptd.CdbLength = 10;
	sptdwb.sptd.DataIn = SCSI_IOCTL_DATA_OUT;
	sptdwb.sptd.SenseInfoLength = 24;
	sptdwb.sptd.DataTransferLength = 512 * write_size;
	sptdwb.sptd.TimeOutValue = 2;
	sptdwb.sptd.DataBuffer = write_data;
	sptdwb.sptd.SenseInfoOffset = offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE, abRequestSense);
	sptdwb.sptd.Cdb[0] = 0x2A;      //opcode: Host write data to storage device  //CDB發送給U盤設備SCSI命令描述符。
	sptdwb.sptd.Cdb[1] = 0x00;
	sptdwb.sptd.Cdb[2] = (offset_sector >> 24) & 0xff;           //Logical Block Address
	sptdwb.sptd.Cdb[3] = (offset_sector >> 16) & 0xff;
	sptdwb.sptd.Cdb[4] = (offset_sector >> 8) & 0xff;
	sptdwb.sptd.Cdb[5] = offset_sector & 0xff;
	sptdwb.sptd.Cdb[6] = 0x00;                                  //Reserved
	sptdwb.sptd.Cdb[7] = (write_size >> 8) & 0xff;              //Transfer Length
	sptdwb.sptd.Cdb[8] = write_size & 0xff;
	sptdwb.sptd.Cdb[9] = 0x00;                                  //Control
	length = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE);
	iRet = DeviceIoControl(Dev,
		IOCTL_SCSI_PASS_THROUGH_DIRECT,
		&sptdwb,
		length,
		&sptdwb,
		length,
		&bytesReturn,
		NULL);
	/*
	* "Sends a control code directly to a specified device driver, causing the corresponding device to perform the corresponding operation."
	* BOOL DeviceIoControl(
		HANDLE       hDevice,                   // A handle to the device on which the operation is to be performed.
		DWORD        dwIoControlCode,           // The control code for the operation.
		LPVOID       lpInBuffer,                // A pointer to the input buffer that contains the data required to perform the operation.
		DWORD        nInBufferSize,             // The size of the input buffer, in bytes.
		LPVOID       lpOutBuffer,               // A pointer to the output buffer that is to receive the data returned by the operation.
		DWORD        nOutBufferSize,            // The size of the output buffer, in bytes.
		LPDWORD      lpBytesReturned,           // A pointer to a variable that receives the size of the data stored in the output buffer, in bytes.
		LPOVERLAPPED lpOverlapped               // A pointer to an OVERLAPPED structure.
		);
	*/
	if (0 == iRet)
	{
		return 0;
	}
	return iRet;
}

bool read_value(HANDLE Dev, BYTE *read_buffer, int offset_sector, int read_size) {
	int iRet;
	ULONG length = 0;
	DWORD bytesReturn;
	SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE sptdwb;
	ZeroMemory(&sptdwb, sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE));
	sptdwb.sptd.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
	sptdwb.sptd.PathId = 0;
	sptdwb.sptd.TargetId = 1;
	sptdwb.sptd.Lun = 0;
	sptdwb.sptd.CdbLength = 10;
	sptdwb.sptd.DataIn = SCSI_IOCTL_DATA_IN;
	sptdwb.sptd.SenseInfoLength = 24;
	sptdwb.sptd.DataTransferLength = 512;
	sptdwb.sptd.TimeOutValue = 2;
	sptdwb.sptd.DataBuffer = read_buffer;
	sptdwb.sptd.SenseInfoOffset = offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE, abRequestSense);
	sptdwb.sptd.Cdb[0] = 0x28;        //opcode: Host read data from storage device
	sptdwb.sptd.Cdb[1] = 0x00;
	sptdwb.sptd.Cdb[2] = (offset_sector >> 24) & 0xff;
	sptdwb.sptd.Cdb[3] = (offset_sector >> 16) & 0xff;
	sptdwb.sptd.Cdb[4] = (offset_sector >> 8) & 0xff;
	sptdwb.sptd.Cdb[5] = offset_sector & 0xff;
	sptdwb.sptd.Cdb[6] = 0x00;
	sptdwb.sptd.Cdb[7] = (read_size >> 8) & 0xff;
	sptdwb.sptd.Cdb[8] = read_size & 0xff;
	sptdwb.sptd.Cdb[9] = 0x00;
	length = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE);
	iRet = DeviceIoControl(Dev,
		IOCTL_SCSI_PASS_THROUGH_DIRECT,
		&sptdwb,
		length,
		&sptdwb,
		length,
		&bytesReturn,
		NULL);
	if (0 == iRet)
	{
		return 0;
	}
	return iRet;
}

//////////////////// Translate string type ///////////////////                // "Managed memory" is cleaned up by a Garbage Collector (GC)
std::string managedStrToNative(System::String^ sysstr) {                      // "Unmanaged memory" is cleaned up by your program or OS.
                                                                              // Defined by Microsoft.
	using System::IntPtr;
	using System::Runtime::InteropServices::Marshal;

	IntPtr ip = Marshal::StringToHGlobalAnsi(sysstr);  // Marshal the managed string to unmanaged memory.
	std::string outString = static_cast<const char*>(ip.ToPointer());   // Get the string back from unmanaged memory
	Marshal::FreeHGlobal(ip);      // Always free the unmanaged string.
	return outString;
}
LPCWSTR stringToLPCWSTR(std::string orig) {
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE); // _TRUNCATE : functions will copy as much of the string as will
	return wcstring;                                                          //             fit, leaving the dest buffer null-terminated, and
}                                                                             //             return sucessfully.

int read_disk_size(HANDLE dsk) {               //Reference: https://blog.csdn.net/u011164819/article/details/52045151
	int iRet;
	ULONG length = 0;
	DWORD bytesReturn;
	SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE sptdwb;
	BYTE read_size_buf[64 * 1024 + 10];
	sptdwb.sptd.Length = sizeof(SCSI_PASS_THROUGH_DIRECT);
	sptdwb.sptd.PathId = 0;
	sptdwb.sptd.TargetId = 1;
	sptdwb.sptd.Lun = 0;
	sptdwb.sptd.CdbLength = 10;
	sptdwb.sptd.DataIn = SCSI_IOCTL_DATA_IN;
	sptdwb.sptd.SenseInfoLength = 24;
	sptdwb.sptd.DataTransferLength = 8;
	sptdwb.sptd.TimeOutValue = 2;
	sptdwb.sptd.DataBuffer = read_size_buf;
	sptdwb.sptd.SenseInfoOffset = offsetof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE, abRequestSense);
	sptdwb.sptd.Cdb[0] = 0x25;                //opcode: Read storage volume
	length = sizeof(SCSI_PASS_THROUGH_DIRECT_WITH_REQSENSE);
	iRet = DeviceIoControl(dsk,
		IOCTL_SCSI_PASS_THROUGH_DIRECT,
		&sptdwb,
		length,
		&sptdwb,
		length,
		&bytesReturn,
		NULL);
	if (0 == iRet)
	{
		return 0;
	}
	int sectors = read_size_buf[0] * (1 << 24) + read_size_buf[1] * (1 << 16) + read_size_buf[2] * (1 << 8) + read_size_buf[3] + 1;
	return sectors;
};

int init_disk(HANDLE dsk) {
	/*
	*  Read the origin volume and check it's MBR and FAT type
	*  Clear the system sector with 1000 sectors from 0, FAT, root
	*/
	BYTE Readbuf[512] = { 0 };
	BYTE ZeroSect[512] = { 0 };
	BYTE gzero[51200] = { 0 };
	BYTE *rdbuf;
	rdbuf = Readbuf;
	int rRET = 0;
	rRET = read_value(dsk, rdbuf, 0, 1);
	for (int i = 0; i < 512; i++) 
	{
		Readbuf[i] = rdbuf[i]; 
	}
	
	//read info
	
	int booter_start = 0;
	int fat_start = 0;
	int cluster_start = 0;
	int fat_size = 0;
	int clr_rsv = 100;
	
	if (Readbuf[0] == 0xEB && Readbuf[13] == 0x00) 
	{
		int clus_size = (int)pow(2,Readbuf[109]);
		fat_start = (Readbuf[83] << 24) + (Readbuf[82] << 16) + (Readbuf[81] << 8) + Readbuf[80];        //little-endian 
		fat_size = (Readbuf[87] << 24) + (Readbuf[86] << 16) + (Readbuf[85] << 8) + Readbuf[84];
		cluster_start = (Readbuf[91] << 24) + (Readbuf[90] << 16) + (Readbuf[89] << 8) + Readbuf[88];
	}//exFAT WITHOUT MBR
	else if (Readbuf[0] == 0xEB && Readbuf[13] != 0x00) {
		fat_start = (Readbuf[15] << 8) + (Readbuf[14]);
		fat_size = (Readbuf[39] << 24) + (Readbuf[38] << 16) + (Readbuf[37] << 8) + Readbuf[36];
		cluster_start = fat_start + fat_size*2 ;
	}//FAT32 WITHOUT MBR
	else 
	{
		booter_start = 2048;
		int startsec = (Readbuf[457] << 24) + (Readbuf[456] << 16) + (Readbuf[455] << 8) + Readbuf[454];
		rRET = read_value(dsk, rdbuf, startsec, 1);

		for (int i = 0; i < 512; i++)
		{ 
			Readbuf[i] = rdbuf[i]; 
		}
		if (Readbuf[0] == 0xEB && Readbuf[13] == 0x00) 
		{
			fat_start = (Readbuf[83] << 24) + (Readbuf[82] << 16) + (Readbuf[81] << 8) + Readbuf[80];
			fat_start += booter_start;
			fat_size = (Readbuf[87] << 24) + (Readbuf[86] << 16) + (Readbuf[85] << 8) + Readbuf[84];
			fat_size += booter_start;
			cluster_start = (Readbuf[91] << 24) + (Readbuf[90] << 16) + (Readbuf[89] << 8) + Readbuf[88];
			cluster_start += booter_start;
		}//exFAT WITH MBR
		else if (Readbuf[0] == 0xEB && Readbuf[13] != 0x00)
		{
			fat_start = (Readbuf[15] << 8) + (Readbuf[14]);
			fat_start += booter_start;
			fat_size = (Readbuf[39] << 24) + (Readbuf[38] << 16) + (Readbuf[37] << 8) + Readbuf[36];
			fat_size += booter_start;
			cluster_start = fat_start + fat_size * 2;
			cluster_start += booter_start;
		}//FAT32 WITH MBR
	}//with MBR
	
	for (int i = 0; i < 20; i++) 
	{ 
		rRET = write_value(dsk, gzero, i * 100, 100); 
	}
	for (int i = 0; i < 20; i++)
	{ 
		rRET = write_value(dsk, gzero, 2048 +i * 100, 100);
	}

	int fat_clr_cnt = fat_size / 100 + 100;
	for (int i = 0; i < fat_clr_cnt*2; i++)
	{ 
		rRET = write_value(dsk, gzero, fat_start + i * 100 - clr_rsv, 100); 
	}
	for (int i = 0; i < 10; i++)
	{ 
		rRET = write_value(dsk, gzero, cluster_start + i * 100 - clr_rsv, 100);
	}
	return 0;
}

DWORD get_volume_id() {
	SYSTEMTIME s;
	DWORD d;
	WORD lo, hi, tmp;
	GetLocalTime(&s);
	lo = s.wDay + (s.wMonth << 8);
	tmp = (s.wMilliseconds / 10) + (s.wSecond << 8);
	lo += tmp;
	hi = s.wMinute + (s.wHour << 8);
	hi += s.wYear;
	d = lo + (hi << 16);
	return d;
}

DWORD GetFATSizeSectors(DWORD DskSize, DWORD ReservedSecCnt, DWORD SecPerClus, DWORD NumFATs, DWORD BytesPerSect) {
	ULONGLONG Numerator, Denominator;
	ULONGLONG FatElementSize = 4;
	ULONGLONG ReservedClusCnt = 2;
	ULONGLONG FatSz;

	Numerator = DskSize - ReservedSecCnt + ReservedClusCnt * SecPerClus;
	Denominator = SecPerClus * BytesPerSect / FatElementSize + NumFATs;
	FatSz = Numerator / Denominator + 1;
	return (DWORD)FatSz;
}

BYTE* FAT32_boot_sector_setting(HANDLE dsk , int cluster_size, int reserved_offset) {
	BYTE b_sec[512] = { 0 };
	FAT32_BOOTSECTOR* FAT32BootSec = (FAT32_BOOTSECTOR*)VirtualAlloc(NULL, 512, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	
	/*
	* VirtualAlloc(
		LPVOID lpAddress,
		SIZE_T dwSize,
		DWORD  flAllocationType,
		DWORD  flProtect
	);	
	* "Memory allocated by this function is automatically initialized to zero."
	* lpAddress: The starting address of the region to allocate. If this parameter is NULL, the system determines where to allocate the region.
	* dwsize   : The size of the region, in bytes.
	* flAllocationType: The type of memory allocation.
	* flProtect: The memory protection for the region of pages to be allocated.
	*/
															
	FAT32BootSec->sJmpBoot[0] = 0xEB;
	FAT32BootSec->sJmpBoot[1] = 0x58;
	FAT32BootSec->sJmpBoot[2] = 0x90;
	memcpy(FAT32BootSec->sOEMName, "MSDOS5.0", sizeof(FAT32_BOOTSECTOR::sOEMName));
	FAT32BootSec->wBytsPerSec[0] = 0x00;
	FAT32BootSec->wBytsPerSec[1] = 0x02;
	int clus_size = cluster_size * 2 ;             // cluster_size KB / 512 B = cluster_size * 2
	FAT32BootSec->bSecPerClus = (BYTE)clus_size;
	FAT32BootSec->wRsvdSecCnt = (WORD)(reserved_offset);     // Origin: (WORD)(reserved_offset + 1024) ;
	fat32_fatoffset = reserved_offset + MBR_start_sector;    // Origin: reserved_offset + 1024 + MBR_start_sector;
	FAT32BootSec->bNumFATs = 0x02;
	FAT32BootSec->wRootEntCnt[0] = 0;
	FAT32BootSec->wRootEntCnt[1] = 0;
	FAT32BootSec->wTotSec16[0] = 0;
	FAT32BootSec->wTotSec16[1] = 0;
	FAT32BootSec->bMedia = 0xF8;
	FAT32BootSec->wFATSz16 = 0;
	FAT32BootSec->wSecPerTrk[0] = 0x3F;
	FAT32BootSec->wSecPerTrk[1] = 0x00;
	FAT32BootSec->wNumHeads[0] = 0xFF;
	FAT32BootSec->wNumHeads[1] = 0x00;
	FAT32BootSec->dHiddSec = (DWORD)MBR_start_sector;
	int tol_sec = read_disk_size(dsk);
	FAT32BootSec->dTotSec32 = (DWORD)(tol_sec - MBR_start_sector);
	FatSize = GetFATSizeSectors((DWORD)(tol_sec - MBR_start_sector), (WORD)reserved_offset, (BYTE)clus_size, 2, 512);
	FAT32BootSec->dFATSz32 = FatSize;
	FAT32BootSec->wExtFlags = 0;
	FAT32BootSec->wFSVer = 0;
	FAT32BootSec->dRootClus = (DWORD)2;
	FAT32BootSec->wFSInfo = (WORD)1;
	FAT32BootSec->wBkBootSec = (WORD)6;
	FAT32BootSec->bDrvNum = 0x80;
	FAT32BootSec->Reserved1 = 0;
	FAT32BootSec->bBootSig = 0x29;
	DWORD vol_id = get_volume_id();
	FAT32BootSec->dBS_VolID = vol_id;
	memcpy(FAT32BootSec->sVolLab, "NO NAME    ", 11);
	memcpy(FAT32BootSec->sBS_FilSysType, "FAT32   ", 8);
	((BYTE*)FAT32BootSec)[510] = 0x55;
	((BYTE*)FAT32BootSec)[511] = 0xaa;
	for (int i = 0; i < 512; i++)
	{
		b_sec[i] = ((BYTE*)FAT32BootSec)[i];
	}
	return b_sec;
}

BYTE* FAT32_fsinfo_sector_setting(HANDLE dsk, int usable_size, int free_count) 
{
	BYTE fs_sec[512] = { 0 };
	FAT32_FSINFO* FAT32fsinfoSec = (FAT32_FSINFO*)VirtualAlloc(NULL, 512, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	FAT32fsinfoSec->dLeadSig = 0x41615252;
	FAT32fsinfoSec->dStrucSig = 0x61417272;
	FAT32fsinfoSec->dFree_Count = 10;
	FAT32fsinfoSec->dNxt_Free = 3;
	FAT32fsinfoSec->dTrailSig = 0xaa550000;
	for (int i = 0; i < 512; i++)
	{
		fs_sec[i] = ((BYTE*)FAT32fsinfoSec)[i];
	}
	return fs_sec; 
}

BYTE* exFAT_boot_sector_setting(HANDLE dsk, int cluster_size,int reserved_offset,int MBR) {
	int tol_sec = read_disk_size(dsk);
	int usbsize = 0;
	
	//check disk size
	int clus_size = cluster_size * 2;
	int sector_offset = reserved_offset * 2;
	int cluster_offset = reserved_offset / cluster_size;

	//exfat_fatlen tot
	exfat_fatoffset = 2048 + reserved_offset;
	exfat_fatlen = GetFATSizeSectors((DWORD)(tol_sec), (WORD)reserved_offset, (BYTE)clus_size, 1, 512);
	exfat_clusterheapoffset = exfat_fatoffset + exfat_fatlen +512;
	exfat_clustercount = (DWORD)(int((tol_sec - exfat_clusterheapoffset- MBR_start_sector) / clus_size));
	bitallcsize = int((exfat_clustercount / (512 * 8 * clus_size)) + 1);
	upcasesize = int((12 / clus_size) + 1);
	exfat_rootdict = bitallcsize + upcasesize + 2;      //Unit: cluster
	exfat_upcasedict = exfat_rootdict - upcasesize;     //Unit: cluster
	//

	int Vlength = tol_sec;
	BYTE b_sec[512] = { 0 };
	exFAT_BOOTSECTOR* exFATBootSec = (exFAT_BOOTSECTOR*)VirtualAlloc(NULL, 512, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	exFATBootSec->sJmpBoot[0] = 0xEB;
	exFATBootSec->sJmpBoot[1] = 0x76;
	exFATBootSec->sJmpBoot[2] = 0x90;
	memcpy(exFATBootSec->FileSystemName, "EXFAT   ", sizeof(exFAT_BOOTSECTOR::FileSystemName));
	exFATBootSec->PartitionOffset = (LONGLONG)MBR_start_sector;
	exFATBootSec->VolumeLength = (LONGLONG)(Vlength - MBR_start_sector);
	exFATBootSec->dFatOffset = (DWORD)exfat_fatoffset;
	exFATBootSec->dFatLength = (DWORD)exfat_fatlen;
	exFATBootSec->dClusterHeapOffset = exfat_clusterheapoffset;
	exFATBootSec->dClusterCount = (DWORD)exfat_clustercount;
	exFATBootSec->dFirstClusterOfRootDirectory= (DWORD)exfat_rootdict;
	DWORD vol_id = get_volume_id();
	exFATBootSec->dVolumeSerialNumberl = vol_id;
	exFATBootSec->wFileSystemRevision = 0x0100;
	exFATBootSec->wVolumeFlags = 0;
	exFATBootSec->BytesPerSectorShift = 0x09;     // Sector size in unit of byte expressed in log2.
	//get log2
	int clus_size_log = 0;
	int clus_size_raw = cluster_size * 2;
	while (clus_size_raw != 1)
	{
		clus_size_raw = clus_size_raw / 2;
		clus_size_log++;
	}
	exFATBootSec->SectorsPerClusterShift = (BYTE)clus_size_log;   // Cluster size in unit of sector expressed in log2.
	exFATBootSec->NumberOfFats = 0x01;
	exFATBootSec->DriveSelect = 0x80;
	exFATBootSec->PercentInUse = 0;
	BYTE booter[390] = {0};
	memcpy(exFATBootSec->BootCode, booter, sizeof(exFAT_BOOTSECTOR::BootCode));
	((BYTE*)exFATBootSec)[510] = 0x55;
	((BYTE*)exFATBootSec)[511] = 0xaa;
	for (int i = 0; i < 512; i++)
	{
		b_sec[i] = ((BYTE*)exFATBootSec)[i];
	}
	return b_sec;
}

BYTE* exFAT_reserved_sector_setting() {
	BYTE r_sec[5120];
	exFAT_RESERVED* exFATBootRes = (exFAT_RESERVED*)VirtualAlloc(NULL, 5120, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	exFATBootRes->Reservedsec1[510] = 0x55;
	exFATBootRes->Reservedsec1[511] = 0xaa;
	exFATBootRes->Reservedsec2[510] = 0x55;
	exFATBootRes->Reservedsec2[511] = 0xaa;
	exFATBootRes->Reservedsec3[510] = 0x55;
	exFATBootRes->Reservedsec3[511] = 0xaa;
	exFATBootRes->Reservedsec4[510] = 0x55;
	exFATBootRes->Reservedsec4[511] = 0xaa;
	exFATBootRes->Reservedsec5[510] = 0x55;
	exFATBootRes->Reservedsec5[511] = 0xaa;
	exFATBootRes->Reservedsec6[510] = 0x55;
	exFATBootRes->Reservedsec6[511] = 0xaa;
	exFATBootRes->Reservedsec7[510] = 0x55;
	exFATBootRes->Reservedsec7[511] = 0xaa;
	exFATBootRes->Reservedsec8[510] = 0x55;
	exFATBootRes->Reservedsec8[511] = 0xaa;
	for (int i = 0; i < 5120; i++)
	{
		r_sec[i] = ((BYTE*)exFATBootRes)[i];
	}
	return r_sec;
}

BYTE* MBR_sector_setting(HANDLE dsk , int type) {
	int tol_sec = read_disk_size(dsk);
	BYTE b_sec[512] = { 0 };
	MBR_SECTOR* MBRSec = (MBR_SECTOR*)VirtualAlloc(NULL, 512, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	MBRSec->Boot_Indicator1 = 0x00;     // non-boot
	MBRSec->startCHS1[0] = 0x20;
	MBRSec->startCHS1[1] = 0x21;
	MBRSec->startCHS1[2] = 0x00;
	if (type == 0) 
	{
		MBRSec->type1 = 0x0C;          // FAT32 LBA
	}
	else if (type == 1) 
	{
		MBRSec->type1 = 0x07;          // NTFS, HPFS, exFAT
	}
	MBRSec->endCHS1[0] = 0xFE;
	MBRSec->endCHS1[1] = 0xFF;
	MBRSec->endCHS1[2] = 0xFF;
	MBRSec->StartSector1 = (DWORD)MBR_start_sector;
	MBRSec->PartitionSize1 = (DWORD)(tol_sec - MBR_start_sector);
	((BYTE*)MBRSec)[510] = 0x55;
	((BYTE*)MBRSec)[511] = 0xaa;
	for (int i = 0; i < 512; i++)
	{
		b_sec[i] = ((BYTE*)MBRSec)[i];
	}
	return b_sec;
}

BYTE* exFAT_checksum(BYTE *boot_sec , BYTE *res_send ,BYTE *res_zero) { //repeating 32-bit checksum of the previous 11 sectors.
	BYTE checksum_sec[512] = { 0 };
	UINT32 checksum = 0;
	for (int i = 0; i < 512; i++)
	{
		if (i == 106 || i == 107 || i == 112)  //CHECK SUM pass 106 107 112 B
			continue;
		checksum = (checksum << 31) | (checksum >> 1) + boot_sec[i];
	}
	for (int i = 0; i < 8; i++) 
	{
		for (int j = 0; j < 512; j++)
			checksum = (checksum << 31) | (checksum >> 1) + res_send[j];
	}
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 512; j++)
			checksum = (checksum << 31) | (checksum >> 1) + res_zero[j];
	}
	DWORD ccsum = (DWORD)checksum;
	for (int i = 0; i < 128; i++)
	{
		checksum_sec[4 * i + 3] = (ccsum >> 24) & 0xff;
		checksum_sec[4 * i + 2] = (ccsum >> 16) & 0xff;
		checksum_sec[4 * i + 1] = (ccsum >> 8) & 0xff;
		checksum_sec[4 * i] = (ccsum) & 0xff;
	}
	return checksum_sec;
}

BYTE* initFile(int cluster_size) {           //Set File Directory Entry. According to MS exfat showed on WinHex.
	BYTE initfile[512] = { 0 };
	//file size : byte 
	int bitalllen = exfat_clustercount / 8 + 1;    //The sizeof allocation bitmap
	exfatFile* exFATfilesec = (exfatFile*)VirtualAlloc(NULL, 512, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	exFATfilesec->file1_1 = 0x550483;
	exFATfilesec->file1_2 = 0x420053;
	exFATfilesec->file1_3 = 0x20;
	//UPCASE
	exFATfilesec->file2_1 = 0x82;                     //EntryType of Up-case table
	exFATfilesec->file2_2 = 0xE619D30D;
	exFATfilesec->file2_3 = 0x00;
	exFATfilesec->file2_4 = 0x00;
	exFATfilesec->file2_5 = 0x00;
	exFATfilesec->file2_6 = exfat_upcasedict;
	exFATfilesec->file2_7 = 0x16CC;
	exFATfilesec->file2_8 = 0x00;
	//bitall
	exFATfilesec->file3_1 = 0x81;
	exFATfilesec->file3_2 = 0x00;
	exFATfilesec->file3_3 = 0x00;
	exFATfilesec->file3_4 = 0x00;
	exFATfilesec->file3_5 = 0x00;
	exFATfilesec->file3_6 = 0x02;                //First cluster number of this allocation bitmap.
	exFATfilesec->file3_7 = (DWORD)bitalllen;
	exFATfilesec->file3_8 = 0x00;
	for (int i = 0; i < 512; i++)
	{
		initfile[i] = ((BYTE*)exFATfilesec)[i];
	}
		
	return initfile;
}

BYTE* initFATfile(int bitallc_size , int upcase_size) {
	BYTE initFAT[2048] = { 0 };
	for (int i = 0; i < 8; i++)
	{ 
		initFAT[i] = 0xFF;
	}
	initFAT[0] = 0xF8;
	for (int i = 0; i < bitallc_size; i++) 
	{
		initFAT[8+4*i] = (BYTE)(i+3);
	}
	initFAT[8 + 4 * (bitallc_size - 1)] = 0xFF;
	initFAT[8 + 4 * (bitallc_size - 1) + 1] = 0xFF;
	initFAT[8 + 4 * (bitallc_size - 1) + 2] = 0xFF;
	initFAT[8 + 4 * (bitallc_size - 1) + 3] = 0xFF;
	if (upcase_size > 1)
	{
		initFAT[8 + 4 * bitallc_size] = bitallc_size+3;
		initFAT[8 + 4 * (bitallc_size + 1)] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 1) + 1] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 1) + 2] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 1) + 3] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 2)] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 2) + 1] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 2) + 2] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 2) + 3] = 0xFF;
	}
	else 
	{
		initFAT[8 + 4 * (bitallc_size)] = 0xFF;
		initFAT[8 + 4 * (bitallc_size) + 1] = 0xFF;
		initFAT[8 + 4 * (bitallc_size) + 2] = 0xFF;
		initFAT[8 + 4 * (bitallc_size) + 3] = 0xFF;
		//initFAT[8 + 4 * (bitallc_size) + 4] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 1)] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 1) + 1] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 1) + 2] = 0xFF;
		initFAT[8 + 4 * (bitallc_size + 1) + 3] = 0xFF;
	}
	return initFAT;
}

BYTE* initBitfile(int bitallc_size, int upcase_size) {
	BYTE initBit[512] = {0};
	int allnum = bitallc_size + upcase_size + 1;
	int quotient = allnum / 8;
	int remainder = allnum % 8;
	for (int i = 0; i < quotient; i++)
		initBit[i] = 0xFF;

	switch (remainder)
	{
	case 1:
		initBit[quotient] = 0x01;   //0000 0001
		break;
	case 2:
		initBit[quotient] = 0x03;   //0000 0011
		break;
	case 3:
		initBit[quotient] = 0x07;   //0000 0111
		break;
	case 4:
		initBit[quotient] = 0x0F;   //0000 1111
		break;
	case 5:
		initBit[quotient] = 0x1F;   //0001 1111
		break;
	case 6:
		initBit[quotient] = 0x3F;   //0011 1111
		break;
	case 7:
		initBit[quotient] = 0x7F;   //0111 1111
		break;
	}
	return initBit;
}

// Sector size is set as 512 Byte
bool format_main(System::String^ disk, int MBR, int FAT_type, int cluster_size, int reserved_offset) {
	//FAT type FAT32:0 , exFAT:1 ; MBR type with MBR:1 , without:0 ; 
	//cluster size = int KB ; offset int sectors
	//init
	MBR_start_sector = 0;
	//dev name
	std::string dev = "\\\\.\\";
	dev = dev + managedStrToNative(disk);
	dev.erase(6, 1);
	LPCWSTR handle_dev = stringToLPCWSTR(dev);
	DWORD cbRet;
	int rRET = 0;
	HANDLE hardDev = CreateFile(                         //与U盘通信一般先获取U盘设备句柄。该函数返回U盘的句柄。注意盘符名字为“\\.\X：”的形式。
		handle_dev,
		(GENERIC_READ | GENERIC_WRITE),
		(FILE_SHARE_READ | FILE_SHARE_WRITE),
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL
	);
	                              //Signals the file system driver not to perform any I/O boundary checks on partition read or write calls.
	rRET = DeviceIoControl(hardDev, FSCTL_ALLOW_EXTENDED_DASD_IO, NULL, 0, NULL, 0, &cbRet, NULL); //获得U盘的句柄之后即可用函数
	//rRET = DeviceIoControl(hardDev, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, &cbRet, NULL); 
	                              //Locks a volume if it is not in use.	  
	DeviceLock(hardDev);

	int write_start_sector = 0;
	BYTE MBRbs[512] = { 0 };
	
	//Init
	if (MBR == 1) 
	{
		MBR_start_sector = 2048;
		write_start_sector = MBR_start_sector;
	}
	
	init_disk(hardDev);
	
	progress = 30;
	
	//Formating FAT32 or EXFAT
	if (FAT_type == 0) 
	{
		BYTE b_sec[512] = { 0 };
		ZERO_SECTOR* FirstSectOfFat = (ZERO_SECTOR*)VirtualAlloc(NULL, 512, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		BYTE BS[512] = { 0 };
		BYTE FSINFOS[512] = { 0 };
		BYTE Reserved[512] = { 0 };
		BYTE Undefined[512] = { 0 };
		BYTE ZEROSEC[512] = { 0 };
		FirstSectOfFat->RES1 = 0x0FFFFFF8;  // Reserved cluster 1 media id in low byte
		FirstSectOfFat->RES2 = 0xFFFFFFFF;  // Reserved cluster 2 EOC
		FirstSectOfFat->RES3 = 0x0FFFFFFF;

		for (int i = 0; i < 512; i++)
		{
			ZEROSEC[i] = ((BYTE*)FirstSectOfFat)[i];
		}

		BYTE* boot_sec = FAT32_boot_sector_setting(hardDev, cluster_size, reserved_offset);   //Return a pointer of a sector (512 Byte).
		for (int i = 0; i < 512; i++)
		{
			BS[i] = boot_sec[i];
		}
		int Numsect = read_disk_size(hardDev);
		int usable_size = Numsect - reserved_offset - FatSize * 2;          //"FatSize" in secter
		int ClusterCount = usable_size / (cluster_size * 2);
		int free_count = (usable_size / ClusterCount) - 1;
		BYTE* fs_sec = FAT32_fsinfo_sector_setting(hardDev, usable_size, free_count);
		for (int i = 0; i < 512; i++)
		{
			FSINFOS[i] = fs_sec[i];
		}
		int fatstart = fat32_fatoffset;
		int fattwice = fat32_fatoffset + FatSize;
		int datastart = fat32_fatoffset + 2 * FatSize;
		int fat_clr_time = (FatSize * 2 + 100) / 100 + 2;
		//write sector here
		
		//clear before write bs/fat/file
		rRET = write_value(hardDev, clearbuff, write_start_sector, 100);  //write 100 sectors
		for (int i = 0; i < fat_clr_time; i++) {
			rRET = write_value(hardDev, clearbuff, fatstart+100*i, 1);
		}
		for (int i = 0; i < fat_clr_time; i++) {
			rRET = write_value(hardDev, clearbuff, datastart+100*i, 1);
		}
			
		rRET = write_value(hardDev, BS, write_start_sector, 1);
		write_start_sector++;
		rRET = write_value(hardDev, FSINFOS, write_start_sector, 1);
		write_start_sector = MBR_start_sector + 6;
		rRET = write_value(hardDev, BS, write_start_sector, 1);
		write_start_sector++;
		rRET = write_value(hardDev, FSINFOS, write_start_sector, 1);
		write_start_sector++;
		rRET = write_value(hardDev, ZEROSEC, fatstart, 1);
		rRET = write_value(hardDev, ZEROSEC, fattwice, 1);
		//write the first FAT reserved
	}//FAT32
	else if (FAT_type == 1) 
	{
		BYTE BS[512] = { 0 };
		//BYTE ResSec[5120] = { 0 };
		BYTE CheckSec[512] = { 0 };
		BYTE Undefined[512] = { 0 };
		BYTE ZEROSEC[512] = { 0 };
		BYTE FAT_SEC[512] = { 0 };
		BYTE FAT_SEC2[512] = { 0 };
		BYTE res_send[512] = { 0 };
		BYTE BitAll[512] = { 0 };
		BYTE Upcase[512] = { 0 };
		BYTE initfilesec[512] = { 0 };
		res_send[510] = 0x55;
		res_send[511] = 0xAA;
		BYTE reszero[512] = { 0 };
		ZERO_SECTOR* FirstSectOfFat = (ZERO_SECTOR*)VirtualAlloc(NULL, 512, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		BYTE* boot_sec = exFAT_boot_sector_setting(hardDev, cluster_size, reserved_offset, MBR);
		for (int i = 0; i < 512; i++)
		{
			BS[i] = boot_sec[i];
		}
		
		BYTE* initfatptr = initFATfile(bitallcsize, upcasesize);
		for (int i = 0; i < 512; i++)
		{
			FAT_SEC[i] = initfatptr[i];
		}
		for (int i = 0; i < 512; i++)
		{
			FAT_SEC2[i] = initfatptr[i + 512];
		}

		BYTE* initbitptr = initBitfile(bitallcsize, upcasesize);
		for (int i = 0; i < 512; i++) 
		{ 
			BitAll[i] = initbitptr[i]; 
		}

		BYTE* initfileptr = initFile(cluster_size);
		for (int i = 0; i < 512; i++)
		{
			initfilesec[i] = initfileptr[i];
		}

		BYTE* check_sec = exFAT_checksum(BS, res_send, reszero);
		for (int i = 0; i < 512; i++)
		{
			CheckSec[i] = check_sec[i];
		}
		int fatstart = exfat_fatoffset + MBR_start_sector ;
		int clr_fat_num = exfat_fatlen / 100 + 2;
		int file_start = exfat_clusterheapoffset + MBR_start_sector;
		//WRITE		
		//clear before write clr bs/fat/file
		rRET = write_value(hardDev, clearbuff, write_start_sector, 100);
		for (int i = 0; i < clr_fat_num; i++) 
		{
			rRET = write_value(hardDev, clearbuff, fatstart+100*i, 100);
		}
		for (int i = 0; i < 50; i++) 
		{
			rRET = write_value(hardDev, clearbuff, file_start+100*i, 100);
		}
		//write 10 reserved section (Boot Sector(1), Extended Boot Sector(8), OEM Parameters(1), Resserved(1), Boot Checksum(1))
		for (int k = 0; k < 2; k++)	
		{
			rRET = write_value(hardDev, BS, write_start_sector, 1);
			write_start_sector++;
			for (int j = 0; j < 8; j++) 
			{
				rRET = write_value(hardDev, res_send, write_start_sector, 1);
				write_start_sector++;
			}
			rRET = write_value(hardDev, reszero, write_start_sector, 1);
			write_start_sector++;
			rRET = write_value(hardDev, reszero, write_start_sector, 1);
			write_start_sector++;
			rRET = write_value(hardDev, CheckSec, write_start_sector, 1);
			write_start_sector++;
		}
		//write FAT
		rRET = write_value(hardDev, FAT_SEC, fatstart, 1);
		rRET = write_value(hardDev, FAT_SEC2, fatstart+1, 1);
	
		//write bit allocation
		int bit_start = exfat_clusterheapoffset + MBR_start_sector ;
		rRET = write_value(hardDev, BitAll, bit_start, 1);
		//write up case
		int upcase_start = bit_start + (exfat_upcasedict-2) * (cluster_size*2);
		rRET = write_value(hardDev, upcaseSec1, upcase_start, 1);
		rRET = write_value(hardDev, upcaseSec2, upcase_start+1, 1);
		rRET = write_value(hardDev, upcaseSec3, upcase_start+2, 1);
		rRET = write_value(hardDev, upcaseSec4, upcase_start+3, 1);
		rRET = write_value(hardDev, upcaseSec5, upcase_start+4, 1);
		rRET = write_value(hardDev, upcaseSec6, upcase_start+5, 1);
		rRET = write_value(hardDev, upcaseSec7, upcase_start+6, 1);
		rRET = write_value(hardDev, upcaseSec8, upcase_start+7, 1);
		rRET = write_value(hardDev, upcaseSec9, upcase_start+8, 1);
		rRET = write_value(hardDev, upcaseSec10, upcase_start+9, 1);
		rRET = write_value(hardDev, upcaseSec11, upcase_start+10, 1);
		rRET = write_value(hardDev, upcaseSec12, upcase_start+11, 1);
		//write File Entry
		int initfile = bit_start + (exfat_rootdict - 2) * (cluster_size * 2);
		rRET = write_value(hardDev, initfilesec, initfile, 1);
		//debug mode
	}//EXFAT
	
	if (MBR == 1)
	{
		BYTE* mbr_sec = MBR_sector_setting(hardDev, FAT_type);
		for (int i = 0; i < 512; i++)
		{ 
			MBRbs[i] = mbr_sec[i]; 
		}
		//clear and write MBR
		 
		BYTE zerosect[512] = { 0 };
		rRET = write_value(hardDev, zerosect, 0, 1);
		rRET = write_value(hardDev, MBRbs, 0, 1);	
	}
	
	rRET = DeviceIoControl(hardDev, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, &cbRet, NULL);
	DeviceUnLock(hardDev);
	CloseHandle(hardDev);
	
	return 0;
}
