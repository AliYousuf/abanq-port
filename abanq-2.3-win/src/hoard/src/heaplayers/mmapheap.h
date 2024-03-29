/* -*- C++ -*- */

/*

  Heap Layers: An Extensible Memory Allocation Infrastructure
  
  Copyright (C) 2000-2004 by Emery Berger
  http://www.cs.umass.edu/~emery
  emery@cs.umass.edu
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#ifndef _MMAPHEAP_H_
#define _MMAPHEAP_H_

#if defined(_WIN32)
#include <windows.h>
#else
// UNIX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
//#include <sys/mman.h>
#include <stdio.h>
#include <map>
#endif

#include "sassert.h"
#include "myhashmap.h"
#include "stlallocator.h"
#include "freelistheap.h"
#include "zoneheap.h"
#include "lockedheap.h"
#include "spinlock.h"
#include "hldefines.h"

#if HL_EXECUTABLE_HEAP
#define HL_MMAP_PROTECTION_MASK (PROT_READ | PROT_WRITE | PROT_EXEC)
#else
#define HL_MMAP_PROTECTION_MASK (PROT_READ | PROT_WRITE)
#endif

#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
#define MAP_ANONYMOUS MAP_ANON
#endif


#include <new>

#define _WIN32
#include <windows.h>

/**
 * @class MmapHeap
 * @brief A "source heap" that manages memory via calls to the VM interface.
 * @author Emery Berger
 */

namespace HL {

  class PrivateMmapHeap {
  public:

    /// All memory from here is zeroed.
    enum { ZeroMemory = 1 };

#if defined(_WIN32) 

    // Microsoft Windows aligns all memory to a 64K boundary.
    enum { Alignment = 64 * 1024 };

    inline void * malloc (size_t sz) {
      //    printf ("mmapheap: Size request = %d\n", sz);
#if HL_EXECUTABLE_HEAP
      char * ptr = (char *) VirtualAlloc (NULL, sz, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE);
#else
      char * ptr = (char *) VirtualAlloc (NULL, sz, MEM_RESERVE | MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE);
#endif
      return (void *) ptr;
    }
  
    inline void free (void * ptr, size_t) {
      VirtualFree (ptr, 0, MEM_RELEASE);
    }

    inline void free (void * ptr) {
      VirtualFree (ptr, 0, MEM_RELEASE);
    }
  
    inline static size_t getSize (void * ptr) {
      MEMORY_BASIC_INFORMATION mbi;
      VirtualQuery (ptr, &mbi, sizeof(mbi));
      return (size_t) mbi.RegionSize;
    }

#else

    virtual ~PrivateMmapHeap (void) {}

#if defined(__SVR4)

    // Solaris aligns all memory to a 64K boundary.
    enum { Alignment = 64 * 1024 };
#else
    // Linux and most other operating systems align memory to a 4K boundary.
    enum { Alignment = 4 * 1024 };
#endif

    inline void * malloc (size_t sz) {
#if defined(MAP_ALIGN) && defined(MAP_ANON)
      // Request memory aligned to the Alignment value above.
      void * ptr = mmap ((char *) Alignment, sz, HL_MMAP_PROTECTION_MASK, MAP_PRIVATE | MAP_ALIGN | MAP_ANON, -1, 0);
#elif !defined(MAP_ANONYMOUS)
      static int fd = ::open ("/dev/zero", O_RDWR);
      void * ptr = mmap (NULL, sz, HL_MMAP_PROTECTION_MASK, MAP_PRIVATE, fd, 0);
#else
      void * ptr = mmap (NULL, sz, HL_MMAP_PROTECTION_MASK, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
      if (ptr == MAP_FAILED) {
	ptr = NULL;
      }
      return ptr;
    }
    
#if 1
    inline void free (void * ptr) {
      abort();
      munmap (reinterpret_cast<char *>(ptr), getSize(ptr));
    }

    inline size_t getSize (void * ptr) {
      ptr = ptr;
      abort();
      return Alignment; // Obviously broken. Such is life.
    }
#endif

    void free (void * ptr, size_t sz)
    {
      if ((long) sz < 0) {
	abort();
      }

      munmap (reinterpret_cast<char *>(ptr), sz);
    }

#endif

  };


  class MmapHeap : public PrivateMmapHeap {
#if !defined(_WIN32)

  private:

    class MyHeap : public LockedHeap<SpinLockType, FreelistHeap<ZoneHeap<PrivateMmapHeap, 16384 - 16> > > {}; // FIX ME: 16 = size of ZoneHeap header.

    typedef MyHashMap<void *, size_t, MyHeap> mapType;

  protected:
    mapType MyMap;

    SpinLockType MyMapLock;

  public:
    inline void * malloc (size_t sz) {
      void * ptr = PrivateMmapHeap::malloc (sz);
      MyMapLock.lock();
      MyMap.set (ptr, sz);
      MyMapLock.unlock();
      assert (reinterpret_cast<size_t>(ptr) % Alignment == 0);
      return const_cast<void *>(ptr);
    }

    inline size_t getSize (void * ptr) {
      MyMapLock.lock();
      size_t sz = MyMap.get (ptr);
      MyMapLock.unlock();
      return sz;
    }

    // WORKAROUND: apparent gcc bug.
    void free (void * ptr, size_t sz) {
      PrivateMmapHeap::free (ptr, sz);
    }

    inline void free (void * ptr) {
      assert (reinterpret_cast<size_t>(ptr) % Alignment == 0);
      MyMapLock.lock();
      size_t sz = MyMap.get (ptr);
      PrivateMmapHeap::free (ptr, sz);
      MyMap.erase (ptr);
      MyMapLock.unlock();
    }
#endif
  };

}

#endif
