
#ifndef _myerror_h
#define _myerror_h

/* Generic error message routines */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <ppapi_simple/ps_instance.h>
#include "ppb.h"

extern PP_Instance g_instance;

static void WriteToConsole(const char* message, PP_LogLevel log_level) {
  if (!g_instance)
    return;
  PP_Var var = ppb.var->VarFromUtf8(message, strlen(message));
  ppb.console->Log(g_instance, log_level, var);
  ppb.var->Release(var);
}

void error(const char *fmt, ...)
{
	char buf[1024];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	WriteToConsole(buf, PP_LOGLEVEL_ERROR);
	va_end(ap);
}

void mesg(const char *fmt, ...)
{
	char buf[1024];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	WriteToConsole(buf, PP_LOGLEVEL_LOG);
	va_end(ap);
}

void myperror(const char *msg)
{
	char buffer[1024];

	if ( *msg ) {
		sprintf(buffer, "%s: %s\n", msg, strerror(errno));
		error(buffer);
	} else
		error((char *)strerror(errno));
}

#endif /* _myerror_h */

