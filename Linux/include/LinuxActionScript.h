/*
 * LinuxActionScript.h
 *
 *  Created on: 2011. 1. 18.
 *      Author: zerom
 */

#ifndef LINUXACTIONSCRIPT_H_
#define LINUXACTIONSCRIPT_H_

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <pthread.h>

namespace Robot
{
class LinuxActionScript
{
private:
    static const int LINE_BUFFERSIZE = 512;
    static pthread_t m_pthread_id;
    static pid_t mp3_pid;

    static char* SkipLeading(const char* str);
    static int ParseLine(const char* linebuffer, int* pagenumber, char* filepath);

    static void* ScriptThreadProc(void* data);

public:
    static bool m_stop;
    static bool m_is_running;

    static int ScriptStart(const char* filename);
    static int PlayMP3Wait(const char* filename);
    static int PlayMP3(const char* filename);
    static int GetPlayable() {
        int status;
        return waitpid(mp3_pid, &status, 0);
    }
};
}

#endif /* LINUXACTIONSCRIPT_H_ */
