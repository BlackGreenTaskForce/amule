// This file is part of the aMule Project
//
// Copyright (c) 2003-2004 aMule Project ( http://www.amule-project.net )
// Copyright (C) 2002 Merkur ( merkur-@users.sourceforge.net / http://www.emule-project.net )
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#include <zlib.h>		// Needed for uLongf

#include "packets.h"		// Interface declarations
#include "StringFunctions.h"	// Needed for nstrdup
#include "CMemFile.h"		// Needed for CMemFile
#include "otherstructs.h"	// Needed for Header_Struct
#include "types.h"		// Needed for wxFileSize_t

// Copy constructor
Packet::Packet(Packet &p)
{
	size 		= p.size;
	opcode		= p.opcode;
	prot 		= p.prot;
	m_bSplitted 	= p.m_bSplitted;
	m_bLastSplitted = p.m_bLastSplitted;
	m_bPacked 	= p.m_bPacked;
	m_bFromPF 	= p.m_bFromPF;
	memcpy(head, p.head, sizeof head);
	tempbuffer	= NULL;
	if (p.completebuffer) {
		completebuffer 	= new char[size + 10];;
		pBuffer 	= completebuffer + 6;
	} else {
		completebuffer 	= NULL;
		if (p.pBuffer) {
			pBuffer = new char[size + 1];
		} else {
			pBuffer = NULL;
		}
	}
	if (pBuffer)
		memcpy( pBuffer, p.pBuffer, size + 1);
}

Packet::Packet(uint8 protocol)
{
	size 		= 0;
	opcode		= 0;
	prot 		= protocol;
	m_bSplitted 	= false;
	m_bLastSplitted = false;
	m_bPacked 	= false;
	m_bFromPF 	= false;
	memset(head, 0, sizeof head);
	tempbuffer	= NULL;
	completebuffer 	= NULL;
	pBuffer 	= NULL;
}

// only used for receiving packets
Packet::Packet(char* header)
{
	memset(head, 0, sizeof head);
	Header_Struct* head = (Header_Struct*) header;
	size 		= ENDIAN_SWAP_32(head->packetlength) - 1;
	opcode		= head->command;
	prot		= head->eDonkeyID;
	m_bSplitted 	= false;
	m_bLastSplitted = false;
	m_bPacked 	= false;
	m_bFromPF 	= false;
	tempbuffer	= NULL;
	completebuffer 	= NULL;
	pBuffer 	= NULL;
}

Packet::Packet(CMemFile* datafile, uint8 protocol, uint8 ucOpcode)
{
	size		= datafile->GetLength();
	opcode		= ucOpcode;
	prot		= protocol;
	m_bSplitted 	= false;
	m_bLastSplitted = false;
	m_bPacked 	= false;
	m_bFromPF 	= false;
	memset(head, 0, sizeof head);
	tempbuffer = NULL;
	completebuffer = new char[size + 10];
	pBuffer = completebuffer + 6;

	BYTE* tmp = datafile->Detach();
	memcpy(pBuffer, tmp, size);
	free(tmp); 
}

Packet::Packet(int8 in_opcode, uint32 in_size, uint8 protocol, bool bFromPF)
{
	size		= in_size;
	opcode		= in_opcode;
	prot		= protocol;
	m_bSplitted 	= false;
	m_bLastSplitted = false;
	m_bPacked 	= false;
	m_bFromPF	= bFromPF;
	memset(head, 0, sizeof head);
	tempbuffer	= NULL;
	if (in_size) {
		completebuffer = new char[in_size + 10];
		pBuffer = completebuffer + 6;
		memset(completebuffer, 0, in_size + 10);
	} else {
		completebuffer = NULL;
		pBuffer = NULL;
	}
}

// only used for splitted packets!
Packet::Packet(char* pPacketPart, uint32 nSize, bool bLast, bool bFromPF)
{
	size		= nSize - 6;
	opcode		= 0;
	prot		= 0;
	m_bSplitted	= true;
	m_bLastSplitted	= bLast;
	m_bPacked	= false;
	m_bFromPF	= bFromPF;
	memset(head, 0, sizeof head);
	tempbuffer	= NULL;
	completebuffer	= pPacketPart;
	pBuffer		= NULL;
}

Packet::~Packet()
{
	// Never deletes pBuffer when completebuffer is not NULL
	if (completebuffer) {
		delete [] completebuffer;
	} else if (pBuffer) {
	// On the other hand, if completebuffer is NULL and pBuffer is not NULL 
		delete [] pBuffer;
	}
	
	if (tempbuffer) {
		delete [] tempbuffer;
	}
}

void Packet::AllocDataBuffer(void)
{
	wxASSERT(completebuffer == NULL);
	pBuffer = new char[size + 1];
}

void Packet::CopyToDataBuffer(unsigned int offset, const char *data, unsigned int n)
{
	wxASSERT(offset + n <= size + 1);
	memcpy(pBuffer + offset, data, n);
}

char* Packet::GetPacket() {
	if (completebuffer) {
		if (!m_bSplitted) {
			memcpy(completebuffer, GetHeader(), 6);
		}
		return completebuffer;
	} else {
		if (tempbuffer){
			delete [] tempbuffer;
			#warning Phoenix - why??? Check this info and clean!
			tempbuffer = NULL; // 'new' may throw an exception
		}
		tempbuffer = new char[size+10];
		memcpy(tempbuffer    , GetHeader(), 6   );
		memcpy(tempbuffer + 6, pBuffer    , size);
		return tempbuffer;
	}
}

char* Packet::DetachPacket() {
	if (completebuffer) {
		if (!m_bSplitted) {
			memcpy(completebuffer, GetHeader(), 6);
		}
		char* result = completebuffer;
		completebuffer = pBuffer = NULL;
		return result;
	} else{
		if (tempbuffer){
			delete[] tempbuffer;
			tempbuffer = NULL; // 'new' may throw an exception
		}
		tempbuffer = new char[size+10];
		memcpy(tempbuffer,GetHeader(),6);
		memcpy(tempbuffer+6,pBuffer,size);
		char* result = tempbuffer;
		tempbuffer = 0;
		return result;
	}
}

char* Packet::GetHeader() {
	wxASSERT( !m_bSplitted );

	Header_Struct* header = (Header_Struct*) head;
	header->command = opcode;
	header->eDonkeyID =  prot;
	header->packetlength = ENDIAN_SWAP_32(size + 1);

	return head;
}

char* Packet::GetUDPHeader() {
	wxASSERT( !m_bSplitted );

	memset(head, 0, 6);
	UDP_Header_Struct* header = (UDP_Header_Struct*) head;
	header->command = opcode;
	header->eDonkeyID =  prot;

	return head;
}

void Packet::PackPacket() {
	wxASSERT(!m_bSplitted);

	uLongf newsize = size + 300;
	BYTE* output = new BYTE[newsize];

	uint16 result = compress2(output, &newsize, (BYTE*) pBuffer, size, Z_BEST_COMPRESSION);

	if (result != Z_OK || size <= newsize) {
		delete[] output;
		return;
	}

	prot = OP_PACKEDPROT;
	memcpy(pBuffer, output, newsize);
	delete[] output;
	m_bPacked = true;
	
	size = newsize;
}

bool Packet::UnPackPacket(UINT uMaxDecompressedSize) {
	wxASSERT( prot == OP_PACKEDPROT );

	uint32 nNewSize = size * 10 + 300;

	if (nNewSize > uMaxDecompressedSize){
		//ASSERT(0);
		nNewSize = uMaxDecompressedSize;
	}

	BYTE* unpack = new BYTE[nNewSize];
	uLongf unpackedsize = nNewSize;
	uint16 result = uncompress(unpack, &unpackedsize, (BYTE *) pBuffer, size);

	if (result == Z_OK) {
		wxASSERT( completebuffer == NULL );
		wxASSERT( pBuffer != NULL );

		size = unpackedsize;
		delete[] pBuffer;
		pBuffer = (char *) unpack;
		prot = OP_EMULEPROT;
		return true;
	}

	delete[] unpack;
	return false;
}


///////////////////////////////////////////////////////////////////////////////
// STag

STag::STag()
{
	type = 0;
	tagname = NULL;
	stringvalue = NULL;
	intvalue = 0;
	specialtag = 0;
}

STag::STag(const STag& in)
{
	type = in.type;
	tagname = in.tagname!=NULL ? nstrdup(in.tagname) : NULL;
	if (in.type == 2)
		stringvalue = in.stringvalue!=NULL ? nstrdup(in.stringvalue) : NULL;
	else if (in.type == 3)
		intvalue = in.intvalue;
	else if (in.type == 4)
		floatvalue = in.floatvalue;
	else{
		//wxASSERT(0);
		intvalue = 0;
	}

	specialtag = in.specialtag;
}

STag::~STag()
{
	if (tagname) {
		delete[] tagname;
	}
	if (type == 2 && stringvalue) {
		delete[] stringvalue;
	}
}


///////////////////////////////////////////////////////////////////////////////
// CTag

CTag::CTag(LPCSTR name,uint32 intvalue){
	tag.tagname = nstrdup(name);
	tag.type = 3;
	tag.intvalue = intvalue;
}

CTag::CTag(int8 special,uint32 intvalue){
	tag.type = 3;
	tag.intvalue = intvalue;
	tag.specialtag = special;
}

CTag::CTag(LPCSTR name,LPCSTR strvalue){
	tag.tagname = nstrdup(name);
	tag.type = 2;
	tag.stringvalue = nstrdup(strvalue);
}

CTag::CTag(int8 special, LPCSTR strvalue){
	tag.type = 2;
	tag.stringvalue = nstrdup(strvalue);
	tag.specialtag = special;
}

CTag::CTag(LPCSTR name, const wxString& strvalue){
	tag.tagname = nstrdup(name);
	tag.type = 2;
	tag.stringvalue = nstrdup(unicode2char(strvalue));
}

CTag::CTag(uint8 special, const wxString& strvalue){
	tag.type = 2;
	tag.stringvalue = nstrdup(unicode2char(strvalue));
	tag.specialtag = special;
}

CTag::CTag(const STag& in_tag)
	: tag(in_tag)
{
}

#if defined( UNDEFINED )
CTag::CTag(CFile *file)
{
	// Use the constructor below
	CTag(*file);
}
#endif // UNDEFINED

CTag::CTag(const CFile &in_data)
{
	off_t off;
	if ((off = in_data.Read(&tag.type,1)) == wxInvalidOffset) {
		throw CInvalidPacket("Bad Met File");
	}
	
	uint16 length;
	
	if (tag.type & 0x80)
	{
		tag.type &= 0x7F;
		
		#warning we need to add the new tag types before 2.0.0
		if ((off = in_data.Read(&tag.specialtag,1)) == wxInvalidOffset) {	
			throw CInvalidPacket("Bad Met File");
		}
		tag.tagname = NULL;
	} else {
	
		if ((off = in_data.Read(&length,2)) == wxInvalidOffset) {	
			throw CInvalidPacket("Bad Met File");
		}

		ENDIAN_SWAP_I_16(length);
		if (length == 1) {
			if ((off = in_data.Read(&tag.specialtag,1)) == wxInvalidOffset) {
				throw CInvalidPacket("Bad Met File");
			}
		} else {
			tag.tagname = new char[length+1];
			if ((off = in_data.Read(tag.tagname,length)) == wxInvalidOffset) {
				throw CInvalidPacket("Bad Met File");
			}
			tag.tagname[length] = 0;
		}
	}

	// NOTE: It's very important that we read the *entire* packet data, even if we do
	// not use each tag. Otherwise we will get troubles when the packets are returned in 
	// a list - like the search results from a server.
	
	if (tag.type == 2){ // STRING
		if ((off = in_data.Read(&length,2)) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}
		ENDIAN_SWAP_I_16(length);
		tag.stringvalue = new char[length+1];
		if (in_data.Read(tag.stringvalue,length) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}		
		tag.stringvalue[length] = '\0';
	}
	else if (tag.type == 3){ // DWORD
		if (in_data.Read(&tag.intvalue,4) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}				
		ENDIAN_SWAP_I_32(tag.intvalue);
	}
	else if (tag.type == 4){ // FLOAT (used by Hybrid 0.48)
		// What to do with them?
		if (in_data.Read(&tag.floatvalue,4) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}				
	}
	else if (tag.type == 1){ // HASH (never seen)
		printf("CTag::CTag(CFile*); Reading *unverified* HASH tag\n");
		if (in_data.Seek(16, CFile::current) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}								
	}
	else if (tag.type == 5){ // BOOL (never seen; propably 1 byte)
		// NOTE: This is preventive code, it was never tested
		printf("CTag::CTag(CFile*); Reading *unverified* BOOL tag\n");
		if (in_data.Seek(1, CFile::current) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}								
	}
	else if (tag.type == 6){ // BOOL Array (never seen; propably <numbytes> <bytes>)
		// NOTE: This is preventive code, it was never tested
		printf("CTag::CTag(CFile*); Reading *unverified* BOOL Array tag\n");
		uint16 len;
		if (in_data.Read(&len,2) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}										
		ENDIAN_SWAP_I_16(len);
		if (in_data.Seek((len+7)/8, CFile::current) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}								
	}
	else if (tag.type == 7){ // BLOB (never seen; propably <len> <byte>)
		// NOTE: This is preventive code, it was never tested
		printf("CTag::CTag(CFile*); Reading *unverified* BLOB tag\n");
		uint16 len;
		if (in_data.Read(&len,2) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}										
		ENDIAN_SWAP_I_16(len);
		if (in_data.Seek(len, CFile::current) == wxInvalidOffset) {		
			throw CInvalidPacket("Bad Met File");
		}								
	}
	else if (tag.type == TAGTYPE_UINT16){ 
		uint16 value;
		if (in_data.Read(&value,2) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}				
		tag.intvalue = ENDIAN_SWAP_16(value);
	}
	else if (tag.type == TAGTYPE_UINT8){ 
		uint8 value;
		if (in_data.Read(&value,1) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}				
		tag.intvalue = value;
	}	
	else if (tag.type >= TAGTYPE_STR1 && tag.type <= TAGTYPE_STR16) {
		length = tag.type - TAGTYPE_STR1 + 1;
		tag.stringvalue = new char[length+1];
		if (in_data.Read(tag.stringvalue,length) == wxInvalidOffset) {
			throw CInvalidPacket("Bad Met File");
		}		
		tag.stringvalue[length] = '\0';		
		tag.type = 2;
	}
	else{
		if (tag.type==0x00) throw(CInvalidPacket("Bad met file"));
		if (length == 1)
			printf("CTag::CTag(CFile*); Unknown tag: type=0x%02X  specialtag=%u\n", tag.type, tag.specialtag);
		else
			printf("CTag::CTag(CFile*); Unknown tag: type=0x%02X  name=\"%s\"\n", tag.type, tag.tagname);
	}
}

CTag::~CTag()
{
}

bool CTag::WriteTagToFile(CFile* file)
{
	// don't write tags of unknown types, we wouldn't be able to read them in again 
	// and the met file would be corrupted
	if (tag.type==2 || tag.type==3 || tag.type==4){
		file->Write(&tag.type,1);
		
		if (tag.tagname){
			uint16 taglen= (uint16)strlen(tag.tagname);
			ENDIAN_SWAP_I_16(taglen);
			file->Write(&taglen,2);
                        ENDIAN_SWAP_I_16(taglen);
			file->Write(tag.tagname,taglen);
		}
		else{
			uint16 taglen = 1;
			ENDIAN_SWAP_I_16(taglen);
			file->Write(&taglen,2);
                        ENDIAN_SWAP_I_16(taglen);
			file->Write(&tag.specialtag,taglen);
		}

		if (tag.type == 2){
			uint16 len = (uint16)strlen(tag.stringvalue);
			ENDIAN_SWAP_I_16(len);
			file->Write(&len,2);
			ENDIAN_SWAP_I_16(len);
			file->Write(tag.stringvalue,len);
		}
		else if (tag.type == 3){
			uint32 intvalue_endian = ENDIAN_SWAP_32(tag.intvalue);
			file->Write(&intvalue_endian,4);
		}
		else if (tag.type == 4){
			// What to to with them on ppc?
			file->Write(&tag.floatvalue,4);
		}
		//TODO: Support more tag types
		else{
			printf("CTag::WriteTagToFile(CFile*); Unknown tag: type=0x%02X\n", tag.type);
			//wxASSERT(0);
			return false;
		}
		return true;
	}
	else{
		printf("CTag::WriteTagToFile(CFile*); Ignored tag with unknown type=0x%02X\n", tag.type);
		wxASSERT(0);
		return false;
	}
}

wxString CTag::GetFullInfo() const
{
	wxString strTag;
	if (tag.tagname){
		strTag = _T('\"');
		strTag += (char2unicode(tag.tagname));
		strTag += _T('\"');
	}
	else{
		strTag.Printf(_T("0x%02X"), tag.specialtag);
	}
	strTag += _T("=");
	if (tag.type == 2){
		strTag += _T("\"");
		strTag += (char2unicode(tag.stringvalue));
		strTag += _T("\"");
	}
	else if (tag.type == 3){
		TCHAR szBuff[16];
		snprintf(szBuff, 10, "%i",tag.intvalue);
		strTag += (char2unicode(szBuff));
	}
	else if (tag.type == 4){
		TCHAR szBuff[16];
		snprintf(szBuff, ELEMENT_COUNT(szBuff), "%f", tag.floatvalue);
		strTag += (char2unicode(szBuff));
	}
	else{
		wxString strBuff;
		strBuff.Printf(_T("Type=%u"), tag.type);
		strTag += strBuff;
	}
	return strTag;
}

CInvalidPacket::CInvalidPacket(const char* reason)
{
	// that was _bad_ practice unless length is guaranteed - Unleashed
	// strcpy(m_acWhat, "(NULL)");

	strncpy(m_acWhat, reason ? reason : "(NULL)", sizeof(m_acWhat));
	m_acWhat[sizeof(m_acWhat) - 1] = '\0';
}

const char* CInvalidPacket::what() const throw()
{
	return m_acWhat;
}
