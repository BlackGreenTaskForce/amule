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

// handling incoming connections (up or downloadrequests)

#ifndef LISTENSOCKET_H
#define LISTENSOCKET_H


#include "types.h"		// Needed for uint8, uint16, uint32 and uint64
#include "EMSocket.h"		// Needed for CEMSocket
#include "CTypedPtrList.h"
#include <wx/dynarray.h>

#include <map> 

WX_DECLARE_OBJARRAY(wxString, ArrayOfwxStrings);

class CUpDownClient;
class CPacket;
class CTimerWnd;
class CPreferences;

// CClientReqSocket;

class CClientReqSocketHandler;

class CClientReqSocket : public CEMSocket
{
friend class CClientSocket;

	DECLARE_DYNAMIC_CLASS(CClientReqSocket)

	CClientReqSocket(); /*{};*/

public:
	CClientReqSocket(CPreferences* in_prefs, CUpDownClient* in_client = 0);	
	virtual ~CClientReqSocket();
	void		Disconnect(const wxString& strReason);


	void		ResetTimeOutTimer();
	bool		CheckTimeOut();
	void		Safe_Delete();

	bool		deletethis; // 0.30c (Creteil), set as bool

	bool		Create();
	void		OnClose(int nErrorCode);
	void		OnSend(int nErrorCode);
	void		OnReceive(int nErrorCode);
	void		OnError(int nErrorCode);
	void		OnConnect(int nErrorCode);
	virtual	void OnInit();
	virtual	bool Close(); /*	{return wxSocketBase::Close();}*/
	
	uint32		timeout_timer;
	bool		hotrank;
	CUpDownClient*	client;
	CPreferences* 	app_prefs;

protected:
	bool	 PacketReceived(Packet* packet);

private:
//	void	Delete_Timed();
	bool	ProcessPacket(const char *packet, uint32 size, uint8 opcode);
	bool	ProcessExtPacket(const char *packet, uint32 size, uint8 opcode);

	CClientReqSocketHandler* my_handler;
};


class CClientReqSocketHandler: public wxEvtHandler
{
public:
	CClientReqSocketHandler(CClientReqSocket* parent) {
		socket = parent;
	}

	CClientReqSocket* socket;

private:
	void ClientReqSocketHandler(wxSocketEvent& event);
	
	DECLARE_EVENT_TABLE()
};



//WX_DECLARE_LIST(CClientReqSocket,SocketListL);

// CListenSocket command target
class CListenSocket : public wxSocketServer{
	DECLARE_DYNAMIC_CLASS(CListenSocket)

public:
	CListenSocket() : wxSocketServer(happyCompiler) {};
	CListenSocket(CPreferences* in_prefs,wxSockAddress& addr);
	~CListenSocket();
	bool	StartListening();
	void	StopListening();
	virtual void OnAccept(int nErrorCode);
	void	Process();
	void	RemoveSocket(CClientReqSocket* todel);
	void	AddSocket(CClientReqSocket* toadd);
	uint16	GetOpenSockets()		{return socket_list.GetCount();}
	void	KillAllSockets();
	bool	TooManySockets(bool bIgnoreInterval = false);
	uint32	GetMaxConnectionReached()	{return maxconnectionreached;}
	bool    IsValidSocket(CClientReqSocket* totest);
	void	AddConnection();
	void	RecalculateStats();
	void	ReStartListening();
	void	UpdateConnectionsStatus();
	
	float	GetMaxConperFiveModifier();
	uint32	GetPeakConnections()		{ return peakconnections; }
	uint32	GetTotalConnectionChecks()	{ return totalconnectionchecks; }
	float	GetAverageConnections()		{ return averageconnections; }
	uint32	GetActiveConnections()		{ return activeconnections; }
	
private:
	bool bListening;
	CPreferences* app_prefs;
	CTypedPtrList<CPtrList, CClientReqSocket*> socket_list;
	//SocketListL socket_list;
	uint16 opensockets;
	uint16 m_OpenSocketsInterval;
	uint32 maxconnectionreached;
	wxIPV4address happyCompiler;
	uint16	m_ConnectionStates[3];
	uint16	m_nPeningConnections;
	uint32	peakconnections;
	uint32	totalconnectionchecks;
	float	averageconnections;
	uint32	activeconnections;

public:
	std::map<uint64, uint32> offensecounter;
	std::map<uint64, uint64> hashbase;  

};

#endif // LISTENSOCKET_H
