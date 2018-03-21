// dll接口

#pragma once


#include "ai.h"
#include <string>
#include "pthread.h"
#include <csignal>

#ifdef _MSC_VER

// Du 2017.2.6
// EXCLUDE windows sockets
#define WIN32_LEAN_AND_MEAN
#include"windows.h"
#define __MY_LOADDLL(x) LoadLibraryA(x)
#define __MY_CLOSEDLL(x) FreeLibrary(x)
#define __MY_GETFUNC GetProcAddress
typedef HMODULE __MY_DLLHANDLE;

#endif

#ifdef __GNUC__

#include<dlfcn.h>
#define __MY_LOADDLL(x) dlopen(x, RTLD_LAZY)
#define __MY_CLOSEDLL(x) dlclose(x)
#define __MY_GETFUNC dlsym
typedef void* __MY_DLLHANDLE;

#endif

typedef PlayerControl(*TPlayerAi)(const Info info);

class Player 
{
public:
    Player(std::string _file_name);
    ~Player();

    PlayerControl run(const Info info);
    PlayerControl timedRun(const Info info, int msecond);
    bool load();

    void kill();

    inline std::string getFileName() const { return file_name; }
    inline bool isValid() const { return valid; }

    // 玩家ai函数
    TPlayerAi   player_ai;

private:
    // 玩家动态链接库文件名
    std::string file_name;
    // 是否有效
    bool        valid;
    // 文件柄
    __MY_DLLHANDLE hDLL;
};


typedef struct {
    Player* player;
    int* flag;
    PlayerControl* pc;
    const Info* info;
}THREAD_PARAM;
