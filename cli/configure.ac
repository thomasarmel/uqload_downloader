AC_INIT([uqload_downloader], [1.0])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([Downloader.cpp], [uqload_downloader.cpp])
AC_CHECK_LIB([curl], [curl_easy_setopt], [echo "libcurl library is present"], [AC_MSG_ERROR(["libcurl library is not present, please consider installing libcurl-openssl / libcurl-gnutls"])] )
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC
AC_PROG_CXX
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
