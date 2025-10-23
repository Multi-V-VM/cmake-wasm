/* Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
   file LICENSE.rst or https://cmake.org/licensing for details.  */
#include <cerrno> // errno
#include <cstdio> // SEEK_SET

#include <fcntl.h>
#include <unistd.h>

#include "cmFileLock.h"
#include "cmSystemTools.h"

cmFileLock::cmFileLock() = default;

cmFileLockResult cmFileLock::Release()
{
#ifdef __wasi__
  // WASI: advisory locks not available; treat as success.
  this->Filename = "";
  if (this->File >= 0) {
    ::close(this->File);
    this->File = -1;
  }
  return cmFileLockResult::MakeOk();
#else
  if (this->Filename.empty()) {
    return cmFileLockResult::MakeOk();
  }
  int const lockResult = this->LockFile(F_SETLK, F_UNLCK);

  this->Filename = "";

  ::close(this->File);
  this->File = -1;

  if (lockResult == 0) {
    return cmFileLockResult::MakeOk();
  }
  return cmFileLockResult::MakeSystem();
#endif
}

cmFileLockResult cmFileLock::OpenFile()
{
  this->File = ::open(this->Filename.c_str(), O_RDWR);
  if (this->File == -1) {
    return cmFileLockResult::MakeSystem();
  }
  return cmFileLockResult::MakeOk();
}

cmFileLockResult cmFileLock::LockWithoutTimeout()
{
#ifdef __wasi__
  return cmFileLockResult::MakeOk();
#else
  if (this->LockFile(F_SETLKW, F_WRLCK) == -1) {
    return cmFileLockResult::MakeSystem();
  }
  return cmFileLockResult::MakeOk();
#endif
}

cmFileLockResult cmFileLock::LockWithTimeout(unsigned long seconds)
{
#ifdef __wasi__
  (void)seconds;
  return cmFileLockResult::MakeOk();
#else
  while (true) {
    if (this->LockFile(F_SETLK, F_WRLCK) == -1) {
      if (errno != EACCES && errno != EAGAIN) {
        return cmFileLockResult::MakeSystem();
      }
    } else {
      return cmFileLockResult::MakeOk();
    }
    if (seconds == 0) {
      return cmFileLockResult::MakeTimeout();
    }
    --seconds;
    cmSystemTools::Delay(1000);
  }
#endif
}

int cmFileLock::LockFile(int cmd, int type) const
{
#ifdef __wasi__
  (void)cmd; (void)type;
  return 0;
#else
  struct ::flock lock;
  lock.l_start = 0;
  lock.l_len = 0;                         // lock all bytes
  lock.l_pid = 0;                         // unused (for F_GETLK only)
  lock.l_type = static_cast<short>(type); // exclusive lock
  lock.l_whence = SEEK_SET;
  return ::fcntl(this->File, cmd, &lock);
#endif
}
