//
// This file is part of the aMule Project.
//
// Copyright (c) 2006-2008 Mikkel Schubert ( xaignar@amule.org / http:://www.amule.org )
// Copyright (c) 2003-2008 aMule Team ( admin@amule.org / http://www.amule.org )
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
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA
//


#include <wx/app.h>			// Needed for wxTheApp

#include "ThreadTasks.h"		// Interface declarations
#include "PartFile.h"			// Needed for CPartFile
#include "Logger.h"			// Needed for Add(Debug)LogLineM
#include <common/Format.h>		// Needed for CFormat
#include <common/FileFunctions.h>	// Needed for CheckFileExists
#include "amule.h"			// Needed for theApp
#include "KnownFileList.h"		// Needed for theApp->knownfiles
#include "Preferences.h"		// Needed for thePrefs
#include "ScopedPtr.h"			// Needed for CScopedPtr and CScopedArray



//! This hash represents the value for an empty MD4 hashing
const byte g_emptyMD4Hash[16] = {
	0x31, 0xD6, 0xCF, 0xE0, 0xD1, 0x6A, 0xE9, 0x31, 
	0xB7, 0x3C, 0x59, 0xD7, 0xE0, 0xC0, 0x89, 0xC0 };


////////////////////////////////////////////////////////////
// CHashingTask

CHashingTask::CHashingTask(const CPath& path, const CPath& filename, const CPartFile* part)
	// GetPrintable is used to improve the readability of the log.
	: CThreadTask(wxT("Hashing"), path.JoinPaths(filename).GetPrintable(), (part ? ETP_High : ETP_Normal)),
	  m_path(path),
	  m_filename(filename),
	  m_toHash((EHashes)(EH_MD4 | EH_AICH)),
	  m_owner(part)
{
	// We can only create the AICH hashset if the file is a knownfile or 
	// if the partfile is complete, since the MD4 hashset is checked first,
	// so that the AICH hashset only gets assigned if the MD4 hashset 
	// matches what we expected. Due to the rareity of post-completion
	// corruptions, this gives us a nice speedup in most cases.
	if (part && !part->GetGapList().empty()) {
		m_toHash = EH_MD4;
	}
}


CHashingTask::CHashingTask(const CKnownFile* toAICHHash)
	// GetPrintable is used to improve the readability of the log.
	: CThreadTask(wxT("AICH Hashing"), toAICHHash->GetFilePath().JoinPaths(toAICHHash->GetFileName()).GetPrintable(), ETP_Low),
	  m_path(toAICHHash->GetFilePath()),
	  m_filename(toAICHHash->GetFileName()),
	  m_toHash(EH_AICH),
	  m_owner(toAICHHash)
{
}


void CHashingTask::Entry()
{
	CFile file;

	CPath fullPath = m_path.JoinPaths(m_filename);
	if (!file.Open(fullPath, CFile::read)) {
		AddDebugLogLineM(true, logHasher,
			CFormat(wxT("Warning, failed to open file, skipping: %s")) % fullPath);
		return;
	}
	
	uint64 fileLength = 0;
	try {
		fileLength = file.GetLength();
	} catch (const CIOFailureException&) {
		AddDebugLogLineM(true, logHasher,
			CFormat(wxT("Warning, failed to retrieve file-length, skipping: %s")) % fullPath);
		return;
	}

	if (fileLength > MAX_FILE_SIZE) {
		AddDebugLogLineM(true, logHasher,
			CFormat(wxT("Warning, file is larger than supported size, skipping: %s")) % fullPath);
		return;
	} else if (fileLength == 0) {
		if (m_owner) {
			// It makes no sense to try to hash empty partfiles ...
			wxASSERT(0);
		} else {
			// Zero-size partfiles should be hashed, but not zero-sized shared-files.
			AddDebugLogLineM( true, logHasher,
				CFormat(wxT("Warning, 0-size file, skipping: %s")) % fullPath);
		}
		
		return;
	}
	
	// For thread-safety, results are passed via a temporary file object.
	CScopedPtr<CKnownFile> knownfile(new CKnownFile());
	knownfile->m_filePath = m_path;
	knownfile->SetFileName(m_filename);
	knownfile->SetFileSize(fileLength);
	knownfile->m_lastDateChanged = CPath::GetModificationTime(fullPath);
	knownfile->m_AvailPartFrequency.insert(
		knownfile->m_AvailPartFrequency.begin(),
		knownfile->GetPartCount(), 0);
	
	if ((m_toHash & EH_MD4) && (m_toHash & EH_AICH)) {
		knownfile->GetAICHHashset()->FreeHashSet();
		AddLogLineM( false, logHasher, CFormat( 
			_("Starting to create MD4 and AICH hash for file: %s")) %
			m_filename );
	} else if ((m_toHash & EH_MD4)) {
		AddLogLineM( false, logHasher, CFormat(
			_("Starting to create MD4 hash for file: %s")) % m_filename );
	} else if ((m_toHash & EH_AICH)) {
		knownfile->GetAICHHashset()->FreeHashSet();
		AddLogLineM( false, logHasher, CFormat(
			_("Starting to create AICH hash for file: %s")) % m_filename );
	} else {
		wxCHECK_RET(0, (CFormat(wxT("No hashes requested for file, skipping: %s"))
			% m_filename).GetString());
	}
	
	
	// This loops creates the part-hashes, loop-de-loop.
	try {
		while (!file.Eof() && !TestDestroy()) {
			if (CreateNextPartHash(&file, knownfile.get(), m_toHash) == false) {
				AddDebugLogLineM(true, logHasher,
					CFormat(wxT("Error while hashing file, skipping: %s"))
						% m_filename);
			
				return;
			}
		}
	} catch (const CSafeIOException& e) {
		AddDebugLogLineM(true, logHasher, wxT("IO exception while hashing file: ") + e.what());
		return;
	}

	if ((m_toHash & EH_MD4) && !TestDestroy()) {
		// If the file is < PARTSIZE, then the filehash is that one hash,
		// otherwise, the filehash is the hash of the parthashes
		if ( knownfile->m_hashlist.size() == 1 ) {
			knownfile->m_abyFileHash = knownfile->m_hashlist[0];
			knownfile->m_hashlist.clear();
		} else if ( knownfile->m_hashlist.size() ) {
			CMD4Hash hash;
			knownfile->CreateHashFromHashlist(knownfile->m_hashlist, &hash);
			knownfile->m_abyFileHash = hash;
		} else {
			// This should not happen!
			wxASSERT(0);
		}
	}
	
	// Did we create a AICH hashset?
	if ((m_toHash & EH_AICH) && !TestDestroy()) {
		CAICHHashSet* AICHHashSet = knownfile->GetAICHHashset();

		AICHHashSet->ReCalculateHash(false);
		if (AICHHashSet->VerifyHashTree(true) ) {
			AICHHashSet->SetStatus(AICH_HASHSETCOMPLETE);
			if (!AICHHashSet->SaveHashSet()) {
				AddDebugLogLineM( true, logHasher,
					CFormat(wxT("Warning, failed to save AICH hashset for file: %s"))
						% m_filename );
			}
		}
	}
	
	if ((m_toHash == EH_AICH) && !TestDestroy()) {
		CHashingEvent evt(MULE_EVT_AICH_HASHING, knownfile.release(), m_owner);
		
		wxPostEvent(wxTheApp, evt);
	} else if (!TestDestroy()) {
		CHashingEvent evt(MULE_EVT_HASHING, knownfile.release(), m_owner);
		
		wxPostEvent(wxTheApp, evt);
	}
}


bool CHashingTask::CreateNextPartHash(CFile* file, CKnownFile* owner, EHashes toHash)
{
	wxCHECK_MSG(!file->Eof(), false, wxT("Unexpected EOF in CreateNextPartHash"));
	
	// We'll read at most PARTSIZE bytes per cycle
	const uint64 partLength = std::min<uint64>(PARTSIZE, file->GetLength() - file->GetPosition());
	
	CMD4Hash hash;
	CMD4Hash* md4Hash = ((toHash & EH_MD4) ? &hash : NULL);
	CAICHHashTree* aichHash = NULL;

	// Setup for AICH hashing
	if (toHash & EH_AICH) {
		aichHash = owner->GetAICHHashset()->m_pHashTree.FindHash(file->GetPosition(), partLength);
	}

	owner->CreateHashFromFile(file, partLength, md4Hash, aichHash);
	
	if (toHash & EH_MD4) {
		// Store the md4 hash
		owner->m_hashlist.push_back(hash);
	
		// This is because of the ed2k implementation for parts. A 2 * PARTSIZE 
		// file i.e. will have 3 parts (see CKnownFile::SetFileSize for comments). 
		// So we have to create the hash for the 0-size data, which will be the default
		// md4 hash for null data: 31D6CFE0D16AE931B73C59D7E0C089C0	
		if ((partLength == PARTSIZE) && file->Eof()) {
			owner->m_hashlist.push_back(CMD4Hash(g_emptyMD4Hash));
		}
	}

	return true;
}


void CHashingTask::OnLastTask()
{
	if (GetType() == wxT("Hashing")) {
		// To prevent rehashing in case of crashes, we 
		// explicity save the list of hashed files here.
		theApp->knownfiles->Save();

		// Make sure the AICH-hashes are up to date.
		CThreadScheduler::AddTask(new CAICHSyncTask());
	}
}


////////////////////////////////////////////////////////////
// CAICHSyncTask

CAICHSyncTask::CAICHSyncTask()
	: CThreadTask(wxT("AICH Syncronizing"), wxEmptyString, ETP_Low)
{
}


void CAICHSyncTask::Entry()
{
	ConvertToKnown2ToKnown264();
	
	AddDebugLogLineM( false, logAICHThread, wxT("Syncronization thread started.") );
	
	// We collect all masterhashs which we find in the known2.met and store them in a list
	std::list<CAICHHash> hashlist;
	const CPath fullpath = CPath(theApp->ConfigDir + KNOWN2_MET_FILENAME);
	
	CFile file;
	if (!file.Open(fullpath, (fullpath.FileExists() ? CFile::read_write : CFile::write))) {
		AddDebugLogLineM( true, logAICHThread, wxT("Error, failed to open 'known2_64.met' file!") );
		return;
	}

	uint32 nLastVerifiedPos = 0;
	try {
		if (file.Eof()) {
			file.WriteUInt8(KNOWN2_MET_VERSION);
		} else {
			if (file.ReadUInt8() != KNOWN2_MET_VERSION) {
				throw CEOFException(wxT("Invalid met-file header found, removing file."));
			}
			
			uint64 nExistingSize = file.GetLength();
			while (file.GetPosition() < nExistingSize) {
				// Read the next hash
				hashlist.push_back(CAICHHash(&file));

				uint32 nHashCount = file.ReadUInt32();
				if (file.GetPosition() + nHashCount * CAICHHash::GetHashSize() > nExistingSize){
					throw CEOFException(wxT("Hashlist ends past end of file."));
				}

				// skip the rest of this hashset
				nLastVerifiedPos = file.Seek(nHashCount * HASHSIZE, wxFromCurrent);
			}
		}
	} catch (const CEOFException&) {
		AddDebugLogLineM(true, logAICHThread, wxT("Hashlist corrupted, truncating file."));
		file.SetLength(nLastVerifiedPos);
	} catch (const CIOFailureException& e) {
		AddDebugLogLineM(true, logAICHThread, wxT("IO failure while reading hashlist (Aborting): ") + e.what());
		
		return;		
	}
	
	AddDebugLogLineM( false, logAICHThread, wxT("Masterhashes of known files have been loaded.") );

#ifndef AMULE_DAEMON
	// Since we will be modifying objects in the main thread, 
	// we need to keep it from messing with the same objects.
	wxMutexGuiLocker guiLock;
#else
	//#warning Thread-safety needed
#endif
	
	// Now we check that all files which are in the sharedfilelist have a
	// corresponding hash in our list. Those how don't are queued for hashing.
	for (unsigned i = 0; i < theApp->sharedfiles->GetCount(); ++i) {
		const CKnownFile* kfile = theApp->sharedfiles->GetFileByIndex(i);
	
		if (TestDestroy()) {
			break;
		} else if (kfile && !kfile->IsPartFile()) {
			CAICHHashSet* hashset = kfile->GetAICHHashset();

			if (hashset->GetStatus() == AICH_HASHSETCOMPLETE) {
				if (std::find(hashlist.begin(), hashlist.end(), hashset->GetMasterHash()) != hashlist.end()) {
					continue;
				}
			}

			hashset->SetStatus(AICH_ERROR);

			CThreadScheduler::AddTask(new CHashingTask(kfile));
		}
	}
}


bool CAICHSyncTask::ConvertToKnown2ToKnown264()
{
	// converting known2.met to known2_64.met to support large files
	// changing hashcount from uint16 to uint32

	const CPath oldfullpath = CPath(theApp->ConfigDir + OLD_KNOWN2_MET_FILENAME);
	const CPath newfullpath = CPath(theApp->ConfigDir + KNOWN2_MET_FILENAME);
	
	if (newfullpath.FileExists() || !oldfullpath.FileExists()) {
		// In this case, there is nothing that we need to do.
		return false;
	}

	CFile oldfile;
	CFile newfile;

	if (!oldfile.Open(oldfullpath, CFile::read)) {
		AddDebugLogLineM(true, logAICHThread, wxT("Failed to open 'known2.met' file."));
		
		// else -> known2.met also doesn't exists, so nothing to convert
		return false;
	}


	if (!newfile.Open(newfullpath, CFile::write_excl)) {
		AddDebugLogLineM(true, logAICHThread, wxT("Failed to create 'known2_64.met' file."));
		
		return false;
	}

	AddLogLineM(false, CFormat(_("Converting old AICH hashsets in '%s' to 64b in '%s'."))
			% OLD_KNOWN2_MET_FILENAME % KNOWN2_MET_FILENAME);

	try {
		newfile.WriteUInt8(KNOWN2_MET_VERSION);
		
		while (newfile.GetPosition() < oldfile.GetLength()) {
			CAICHHash aichHash(&oldfile);
			uint32 nHashCount = oldfile.ReadUInt16();
			
			CScopedArray<byte> buffer(new byte[nHashCount * CAICHHash::GetHashSize()]);
			
			oldfile.Read(buffer.get(), nHashCount * CAICHHash::GetHashSize());
			newfile.Write(aichHash.GetRawHash(), CAICHHash::GetHashSize());
			newfile.WriteUInt32(nHashCount);
			newfile.Write(buffer.get(), nHashCount * CAICHHash::GetHashSize());
		}
		newfile.Flush();
	} catch (const CEOFException& e) {
		AddDebugLogLineM(true, logAICHThread, wxT("Error reading old 'known2.met' file.") + e.what());
		return false;
	} catch (const CIOFailureException& e) {
		AddDebugLogLineM(true, logAICHThread, wxT("IO error while converting 'known2.met' file: ") + e.what());
		return false;
	}
	
	// FIXME LARGE FILES (uncomment)
	//DeleteFile(oldfullpath);

	return true;
}



////////////////////////////////////////////////////////////
// CCompletionTask


CCompletionTask::CCompletionTask(const CPartFile* file)
	// GetPrintable is used to improve the readability of the log.
	: CThreadTask(wxT("Completing"), file->GetFullName().GetPrintable(), ETP_High),
	  m_filename(file->GetFileName()),
	  m_metPath(file->GetFullName()),
	  m_category(file->GetCategory()),
	  m_owner(file),
	  m_error(false)
{
	wxASSERT(m_filename.IsOk());
	wxASSERT(m_metPath.IsOk());
	wxASSERT(m_owner);
}
	

void CCompletionTask::Entry()
{
	CPath targetPath;
   
	{
#ifndef AMULE_DAEMON
		// Prevent the preference values from changing underneeth us.
		wxMutexGuiLocker guiLock;
#else
		//#warning Thread-safety needed
#endif
		
		targetPath = CPath(theApp->glob_prefs->GetCategory(m_category)->incomingpath);
		if (!targetPath.DirExists()) {
			targetPath = thePrefs::GetIncomingDir();
		}
	}
	
	// Check if the target directory is on a Fat32 FS, since that needs extra cleanups.
	bool isFat32 = (CheckFileSystem(targetPath) == FS_IsFAT32);
	CPath dstName = m_filename.Cleanup(true, isFat32);

	// Avoid empty filenames ...
	if (!dstName.IsOk()) {
		dstName = CPath(wxT("Unknown"));
	}

	if (m_filename != dstName) {
		AddLogLineM(true, logPartFile, CFormat(_("WARNING: The filename '%s' is invalid and has been renamed to '%s'."))
			% m_filename % dstName);
	}
	
	// Avoid saving to an already existing filename
	CPath newName = targetPath.JoinPaths(dstName);
	for (unsigned count = 0; newName.FileExists(); ++count) {
		wxString postfix = wxString::Format(wxT("(%u)"), count);

		newName = targetPath.JoinPaths(dstName.AddPostfix(postfix));
	}

	if (newName != targetPath.JoinPaths(dstName)) {
		AddLogLineM(true, logPartFile, CFormat(_("WARNING: The file '%s' already exists, new file renamed to '%s'."))
			% dstName % newName.GetFullName());
	}

	// Move will handle dirs on the same partition, otherwise copy is needed.
	CPath partfilename = m_metPath.RemoveExt();
	if (!CPath::RenameFile(partfilename, newName)) {
		if (!CPath::CloneFile(partfilename, newName, true)) {
			m_error = true;
			return;
		}
		
		if (!CPath::RemoveFile(partfilename)) {
			AddLogLineM(true, logPartFile, CFormat(_("WARNING: Could not remove original '%s' after creating backup"))
				% partfilename);
		}
	}

	// Removes the various other data-files	
	const wxChar* otherMetExt[] = { wxT(""), PARTMET_BAK_EXT, wxT(".seeds"), NULL };
	for (size_t i = 0; otherMetExt[i]; ++i) {
		CPath toRemove = m_metPath.AppendExt(otherMetExt[i]);

		if (toRemove.FileExists()) {
			if (!CPath::RemoveFile(toRemove)) {
				AddLogLineM(true, logPartFile, CFormat(_("WARNING: Failed to delete %s")) % toRemove);
			}
		}
	}

	m_newName = newName;
}


void CCompletionTask::OnExit()
{
	// Notify the app that the completion has finished for this file.
	CCompletionEvent evt(m_error, m_owner, m_newName);
	
	wxPostEvent(wxTheApp, evt);
}




////////////////////////////////////////////////////////////
// CHashingEvent

DEFINE_LOCAL_EVENT_TYPE(MULE_EVT_HASHING)
DEFINE_LOCAL_EVENT_TYPE(MULE_EVT_AICH_HASHING)

CHashingEvent::CHashingEvent(wxEventType type, CKnownFile* result, const CKnownFile* owner)
	: wxEvent(-1, type),
	  m_owner(owner),
	  m_result(result)
{
}


wxEvent* CHashingEvent::Clone() const
{
	return new CHashingEvent(GetEventType(), m_result, m_owner);
}


const CKnownFile* CHashingEvent::GetOwner() const
{
	return m_owner;
}


CKnownFile* CHashingEvent::GetResult() const
{
	return m_result;
}




////////////////////////////////////////////////////////////
// CCompletionEvent

DEFINE_LOCAL_EVENT_TYPE(MULE_EVT_FILE_COMPLETED)


CCompletionEvent::CCompletionEvent(bool errorOccured, const CPartFile* owner, const CPath& fullPath)
	: wxEvent(-1, MULE_EVT_FILE_COMPLETED),
	  m_fullPath(fullPath),
	  m_owner(owner),
	  m_error(errorOccured)
{
}


wxEvent* CCompletionEvent::Clone() const
{
	return new CCompletionEvent(m_error, m_owner, m_fullPath);
}


bool CCompletionEvent::ErrorOccured() const
{
	return m_error;
}

	
const CPartFile* CCompletionEvent::GetOwner() const
{
	return m_owner;
}
	

const CPath& CCompletionEvent::GetFullPath() const
{
	return m_fullPath;
}

// File_checked_for_headers
