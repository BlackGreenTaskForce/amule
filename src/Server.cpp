//
// This file is part of the aMule Project.
//
// Copyright (c) 2003-2005 aMule Team ( admin@amule.org / http://www.amule.org )
// Copyright (c) 2002 Merkur ( devs@emule-project.net / http://www.emule-project.net )
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
#pragma implementation "Server.h"
#endif

#include "Server.h"		// Interface declarations.
#include "SafeFile.h"		// Needed for CSafeFile
#include "OtherFunctions.h"	// Needed for nstrdup
#include "NetworkFunctions.h" // Needed for StringIPtoUint32
#include "OtherStructs.h"	// Needed for ServerMet_Struct
#include "Packet.h"		// Needed for CTag
#include "StringFunctions.h" // Needed for unicode2char 
#include "Logger.h"
#include "Format.h"

#include <wx/intl.h>	// Needed for _

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(TagList)

CServer::CServer(ServerMet_Struct* in_data)
{
	port = in_data->port;
	ip = in_data->ip;

	Init();
}

CServer::CServer(uint16 in_port, const wxString i_addr)
{

	port = in_port;
	ip = StringIPtoUint32(i_addr);

	Init();

	// GonoszTopi - Init() would clear dynip !
	// Check that the ip isn't in fact 0.0.0.0
	if (!ip && !StringIPtoUint32( i_addr, ip ) ) {
		// If ip == 0, the address is a hostname
		dynip = i_addr;
	}
}

#ifdef CLIENT_GUI
CServer::CServer(CEC_Server_Tag *tag)
{
	ip = tag->GetIPv4Data().IP();
	ipfull = Uint32toStringIP(ip);
	port = tag->GetIPv4Data().port;

	listname = tag->ServerName();
	description = tag->ServerDesc();
	maxusers = tag->GetMaxUsers();
	
	files = tag->GetFiles();
	users = tag->GetUsers();
   
	preferences = tag->GetPrio(); // SRV_PR_NORMAL = 0, so it's ok
    staticservermember = tag->GetStatic();

	ping = tag->GetPing();
	failedcount = tag->GetFailed();	
}
#endif

// copy constructor
CServer::CServer(CServer* pOld)
{
	wxASSERT(pOld != NULL);
	taglist = new TagList;
	// Got a bt here with pOld->taglist == NULL. Check that.
	if (pOld && pOld->taglist) {
		for(	TagList::Node* pos = pOld->taglist->GetFirst();
			pos != NULL;
			pos = pos->GetNext()) {
			CTag* pOldTag = pos->GetData(); //pOld->taglist->GetAt(pos);
			taglist->Append(pOldTag->CloneTag()); //AddTail(pOldTag->CloneTag());
		}
	}
	port = pOld->port;
	ip = pOld->ip; 
	realport = pOld->realport;
	staticservermember=pOld->IsStaticMember();
	tagcount = pOld->tagcount;
	ipfull = pOld->ipfull;
	files = pOld->files;
	users = pOld->users;
	preferences = pOld->preferences;
	ping = pOld->ping;
	failedcount = pOld->failedcount; 
	lastpinged = pOld->lastpinged;
	description = pOld->description;
	listname = pOld->listname;
	dynip = pOld->dynip;
	maxusers = pOld->maxusers;
	m_strVersion = pOld->m_strVersion;
	m_uTCPFlags = pOld->m_uTCPFlags;
	m_uUDPFlags = pOld->m_uUDPFlags;
	challenge = pOld->challenge;
	softfiles = pOld->softfiles;
	hardfiles = pOld->hardfiles;
	m_uDescReqChallenge = pOld->m_uDescReqChallenge;
	m_uLowIDUsers = pOld->m_uLowIDUsers;
	lastdescpingedcout = pOld->lastdescpingedcout;
	m_auxPorts = pOld->m_auxPorts;
}

CServer::~CServer()
{

	if(taglist) {
		for(TagList::Node* pos = taglist->GetFirst(); pos != NULL; pos=pos->GetNext()) {
			delete pos->GetData(); //taglist->GetAt(pos);
		}
		taglist->Clear(); //RemoveAll();
		delete taglist;
		taglist=NULL;
	}
}

void CServer::Init() {
	
	ipfull = Uint32toStringIP(ip);
	
	taglist = new TagList;
	realport = 0;
	tagcount = 0;
	files = 0;
	users = 0;
	preferences = 0;
	ping = 0;
	description.Clear();
	listname.Clear();
	dynip.Clear();
	failedcount = 0; 
	lastpinged = 0;
	staticservermember=0;
	maxusers=0;
	m_uTCPFlags = 0;
	m_uUDPFlags = 0;
	challenge = 0;
	softfiles = 0;
	hardfiles = 0;	
	m_strVersion = _("Unknown");
	m_uLowIDUsers = 0;
	m_uDescReqChallenge = 0;
	lastdescpingedcout = 0;
	m_auxPorts.Clear();
}	

bool CServer::AddTagFromFile(CFileDataIO* servermet){
	
	if (servermet == NULL) {
		return false;
	}
	
	try {
		CTag tag(*servermet, true);

		switch(tag.GetNameID()){		
		case ST_SERVERNAME:
			#if wxUSE_UNICODE
			if (listname.IsEmpty())
			#endif
				listname = tag.GetStr();
			break;
		case ST_DESCRIPTION:
			#if wxUSE_UNICODE
			if (description.IsEmpty())
			#endif
				description = tag.GetStr();		
			break;
		case ST_PREFERENCE:
			preferences =tag.GetInt();
			break;
		case ST_PING:
			ping = tag.GetInt();
			break;
		case ST_DYNIP:
			#if wxUSE_UNICODE
			if (dynip.IsEmpty())
			#endif	
				dynip = tag.GetStr();
			break;
		case ST_FAIL:
			failedcount = tag.GetInt();
			break;
		case ST_LASTPING:
			lastpinged = tag.GetInt();
			break;
		case ST_MAXUSERS:
			maxusers = tag.GetInt();
			break;
		case ST_SOFTFILES:
			softfiles = tag.GetInt();
			break;
		case ST_HARDFILES:
			hardfiles = tag.GetInt();
			break;
		case ST_VERSION:
			if (tag.IsStr()) {
				#ifdef wxUSE_UNICODE
				if (m_strVersion.IsEmpty())
				#endif
					m_strVersion = tag.GetStr();
			} else if (tag.IsInt()) {
				m_strVersion = wxString::Format(wxT("%u.%u"), tag.GetInt() >> 16, tag.GetInt() & 0xFFFF);
			} else {
				wxASSERT(0);
			}
			break;
		case ST_UDPFLAGS:
			if (tag.IsInt()) {
				m_uUDPFlags = tag.GetInt();
			}
			break;
		case ST_AUXPORTSLIST:
			if (tag.IsStr()) {
				m_auxPorts = tag.GetStr();
				realport = port;
				port = StrToULong(m_auxPorts.BeforeFirst(','));
			}
			break;
		case ST_LOWIDUSERS:
			if (tag.IsInt())			
				m_uLowIDUsers = tag.GetInt();
			break;
		default:
			if (tag.GetNameID()){
				wxASSERT(0);
			} else if (!CmpED2KTagName(tag.GetName(), "files")) {
				wxASSERT( tag.IsInt() );
				if (tag.IsInt()) {
					files = tag.GetInt();
				}
			} else if (!CmpED2KTagName(tag.GetName(), "users")) {
				wxASSERT( tag.IsInt() );
				if (tag.IsInt()) {
					users = tag.GetInt();
				}
			}
		}
	} catch (const CInvalidPacket& e) {
		AddDebugLogLineM( true, logPacketErrors,
			wxT("Caught CInvalidPacket exception in CServer::AddTagFromFile! server.met is corrupted.")
		);
		
		throw;
	} catch (const wxString& error) {
		AddDebugLogLineM( true, logPacketErrors,
			CFormat( wxT("Caught exception in CServer::AddTagFromFile! server.met is corrupted.\nError: %s") )
				% error
		);
		
		throw CInvalidPacket(wxT("Error reading server.met"));
	} catch (...) {
		AddDebugLogLineM( true, logGeneral,
			wxT("Caught unknown exception in CServer::AddTagFromFile! server.met is corrupted.") );
		
		throw CInvalidPacket(wxT("Error reading server.met"));
	}
	return true;
}

void CServer::SetListName(const wxString& newname)
{
	listname = newname;
}

void CServer::SetDescription(const wxString& newname)
{
	description = newname;
}

void CServer::SetID(uint32 newip)
{
	wxASSERT(newip);
	ip = newip;
	ipfull = Uint32toStringIP(ip);
}

void CServer::SetDynIP(const wxString& newdynip)
{
	dynip = newdynip;
}


void CServer::SetLastDescPingedCount(bool bReset)
{
	if (bReset) {
		lastdescpingedcout = 0;
	} else {
		lastdescpingedcout++;
	}
}
