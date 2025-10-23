/* WASI stubs for libarchive disk and filter forking APIs to satisfy link. */
#include "archive_platform.h"
#include "archive.h"
#include "archive_entry.h"
#include "filter_fork.h"

#ifdef __wasi__

/* Disk read API stubs */
struct archive* archive_read_disk_new(void)
{
  /* Return a basic archive reader; callers may still free it. */
  return archive_read_new();
}

int archive_read_disk_entry_from_file(struct archive* a,
                                      struct archive_entry* e,
                                      int fd,
                                      const struct stat* st)
{
  (void)a; (void)e; (void)fd; (void)st;
  /* No-op: report success so callers can proceed; metadata may be missing. */
  return ARCHIVE_OK;
}

int archive_read_disk_set_gname_lookup(struct archive* a,
                                       void* private_data,
                                       const char* (*lookup)(void*, la_int64_t),
                                       void (*cleanup)(void*))
{
  (void)a; (void)private_data; (void)lookup; (void)cleanup;
  return ARCHIVE_OK;
}

int archive_read_disk_set_uname_lookup(struct archive* a,
                                       void* private_data,
                                       const char* (*lookup)(void*, la_int64_t),
                                       void (*cleanup)(void*))
{
  (void)a; (void)private_data; (void)lookup; (void)cleanup;
  return ARCHIVE_OK;
}

/* Disk write API stubs */
struct archive* archive_write_disk_new(void)
{
  /* Return a basic archive writer; callers may still free it. */
  return archive_write_new();
}

int archive_write_disk_set_options(struct archive* a, int flags)
{
  (void)a; (void)flags; return ARCHIVE_OK;
}

/* External filter forking API stubs */
int __archive_create_child(const char* cmd, int* child_stdin, int* child_stdout,
#if defined(_WIN32) && !defined(__CYGWIN__)
                           HANDLE* out_child)
#else
                           pid_t* out_child)
#endif
{
  (void)cmd;
  if (child_stdin) *child_stdin = -1;
  if (child_stdout) *child_stdout = -1;
#if defined(_WIN32) && !defined(__CYGWIN__)
  if (out_child) *out_child = NULL;
#else
  if (out_child) *out_child = (pid_t)-1;
#endif
  return ARCHIVE_FAILED;
}

void __archive_check_child(int in, int out)
{
  (void)in; (void)out; /* no-op */
}

#endif /* __wasi__ */

