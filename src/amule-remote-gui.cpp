#include <unistd.h>			// Needed for close(2) and sleep(3)
#include <wx/defs.h>
#include <wx/gauge.h>
#include <wx/textctrl.h>

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

#include "muuli_wdr.h"			// Needed for IDs
#include "amuleDlg.h"			// Needed for CamuleDlg
#include "SearchDlg.h"			// Needed for CSearchDlg
#include "SharedFilesCtrl.h"		// Needed for CSharedFilesCtrl
#include "DownloadListCtrl.h"		// Needed for CDownloadListCtrl
#include "ClientListCtrl.h"

#include "ECSocket.h"
#include "ECPacket.h"
#include "ECcodes.h"

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
	sharedfiles->DoRequery(EC_OP_GET_SHARED_FILES, EC_TAG_KNOWNFILE);
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

	glob_prefs = new CPreferencesRem(0);
	
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

	CRemoteConnect *connect = new CRemoteConnect;
	
	statistics = new CStatisticsRem();
	
	clientlist = new CClientListRem(connect);
	searchlist = new CSearchListRem(connect);
	//knownfiles = new CKnownFilesRem(connect);
	serverlist = new CServerListRem(connect);
	
	sharedfiles	= new CSharedFilesRem(connect);
	clientcredits = new CClientCreditsRem();
	
	// bugfix - do this before creating the uploadqueue
	downloadqueue = new CDownQueueRem(connect);
	uploadqueue = new CUpQueueRem(connect);
	ipfilter = new CIPFilterRem();


	// Create main dialog
	InitGui(0, geom_string);

	connect->Connect("localhost", 4712);
	
	//m_app_state = APP_STATE_RUNNING;
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

// remote gui doesn't have it's own log
void CamuleRemoteGuiApp::QueueLogLine(bool, wxString const&)
{
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

void CamuleRemoteGuiApp::NotifyEvent(GUIEvent event)
{
	switch (event.ID) {
	        case SEARCH_REQ:
			break;
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
			break;
	        case PARTFILE_RESUME:
			break;
	        case PARTFILE_STOP:
			break;
	        case PARTFILE_PRIO_AUTO:
			break;
	        case PARTFILE_PRIO_SET:
			break;
	        case PARTFILE_SET_CAT:
			break;
	        case PARTFILE_DELETE:
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

CServer *CServerConnectRem::GetCurrentServer()
{
	// lfroen: must find out how to do such
	return 0;
}

CServerListRem::CServerListRem(CRemoteConnect *conn) : CRemoteContainer<CServer, uint32, CEC_Server_Tag>(conn)
{
}

void CServerListRem::UpdateServerMetFromURL(wxString url)
{
	// FIXME: add command
}

void CServerListRem::SaveServermetToFile()
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

CServer *CServerListRem::CreateItem(CEC_Server_Tag *)
{
	return 0;
}

void CServerListRem::DeleteItem(CServer *)
{
}

uint32 CServerListRem::GetItemID(CServer *)
{
	return 0;
}

void CServerListRem::ProcessItemUpdate(CEC_Server_Tag *, CServer *)
{
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

void CSharedFilesRem::Reload(bool, bool)
{
	CECPacket req(EC_OP_SHAREDFILES_RELOAD);
	m_conn->Send(&req);
}

void CSharedFilesRem::AddFilesFromDirectory(wxString)
{
	// should not get here. You can't do it remotely.
}

CKnownFile *CSharedFilesRem::CreateItem(CEC_SharedFile_Tag *)
{
	return 0;
}

void CSharedFilesRem::DeleteItem(CKnownFile *)
{
}

CMD4Hash CSharedFilesRem::GetItemID(CKnownFile *)
{
}

void CSharedFilesRem::ProcessItemUpdate(CEC_SharedFile_Tag *, CKnownFile *)
{
}

/*!
 * Connection to remote core
 * 
 */
CRemoteConnect::CRemoteConnect()
{
	m_ECSocket = new ECSocket;
	m_isConnected = false;
}

bool CRemoteConnect::Connect(const char *host, int port)
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
    packet.AddTag(CECTag(EC_TAG_CLIENT_NAME, wxString("amule-remote")));
    packet.AddTag(CECTag(EC_TAG_CLIENT_VERSION, wxString("0x0001")));
    packet.AddTag(CECTag(EC_TAG_PROTOCOL_VERSION, (uint16)0x01f1));

    wxString pass_hash = "81dc9bdb52d04dc20036dbd8313ed055";
	packet.AddTag(CECTag(EC_TAG_PASSWD_HASH, pass_hash));

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
    } else if (reply->GetOpCode() != EC_OP_AUTH_OK) {
        AddLogLineM(true,_("ExternalConn: Bad reply from server. Connection closed.\n"));
    } else {
        m_isConnected = true;
        if (reply->GetTagByName(EC_TAG_SERVER_VERSION)) {
                AddLogLineM(true, _("Succeeded! Connection established to aMule ") +
                	reply->GetTagByName(EC_TAG_SERVER_VERSION)->GetStringData() + wxT("\n"));
        } else {
                AddLogLineM(true, _("Succeeded! Connection established.\n"));
        }
    }

}

CECPacket *CRemoteConnect::SendRecv(CECPacket *packet)
{
    if (! m_ECSocket->WritePacket(packet) ) {
    	return 0;
    }
    CECPacket *reply = m_ECSocket->ReadPacket();

	return reply;
}

void CRemoteConnect::Send(CECPacket *)
{
}

CUpQueueRem::CUpQueueRem(CRemoteConnect *conn) : CRemoteContainer<CUpDownClient, uint32, CEC_UpDownClient_Tag>(conn)
{
}

POSITION CUpQueueRem::GetFirstFromUploadList()
{
	it = m_items.begin();
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
	return false;
}

CPartFile *CDownQueueRem::CreateItem(CEC_PartFile_Tag *)
{
}

void CDownQueueRem::DeleteItem(CPartFile *)
{
}
CMD4Hash CDownQueueRem::GetItemID(CPartFile *)
{
}
void CDownQueueRem::ProcessItemUpdate(CEC_PartFile_Tag *, CPartFile *)
{
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
}

void CSearchListRem::Clear()
{
	// FIXME: add code
	wxASSERT(0);
}

void CSearchListRem::NewSearch(wxString type, uint32 search_id)
{
	// FIXME: add code
	wxASSERT(0);
}

void CSearchListRem::StopGlobalSearch()
{
	// FIXME: add code
	wxASSERT(0);
}

const std::multimap<uint32, CUpDownClient*>& CClientListRem::GetClientList()
{
	std::multimap<uint32, CUpDownClient*> dummy;
	
	return dummy;
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
