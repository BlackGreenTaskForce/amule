#include <unistd.h>			// Needed for close(2) and sleep(3)
#include <wx/defs.h>
#include <wx/gauge.h>
#include <wx/textctrl.h>

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "amule-remote-gui.h"
// implementations for headers which don't have a corresponding .cpp file
#pragma implementation "GetTickCount.h"
#pragma implementation "GuiEvents.h"
#pragma implementation "updownclient.h"
#endif

#ifdef __WXMSW__
	#include <winsock.h>
	#include <wx/msw/winundef.h>
#else
	#ifdef __BSD__
		#include <sys/types.h>
	#endif /* __BSD__ */
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

#ifdef __WXGTK__

	#ifdef __BSD__
     	#include <sys/param.h>
       	#include <sys/mount.h>
	#else 
		#include <execinfo.h>
		#include <mntent.h>
	#endif /* __BSD__ */

	#include <X11/Xlib.h>		// Needed for XParseGeometry
	#include <gdk/gdk.h>
	#include <gtk/gtk.h>
	
#endif

#ifdef HAVE_CONFIG_H
	#include "config.h"		// Needed for HAVE_GETRLIMIT, HAVE_SETRLIMIT,
					//   HAVE_SYS_RESOURCE_H, LOCALEDIR, PACKAGE, 
					//   PACKAGE_STRING and VERSION
#endif

#include <wx/filefn.h>
#include <wx/ffile.h>
#include <wx/file.h>
#include <wx/filename.h>		// Needed for wxFileName::GetPathSeparator()
#include <wx/log.h>
#include <wx/timer.h>
#include <wx/config.h>
#include <wx/clipbrd.h>			// Needed for wxClipBoard
#include <wx/socket.h>			// Needed for wxSocket
#include <wx/splash.h>			// Needed for wxSplashScreen
#include <wx/utils.h>
#include <wx/ipc.h>
#include <wx/intl.h>			// Needed for i18n
#include <wx/mimetype.h>		// For launching default browser
#include <wx/textfile.h>		// Needed for wxTextFile
#include <wx/cmdline.h>			// Needed for wxCmdLineParser
#include <wx/tokenzr.h>			// Needed for wxStringTokenizer
#include <wx/msgdlg.h>			// Needed for wxMessageBox
#include <wx/url.h>

#include "amule.h"			// Interface declarations.
#include "GetTickCount.h"		// Needed for GetTickCount
#include "Server.h"			// Needed for GetListName
#include "OtherFunctions.h"		// Needed for GetTickCount
#include "TransferWnd.h"		// Needed for CTransferWnd
#include "SharedFilesWnd.h"		// Needed for CSharedFilesWnd
#include "ServerWnd.h"			// Needed for CServerWnd
#include "StatisticsDlg.h"		// Needed for CStatisticsDlg
#include "Preferences.h"		// Needed for CPreferences
#include "StringFunctions.h"
#include "PartFile.h"			// Needed for CPartFile
#include "updownclient.h"
#include "Logger.h"
#include "muuli_wdr.h"			// Needed for IDs
#include "amuleDlg.h"			// Needed for CamuleDlg
#include "SearchList.h"			// Needed for CSearchFile
#include "SearchDlg.h"			// Needed for CSearchDlg
#include "SharedFilesCtrl.h"		// Needed for CSharedFilesCtrl
#include "DownloadListCtrl.h"		// Needed for CDownloadListCtrl
#include "ClientListCtrl.h"
#include "ServerListCtrl.h"

#include "CMD4Hash.h"
#include "ECSocket.h"
#include "ECPacket.h"
#include "ECcodes.h"

#include "MD5Sum.h"

CEConnectDlg::CEConnectDlg() :
	wxDialog(theApp.amuledlg, -1, _("Connect to remote amule"), wxDefaultPosition )
{
	CoreConnect(this, TRUE);
	
	//
	// FIXME: there should be preferences for this
	CastChild(ID_REMOTE_PORT, wxTextCtrl)->SetValue(wxT("4712"));
	CastChild(ID_REMOTE_HOST, wxTextCtrl)->SetValue(wxT("localhost"));
	
	CentreOnParent();
}

wxString CEConnectDlg::PassHash()
{
	return MD5Sum(passwd).GetHash();
}
	
BEGIN_EVENT_TABLE(CEConnectDlg, wxDialog)
  EVT_BUTTON(wxID_OK, CEConnectDlg::OnOK)
END_EVENT_TABLE()

void CEConnectDlg::OnOK(wxCommandEvent& evt)
{
	wxString s_port = CastChild(ID_REMOTE_PORT, wxTextCtrl)->GetValue();
	port = StrToLong( s_port );
	
	host = CastChild(ID_REMOTE_HOST, wxTextCtrl)->GetValue();
	passwd = CastChild(ID_EC_PASSWD, wxTextCtrl)->GetValue();
	
	evt.Skip();
}

BEGIN_EVENT_TABLE(CamuleRemoteGuiApp, wxApp)

	// Core timer
		EVT_TIMER(ID_CORETIMER, CamuleRemoteGuiApp::OnCoreTimer)
		
//		EVT_CUSTOM(wxEVT_NOTIFY_EVENT, -1, CamuleRemoteGuiApp::OnNotifyEvent)
		
END_EVENT_TABLE()


IMPLEMENT_APP(CamuleRemoteGuiApp)


int CamuleRemoteGuiApp::OnExit()
{
	if (core_timer) {
		// Stop the Core Timer
		delete core_timer;
	}
	return wxApp::OnExit();
}

void CamuleRemoteGuiApp::OnCoreTimer(AMULE_TIMER_EVENT_CLASS&)
{
	if ( connect->Busy() ) {
		return;
	}
	// always query connection state
	serverconnect->ReQuery();
	if ( theApp.amuledlg->sharedfileswnd->IsShown() ) {
		sharedfiles->DoRequery(EC_OP_GET_SHARED_FILES, EC_TAG_KNOWNFILE);
		sharedfiles->ReloadControl();
	} else if ( theApp.amuledlg->serverwnd->IsShown() ) {
		//serverlist->FullReload(EC_OP_GET_SERVER_LIST);
		//serverlist->ReloadControl();
	} else if ( theApp.amuledlg->transferwnd->IsShown() ) {
		downloadqueue->DoRequery(EC_OP_GET_DLOAD_QUEUE, EC_TAG_PARTFILE);
		//downloadqueue->ReloadControl();
	} else if ( theApp.amuledlg->searchwnd->IsShown() ) {
		if ( searchlist->m_curr_search != 0 ) {
			searchlist->DoRequery(EC_OP_SEARCH_RESULTS, EC_TAG_SEARCHFILE);
		}
	}
}

void CamuleRemoteGuiApp::ShutDown() {
	amuledlg->Destroy();
}

bool CamuleRemoteGuiApp::OnInit()
{
	amuledlg = NULL;
	
	if ( !wxApp::OnInit() ) {
		return false;
	}

	
	// Create the Core timer
	core_timer = new wxTimer(this,ID_CORETIMER);
	if (!core_timer) {
		printf("Fatal Error: Failed to create Core Timer");
		OnExit();
	}

	// Parse cmdline arguments.
	wxCmdLineParser cmdline(wxApp::argc, wxApp::argv);
	cmdline.AddSwitch(wxT("v"), wxT("version"), wxT("Displays the current version number."));
	cmdline.AddSwitch(wxT("h"), wxT("help"), wxT("Displays this information."));
	cmdline.AddOption(wxT("geometry"), wxEmptyString, wxT("Sets the geometry of the app.\n\t\t\t<str> uses the same format as standard X11 apps:\n\t\t\t[=][<width>{xX}<height>][{+-}<xoffset>{+-}<yoffset>]"));
	cmdline.Parse();

	
	bool geometry_enabled = false;
	wxString geom_string;
	if ( cmdline.Found(wxT("geometry"), &geom_string) ) {
		geometry_enabled = true;
	}

	connect = new CRemoteConnect;
	
	// Load Preferences
	glob_prefs = new CPreferencesRem(connect);
	
	serverconnect = new CServerConnectRem(connect);
	statistics = new CStatistics();
	
	clientlist = new CClientListRem(connect);
	searchlist = new CSearchListRem(connect);
	serverlist = new CServerListRem(connect);
	
	sharedfiles	= new CSharedFilesRem(connect);
	knownfiles = new CKnownFilesRem(sharedfiles);

	clientcredits = new CClientCreditsRem();
	
	// bugfix - do this before creating the uploadqueue
	downloadqueue = new CDownQueueRem(connect);
	uploadqueue = new CUpQueueRem(connect);
	ipfilter = new CIPFilterRem();


	// Create main dialog
	InitGui(0, geom_string);

	CEConnectDlg *dialog = new CEConnectDlg;
	do {
		
		if ( dialog->ShowModal() != wxID_OK ) {
			dialog->Destroy();
			return false;
		}
	} while ( !connect->Connect(dialog->Host(), dialog->Port(), dialog->Login(), dialog->PassHash()) );
	amuledlg->AddLogLine(true, _("Connected to amule at ") + dialog->Host());
	dialog->Destroy();
	
	serverlist->FullReload(EC_OP_GET_SERVER_LIST);
	serverlist->ReloadControl();
	
	glob_prefs->LoadRemote();
	
	IsReady = true;
	
	// Start the Core Timer
	core_timer->Start(1000);	

    //amuledlg->StartGuiTimer();

	return true;

}

void CamuleRemoteGuiApp::ShowAlert(wxString msg, wxString title, int flags)
{
	CamuleGuiBase::ShowAlert(msg, title, flags);
}

int CamuleRemoteGuiApp::InitGui(bool geometry_enabled, wxString &geom_string)
{
	CamuleGuiBase::InitGui(geometry_enabled, geom_string);
	SetTopWindow(amuledlg);
	return 0;
}

bool CamuleRemoteGuiApp::CopyTextToClipboard(wxString strText)
{
	return CamuleGuiBase::CopyTextToClipboard(strText);
}

uint32 CamuleRemoteGuiApp::GetPublicIP()
{
	return 0;
}

wxString CamuleRemoteGuiApp::GetLog(bool)
{
	return wxEmptyString;
}

wxString CamuleRemoteGuiApp::GetServerLog(bool)
{
	return wxEmptyString;
}

//
// Remote gui can't create links by itself. Pass request or retrieve from container ?
//
wxString CamuleRemoteGuiApp::CreateED2kLink(CAbstractFile const*)
{
	return wxEmptyString;
}

wxString CamuleRemoteGuiApp::CreateED2kSourceLink(CAbstractFile const *)
{
	return wxEmptyString;
}

wxString CamuleRemoteGuiApp::CreateED2kHostnameSourceLink(CAbstractFile const *)
{
	return wxEmptyString;
}

wxString CamuleRemoteGuiApp::CreateHTMLED2kLink(CAbstractFile const*f)
{
	wxString strCode = wxT("<a href=\"") + 
		CreateED2kLink(f) + wxT("\">") + 
		CleanupFilename(f->GetFileName(), true) + wxT("</a>");
	return strCode;
}


wxString validateURI(const wxString url)
{
	wxString strURI;
#if wxCHECK_VERSION_FULL(2,5,3,2)
	wxURI* uri = new wxURI(url);
	strURI=uri->BuildURI();
#else
	strURI=wxURL::ConvertToValidURI(url);
	// The following cause problems, so we escape them
	strURI.Replace(wxT("\""), wxT("%22")); 
	strURI.Replace(wxT("'"),  wxT("%27")); 
	strURI.Replace(wxT("`"),  wxT("%60")); 
#endif
	return strURI;
}

wxString CamuleRemoteGuiApp::GenFakeCheckUrl(const CAbstractFile *f)
{
	wxString strURL = wxT("http://donkeyfakes.gambri.net/index.php?action=search&ed2k=");
	strURL = validateURI( strURL +  CreateED2kLink( f ) );
	return strURL;
}

// jugle.net fake check
wxString CamuleRemoteGuiApp::GenFakeCheckUrl2(const CAbstractFile *f)
{
	wxString strURL = wxT("http://www.jugle.net/?fakecheck=%s");
	strURL = validateURI( strURL +  CreateED2kLink( f ) );
	return strURL;
}

bool CamuleRemoteGuiApp::AddServer(CServer *)
{
	// FIXME: add remote command
	return true;
}

void CamuleRemoteGuiApp::NotifyEvent(const GUIEvent& event)
{
	switch (event.ID) {
	        case SEARCH_ADD_TO_DLOAD:
			break;

	        case PARTFILE_REMOVE_NO_NEEDED:
			break;
	        case PARTFILE_REMOVE_FULL_QUEUE:
			break;
	        case PARTFILE_REMOVE_HIGH_QUEUE:
			break;
	        case PARTFILE_CLEANUP_SOURCES:
			break;
	        case PARTFILE_SWAP_A4AF_THIS:
			break;
        	case PARTFILE_SWAP_A4AF_OTHERS:
			break;
	        case PARTFILE_SWAP_A4AF_THIS_AUTO:
			break;
	        case PARTFILE_PAUSE:
	        	downloadqueue->Pause((CPartFile *)event.ptr_value);
				break;
	        case PARTFILE_RESUME:
	        	downloadqueue->Resume((CPartFile *)event.ptr_value);
				break;
	        case PARTFILE_STOP:
	        	downloadqueue->Stop((CPartFile *)event.ptr_value);
				break;
	        case PARTFILE_PRIO_AUTO:
	        	downloadqueue->AutoPrio((CPartFile *)event.ptr_value, event.long_value);
				break;
	        case PARTFILE_PRIO_SET:
	        	downloadqueue->AutoPrio((CPartFile *)event.ptr_value, event.long_value);
				break;
	        case PARTFILE_SET_CAT:
	        	downloadqueue->Category((CPartFile *)event.ptr_value, event.byte_value);
				break;
	        case PARTFILE_DELETE:
		        downloadqueue->Delete((CPartFile *)event.ptr_value);
				break;
			
	        case KNOWNFILE_SET_UP_PRIO:
			break;
	        case KNOWNFILE_SET_UP_PRIO_AUTO:
			break;
	        case KNOWNFILE_SET_COMMENT:
			break;

			// download queue
	        case DLOAD_SET_CAT_PRIO:
			break;
	        case DLOAD_SET_CAT_STATUS:
			break;

			case ADDLOGLINE:
			case ADDDEBUGLOGLINE:
				printf("LOG: %s\n", event.string_value.GetData());
				break;
			default:
				printf("ERROR: bad event %d\n", event.ID);
				wxASSERT(0);
	}
}

CPreferencesRem::CPreferencesRem(CRemoteConnect *conn)
{
	m_conn = conn;

	CPreferences::BuildItemList( theApp.ConfigDir);
	CPreferences::LoadAllItems( wxConfigBase::Get() );
}

bool CPreferencesRem::LoadRemote()
{
	//
	// override local settings with remote
	CECPacket req(EC_OP_GET_PREFERENCES);
	// bring them all !
	req.AddTag(CECTag(EC_TAG_SELECT_PREFS, (uint32)(0xffffffff)));
	CECPacket *prefs = m_conn->SendRecv(&req);
	
	if ( !prefs ) {
		return false;
	}
	CECTag *curr;
	if ( (curr = prefs->GetTagByName(EC_TAG_PREFS_CATEGORIES)) != 0 ) {
		// start from '1' to skip default category "all"
		for (int i = 1; i < curr->GetTagCount(); i++) {
			CECTag *cat_tag = curr->GetTagByIndex(i);
			Category_Struct *cat = new Category_Struct;
			cat->title = cat_tag->GetTagByName(EC_TAG_CATEGORY_TITLE)->GetStringData();
			cat->incomingpath = cat_tag->GetTagByName(EC_TAG_CATEGORY_PATH)->GetStringData();
			cat->comment = cat_tag->GetTagByName(EC_TAG_CATEGORY_COMMENT)->GetStringData();
			cat->color =  cat_tag->GetTagByName(EC_TAG_CATEGORY_COLOR)->GetInt32Data();
			cat->prio = cat_tag->GetTagByName(EC_TAG_CATEGORY_PRIO)->GetInt8Data();
			theApp.glob_prefs->AddCat(cat);
			// update gui
			theApp.amuledlg->transferwnd->AddCategory(cat);
		}
	}
	delete prefs;
	return true;
}

//
// Container implementation
//
CServerConnectRem::CServerConnectRem(CRemoteConnect *conn)
{
	m_Conn = conn;
}

void CServerConnectRem::ConnectToAnyServer()
{
	CECPacket req(EC_OP_SERVER_CONNECT);
	m_Conn->Send(&req);
}

void CServerConnectRem::StopConnectionTry()
{
	// lfroen: isn't Disconnect the same ?
}

void CServerConnectRem::Disconnect()
{
	CECPacket req(EC_OP_SERVER_DISCONNECT);
	m_Conn->Send(&req);
}

void CServerConnectRem::ConnectToServer(CServer *server)
{
	CECPacket req(EC_OP_SERVER_CONNECT);
	uint32 ip = server->GetIP();
	uint16 port = server->GetPort();
	req.AddTag(CECTag(EC_TAG_SERVER, EC_IPv4_t(ip, port)));
	m_Conn->Send(&req);
}

bool CServerConnectRem::ReQuery()
{
	CECPacket stat_req(EC_OP_STAT_REQ);
	CECPacket *stats = m_Conn->SendRecv(&stat_req);
	if ( !stats ) {
		return false;
	}
	CEC_ConnState_Tag *tag = (CEC_ConnState_Tag *)stats->GetTagByName(EC_TAG_CONNSTATE);
    if (!tag) {
            return false;
    }

    CServer *server;
	m_ID = tag->ClientID();
	switch (m_ID) {
		case 0:  // not connected
	    case 0xffffffff: // connecting
			theApp.amuledlg->ShowConnectionState(false);
	    	if ( m_CurrServer ) {
	    		theApp.amuledlg->serverwnd->serverlistctrl->HighlightServer(m_CurrServer, false);
	    		m_CurrServer = 0;
	    	}
	    	break;
	    default: {// connected
		    	CECTag *srvtag = tag->GetTagByIndex(0);
		    	if ( !srvtag ) {
		    		return false;
		    	}
		    	server = theApp.serverlist->GetByID(srvtag->GetIPv4Data().IP());
		    	if ( m_CurrServer && (server != m_CurrServer) ) {
		    		theApp.amuledlg->serverwnd->serverlistctrl->HighlightServer(m_CurrServer, false);
		    	}
		    	theApp.amuledlg->serverwnd->serverlistctrl->HighlightServer(server, true);
		    	
				theApp.amuledlg->ShowConnectionState(true,
					server->GetListName() + wxT(" ") + server->GetAddress());
		    	break;
	    }
	}
	return true;
}

CServerListRem::CServerListRem(CRemoteConnect *conn) : CRemoteContainer<CServer, uint32, CEC_Server_Tag>(conn)
{
}

void CServerListRem::UpdateServerMetFromURL(wxString url)
{
	// FIXME: add command
}

void CServerListRem::SaveServerMet()
{
	// lfroen: stub, nothing to do
}

void CServerListRem::RemoveServer(CServer* server)
{
	CECPacket req(EC_OP_SERVER_REMOVE);
	uint32 ip = server->GetIP();
	uint16 port = server->GetPort();
	req.AddTag(CECTag(EC_TAG_SERVER, EC_IPv4_t(ip, port)));
	m_conn->Send(&req);
}

CServer *CServerListRem::GetServerByAddress(const wxString& address, uint16 port)
{
	// FIXME: add code, nothing special
	return 0;
}

CServer *CServerListRem::CreateItem(CEC_Server_Tag *tag)
{
	return new CServer(tag);
}

void CServerListRem::DeleteItem(CServer *)
{
}

uint32 CServerListRem::GetItemID(CServer *server)
{
	return server->GetIP();
}

void CServerListRem::ProcessItemUpdate(CEC_Server_Tag *, CServer *)
{
	// server list is always realoaded from scratch
	wxASSERT(0);
}

void CServerListRem::ReloadControl()
{
	for(uint32 i = 0;i < GetCount(); i++) {
		CServer *srv = GetByIndex(i);
		theApp.amuledlg->serverwnd->serverlistctrl->RefreshServer(srv);
	}
	m_dirty = false;
}

void CIPFilterRem::Reload()
{
	CECPacket req(EC_OP_IPFILTER_RELOAD);
	m_conn->Send(&req);
}

void CIPFilterRem::Update(wxString /*url*/)
{
	// FIXME: add command
	wxASSERT(0);
}

CSharedFilesRem::CSharedFilesRem(CRemoteConnect *conn) : CRemoteContainer<CKnownFile, CMD4Hash, CEC_SharedFile_Tag>(conn)
{
}

void CSharedFilesRem::ReloadControl()
{
	if ( m_dirty ) {
		theApp.amuledlg->sharedfileswnd->sharedfilesctrl->DeleteAllItems();
	}
	for(uint32 i = 0;i < GetCount(); i++) {
		CKnownFile *f = GetByIndex(i);
		if ( m_dirty ) {
			theApp.amuledlg->sharedfileswnd->sharedfilesctrl->ShowFile(f);
		} else {
			theApp.amuledlg->sharedfileswnd->sharedfilesctrl->UpdateItem(f);
		}
	}
	m_dirty = false;
}

void CSharedFilesRem::Reload(bool, bool)
{

	//CECPacket req(EC_OP_SHAREDFILES_RELOAD);
}

void CSharedFilesRem::AddFilesFromDirectory(wxString)
{
	// should not get here. You can't do it remotely.
}

CKnownFile *CSharedFilesRem::CreateItem(CEC_SharedFile_Tag *tag)
{
	CKnownFile *file = new CKnownFile(tag);

	m_enc_map[file->GetFileHash()] = otherfunctions::RLE_Data(file->GetPartCount(), true);

	ProcessItemUpdate(tag, file);
	
	return file;
}

void CSharedFilesRem::DeleteItem(CKnownFile *file)
{
	m_enc_map.erase(file->GetFileHash());
}

CMD4Hash CSharedFilesRem::GetItemID(CKnownFile *file)
{
	return file->GetFileHash();
}

void CSharedFilesRem::ProcessItemUpdate(CEC_SharedFile_Tag *tag, CKnownFile *file)
{
	CECTag *parttag = tag->GetTagByName(EC_TAG_PARTFILE_PART_STATUS);
	const unsigned char *data = m_enc_map[file->GetFileHash()].Decode((unsigned char *)parttag->GetTagData(),
		parttag->GetTagDataLen());
	for(int i = 0;i < file->GetPartCount();i++) {
		file->m_AvailPartFrequency[i] = data[i];
	}
}

/*!
 * Connection to remote core
 * 
 */
CRemoteConnect::CRemoteConnect()
{
	m_ECSocket = new ECSocket;
	m_isConnected = false;
	m_busy = false;
}

bool CRemoteConnect::Connect(const wxString &host, int port,
	const wxString &WXUNUSED(login), const wxString &pass)
{
	wxIPV4address addr;

	addr.Hostname(host);
	addr.Service(port);

	m_ECSocket->Connect(addr, false);
	m_ECSocket->WaitOnConnect(10);

   if (!m_ECSocket->IsConnected()) {
            // no connection => close gracefully
            AddLogLineM(true, _("Connection Failed. Unable to connect to the specified host"));
            return false;
    } 
    // Authenticate ourselves
    CECPacket packet(EC_OP_AUTH_REQ);
    packet.AddTag(CECTag(EC_TAG_CLIENT_NAME, wxString(wxT("amule-remote"))));
    packet.AddTag(CECTag(EC_TAG_CLIENT_VERSION, wxString(wxT("0x0001"))));
    packet.AddTag(CECTag(EC_TAG_PROTOCOL_VERSION, (uint16)EC_CURRENT_PROTOCOL_VERSION));

	packet.AddTag(CECTag(EC_TAG_PASSWD_HASH, CMD4Hash(pass)));

#ifdef CVSDATE
	packet.AddTag(CECTag(EC_TAG_CVSDATE, wxT(CVSDATE)));
#endif

    if (! m_ECSocket->WritePacket(&packet) ) {
    	return false;
    }
    CECPacket *reply = m_ECSocket->ReadPacket();
    if (!reply) {
    	return false;
    }
	if (reply->GetOpCode() == EC_OP_AUTH_FAIL) {
		const CECTag *reason = reply->GetTagByName(EC_TAG_STRING);
		if (reason != NULL) {
			AddLogLineM(true, wxString(_("ExternalConn: Access denied because: ")) + 
				wxGetTranslation(reason->GetStringData()));
		} else {
		    AddLogLineM(true, _("ExternalConn: Access denied"));
		}
		delete reply;
		return false;
    } else if (reply->GetOpCode() != EC_OP_AUTH_OK) {
        AddLogLineM(true,_("ExternalConn: Bad reply from server. Connection closed.\n"));
		delete reply;
		return false;
    } else {
        m_isConnected = true;
        if (reply->GetTagByName(EC_TAG_SERVER_VERSION)) {
                AddLogLineM(true, _("Succeeded! Connection established to aMule ") +
                	reply->GetTagByName(EC_TAG_SERVER_VERSION)->GetStringData() + wxT("\n"));
        } else {
                AddLogLineM(true, _("Succeeded! Connection established.\n"));
        }
    }
    m_isConnected = true;
    m_ECSocket->SetFlags(wxSOCKET_BLOCK);
    
	return true;
}

CECPacket *CRemoteConnect::SendRecv(CECPacket *packet)
{
	m_busy = true;
    if (! m_ECSocket->WritePacket(packet) ) {
		m_busy = false;
    	return 0;
    }
    CECPacket *reply = m_ECSocket->ReadPacket();

	m_busy = false;
	return reply;
}

void CRemoteConnect::Send(CECPacket *packet)
{
	m_busy = true;
    if (! m_ECSocket->WritePacket(packet) ) {
		m_busy = false;
    	return;
    }
    CECPacket *reply = m_ECSocket->ReadPacket();

	m_busy = false;
    delete reply;
}

CUpQueueRem::CUpQueueRem(CRemoteConnect *conn) : CRemoteContainer<CUpDownClient, uint32, CEC_UpDownClient_Tag>(conn)
{
}

POSITION CUpQueueRem::GetFirstFromUploadList()
{
	it = m_items.begin();
	if ( it == m_items.end() ) {
		return 0;
	}
	POSITION pos;
	pos.m_ptr = (void *)&it;
	return pos;
}

CUpDownClient *CUpQueueRem::GetNextFromUploadList(POSITION &pos)
{
	std::list<CUpDownClient *>::iterator *i = (std::list<CUpDownClient *>::iterator *)pos.m_ptr;
	(*i)++;
	if ( *i == m_items.end() ) {
		pos = 0;
		return 0;
	}
	return *(*i);
}

// waiting list can be quite long. i see no point transferring it
POSITION CUpQueueRem::GetFirstFromWaitingList()
{
	POSITION pos;
	pos.m_ptr = NULL;
	return pos;
}

CUpDownClient *CUpQueueRem::GetNextFromWaitingList(POSITION &)
{
	return NULL;
}

CUpDownClient *CUpQueueRem::CreateItem(CEC_UpDownClient_Tag *)
{
	return 0;
}

void CUpQueueRem::DeleteItem(CUpDownClient *)
{
}

uint32 CUpQueueRem::GetItemID(CUpDownClient *)
{
	return 0;
}
void CUpQueueRem::ProcessItemUpdate(CEC_UpDownClient_Tag *, CUpDownClient *)
{
}

CDownQueueRem::CDownQueueRem(CRemoteConnect *conn) : CRemoteContainer<CPartFile, CMD4Hash, CEC_PartFile_Tag>(conn)
{
}

bool CDownQueueRem::AddED2KLink(const wxString &link, int)
{
	CECPacket req(EC_OP_ED2K_LINK);
	req.AddTag(CECTag(EC_TAG_STRING, link));
	
	m_conn->Send(&req);
	return true;
}

void CDownQueueRem::StopUDPRequests()
{
	// have no idea what is it about
}

void CDownQueueRem::ResetCatParts(int)
{
	// TODO: add command
	wxASSERT(0);
}

bool CDownQueueRem::IsPartFile(const CKnownFile *) const
{
	// hope i understand it right
	return true;
}

CPartFile *CDownQueueRem::CreateItem(CEC_PartFile_Tag *tag)
{
	CPartFile *file = new CPartFile(tag);
	m_enc_map[file->GetFileHash()] = otherfunctions::PartFileEncoderData(file->GetPartCount(), 10);
	ProcessItemUpdate(tag, file);
	
	theApp.amuledlg->transferwnd->downloadlistctrl->AddFile(file);
	return file;
}

void CDownQueueRem::DeleteItem(CPartFile *file)
{
	m_enc_map.erase(file->GetFileHash());
}

CMD4Hash CDownQueueRem::GetItemID(CPartFile *file)
{
	return file->GetFileHash();
}

void CDownQueueRem::ProcessItemUpdate(CEC_PartFile_Tag *tag, CPartFile *file)
{
	//
	// update status
	//
	file->kBpsDown = tag->Speed() / 1024.0;

	if ( file->kBpsDown > 0 ) {
		file->transfered = tag->SizeXfer();
		file->percentcompleted = (100.0*file->completedsize) / file->m_nFileSize;
		file->completedsize = tag->SizeDone();
	}

	file->transferingsrc = tag->SourceXferCount();
	file->m_notCurrentSources = tag->SourceNotCurrCount();
	file->m_source_count = tag->SourceCount();
	file->m_a4af_source_count = tag->SourceCountA4AF();
    file->status = tag->FileStatus();

	file->m_category = tag->FileCat();
	
	//
	// Copy part/gap status
	//
	CECTag *gaptag = tag->GetTagByName(EC_TAG_PARTFILE_GAP_STATUS);
	CECTag *parttag = tag->GetTagByName(EC_TAG_PARTFILE_PART_STATUS);
	CECTag *reqtag = tag->GetTagByName(EC_TAG_PARTFILE_REQ_STATUS);
	if (gaptag && parttag && reqtag) {
		POSITION curr_pos;

		wxASSERT(m_enc_map.count(file->GetFileHash()));
		
		otherfunctions::PartFileEncoderData &encoder = m_enc_map[file->GetFileHash()];
		encoder.Decode(
			(unsigned char *)gaptag->GetTagData(), gaptag->GetTagDataLen(),
			(unsigned char *)parttag->GetTagData(), parttag->GetTagDataLen());
			
		const Gap_Struct *reqparts = (const Gap_Struct *)reqtag->GetTagData();
		int reqcount = reqtag->GetTagDataLen() / sizeof(Gap_Struct);
		
		// adjust size of gaplist to reqcount
		int gap_size = encoder.m_gap_status.Size() / (2 * sizeof(uint32));
		while ( file->gaplist.GetCount() > gap_size ) {
			file->gaplist.RemoveHead();
		}
		while ( file->gaplist.GetCount() != gap_size ) {
			file->gaplist.AddHead(new Gap_Struct);
		}
		const uint32 *gap_info = (const uint32 *)encoder.m_gap_status.Buffer();
		curr_pos = file->gaplist.GetHeadPosition();
		for (int j = 0; j < gap_size;j++) {
			Gap_Struct* gap = file->gaplist.GetNext(curr_pos);
			gap->start = ntohl(gap_info[2*j]);
			gap->end = ntohl(gap_info[2*j+1]);
		}
		
		// adjust size of requested block list
		while ( file->requestedblocks_list.GetCount() > reqcount ) {
			file->requestedblocks_list.RemoveHead();
		}
		while ( file->requestedblocks_list.GetCount() != reqcount ) {
			file->requestedblocks_list.AddHead(new Requested_Block_Struct);
		}
		curr_pos = file->requestedblocks_list.GetHeadPosition();
		for (int i = 0; i < reqcount;i++) {
			Requested_Block_Struct* block = file->requestedblocks_list.GetNext(curr_pos);
			block->StartOffset = ntohl(reqparts[i].start);
			block->EndOffset = ntohl(reqparts[i].end);
		}
		// copy parts frequency
		const unsigned char *part_info = encoder.m_part_status.Buffer();
		for(int i = 0;i < file->GetPartCount();i++) {
			file->m_SrcpartFrequency[i] = part_info[i];
		}
	} else {
		printf("ERROR: %p %p %p\n", gaptag, parttag, reqtag);
	}
	theApp.amuledlg->transferwnd->downloadlistctrl->UpdateItem(file);
}

void CDownQueueRem::Pause(CPartFile *file)
{
	CECPacket req(EC_OP_PARTFILE_PAUSE);
	req.AddTag(CECTag(EC_TAG_PARTFILE, file->GetFileHash()));
	
	m_conn->Send(&req);
}

void CDownQueueRem::Resume(CPartFile *file)
{
	CECPacket req(EC_OP_PARTFILE_RESUME);
	req.AddTag(CECTag(EC_TAG_PARTFILE, file->GetFileHash()));
	
	m_conn->Send(&req);
}

void CDownQueueRem::Stop(CPartFile *file)
{
	CECPacket req(EC_OP_PARTFILE_STOP);
	req.AddTag(CECTag(EC_TAG_PARTFILE, file->GetFileHash()));
	
	m_conn->Send(&req);
}

void CDownQueueRem::Delete(CPartFile *file)
{
	CECPacket req(EC_OP_PARTFILE_DELETE);
	req.AddTag(CECTag(EC_TAG_PARTFILE, file->GetFileHash()));
	
	m_conn->Send(&req);
}

void CDownQueueRem::Prio(CPartFile *file, uint8 prio)
{
	CECPacket req(EC_OP_PARTFILE_PRIO_SET);
	
	CECTag hashtag(EC_TAG_PARTFILE, file->GetFileHash());
	hashtag.AddTag(CECTag(EC_TAG_PARTFILE_PRIO, prio));
	
	m_conn->Send(&req);
}

void CDownQueueRem::AutoPrio(CPartFile *file, bool flag)
{
	CECPacket req(EC_OP_PARTFILE_PRIO_SET);
	
	CECTag hashtag(EC_TAG_PARTFILE, file->GetFileHash());
	
	hashtag.AddTag(CECTag(EC_TAG_PARTFILE_PRIO, (uint8)(flag ? PR_AUTO : file->GetDownPriority())));
	
	m_conn->Send(&req);
}

void CDownQueueRem::Category(CPartFile *file, uint8 cat)
{
	CECPacket req(EC_OP_PARTFILE_SET_CAT);
	file->m_category = cat;
	
	CECTag hashtag(EC_TAG_PARTFILE, file->GetFileHash());
	hashtag.AddTag(CECTag(EC_TAG_PARTFILE_CAT, cat));
	
	m_conn->Send(&req);
}


CClientListRem::CClientListRem(CRemoteConnect *conn)
{
	m_conn = conn;
}

void CClientListRem::FilterQueues()
{
	// FIXME: add code
	wxASSERT(0);
}

CSearchListRem::CSearchListRem(CRemoteConnect *conn) : CRemoteContainer<CSearchFile, CMD4Hash, CEC_SearchFile_Tag>(conn)
{
	m_curr_search = 0;
}

void CSearchListRem::Clear()
{
	// FIXME: add code
	wxASSERT(0);
}

bool CSearchListRem::StartNewSearch(long nSearchID, bool global_search, wxString &searchString,
	wxString& typeText, wxString &extension, uint32 min_size, uint32 max_size, uint32 availability)
{
	CECPacket search_req(EC_OP_SEARCH_START);
	search_req.AddTag(CEC_Search_Tag (searchString,
		global_search ? EC_SEARCH_GLOBAL : EC_SEARCH_LOCAL, typeText,
		extension, availability, min_size, max_size));
		
	m_conn->Send(&search_req);
	m_curr_search = nSearchID;
	return true;
}

void CSearchListRem::StopGlobalSearch()
{
	//CECPacket search_req(EC_OP_SEARCH_STOP);
	//m_conn->Send(&search_req);
}

CSearchFile::CSearchFile(CEC_SearchFile_Tag *tag)
{
	m_strFileName = tag->FileName();
	m_abyFileHash = tag->ID();
	m_nFileSize = tag->SizeFull();

	m_nSearchID = theApp.searchlist->m_curr_search;

}

// dtor is virtual - must be implemented
CSearchFile::~CSearchFile()
{	
}

CSearchFile *CSearchListRem::CreateItem(CEC_SearchFile_Tag *tag)
{
	CSearchFile *file = new CSearchFile(tag);

	theApp.amuledlg->searchwnd->AddResult(file);
	
	return file;
}

void CSearchListRem::DeleteItem(CSearchFile *)
{
}

CMD4Hash CSearchListRem::GetItemID(CSearchFile *file)
{
	return file->GetFileHash();
}

void CSearchListRem::ProcessItemUpdate(CEC_SearchFile_Tag *, CSearchFile *)
{
}

bool CUpDownClient::IsBanned() const
{
	// FIXME: add code
	return false;
}

//
// Those functions have different implementation in remote gui
//
void  CUpDownClient::Ban()
{
	// FIXME: add code
	wxASSERT(0);
}

void  CUpDownClient::UnBan()
{
	// FIXME: add code
	wxASSERT(0);
}

void CUpDownClient::RequestSharedFileList()
{
	// FIXME: add code
	wxASSERT(0);
}

void CKnownFile::SetFileComment(const wxString &)
{
	// FIXME: add code
	wxASSERT(0);
}

void CKnownFile::SetFileRate(unsigned char)
{
	// FIXME: add code
	wxASSERT(0);
}

// I don't think it will be implemented - too match data transfer. But who knows ?
wxString CUpDownClient::ShowDownloadingParts() const
{
	return wxEmptyString;
}
bool CUpDownClient::SwapToAnotherFile(bool bIgnoreNoNeeded, bool ignoreSuspensions,
										bool bRemoveCompletely, CPartFile* toFile)
{
	// FIXME: add code
	wxASSERT(0);
	return false;
}
//
// Those functions are virtual. So even they don't get called they must
// be defined so linker will be happy
//
CPacket* CKnownFile::CreateSrcInfoPacket(CUpDownClient const*)
{
	wxASSERT(0);
	return 0;
}

bool CKnownFile::LoadFromFile(class CFileDataIO const*)
{
	wxASSERT(0);
	return false;
}

void CKnownFile::UpdatePartsInfo()
{
	wxASSERT(0);
}

void CKnownFile::SetFileSize(uint32)
{
	wxASSERT(0);
}

CPacket* CPartFile::CreateSrcInfoPacket(CUpDownClient const*)
{
	wxASSERT(0);
	return 0;
}

void CPartFile::UpdatePartsInfo()
{
	wxASSERT(0);
}

void CPartFile::UpdateFileRatingCommentAvail()
{
	wxASSERT(0);
}

bool CPartFile::SavePartFile(bool)
{
	wxASSERT(0);
	return false;
}

//
// since gui is not linked with amule.cpp - define events here
//
DEFINE_EVENT_TYPE(wxEVT_NOTIFY_EVENT)
DEFINE_EVENT_TYPE(wxEVT_AMULE_TIMER)

DEFINE_EVENT_TYPE(wxEVT_CORE_FILE_HASHING_FINISHED)
DEFINE_EVENT_TYPE(wxEVT_CORE_FILE_HASHING_SHUTDOWN)
DEFINE_EVENT_TYPE(wxEVT_CORE_FINISHED_FILE_COMPLETION)
DEFINE_EVENT_TYPE(wxEVT_CORE_FINISHED_HTTP_DOWNLOAD)
DEFINE_EVENT_TYPE(wxEVT_CORE_SOURCE_DNS_DONE)
DEFINE_EVENT_TYPE(wxEVT_CORE_UDP_DNS_DONE)
DEFINE_EVENT_TYPE(wxEVT_CORE_SERVER_DNS_DONE)
