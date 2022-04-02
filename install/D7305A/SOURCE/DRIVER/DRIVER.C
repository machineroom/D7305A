/*{{{  module header */
/*
-- ----------------------------------------------------------------------------
--
--     SCCS ID : @(#)  driver.c 1.2 92/12/18 16:06:51
--
--     Object Name : driver.c
--     Revision    : 1.2
--
--     (c) Copyright INMOS Limited.
--     All Rights Reserved.
--
--     DESCRIPTION
--         Main entry point for the INMOS driver program.
--
--     NOTES
--         None.
--
--     HISTORY
--         1-May-1990    Antony King    Created file.
--
-- ----------------------------------------------------------------------------
*/

static char SccsId[] = "@(#)  driver.c 1.2 92/12/18 16:06:51 Copyright INMOS Limited";
/*}}}*/

/*{{{  include files */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include <ctype.h>
#include <errno.h>
#include <string.h>

#ifdef _MSDOS
#ifndef MSDOS
#define MSDOS
#endif
#endif

#ifdef MSDOS
#include <process.h>
#endif

#ifdef VMS
#include <processes.h>
#endif
/*}}}*/

/*{{{  macro definitions */
#ifdef MSDOS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#if (_MSC_VER >= 700)
#define EXEC _execvpe
#else
#define EXEC execvpe
#endif
#endif

#ifdef VMS
#define EXIT_SUCCESS 1
#define EXIT_FAILURE 42

#define EXEC execve
#endif

#define SERVER_DEF "iserver"
#define SERVER_ENV "ISERVER"

#define SA "/sa"
#define SB "/sb"
#define SC "/sc"
#define SE "/se"
#define SI "/si"
#define SL "/sl"
#define SP "/sp"
#define SR "/sr"
#define SS "/ss"

#define EXTENSION ".btl"

#define IDUMP  "idump.btl"
#define IDEBUG "idebug.btl"
/*}}}*/

/*{{{  static procedures */
/*{{{  SizeArguments */
static int SizeArguments (int Argc, va_list Arguments)
{
    while (va_arg(Arguments, char *) != NULL)
        Argc++;

    return(Argc);
}
/*}}}*/

/*{{{  CopyArguments */
static int CopyArguments (int Argc, char **Argv, va_list Arguments)
{
    while ((Argv[Argc] = va_arg(Arguments, char *)) != NULL)
        Argc++;

    return(Argc);
}
/*}}}*/

/*{{{  MakeArguments */
static char **MakeArguments (char *Program, int Argc, char **Argv, ...)
{
    int NewArgc = 0;
    va_list Arguments;
    char **NewArgv = NULL;

    va_start(Arguments, Argv);
    NewArgv = (char **) malloc((SizeArguments(Argc + 1, Arguments) + 1) * sizeof(char *));
    NewArgv[NewArgc++] = Program;
    NewArgc = CopyArguments(NewArgc, NewArgv, Arguments);
    while (*Argv != NULL)
        NewArgv[NewArgc++] = *Argv++;
    NewArgv[NewArgc] = *Argv;
    va_end(Arguments);

    return(NewArgv);
}
/*}}}*/

/*{{{  MakeExtension */
static char *MakeExtension (char *Path, char *Extension)
{
    int Index = 0;
    char *NewPath = NULL;

    if ((NewPath = strrchr(Path, '.')) == NULL)
        Index = strlen(Path);
    else
        Index = NewPath - Path;

    NewPath = (char *) malloc(((Index + strlen(Extension)) + 1) * sizeof(char));

    memcpy(NewPath, Path, Index);
    strcpy(&NewPath[Index], Extension);

    for (Index = 0; NewPath[Index] != '\0'; Index++)
    {
        if (isupper(NewPath[Index]))
            NewPath[Index] = (char) tolower(NewPath[Index]);
        else if (NewPath[Index] == '/')
            NewPath[Index] = '\\';
    }

    return(NewPath);
}
/*}}}*/
/*}}}*/

/*{{{  global procedures */
/*{{{  main */
int main (int Argc, char **Argv, char **Envp)
{
    char *Server = NULL, *Tool = NULL;

    if ((Server = getenv(SERVER_ENV)) == NULL)
        Server = SERVER_DEF;

    Tool = MakeExtension(Argv[0], EXTENSION);

    if (strstr(Tool, IDEBUG) == NULL)
    {
        if (strstr(Tool, IDUMP) == NULL)
            Argv = MakeArguments(Server, Argc - 1, &Argv[1], SE, SR, SS, SC, Tool, NULL);
        else
            Argv = MakeArguments(Server, Argc - 1, &Argv[1], SA, SS, SC, Tool, NULL);
    }
    else
    {
        if (Argc == 1)
            Argv = MakeArguments(Server, Argc - 1, &Argv[1], SE, SR, SS, SC, Tool, NULL);
        else
            Argv = MakeArguments(Server, Argc - 1, &Argv[1], SS, SC, Tool, NULL);
    }

    if (EXEC(Server, Argv, Envp) == -1)
    {
        fprintf(stderr, "Fatal-driver- unable to execute '%s', %s\n", Server, strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
        exit(EXIT_SUCCESS);

    return(EXIT_SUCCESS);
}
/*}}}*/
/*}}}*/
