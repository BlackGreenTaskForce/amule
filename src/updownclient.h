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

#ifndef UPDOWNCLIENT_H
#define UPDOWNCLIENT_H

#include <wx/defs.h>		// Needed before any other wx/*.h
#include <wx/string.h>		// Needed for wxString
#include <wx/dcmemory.h>	// Needed for wxMemoryDC
#include <wx/gdicmn.h>		// Needed for wxRect
#include "types.h"		// Needed for int8, int16, uint8, uint16, uint32 and uint64
#include "CTypedPtrList.h"	// Needed for CTypedPtrList
#include "GetTickCount.h"	// Needed for GetTickCount
#include "CMD4Hash.h"
#include "otherfunctions.h"
#include <list>

class CPartFile;
class CClientReqSocket;
class CClientCredits;
class Packet;
class CFriend;
class Requested_Block_Struct;
class CKnownFile;
class Pending_Block_Struct;
class CSafeMemFile;
class CMemFile;
class Requested_File_Struct;
class TransferredData;

// uploadstate
#define	US_UPLOADING		0
#define	US_ONUPLOADQUEUE	1
#define	US_WAITCALLBACK		2
#define	US_CONNECTING		3
#define	US_PENDING		4
#define	US_LOWTOLOWIP		5
#define US_BANNED		6
#define US_ERROR		7
#define US_NONE			8

// downloadstate
#define	DS_DOWNLOADING		0
#define	DS_ONQUEUE		1
#define	DS_CONNECTED		2
#define	DS_CONNECTING		3
#define	DS_WAITCALLBACK		4
#define	DS_REQHASHSET		5
#define	DS_NONEEDEDPARTS	6
#define	DS_TOOMANYCONNS		7
#define	DS_LOWTOLOWIP		8
#define DS_BANNED		9
#define DS_ERROR		10
#define	DS_NONE			11

// m_byChatstate
#define	MS_NONE			0
#define	MS_CHATTING		1
#define	MS_CONNECTING		2
#define	MS_UNABLETOCONNECT	3

#if 0
// clientsoft
#define SO_EMULE		0
#define SO_CDONKEY		1
#define SO_LXMULE		2
#define SO_AMULE		3
#define	SO_SHAREAZA		4
#define SO_EDONKEYHYBRID	50
#define	SO_EDONKEY		51
#define SO_MLDONKEY		52
#define SO_NEW_MLDONKEY		152
#define SO_OLDEMULE		53
#define SO_UNKNOWN		54
#endif

enum EClientSoftware{
	SO_EMULE		= 0,
	SO_CDONKEY		= 1,
	SO_LXMULE		= 2,
	SO_AMULE		= 3,
	SO_SHAREAZA		= 4,
	SO_NEW2_MLDONKEY	= 10,
	SO_LPHANT		= 20,
	SO_EDONKEYHYBRID	= 50,
	SO_EDONKEY		= 51,
	SO_MLDONKEY		= 52,
	SO_OLDEMULE		= 53,
	SO_UNKNOWN		= 54,
	SO_NEW_MLDONKEY		= 152
};

// For ET_COMPATIBLE unknown
#define SO_COMPAT_UNK	0xFF

enum ESecureIdentState{
	IS_UNAVAILABLE		= 0,
	IS_ALLREQUESTSSEND	= 0,
	IS_SIGNATURENEEDED	= 1,
	IS_KEYANDSIGNEEDED	= 2,
};
enum EInfoPacketState{
	IP_NONE			= 0,
	IP_EDONKEYPROTPACK	= 1,
	IP_EMULEPROTPACK	= 2,
	IP_BOTH			= 3,
};



struct PartFileStamp {
	CPartFile* file;
	DWORD timestamp;
};

class CUpDownClient {
	friend class CUploadQueue;
public:
	//base
	CUpDownClient(CClientReqSocket* sender = 0);
	CUpDownClient(uint16 in_port, uint32 in_userid, uint32 in_serverup, uint16 in_serverport,CPartFile* in_reqfile);
	~CUpDownClient();
//	void		Destroy();
	bool		Disconnected(const wxString& strReason, bool bFromSocket = false);
	bool		TryToConnect(bool bIgnoreMaxCon = false);
	void		ConnectionEstablished();
	uint32		GetUserID() const		{ return m_nUserID; }
	void		SetUserID(uint32 nUserID)	{ m_nUserID = nUserID; }
	const wxString&	GetUserName() const		{ return m_Username; }
	uint32		GetIP() const 			{ return m_dwUserIP; }
	bool		HasLowID() const 		{ return (m_nUserID < 16777216); }
	const wxString&	GetFullIP() const		{ return m_FullUserIP; }
	uint32		GetUserPort() const		{ return m_nUserPort; }
	uint32		GetTransferedUp() const 	{ return m_nTransferedUp; }
	uint32		GetTransferedDown() const	{ return m_nTransferedDown; }
	uint32		GetServerIP() const		{ return m_dwServerIP; }
	void		SetServerIP(uint32 nIP)		{ m_dwServerIP = nIP; }
	uint16		GetServerPort()	const		{ return m_nServerPort; }
	void		SetServerPort(uint16 nPort)	{ m_nServerPort = nPort; }	
	const CMD4Hash&	GetUserHash() const		{ return m_UserHash; }
	void		SetUserHash(const CMD4Hash& userhash) { m_UserHash = userhash; ValidateHash(); }
	void		ValidateHash()			{ m_HasValidHash = !m_UserHash.IsEmpty(); }
	bool		HasValidHash() const		{ return m_HasValidHash; }
	uint32		GetVersion() const		{ return m_nClientVersion;}
	uint8		GetMuleVersion() const		{ return m_byEmuleVersion;}
	bool		ExtProtocolAvailable() const	{ return m_bEmuleProtocol;}
	bool		IsEmuleClient()	const		{ return m_byEmuleVersion;}
	CClientCredits*	Credits()			{ return credits;}
	bool		IsBanned() const
		{ return (m_bBanned && m_nDownloadState != DS_DOWNLOADING); }
	const wxString&	GetClientFilename() const	{ return ClientFilename; }
	bool		SupportsUDP() const		{ return m_byUDPVer != 0 && m_nUDPPort != 0; }
	uint16		GetUDPPort() const		{ return m_nUDPPort; }
	void		SetUDPPort(uint16 nPort)	{ m_nUDPPort = nPort; }
	uint8		GetUDPVersion() const		{ return m_byUDPVer; }
	uint8		GetExtendedRequestsVersion() const { return m_byExtendedRequestsVer; }
	bool		IsFriend() const 		{ return m_Friend != NULL; }
	float		GetCompression() const
		{ return (float)compressiongain / notcompressed * 100.0f; } // Add rod show compression
	void		ResetCompressionGain()
		{ compressiongain = 0; notcompressed = 1; } // Add show compression

	void		ClearDownloadBlockRequests();
	void		RequestSharedFileList();
	void		ProcessSharedFileList(const char* pachPacket, uint32 nSize, LPCTSTR pszDirectory = NULL);
	
	wxString	GetUploadFileInfo();
	
	void		CheckForGPLEvilDoer();

	void		SetUserName(const wxString& NewName) { m_Username = NewName; }
	
	uint8		GetClientSoft() const		{ return m_clientSoft; }
	void		ReGetClientSoft();
	bool		ProcessHelloAnswer(const char *pachPacket, uint32 nSize);
	bool		ProcessHelloPacket(const char *pachPacket, uint32 nSize);
	void		SendHelloAnswer();
	bool		SendHelloPacket();
	void		SendMuleInfoPacket(bool bAnswer);
	void		ProcessMuleInfoPacket(const char* pachPacket, uint32 nSize);
	void		ProcessMuleCommentPacket(const char *pachPacket, uint32 nSize);
	bool		Compare(const CUpDownClient* tocomp, bool bIgnoreUserhash = false);
	void		SetLastSrcReqTime()		{ m_dwLastSourceRequest = ::GetTickCount(); }
	void		SetLastSrcAnswerTime()		{ m_dwLastSourceAnswer = ::GetTickCount(); }
	void		SetLastAskedForSources()	{ m_dwLastAskedForSources = ::GetTickCount(); }
	uint32		GetLastSrcReqTime() const 	{ return m_dwLastSourceRequest; }
	uint32		GetLastSrcAnswerTime() const	{ return m_dwLastSourceAnswer; }
	uint32		GetLastAskedForSources() const	{ return m_dwLastAskedForSources; }
	DWORD		GetEnteredConnectedState() const{ return m_dwEnteredConnectedState; }
	bool		GetFriendSlot() const 		{ return m_bFriendSlot; }
	void		SetFriendSlot(bool bNV)		{ m_bFriendSlot = bNV; }
	void		SetCommentDirty(bool bDirty = true){ m_bCommentDirty = bDirty; }
	uint8		GetSourceExchangeVersion() const{ return m_bySourceExchangeVer; }
	bool		SafeSendPacket(Packet* packet);
	
	void		SendPublicKeyPacket();
	void		SendSignaturePacket();
	void		ProcessPublicKeyPacket(const uchar* pachPacket, uint32 nSize);
	void		ProcessSignaturePacket(const uchar* pachPacket, uint32 nSize);
	uint8		GetSecureIdentState() {
		if (m_SecureIdentState != IS_UNAVAILABLE) {
			if (!SecIdentSupRec) {
				printf("\nWrong Tags on SecIdentState packet!!\n");
				printf("%s\n",unicode2char(GetClientFullInfo()));
				printf("User Disconnected.\n");			
			}
			wxASSERT(SecIdentSupRec);
		}
		return m_SecureIdentState;
	}
	void		SendSecIdentStatePacket();
	void		ProcessSecIdentStatePacket(const uchar* pachPacket, uint32 nSize);

	uint8		GetInfoPacketsReceived() const { return m_byInfopacketsReceived; }
	void		InfoPacketsReceived();

	CClientReqSocket	*socket;
	CClientCredits 		*credits;
	CFriend 		*m_Friend;

	//upload
	uint32		compressiongain; // Add show compression
	uint32  	notcompressed; // Add show compression
	uint8		GetUploadState() const		{ return m_nUploadState; }
	void		SetUploadState(uint8 news)	{ m_nUploadState = news; }
	uint32		GetWaitStartTime() const	{ return m_dwWaitTime; }
	uint32		GetWaitTime() const 		{ return m_dwUploadTime-m_dwWaitTime; }
	bool		IsDownloading()	const 		{ return (m_nUploadState == US_UPLOADING); }
	bool		HasBlocks() const
		{ return !(m_BlockSend_queue.IsEmpty() && m_BlockRequests_queue.IsEmpty()); }
	float		GetKBpsUp()	const 		{ return kBpsUp; }
	uint32		GetScore(bool sysvalue, bool isdownloading = false, bool onlybasevalue = false) const;
	void		AddReqBlock(Requested_Block_Struct* reqblock);
	bool		CreateNextBlockPackage();
	void		SetUpStartTime(uint32 dwTime = 0);
	uint32		GetUpStartTimeDelay() const	{ return ::GetTickCount() - m_dwUploadTime; }
	void		SetWaitStartTime(uint32 dwTime = 0);
	void		SendHashsetPacket(const CMD4Hash& forfileid);
	bool		SupportMultiPacket() const { return m_bMultiPacket;	}

	void		SetUploadFileID(CKnownFile* newreqfile);
	void		ProcessExtendedInfo(const CSafeMemFile* data, CKnownFile* tempreqfile);
	void		ProcessFileInfo(const CSafeMemFile* data, const CPartFile* file);
	void		ProcessFileStatus(bool bUdpPacket, const CSafeMemFile* data, const CPartFile* file);
	
	const CMD4Hash&	GetUploadFileID() const	{ return m_requpfileid; }
	CPartFile*	GetDownloadFile()	{ return reqfile; }
	uint32		SendBlockData(float kBpsToSend);
	void		ClearUploadBlockRequests();
	void		SendRankingInfo();
	void		SendCommentInfo(CKnownFile *file);
	void		AddRequestCount(const CMD4Hash& fileid);
	bool 		IsDifferentPartBlock() const;
	void		UnBan();
	void		Ban();
	bool		m_bAddNextConnect;	// VQB Fix for LowID slots only on connection
	uint32		GetBanTime() const		{ return m_dwBanTime; }
	uint32		GetAskedCount() const 		{ return m_cAsked; }
	void		AddAskedCount()			{ m_cAsked++; }
	void		SetAskedCount(uint32 m_cInAsked){ m_cAsked = m_cInAsked; }
	void		FlushSendBlocks();	// call this when you stop upload, 
						// or the socket might be not able to send
	void		SetLastUpRequest()		{ m_dwLastUpRequest = ::GetTickCount(); }
	uint32		GetLastUpRequest() const 	{ return m_dwLastUpRequest; }
	void		UDPFileReasked();
	uint32		GetSessionUp() const 		{ return m_nTransferedUp - m_nCurSessionUp; }
	void		ResetSessionUp()		{ m_nCurSessionUp = m_nTransferedUp; }
	uint16		GetUpPartCount() const 		{ return m_nUpPartCount; }
	void		DrawUpStatusBar(wxMemoryDC* dc, wxRect rect, bool onlygreyrect, bool  bFlat);

	//download
	uint32		GetAskedCountDown() const 	{ return m_cDownAsked; }
	void		AddAskedCountDown()		{ m_cDownAsked++; }
	void		SetAskedCountDown(uint32 m_cInDownAsked){ m_cDownAsked = m_cInDownAsked; }
	uint8		GetDownloadState() const	{ return m_nDownloadState; }
	void		SetDownloadState(uint8 byNewState);
	uint32		GetLastAskedTime() const	{ return m_dwLastAskedTime; }

	bool		IsPartAvailable(uint16 iPart) const
		{ return ( (iPart >= m_nPartCount) || (!m_abyPartStatus) )? 0:m_abyPartStatus[iPart]; }
	bool		IsUpPartAvailable(uint16 iPart) const 
		{ return ( (iPart >= m_nUpPartCount) || (!m_abyUpPartStatus) )? 0:m_abyUpPartStatus[iPart];}

	const uint8*	GetPartStatus() const		{ return m_abyPartStatus; }
	float		GetKBpsDown() const		{ return kBpsDown; }
	float		CalculateKBpsDown();
	uint16		GetRemoteQueueRank() const	{ return m_nRemoteQueueRank; }
	uint16		GetOldRemoteQueueRank() const	{ return m_nOldRemoteQueueRank; }
	void		SetRemoteQueueFull(bool flag)	{ m_bRemoteQueueFull = flag; }
	bool		IsRemoteQueueFull() const 	{ return m_bRemoteQueueFull; }
	void		SetRemoteQueueRank(uint16 nr);
	void		DrawStatusBar(wxMemoryDC* dc, wxRect rect, bool onlygreyrect, bool  bFlat);
	bool		AskForDownload();
	void		SendStartupLoadReq();
	void		SendFileRequest();
	void		ProcessHashSet(const char *packet, uint32 size);
	bool		AddRequestForAnotherFile(CPartFile* file);
	void		SendBlockRequests();
	void		ProcessBlockPacket(const char* packet, uint32 size, bool packed = false);
	uint16		GetAvailablePartCount() const;
	bool		SwapToAnotherFile(bool bIgnoreNoNeeded, bool ignoreSuspensions, bool bRemoveCompletely, CPartFile* toFile = NULL);
	void		DontSwapTo(CPartFile* file);
	bool		IsSwapSuspended(const CPartFile* file);
	bool		DoSwap(CPartFile* SwapTo, bool anotherfile = false);
	void		UDPReaskACK(uint16 nNewQR);
	void		UDPReaskFNF();
	void		UDPReaskForDownload();
	bool		IsSourceRequestAllowed();
	// -khaos--+++> Download Sessions Stuff Imported from eMule 0.30c (Creteil) BEGIN ...
	void		SetDownStartTime()		{ m_dwDownStartTime = ::GetTickCount(); }
	uint32		GetDownTimeDifference() 
		{ uint32 myTime = m_dwDownStartTime; m_dwDownStartTime = 0; return ::GetTickCount() - myTime; }
	bool		GetTransferredDownMini() const	{ return m_bTransferredDownMini; }
	void		SetTransferredDownMini()	{ m_bTransferredDownMini=true; }
	void		InitTransferredDownMini()	{ m_bTransferredDownMini=false; }
	//
	//	A4AF Stats Stuff:
	//		In CPartFile::Process, I am going to keep a tally of how many clients
	//		in that PF's source list are A4AF for other files.  This tally is worthless
	//		to the PartFile that it belongs to, but when we add all of these counts up for
	//		each PartFile, we will get an accurate count of how many A4AF requests there are
	//		total.  This is for the Found Sources section of the tree.  This is a better, faster
	//		option than looping through the lists for unavailable sources.
	//
	uint16		GetA4AFCount() const 		{ return m_OtherRequests_list.GetCount(); }
	// -khaos--+++> Download Sessions Stuff Imported from eMule 0.30c (Creteil) END ...

	uint16		GetUpCompleteSourcesCount() const { return m_nUpCompleteSourcesCount; }
	void		SetUpCompleteSourcesCount(uint16 n){ m_nUpCompleteSourcesCount = n; }
	
	int		sourcesslot;

	//chat
	uint8		GetChatState()			{ return m_byChatstate; }
	void		SetChatState(uint8 nNewS)	{ m_byChatstate = nNewS; }

	//File Comment 
	const wxString&	GetFileComment() const 		{ return m_strComment; }
	void		SetFileComment(const char *desc){ m_strComment = char2unicode(desc); }
	uint8		GetFileRate() const		{ return m_iRate; }
	
	wxString	GetSoftStr() const 		{ return m_clientVerString.Left(m_SoftLen); }
	wxString	GetSoftVerStr() const		{ return m_clientVerString.Mid(m_SoftLen+1); }
	
	void		SetFileRate(int8 iNewRate)	{ m_iRate=iNewRate; }

	uint16		GetKadPort() const		{ return m_nKadPort; }
	void		SetKadPort(uint16 nPort)	{ m_nKadPort = nPort; }
	
	// Barry - Process zip file as it arrives, don't need to wait until end of block
	int unzip(Pending_Block_Struct *block, BYTE *zipped, uint32 lenZipped, BYTE **unzipped, uint32 *lenUnzipped, int iRecursion = 0);
	// Barry - Sets string to show parts downloading, eg NNNYNNNNYYNYN
	wxString	ShowDownloadingParts();
	void 		UpdateDisplayedInfo(bool force = false);
	int 		GetFileListRequested() const 	{ return m_iFileListRequested; }
	void 		SetFileListRequested(int iFileListRequested) 
		{ m_iFileListRequested = iFileListRequested; }
	
	void		ResetFileStatusInfo();
	
	bool		CheckHandshakeFinished(UINT protocol, UINT opcode) const;
		
	CPartFile*	reqfile;
	
	bool		GetSentCancelTransfer() const { return m_fSentCancelTransfer; }
	void		SetSentCancelTransfer(bool bVal) { m_fSentCancelTransfer = bVal; }
	
	wxString	GetClientFullInfo();

private:

	// base
	void		Init();
	bool		ProcessHelloTypePacket(const CSafeMemFile& data);
	void		SendHelloTypePacket(CMemFile* data);
	void		ClearHelloProperties(); // eMule 0.42
	bool		m_bIsBotuser;
//	bool		isfriend;
	uint32		m_dwUserIP;
	uint32		m_dwServerIP;
	uint32		m_nUserID;
	int16		m_nUserPort;
	int16		m_nServerPort;
	uint32		m_nClientVersion;
	uint32		m_cSendblock;
	uint8		m_byEmuleVersion;
	uint8		m_byDataCompVer;
	uint8		m_SoftLen;
	bool		m_bEmuleProtocol;
	wxString	m_Username;
	wxString	m_FullUserIP;
	CMD4Hash	m_UserHash;
	bool		m_HasValidHash;
	uint16		m_nUDPPort;
	uint8		m_byUDPVer;
	uint8		m_bySourceExchangeVer;
	uint8		m_byAcceptCommentVer;
	uint8		m_byExtendedRequestsVer;
	uint8		m_cFailed;
	uint8		m_clientSoft;
	uint32		m_dwLastSourceRequest;
	uint32		m_dwLastSourceAnswer;
	uint32		m_dwLastAskedForSources;
	int		m_iFileListRequested;
	bool		m_bFriendSlot;
	bool		m_bCommentDirty;
	bool		m_bIsHybrid;
	bool		m_bIsNewMLD;
	bool		m_bIsML;
	bool		m_bGPLEvildoer;
 	bool		m_bSupportsPreview;
 	bool		m_bPreviewReqPending;
 	bool		m_bPreviewAnsPending;
	uint16		m_nKadPort;
	bool		m_bMultiPacket;
		
 	
	// Kry - Secure User Ident import
	ESecureIdentState	m_SecureIdentState; 
	uint8		m_byInfopacketsReceived;			// have we received the edonkeyprot and emuleprot packet already (see InfoPacketsReceived() )
	uint32		m_dwLastSignatureIP;
	uint8		m_bySupportSecIdent;
	
	uint32		m_byCompatibleClient;
	CTypedPtrList<CPtrList, Packet*>	m_WaitingPackets_list;
	std::list<PartFileStamp>	m_DontSwap_list;
	DWORD		m_lastRefreshedDLDisplay;

	//upload
	void CreateStandartPackets(const unsigned char* data,uint32 togo, Requested_Block_Struct* currentblock);
	void CreatePackedPackets(const unsigned char* data,uint32 togo, Requested_Block_Struct* currentblock);
	
	float		kBpsUp;
	uint32		msSentPrev;
	bool		m_bBanned;
	uint32		m_nTransferedUp;
	uint8		m_nUploadState;
	uint32		m_dwWaitTime;
	uint32		m_dwUploadTime;
	uint32		m_nMaxSendAllowed;
	uint32		m_cAsked;
	uint32		m_dwLastUpRequest;
	uint32		m_dwBanTime;
	bool		m_bUsedComprUp;	//only used for interface output
	uint32		m_nCurSessionUp;
	uint16		m_nUpPartCount;
	CMD4Hash	m_requpfileid;
	uint16		m_nUpCompleteSourcesCount;

public:
	uint8*		m_abyUpPartStatus;
	uint16		m_lastPartAsked;
	DWORD		m_dwEnteredConnectedState;
	wxString	m_strModVersion;
	
	CTypedPtrList<CPtrList, Packet*>		 m_BlockSend_queue;
	CTypedPtrList<CPtrList, Requested_Block_Struct*> m_BlockRequests_queue;
	CTypedPtrList<CPtrList, Requested_Block_Struct*> m_DoneBlocks_list;
	CTypedPtrList<CPtrList, Requested_File_Struct*>	 m_RequestedFiles_list;
	//download
	bool		m_bRemoteQueueFull;
	bool		usedcompressiondown; //only used for interface output
	uint8		m_nDownloadState;
	uint16		m_nPartCount;
	uint32		m_cDownAsked;
	uint32		m_dwLastAskedTime;
	wxString	ClientFilename;
	uint32		m_nTransferedDown;
	// -khaos--+++> Download Session Stats Imported from eMule 0.30c (Creteil) BEGIN ...
	bool		m_bTransferredDownMini;
	uint32		m_dwDownStartTime;
	// -khaos--+++> Download Session Stats Imported from eMule 0.30c (Creteil) END ...
	uint32		m_nLastBlockOffset;   // Patch for show parts that you download [Cax2]
	uint16		m_cShowDR;
	uint32		m_dwLastBlockReceived;
	uint16		m_nRemoteQueueRank;
	uint16		m_nOldRemoteQueueRank;
	bool		m_bCompleteSource;
	bool		m_bReaskPending;
	bool		m_bUDPPending;
	bool		m_bHashsetRequested;

	CTypedPtrList<CPtrList,Pending_Block_Struct*>	m_PendingBlocks_list;
	CTypedPtrList<CPtrList,Requested_Block_Struct*>	m_DownloadBlocks_list;

	float		kBpsDown;
	float		fDownAvgFilter;
	uint32		msReceivedPrev;
	uint32		bytesReceivedCycle;
	// chat
	uint8 		m_byChatstate;
	wxString	m_strComment;
	int8		m_iRate;
	unsigned int 
		m_fHashsetRequesting : 1, // we have sent a hashset request to this client
		m_fNoViewSharedFiles : 1, // client has disabled the 'View Shared Files' feature, 
					  // if this flag is not set, we just know that we don't know 
					  // for sure if it is enabled
		m_fSupportsPreview   : 1,
		m_fSentCancelTransfer: 1, // we have sent an OP_CANCELTRANSFER in the current connection
		m_fSharedDirectories : 1; // client supports OP_ASKSHAREDIRS opcodes
		
	/* Razor 1a - Modif by MikaelB */
	
	int		GetHashType() const;
	bool		m_bHelloAnswerPending;
	// Kry - Atribute to get the 1.x / 2.x / CVS flags
	// Why this way? Well, on future is expected that count(2.x) > count(1x)
	// So I prefer to set the 1.x flag because it will be less CPU. 
	// I know. I'm paranoid on CPU.
	// (Extended_aMule_SO & 1)  -> 1.x 
	// !(Extended_aMule_SO & 1) -> 2.x 	
	// (Extended_aMule_SO & 2)  -> CVS 
	uint8		Extended_aMule_SO;
	
	uint8*		m_abyPartStatus;

public:
	/* m_OtherRequests_list --> public instead of private */
	CTypedPtrList<CPtrList, CPartFile*>	m_OtherRequests_list;
	/* Same for m_OtherNoNeeded_list --> public instead of private (Creteil) */
	CTypedPtrList<CPtrList, CPartFile*>	m_OtherNoNeeded_list;

	/* IsValidSource function
	 * @return true id it's valid source
	*/
	bool IsValidSource() const	{ return m_ValidSource; };

	/* SetValidSource function
	 * @param boolean - set valid source
	*/
	void SetValidSource(bool in)	{ m_ValidSource = in; };

	/* SwapToThisFile function
	 * @param CPartFile* - the file
	*/
	void SwapToThisFile(CPartFile* file);

	uint8 GetExtended_aMule_SO() const{ return Extended_aMule_SO; };
	
private:

	/* valid source attribute */
	bool m_ValidSource;

	/* End modif */

	// Support for tag ET_MOD_VERSION [BlackRat]
public:

	const wxString&	GetClientModString() const { return m_strModVersion; }
	const wxString&	GetClientVerString() const { return m_clientVerString; }

private:

	wxString		m_clientVerString;

	int SecIdentSupRec;
	
	// Hash anti-thief from HoaX_69 [BlackRat]
public:

	bool			thief;  // is a thief ?
	uint64 getUID() const
	{
		uint64 ip = ((uint64) m_dwUserIP) << 32;
		uint64 port = ((uint64) m_nUserPort) << 16;
		uint64 uid = ip + port;
		return uid;
	}
	//return ((uint64)m_dwUserIP<<32)+((uint64)m_nUserPort<<16); }
	int	leechertype; // what kind of leecher is it ?
  
};

#define	MAKE_CLIENT_VERSION(mjr, min, upd) \
	((UINT)(mjr)*100U*10U*100U + (UINT)(min)*100U*10U + (UINT)(upd)*100U)

#endif // UPDOWNCLIENT_H
