//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Name:         wxCasFrame Class
///
/// Purpose:      wxCas main frame
///
/// Author:       ThePolish <thepolish@vipmail.ru>
///
/// Copyright (C) 2004 by ThePolish
///
/// Derived from CAS by Pedro de Oliveira <falso@rdk.homeip.net>
///
/// Pixmats from aMule http://www.amule.org
///
/// This program is free software; you can redistribute it and/or modify
///  it under the terms of the GNU General Public License as published by
/// the Free Software Foundation; either version 2 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the
/// Free Software Foundation, Inc.,
/// 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _WXCASFRAME_H
#define _WXCASFRAME_H

#ifdef __GNUG__
#pragma interface "wxcasframe.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h"
#include "wx/wxprec.h"

#ifdef __BORLANDC__
 #pragma hdrstop
#endif

// For all others, include the necessary headers
#ifndef WX_PRECOMP
 #include "wx/wx.h"
#endif

#include <wx/statline.h>
#include <wx/toolbar.h>
#include <wx/timer.h>
#include <wx/filename.h>

#include "onlinesig.h"


#ifdef __LINUX__		// System monitoring on Linux
#include "linuxmon.h"
#endif

/// Main wxCas Frame
class WxCasFrame: public wxFrame
{
private:
	wxToolBar *m_toolbar;
	wxBitmap m_toolBarBitmaps[ 6 ];

	wxBoxSizer *m_frameVBox;
	wxBoxSizer *m_mainPanelVBox;

	wxPanel *m_mainPanel;

	wxStaticLine *m_staticLine;
#ifdef __WXMSW__

	wxStaticLine *m_BottomStaticLine;
#endif

	wxStaticBox *m_sigPanelSBox;
	wxStaticBoxSizer *m_sigPanelSBoxSizer;

	wxStaticBox *m_monPanelSBox;
	wxStaticBoxSizer *m_monPanelSBoxSizer;

	wxStaticBox *m_hitPanelSBox;
	wxStaticBoxSizer *m_hitPanelSBoxSizer;

	wxStaticText *m_statLine_1;
	wxStaticText *m_statLine_2;
	wxStaticText *m_statLine_3;
	wxStaticText *m_statLine_4;
	wxStaticText *m_statLine_5;
	wxStaticText *m_statLine_6;

	wxStaticText *m_hitLine;
	wxButton *m_hitButton;

	wxTimer * m_refresh_timer;
	wxTimer * m_ftp_update_timer;

	OnLineSig *m_aMuleSig;
	unsigned int m_maxLineCount;

#ifdef __LINUX__		// System monitoring on Linux

	wxStaticText *m_sysLine_1;
	wxStaticText *m_sysLine_2;
	LinuxMon *m_sysMonitor;
#endif

	enum
	{
	    ID_BAR_REFRESH = 1000,
	    ID_BAR_SAVE,
	    ID_BAR_PRINT,
	    ID_BAR_PREFS,
	    ID_BAR_ABOUT,
	    ID_REFRESH_TIMER,
	    ID_FTP_UPDATE_TIMER,
	    ID_HIT_BUTTON
	};

	// Get maximum of 2 uint
	unsigned int GetMaxUInt( const unsigned int a, const unsigned int b )
	{
		return ( ( a ) > ( b ) ? ( a ) : ( b ) );
	}

protected:
	bool UpdateStatsPanel ();
	void UpdateAll ( bool forceFitting = FALSE );

	void OnBarRefresh ( wxCommandEvent & event );
	void OnBarAbout ( wxCommandEvent & event );
	void OnBarSave ( wxCommandEvent & event );
	void OnBarPrint ( wxCommandEvent & event );
	void OnBarPrefs ( wxCommandEvent & event );
	void OnRefreshTimer ( wxTimerEvent & event );
	void OnFtpUpdateTimer ( wxTimerEvent & event );
	void OnHitButton ( wxCommandEvent & event );

	DECLARE_EVENT_TABLE ();

public:
	/// Constructor
	WxCasFrame ( const wxString& title );

	/// Destructor
	~WxCasFrame ();

	/// Get Online statistics image
	wxImage *GetStatImage () const;

	/// Refresh timer period changing
	bool ChangeRefreshPeriod( const int newPeriod );

	/// Refresh timer period changing
	bool ChangeFtpUpdatePeriod( const int newPeriod );

	/// Set amulesig.dat file
	void SetAmuleSigFile( const wxFileName& file );
};

#endif /* _WXCASFRAME_H */
