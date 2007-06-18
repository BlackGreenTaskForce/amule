//
// This file is part of the aMule Project.
//
// Copyright (c) 2003-2007 aMule Team ( admin@amule.org / http://www.amule.org )
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

#ifndef ED2KC2CUDP_H
#define ED2KC2CUDP_H

// Extended prot client <-> Extended prot client UDP
enum ED2KExtendedClientUDP {
	OP_REASKFILEPING			= 0x90,	// <HASH 16>
	OP_REASKACK					= 0x91,	// <RANG 2>
	OP_FILENOTFOUND				= 0x92,	// (null)
	OP_QUEUEFULL				= 0x93,	// (null)
	OP_REASKCALLBACKUDP			= 0x94,
	OP_PORTTEST					= 0xFE	// Connection Test
};

#endif // ED2KC2CUDP_H
