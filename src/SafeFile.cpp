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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "SafeFile.h"
#endif

#include "SafeFile.h"		// Interface declarations.
#include "OtherFunctions.h"
#include "StringFunctions.h"		// Needed for unicode2char

#include "Packet.h"
#include "kademlia/utils/UInt128.h"

#define CHECK_BOM(size,x) ((size > 3)  && (x[0] == (char)0xEF) && (x[1] == (char)0xBB) && (x[2] == (char)0xBF))

const char BOMHeader[3] = {0xEF,0xBB,0xBF};

///////////////////////////////////////////////////////////////////////////////
// CFileDataIO

uint8 CFileDataIO::ReadUInt8() const
{
	uint8 nVal = 0;
	Read(&nVal, sizeof(nVal));

	return nVal;
}


uint16 CFileDataIO::ReadUInt16() const
{
	uint16 nVal = 0;
	Read(&nVal, sizeof(nVal));

	return ENDIAN_SWAP_16(nVal);
}


uint32 CFileDataIO::ReadUInt32() const
{
	uint32 nVal = 0;
	Read(&nVal, sizeof(nVal));

	return ENDIAN_SWAP_32(nVal);
}



void CFileDataIO::ReadUInt128(Kademlia::CUInt128 *pVal) const
{
	Read(pVal->getDataPtr(), 16);
}


void CFileDataIO::ReadHash16(uchar* pVal) const
{
	Read(pVal, 16);
}


wxString CFileDataIO::ReadString(bool bOptUTF8, uint8 SizeLen, bool SafeRead) const
{
	
	uint32 length;
	switch (SizeLen) {
		case 2:
			length = ReadUInt16();
			break;
		case 4:
			length = ReadUInt32();			
			break;
		default:
			// Not uint16 neither uint32. BAD THING.
			// Let's assume uint16 for not to crash.
			wxASSERT(0);
			printf("Unexpected string len size %d on ReadString! Report on forum.amule.org!\n", SizeLen);
			length = ReadUInt16();				
			break;					
	}	

	if (SafeRead) {
		if ( length > GetLength() - GetPosition() ) {
			length = GetLength() - GetPosition();
		}	
	}
	
	char* val = NULL;
	try {
		val = new char[length + 1];
		// We only need to set the the NULL terminator, since we know that
		// reads will either succeed or throw an exception, in which case
		// we wont be returning anything
		val[length] = 0;
		
		Read(val, length);
		wxString str;
		
		if (CHECK_BOM(length,val)) {
			// This is a UTF8 string with a BOM header, skip header.
			str = UTF82unicode(val+3);
		} else {
			if (bOptUTF8) {
				str = UTF82unicode(val);
				if (str.IsEmpty()) {
					// Fallback to system locale
					//printf("Failed UTF8 conversion (READ), going for current locale: %s\n",val);
					str = char2unicode(val);
				}					
			} else {
				str = char2unicode(val);
			}
		}
		delete[] val;

		return str;
	} catch ( ... ) {
		// Have to avoid mem-leaks
		delete[] val;
		
		// Re-throw
		throw;
	}
}


void CFileDataIO::WriteUInt8(uint8 nVal)
{
	Write(&nVal, sizeof nVal);
}

void CFileDataIO::WriteUInt16(uint16 nVal)
{
	ENDIAN_SWAP_I_16(nVal);
	
	Write(&nVal, sizeof nVal);
}

void CFileDataIO::WriteUInt32(uint32 nVal)
{
	ENDIAN_SWAP_I_32(nVal);

	Write(&nVal, sizeof nVal);
}


void CFileDataIO::WriteUInt128(const Kademlia::CUInt128 *pVal)
{
	Write(pVal->getData(), 16);
}


void CFileDataIO::WriteHash16(const uchar* pVal)
{
	Write(pVal, 16);
}


void CFileDataIO::WriteStringCore(const char *s, EUtf8Str eEncode, uint8 SizeLen)
{
	unsigned int sLength = s ? strlen(s) : 0;
	unsigned int real_length = 0;
	if (eEncode == utf8strOptBOM) {
		real_length = sLength + 3; // For BOM header.
	} else {
		real_length = sLength;
	}			
	switch (SizeLen) {
		case 2:
			WriteUInt16(real_length);
			break;
		case 4:
			WriteUInt32(real_length);
			break;
		default:
			// Not uint16 neither uint32. BAD THING.
			// Let's assume uint16 for not to crash.
			wxASSERT(0);
			printf(	"Unexpected string len size %d on WriteString!"
				" Report on forum.amule.org!\n", SizeLen);
			WriteUInt16(real_length);
			break;					
	}		
	if (sLength) {
		if (eEncode == utf8strOptBOM) {
			Write(BOMHeader,3);
		}
		Write(s, sLength);
	}
}

void CFileDataIO::WriteString(const wxString& rstr, EUtf8Str eEncode, uint8 SizeLen)
{
	// We dont include the NULL terminator. Dont know why.
	// It is because we write the size, so the NULL is not necessary.
	if ((eEncode == utf8strRaw) || (eEncode == utf8strOptBOM)) {
		Unicode2CharBuf s(unicode2UTF8(rstr));
		if (s) {
			WriteStringCore(s, eEncode, SizeLen);
		} else {
			// unicode2UTF-8 might fail in non-unicode builds.
			Unicode2CharBuf s1(unicode2char(rstr));
			WriteStringCore(s1, utf8strNone, SizeLen);
		}
	} else {
		Unicode2CharBuf s(unicode2char(rstr));
		WriteStringCore(s, eEncode, SizeLen);
	}

}


///////////////////////////////////////////////////////////////////////////////
// CSafeFile

off_t CSafeFile::Read(void *pBuf, off_t nCount) const
{
	if ( GetPosition() + nCount > GetLength() )
		// For lack of better
		throw CInvalidPacket("Read after end of CSafeFile");
		// AfxThrowFileException(CFileException::endOfFile, 0, GetFileName());
	
	return CFile::Read( pBuf, nCount );
}

size_t CSafeFile::Write(const void *pBuf, size_t nCount)
{
	return CFile::Write( pBuf, nCount );
}

///////////////////////////////////////////////////////////////////////////////
// CSafeMemFile

uint8 CSafeMemFile::ReadUInt8() const
{
	if ((off_t)(m_position + sizeof(uint8)) > m_BufferSize)
		throw CInvalidPacket("EOF");
	return CFileDataIO::ReadUInt8();
}

uint16 CSafeMemFile::ReadUInt16() const
{
	if ((off_t)(m_position + sizeof(uint16)) > m_BufferSize)
		throw CInvalidPacket("EOF");
	return CFileDataIO::ReadUInt16();
}

uint32 CSafeMemFile::ReadUInt32() const
{
	if ((off_t)(m_position + sizeof(uint32)) > m_BufferSize)
		throw CInvalidPacket("EOF");
	return CFileDataIO::ReadUInt32();
}

void CSafeMemFile::ReadUInt128(Kademlia::CUInt128* pVal) const
{
	if ((off_t)(m_position + sizeof(uint32)*4) > m_BufferSize)
		throw CInvalidPacket("EOF");
	CFileDataIO::ReadUInt128(pVal);
}


void CSafeMemFile::ReadHash16(unsigned char* pVal) const
{
	if ((off_t)(m_position + 16 /* Hash size*/) > m_BufferSize)
		throw CInvalidPacket("EOF");
	CFileDataIO::ReadHash16(pVal);
}

void CSafeMemFile::WriteUInt8(uint8 nVal)
{
	if (m_position + sizeof(uint8) > m_BufferSize)
		enlargeBuffer(m_position + sizeof(uint8));
	CFileDataIO::WriteUInt8(nVal);
}

void CSafeMemFile::WriteUInt16(uint16 nVal)
{
	if (m_position + sizeof(uint16) > m_BufferSize)
		enlargeBuffer(m_position + sizeof(uint16));
	CFileDataIO::WriteUInt16(nVal);
}

void CSafeMemFile::WriteUInt32(uint32 nVal)
{
	if (m_position + sizeof(uint32) > m_BufferSize)
		enlargeBuffer(m_position + sizeof(uint32));
	CFileDataIO::WriteUInt32(nVal);
}


void CSafeMemFile::WriteUInt128(const Kademlia::CUInt128* pVal)
{
	if (m_position + sizeof(uint32)*4 > m_BufferSize)
		enlargeBuffer(m_position + sizeof(uint32)*4);
	CFileDataIO::WriteUInt128(pVal);
}


void CSafeMemFile::WriteHash16(const uchar* pVal)
{
	if (m_position + sizeof(uint32)*4 /* 16 bytes */> m_BufferSize)
		enlargeBuffer(m_position + sizeof(uint32)*4);
	CFileDataIO::WriteHash16(pVal);
}

///////////////////////////////////////////////////////////////////////////////
// CSafeBufferedFile

off_t CSafeBufferedFile::Read(void *pBuf, off_t nCount) const
{
	if ( GetPosition() + nCount > GetLength() )
		throw CInvalidPacket("Reading past end of CSafeBufferedFile!");
//		AfxThrowFileException(CFileException::endOfFile, 0, GetFileName());

	return CFile::Read( pBuf, nCount );
}

size_t CSafeBufferedFile::Write(const void *pBuf, size_t nCount)
{
	return CFile::Write( pBuf, nCount );
}
