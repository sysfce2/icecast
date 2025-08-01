AC_INIT([Icecast], [2.4.99.3], [icecast@xiph.org])

AC_PREREQ([2.54])

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AC_PROG_CC

AX_CHECK_COMPILE_FLAG([-std=c99], [
  AX_APPEND_FLAG([-std=c99])
], [
  AC_MSG_WARN([Compiler does not accept -std=c99 flag!])
])

AC_PROG_CC_C99

AS_IF([test "${ac_cv_prog_cc_c99}" = "no"], [
  AC_MSG_ERROR([No C99 compiler found!])
])

dnl With clang, we want an error for unknown flags instead of just warn
AX_APPEND_COMPILE_FLAGS([-Werror=unknown-warning-option -Werror=invalid-command-line-argument])

AX_APPEND_COMPILE_FLAGS([-Wall -Wextra -Wno-unused-parameter])

AC_SYS_LARGEFILE
AC_DEFINE([_GNU_SOURCE], 1, [Define to include GNU extensions to POSIX])

dnl Set build/host to default values
AC_CANONICAL_HOST
AC_CANONICAL_BUILD

dnl Check for which host we are compiling
AS_CASE("${host_os}",
  [linux*], [SYS="linux"],
  [darwin*], [SYS="darwin"],
  [SYS="${host_os}"]
)

AM_INIT_AUTOMAKE([tar-ustar foreign dist-zip subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AM_MAINTAINER_MODE([enable])

LT_INIT


dnl Sanitizer flags

AC_ARG_WITH([sanitizer],
    [AS_HELP_STRING([--with-sanitizer=(address/memory/undefined/thread)],
        [build with sanitizer flags (default disabled)])],
     [],
     [with_sanitizer=no])

AS_VAR_IF(with_sanitizer, no, [], [
    AX_CHECK_COMPILE_FLAG([-fsanitize=${with_sanitizer}], [
        AX_APPEND_FLAG([-fsanitize=${with_sanitizer}])
        AX_APPEND_FLAG([-fsanitize=${with_sanitizer}], [LDFLAGS])
    ], [
        AC_MSG_ERROR(["-fsanitize=${with_sanitizer} not supported!"])
    ])
    AX_APPEND_FLAG([-g])

    AS_CASE("${with_sanitizer}",
      [address], [
        AX_APPEND_COMPILE_FLAGS([-fsanitize-address-use-after-scope -fno-omit-frame-pointer -fsanitize=pointer-compare -fsanitize=pointer-subtract])
      ],
      [memory], [
        AX_APPEND_COMPILE_FLAGS([-fPIE -pie])
      ],
      [thread], [
        AX_APPEND_COMPILE_FLAGS([-fPIE -pie])
      ],
      []
    )
])

dnl Check for attributes
AX_GCC_TYPE_ATTRIBUTE([transparent_union])

dnl Checks for header files.
AC_HEADER_ASSERT
AC_HEADER_STDC
AC_HEADER_TIME

AC_CHECK_HEADERS([stdint.h inttypes.h], [
  ice_found_int_headers="yes";
  break;
])

AS_IF([test "$ice_found_int_headers" != "yes"], [
  AC_MSG_ERROR([Unable to find the standard integers headers])
])

AC_CHECK_HEADERS([sys/types.h])
AC_CHECK_HEADERS([sys/timeb.h])
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([pwd.h grp.h])
AC_CHECK_HEADERS([sys/resource.h])
AC_CHECK_HEADERS([crypt.h])
AC_CHECK_HEADERS([spawn.h])

AC_C_BIGENDIAN

XIPH_NET

dnl Check for functions
AC_FUNC_FORK
AC_FUNC_CHOWN

AC_CHECK_FUNCS([setuid])
AC_CHECK_FUNCS([chroot])
AC_CHECK_FUNCS([strcasestr])
AC_CHECK_FUNCS([gethostname])
AC_CHECK_FUNCS([uname])
AC_CHECK_FUNCS([setenv])
AC_CHECK_FUNCS([setresuid])
AC_CHECK_FUNCS([setresgid])
AC_CHECK_FUNCS([localtime_r])
AC_CHECK_FUNCS([gettimeofday])
AC_CHECK_FUNCS([ftime])
AC_CHECK_FUNCS([getrlimit])

dnl Checked only for reporting in version display as of now (may be used in future versions):
AC_CHECK_FUNCS([pipe pipe2 socketpair posix_spawn posix_spawnp])
AC_CHECK_FUNCS([posix_fadvise posix_fallocate posix_madvise])
AC_CHECK_FUNCS([fallocate ftruncate])

dnl check for crypt():
AC_SEARCH_LIBS([crypt_r], [crypt], [
    AC_DEFINE([HAVE_CRYPT_R], [1], [Define if you have crypt_r])
  ], [
  AC_SEARCH_LIBS([crypt], [crypt], [
    AC_DEFINE([HAVE_CRYPT], [1], [Define if you have crypt])
  ])
])

dnl Do not check for poll on Darwin, it is broken in some versions
AS_IF([test "${SYS}" != "darwin"], [
  AC_CHECK_FUNCS([poll])
])

AC_SEARCH_LIBS([nanosleep], [rt posix4], [
  AC_DEFINE([HAVE_NANOSLEEP], [1], [Define if you have nanosleep])
])

dnl Checks for types and typedefs
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_CHECK_TYPES([sig_atomic_t], [], [], [[#include <signal.h>]])

dnl Checks for required libraries

dnl
dnl libigloo
dnl
PKG_CHECK_MODULES([LIBIGLOO], [igloo >= 0.9.2], [], [
  AC_MSG_ERROR([${LIBIGLOO_PKG_ERRORS}. libigloo (v0.9.2 or newer) is required.])
])

CFLAGS="${CFLAGS} ${LIBIGLOO_CFLAGS}"
LIBS="${LIBS} ${LIBIGLOO_LIBS}"


dnl
dnl libxml2
dnl
PKG_CHECK_MODULES([LIBXML2], [libxml-2.0], [], [
  AC_MSG_ERROR([${LIBXML2_PKG_ERRORS}. libxml2 is required.])
])

CFLAGS="${CFLAGS} ${LIBXML2_CFLAGS}"
LIBS="${LIBS} ${LIBXML2_LIBS}"


dnl
dnl libxslt
dnl
PKG_CHECK_MODULES([LIBXSLT], [libxslt], [], [
  AC_MSG_ERROR([${LIBXSLT_PKG_ERRORS}. libxslt is required.])
])

CFLAGS="${CFLAGS} ${LIBXSLT_CFLAGS}"
LIBS="${LIBS} ${LIBXSLT_LIBS}"


dnl
dnl libvorbis
dnl
PKG_CHECK_MODULES([VORBIS], [vorbis >= 1.0], [], [
  AC_MSG_ERROR([${VORBIS_PKG_ERRORS}. Must have libvorbis v1.0 or above installed.])
])

CFLAGS="${CFLAGS} ${VORBIS_CFLAGS}"
LIBS="${LIBS} ${VORBIS_LIBS}"


dnl Checks for optional libraries

dnl
dnl libogg
dnl
PKG_HAVE_WITH_MODULES([OGG], [ogg], [
  CFLAGS="${CFLAGS} ${OGG_CFLAGS}"
  LIBS="${LIBS} ${OGG_LIBS}"
])

dnl
dnl libtheora
dnl
PKG_HAVE_WITH_MODULES([THEORA], [theora], [
  CFLAGS="${CFLAGS} ${THEORA_CFLAGS}"
  LIBS="${LIBS} ${THEORA_LIBS}"
])

dnl
dnl libspeex
dnl
PKG_HAVE_WITH_MODULES([SPEEX], [speex], [
  CFLAGS="${CFLAGS} ${SPEEX_CFLAGS}"
  LIBS="${LIBS} ${SPEEX_LIBS}"
])

dnl
dnl libcurl
dnl
PKG_HAVE_WITH_MODULES([CURL], [libcurl], [
  CFLAGS="${CFLAGS} ${CURL_CFLAGS}"
  LIBS="${LIBS} ${CURL_LIBS}"
])

dnl
dnl openssl
dnl
PKG_HAVE_WITH_MODULES([OPENSSL], [openssl >= 1.1.0], [
  CFLAGS="${CFLAGS} ${OPENSSL_CFLAGS}"
  LIBS="${LIBS} ${OPENSSL_LIBS}"
])

dnl
dnl libmaxminddb
dnl
PKG_HAVE_WITH_MODULES([MAXMINDDB], [libmaxminddb >= 1.3.2], [
  CFLAGS="${CFLAGS} ${MAXMINDDB_CFLAGS}"
  LIBS="${LIBS} ${MAXMINDDB_LIBS}"
])

dnl
dnl librhash - first try pkgconfig and then basic search
dnl since the function is defined in rhash.h we need to check for that first,
dnl before we can check for the function, bit crude but seems to work.
dnl

PKG_CHECK_MODULES([LIBRHASH], [librhash], [
  CFLAGS="${CFLAGS} ${LIBRHASH_CFLAGS}"
  LIBS="${LIBS} ${LIBRHASH_LIBS}"
], [
    AC_CHECK_HEADER([rhash.h], [
      AC_CHECK_LIB(rhash, rhash_library_init, [], [
        AC_MSG_ERROR([${LIBRHASH_PKG_ERRORS}. librhash is required.])
      ])
    ], [
      AC_MSG_ERROR([${LIBRHASH_PKG_ERRORS}. librhash is required.])
    ])
])


dnl Check for library-specific functions
AC_CHECK_FUNCS([xsltSaveResultToString])

dnl we still use format_kate as it doesn't need libkate to work
#ICECAST_OPTIONAL="$ICECAST_OPTIONAL format_kate.o"

ACX_PTHREAD([], [AC_MSG_ERROR([POSIX threads missing])])

CFLAGS="${CFLAGS} ${PTHREAD_CFLAGS}"
CPPFLAGS="${CPPFLAGS} ${PTHREAD_CPPFLAGS}"
LIBS="${LIBS} ${PTHREAD_LIBS}"

dnl Check functions requiring -pthread:
AC_CHECK_FUNCS([pthread_setname_np])

dnl Feature enable/disable arguments

AC_ARG_ENABLE([yp],
  AS_HELP_STRING([--enable-yp],
    [enable yellowpage directory listing support (default: auto)])
)

AS_IF([test "x$enable_yp" != "xno"], [
  AS_IF([test "x$have_curl" != "xyes"], [
    AS_IF([test "x$enable_yp" == "xyes"], [
      AC_MSG_ERROR([cURL is required for YP support])
    ])
    enable_yp="no"
  ], [
    AC_DEFINE([USE_YP], 1, [Define to compile in YP support code])
    enable_yp="yes"
  ])
])

AM_CONDITIONAL([ENABLE_YP],
               [test "x$enable_yp" = "xyes"])


AC_ARG_ENABLE([client-tests],
  AS_HELP_STRING([--enable-client-tests],
    [enable client tests module (default: disabled)])
)

AS_IF([test "x$enable_client_tests" == "xyes"], [
  AC_DEFINE([ENABLE_MODULE_CLIENT_TESTS], 1, [Define to compile client test module])
], [enable_client_tests="no"])

AM_CONDITIONAL([ENABLE_MODULE_CLIENT_TESTS],
               [test "x$enable_client_tests" = "xyes"])

AC_ARG_WITH([default-config],
  AS_HELP_STRING([--with-default-config=PATH],
    [Sets default Icecast configuration file (used when no configuration file is given)]),
  [], [with_default_config=no]
)

AS_IF([test "$with_default_config" != "no"], [

  AS_IF([test "$with_default_config" = "yes"], [
    AC_MSG_ERROR([You need to supply a path as value for --with-default-config])
  ])

  AC_DEFINE_UNQUOTED([ICECAST_DEFAULT_CONFIG], ["$with_default_config"], [Define to default config file path])
  
])

AC_ARG_ENABLE([devel-logging],
  AS_HELP_STRING([--enable-devel-logging],
    [enable development logging (default: disabled)])
)

AS_IF([test "x$enable_devel_logging" == "xyes"], [
  AC_DEFINE([DEVEL_LOGGING], 1, [Define to enable development logging])
], [enable_devel_logging="no"])

dnl Make substitutions
AC_SUBST(XIPH_LIBS)
AC_SUBST(XIPH_CPPFLAGS)
AC_SUBST(XIPH_CFLAGS)
AC_SUBST(XIPH_LDFLAGS)

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([
  Makefile
  conf/Makefile
  src/Makefile
  src/common/avl/Makefile
  src/common/httpp/Makefile
  src/common/thread/Makefile
  src/common/log/Makefile
  src/common/net/Makefile
  src/common/timing/Makefile
  doc/Makefile
  web/Makefile
  admin/Makefile
  win32/Makefile
  examples/Makefile
  tests/Makefile
])

AC_OUTPUT

AS_ECHO(["
Icecast configuration
---------------------
Version        : ${VERSION}
cURL           : ${have_curl}
TLS (openSSL)  : ${have_openssl}

Format/Codec support:
  Ogg          : ${have_ogg}
  Theora       : ${have_theora}
  Speex        : ${have_speex}

Features:
  YP support   : ${enable_yp}
  Client tests : ${enable_client_tests}

Development logging: ${enable_devel_logging}"])
