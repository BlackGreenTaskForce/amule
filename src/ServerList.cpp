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

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "ServerList.h"
#endif

#include "Types.h"

#include <wx/defs.h>		// Needed before any other wx/*.h
#ifdef __WXMSW__
	#include <wx/msw/winundef.h>
#endif

#include <wx/txtstrm.h>
#include <wx/listimpl.cpp>
#include <wx/wfstream.h>
#include <wx/intl.h>		// Needed for _
#include <wx/filename.h>	// Needed for wxFileName
#include <wx/url.h>			// Needed for wxURL

#ifndef AMULE_DAEMON
	#include <wx/msgdlg.h>		// Needed for wxMessageBox
#else 
	#define wxMessageBox(x,y,z) AddLogLineM(true,x)
#endif

#include "ServerList.h"		// Interface declarations.
#include "ListenSocket.h"	// Needed for CListenSocket
#include "DownloadQueue.h"	// Needed for CDownloadQueue
#include "UploadQueue.h"	// Needed for CUploadQueue
#include "ServerConnect.h"		// Needed for CServerConnect
#include "Server.h"		// Needed for CServer and SRV_PR_*
#include "OtherStructs.h"	// Needed for ServerMet_Struct
#include "Packet.h"		// Needed for CInvalidPacket
#include "OPCodes.h"		// Needed for MET_HEADER
#include "SafeFile.h"		// Needed for CSafeFile
#include "HTTPDownload.h"	// Needed for HTTPThread
#include "Preferences.h"	// Needed for CPreferences
#include "amule.h"			// Needed for theApp
#include "GetTickCount.h" // Neeed for GetTickCount
#include "NetworkFunctions.h" // Needed for StringIPtoUint32
#include "Statistics.h"		// Needed for CStatistics


CServerList::CServerList()
{
	serverpos = 0;
	searchserverpos = 0;
	statserverpos = 0;
	delservercount = 0;
	m_nLastED2KServerLinkCheck = ::GetTickCount();
}

bool CServerList::Init()
{
	// Load Metfile
	wxString strTempFilename;
	printf("*** reading servers\n");
	strTempFilename = theApp.ConfigDir + wxT("server.met");
	bool bRes = AddServermetToList(strTempFilename, false);

	// insert static servers from textfile
	strTempFilename=  theApp.ConfigDir + wxT("staticservers.dat");
	AddServersFromTextFile(strTempFilename);
	
	// Send the auto-update of server.met via HTTPThread requests
	current_url_index = 0;
	if ( thePrefs::AutoServerlist()) {
		AutoUpdate();
	}	
	
	return bRes;
}

bool CServerList::AddServermetToList(const wxString& strFile, bool merge)
{
	
	if (!merge) {
		Notify_ServerRemoveAll();
		RemoveAllServers();
	}
	CSafeFile servermet;
	if(!wxFileExists(strFile)) {
		AddLogLineM(false, _("Failed to load server.met!"));
		return false;
	}

	if (!servermet.Open(strFile,CFile::read)){ 
		AddLogLineM(false, _("Failed to load server.met!"));
		return false;
	}

	try {

		Notify_ServerFreeze();
		
		uchar version = servermet.ReadUInt8();
		
		if (version != 0xE0 && version != MET_HEADER) {
			AddLogLineM(false, wxString::Format(_("Invalid versiontag in server.met (0x%x , size %i)!"),version, sizeof(version)));
			throw CInvalidPacket("Corrupted server.met");
		}

		uint32 fservercount = servermet.ReadUInt32();

		ServerMet_Struct sbuffer;
		uint32 iAddCount = 0;

		for (uint32 j = 0;j < fservercount; ++j) {
			// get server

			sbuffer.ip = servermet.ReadUInt32();
			sbuffer.port = servermet.ReadUInt16();
			
			sbuffer.tagcount = servermet.ReadUInt32();
			
			CServer* newserver = new CServer(&sbuffer);

			//add tags
			for (uint32 i=0;i < sbuffer.tagcount; ++i) {
				newserver->AddTagFromFile(&servermet);
				// Removing warning. As long as SRV_PR_MIN is = 0, no need to compare
				if ( /* newserver->GetPreferences() < SRV_PR_MIN || */ newserver->GetPreferences() > SRV_PR_MAX)
					newserver->SetPreference(SRV_PR_NORMAL);
			}

			if (newserver->GetPreferences() > SRV_PR_HIGH) {
				newserver->SetPreference(SRV_PR_NORMAL);
			}
			
			// set listname for server
			if (newserver->GetListName().IsEmpty()) {
				newserver->SetListName(wxT("Server ") +newserver->GetAddress());
			}
			if (!theApp.AddServer(newserver)) {
				CServer* update = theApp.serverlist->GetServerByAddress(newserver->GetAddress(), newserver->GetPort());
				if(update) {
					update->SetListName( newserver->GetListName());
					if(!newserver->GetDescription().IsEmpty()) {
						update->SetDescription( newserver->GetDescription());
					}
					Notify_ServerRefresh(update);
				}
				delete newserver;
			} else {
				++iAddCount;
			}

			// don't yield all the time
			// Kry - What's this supposed to do?
			/*
			if(j%75==0) {
				theApp.Yield();
			}
			*/
		}
		Notify_ServerThaw();
    
		if (!merge) {
			AddLogLineM(true, wxString::Format(_("%i servers in server.met found"),fservercount));
		} else {
			AddLogLineM(true, wxString::Format(_("%d servers added"), iAddCount));
		}
	}
	catch (CInvalidPacket e) {
		AddLogLineM(true,_("Error: the file server.met is corrupted"));
		servermet.Close();
		Notify_ServerThaw();
		return false;
	}
	servermet.Close();
	return true;
}

bool CServerList::AddServer(CServer* in_server)
{
	if (thePrefs::FilterBadIPs()) {
		if ( !in_server->HasDynIP() && !IsGoodIP( in_server->GetIP() )) {
			return false;
		}
	}
	CServer* test_server = GetServerByAddress(in_server->GetAddress(), in_server->GetPort());
	// lfroen - it's ok, gui status checked in Notify
	// if (test_server && theApp.amuledlg) {
	if (test_server) {
		test_server->ResetFailedCount();
		Notify_ServerRefresh( test_server );
		return false;
	}
	list.AddTail(in_server); //AddTail(in_server);
	return true;
}

void CServerList::ServerStats()
{
	uint32 temp;
	temp = (uint32)time(NULL);
	
	if(theApp.serverconnect->IsConnected() && list.GetCount() > 0) {
		CServer* ping_server = GetNextStatServer();
		CServer* test = ping_server;
		if(!ping_server) {
			return;
		}
	        while(ping_server->GetLastPinged() != 0 && (::GetTickCount() - ping_server->GetLastPinged()) < UDPSERVSTATREASKTIME) {
			ping_server = this->GetNextStatServer();
			if(ping_server == test) {
				return;
			}
		}
		if(ping_server->GetFailedCount() >= thePrefs::GetDeadserverRetries() && thePrefs::DeadServer() && !ping_server->IsStaticMember()) {
			Notify_ServerRemove(ping_server);
			return;
		}
		CPacket* packet = new CPacket(OP_GLOBSERVSTATREQ, 4);
		srand((unsigned)time(NULL));
		uint32 time = 0x55AA0000 + (uint16)rand();
		ping_server->SetChallenge(time);
		packet->CopyUInt32ToDataBuffer(time);
		ping_server->SetLastPinged(::GetTickCount());
		//ping_server->SetLastPingedTime(temp);
		ping_server->AddFailedCount();
		Notify_ServerRefresh(ping_server);
		theApp.statistics->AddUpDataOverheadServer(packet->GetPacketSize());
		theApp.serverconnect->SendUDPPacket(packet, ping_server, true);
		
		ping_server->SetLastDescPingedCount(false);
		if(ping_server->GetLastDescPingedCount() < 2) {
			// eserver 16.45+ supports a new OP_SERVER_DESC_RES answer, if the OP_SERVER_DESC_REQ contains a uint32
			// challenge, the server returns additional info with OP_SERVER_DESC_RES. To properly distinguish the
			// old and new OP_SERVER_DESC_RES answer, the challenge has to be selected carefully. The first 2 bytes 
			// of the challenge (in network byte order) MUST NOT be a valid string-len-int16!
			uint32 randomness = 1 + (int) (((float)(0xFFFF))*rand()/(RAND_MAX+1.0));
			uint32 uDescReqChallenge = ((uint32)randomness << 16) + INV_SERV_DESC_LEN; // 0xF0FF = an 'invalid' string length.
			packet = new CPacket( OP_SERVER_DESC_REQ,4);
			packet->CopyUInt32ToDataBuffer(uDescReqChallenge);
			theApp.statistics->AddUpDataOverheadServer(packet->GetPacketSize());
			theApp.serverconnect->SendUDPPacket(packet, ping_server, true);
		} else {
			ping_server->SetLastDescPingedCount(true);
		}
	}
}


void CServerList::RemoveServer(CServer* out_server)
{
	if (out_server == theApp.serverconnect->GetCurrentServer()) {
		wxMessageBox(_("You are connected to the server you are trying to delete. please disconnect first."), _("Info"), wxOK);	
	} else {
	
		POSITION pos = list.Find( out_server );
		if ( pos != NULL ) {
			if (theApp.downloadqueue->GetUDPServer() == out_server) {
				theApp.downloadqueue->SetUDPServer( 0 );
			}	
			list.RemoveAt(pos);
			++delservercount;
			delete out_server;
			return;
		}
	}
}

void CServerList::RemoveAllServers()
{
	delservercount += list.GetSize();
	// no connection, safely remove all servers
	while ( !list.IsEmpty() ) {
		delete list.GetTail();
		list.RemoveTail();
	}
}

void CServerList::GetStatus(uint32 &total, uint32 &failed, uint32 &user, uint32 &file, uint32 &tuser, uint32 &tfile,float &occ)
{
	total = list.GetCount();
	failed = 0;
	user = 0;
	file = 0;
	tuser=0;
	tfile = 0;
	occ=0;
	uint32 maxusers=0;
	uint32 tuserk = 0;

	CServer* curr;
	for (POSITION pos = list.GetHeadPosition(); pos !=0; ) {
		curr = (CServer*)list.GetNext(pos);
		if( curr->GetFailedCount() ) {
			++failed;
		} else {
			user += curr->GetUsers();
			file += curr->GetFiles();
		}
		tuser += curr->GetUsers();
		tfile += curr->GetFiles();
		
		if (curr->GetMaxUsers()) {
			tuserk += curr->GetUsers(); // total users on servers with known maximum
			maxusers+=curr->GetMaxUsers();
		}
	}
	if (maxusers>0) {
		occ=(float)(tuserk*100)/maxusers;
	}
}

void CServerList::GetUserFileStatus(uint32 &user, uint32 &file)
{
	user = 0;
	file = 0;
	CServer* curr;
	for (POSITION pos = list.GetHeadPosition(); pos !=0; ) {
		curr = (CServer*)list.GetNext(pos);
		if( !curr->GetFailedCount() ) {
			user += curr->GetUsers();
			file += curr->GetFiles();
		}
	}
}

CServerList::~CServerList()
{
	SaveServermetToFile();
	while ( !list.IsEmpty() ) {
		delete list.GetTail();
		list.RemoveTail();
	}
}

void CServerList::AddServersFromTextFile(wxString strFilename,bool isstaticserver, bool writetolog)
{
	// emanuelw(20030731) added writetolog
	wxString strLine;
	//CStdioFile f;
	//wxFFile f;
	//if (!f.Open(strFilename, CFile::modeRead | CFile::typeText))
	if(!wxFileName::FileExists(strFilename)) {
		// no file. do nothing.
		return;
	}
	wxFileInputStream stream(strFilename);
	if(!stream.Ok()) {
		return;
	}

	wxTextInputStream f(stream);

	while(strLine=f.ReadLine()) {
		if(stream.Eof()) {
			break; // stop iteration if end of file is met..
		}
		// (while won't do it for us)
		// format is host:port,Name
		if (strLine.Length() < 5) {
			continue;
		}
		if (strLine.GetChar(0) == '#' || strLine.GetChar(0) == '/') {
			continue;
		}

		// fetch host
		int pos = strLine.Find(wxT(":"));
		if (pos == -1) {
			pos = strLine.Find(wxT(","));
			if (pos == -1) {
				continue;
			}
		}
		wxString strHost = strLine.Left(pos);
		strLine = strLine.Mid(pos+1);
		// fetch  port
		pos = strLine.Find(wxT(","));
		if (pos == -1) {
			continue;
		}
		wxString strPort = strLine.Left(pos);
		strLine = strLine.Mid(pos+1);

		// Barry - fetch priority
		pos = strLine.Find(wxT(","));
		int priority = SRV_PR_NORMAL;
		if (pos == 1) {
			wxString strPriority = strLine.Left(pos);
			try {
				priority = StrToLong(strPriority);
				if ((priority < 0) || (priority > 2)) {
					priority = SRV_PR_NORMAL;
				}
			} catch (...) {
			}
			strLine = strLine.Mid(pos+1);
		}

		// fetch name
		wxString strName = strLine;
		strName.Replace(wxT("\r"),wxT( ""));
		strName.Replace(wxT("\n"),wxT( ""));

		// emanuelw(20030924) fix: if there is no name the ip is set as name
		if(wxStrlen(strName) == 0) {
			strName = strHost;
		}

		// create server object and add it to the list
		CServer* nsrv = new CServer(StrToULong(strPort), strHost);
		nsrv->SetListName(strName);

		// emanuelw(20030924) fix: isstaticserver now is used! before it was always true
		nsrv->SetIsStaticMember(isstaticserver);

		// Barry - Was always high
		nsrv->SetPreference(priority);

		// emanuelw(20030924) added: create log entry
		if(writetolog == true) {
			AddLogLineM(true,wxString(_("Server added: "))+nsrv->GetAddress()); 
		}

		if (!theApp.AddServer(nsrv))	{
			delete nsrv;
			CServer* srvexisting = GetServerByAddress(strHost, StrToULong(strPort));
			if (srvexisting) {
				srvexisting->SetListName(strName);
				srvexisting->SetIsStaticMember(true);
				// Barry - Was always high
				srvexisting->SetPreference(priority); 
				Notify_ServerRefresh(srvexisting);
			}
		}
	}
}

void CServerList::Sort()
{
	POSITION pos1, pos2;
	uint16 i = 0;
	for( pos1 = list.GetHeadPosition(); (pos2 = pos1 ) != NULL;) {
		list.GetNext(pos1);
		CServer* cur_server = list.GetAt(pos2);
		if (cur_server->GetPreferences()== SRV_PR_HIGH) {
			list.AddHead(cur_server);
			list.RemoveAt(pos2);
		} else if (cur_server->GetPreferences() == SRV_PR_LOW) {
			list.AddTail(cur_server);
			list.RemoveAt(pos2);
		}
		++i;
		if (i == list.GetCount()) {
			break;
		}
	}
}

CServer* CServerList::GetNextServer()
{
	CServer* nextserver = 0;
	uint32 i = 0;
	if (serverpos>=((uint32)list.GetCount())) {
		return 0;
	}
	while (!nextserver && i != (uint32)list.GetCount()) {
		POSITION posIndex = list.FindIndex(serverpos);
		if (posIndex == NULL) {	// check if search position is still valid (could be corrupted by server delete operation)
			posIndex = list.GetHeadPosition();
			serverpos= 0; // <<--9/27/02 zg
		}

		nextserver = list.GetAt(posIndex);
		++serverpos;
		++i;
		// TODO: Add more option to filter bad server like min ping, min users etc
		//if (nextserver->preferences = ?)
		//	nextserver = 0;
		//if (serverpos == list.GetCount()) return 0;//	serverpos = 0;
	}
	return nextserver;
}

CServer* CServerList::GetNextSearchServer()
{
	CServer* nextserver = 0;
	uint32 i = 0;
	while (!nextserver && i != (uint32)list.GetCount()) {
		POSITION posIndex = list.FindIndex(searchserverpos);
		if (posIndex == NULL) {	// check if search position is still valid (could be corrupted by server delete operation)
			posIndex = list.GetHeadPosition();
			searchserverpos=0;
		}
		nextserver = list.GetAt(posIndex);
		++searchserverpos;
		++i;
		if (searchserverpos == (uint32)list.GetCount()) {
			searchserverpos = 0;
		}
	}
	return nextserver;
}

CServer* CServerList::GetNextStatServer()
{
	CServer* nextserver = 0;
	uint32 i = 0;
	while (!nextserver && i != (uint32)list.GetCount()) {
		POSITION posIndex = list.FindIndex(statserverpos);
		if (posIndex == NULL) {	// check if search position is still valid (could be corrupted by server delete operation)
			posIndex = list.GetHeadPosition();
			statserverpos=0;
		}

		nextserver = list.GetAt(posIndex);
		++statserverpos;
		++i;
		if (statserverpos == (uint32)list.GetCount()) {
			statserverpos = 0;
		}
	}
	return nextserver;
}


CServer* CServerList::GetNextServer(CServer* lastserver)
{
	if (list.IsEmpty()) {
		return 0;
	}
	if (!lastserver) {
		return list.GetHead();
	}

	POSITION pos = list.Find(lastserver);
	if (!pos) {
		return list.GetHead();
	}
	list.GetNext(pos);
	if (!pos) {
		return NULL;
	} else {
		return list.GetAt(pos);
	}
}

CServer* CServerList::GetServerByAddress(const wxString& address, uint16 port)
{
	for (POSITION pos = list.GetHeadPosition();pos != 0;) {
		CServer *s = list.GetNext(pos); // i_a: small speed optimization
		if (port == s->GetPort() && s->GetAddress() == address) {
			return s;
		}
	}
	return NULL;
}

CServer* CServerList::GetServerByIP(uint32 nIP){
	for (POSITION pos = list.GetHeadPosition();pos != 0;){
        CServer* s = list.GetNext(pos);
		if (s->GetIP() == nIP)
			return s; 
	}
	return NULL;
}

CServer* CServerList::GetServerByIP(uint32 nIP, uint16 nPort){
	for (POSITION pos = list.GetHeadPosition();pos != 0;){
        CServer* s = list.GetNext(pos);
		if (s->GetIP() == nIP && s->GetPort() == nPort)
			return s; 
	}
	return NULL;
}

bool CServerList::SaveServermetToFile()
{
	wxString newservermet(theApp.ConfigDir + wxT("server.met.new"));
	CSafeFile servermet;
	servermet.Open(newservermet, CFile::write);
	if (!servermet.IsOpened()) {
		AddLogLineM(false,_("Failed to save server.met!"));
		return false;
	}


	servermet.WriteUInt8(0xE0);
	
	uint32 fservercount = list.GetCount(); 
	servermet.WriteUInt32(fservercount);
	
	CServer* nextserver;
	
		for (uint32 j = 0; j != fservercount; ++j){
			nextserver = GetServerByIndex(j);

			uint16 tagcount = 12;
			if (!nextserver->GetListName().IsEmpty()) 
				++tagcount;
			if (!nextserver->GetDynIP().IsEmpty())
				++tagcount;
			if (!nextserver->GetDescription().IsEmpty())
				++tagcount;
			if (nextserver->GetConnPort() != nextserver->GetPort())
				++tagcount;		
			
			servermet.WriteUInt32(nextserver->GetIP());
			servermet.WriteUInt16(nextserver->GetPort());
			servermet.WriteUInt32(tagcount);
						
			if (!nextserver->GetListName().IsEmpty()) {
				CTag servername( ST_SERVERNAME, nextserver->GetListName() );
				servername.WriteTagToFile(&servermet);
			}
			if (!nextserver->GetDynIP().IsEmpty()) {
				CTag serverdynip( ST_DYNIP, nextserver->GetDynIP() );
				serverdynip.WriteTagToFile(&servermet);
			}
			if (!nextserver->GetDescription().IsEmpty()) {
				CTag serverdesc( ST_DESCRIPTION, nextserver->GetDescription() );
				serverdesc.WriteTagToFile(&servermet);
			}
			if (nextserver->GetConnPort() != nextserver->GetPort()) {
				CTag auxportslist( ST_AUXPORTSLIST, nextserver->GetAuxPortsList() );
				auxportslist.WriteTagToFile(&servermet);
			}
			CTag serverfail(ST_FAIL, nextserver->GetFailedCount() );
			serverfail.WriteTagToFile(&servermet);
			CTag serverpref( ST_PREFERENCE, nextserver->GetPreferences() );
			serverpref.WriteTagToFile(&servermet);
			CTag serveruser("users", nextserver->GetUsers() );
			serveruser.WriteTagToFile(&servermet);
			CTag serverfiles("files", nextserver->GetFiles() );
			serverfiles.WriteTagToFile(&servermet);
			CTag serverping(ST_PING, nextserver->GetPing() );
			serverping.WriteTagToFile(&servermet);
			CTag serverlastp(ST_LASTPING, nextserver->GetLastPinged() );
			serverlastp.WriteTagToFile(&servermet);
			CTag servermaxusers(ST_MAXUSERS, nextserver->GetMaxUsers() );
			servermaxusers.WriteTagToFile(&servermet);
			CTag softfiles(ST_SOFTFILES, nextserver->GetSoftFiles() );
			softfiles.WriteTagToFile(&servermet);
			CTag hardfiles(ST_HARDFILES, nextserver->GetHardFiles() );
			hardfiles.WriteTagToFile(&servermet);
			CTag version(ST_VERSION, nextserver->GetVersion());
			version.WriteTagToFile(&servermet);
			CTag tagUDPFlags(ST_UDPFLAGS, nextserver->GetUDPFlags() );
			tagUDPFlags.WriteTagToFile(&servermet);
			CTag tagLowIDUsers(ST_LOWIDUSERS, nextserver->GetLowIDUsers() );
			tagLowIDUsers.WriteTagToFile(&servermet);			
	}
	
	servermet.Flush();
	servermet.Close();
	wxString curservermet(theApp.ConfigDir + wxT("server.met"));
	wxString oldservermet(theApp.ConfigDir + wxT("server_met.old"));
	if ( wxFileExists(oldservermet) ) {
		wxRemoveFile(oldservermet);
	}
	if ( wxFileExists(curservermet) ) {
		wxRenameFile(curservermet,oldservermet);
	}
	wxRenameFile(newservermet,curservermet);
	return true;
}


void CServerList::Process()
{
	// emanuelw(20030924) added:Check for new server links once per second.	
	if ((::GetTickCount() - m_nLastED2KServerLinkCheck) >= 1000) {
		wxString filename(theApp.ConfigDir + wxT("ED2KServers"));
	
		if (wxFile::Exists(filename)) {
			AddServersFromTextFile(filename, false, true);
			SaveServermetToFile();
			wxRemoveFile(filename);
		}
		m_nLastED2KServerLinkCheck = ::GetTickCount();
	}
}


void CServerList::RemoveDeadServers()
{
	if ( thePrefs::DeadServer() ) {
		for ( POSITION pos = list.GetHeadPosition(); pos != NULL; ) {
			CServer* cur_server = theApp.serverlist->list.GetNext( pos );
			if ( cur_server->GetFailedCount() > thePrefs::GetDeadserverRetries() && !cur_server->IsStaticMember()) {
				RemoveServer(cur_server);
			}
		}
	}
}

void CServerList::UpdateServerMetFromURL(wxString strURL)
{
	if (strURL.Find(wxT("://")) == -1) {
		AddLogLineM(true, _("Invalid URL"));
		return;
	}
	URLUpdate = strURL;
	wxString strTempFilename(theApp.ConfigDir + wxT("server.met.download"));
	CHTTPDownloadThread *downloader = new CHTTPDownloadThread(strURL,strTempFilename, HTTP_ServerMet);
	downloader->Create();
	downloader->Run();
}

void CServerList::DownloadFinished(uint32 result) {
	if(result==1) {
		wxString strTempFilename(theApp.ConfigDir + wxT("server.met.download"));
		// curl succeeded. proceed with server.met loading
		theApp.serverlist->AddServermetToList(strTempFilename);
		theApp.serverlist->SaveServermetToFile();
		// So, file is loaded and merged, and also saved
		wxRemoveFile(strTempFilename);
	} else {
		AddLogLineM(true, _("Failed to download the server list from ") + URLUpdate);
	}
}


void CServerList::AutoUpdate() {
	
	uint8 url_count = theApp.glob_prefs->adresses_list.GetCount();
	
	if (!url_count) {
		AddLogLineM(true, _("No serverlist address entry in 'addresses.dat' found. Please paste a valid serverlist address into this file in order to auto-update your serverlist"));
		return;
	}
	
	wxString strURLToDownload; 
	wxString strTempFilename;

	// Do current URL. Callback function will take care of the others.
	while ( current_url_index < url_count ) {
		wxString URI = theApp.glob_prefs->adresses_list[current_url_index];

		// We use wxURL to validate the URI
		if ( wxURL( URI ).GetError() == wxURL_NOERR ) {
			// Ok, got a valid URI
			URLAutoUpdate = strURLToDownload;
			strTempFilename =  theApp.ConfigDir + wxT("server_auto.met");
		
			CHTTPDownloadThread *downloader = new CHTTPDownloadThread(strURLToDownload,strTempFilename, HTTP_ServerMetAuto);
			downloader->Create();
			downloader->Run();
		
			return;
		} else {
			AddLogLineM(true, _("Warning, invalid URL specified for auto-updating of servers: ") + URI);
		}
		
		current_url_index++;
	}

	AddLogLineM(true, _("No valid server.met auto-download url on addresses.dat"));
}


void CServerList::AutoDownloadFinished(uint32 result) {
	if(result==1) {
		wxString strTempFilename(theApp.ConfigDir + wxT("server_auto.met"));
		// curl succeeded. proceed with server.met loading
		theApp.serverlist->AddServermetToList(strTempFilename);
		theApp.serverlist->SaveServermetToFile();
		// So, file is loaded and merged, and also saved
		wxRemoveFile(strTempFilename);
	} else {
		AddLogLineM(true, _("Failed to download the server list from ") + URLUpdate);
	}
	
	++current_url_index;
	

	if (current_url_index < theApp.glob_prefs->adresses_list.GetCount()) {		
		// Next!	
		AutoUpdate();
	}
	
}
