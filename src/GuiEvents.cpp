#include <wx/app.h>

#include "GuiEvents.h"
#include "amule.h"
#include "PartFile.h"
#include "DownloadQueue.h"
#include "updownclient.h"
#include "ServerList.h"
#include "Preferences.h"

#ifndef AMULE_DAEMON
#	include "ChatWnd.h"
#	include "amuleDlg.h"
#	include "ServerWnd.h"
#	include "SearchDlg.h"
#	include "TransferWnd.h"
#	include "SharedFilesWnd.h"
#	include "ServerListCtrl.h"
#	include "ClientListCtrl.h"
#	include "SharedFilesCtrl.h"
#	include "DownloadListCtrl.h"
#endif

#ifdef AMULE_DAEMON
#	define NOT_ON_DAEMON(x) WXUNUSED(x)
#else
#	define NOT_ON_DAEMON(x) x
#endif


DEFINE_LOCAL_EVENT_TYPE(MULE_EVT_NOTIFY);


namespace MuleNotify
{

	void HandleNotification(const CMuleNotiferBase& ntf)
	{
		if (wxThread::IsMain()) {
#if defined(AMULE_DAEMON) or defined(REMOTE_GUI)
			ntf.Notify();
#else
			if (theApp.amuledlg) {
				ntf.Notify();
			}
#endif
		} else {
			CMuleGUIEvent evt(ntf.Clone());
			wxPostEvent(wxTheApp, evt);
		}
	}
	
	void Search_Add_Download(CSearchFile* file, uint8 category)
	{
		theApp.downloadqueue->AddSearchToDownload(file, category);
	}
	

	void ShowUserCount(wxString NOT_ON_DAEMON(str))
	{
#ifndef AMULE_DAEMON
		theApp.amuledlg->ShowUserCount(str);
#endif
	}


	void Search_Update_Progress(uint32 NOT_ON_DAEMON(val))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->searchwnd) {
			if (val == 0xffff) {
				// Global search ended
				theApp.amuledlg->searchwnd->ResetControls();
			} else {
				theApp.amuledlg->searchwnd->UpdateProgress(val);
			}
		}
#endif
	}

	
#ifdef CLIENT_GUI
	
	void PartFile_RemoveNoNeeded(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_REMOVE_NO_NEEDED);
	}

	void PartFile_RemoveFullQueue(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_REMOVE_FULL_QUEUE);
	}
	
	void PartFile_RemoveHighQueue(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_REMOVE_HIGH_QUEUE);
	}
	
	void PartFile_SourceCleanup(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_CLEANUP_SOURCES);
	}
	
	void PartFile_Swap_A4AF(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_SWAP_A4AF_THIS);
	}

	void PartFile_Swap_A4AF_Auto(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_SWAP_A4AF_THIS_AUTO);
	}
	
	void PartFile_Swap_A4AF_Others(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_SWAP_A4AF_OTHERS);
	}

	void PartFile_Pause(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_PAUSE);
	}
	
	void PartFile_Resume(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_RESUME);
	}
	
	void PartFile_Stop(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_STOP);
	}

	void PartFile_PrioAuto(CPartFile* file, bool val)
	{
		theApp.downloadqueue->AutoPrio(file, val);
	}

	void PartFile_PrioSet(CPartFile* file, uint8 newDownPriority, bool)
	{
		theApp.downloadqueue->Prio(file, newDownPriority);
	}

	void PartFile_Delete(CPartFile* file)
	{
		theApp.downloadqueue->SendFileCommand(file, EC_OP_PARTFILE_DELETE);
	}
	
	void PartFile_SetCat(CPartFile* file, uint32 val)
	{
		theApp.downloadqueue->Category(file, val);
	}
	
	void KnownFile_Up_Prio_Set(CKnownFile* file, uint8 val)
	{
		theApp.sharedfiles->SetFilePrio(file, val);
	}
	
	void KnownFile_Up_Prio_Auto(CKnownFile* file)
	{
		theApp.sharedfiles->SetFilePrio(file, PR_AUTO);
	}

	void Download_Set_Cat_Prio(uint8, uint8)
	{
	}
	
	void Download_Set_Cat_Status(uint8, int)
	{
	}
	
#else

	void SharedFilesShowFile(CKnownFile* NOT_ON_DAEMON(file))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->sharedfileswnd and theApp.amuledlg->sharedfileswnd->sharedfilesctrl) {
			theApp.amuledlg->sharedfileswnd->sharedfilesctrl->ShowFile(file);
		}
#endif
	}

	void SharedFilesRemoveFile(CKnownFile* NOT_ON_DAEMON(file))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->sharedfileswnd and theApp.amuledlg->sharedfileswnd->sharedfilesctrl) {
			theApp.amuledlg->sharedfileswnd->sharedfilesctrl->RemoveFile(file);
		}
#endif
	}
	
	void SharedFilesRemoveAllFiles()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->sharedfileswnd) {
			theApp.amuledlg->sharedfileswnd->RemoveAllSharedFiles();
		}
#endif
	}

	
	void SharedFilesShowFileList()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->sharedfileswnd and theApp.amuledlg->sharedfileswnd->sharedfilesctrl) {
			theApp.amuledlg->sharedfileswnd->sharedfilesctrl->ShowFileList();
		}
#endif
	}

	void SharedFilesSort()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->sharedfileswnd and theApp.amuledlg->sharedfileswnd->sharedfilesctrl) {
			theApp.amuledlg->sharedfileswnd->sharedfilesctrl->SortList();
		}
#endif
	}


	void SharedFilesUpdateItem(CKnownFile* NOT_ON_DAEMON(file))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->sharedfileswnd and theApp.amuledlg->sharedfileswnd->sharedfilesctrl) {
			theApp.amuledlg->sharedfileswnd->sharedfilesctrl->UpdateItem(file);
		}
#endif
	}


	void DownloadCtrlUpdateItem(const void* NOT_ON_DAEMON(item))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd and theApp.amuledlg->transferwnd->downloadlistctrl) {
			theApp.amuledlg->transferwnd->downloadlistctrl->UpdateItem(item);
		}
#endif
	}

	void DownloadCtrlAddFile(CPartFile* NOT_ON_DAEMON(file))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd and theApp.amuledlg->transferwnd->downloadlistctrl ) {
			theApp.amuledlg->transferwnd->downloadlistctrl->AddFile(file);
		}
#endif
	}

	void DownloadCtrlAddSource(CPartFile* NOT_ON_DAEMON(owner), CUpDownClient* NOT_ON_DAEMON(source), DownloadItemType NOT_ON_DAEMON(type))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd and theApp.amuledlg->transferwnd->downloadlistctrl) {
			if (owner->ShowSources()) {
				theApp.amuledlg->transferwnd->downloadlistctrl->AddSource(owner, source, type);
			}
		}
#endif
	}
	
	void DownloadCtrlRemoveFile(CPartFile* NOT_ON_DAEMON(file))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd and theApp.amuledlg->transferwnd->downloadlistctrl) {
			theApp.amuledlg->transferwnd->downloadlistctrl->RemoveFile(file);
		}
#endif
	}
	
	void DownloadCtrlRemoveSource(const CUpDownClient* NOT_ON_DAEMON(source), const CPartFile* NOT_ON_DAEMON(owner))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd and theApp.amuledlg->transferwnd->downloadlistctrl) {
			if ((owner == NULL) or owner->ShowSources()) {
				theApp.amuledlg->transferwnd->downloadlistctrl->RemoveSource(source, owner);
			}
		}
#endif
	}
	
	void DownloadCtrlHideSource(CPartFile* NOT_ON_DAEMON(file))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd and theApp.amuledlg->transferwnd->downloadlistctrl) {
			theApp.amuledlg->transferwnd->downloadlistctrl->ShowSources(file, false);
		}
#endif
	}

	void DownloadCtrlSort()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd and theApp.amuledlg->transferwnd->downloadlistctrl) {
			theApp.amuledlg->transferwnd->downloadlistctrl->SortList();
		}
#endif
	}
	
	
	void ClientCtrlAddClient(CUpDownClient* NOT_ON_DAEMON(client), ViewType NOT_ON_DAEMON(type))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd and theApp.amuledlg->transferwnd->clientlistctrl) {
			theApp.amuledlg->transferwnd->clientlistctrl->InsertClient(client, type);
		}
#endif
	}
	
	void ClientCtrlRefreshClient(CUpDownClient* NOT_ON_DAEMON(client), ViewType NOT_ON_DAEMON(type))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd and theApp.amuledlg->transferwnd->clientlistctrl) {
			theApp.amuledlg->transferwnd->clientlistctrl->UpdateClient(client, type);
		}
#endif
	}
	
	void ClientCtrlRemoveClient(CUpDownClient* NOT_ON_DAEMON(client), ViewType NOT_ON_DAEMON(type))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd and theApp.amuledlg->transferwnd->clientlistctrl) {
			theApp.amuledlg->transferwnd->clientlistctrl->RemoveClient(client, type);
		}
#endif
	}
	
	
	void ServerAdd(CServer* NOT_ON_DAEMON(server))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->serverwnd and theApp.amuledlg->serverwnd->serverlistctrl) {
			theApp.amuledlg->serverwnd->serverlistctrl->AddServer(server);
		}
#endif
	}
	
	void ServerRemove(CServer* NOT_ON_DAEMON(server))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->serverwnd and theApp.amuledlg->serverwnd->serverlistctrl) {
			theApp.amuledlg->serverwnd->serverlistctrl->RemoveServer(server);
		}
#endif
	}

	void ServerRemoveDead()
	{
		if (theApp.serverlist) {
			theApp.serverlist->RemoveDeadServers();
		}		
	}
	
	void ServerRemoveAll()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->serverwnd and theApp.amuledlg->serverwnd->serverlistctrl) {
			theApp.amuledlg->serverwnd->serverlistctrl->DeleteAllItems();
		}
#endif
	}
	
	void ServerHighlight(CServer* NOT_ON_DAEMON(server), bool NOT_ON_DAEMON(highlight))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->serverwnd and theApp.amuledlg->serverwnd->serverlistctrl) {
			theApp.amuledlg->serverwnd->serverlistctrl->HighlightServer(server, highlight);
		}
#endif
	}
	
	void ServerRefresh(CServer* NOT_ON_DAEMON(server))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->serverwnd and theApp.amuledlg->serverwnd->serverlistctrl) {
			theApp.amuledlg->serverwnd->serverlistctrl->RefreshServer(server);
		}
#endif
	}
	
	void ServerFreeze()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->serverwnd and theApp.amuledlg->serverwnd->serverlistctrl) {
			theApp.amuledlg->serverwnd->serverlistctrl->Freeze();
		}
#endif
	}
	
	void ServerThaw()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->serverwnd and theApp.amuledlg->serverwnd->serverlistctrl) {
			theApp.amuledlg->serverwnd->serverlistctrl->Thaw();
		}
#endif
	}
	
	void ServerUpdateED2KInfo()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->serverwnd) {
			theApp.amuledlg->serverwnd->UpdateED2KInfo();
		}
#endif
	}
	
	void ServerUpdateKadKInfo()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->serverwnd) {
			theApp.amuledlg->serverwnd->UpdateKadInfo();
		}		
#endif
	}


	void SearchCancel()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->searchwnd) {
			theApp.amuledlg->searchwnd->ResetControls();
		}
#endif
	}
	
	void SearchLocalEnd()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->searchwnd) {
			theApp.amuledlg->searchwnd->LocalSearchEnd();
		}
#endif
	}
	
	void Search_Update_Sources(CSearchFile* NOT_ON_DAEMON(result))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg and theApp.amuledlg->searchwnd) {
			theApp.amuledlg->searchwnd->UpdateResult(result);
		}
#endif
	}
	
	void Search_Add_Result(CSearchFile* NOT_ON_DAEMON(result))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg and theApp.amuledlg->searchwnd) {
			theApp.amuledlg->searchwnd->AddResult(result);
		}
#endif
	}

	
	void ChatRefreshFriend(uint32 NOT_ON_DAEMON(lastUsedIP), uint32 NOT_ON_DAEMON(lastUsedPort), wxString NOT_ON_DAEMON(name))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->chatwnd) {
			theApp.amuledlg->chatwnd->RefreshFriend(CMD4Hash(), name, lastUsedIP, lastUsedPort);
		}
#endif
	}
	
	void ChatConnResult(bool NOT_ON_DAEMON(success), uint64 NOT_ON_DAEMON(id), wxString NOT_ON_DAEMON(message))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->chatwnd) {
			theApp.amuledlg->chatwnd->ConnectionResult(success, message, id);
		}
#endif
	}
	
	void ChatProcessMsg(uint64 NOT_ON_DAEMON(sender), wxString NOT_ON_DAEMON(message))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->chatwnd) {
			theApp.amuledlg->chatwnd->ProcessMessage(sender, message);
		}
#endif
	}
	

	void ShowConnState(long NOT_ON_DAEMON(state))
	{
#ifndef AMULE_DAEMON
#ifdef CLIENT_GUI
		theApp.m_ConnState = state;
#endif
		
		theApp.amuledlg->ShowConnectionState();
#endif
	}
	
	void ShowQueueCount(uint32 NOT_ON_DAEMON(count))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd) {
			theApp.amuledlg->transferwnd->ShowQueueCount(count);
		}
#endif
	}
	
	void ShowUpdateCatTabTitles()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd) {
			theApp.amuledlg->transferwnd->UpdateCatTabTitles();
		}
#endif
	}
	
	void ShowGUI()
	{
#ifndef AMULE_DAEMON
		theApp.amuledlg->Show_aMule(true);
#endif
	}
	

	void CategoryAdded()
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd) {
			theApp.amuledlg->transferwnd->AddCategory(theApp.glob_prefs->GetCategory(0));
		}
#endif
	}
	
	void CategoryUpdate(uint32 NOT_ON_DAEMON(cat))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd) {
			theApp.amuledlg->transferwnd->UpdateCategory(cat);
			theApp.amuledlg->transferwnd->downloadlistctrl->Refresh();
			theApp.amuledlg->searchwnd->UpdateCatChoice();
		}
#endif
	}
	
	void CategoryDelete(uint32 NOT_ON_DAEMON(cat))
	{
#ifndef AMULE_DAEMON
		if (theApp.amuledlg->transferwnd) {
			theApp.amuledlg->transferwnd->RemoveCategory(cat);			
			theApp.amuledlg->searchwnd->UpdateCatChoice();
		}
#endif
	}

	
	void PartFile_RemoveNoNeeded(CPartFile* file)
	{
		file->CleanUpSources( true,  false, false );
	}
	
	void PartFile_RemoveFullQueue(CPartFile* file)
	{
		file->CleanUpSources( false, true,  false );
	}
	
	void PartFile_RemoveHighQueue(CPartFile* file)
	{
		file->CleanUpSources( false, false, true  );
	}
	
	void PartFile_SourceCleanup(CPartFile* file)
	{
		file->CleanUpSources( true,  true,  true  );
	}
	
	void PartFile_Swap_A4AF(CPartFile* file)
	{
		if ((file->GetStatus(false) == PS_READY || file->GetStatus(false) == PS_EMPTY)) {
			CPartFile::SourceSet::iterator it = file->GetA4AFList().begin();
			for ( ; it != file->GetA4AFList().end(); ) {
				CUpDownClient *cur_source = *it++;

				cur_source->SwapToAnotherFile(true, false, false, file);
			}
		}
	}
	
	void PartFile_Swap_A4AF_Auto(CPartFile* file)
	{
		file->SetA4AFAuto(not file->IsA4AFAuto());
	}
	
	void PartFile_Swap_A4AF_Others(CPartFile* file)
	{
		if ((file->GetStatus(false) == PS_READY) || (file->GetStatus(false) == PS_EMPTY)) {
			CPartFile::SourceSet::iterator it = file->GetSourceList().begin();
			for( ; it != file->GetSourceList().end(); ) {
				CUpDownClient* cur_source = *it++;

				cur_source->SwapToAnotherFile(false, false, false, NULL);
			}
		}
	}
	
	void PartFile_Pause(CPartFile* file)
	{
		file->PauseFile();
	}

	void PartFile_Resume(CPartFile* file)
	{
		file->ResumeFile();
		file->SavePartFile();
	}

	void PartFile_Stop(CPartFile* file)
	{
		file->StopFile();
	}

	void PartFile_PrioAuto(CPartFile* file, bool val)
	{
		file->SetAutoDownPriority(val);
	}

	void PartFile_PrioSet(CPartFile* file, uint8 newDownPriority, bool bSave)
	{
		file->SetDownPriority(newDownPriority, bSave);
	}
	
	void PartFile_Delete(CPartFile* file)
	{
		file->Delete();
	}
	
	void PartFile_SetCat(CPartFile* file, uint32 val)
	{
		file->SetCategory(val);
	}

	
	void KnownFile_Up_Prio_Set(CKnownFile* file, uint8 val)
	{
		file->SetAutoUpPriority(false);
		file->SetUpPriority(val);
	}
	
	void KnownFile_Up_Prio_Auto(CKnownFile* file)
	{
		file->SetAutoUpPriority(true);
		file->UpdateAutoUpPriority();
	}
	
	void KnownFile_Comment_Set(CKnownFile* file, wxString comment)
	{
		file->SetFileComment(comment);
	}
	

	void Download_Set_Cat_Prio(uint8 cat, uint8 newprio)
	{
		theApp.downloadqueue->SetCatPrio(cat, newprio);
	}
	
	void Download_Set_Cat_Status(uint8 cat, int newstatus)
	{
		theApp.downloadqueue->SetCatStatus(cat, newstatus);
	}

#endif	// #ifdef CLIENT_GUI
};

