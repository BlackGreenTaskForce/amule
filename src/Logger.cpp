#include "Logger.h"
#include "amule.h"
#include "Preferences.h"

#include <wx/thread.h>


CDebugCategory::CDebugCategory( DebugType type, const wxString& name )
	: m_name( name ),
	  m_type( type )
{
	m_enabled = false;
}


bool CDebugCategory::IsEnabled() const
{
	return m_enabled;
}


void CDebugCategory::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}


const wxString& CDebugCategory::GetName() const
{
	return m_name;
}


DebugType CDebugCategory::GetType() const
{
	return m_type;
}








CDebugCategory g_debugcats[] = {
	CDebugCategory( logGeneral,			wxT("General") ),
	CDebugCategory( logHasher,			wxT("Hasher") ),
	CDebugCategory( logClient,			wxT("ED2k Client") ),
	CDebugCategory( logLocalClient,		wxT("Local Client Protocol") ),
	CDebugCategory( logRemoteClient,	wxT("Remote Client Protocl") ),
	CDebugCategory( logPacketErrors,	wxT("Packet Parsing Errors") ),
	CDebugCategory( logCFile,			wxT("The CFile Class") ),
	CDebugCategory( logFileIO,			wxT("FileIO") ),
	CDebugCategory( logZLib,			wxT("ZLib") ),
	CDebugCategory( logAICHThread,		wxT("AICH-Hasher") ),
	CDebugCategory( logAICHTransfer,	wxT("AICH-Transfer") ),
	CDebugCategory( logAICHRecovery,	wxT("AICH-Recovery") ),
	CDebugCategory( logListenSocket,	wxT("ListenSocket") ),
	CDebugCategory( logCredits,			wxT("Credits") ),
	CDebugCategory( logClientUDP,		wxT("ClientUDPSocket") ),
	CDebugCategory( logDownloadQueue,	wxT("DownloadQueue") ),
	CDebugCategory( logIPFilter,		wxT("IPFilter") ),
	CDebugCategory( logKnownFiles,		wxT("KnownFileList") ),
	CDebugCategory( logPartFile,		wxT("PartFiles") ),
	CDebugCategory( logSHAHashSet,		wxT("SHAHashSet") ),
	CDebugCategory( logServer,			wxT("Servers") )
};


const int categoryCount = sizeof( g_debugcats ) / sizeof( g_debugcats[0] );



bool CLogger::IsEnabled( DebugType type )
{
#ifdef __VERBOSE_OUTPUT__
	int index = (int)type;
	
	if ( index >= 0 && index <= categoryCount ) {
		const CDebugCategory& cat = g_debugcats[ index ];
		wxASSERT( type == cat.GetType() );

		return ( cat.IsEnabled() && thePrefs::GetVerbose() );
	} 

	wxASSERT( false );
#endif
	return false;
}


void CLogger::SetEnabled( DebugType type, bool enabled ) 
{
	int index = (int)type;
	
	if ( index >= 0 && index <= categoryCount ) {
		CDebugCategory& cat = g_debugcats[ index ];
		wxASSERT( type == cat.GetType() );

		cat.SetEnabled( enabled );
	} else {
		wxASSERT( false );
	}
}


void CLogger::AddLogLine( bool critical, const wxString str )
{
	GUIEvent event( ADDLOGLINE, critical, str );
	
	if ( wxThread::IsMain() ) {
		theApp.NotifyEvent( event );
	} else {
		wxPostEvent( &theApp, event );
	}
}


void CLogger::AddDebugLogLine( bool critical, DebugType type, const wxString& str )
{
	int index = (int)type;
	
	if ( index >= 0 && index <= categoryCount ) {
		const CDebugCategory& cat = g_debugcats[ index ];
		wxASSERT( type == cat.GetType() );

		wxString line = cat.GetName() + wxT(": ") + str;
		
#if __VERBOSE_OUTPUT__
		GUIEvent event( ADDDEBUGLOGLINE, critical, str );
		
		if ( wxThread::IsMain() ) {
			theApp.NotifyEvent( event );
		} else {
			wxPostEvent( &theApp, event );
		}
#else
		printf("%s\n", unicode2char( line ) );
#endif
	} else {
		wxASSERT( false );
	}
}


const CDebugCategory& CLogger::GetDebugCategory( int index )
{
	wxASSERT( index >= 0 && index <= categoryCount );

	return g_debugcats[ index ];
}


unsigned int CLogger::GetDebugCategoryCount()
{
	return categoryCount;
}




