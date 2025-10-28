/* Automation Studio generated header file */
/* Do not edit ! */
/* FileUser 0.00.0 */

#ifndef _FILEUSER_
#define _FILEUSER_
#ifdef __cplusplus
extern "C" 
{
#endif
#ifndef _FileUser_VERSION
#define _FileUser_VERSION 0.00.0
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
#ifdef _SG4
		#include "sys_lib.h"
		#include "FileIO.h"
#endif


/* Constants */
#ifdef _REPLACE_CONST
 #define fuDRIVE_FTP 1U
 #define fuDRIVE_FILEIO 0U
 #define fuFILE_LOAD_OK 4
#else
 _GLOBAL_CONST unsigned char fuDRIVE_FTP;
 _GLOBAL_CONST unsigned char fuDRIVE_FILEIO;
 _GLOBAL_CONST signed char fuFILE_LOAD_OK;
#endif




/* Datatypes and datatypes of function blocks */
typedef struct FILE_EXT
{	plcstring Name[5];
} FILE_EXT;

typedef struct FILEOUT_IF
{	plcbit OpenDirectory;
	unsigned short OpenFile;
	plcbit CopyFile;
	plcbit DeleteFile;
	plcbit PageUp;
	plcbit SaveAs;
	plcbit Save;
	plcbit PageDown;
	plcbit Refresh;
	plcbit ClearStatus;
	unsigned short SelectedFileNum;
	plcstring SelectedFile[35];
	plcstring SelectedDirectory[35];
	plcstring NewFileNameBase[31];
	plcstring NewFileName[35];
	unsigned long FileSize;
	unsigned char ExtensionIndex;
	struct FILE_EXT Extension[5];
	unsigned char StreamEnable;
	unsigned char StreamGo;
	unsigned char StreamClose;
	unsigned char StreamOffset;
	unsigned char StreamLength;
	unsigned char Stream_adr;
	plcstring pDestDev[21];
} FILEOUT_IF;

typedef struct FILEMGR_obj
{	plcstring Message[51];
	unsigned char Error;
	plcstring Device[21];
	plcstring Path[81];
	signed char Status;
	unsigned long FileLength;
	plcstring OpenFileName[36];
	unsigned char OpenFileNum;
	plcdt OpenFileDate;
	unsigned short Type;
	unsigned long BufferSize;
	unsigned long Data_adr;
	unsigned short MaxDirectoryFiles;
	unsigned long _if_in_adr;
	unsigned long _if_out_adr;
	unsigned long _fname_adr;
	unsigned long _ftp_adr;
	unsigned short _state;
	unsigned short _error;
	unsigned long _fileptr;
	unsigned long _numfiles;
	unsigned short _infostatus;
	unsigned short _readstatus;
	unsigned short _fileopenstatus;
	unsigned short _filereadstatus;
	unsigned short _filecreatestatus;
	unsigned short _filewritestatus;
	unsigned short _userfileptr;
	struct DirInfo _dirinfo;
	struct DirRead _dirread;
	struct FileOpen _fileopen;
	struct FileRead _fileread;
	struct FileDelete _filedelete;
	struct FileCopy _filecopy;
	struct FileCreate _filecreate;
	struct FileWrite _filewrite;
	struct FileClose _fileclose;
} FILEMGR_obj;

typedef struct FILEIN_IF
{	unsigned short TotalPageFileNum;
	unsigned short TotalDirectoryFileNum;
	plcstring FileNames[20][35];
	plcdt FileDates[20];
	unsigned long FileSizes[20];
	plcstring OpenFileName[35];
	unsigned char PageNum;
	unsigned char ExtensionIndex;
	signed char Status;
	unsigned long FileLength;
	unsigned long BufferSize;
	plcbit StreamReady;
	plcbit StreamError;
	plcstring Message[50];
} FILEIN_IF;



/* Prototyping of functions and function blocks */
_BUR_PUBLIC signed long FU_init(unsigned long fm_adr, plcstring* sDevice, plcstring* Path, unsigned long data_adr, unsigned long size, unsigned char Type, unsigned long _if_in_adr, unsigned long _if_out_adr, plcstring* sDevice2, unsigned long _fname_adr, unsigned long _ftp_adr, unsigned short MaxFiles);
_BUR_PUBLIC signed long FU_serv(unsigned long fm_adr);
_BUR_PUBLIC plcbit FU_setBufPointer(unsigned long fm_adr, unsigned long pMem, unsigned long size);


#ifdef __cplusplus
};
#endif
#endif /* _FILEUSER_ */

