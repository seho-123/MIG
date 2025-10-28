/*********************************************************
* FILE		:		xml_lib.c(Task)
*
* OBJECTs	:	 
*
* OVERVIEW	:	
*			
*	
* HISTORY : 			08.31.07	- Version 1.0
					09.18.07	- Version 1.1
					12.03.07	- Version 1.2
					12.19.07	- Version 1.31 [Small changes]
					01.18.08	- Version	 1.50 [handle server fault response]
					02.07.08	- Version 1.51 [Logger]
					02.14.08	- Version 1.52 [STRING_LENGTH_MAX 40]
					02.19.08	- Version 1.53 [Client Side: Clear server area]
					02.28.08	- Version 1.54 [Null String]
					10.16.09	- modified by KK
					10.19.11 - type DATE_TIME added KK
				
*
* DEPENDANCIES:
*********************************************************/
#include <bur/plc.h>			
#include <bur/plctypes.h>
#include <AsString.h>
#include <string.h>
#include <sys_lib.h>

#include <xml_def.h>

/*********************************************************
* NAME		:	XMLp_CallCreate
*
* OBJECT	:	XML_Method_typ
*
* FUNCTION	:	Client XML Method Call:

					- Creates XML MethodCall string
					- Does not add HTTP formatting
					- Stores created string in .Message array
*
*
* PARAM 	:		
				Name: Method Name to Create Client XML Message

					- Must be a valid PV Name with specific elements
					- .client structure must exist with at least 1 element
					- .client structure can contain
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				

*********************************************************/
DINT XMLp_CallCreate( XML_Method_typ *m, unsigned char* PVNames)
{
STRING PVBaseName[STRING_LENGTH_MAX];
UDINT offset=0, datalen, dataadr;
UINT indent=0 ;
UDINT indx=0 , nameLen = 0;
DINT status;
BOOL done = 0;
int i ,k;

	/* Bad Init */
	if ( m->InitStatus != 0 ) {
		m->Status	= XML_STATUS_CALL_ERROR;
		return -1;
	}

	/* Clear */
	memset( (USINT *) m->pMessage, 	0x00, m->BufferSize );
	memset( &m->CreateInfo, 	0x00, sizeof(XML_CreateInfo_typ));

	
	/********************************************************************************************************/

	/*	<?version: "1.0" ?> 	*/	
	_AddStringToFile( (USINT *) m->pMessage, &offset, 	XML_BODY_VERSION, 			"\r\n" );
	
	/*	<params>		*/
	xml_InsertKey( (USINT *) m->pMessage, XML_BODY_PARAMS_IDX,	&offset,	indent++, XML_OPEN);

	
	while(!done)
	{	

		strtok(&PVNames[indx] , "&");
		//nameLen = strlen(&PVNames[indx]);
		i=0;k=0;
		while(PVNames[indx + i]!=0){
			if(PVNames[indx+ i]!=' ')
				PVBaseName[k++]= PVNames[indx + i];
			i++;				
		}
		PVBaseName[k]=0;
		indx+=(strlen(&PVNames[indx])+1);
		
		if(strlen(PVBaseName)==0){
			done = YES;
			break;	
		}
		

						
		/*	<param> 	*/				
		xml_InsertKey( (USINT *) m->pMessage, XML_BODY_PARAM_IDX, 	&offset,	indent++, XML_OPEN);

		/* name of param */
		xml_InsertComplexLine( (USINT *) m->pMessage,	XML_BODY_NAME_IDX, 0,	
								&offset, PVBaseName, indent++ );
			
		/* members */
		status = xml_AddItems(  (USINT *) m->pMessage, PVBaseName, &indent, &offset, m->BufferSize, m->DiagInfo.Logger );
		
		/* Error */
		if ( status < 0 ) {
			m->CreateInfo.ErrorNum 	= status; 
			m->Status				= status;			
			return -1;
		}
		
		/* Success */
		else {
			
			/*			</param> 	*/
			/*		</params>		*/
			xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_PARAM_IDX, 	&offset,	indent--, XML_CLOSE);		
						
		}

	}
	
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_PARAMS_IDX,	&offset,	indent--, XML_CLOSE);
	
	/* Update: Length in Bytes */
	m->CreateInfo.LengthInBytes	= offset;
	
	/* Diagnostic Logger */
	if ( m->DiagInfo.Logger ) {
		ERRxwarning(0xC350, 0, "Call Create");
		ERRxwarning(0xC351, m->CreateInfo.LengthInBytes, m->MethodName);
	}

	
	return YES;

}
/*********************************************************
* NAME		:	XMLp_CallParse
*
* OBJECT	:	
*
* FUNCTION	:	Server XML Method Parse

				This function should be invoked after the TCP/IP
				frame has been processed, the HTTP header processed
				and the XML body move to the .Message array.

				This function will anaylze the XML .Message

					- Parse Line by Line
					- Extract Keywords and values
					
*
*
* PARAM 	:		
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				09.10.07	- Add recursive features
				09.18.07  - Read Function
				

*********************************************************/
DINT XMLp_CallParse( XML_Method_typ *m )
{
STRING PVName[STRING_LENGTH_MAX], sErrorText[STRING_LENGTH_MAX];
UDINT offset = 0, datalen, dataadr;
LineElement_typ	Line;
DINT status = 0;
BOOL done = NO, alldone = NO , LastLineDetected, IgnoreNotFound;
int i;


	/* Bad Init */
	if ( m->InitStatus != 0 ) {
		m->Status	= XML_STATUS_CALL_ERROR;
		return -1;
	}
	
	/* Reset Diag and Fault Data */
	IgnoreNotFound				= m->ParseInfo.IgnoreNotFound;
	memset( &m->ParseInfo, 0, sizeof(XML_ParseInfo_typ) );
	m->ParseInfo.IgnoreNotFound	= IgnoreNotFound;
	
	m->Fault		= NO;
	m->FaultCode	= 0;
	strcpy( m->FaultString, "" );

	/* Line 1: 		<?xml version="1.0"?> 		*/
	if ( xml_GetLineInformation( m, &offset, &Line, &LastLineDetected )  != XML_VERSION ) {
		return xml_FaultDetected( m, 			0, "?<Version>", 		0 );
		
	}

	/*	<params>		*/
	if ( xml_GetLineInformation( m, &offset, &Line, &LastLineDetected ) != XML_BODY_PARAMS_IDX ) {
		return xml_FaultDetected( m, 0, "?<params>", 0 );
	}

	
		
	while(!alldone){
	
		/*		<param> 	*/	
		if ( xml_GetLineInformation( m, &offset, &Line, &LastLineDetected ) == XML_BODY_PARAM_IDX ) {

			if ( xml_GetLineInformation( m, &offset, &Line, &LastLineDetected ) == XML_BODY_NAME_IDX ) {
				ERRxwarning(0xC350, 1957, Line.sValue);
				strcpy(PVName , Line.sValue);
			}
			done = 0;
			/* Process All Values */
			while (!done ) {
			
				/*		<value> 	*/
				status = xml_GetLineInformation( m, &offset, &Line, &LastLineDetected);
				if ( status < 0 ) {
					return xml_FaultDetected( m, status, "?", 0 );
				}

				/* Good Response */
				switch ( status ) {
					case XML_BODY_VALUE_IDX:

						/* */
						switch ( Line.Key[1] ) {
							case XML_BODY_TYPE_I4_IDX:
							case XML_BODY_TYPE_BOOL_IDX:
							case XML_BODY_TYPE_DOUBLE_IDX:
							case XML_BODY_TYPE_STRING_IDX:
							case XML_BODY_TYPE_DATE_TIME_IDX:
							case XML_BODY_TYPE_NULL_IDX:
							
								/* Recursive Function Entry:  */
								status = xml_GetElement( m, PVName, &offset, 0, &Line );
								if ( status < 0 ) {
									strcpy( sErrorText, "!: " );
									strcat( sErrorText, PVName );
									if ( m->ParseInfo.IgnoreNotFound ) {
										if ( m->ParseInfo.ItemsNotFound < 10 ) {
											strcpy( m->ParseInfo.ErrorMessage[m->ParseInfo.ItemsNotFound], "Missing: ");
											strcat( m->ParseInfo.ErrorMessage[m->ParseInfo.ItemsNotFound], sErrorText );
										}
										m->ParseInfo.ItemsNotFound++;
									}
									else
										return xml_FaultDetected( m, status, sErrorText, 0 );
								}
								
							break;

							default:
								return xml_FaultDetected( m, Line.Key[1], "?<value>", 0 );
							break;
						}
					break;

					case XML_BODY_PARAM_IDX:
					
						done	= YES;
						
					break;

					default:
						return xml_FaultDetected( m, status, "?</param>", 0 );
					break;
					
				}
				
				
			}
		}
		else		
			alldone = YES;	

	}
	
	/* Success */
	return YES;
}



/*********************************************************
* NAME		:	XMLp_Init
*
* OBJECT	:	XML_Method_typ
*
* FUNCTION	:	
*
*
* PARAM 	:		
				
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				02.07.08	- Logger
				09.05.09	- ParseInfo.IgnoreNotFound

*********************************************************/
BOOL XMLp_Init( XML_Method_typ *m,  UDINT BufferSize )
{

	

	/* Null Pointer: Allocate */
	if ( !m->pMessage ) {
		m->InitStatus		= TMP_alloc(BufferSize, (void**)	&m->pMessage);  
	}
	/* Pointer Assigned: Clear */
	else {
		TMP_free(m->BufferSize, (void**)	m->pMessage);  
		m->InitStatus		= TMP_alloc(BufferSize, (void**)	&m->pMessage);  
	}

	/* Return:  */
	if ( !m->InitStatus ) {
		memset(m->pMessage , 0 , BufferSize);
		m->BufferSize				= BufferSize;
		//m->DiagInfo.Logger			= 0;
		return YES;
	}
	else
		return NO;
}

/*********************************************************
* NAME		:	XMLp_Destroy
*
* OBJECT	:	XML_Method_typ
*
* FUNCTION	:	
*
*
* PARAM 	:		
				
	
*			
*
* HISTORY	:	10.21.09	- created by KK
*********************************************************/
BOOL XMLp_Destroy( XML_Method_typ *m )
{	

	/* Good Pointer: Free memory  */
	if ( m->pMessage ) {
		if(TMP_free(m->BufferSize, (void**)	m->pMessage))
			return NO;  
	}
	m->BufferSize = 0;
	m->pMessage = 0;
	return YES;
}

/*********************************************************
* NAME: 			AddIndex Public
*
*
* FUNCTION: 		
*		   
*
* PARAMETERS:	None
*
* RETURN:		
*
* HISTORY:		11.10.05		- 
*				
*			  
*********************************************************/
DINT AddIndex( STRING Dest[STRING_LENGTH_MAX], STRING Name[STRING_LENGTH_MAX], USINT num )
{
STRING s[3];

	strcpy( Dest, Name);
	strcat( Dest, "[" );
	itoa(num, (UDINT) s );	
	strcat( Dest, s );
	strcat( Dest, "]" );
	return strlen(Dest);
}

/*********************************************************
* NAME		:	xml_SetErrorFlag(Helper)
*
* OBJECT	:	XML_Method_typ
*
* FUNCTION	:	
*	 
* RETURN	:	DINT:	>0 = success
						<0 = Error

				

*
* HISTORY	:	10.06.07 - Version 1
*
*********************************************************/
DINT xml_SetErrorFlag( XML_Method_typ *m  )
{
STRING PVName[100];
UDINT  datalen, dataadr;

	/* Update Error Signal: Prepare PV Name */
	strcpy( 	PVName, m->PVClient );
	strcat( 	PVName, ".Error" );
	if ( !PV_xgetadr(PVName, &dataadr, &datalen) ) {
		if ( dataadr > 0 ) {
			*(BOOL *) dataadr	= YES;
			return YES;
		}
	}

	/* Error */
	return -1;
	
}


/*********************************************************
* NAME		:	xml_GetLineInformation(Helper)
*
* OBJECT	:	N/A
*
* FUNCTION	:	Parser Operation: Retrieves the next line of XML text and
				populates LineElement_typ and adjusts *offset.
*	 
* RETURN	:	DINT: 	>0 = success
						<0 = Error

				

*
* HISTORY	:	06.13.06 - Version 1
				09.18.07	- Allow <i4>12</i4> on single line
				02.07.08	- Bad XML Detection and Logger
*
*********************************************************/
DINT xml_GetLineInformation( XML_Method_typ *m, UDINT	*offset, LineElement_typ *line, BOOL *lastlinedected )
{
DINT status;
STRING LineText[120], ItemText[120],sValue[STRING_LENGTH_MAX];
BOOL done 		= OFF, Slash;
UINT Levels, idx 	= 0;
UDINT LineOffset 	= 0;
USINT *msg		= (USINT *) m->pMessage;

	/* Clear */
	memset ( line, 0x00, sizeof(LineElement_typ) );

	/* Get Entire Line */
	_xmlGetLine( msg, offset, LineText, lastlinedected, &Levels );

	/* Process Each Item */
	while ( !done ) {
		
		status	= _xmlGetStringFromFile( LineText, &LineOffset, ItemText );

		switch ( status ) {

			/* Value */
			case 1:
				line->NumValues++;
				if ( strlen(ItemText) >= STRING_LENGTH_MAX )
					ItemText[STRING_LENGTH_MAX - 1]	= 0;
				strcpy( sValue, ItemText );
			break;

			/* Keyword */
			case 2:
				if ( idx < 4 ) {
					line->Key[idx]	= xml_GetKeyIndex( ItemText, &Slash );
					if ( line->Key[idx] < 0 ) {
						if ( m->DiagInfo.Logger ) {
							ERRxwarning(0xC350, line->Key[idx], "Bad XML");
							ERRxwarning(0xC350, line->Key[idx], ItemText);	
						}
						return -11;
					}
					else {
						idx++;
						line->NumKeys++;
						if ( Slash )	
							line->NumCloseKeys++;
						else	 
							line->NumOpenKeys++;
					}
				}
				else 
					return -10;
			break;

			default:
				done = YES;
			break;
		}	
	}


	/* Sanity Check */
	switch ( line->NumValues ) {
		case 0:
			/* Assume Null String: OK */
			if ( line->Key[1] == XML_BODY_TYPE_STRING_IDX ) {
				strcpy( line->sValue, "" );
			}
			else if ( line->NumKeys > 1 )
				return -20;
			
			/* Return: Success */
			m->ParseInfo.LinesProcessed++;
			return line->Key[0];
		break;

		case 1:
			if ( line->NumOpenKeys != line->NumCloseKeys )
				return -21;
			else if ( line->NumKeys == 0 )
				return -22;

			/* <i4>10</i4> */
			switch ( line->Key[0] ) {
				
				case XML_BODY_TYPE_I4_IDX:
				case XML_BODY_TYPE_DATE_TIME_IDX:
					line->iValue	= atoi( (UDINT) sValue);
				break;
				
				case XML_BODY_TYPE_BOOL_IDX:
					if ( !strcmp(sValue, XML_FALSE) )
						line->bValue	= OFF;
					else if ( !strcmp(sValue, XML_TRUE) )
						line->bValue	= ON;
					else 
						line->bValue	= (BOOL) atoi( (UDINT) sValue);
				break;
				
				case XML_BODY_TYPE_DOUBLE_IDX:
					line->rValue	= atof( (UDINT) sValue);
				break;

				case XML_BODY_METHOD_NAME_IDX:
				case XML_BODY_TYPE_STRING_IDX:
				case XML_BODY_NAME_IDX:
					strcpy( line->sValue, sValue );
				break;

				/* <value><i4>10</i4></value>*/
				case XML_BODY_VALUE_IDX:
					
					/* Determine Type: */
					switch ( line->Key[1] ) {
						
						case XML_BODY_TYPE_I4_IDX:
						case XML_BODY_TYPE_DATE_TIME_IDX:
							line->iValue	= atoi( (UDINT) sValue);
						break;
						
						case XML_BODY_TYPE_BOOL_IDX:
							if ( !strcmp(sValue, XML_FALSE) )
								line->bValue	= OFF;
							else if ( !strcmp(sValue, XML_TRUE) )
								line->bValue	= ON;
							else 
								line->bValue	= (BOOL) atoi( (UDINT) sValue);
						break;
						
						case XML_BODY_TYPE_DOUBLE_IDX:
							line->rValue	= atof( (UDINT) sValue);
						break;

						case XML_BODY_METHOD_NAME_IDX:
						case XML_BODY_TYPE_STRING_IDX:
						case XML_BODY_NAME_IDX:
							strcpy( line->sValue, sValue );
						break;

						default:
							return -27;
						break;
					}
				break;

				default:
					return -26;
				break;
			}

			/* Return: Success */
			m->ParseInfo.LinesProcessed++;
			return line->Key[0];

		break;

		default:
			return -24;
		break;
	}

	return -25;
	
}
/*********************************************************
* NAME		:	xml_GetElement(Helper)
*
* OBJECT	:	N/A
*
* FUNCTION	:	Parser Operation: Primary Recursive Function

				Processes a level of <value>/</value> fields.

				
*	 
* RETURN	:	DINT:	1 	= success
						<0 	= Error

				

*
* HISTORY	:	06.13.06 - Version 1
				09.18.07	- Handle <i4> as first element
				02.07.08	- Logger
*
*********************************************************/
DINT xml_GetElement( XML_Method_typ *m, STRING *pBaseName, UDINT	*offset, UINT idx, LineElement_typ *line)
{
DINT status;
BOOL done = OFF;
STRING StructName[STRING_LENGTH_MAX], Name[100];
BOOL LastLineDetected;
DataElement_typ MemberDataElement;

	/* Line 1: <value><i4>12</i4></value> */
	if ( line->Key[1] > XML_BODY_TYPE_NULL_IDX ) {
		return xml_ProcessValue( pBaseName, line, idx );
	}
	
	/* Line 1: <value> */
	else {

		status	= xml_GetLineInformation(m, offset, line, &LastLineDetected );

		/* Line 2: <struct> or <array> or [<i4>] */
		switch ( status )  {

			/* Line 2: <i4> or <boolean> or <string> or <double> */
			case XML_BODY_TYPE_I4_IDX:
			case XML_BODY_TYPE_BOOL_IDX:	
			case XML_BODY_TYPE_DOUBLE_IDX:
			case XML_BODY_TYPE_STRING_IDX:
			case XML_BODY_TYPE_DATE_TIME_IDX:	
				status = xml_ProcessValue( pBaseName, line, idx );
				if ( status < 0 )
					return -34;
				
				/* </value> */
				switch ( xml_GetLineInformation(m, offset, line, &LastLineDetected ) )  {
		
					case XML_BODY_VALUE_IDX:
						if ( line->NumCloseKeys != 1 ) {
							return -32;
						}
						return YES;
					break;

					default:	
						return -33;
					break;	
				}
				
			break;

			/* Line 2: <array> */
			case XML_BODY_ARRAY_IDX:

				/* Line 3: <data> */
				switch ( xml_GetLineInformation(m, offset, line, &LastLineDetected) )  {
					case XML_BODY_DATA_IDX:

						idx = 0;
						
						while (!done) {
							
							/* Line 4: <value><i4>12</i4></value> */
							switch ( xml_GetLineInformation( m, offset, line, &LastLineDetected) ) {

								/* <value> */
								case XML_BODY_VALUE_IDX:

									strcpy( Name, pBaseName );

									/* Structure Array? */
									status	= PV_ninfo( Name, &MemberDataElement.Type, &MemberDataElement.Length, &MemberDataElement.Dim);
									if ( status == 0 ) {
										if ( MemberDataElement.Type == 15 ) {
											xml_AppendIndex( Name, idx );
										}
									}
									else {
										if ( m->DiagInfo.Logger ) {
											ERRxwarning(0xC350, status, "PV_ninfo: Status");
											ERRxwarning(0xC350, 1, Name);
										}
										return -33;	
									}
			
									/* Launch Recursive Function */
									status	= xml_GetElement( m, Name, offset, idx, line );
									if ( status < 0 ) {
										if ( m->ParseInfo.IgnoreNotFound ) {
											if ( m->ParseInfo.ItemsNotFound < 10 )
												strcpy( m->ParseInfo.ErrorMessage[m->ParseInfo.ItemsNotFound], Name );
											m->ParseInfo.ItemsNotFound++;
										}
										else
											return -status;
									}
									
									idx++;
								break;

								/* </data> */
								case XML_BODY_DATA_IDX:
									if ( line->NumCloseKeys == 1 ) {
										switch ( xml_GetLineInformation( m, offset, line, &LastLineDetected) ) {
											case XML_BODY_ARRAY_IDX:
												if ( line->NumCloseKeys == 1 ) {
													switch ( xml_GetLineInformation( m, offset, line, &LastLineDetected) ) {
														case XML_BODY_VALUE_IDX:
															if ( line->NumCloseKeys != 1 ) 
																return -6;
															
															return YES;
														break;
														default:
															return -2;
														break;

													}
												}	
											break;
											default:
												return -3;
											break;
										}
									}
								break;
								
								default:	
									return -4;
								break;
							}
						}
					break;

					default:
						return -5;
					break;
				
				}
			break;

			/* Line 2: <struct> */
			case XML_BODY_STRUCT_IDX:

				/* Process All Members */
				while (!done) {
					
					/* Line 3: <member> */
					switch ( xml_GetLineInformation( m, offset, line, &LastLineDetected) ) {
						
						case XML_BODY_MEMBER_IDX:
							/* Line 4: <name>Level</name> */
							switch ( xml_GetLineInformation( m, offset, line, &LastLineDetected) ) {

								/* <name> */
								case XML_BODY_NAME_IDX:
									strcpy( StructName, line->sValue );
									strcpy( Name, pBaseName );
									strcat( Name, "." );
									strcat( Name, StructName );

									status	= PV_ninfo( Name, &MemberDataElement.Type, &MemberDataElement.Length, &MemberDataElement.Dim);
									if ( status != 0 ) {
										if ( m->DiagInfo.Logger ) {
											ERRxwarning(0xC350, status, "PV_ninfo");
											ERRxwarning(0xC350, 1, pBaseName);
											ERRxwarning(0xC350, 2, Name);
											ERRxwarning(0xC350, 3, StructName);
										}

										if ( !m->ParseInfo.IgnoreNotFound ) 
											return -32;
									}

									/* <value> */
									switch ( xml_GetLineInformation( m, offset, line, &LastLineDetected) ) {

										/* <value> */
										case XML_BODY_VALUE_IDX:
											/* Launch Recursive Function */
											status	= xml_GetElement( m, Name, offset, 0, line );
											if ( status < 0 ) {
												if ( m->ParseInfo.IgnoreNotFound ) {
													if ( m->ParseInfo.ItemsNotFound < 10 ) {
														strcpy( m->ParseInfo.ErrorMessage[m->ParseInfo.ItemsNotFound], "Missing1: ");
														strcat( m->ParseInfo.ErrorMessage[m->ParseInfo.ItemsNotFound], Name);
													}
													m->ParseInfo.ItemsNotFound++;
												}
												else
													return -status;
											}
											
											switch ( xml_GetLineInformation( m, offset, line, &LastLineDetected) ) {
												/* </member> */
												case XML_BODY_MEMBER_IDX:
													if ( line->NumCloseKeys != 1 ) {
														return -20;
													}
												break;
												#if 0
												/* </value> */
												case XML_BODY_VALUE_IDX:
													if ( line->NumCloseKeys != 1 ) {
														return -28;
													}
													switch ( xml_GetLineInformation( m, offset, line, &LastLineDetected) ) {
														/* </member> */
														case XML_BODY_MEMBER_IDX:
															if ( line->NumCloseKeys != 1 ) {
																return -27;
															}
														break;
														default:
															return -29;
														break;
													}
												break;
												#endif
												default:
													if ( m->DiagInfo.Logger ) 
														ERRxwarning(0xC350, line->Key[0], "Key[0]");
													return -31;
												break;
											}
					
										break;
										
										default:
											return -22;
										break;
									}									

									
									
								break;
								
								default:
									return -23;
								break;
							}
						break;

						/* </struct> */
						case XML_BODY_STRUCT_IDX:
							if ( line->NumCloseKeys == 1 ) {

								switch ( xml_GetLineInformation( m, offset, line, &LastLineDetected) ) {

									/* </value> */
									case XML_BODY_VALUE_IDX:

										if ( line->NumCloseKeys != 1 ) 
											return -24;
										
										/* Success */	
										return YES; 	
									break;
									default:
										return -25;
									break;
								}
							}
						break;

						case -1:
						break;
						
						/* Could be End of Struct */
						default:
							return 1;
						break;
					}
				}
				
				
			break;

			default:
				return (-50 - status);
			break;
			
			
		}	
	}
	

	return -100;
}

/*********************************************************
* NAME		:	xml_ProcessValue(Helper)
*
* OBJECT	:	N/A
*
* FUNCTION	:	Parser Operation: Assigns Values from XML to Variable
*	 
* RETURN	:	DINT:	1 	= success
						<0 	= Error

				

*
* HISTORY	:	06.13.06 - Version 1
				02.28.08	- Check: NULL String, String to big
				06.23.10	- TIME
*
*********************************************************/
DINT xml_ProcessValue( STRING *pName, LineElement_typ *line, UINT idx )
{
DataElement_typ MemberDataElement;
DINT status;
UDINT datalen, dataadr, size;

	/* Get PV Information:	 */
	status	= PV_ninfo( pName, &MemberDataElement.Type, &MemberDataElement.Length, &MemberDataElement.Dim);

	/* Not Found */
	if ( status != 0 )
		return -101;

	size	= MemberDataElement.Length / MemberDataElement.Dim;

	/* Get PV Address */
	if ( PV_xgetadr( pName, &dataadr, &datalen ) != 0 ) 
		return -102;
	else if ( !dataadr )
		return -103;

	/* Adjust Destination Pointer */
	dataadr +=	(idx * size);
								
	/* Assign */
	switch ( MemberDataElement.Type ) {
				
		case TYPE_BOOL:
			*(BOOL *) dataadr	= (BOOL) line->bValue;
		break;
		case TYPE_USINT:
			*(USINT *) dataadr= (USINT) line->iValue;
		break;
		case TYPE_SINT:
			*(SINT *) dataadr	= (SINT) line->iValue;
		break;
		case TYPE_UINT:
			*(UINT *) dataadr	= (UINT) line->iValue;
		break;
		case TYPE_INT:
			*(INT *) dataadr	= (INT) line->iValue;
		break;
		case TYPE_UDINT:
			*(UDINT *) dataadr= (UDINT) line->iValue;
		break;
		case TYPE_DINT:
			*(DINT *) dataadr	= (DINT) line->iValue;
		break;
		case TYPE_REAL:
			*(REAL *) dataadr	= line->rValue;
		break;
		case TYPE_STRING:
			if ( strlen(line->sValue) <= STRING_LENGTH_MAX ) {
				strcpy( (USINT *) dataadr, line->sValue );
				/* Check: NULL String */
				if ( !strcmp( line->sValue, XML_STRING_NULL ) ) {
					strcpy( line->sValue, "" );
				}
			}
			else {
				strcpy( line->sValue, "String to big" );
				/*return -104;*/
			}
		break;

		case TYPE_DATE_TIME:
			*(UDINT *) dataadr= (UDINT) line->iValue;
		break;
		
		case TYPE_TIME:
			*(UDINT *) dataadr= (UDINT) line->iValue;
		break;
		default:
			return -105;
		break;
	}

	return YES;
}

/*********************************************************
* NAME		:	xml_AppendIndex(Helper)
*
* OBJECT	:	N/A
*
* FUNCTION	:	String Operation: Appends an array index to Name string
				name[1]
*	 
* RETURN	:	DINT: String length

				

*
* HISTORY	:	06.13.06 - Version 1
*
*********************************************************/
DINT xml_AppendIndex( STRING Dest[100], USINT num )
{
STRING s[3];

	strcat( Dest, "[" );
	itoa(num, (UDINT) s );	
	strcat( Dest, s );
	strcat( Dest, "]" );
	return strlen(Dest);
}


/*********************************************************
* NAME		:	xml_AddItems(Helper)
*
* OBJECT	:	N/A
*
* FUNCTION	:	Call Operation: Primary Recursive Function.
				Adds <value> fields to XML message based on 
				data type.
*	 
* RETURN	:	DINT: 	>0 Success
						<0 Failure

				

*
* HISTORY	:	06.13.06 - Version 1
				02.07.08	- Logger
				06.23.10	- Change to MemberDataElement
*
*********************************************************/
DINT xml_AddItems( USINT *msg, char* PVBaseName, UINT *Indent, UDINT	*offset, UDINT BufferSize, BOOL Logger )
{
DataElement_typ MemberDataElement[2];
STRING PVName[100];
UDINT datalen, dataadr;
UINT i=0, j, idx, size, indent, Dim;
DINT status;


	/* Check Buffer */
	if ( *offset > (BufferSize -XML_BUFFER_REMAIN) ) 
		return -99;
		
	/* Get Indent */
	indent	= *Indent;
		
	/* Determine Complexity */
	status	= PV_ninfo( PVBaseName, &MemberDataElement[0].Type, &MemberDataElement[0].Length, &Dim);
	
		
	if ( status < 0) {
		if ( Logger ) {
			ERRxwarning(0xC350, status, "Add Items: Status");
			ERRxwarning(0xC350, 1, PVBaseName);
		}
		return -status;
	}
	
	/* Complex: Single */
	if ( MemberDataElement[0].Type == 0 ) {

		/*	<value> 				*/
		/*		<struct>			*/
		xml_InsertKey( msg, XML_BODY_VALUE_IDX, 	offset, indent++, XML_OPEN);
		xml_InsertKey( msg, XML_BODY_STRUCT_IDX,	offset, indent++, XML_OPEN);
		
		/* Cycle through Items */
		for (i=0;i<Dim;i++) {
			
			/* Located PV */
			if ( !PV_item( PVBaseName, i, MemberDataElement[1].Name ) ) {
				strcpy( PVName, PVBaseName );
				strcat( PVName, "." );
				strcat( PVName, MemberDataElement[1].Name );

				if ( !PV_ninfo( PVName, &MemberDataElement[1].Type, &MemberDataElement[1].Length, &MemberDataElement[1].Dim) ) {

					/*			<member>								*/
					/*				<name>Name</name>				*/
					xml_InsertKey( msg, XML_BODY_MEMBER_IDX,	offset, indent++, XML_OPEN);	
					xml_InsertComplexLine( msg,XML_BODY_NAME_IDX, 0,	offset, (void *) MemberDataElement[1].Name, indent );

					/* Add Values */
					status	= xml_AddItems( msg, PVName, &indent, offset, BufferSize, Logger );
					
					/* Error */
					if ( status < 0 ) {
						return status;
					}
					
					/*			</member>	*/
					xml_InsertKey( msg, XML_BODY_MEMBER_IDX,	offset, indent--, XML_CLOSE);	

				}
				else {
					if ( Logger ) {
						ERRxwarning(0xC350, 1, "Add Items: Status");
						ERRxwarning(0xC350, 1, PVName);
					}
					return -1;
				}
							
			}
		}

		/**************  Close Structure **************/
		/*		</struct>		*/
		/*	</value>			*/
		xml_InsertKey( msg, XML_BODY_STRUCT_IDX,	offset, indent--, XML_CLOSE);	
		xml_InsertKey( msg, XML_BODY_VALUE_IDX, 	offset, indent--, XML_CLOSE);	
		
	}	

	/* Complex: Array */
	else if ( MemberDataElement[0].Type == 15 ) {

		/*	<value> 				*/
		/*		<array>			*/
		/*			<data>		*/
		xml_InsertKey( msg, XML_BODY_VALUE_IDX, 	offset, indent++, XML_OPEN);
		xml_InsertKey( msg, XML_BODY_ARRAY_IDX,	offset, indent++, XML_OPEN);
		xml_InsertKey( msg, XML_BODY_DATA_IDX,		offset, indent++, XML_OPEN);	

		for (j=0;j<Dim;j++) {

			/* Create PV Name */
			AddIndex( PVName, PVBaseName, j );
			
			/* Add Values */
			status	= xml_AddItems( msg, PVName, &indent, offset, BufferSize, Logger );
					
			/* Error */
			if ( status < 0 ) {
				return status;
			}
		}

		/*							</data> 		*/
		/*						</array>		*/
		/*					</value>			*/
		xml_InsertKey( msg, XML_BODY_DATA_IDX,		offset, indent--, XML_CLOSE);	
		xml_InsertKey( msg, XML_BODY_ARRAY_IDX,	offset, indent--, XML_CLOSE);
		xml_InsertKey( msg, XML_BODY_VALUE_IDX, 	offset, indent--, XML_CLOSE);
	}
	
	/* Simple: Base Name */
	else {
		
		/* Direct */
		strcpy( PVName, PVBaseName );

		/* Get PV Address */
		if ( PV_xgetadr(PVName, &dataadr, &datalen) != 0 ) {
			return XML_STATUS_CALL_ERROR;
		}

		/* Determine XML Data Type */
		idx 	= xml_DataTypeToValueType( MemberDataElement[0].Type );
		size	= datalen / Dim;
		
		/* Note: */
		switch (idx) {
			
			case XML_BODY_TYPE_I4_IDX:
			case XML_BODY_TYPE_DOUBLE_IDX:
			case XML_BODY_TYPE_BOOL_IDX:
			case XML_BODY_TYPE_STRING_IDX:
			case XML_BODY_TYPE_DATE_TIME_IDX:	

				/* Single Element */
				if ( Dim == 1 )  {
					/*				<value><i4>12</i4></value>	*/
					xml_InsertComplexLine( msg, idx,	size, offset, (void *) dataadr, indent );
				}

				/* Array */
				else {
					/*				<value> 					*/
					/*					<array> 				*/
					/*						<data>			*/
					xml_InsertKey( msg, XML_BODY_VALUE_IDX, 	offset, indent++, XML_OPEN);		
					xml_InsertKey( msg, XML_BODY_ARRAY_IDX, 	offset, indent++, XML_OPEN);		
					xml_InsertKey( msg, XML_BODY_DATA_IDX,		offset, indent++, XML_OPEN);		
					for (j=0;j<Dim;j++) {

						/*							<value><i4>12</i4></value>	*/
						xml_InsertComplexLine( msg, idx,	size, offset, (void *) dataadr, indent );
						dataadr += size;
					}
					/*							</data> 	*/
					/*						</array>		*/
					/*					</value>			*/
					xml_InsertKey( msg, XML_BODY_DATA_IDX,		offset, indent--, XML_CLOSE);
					xml_InsertKey( msg, XML_BODY_ARRAY_IDX, 	offset, indent--, XML_CLOSE);
					xml_InsertKey( msg, XML_BODY_VALUE_IDX, 	offset, indent--, XML_CLOSE);
				}
			
			break;
			
			/* Error */
			default:
				return XML_STATUS_CALL_ERROR;
			break;
		}
	}
	
	/* Set Indent */
	*Indent = indent;
	
	return i;
}

/*********************************************************
* NAME		:	xml_MessageViewer
*
* OBJECT	:	XML_Method_typ
*
* FUNCTION	:	
*
*
* PARAM 	:		
				
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				

*********************************************************/
BOOL xml_MessageViewer( XML_Method_typ *m, UINT line, USINT *text )
{
UDINT offset = 0;
UINT i, Levels;
BOOL LastLineDetected;

	/* Get Line */
	for (i=0;i<line;i++) {
		if ( _xmlGetLine( (USINT *) m->pMessage, &offset, text, &LastLineDetected, &Levels) < 0 ) {
			strcpy( text, "End of Message" );
			return OFF;
		}
	}

	/* Success */
	m->ViewLevel	= Levels;
	return ON;
}

/*********************************************************
* NAME		:	xml_FaultDetected
*
* OBJECT	:	
*
* FUNCTION	:	

				
					
*
*
* PARAM 	:		
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				

*********************************************************/
DINT xml_FaultDetected( XML_Method_typ *m,	DINT FaultCode, STRING FaultString[STRING_LENGTH_MAX], UDINT offset )
{

	/* Assign */
	m->ParseInfo.ErrorNum	= FaultCode;
	m->DiagInfo.TotalFaults++;
	
	/* Standard */
	m->Status					= XML_STATUS_SENDNOW;
	//xml_CreateFaultMessage( m, &offset, FaultCode, FaultString );
	ERRxwarning(0xC350, 0, FaultString);
	m->CreateInfo.LengthInBytes 	= offset + 1;
	
	/* Return */
	return -1;
}

/*********************************************************
* NAME		:	xml_FaultDetectedInResponse
*
* OBJECT	:	
*
* FUNCTION	:	

				
					
*
*
* PARAM 	:		
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				

*********************************************************/
DINT xml_FaultDetectedInResponse( XML_Method_typ *m,	DINT FaultCode, STRING FaultString[STRING_LENGTH_MAX] )
{

	/* Assign */
	m->ParseInfo.ErrorNum	= FaultCode;
	m->DiagInfo.TotalFaults++;
	m->Status		= XML_STATUS_RESP_ERROR;
	m->Fault			= YES;
	m->FaultCode	= FaultCode;
	strcpy( m->FaultString, FaultString );
	xml_SetErrorFlag( m );
		
	/* Return */
	return -1;
}


/*********************************************************
* NAME		:	xml_InsertKey(helper)
*
* OBJECT	:	
*
* FUNCTION	:	
*
*
* PARAM 	:		
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				

*********************************************************/
DINT xml_InsertKey( USINT *msg, UINT Type, UDINT  *offset, UINT Indent, BOOL Option )
{
UINT i;

	if ( Indent > 100 )
		Indent	= 0;

	/* Add Indention Level */
	for (i=0;i<Indent;i++) _AddStringToFile(	msg, offset, XML_BODY_INDENT_1, "" );

	/* Option: Open or Close */
	if (Option) 	_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
	else			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 		"" );
		
	/* */
	switch (Type ) {
		/*	</param> */
		case XML_BODY_PARAM_IDX:
			_AddStringToFile(	msg, offset, XML_BODY_PARAM,		"" );
		break;
		/*	</params> */
		case XML_BODY_PARAMS_IDX:
			_AddStringToFile(	msg, offset, XML_BODY_PARAMS,		"" );
		break;
		/*	</methodCall> */
		case XML_BODY_METHOD_CALL_IDX:
			_AddStringToFile(	msg, offset, XML_BODY_METHOD_CALL,	"" );
		break;
		/*	</value> */
		case XML_BODY_VALUE_IDX:
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,		"" );
		break;
		/*	</fault> */
		case XML_BODY_FAULT_IDX:
			_AddStringToFile(	msg, offset, XML_BODY_FAULT,			"" );
		break;
		/*	</struct> */
		case XML_BODY_STRUCT_IDX:
			_AddStringToFile(	msg, offset, XML_BODY_STRUCT,		"" );
		break;
		/*	</member> */
		case XML_BODY_MEMBER_IDX:
			_AddStringToFile(	msg, offset, XML_BODY_MEMBER,		"" );
		break;
		/*	</methodResponse> */
		case XML_BODY_METHOD_RESPONSE_IDX:
			_AddStringToFile(	msg, offset, XML_BODY_METHOD_RESPONSE,		"" );
		break;
		/*	</array> */
		case XML_BODY_ARRAY_IDX:
			_AddStringToFile(	msg, offset, XML_BODY_ARRAY,		"" );
		break;
		/*	</data> */
		case XML_BODY_DATA_IDX:
			_AddStringToFile(	msg, offset, XML_BODY_DATA, 			"" );
		break;

		default:
			ERRxwarning(0xC350, Type, "xml_InsertKey: Type");
			return -1;
		break;
	}

	/* Terminator */
	_AddStringToFile(	msg, offset, XML_BODY_END,	"\r\n" );

	/* Success */
	return 1;
}

/*********************************************************
* NAME		:	xml_InsertComplexLine(helper)
*
* OBJECT	:	
*
* FUNCTION	:	
*
*
* PARAM 	:		
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				09.18.07	- BOOL remove True/False

*********************************************************/
DINT xml_InsertComplexLine( USINT *msg, UINT Type, UINT size, UDINT  *offset, void *pData, UINT Indent)
{
UINT i;

	if ( Indent > 100 )
		Indent	= 0;

	/* Add Indention Level */
	for (i=0;i<Indent;i++) _AddStringToFile(	msg, offset, XML_BODY_INDENT_1, "" );

	/* */
	switch (Type ) {
		
		case XML_BODY_NAME_IDX:

			/*	<name> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 		"" );
			_AddStringToFile(	msg, offset, XML_BODY_NAME, 		"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"" );

			/* Value */
			_AddStringToFile(	msg, offset, (USINT *) pData,		"" );

			/* </name>LF */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_NAME, 		"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"\r\n" );
		break;

		case XML_BODY_METHOD_NAME_IDX:

			/*	<methodName> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 			"" );
			_AddStringToFile(	msg, offset, XML_BODY_METHOD_NAME,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"" );

			/* Value */
			_AddStringToFile(	msg, offset, (USINT *) pData,		"" );

			/* </methodName>LF */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_METHOD_NAME,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"\r\n" );
		break;
		
		case XML_BODY_TYPE_I4_IDX:

			/*	<value> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 		"" );
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"" );

			/* <i4> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 	"" );
			_AddStringToFile(	msg, offset, XML_BODY_TYPE_I4,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"" );

			/* Value */
			switch (size) {
				case 1:
					_AddIntToFile( 	msg, offset, *(SINT *) pData,		"" );
				break;
				case 2:
					_AddIntToFile( 	msg, offset, *(INT *) 	pData,		"" );
				break;
				case 4:
					_AddDintToFile( 	msg, offset, *(DINT *) pData,		"" );
				break;
			}
			

			/* </i4> */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_TYPE_I4,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"" );

			/* </value>LF */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"\r\n" );
		break;

		case XML_BODY_TYPE_BOOL_IDX:

			/*	<value> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 		"" );
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"" );

			/* <boolean> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 	"" );
			_AddStringToFile(	msg, offset, XML_BODY_TYPE_BOOL,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"" );

			/* Value */
			_AddIntToFile( 	msg, offset, (BOOL) *(DINT *) pData,	"" );
			
			/* </boolean> */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_TYPE_BOOL,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"" );

			/* </value>LF */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"\r\n" );
		break;
		
		case XML_BODY_TYPE_DOUBLE_IDX:

			/*	<value> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 		"" );
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,		"" );

			/* <double> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 		"" );
			_AddStringToFile(	msg, offset, XML_BODY_TYPE_DOUBLE,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"" );

			/* Value */
			_AddFloatToFile(		msg, offset, *(REAL *) pData,			"" );

			/* </double> */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_TYPE_DOUBLE,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"" );

			/* </value>LF */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,			"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"\r\n" );
	
		break;
		case XML_BODY_TYPE_STRING_IDX:

			/*	<value> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 			"" );
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,			"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"" );
			
			/* <string> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 		"" );
			_AddStringToFile(	msg, offset, XML_BODY_TYPE_STRING,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"" );

			/* Value */
			_AddStringToFile(	msg, offset, (USINT *) pData,			"" );

			/* </string> */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_TYPE_STRING,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"" );

			/* </value>LF */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,			"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"\r\n" );
		break;

		case XML_BODY_TYPE_DATE_TIME_IDX:

			/*	<value> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 		"" );
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"" );
			
			/* <date> */
			_AddStringToFile(	msg, offset, XML_BODY_OPEN, 		"" );
			_AddStringToFile(	msg, offset, XML_BODY_TYPE_DATE_TIME,    "" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"" );

			/* Value */
			//_AddStringToFile(	msg, offset, (USINT *) pData,		"" );
			_AddDintToFile( 	msg, offset, *(UDINT *) pData,		"" );

			/* </date> */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_TYPE_DATE_TIME,	"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"" );

			/* </value>LF */
			_AddStringToFile(	msg, offset, XML_BODY_CLOSE,		"" );
			_AddStringToFile(	msg, offset, XML_BODY_VALUE,			"" );
			_AddStringToFile(	msg, offset, XML_BODY_END,			"\r\n" );
		break;

	}
	

	/* OK */
	return 1;
}

/*********************************************************
* NAME		:	xml_CreateFaultMessage
*
* OBJECT	:	XML_Method_typ
*
* FUNCTION	:	 Server Function Only!
*
*
* PARAM 	:		
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				

*********************************************************/
DINT xml_CreateFaultMessage( XML_Method_typ *m,  UDINT	*offset, DINT FaultCode, STRING FaultString[STRING_LENGTH_MAX] )
{
	/* Assign */
	m->Fault			= YES;
	m->FaultCode	= FaultCode;
	strcpy( m->FaultString, FaultString );

	/* Clear */
	memset( (USINT *) m->pMessage, 	0x00, m->BufferSize );
	memset( &m->CreateInfo, 	0x00, sizeof(XML_CreateInfo_typ));
	*offset	= 0;

	/* Open Response: Version */
	_AddStringToFile( (USINT *) m->pMessage, offset, XML_BODY_VERSION,		"\r\n" ); 			/*	<?version: "1.0" ?> */
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_METHOD_RESPONSE_IDX,	offset,  0, XML_OPEN); /*	<methodResponse> */

	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_FAULT_IDX, 	offset, 1, XML_OPEN);			/*	<fault> 		*/
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_VALUE_IDX, 	offset, 2, XML_OPEN);			/*	<value> 		*/
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_STRUCT_IDX,	offset, 3, XML_OPEN);			/*	<struct>		*/

	/* Fault Code */
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_MEMBER_IDX,	offset, 4, XML_OPEN);			/*	<member>	*/
	xml_InsertComplexLine( (USINT *) m->pMessage,	XML_BODY_NAME_IDX,		0,	
								offset, XML_BODY_FAULT_CODE, 5 );						/*	<name>faultCode</name> */
	xml_InsertComplexLine( (USINT *) m->pMessage,	XML_BODY_TYPE_I4_IDX,	4,	
								offset, (void *) &m->FaultCode, 5 );						/*	<value><i4>11</i4></value> */
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_MEMBER_IDX,	offset, 5, XML_CLOSE);		/*	</member>	*/

	/* Fault String */
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_MEMBER_IDX,	offset, 4, XML_OPEN);			/*	<member>	*/
	xml_InsertComplexLine( (USINT *) m->pMessage,	XML_BODY_NAME_IDX,		0,	
								offset, XML_BODY_FAULT_STRING, 5 ); 					/*	<name>faultString</name> */
	xml_InsertComplexLine( (USINT *) m->pMessage,	XML_BODY_TYPE_STRING_IDX,	0,	
								offset, m->FaultString, 5 );								/*	<value><string>11</string></value> */
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_MEMBER_IDX,	offset, 5, XML_CLOSE);		/*	</member>	*/
	
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_STRUCT_IDX,	offset, 4, XML_CLOSE);		/*	</struct>		*/
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_VALUE_IDX, 	offset, 3, XML_CLOSE);		/*	</value>		*/
	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_FAULT_IDX, 	offset, 2, XML_CLOSE);		/*	</fault>		*/

	xml_InsertKey( (USINT *) m->pMessage,	XML_BODY_METHOD_RESPONSE_IDX,	offset,  1, XML_CLOSE); /*	</methodResponse> */

	return 1;
}
/*********************************************************
* NAME		:	xml_DataTypeToValueType(helper)
*
* OBJECT	:	N/A
*
* FUNCTION	:	Returns xml data type
*
*
* PARAM 	:		B&R Data type
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				06.23.10	- TIME
				

*********************************************************/
UINT xml_DataTypeToValueType(  UDINT br_datatype )
{
	switch ( br_datatype ) {
		case TYPE_BOOL:
			return XML_BODY_TYPE_BOOL_IDX;
		break;
		case TYPE_USINT:
		case TYPE_UINT:
		case TYPE_UDINT:
		case TYPE_SINT:
		case TYPE_INT:
		case TYPE_DINT:
		case TYPE_TIME:
			return XML_BODY_TYPE_I4_IDX;
		break;
		case TYPE_REAL:
			return XML_BODY_TYPE_DOUBLE_IDX;
		break;
		case TYPE_STRING:
			return XML_BODY_TYPE_STRING_IDX;
		break;

		case TYPE_DATE_TIME:
			return XML_BODY_TYPE_DATE_TIME_IDX;
		break;
		
		default:
			ERRxwarning(0xC350, br_datatype, "xml_DataTypeToValueType: Type");
			return -1;
		break;	
	}
}

/*********************************************************
* NAME		:	xml_GetKeyIndex
*
* OBJECT	:	XML_Method_typ
*
* FUNCTION	:	
*
*
* PARAM 	:		
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				09.18.07	- Handle I4 Variant <integer>
				

*********************************************************/
DINT xml_GetKeyIndex( USINT *pItemText, BOOL *SlashFound )
{
STRING s[STRING_LENGTH_MAX];

	/* Version */
	if ( *(pItemText+0) == cQUESTION ) {
		*SlashFound		= NO;
		return 	XML_VERSION;
	}
	
	/* End Keyword */
	else if ( *(pItemText+0) == cSLASH) {
		*SlashFound		= YES;
		strcpy( s, (pItemText+1));
	}
	else {
		*SlashFound		= NO;
		strcpy( s, pItemText );
	}
	
	/* I4 */
	if ( !strcmp( s, XML_BODY_TYPE_I4) )
		return XML_BODY_TYPE_I4_IDX;

	/* Integer */
	else if ( !strcmp( s, XML_BODY_TYPE_I4_V1) )
		return XML_BODY_TYPE_I4_IDX;

	/* BOOL */
	else if ( !strcmp( s, XML_BODY_TYPE_BOOL) )
		return XML_BODY_TYPE_BOOL_IDX;

	/* String */
	else if ( !strcmp( s, XML_BODY_TYPE_STRING) )
		return XML_BODY_TYPE_STRING_IDX;

	/* Double */
	else if ( !strcmp( s, XML_BODY_TYPE_DOUBLE) )
		return XML_BODY_TYPE_DOUBLE_IDX;

	/* Date & Time*/
	else if ( !strcmp( s, XML_BODY_TYPE_DATE_TIME) )
		return XML_BODY_TYPE_DATE_TIME_IDX;

	/* param */
	else if ( !strcmp( s, XML_BODY_PARAM) )
		return XML_BODY_PARAM_IDX;

	/* params */
	else if ( !strcmp( s, XML_BODY_PARAMS) )
		return XML_BODY_PARAMS_IDX;

	/* Method Call */
	else if ( !strcmp( s, XML_BODY_METHOD_CALL) )
		return XML_BODY_METHOD_CALL_IDX;

	/* Struct */
	else if ( !strcmp( s, XML_BODY_STRUCT) )
		return XML_BODY_STRUCT_IDX;
	
	/* Value */
	else if ( !strcmp( s, XML_BODY_VALUE) )
		return XML_BODY_VALUE_IDX;

	/* Method Response */
	else if ( !strcmp( s, XML_BODY_METHOD_RESPONSE) )
		return XML_BODY_METHOD_RESPONSE_IDX;

	/* Fault */
	else if ( !strcmp( s, XML_BODY_FAULT) )
		return XML_BODY_FAULT_IDX;
	
	/* Member */
	else if ( !strcmp( s, XML_BODY_MEMBER) )
		return XML_BODY_MEMBER_IDX;

	/* Method Name */
	else if ( !strcmp( s, XML_BODY_METHOD_NAME) )
		return XML_BODY_METHOD_NAME_IDX;

	/* Array */
	else if ( !strcmp( s, XML_BODY_ARRAY) )
		return XML_BODY_ARRAY_IDX;

	/* Data */
	else if ( !strcmp( s, XML_BODY_DATA) )
		return XML_BODY_DATA_IDX;

	/* name */
	else if ( !strcmp( s, XML_BODY_NAME) )
		return XML_BODY_NAME_IDX;

	/* None */
	else {
		ERRxwarning(0xC350, 0, "xml_GetKeyIndex: ");
		return -1;
	}

}
/*********************************************************
* NAME		:	xml_GetOpIndex
*
* OBJECT	:	n/a
*
* FUNCTION	:	
*
*
* PARAM 	:		
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				

*********************************************************/
DINT xml_GetOpIndex( USINT *pOpName )
{
STRING s[STRING_LENGTH_MAX];

	strcpy( s, pOpName );

	/* Read */
	if ( !strcmp( s, XML_OP_READ) )
		return XML_OP_READ_IDX;

	/* Read: Alternate Spelling */
	else if ( !strcmp( s, XML_OP_READ_2) )
		return XML_OP_READ_IDX;

	/* Read: Alternate Spelling */
	else if ( !strcmp( s, XML_OP_READ_3) )
		return XML_OP_READ_IDX;

	/* Write */
	else if ( !strcmp( s, XML_OP_WRITE) )
		return XML_OP_WRITE_IDX;

	/* Write: Alternate Spelling */
	else if ( !strcmp( s, XML_OP_WRITE_2) )
		return XML_OP_WRITE_IDX;

	/* Write: Alternate Spelling */
	else if ( !strcmp( s, XML_OP_WRITE_3) )
		return XML_OP_WRITE_IDX;

	/* Method */
	else if ( !strcmp( s, XML_OP_METHOD) )
		return XML_OP_METHOD_IDX;

	/* Method: Alternate Spelling */
	else if ( !strcmp( s, XML_OP_METHOD_2) )
		return XML_OP_METHOD_IDX;

	/* Method: Alternate Spelling */
	else if ( !strcmp( s, XML_OP_METHOD_3) )
		return XML_OP_METHOD_IDX;

	/* File Write */
	else if ( !strcmp( s, XML_OP_FILEWRITE) )
		return XML_OP_FILEWRITE_IDX;

	/* File Read */
	else if ( !strcmp( s, XML_OP_FILEREAD) )
		return XML_OP_FILEREAD_IDX;

	else
		return -1;
}
/*********************************************************
* NAME		:	xml_MakeCallName
*
* OBJECT	:	n/a
*
* FUNCTION	:	
*
*
* PARAM 	:		
	
*			
*
* HISTORY	:	06.10.07	- Version 1
				

*********************************************************/
DINT xml_MakeCallName(	USINT *pDest, USINT *pOpName, USINT *pMethodName )
{
	strcpy(	pDest, pOpName );
	strcat( 	pDest, "." );
	strcat( 	pDest, pMethodName );
	return 1;
}

void _AddIntToFile( USINT *f0, UDINT  *offset, INT x, USINT *endchar )
{
USINT *dest = f0 + *offset;
USINT  buffer[32];

	itoa(x, (UDINT) buffer);
	strcat(dest, buffer);
	strcat(  dest, endchar);
	*offset 	+= strlen(dest);
}
void _AddDintToFile( USINT *f0, UDINT  *offset, DINT x, USINT *endchar )
{
USINT *dest = f0 + *offset;
USINT  buffer[32];

	itoa(x, (UDINT) buffer);
	strcat(dest, buffer);
	strcat(  dest, endchar);
	*offset 	+= strlen(dest);
}

void _AddFloatToFile( USINT *f0, UDINT	*offset, REAL x, USINT *endchar )
{
USINT *dest = f0 + *offset;
USINT  buffer[32];

	ftoa(x, (UDINT) buffer);
	strcat(dest, buffer);
	strcat(  dest, endchar);
	*offset 	+= strlen(dest);
}
void _AddStringToFile( USINT *f0, UDINT  *offset, USINT *s, USINT *endchar )
{
USINT *dest = f0 + *offset;

	strcat(dest, s);
	strcat(dest, endchar);
	*offset 	+= strlen(dest);
}
/*********************************************************
* NAME: 			_xmlGetLine
*
*
* FUNCTION: 		
*		   
*
* PARAMETERS:	None
*
* RETURN:		
*
* HISTORY:		11.10.05		- 
				07.31.07	- Handle preceeding spaces and ignore <CR>
*				
*			  
*********************************************************/
DINT _xmlGetLine( USINT *f0, UDINT	*offset, USINT *dest, BOOL *lastline, UINT *levels )
{
USINT *src0 = f0 + *offset;
USINT *src	= src0;
USINT  buffer[LINE_LENGTH_MAX];
UINT i,j=0,Levels=0;
DINT Found = -1;
BOOL FirstCharFound = 0;

	/* Default */
	*lastline		= 0;	

	/* Search: for endchar */
	for (i=0;i<LINE_LENGTH_MAX;i++,src++) {
		
		/* Ignore Tab and space */
		if ( *src != 9 && *src != ' ') {
			
			/* LF */
			if ( *src == cLF ) {
				/* LF in 1st Char: Ignore */
				if ( j > 0 ) {
					Found = 1;
					buffer[j] = 0;
					break;
				}
			}

			/* CR */
			else if ( *src == cCR ) {
				
			}

			/* Null  */
			else if ( *src == 0 ) {
				Found	= 1;
				buffer[j]	= 0;
				*lastline	= 1;	
				break;
			}
			
			/* Normal Character */
			else {
				/* Not a Space */
				if ( (*src != ' ') || FirstCharFound ) {
					FirstCharFound	= YES;
					buffer[j]			= *src; 	
					j++;
				}
			}
		}
		else {
			Levels++;
		}
	}

	
	/* Update if Valid Dest Pointer */
	if ( dest > 0 ) 
		strcpy( dest, buffer );
		
	*offset 	+=	(i);

	/* Check EOF */
	*levels	= Levels;
	return Found;
	
}


/*********************************************************
* NAME: 			_xmlGetStringFromFile
*
*
* FUNCTION: 		
*		   
*
* PARAMETERS:	None
*
* RETURN:		
*
* HISTORY:		11.10.05		- 
*				
*			  
*********************************************************/
DINT _xmlGetStringFromFile( USINT *f0, UDINT  *offset, USINT *dest )
{
USINT *src0 = f0 + *offset;
USINT *src	= src0;
USINT  buffer[LINE_LENGTH_MAX];
UINT i,j=0;
DINT Found = -1;

	/* Search: for endchar */
	for (i=0;i<LINE_LENGTH_MAX;i++,src++) {
		
		/* Ignore Tab and space  */
		if ( *src != 9 &&  *src != ' ') {
			
			/* < */
			if ( *src == cLT ) {
				if ( j > 0 ) {
					Found	= 1;	
					buffer[j] 	= 0;
					break;
				}
			}
			
			/* > */
			else if ( *src == cGT) {
				if ( j > 0 ) {
					Found = 2;
					buffer[j] = 0;
					break;
				}
			}
			
			/* LF */
			else if ( *src == cLF ) {
				/* End Of Line: No string to report */
				if ( j == 0 ) {
					Found = 4;
					return Found;
				}
				else {
					Found = 3;
					buffer[j] = 0;
					break;
				}
			}

			else if ( *src == 0 ) {
				Found = 5;
				return Found;
			}
			
			/* Normal Character */
			else {
				buffer[j]	= *src; 	
				j++;
			}
		}
	}

	
	/* Update if Valid Dest Pointer */
	if ( dest > 0 ) {
		strcpy( dest, buffer );
	}	
	*offset 	+=	(i);

	/* Check EOF */
	return Found;
	
}

/*********************************************************
* NAME: 			_GetStringFromFile
*
*
* FUNCTION: 		
*		   
*
* PARAMETERS:	None
*
* RETURN:		
*
* HISTORY:		11.10.05		- 
*				
*			  
*********************************************************/
BOOL _GetStringFromFile( USINT *f0, UDINT  *offset, USINT *dest, USINT *endchar, BOOL ignore )
{
USINT *src0;
USINT *src;
USINT  buffer[LINE_LENGTH_MAX];
UINT i,j=0;

	/* Handle Null Offset */
	if ( !offset ) 		src0		= f0;
	else 			src0 	= f0 + *offset;

	/* Assign */
	src	= src0;
	
	/* Search: for endchar */
	for (i=0;i<LINE_LENGTH_MAX;i++,src++) {
		/* Ignore Tab */
		if ( *src != 9 ) { 
			buffer[j]	= *src; 	
			if ((*src == *endchar) || (*src == 0) ) {
				if ( j > 0 ) {
					buffer[j] = 0;
					break;
				}	
			}
			/* Always Insert Space on 1st char */
			if ( (j==0) && (ignore))
				buffer[0]	= 32;
			j++;
		}
	}
	/* Update if Valid Dest Pointer */
	if ( dest > 0 ) {
		if ( ignore )
			strcpy( dest, &buffer[1] ); 
		else
			strcpy( dest, buffer );
	}	

	/* Update Offset: Valid Pointer Only */
	if ( offset > 0) 	*offset 	+=	(j);
	
	/* Check EOF */
	if (*src == 0) 
		return 1;
	else
		return 0;
}
/** EOF **/
