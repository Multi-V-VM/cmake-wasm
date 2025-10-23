// Minimal libuv stubs for WASI to satisfy CMake build-time linking.
#include "uv.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

static uv_loop_t default_loop_storage;
static int default_loop_inited = 0;

int uv_translate_sys_error(int sys_err) {
  // libuv error codes are negative errno values.
  if (sys_err == 0) return 0;
  return -abs(sys_err);
}

int uv_loop_init(uv_loop_t* loop) {
  if (!loop) return UV_EINVAL;
  memset(loop, 0, sizeof(*loop));
  return 0;
}

int uv_run(uv_loop_t* loop, uv_run_mode mode) {
  (void)loop;
  (void)mode;
  return 0;
}

void uv_close(uv_handle_t* handle, uv_close_cb close_cb) {
  if (close_cb) close_cb(handle);
}

int uv_is_closing(const uv_handle_t* handle) {
  (void)handle;
  return 0;
}

// Async
int uv_async_init(uv_loop_t* loop, uv_async_t* async, uv_async_cb cb) {
  (void)loop; (void)async; (void)cb; return 0;
}
int uv_async_send(uv_async_t* async) { (void)async; return 0; }

// Signals
int uv_signal_init(uv_loop_t* loop, uv_signal_t* h) { (void)loop; (void)h; return 0; }
int uv_signal_start(uv_signal_t* h, uv_signal_cb cb, int signum) { (void)h; (void)cb; (void)signum; return 0; }
int uv_signal_stop(uv_signal_t* h) { (void)h; return 0; }

// Idle
int uv_idle_init(uv_loop_t* loop, uv_idle_t* h) { (void)loop; (void)h; return 0; }
int uv_idle_start(uv_idle_t* h, uv_idle_cb cb) { (void)h; (void)cb; return 0; }
int uv_idle_stop(uv_idle_t* h) { (void)h; return 0; }

// Timers
int uv_timer_init(uv_loop_t* loop, uv_timer_t* h) { (void)loop; (void)h; return 0; }
int uv_timer_start(uv_timer_t* h, uv_timer_cb cb, uint64_t timeout, uint64_t repeat) {
  (void)h; (void)cb; (void)timeout; (void)repeat; return 0;
}
int uv_timer_stop(uv_timer_t* h) { (void)h; return 0; }

// TTY & Pipe & Stream
int uv_tty_init(uv_loop_t* loop, uv_tty_t* h, uv_file fd, int readable) {
  (void)loop; (void)h; (void)fd; (void)readable; return 0;
}
int uv_pipe_init(uv_loop_t* loop, uv_pipe_t* h, int ipc) { (void)loop; (void)h; (void)ipc; return 0; }
int uv_pipe_open(uv_pipe_t* h, uv_file fd) { (void)h; (void)fd; return 0; }
int uv_read_stop(uv_stream_t* s) { (void)s; return 0; }
int uv_write(uv_write_t* req, uv_stream_t* handle, const uv_buf_t bufs[], unsigned int nbufs, uv_write_cb cb) {
  (void)req; (void)handle; (void)bufs; (void)nbufs; if (cb) cb(req, 0); return 0;
}

// Process/spawn
int uv_spawn(uv_loop_t* loop, uv_process_t* p, const uv_process_options_t* options) {
  (void)loop; (void)p; (void)options; return UV_ENOSYS;
}
int uv_kill(int pid, int signum) { (void)pid; (void)signum; return UV_ENOSYS; }
int uv_os_getppid(void) { return 0; }
int uv_process_kill(uv_process_t* p, int signum) { (void)p; (void)signum; return UV_ENOSYS; }

// FS APIs (minimal stubs)
int uv_fs_open(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, int mode, uv_fs_cb cb) {
  (void)loop; (void)flags; (void)mode; req->result = UV_ENOSYS; if (cb) cb(req); return UV_ENOSYS;
}
void uv_fs_req_cleanup(uv_fs_t* req) { (void)req; }
int uv_fs_realpath(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) {
  (void)loop; (void)path; req->result = UV_ENOSYS; if (cb) cb(req); return UV_ENOSYS;
}
int uv_fs_stat(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb) { (void)loop; (void)path; req->result = UV_ENOSYS; if (cb) cb(req); return UV_ENOSYS; }
int uv_fs_symlink(uv_loop_t* loop, uv_fs_t* req, const char* a, const char* b, int flags, uv_fs_cb cb) {
  (void)loop; (void)a; (void)b; (void)flags; req->result = UV_ENOSYS; if (cb) cb(req); return UV_ENOSYS;
}
int uv_fs_link(uv_loop_t* loop, uv_fs_t* req, const char* a, const char* b, uv_fs_cb cb) {
  (void)loop; (void)a; (void)b; req->result = UV_ENOSYS; if (cb) cb(req); return UV_ENOSYS;
}
int uv_fs_get_system_error(const uv_fs_t* req) { (void)req; return UV_ENOSYS; }

// Time
int uv_gettimeofday(uv_timeval64_t* tv) {
  if (!tv) return UV_EINVAL;
  tv->tv_sec = 0; tv->tv_usec = 0; return 0;
}

// Misc
void uv_disable_stdio_inheritance(void) { }
int uv_cpumask_size(void) { return 0; }
int uv_is_active(const uv_handle_t* h) { (void)h; return 0; }
int uv_is_readable(const uv_stream_t* s) { (void)s; return 0; }
int uv_is_writable(const uv_stream_t* s) { (void)s; return 0; }
