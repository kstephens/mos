#ifndef __rcs_id__
#ifndef __rcs_id_mos_os_c__
#define __rcs_id_mos_os_c__
static const char __rcs_id_mos_os_c[] = "$Id: os.c,v 1.3 1999-12-26 20:05:58 stephensk Exp $";
#endif
#endif /* __rcs_id__ */

#include "mos/mos.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

mos_ANNOT("Module: os")
mos_ANNOT("Doc: Is the user interface to the host operating system (most ANSI C and POSIX system calls).")

mos_METHOD(os,system_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  mos_return(mos_integer_make(system(mos_string_V(mos_ARGV[0]))));
}
mos_METHOD_END

mos_METHOD(os,exit_)
{
  exit(mos_INT(mos_send(mos_ARGV[0], mos_s(asInteger))));
  mos_return(mos_undef);
}
mos_METHOD_END

mos_METHOD(os,abort)
{
  abort();
  mos_return(mos_undef);
}
mos_METHOD_END

mos_METHOD(os,fork)
{
  mos_return(mos_integer_make(fork()));
}
mos_METHOD_END

mos_METHOD(os,exec_)
{
  int argc;
  char **argv;
  
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asVector));
  
  argc = mos_INT(mos_send(mos_ARGV[0], mos_s(length)));
  argv = alloca(sizeof(argv[0]) + (argc + 1));
  
  {
    int i;
    
    for ( i = 0; i < argc; i ++ ) {
      argv[i] = mos_string_V(mos_send(mos_send(mos_ARGV[0], mos_s(get_), mos_integer_make(i)), mos_s(asString)));
    }
    argv[i] = 0;
  }
  
  mos_return(mos_integer_make(execv(argv[0], argv + 1)));
}
mos_METHOD_END

/*************************************************************************/

mos_METHOD(os,cwd)
{
#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif
  char buf[MAXPATHLEN+1];
  char *v;

#if 0
  v = getwd(buf);
#else
  v = getcwd(buf, sizeof(buf) - 1);
#endif
  mos_return(mos_send(mos_string_make_(v), mos_s(deepen)));
}
mos_METHOD_END

mos_METHOD(os,cwd_)
{
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  mos_return(mos_integer_make(chdir(mos_string_V(mos_ARGV[0]))));
}
mos_METHOD_END


/*************************************************************************/
/*
** Stat
*/
mos_ANNOT("Doc: Represents the C struct stat structure.")
mos_OBJECT(stat)
mos_OBJECT_A(st__dev,0)
mos_OBJECT_A(st__ino,1)
mos_OBJECT_A(st__nlink,2)
mos_OBJECT_A(st__uid,3)
mos_OBJECT_A(st__gid,4)
mos_OBJECT_A(st__rdev,5)
mos_OBJECT_A(st__size,6)
mos_OBJECT_A(st__atime,7)
mos_OBJECT_A(st__mtime,8)
mos_OBJECT_A(st__ctime,9)
mos_OBJECT_A(st__blksize,10)
mos_OBJECT_SLOTS(stat)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_S(mos_undef)
mos_OBJECT_END(protos,stat,mos_object,basicMeta)
mos_ANNOT_END

static
mos_value stat_to_obj(struct stat *s)
{
  mos_value x;
  
  x = mos_send(mos_o(stat), mos_s(clone));
#define SET(X) mos_send(x, mos_s(st__##X##_), mos_integer_make(s->st_##X));
  SET(dev)
  SET(ino)
  SET(nlink)
  SET(uid)
  SET(gid)
  SET(rdev)
  SET(size)
  SET(atime)
  SET(mtime)
  SET(ctime)
  SET(blksize)
#undef SET
  
  return x;
}

mos_METHOD(os,stat_)
{
  struct stat s;
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  mos_return(stat(mos_string_V(mos_ARGV[0]), &s) == 0 ? stat_to_obj(&s) : mos_false);
}
mos_METHOD_END

mos_METHOD(os,lstat_)
{
  struct stat s;
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  mos_return(lstat(mos_string_V(mos_ARGV[0]), &s) == 0 ? stat_to_obj(&s) : mos_false);
}
mos_METHOD_END

mos_METHOD(os,fstat_)
{
  struct stat s;
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asInteger));
  mos_return(fstat(mos_INT(mos_ARGV[0]), &s) == 0 ? stat_to_obj(&s) : mos_false);
}
mos_METHOD_END


/*************************************************************************/

mos_ANNOT("Doc: The object to get the environment variables.")
mos_METHOD(env,get_)
{
  extern const char *getenv(const char *);
  mos_ARGV[0] = mos_send(mos_ARGV[0], mos_s(asString));
  mos_return(mos_string_make_(getenv(mos_string_V(mos_ARGV[0]))));
}
mos_METHOD_END

mos_OBJECT(env)
mos_OBJECT_M(constant,clone)
mos_OBJECT_M(env,get_)
mos_OBJECT_SLOTS(env)
mos_OBJECT_END(os,env,mos_object,basicMeta)
mos_ANNOT_END

/*************************************************************************/

mos_OBJECT(os)
mos_OBJECT_M(os,exit_)
mos_OBJECT_M(os,abort)
mos_OBJECT_M(os,system_)
mos_OBJECT_M(os,fork)
mos_OBJECT_M(os,exec_)
mos_OBJECT_M(os,cwd)
mos_OBJECT_M(os,cwd_)
mos_OBJECT_M(os,stat_)
mos_OBJECT_M(os,lstat_)
mos_OBJECT_M(os,fstat_)
mos_OBJECT_SLOTS(os)
mos_OBJECT_END(system,os,mos_object,basicMeta)

mos_ANNOT_END
mos_ANNOT_END

/*************************************************************************/

mos_INIT(os,80)
{
  /* os stdio {in,out,err} */
  {
    mos_value o;
    
    o = mos_send(mos_o(empty), mos_s(clone));
   
    mos_addSlot(o, mos_s(in), mos_FILE(in));
    mos_name_object_("@O stdio", "in", mos_FILE(in));
    
    mos_addSlot(o, mos_s(out), mos_FILE(out));
    mos_name_object_("@O stdio", "out", mos_FILE(out));
    
    mos_addSlot(o, mos_s(err), mos_FILE(err));
    mos_name_object_("@O stdio", "err", mos_FILE(err));
    
    mos_addSlot(mos_o(os), mos_s(stdio), o);
    
  }

  /* os argv */
  {
    mos_value *v;
    int i;
    
    v = mos_malloc(sizeof(v[0]) * *argcp);
    for ( i = 0; i < *argcp; i ++ ) {
      v[i] = mos_string_make((*argvp)[i], strlen((*argvp)[i]));
    }

    mos_addSlot(mos_o(os), mos_s(argv), mos_vector_make(*argcp, v));
  }

  return 0;
}
