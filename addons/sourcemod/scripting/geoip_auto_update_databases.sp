
#include    <   sourcemod       >
#include    <   GeoResolver     >

public  Plugin  myinfo  =
{
    name            =   "GeoResolver: Auto Updater"                             ,
    author          =   "Hattrick HKS (claudiuhks)"                             ,
    description     =   "Updates The MaxMind® Databases During Map Change"      ,
    version         =   __DATE__                                                ,
    url             =   "https://forums.alliedmods.net/showthread.php?t=267805" ,
};

public void OnPluginStart()
{
    HookEventEx ("player_connect",          OnUserJoin_Pre, EventHookMode_Pre);
    HookEventEx ("player_connect_client",   OnUserJoin_Pre, EventHookMode_Pre);
    HookEventEx ("player_client_connect",   OnUserJoin_Pre, EventHookMode_Pre);
}

public bool OnClientConnect(int nUser, char[] szMsg, int nMsgMaxLen)
{
    static char szIpAddr[PLATFORM_MAX_PATH] = { EOS, ... };

    if (nUser > 0 && nUser <= MaxClients)
    {
        if (GetClientIP(nUser, szIpAddr, sizeof (szIpAddr), true))
        {
            if (StrContains(szIpAddr, ".", false) != -1)
            {
                GeoRT_Add(szIpAddr);
            }
        }
    }

    return true;
}

public void OnMapEnd()
{
    static const char pszcDb[][] =
    {
        "GeoLite2-City.mmdb",   "GeoIP2-City.mmdb",
        "GeoLiteCity.dat",      "GeoIPCity.dat",
        "GeoLiteISP.dat",       "GeoIPISP.dat",
        "GeoLite2-ASN.mmdb",    "GeoIP2-ISP.mmdb",
    };

    static char szPath[PLATFORM_MAX_PATH] = { EOS, ... }, szFile[PLATFORM_MAX_PATH] = { EOS, ... };
    static DirectoryListing xDir = null;
    static FileType xType = FileType_Unknown;
    static int nIter = 0;

    GeoRT_Free();
    {
        BuildPath(Path_SM, szPath, sizeof (szPath), "data/GeoResolver/Update");
        {
            if (DirExists(szPath))
            {
                xDir = OpenDirectory(szPath);
                {
                    if (xDir != null)
                    {
                        while (ReadDirEntry(xDir, szFile, sizeof (szFile), xType))
                        {
                            if (xType == FileType_File)
                            {
                                for (nIter = 0; nIter < sizeof (pszcDb); nIter++)
                                {
                                    if (0 == strcmp(szFile, pszcDb[nIter], true))
                                    {
                                        CloseHandle(xDir);
                                        {
                                            GeoR_Reload();
                                            {
                                                xDir = null;
                                                {
                                                    return;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        CloseHandle(xDir);
                        {
                            xDir = null;
                        }
                    }
                }
            }
        }
    }
}

public Action OnUserJoin_Pre(Handle xEv, const char[] szEvName, bool bEvNoBC)
{
    static char szIpAddr[PLATFORM_MAX_PATH] = { EOS, ... }, szRandom[PLATFORM_MAX_PATH] = { EOS, ... };

    if (EOS == szRandom[0])
    {
        FormatEx(szRandom, sizeof (szRandom), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'), GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'),
            GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'), GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'),
            GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'), GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'),
            GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'), GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'),
            GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'), GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'));
    }

    if (xEv != INVALID_HANDLE)
    {
        GetEventString(xEv, "address", szIpAddr, sizeof (szIpAddr), szRandom);
        {
            if (strcmp(szRandom, szIpAddr, false))
            {
                if (StrContains(szIpAddr, ".", false) != -1)
                {
                    GeoRT_Add(szIpAddr);
                }
            }
        }

        GetEventString(xEv, "ip", szIpAddr, sizeof (szIpAddr), szRandom);
        {
            if (strcmp(szRandom, szIpAddr, false))
            {
                if (StrContains(szIpAddr, ".", false) != -1)
                {
                    GeoRT_Add(szIpAddr);
                }
            }
        }
    }

    return Plugin_Continue;
}
