//
// This file is part of the aMule Project
//
// Copyright (c) 2003-2004 aMule Project ( http://www.amule-project.net )
// Copyright (C) 2002 Merkur ( merkur-@users.sourceforge.net / http://www.emule-project.net )
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

#ifndef SHAREDFILELIST_H
#define SHAREDFILELIST_H

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "SharedFileList.h"
#endif

#include <map>
#include <wx/defs.h>		// Needed before any other wx/*.h
#include <wx/thread.h>		// Needed for wxMutex

#include "Types.h"		// Needed for uint16 and uint64
#include "CTypedPtrList.h"	// Needed for CTypedPtrList

struct UnknownFile_Struct;

class CKnownFileList;
class CKnownFile;
class CSafeMemFile;
class CMD4Hash;
class CServer;
class CUpDownClient;

typedef std::map<CMD4Hash,CKnownFile*> CKnownFileMap;

class CSharedFileList{
	friend class CSharedFilesCtrl;
	friend class CClientReqSocket;
public:
	CSharedFileList(CKnownFileList* in_filelist);
	~CSharedFileList();
	
	void 	Reload(bool firstload = false);
	void	SafeAddKFile(CKnownFile* toadd, bool bOnlyAdd = false);
	void	RemoveFile(CKnownFile* toremove);
	wxMutex	list_mut;
	CKnownFile*	GetFileByID(const CMD4Hash& filehash);
	short	GetFilePriorityByID(const CMD4Hash& filehash);
	const CKnownFile* GetFileByIndex(unsigned int index) const;
	CKnownFileList*	filelist;
	void	CreateOfferedFilePacket(CKnownFile* cur_file,CSafeMemFile* files, CServer* pServer, CUpDownClient* pClient);
	uint64	GetDatasize();
	uint16	GetCount()	{return m_Files_map.size(); }
	void	UpdateItem(CKnownFile* toupdate);
	void	AddFilesFromDirectory(wxString directory);
	void    GetSharedFilesByDirectory(const wxString directory,CTypedPtrList<CPtrList, CKnownFile*>& list);
	void	ClearED2KPublishInfo();
	void	RepublishFile(CKnownFile* pFile);
	void	Process();

	// Not needed yet. Can be included once we add the converter
	void	PublishNextTurn()	{ m_lastPublishED2KFlag=true;	}
	
private:
	void	FindSharedFiles();
	bool	reloading;
	
	void	SendListToServer();
	uint32 m_lastPublishED2K;
	bool	 m_lastPublishED2KFlag;	

	CKnownFileMap		m_Files_map;
};

#endif // SHAREDFILELIST_H
