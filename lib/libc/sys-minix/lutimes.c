#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#ifdef __weak_alias
__weak_alias(lutimes, __lutimes50)
#endif

int lutimes(const char *name, const struct timeval tv[2])
{
  message m;

  if (name == NULL) {
	errno = EINVAL;
	return -1;
  }
  if (name[0] == '\0') { /* X/Open requirement */
	errno = ENOENT;
	return -1;
  }
  memset(&m, 0, sizeof(m));
  m.VFS_UTIMENS_FD = strlen(name) + 1;
  m.VFS_UTIMENS_NAME = (char *) __UNCONST(name);
  if (tv == NULL) {
	m.VFS_UTIMENS_ATIME = m.VFS_UTIMENS_MTIME = 0;
	m.VFS_UTIMENS_ANSEC = m.VFS_UTIMENS_MNSEC = UTIME_NOW;
  }
  else {
	/* For now just truncate time_t values to 32bits. */
	m.VFS_UTIMENS_ATIME = (int32_t)tv[0].tv_sec;
	m.VFS_UTIMENS_MTIME = (int32_t)tv[1].tv_sec;
	m.VFS_UTIMENS_ANSEC = (int32_t)tv[0].tv_usec * 1000;
	m.VFS_UTIMENS_MNSEC = (int32_t)tv[1].tv_usec * 1000;
  }
  m.VFS_UTIMENS_FLAGS = AT_SYMLINK_NOFOLLOW;

  return(_syscall(VFS_PROC_NR, VFS_UTIMENS, &m));
}
