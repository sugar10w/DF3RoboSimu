// dll�ӿ�

#pragma once


#include "ai.h"
#include <string>


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
    Player();
    Player(std::string _file_name);
    ~Player();

    bool run(const Info info);
    bool load();

    inline void kill() { valid = false; }
    inline std::string getFileName() const { return file_name; }
    inline bool isValid() const { return valid; }

private:
    // ���ai����
    TPlayerAi   player_ai;
    // ��Ҷ�̬���ӿ��ļ���
    std::string file_name;
    // �Ƿ���Ч
    bool        valid;
    // �ļ���
    __MY_DLLHANDLE hDLL;
};

