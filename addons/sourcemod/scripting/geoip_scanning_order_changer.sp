
#include    <   sourcemod       >
#include    <   GeoResolver     >

public  Plugin  myinfo  =
{
    name            =   "GeoResolver: Scanning Order Changer"                   ,
    author          =   "Hattrick HKS (claudiuhks)"                             ,
    description     =   "Changes The Order Of Scanning"                         ,
    version         =   __DATE__                                                ,
    url             =   "https://forums.alliedmods.net/showthread.php?t=267805" ,
};

public void OnPluginStart()
{
    GeoR_Order  (GEOIP_PAID_FIRST);

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
    GeoRT_Free();
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
