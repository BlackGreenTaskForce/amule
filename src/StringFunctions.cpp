//
// This file is part of the aMule Project.
//
// Copyright (c) 2004 Angel Vidal Veiga - Kry (kry@amule.org)
// Copyright (c) 2003 aMule Team ( http://www.amule-project.net )
//
// Any parts of this program derived from the xMule, lMule or eMule project,
// or contributed by third-party developers are copyrighted by their
// respective authors.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA, 02111-1307, USA
//

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "StringFunctions.h"
#endif

#include <ctype.h>

#include "StringFunctions.h"
#include <wx/filename.h>


// Implementation of the non-inlines
static byte base16Chars[17] = "0123456789ABCDEF";

wxString URLEncode(wxString sIn)
{
	wxString sOut;
	unsigned char curChar;
	
	for ( unsigned int i = 0; i < sIn.Length(); ++i ) {
		curChar = sIn.GetChar( i );

		if ( isalnum( curChar ) ) {
	        sOut += curChar;
	    } else if( isspace ( curChar ) ) {
		    sOut += wxT("+");
		} else {
			sOut += wxT("%");
			sOut += base16Chars[ curChar >> 4];
			sOut += base16Chars[ curChar & 0xf];
		}

	}

	return sOut;
}

wxString TruncateFilename(const wxString& filename, size_t length, bool isFilePath)
{
	// Check if there's anything to do
	if ( filename.Length() <= length )
		return filename;
	
	wxString file = filename;
	
	// If the filename is a path, then prefer to remove from the path, rather than the filename
	if ( isFilePath ) {
		wxString path = file.BeforeLast( wxFileName::GetPathSeparator() );
		file          = file.AfterLast( wxFileName::GetPathSeparator() );

		if ( path.Length() >= length ) {
			path.Clear();
		} else if ( file.Length() >= length ) {
			path.Clear();
		} else {
			// Minus 6 for "[...]" + seperator
			int pathlen = length - file.Length() - 6;
			
			if ( pathlen > 0 ) {
				path = wxT("[...]") + path.Right( pathlen );
			} else {
				path.Clear();
			}
		}
		
		if ( !path.IsEmpty() ) {
			file = path + wxFileName::GetPathSeparator() + file;
		}
	}

	if ( file.Length() > length ) {
		if ( length > 5 ) {		
			file = file.Left( length - 5 ) + wxT("[...]");
		} else {
			file.Clear();
		}
	}
	

	return file;
}

// Strips specific chars to ensure legal filenames
wxString CleanupFilename(const wxString& filename, bool keepSpaces)
{
	wxString result;

	for ( unsigned int i = 0; i < filename.Length(); i++ ) {
		switch ( filename[ i ] ) {
			case wxT('/'):
#ifdef __WXMSW__
			case wxT('\"'):
			case wxT('*'):
			case wxT('<'):
			case wxT('>'):
			case wxT('?'):
			case wxT('|'):
			case wxT('\\'):
			case wxT(':'):
#endif
				continue;
			case wxT(' '):
				if ( !keepSpaces ) {
					result += wxT('_');
					continue;
				}
				
			default:
				// Many illegal for filenames in windows below the 32th char (which is space).
				if ( (wxUChar) filename[i] > 31 ) {
						result += filename[i];
				}
		}
	}

	return result;
}

