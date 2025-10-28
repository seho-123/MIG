/********************************************************************
 * COPYRIGHT -- Lenovo
 ********************************************************************
 * Library: FileUser
 * File: FileUserSource.c
 * Author: Frys Electronic
 * Created: September 02, 2009
 * Mofified: 21 Oct 09 - by KK
 *******************************************************************/

#include <bur/plctypes.h>
#include "FileUser.h"
#include <string.h>

/*User Private Library Error Numbers (value assigned as suggested in the B&R help file)*/
#define USER_ERROR_MIN 0xC350
#define USER_ERROR_MAX 0xEA5F
#define USER_ERROR_COB	USER_ERROR_MIN
#define FU_MAX_PAGE_FILES	10
#define FU_MAX_EXT			5
#define FU_DRIVE_FILEIO		0
#define FU_DRIVE_FTP		1 
#define FU_DRIVE_STREAM	2

/* Prototypes */
DINT GetExtType( FILEIN_IF *in, FILEOUT_IF *out ); 
void ConvertToUpper( STRING s[35] );

unsigned short SortDirectory(unsigned long fm_adr);
signed long FU_AddExt(struct FILEOUT_IF* out, struct FILEIN_IF* in);
signed long CreateNewFileName(struct FILEOUT_IF* out);
signed long FindString(plcstring* s, plcstring* ext);


/* LOCAL */
DINT FU_init( 	UDINT fm_adr,  plcstring* sDevice, plcstring* Path, UDINT data_adr, UDINT size, USINT Type, UDINT _if_in_adr, UDINT _if_out_adr, 
				plcstring* sDevice2, UDINT _fname_adr, UDINT _ftp_adr, UINT MaxFiles )
{
FILEMGR_obj *fm = ( FILEMGR_obj *) fm_adr;
FILEIN_IF *in;

	/* Default: */
	fm->_state 			= 0;
	
	fm->_if_in_adr		= _if_in_adr;
	fm->_if_out_adr		= _if_out_adr;
	fm->_fname_adr		= _fname_adr;
	fm->_ftp_adr			= _ftp_adr;
	fm->Type			= Type;
	fm->BufferSize		= size;
	fm->MaxDirectoryFiles	= MaxFiles;
	fm->Data_adr		= data_adr;
	
	/* */
	strcpy( fm->Device, sDevice );

	/* New Stuff */
	in 					= (FILEIN_IF   *) fm->_if_in_adr;
	in->BufferSize		= fm->BufferSize;
	
	/* */
	strcpy( fm->Device, sDevice );

	/* Init: DirInfo */
	fm->_dirinfo.enable 	= 1;
	fm->_dirinfo.pDevice	= (UDINT) fm->Device;
	fm->_dirinfo.pPath 		= (UDINT)Path;
	
	/* Init: DirRead */
	fm->_dirread.enable 	= 1;
	fm->_dirread.pDevice	= (UDINT) fm->Device;
	fm->_dirread.pPath 		= (UDINT)Path;
	fm->_dirread.option 	= FILE_FILE;
	fm->_dirread.data_len 	= sizeof(fiDIR_READ_DATA);
	
	/* Init: FileOpen */
	fm->_fileopen.enable 	= 1;
	fm->_fileopen.pDevice 	= (UDINT) fm->Device;
	fm->_fileopen.mode 		= FILE_RW; 
		
	/* Init: FileRead */
	fm->_fileread.enable	= 1;
	fm->_fileread.offset 	= 0;
	fm->_fileread.pDest 	= data_adr;
	if ( fm->Type != FU_DRIVE_STREAM )
		fm->_fileread.len 	= fm->BufferSize;
	
	/* Init: FileDelete */
	fm->_filedelete.enable 	= 1;
	fm->_filedelete.pDevice	= (UDINT) fm->Device;
	fm->_filedelete.pName	= 0;
	
	/* Init: FileCopy */
	fm->_filecopy.enable	= 1;
	fm->_filecopy.pSrcDev	= (UDINT) fm->Device;
	fm->_filecopy.option	= FILE_OW_DETMEM;
	fm->_filecopy.pDestDev	= (UDINT) sDevice2;
	
	/* Init: FileCreate */
	fm->_filecreate.enable 	= 1;
	fm->_filecreate.pDevice = (UDINT) fm->Device;
	
	/* Init: FileWrite */
	fm->_filewrite.enable	= 1;
	fm->_filewrite.offset 	= 0;
	fm->_filewrite.pSrc 	= data_adr;
	if ( fm->Type != FU_DRIVE_STREAM )
		fm->_filewrite.len 	= fm->BufferSize;

	/* Init: FileClose */
	fm->_fileclose.enable	= 1;
	fm->_fileclose.ident 	= 0;
	
   	return 0;
}
/* Set Buffer Pointer*/
plcbit FU_setBufPointer(unsigned long fm_adr , unsigned long pMem, unsigned long size)
{
FILEMGR_obj *fm			= (FILEMGR_obj *) fm_adr;
	
	fm->Data_adr		= pMem;
	fm->_filewrite.pSrc = fm->Data_adr;
	fm->_fileread.pDest = fm->Data_adr;

	fm->BufferSize		= size;
	fm->_fileread.len 	= fm->BufferSize;
	fm->_filewrite.len 	= fm->BufferSize;

	return 1;
	
}

/* Special: Allow Index 0 and 1 */
DINT FU_serv( UDINT fm_adr )
{
FILEMGR_obj *fm			= (FILEMGR_obj *) fm_adr;
FILEIN_IF *in 			= (FILEIN_IF   *) fm->_if_in_adr;
FILEOUT_IF *out			= (FILEOUT_IF  *) fm->_if_out_adr;
fiDIR_READ_DATA *fname;
UINT i,j,StartIndex;
BOOL FilterExt = 0, FileFound = 0;

	/* Open Directory and Delete File does not require data buffer*/
	if((!fm->Data_adr) && (out->OpenFile || out->Save || out->SaveAs)) return -100;

	/* Method: Open Directory */
	switch ( fm->_state )
	{
			
		/**** Return Directory Information ****/
		case 0: 
		
			if ( out->OpenDirectory ) {
			
				/* FTP */
				if ( fm->Type == FU_DRIVE_FTP ) {
								
				}
				
				/* File IO */
				else {
					/* Reset Status */
					fm->Status = 0;	
					/* Reset when Directory Opened */
					in->PageNum	= 0;		
					
					ERRxwarning(0xC351, 1, "FileIO: Open Dir");

					/* Attach Path */
					fm->_dirinfo.pPath		= (UDINT) out->SelectedDirectory;
					
					/* Call FBK */
					DirInfo(&fm->_dirinfo);
					
					ERRxwarning(0xC352, 2, "FileIO: Open Dir");	
					
					/* Get FBK output information */
					fm->_numfiles	= fm->_dirinfo.filenum;
					fm->_infostatus	= fm->_dirinfo.status;
					
					/* Directory Info: Returned */
					if ( fm->_infostatus == 0 )	{
						fm->_fileptr	= 0;
						/* File Name Limit */
						if ( fm->_numfiles > fm->MaxDirectoryFiles )
							fm->_numfiles = fm->MaxDirectoryFiles;
						
						/* Init File Name Base Pointer */
						fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
						
						ERRxwarning(0xC353, 3, "FileIO: Open Dir");
						ERRxwarning(0xC353, fm->_fname_adr, "FileIO: _fname_adr");
						
						/* Clear Array */
						for (i=0;i<fm->MaxDirectoryFiles;i++,fname++)
							memset( fname, 0, sizeof(fiDIR_READ_DATA) );
						
						/* Return File Names: */	
						fm->_state++;
						
						/* Message */
						strcpy( fm->Message, "Reading Directory...Please Wait");
						ERRxwarning(0xC354, 4, "FileIO: Open Dir");	
					}
					
					/* Directory Info: Unsuccessful */
					else if ( fm->_infostatus != 65535) {
						strcpy( fm->Message, "FileIO: Directory Info: Error");
						if ( fm->_infostatus == 20799)
							fm->Error = FileIoGetSysError();
						fm->_state 			= 100;
						fm->Status			= -1;				/* FileIO: Open Directory Error */
					}
				}	
			}
			
			/* Method: Select Pattern: Process File */
			else if ( out->OpenFile | out->Save ) {
			
				
				/* Selected File has file name */
				if ( strcmp(out->SelectedFile, "") != 0) {
				
					/* Init File Name Base Pointer */
					fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
					
					/* To Upper Case */
					ConvertToUpper( out->SelectedFile );
					
					/* Cycle Array: Directory */
					for (i=0;i<fm->MaxDirectoryFiles;i++,fname++) {

						/* Located File Index: */
						if ( strcmp( out->SelectedFile, fname->Filename) == 0 ) {
							FileFound	= 1;
							break;
						}		
					}
				}
				
				/* Null Selected File: Error */
				else {
					strcpy( fm->Message, "FTP: Null File Name");
					out->OpenFile = 0;
				}	
			
				if ( FileFound ) {
			
					/* Reset Status */
					fm->Status 			= 0;
					
					/* File Size Error: Streaming can have much larger file size than buffer */
					if ( (fname->Filelength > fm->BufferSize) && (fm->Type != FU_DRIVE_STREAM )) {
						strcpy( fm->Message, "File Open: Too Big");
						fm->_state			= 100;
						fm->Status			= -2;				/* File Open: File is too big */
					}
					
					/* File Size: Okay */
					else {
					
						/* FTP ...Talk to Goran 
						if ( fm->Type == fuDRIVE_FTP ) {*/
						if ( fm->Type == 99 ) {
							
						}

							
						/* File IO */
						else {
							/* Initialize file open structrue */
							fm->_fileopen.pFile		= (UDINT) fname->Filename;
				
							/* Call FBK */
							FileOpen(&fm->_fileopen);
				
							/* Get FBK output information */
							fm->_fileopenstatus = fm->_fileopen.status;
			
							/* File Open: Doesn't Exist */
							if (fm->_fileopenstatus == 20708) {
								strcpy( fm->Message, "File Open: Does not Exist");
								fm->_state			= 100;	
								fm->Status			= -3;				/* File Open: File does not exist */
							}
							
							/* File Open: Success */		
							else if (fm->_fileopenstatus == 0) {
								/* Get Name */
								strcpy( fm->OpenFileName, fname->Filename  );
								fm->OpenFileDate	= fname->Date;
								strcpy( in->OpenFileName, fm->OpenFileName );
								GetExtType( in, out );	
								/* Get Size */
								in->FileLength = fname->Filelength;
								/* Reset Status */
								fm->Status 			= 2;
								strcpy( fm->Message, "File Open: Success");
								
								/* Next Step */
								if ( out->OpenFile ) {
									fm->_fileread.pDest	= fm->Data_adr;
									memset( (void *) fm->_fileread.pDest, 0, fm->BufferSize );
									/* Streaming Read */
									if ( out->StreamEnable ) {
										fm->_state		= 10;	
										fm->Status		= 10;
										out->OpenFile 	= 0;
									}
									/* Standard Read: Clear Memory */
									else	 {
										fm->_state		= 2;
									}
								}	
								else if ( out->Save )
									fm->_state		= 3;	/* File Write */
							}

							/* File Open: File Not Found */
							else if ( fm->_fileopenstatus == fiERR_FILE_NOT_FOUND ) {
								strcpy( fm->Message, "File Open: Not Found");
								out->Save	= 0;
								fm->Status	= 10;				/* File Open: File does not exist */
							}
							
							/* File Open: Error */
							else if (fm->_fileopenstatus != 65535) {	
								strcpy( fm->Message, "File Open: Error");
								fm->_state			= 100;
								fm->Status			= -4;				/* File Open: File Error */
								if (fm->_fileopenstatus == 20799)
									fm->Error = FileIoGetSysError();
							}
						}
					}	
				}
				
				/* Can't Find File */
				else {
					strcpy( fm->Message, "Can not find file");
					out->Save	= 0;
					fm->Status	= -6;				/* File Open: File does not exist */
					fm->_state	= 100;
				}
			}
			
			/* NEW: Use out->SelectedFile */
			/* Delete File */
			else if ( out->DeleteFile ) {

				fm->Status	= 0;
			
				/* To Upper Case */
				ConvertToUpper( out->SelectedFile );							
			
				/* Initialize file Delete structrue */
				fm->_filedelete.pName 	= (UDINT) out->SelectedFile;
				
				/* Call FBK */
            			FileDelete(&fm->_filedelete);
            	
				/* Get FBK output information */
            			fm->_infostatus	= fm->_filedelete.status;

				/* Directory Info: Returned */
				if ( fm->_infostatus == 0 ) {
					strcpy( fm->Message, "Delete File: Success");
					out->DeleteFile 	= 0;
					fm->Status		= 1;
				}	

				/* Delete Info: Not Found...still ok */
				else if ( fm->_infostatus == fiERR_FILE_NOT_FOUND ) {
					strcpy( fm->Message, "Delete File: File Not Found");
					out->DeleteFile 	= 0;
					fm->Status		= 1;
				}
					
				/* Directory Info: Unsuccessful */
				else if ( fm->_infostatus != 65535) {
					strcpy( fm->Message, "Delete File: Error!");
					out->DeleteFile 	= 0;
					fm->Status		= -1;	
				}
			}
			
			/* Method: */
			else if ( out->CopyFile ) {

				if(!fm->Data_adr) return -100;
				
				/* Processing: */
				fm->Status		= 0;
						
				/* To Upper Case */
				ConvertToUpper( out->SelectedFile );	
					
				/* Initialize file Copy structrue */
				fm->_filecopy.pSrc		= (UDINT) out->SelectedFile;
				fm->_filecopy.pDest		= 0;		/* Same Name */
	
				/* Call FBK */
				FileCopy(&fm->_filecopy);
		
				/* Copy File: Doesn't Exist */
				if (fm->_filecopy.status == 20708) {
					strcpy( fm->Message, "Copy File: Does not Exist");
					out->CopyFile 	= 0;	
				}
				
				/* Copy File: Success */		
				else if (fm->_filecopy.status == 0) {
					strcpy( fm->Message, "Copy File: Success");
					out->CopyFile 	= 0;
					fm->Status		= 4;
				}
				
				/* Copy File: Error */
				else if (fm->_filecopy.status != 65535) {	
					strcpy( fm->Message, "Copy File: Error");
					out->CopyFile = 0;
				}
			}
			
			/* Method */
			else if ( out->SaveAs ) {

				
				/* Reset Status */
				fm->Status 			= 0;
				
				/* Create with Extension */
				if ( CreateNewFileName(out) == 0 ) {

					/* Initialize file create structrue */
					fm->_filecreate.pFile	= (UDINT) out->SelectedFile;
						
					/* Call FBK */
					FileCreate(&fm->_filecreate);
					
					/* Get FBK output information */
					fm->_filecreatestatus = fm->_filecreate.status;
		
					/* No Need To Go to Error State! */
					/* File Already Exists: 20705 */
					if (fm->_filecreatestatus == fiERR_EXIST ) {
						strcpy( fm->Message, "File Exists: Updating..");
						out->SaveAs 	= 0;
						out->Save		= 1;
						fm->_state		= 0;
					}
					
					/* File Create: Success */		
					else if (fm->_filecreatestatus == 0) {
						strcpy( fm->Message, "File Create: Success");
						fm->Status		= 3;
						fm->_state		= 3;	/* File Create Success: Perform File Write */
					}
						 
					/* File Create: Error */
					else if (fm->_filecreatestatus != 65535) {	
						strcpy( fm->Message, "File Create: Error");
						fm->_state		= 100;
						fm->Status		= -5;				/* File Create: Error */
						if (fm->_filecreatestatus == fiERR_SYSTEM)
							fm->Error = FileIoGetSysError();
					}
				}
				else {
					strcpy( fm->Message, "File Create: Bad Extension");
					fm->Status		= -6;				/* File Create: Bad Extension */
					fm->_state		= 100;
				}	
			}	
		
		break;
		
		/**** Return File Names ****/	
		case 1:
		
			/* Read File Names */
			if (fm->_fileptr < fm->_numfiles) {
				
				/* Init: Dynamic Read Info */
				fm->_dirread.entry 		= fm->_fileptr;
				
				/* Init File Name Base Pointer: Then Index */
				fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
				fname 	+= fm->_fileptr;	
					
				/* Initialize Directory Read structrue */
				fm->_dirread.pData 		= (UDINT) fname->Filename;

				/* Call FBK */
				DirRead(&fm->_dirread);
				
				/* Update Read Status */
				fm->_readstatus		= fm->_dirread.status;
				
				/* Directory Read: Returned */
				if ( fm->_readstatus == 0) {
					ConvertToUpper( fname->Filename );
					strcpy( fm->Message, "Directory Read: OK");
					fm->_fileptr++;
				}
	
				/* Directory Read: Unsuccessful */
				else if (fm->_readstatus != 65535)	{
					fm->Status		= -7;				/* Directory Read Error */
					fm->_state 		= 100;
					strcpy( fm->Message, "Directory Read: Error");
					if ( fm->_readstatus == 20799)
						fm->Error = FileIoGetSysError();
				}
			}
			
			/* File Names Read */
			else {
				strcpy( fm->Message, "Directory Read: Completed");
				fm->_state			= 0;
				fm->Status 			= 1;
				out->OpenDirectory	= 0;
				FilterExt			= 1;
			}	
			
		break;
	
		/**** Read data from file ****/
		case 2: 
			
			/* Initialize file read structure */
			fm->_fileread.ident 	= fm->_fileopen.ident;
			fm->_fileread.pDest	= fm->Data_adr;
			
			/* Call FBK */
			FileRead(&fm->_fileread);

			/* Get status */
			fm->_filereadstatus = fm->_fileread.status;

			/* File Read: Success */
			if (fm->_filereadstatus == 0) {
				fm->_state 			= 4;
				out->OpenFile 		= 0;
				fm->_fileclose.ident = fm->_fileread.ident;
				strcpy( fm->Message, "File Read: Success");				
			}	
			/* File Read: Error */
			else if (fm->_filereadstatus != 65535)	{
				strcpy( fm->Message, "File Read: Error");
				out->OpenFile 		= 0;
				fm->_state 			= 101;
				fm->Status			= -9;				/* File Read Error */
				fm->_fileclose.ident = fm->_fileread.ident;
				if (fm->_filereadstatus == 20799)
					fm->Error = FileIoGetSysError();
			}
			
		break;
		
		/**** File Write: Part 2 of out->SaveAs Method ****/
		case 3: 

			/* Initialize file write structure */
			if ( out->SaveAs )
				fm->_filewrite.ident 	= fm->_filecreate.ident;
			else if ( out->Save )
				fm->_filewrite.ident	= fm->_fileopen.ident;	
			
			/* Size */
			fm->_filewrite.len 			= out->FileSize;
			fm->_filewrite.pSrc		= fm->Data_adr;
			
			/* Clear Commands */
			out->SaveAs	= 0;
			out->Save   = 0;
			
			/* Call FBK */
			FileWrite(&fm->_filewrite);

			/* Get status */
			fm->_filewritestatus = fm->_filewrite.status;

			/* File Write: Success */
			if (fm->_filewritestatus == 0) {
				fm->_state 	= 4;
				fm->_fileclose.ident = fm->_filewrite.ident;
				strcpy( fm->Message, "File Write: Success");
			}	
			/* File Write: Error */
			else if (fm->_filewritestatus != 65535)	{
				strcpy( fm->Message, "File Write: Error");
				fm->Status	= -10;				/* File Write Error */
				fm->_state 	= 101;
				fm->_fileclose.ident = fm->_filewrite.ident;
				if (fm->_filewritestatus == 20799)
					fm->Error = FileIoGetSysError();
			}
			
		break;
				
		/**** FileClose: called after each fileRead or fileWrite ****/
		case 4:

			/* Call FBK */
			FileClose(&fm->_fileclose);

			/* Verify status */
			if (fm->_fileclose.status == 0)  { 
				fm->Status	= 4;							
				fm->_state	= 0;
			}
			else if (fm->_fileclose.status != 65535) {
				strcpy( fm->Message, "File Close: Error");	
				fm->Status	= -11;				/* File Close Error */
				fm->_state 	= 0;
				if (fm->_fileclose.status == 20799)
		 			fm->Error = FileIoGetSysError();
			}

		break;

		/********************************************************************/
		/**** Streaming Output ****/
		case 10: 

			/* Wait Stream Command: Initialize file read structure */
			if ( out->StreamGo ) {
				fm->_fileread.ident 	= fm->_fileopen.ident;
				fm->_fileread.offset	= out->StreamOffset;
				fm->_fileread.len		= out->StreamLength;
				fm->_fileread.pDest	= out->Stream_adr;
				/* Response Signals: Reset */
				in->StreamReady		= 0;
				in->StreamError		= 0;
				fm->_state++;
			}
			
			/* Close Connection */
			else if ( out->StreamClose ) {
				out->StreamClose		= 0;
				fm->_fileclose.ident 	= fm->_fileread.ident;
				fm->_state			= 4;
			}
			
		break;

		/********************************************************************/
		/**** Streaming Output ****/
		case 11: 

			/* Call FBK */
			FileRead(&fm->_fileread);

			/* Get status */
			fm->_filereadstatus = fm->_fileread.status;

			/* File Read: Success */
			if (fm->_filereadstatus == 0) {
				out->StreamGo		= 0;
				in->StreamReady		= 1;					/* Response: Ok */
				fm->_state 			= 10;				
			}	
			/* File Read: Error */
			else if (fm->_filereadstatus != 65535)	{
				out->StreamGo		= 0;
				in->StreamError		= 1;					/* Response: Error */
				fm->_state 			= 101;
				strcpy( fm->Message, "File Stream: Error");
				fm->Status			= -11;				/* File Stream: Error */
				fm->_fileclose.ident = fm->_fileread.ident;
				if (fm->_filereadstatus == 20799)
					fm->Error = FileIoGetSysError();
			}
			
		break;
		/********************************************************************/
			
		/**** Error State ****/
		case 100:
		
			/* Clear Commands */
			out->OpenDirectory 	= 0;
			out->OpenFile 		= 0;
			out->SaveAs 			= 0;
			out->Save			= 0;
			out->StreamClose		= 0;
			
			/* Command: Reset Error */
			fm->_state			= 0;	
				
		break;
		
		/**** Error State w. FileClose ****/
		case 101:
		
			/* Call FBK */
			FileClose(&fm->_fileclose);

			/* Verify status */
			if (fm->_fileclose.status == 0)  { 						
				fm->_state	= 100;
			}
			else if (fm->_fileclose.status != 65535) {
				strcpy( fm->Message, "File Close: Error");	
				fm->Status	= -12;				/* File Close Error */
				fm->_state 	= 100;
				if (fm->_fileclose.status == 20799)
		 			fm->Error = FileIoGetSysError();
			}
		break;

		default:
		break;
		
	}

	/* Input Interface: Files in Directory */
	in->TotalDirectoryFileNum	= fm->_numfiles;
	/* Input Interface: Message */
	strcpy( in->Message, fm->Message );
	
	/* Process Page Scroll: Up */
	if ( out->PageUp ) {
		out->PageUp = 0;
		FilterExt 	= 1;
		if (in->PageNum > 0) 
			in->PageNum--;
	}
	/* Process Page Scroll: Down */
	if ( out->PageDown ) {
		out->PageDown 	= 0;
		FilterExt		= 1;
		if (in->PageNum < (FU_MAX_PAGE_FILES-1) && (in->TotalPageFileNum == FU_MAX_PAGE_FILES) ) 
			in->PageNum++;
	}
	
	/* Perform Extension Filter */
	if ( FilterExt | out->Refresh ) {
		
		/* Reset */
		out->Refresh	= 0;
		
		/* Locate Total Files per Extension */
		j = 0;
		
		/* Init File Name Base Pointer */
		fname	= (fiDIR_READ_DATA *) fm->_fname_adr;

		/* Cycle Array */
		for (i=0;i<fm->_numfiles;i++,fname++) {
		
			/* All Extensions */
			if ( out->ExtensionIndex == 0 )	
				j++;

			/* Special: Extension 0 and 1: Specific Extension */
			else if ( out->ExtensionIndex == 10 ) {
				if (	(FindString( fname->Filename, out->Extension[0].Name) == 0) || 
					(FindString( fname->Filename, out->Extension[1].Name) == 0) )
					j++;
			}	
			
			/* Specific Extension */
			else if ( out->ExtensionIndex <= FU_MAX_EXT ) {
				if (FindString( fname->Filename, out->Extension[out->ExtensionIndex-1].Name) == 0 )
					j++;
			}		
		}

		/* Assign */
		in->TotalDirectoryFileNum	= j;

		/* Locate Total Page Files per Extension */		
		j = 0;
		
		/* Page Scroll Index */
	    StartIndex = in->PageNum * FU_MAX_PAGE_FILES;
	    
		/* Clear */
		for (i=0;i<FU_MAX_PAGE_FILES;i++) {
			memset( (UDINT *) &in->FileNames[i][0], 0, sizeof(in->FileNames[0]));
			in->FileDates[i] = 0;
			in->FileSizes[i] = 0;
		}
				
		/* Init File Name Base Pointer */
		fname	= (fiDIR_READ_DATA *) fm->_fname_adr;
		
		/* Cycle Array */
		for (i=0;i<fm->_numfiles;i++,fname++) {		
			
			/* View All Extensions */
			if ( out->ExtensionIndex == 0 ) {
				if ( j >= StartIndex ) {
					strcpy( &in->FileNames[j-StartIndex][0], fname->Filename );
					in->FileDates[j-StartIndex]	= fname->Date;
					in->FileSizes[j-StartIndex]	= fname->Filelength;
				}	
				j++;
			}
			
			/* Special: Allow Index 0 and 1 */
			else if ( out->ExtensionIndex == 10 ) {
				if ( 	(FindString( fname->Filename, out->Extension[0].Name) == 0) || 
					(FindString( fname->Filename, out->Extension[1].Name) == 0) ) {
					if ( j >= StartIndex ) {
						strcpy( &in->FileNames[j-StartIndex][0],fname->Filename);
						in->FileDates[j-StartIndex]	= fname->Date;
						in->FileSizes[j-StartIndex]	= fname->Filelength;
					}	
					j++;
				}
			}
			
			/* Locate File with Proper Extension */
			else if ( out->ExtensionIndex <= FU_MAX_EXT ) {
				if (FindString( fname->Filename, out->Extension[out->ExtensionIndex-1].Name) == 0 ) {
					if ( j >= StartIndex ) {
						strcpy( &in->FileNames[j-StartIndex][0],fname->Filename);
						in->FileDates[j-StartIndex]	= fname->Date;
						in->FileSizes[j-StartIndex]	= fname->Filelength;
					}	
					j++;
				}
			}
			
			/* Check Page Disply Limit */
			if ( j >= (StartIndex + FU_MAX_PAGE_FILES) ) {
				break;	
			}		
			
		}
		
		/* Files in Page View */
		in->TotalPageFileNum 		= j - StartIndex;
	}
	
	/* Method: Clear Status */
	if ( out->ClearStatus )  {
		out->ClearStatus 	= 0;
		fm->Status			= 0;
	}
	in->Status				= fm->Status;	
	
	return 0;
}
/*********************************************************
* NAME		:	FU_AddExt (Public)
*
* OBJECT	:	FILEOUT_IF, FILEIN_IF
*
* FUNCTION	:	Changes the OpenFileName string to the supplied
				extension index

				i.e.	.OpenFileName	= "PatternFile1.ctf"
					.ExtensionIndex	= 2 	("col")

					Result:

					.OpenFileName	= "PatternFile1.col"
*			  
*
* PARAMETERS:	out
				in
				
*
* RETURN	:	-1: 		Error "." not found
				0:		Success
				1:		Bad Extension Index
*
* HISTORY	:	02.07.07 - Version 1
				
*********************************************************/
DINT FU_AddExt( FILEOUT_IF *out, FILEIN_IF *in )
{	
STRING base[34];
USINT i;

	/* Get Local Copy */
	strcpy( base, in->OpenFileName );

	/* Locate '.' */
	for (i=0;i<strlen(base);i++) {
		if ( base[i] == '.' ) {
			base[i] = 0;			/* Add Null */
			strcpy( out->NewFileNameBase, base	);
			return CreateNewFileName( out );
		}
	}

	return -1;
}

 /* */
BOOL FU_DeleteExt( STRING s[35], STRING d[35]) 
{
USINT i;

	/* Locate '.' */
	for (i=0;i<strlen(s);i++) {
		d[i]	= s[i];
		if ( s[i] == '.' ) {
			d[i]	= 0;
			return 1;
		}
	}
	
	/* Not Found */
	return 0;
}

#if 0
DINT FU_AddExt( UDINT fm_adr, STRING s[35], STRING ext[4] )
{
FILEMGR_obj *fm	= (FILEMGR_obj *) fm_adr;
FILEOUT_IF *out	= (FILEOUT_IF  *) fm->_if_out_adr;
STRING _s[35];
USINT i;

	/* Source String: Local Copy */
	strcpy( _s, s );

	/* Locate '.' */
	for (i=0;i<strlen(_s);i++) {
		/* Found Extension */
		if ( _s[i] == '.' ) {
			memcpy( &_s[i+1], ext, 3); 
			/* Add Null */
			_s[i+4] = 0;
			/* To Upper */
			ConvertToUpper( _s );
			/* Copy */
			strcpy( out->SelectedFile, _s );	
			/* Return */
			return (i);
		}
	}
	
	return 0;
} 
#endif

void ConvertToUpper( STRING s[35] )
{
UINT i;

	/* Verify Letters Only */
	for (i=0;(i<strlen(s)) && (i<35);i++) {
		if ( ((s[i] >= 'A') && (s[i] <= 'Z')) || 
			 ((s[i] >= 'a') && (s[i] <= 'z'))    ) {
			s[i] = s[i] & 0xDF;
		}
	}	
}

/* */
DINT FindString( STRING s[35], STRING ext[4] ) 
{
USINT i, j, s_ext[5];

	/* Locate '.' */
	for (i=0;i<strlen(s);i++) {
		if ( s[i] == '.' ) {
			/* Convert to Next 3 Chars to Upper */
			for (j=1;j<4;j++)
				s[i+j] = s[i+j] & 0xDF;
			memcpy( s_ext,&s[i], 4);
			/* Add Null */
			s_ext[4] = 0;
			return strcmp(s_ext, ext);
			break;
		}
	}
	
	/* Not Found */
	return 255;
}
 
/* */
DINT GetExtType( FILEIN_IF *in, FILEOUT_IF *out ) 
{
USINT i,j, s_ext[5];

	/* Locate '.' */
	for (i=0;i<strlen(in->OpenFileName);i++) {
		if ( in->OpenFileName[i] == '.' ) {
			memcpy( s_ext,&in->OpenFileName[i], 4);
			/* Add Null */
			s_ext[4] = 0;
			for (j=0;j<5;j++) {
				if (strcmp(s_ext, out->Extension[j].Name) == 0) {
					in->ExtensionIndex = j;
					return 0;
				}	
			}	
		}
	}
	
	/* */
	return -1;
}


DINT CreateNewFileName( FILEOUT_IF *out )
{	
STRING base[34];

	if ( (FU_MAX_EXT + 1) > out->ExtensionIndex > 0 ) {
		/* Process */	
		memset( base, 0, sizeof(base));
		strcpy( base, out->NewFileNameBase );
		strcat( base, out->Extension[out->ExtensionIndex-1].Name );
		strcpy( out->SelectedFile, base);
		return 0;
	}
	else
		return 1;		
}

UINT SortDirectory( UDINT fm_adr )
{
FILEMGR_obj *fm			= (FILEMGR_obj *) fm_adr;
fiDIR_READ_DATA *s1, *s2;
fiDIR_READ_DATA hold;
UINT i,j=0;

	/* Init File Name Base Pointer */
	s1	= (fiDIR_READ_DATA *) fm->_fname_adr;
	s2	= (fiDIR_READ_DATA *) fm->_fname_adr;
	
	s2++;
	
		/* Cycle Array: Compare */
		for (i=1;i<fm->_numfiles;i++,s1++,s2++) {		
			if ( strcmp(s1->Filename, s2->Filename) > 0 ) {
				memcpy( &hold, s1, 	sizeof(fiDIR_READ_DATA));
				memcpy( s1, s2, 		sizeof(fiDIR_READ_DATA));
				memcpy( s2, &hold, 	sizeof(fiDIR_READ_DATA));
				j++;
			}
		}
		
	return j;
}	