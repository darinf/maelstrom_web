
#ifndef _myerror_h
#define _myerror_h

/* Generic error message routines */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>


void error(const char *fmt, ...)
{
	char mesg[BUFSIZ];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(mesg, fmt, ap);
	fputs(mesg, stderr);
	va_end(ap);
}

void mesg(const char *fmt, ...)
{
	char mesg[BUFSIZ];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(mesg, fmt, ap);
	fputs(mesg, stdout);
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

