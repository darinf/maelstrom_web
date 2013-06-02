
#ifndef _myerror_h
#define _myerror_h

/* Generic error message routines */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <ppapi_main/ppapi_main.h>
#include "ppb.h"


static void WriteToConsole(const char* mesg, PP_LogLevel log_level) {
  ppb.console->Log(
      PPAPI_GetInstanceId(),
      log_level, 
      ppb.var->VarFromUtf8(mesg, strlen(mesg)));
}

void error(const char *fmt, ...)
{
	char mesg[BUFSIZ];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(mesg, fmt, ap);
	WriteToConsole(mesg, PP_LOGLEVEL_ERROR);
	va_end(ap);
}

void mesg(const char *fmt, ...)
{
	char mesg[BUFSIZ];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(mesg, fmt, ap);
	WriteToConsole(mesg, PP_LOGLEVEL_LOG);
	va_end(ap);
}

void myperror(const char *msg)
{
	char buffer[BUFSIZ];

	if ( *msg ) {
		sprintf(buffer, "%s: %s\n", msg, strerror(errno));
		error(buffer);
	} else
		error((char *)strerror(errno));
}

#endif /* _myerror_h */

