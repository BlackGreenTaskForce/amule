// This file is part of the aMule Project.
//
// Copyright (c) 2003-2004 aMule Project ( http://www.amule-project.net )
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#ifndef CMEMFILE_H
#define CMEMFILE_H

// something that looks like the MFC CMemFile

#include "CFile.h"		// Needed for CFile
#include "types.h"		// Needed for uint8, uint16, uint32
#include "packets.h"
#include "otherfunctions.h"

class CMemFile : public CFile {
public:  
	CMemFile(unsigned int growBytes = 1024);
	CMemFile(BYTE* buffer,unsigned int bufferSize,unsigned int growBytes=0);
	void Attach(BYTE* buffer,unsigned int buffserSize,unsigned int growBytes=0);
	BYTE* Detach();
	virtual ~CMemFile();
	virtual unsigned long GetPosition() const 		{ return fPosition; };
	virtual bool GetStatus(unsigned long none) const 	{ return 1; };
	off_t Seek(off_t offset, wxSeekMode from = wxFromStart);
	virtual void SetLength(unsigned long newLen);
	virtual off_t Length() const { return fFileSize; };
//	virtual void Abort();
//	virtual void Flush();
	virtual bool Close() const;
	
	// Kry - DANGEROUS! JUST FOR USE ON DEBUG BUILD TO DUMP MEMFILES!
	BYTE* GetCurrentBuffer() const 		{ return (fBuffer + fPosition); };

	off_t Read(uint8& v) const		{ return ReadRaw(&v, 1); }
  	size_t Write(const uint8& v)		{ return WriteRaw(&v, 1); }
	
	#if wxBYTE_ORDER == wxLITTLE_ENDIAN
 		
  		off_t Read(uint16& v) const	{ return ReadRaw(&v, 2); }
 		off_t Read(uint32& v) const	{ return ReadRaw(&v, 4);	}
  		//off_t Read(uint8[16] v) const	{ return ReadRaw(v, 16);	}
	
  		size_t Write(const uint16& v)	{ return WriteRaw(&v, 2);	}
		size_t Write(const uint32& v) { return WriteRaw(&v, 4);	}
  		//size_t Write(const uint8[16] v) { return WriteRaw(v, 16);	}
	
	#else 
	
  		virtual off_t Read(uint16& v) const;
 		virtual off_t Read(uint32& v) const;
	
  		virtual size_t Write(const uint16& v);
		virtual size_t Write(const uint32& v);
		
	#endif

	virtual off_t Read(uint8[16]) const;
		
	virtual size_t Write(const uint8[16]);	
		
	size_t Write(const wxString& v) {
		size_t Len = Write((uint16)v.Length());
		return Len + WriteRaw(unicode2char(v), v.Length());
	}
	

	off_t Read(wxString& v) const {
		uint16 len = 0;
		off_t off = Read(len);
		if ( off == sizeof(len) ) {
			char buffer[len+1];
			off += ReadRaw(buffer, len);

			if ( sizeof(len) + len == off ) {
				buffer[len] = 0; // We want to be sure that the string is terminated
				v = char2unicode(buffer);
	//			v.UngetWriteBuf(len);
			}
		}
			
		if (off != (len + sizeof(len))) {
			throw CInvalidPacket("short packet reading string");
		}
		return off;
	}	

	virtual off_t  ReadRaw(void* buf, off_t length) const;
	virtual size_t WriteRaw(const void* buf, size_t length);
	
	off_t ReadHash16(uchar* hash_to_read) const {
		return ReadRaw(hash_to_read,16);
	}

	size_t WriteHash16(const uchar* hash_to_write) {
		return WriteRaw(hash_to_write,16);
	}	

protected:
	virtual off_t  Read(void* buf, off_t length) const;
	virtual size_t Write(const void* buf, size_t length);
	
private:
	void enlargeBuffer(unsigned long size);
	
	unsigned int fGrowBytes;
	mutable unsigned long fPosition;
	unsigned long fBufferSize;
	unsigned long fFileSize;
	int deleteBuffer;
	BYTE* fBuffer;  
};

#endif // CMEMFILE_H
