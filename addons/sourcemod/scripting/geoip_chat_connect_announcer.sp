
#include    <   sourcemod       >
#include    <   GeoResolver     >

public  Plugin  myinfo  =
{
    name            =   "GeoResolver: Join Announcer"                           ,
    author          =   "Hattrick HKS (claudiuhks)"                             ,
    description     =   "Prints Users' Geographical Information While Joining"  ,
    version         =   __DATE__                                                ,
    url             =   "https://forums.alliedmods.net/showthread.php?t=267805" ,
};

/**
 * PRINTS       " \x01Player\x04 HATTRİCK CARAMEL® HACK CS.MONEY\x01 Joined"
 *
 * IF   THE     GEOGRAPHICAL    INFORMATION HAS NOT BEEN DETECTED
 *
 * UNCOMMENT    - '//'      IF  YES
 * COMMENT      + '//'      IF  NO
 */

#define     SHOW_EVEN_IF_NOT_DETECTED       /// Show?

#define     SHOW_PLAYER_DISCONNECT_CHAT     /// Show?
#define     SHOW_PLAYER_TEAM_CHAT           /// Show?

/**
 * PRINTS   ONLY A MESSAGE EACH # SECONDS
 */

#define     CHAT_SPAM_DELAY     4.000000

static float g_fStamp =         0.000000;

static EngineVersion g_xEngVs = Engine_Unknown;

public void OnPluginStart()
{
    g_xEngVs =  GetEngineVersion();

    HookEventEx ("player_disconnect",           OnUserLeave_Pre,    EventHookMode_Pre);
    HookEventEx ("player_disconnect_client",    OnUserLeave_Pre,    EventHookMode_Pre);
    HookEventEx ("player_client_disconnect",    OnUserLeave_Pre,    EventHookMode_Pre);

    HookEventEx ("player_connect",              OnUserJoin_Pre,     EventHookMode_Pre);
    HookEventEx ("player_connect_client",       OnUserJoin_Pre,     EventHookMode_Pre);
    HookEventEx ("player_client_connect",       OnUserJoin_Pre,     EventHookMode_Pre);

    HookEventEx ("player_team",                 OnUserTeam_Pre,     EventHookMode_Pre);

    g_fStamp =  0.000000;
}

public void OnMapStart()
{
    g_fStamp =  0.000000;
}

public void OnMapEnd()
{
    g_fStamp =  0.000000;

    GeoRT_Free  ();
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

public void OnClientPutInServer(int nUser)
{
    if (nUser > 0 && nUser <= MaxClients)
    {
        if (IsClientConnected(nUser))
        {
            CreateTimer(GetRandomFloat(2.000000, 4.000000), Timer_Join, GetClientUserId(nUser), TIMER_FLAG_NO_MAPCHANGE);
        }
    }
}

public Action Timer_Join(Handle xTimer, any nUserId)
{
    static int nUser = 0;
    static float fEngTime = 0.000000;
    static bool bIsp = false, bCountry = false, bCity = false;
    static char szCountry[PLATFORM_MAX_PATH] = { EOS, ... }, szCity[PLATFORM_MAX_PATH] = { EOS, ... },
        szIsp[PLATFORM_MAX_PATH] = { EOS, ... }, szIpAddr[PLATFORM_MAX_PATH] = { EOS, ... };

    if (nUserId > -1 && (fEngTime = GetEngineTime()) > g_fStamp && (nUser = GetClientOfUserId(nUserId)) > 0 && nUser <= MaxClients && IsClientConnected(nUser) &&
        IsClientInGame(nUser) && !IsFakeClient(nUser) && !IsClientSourceTV(nUser) && !IsClientReplay(nUser) && !IsClientInKickQueue(nUser) && !IsClientTimingOut(nUser) &&
        GetClientIP(nUser, szIpAddr, sizeof (szIpAddr), true) && -1 != StrContains(szIpAddr, ".", false))
    {
        GeoRT_Country       (szIpAddr,          szCountry,  sizeof (szCountry));
        GeoRT_City          (szIpAddr,          szCity,     sizeof (szCity));
        GeoRT_ISP           (szIpAddr,          szIsp,      sizeof (szIsp));

        bIsp        =       (strcmp(szIsp,      "N/ A",     false) == 0) ? false : true;
        bCountry    =       (strcmp(szCountry,  "N/ A",     false) == 0) ? false : true;
        bCity       =       (strcmp(szCity,     "N/ A",     false) == 0) ? false : true;

        if (!bCountry)
        {
            GeoRT_Continent (szIpAddr,          szCountry,  sizeof (szCountry));
            {
                bCountry =  (strcmp(szCountry,  "N/ A",     false) == 0) ? false : true;
            }
        }

        if (!bCity)
        {
            GeoRT_Region    (szIpAddr,          szCity,     sizeof (szCity));
            {
                bCity =     (strcmp(szCity,     "N/ A",     false) == 0) ? false : true;
            }
        }

        switch (g_xEngVs)
        {
            case Engine_CSGO:
            {
                if (bCountry        &&      bCity       &&      bIsp)
                {
                    PrintToChatAll(" \x01Player\x04 %N\x01 Joined From\x05 %s\x01,\x05 %s",         nUser, szCity,      szCountry);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bCountry   &&      bCity)
                {
                    PrintToChatAll(" \x01Player\x04 %N\x01 Joined From\x05 %s\x01,\x05 %s",         nUser, szCity,      szCountry);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bCountry   &&      bIsp)
                {
                    PrintToChatAll(" \x01Player\x04 %N\x01 Joined From\x05 %s\x01 [\x05 %s\x01 ]",  nUser, szCountry,   szIsp);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bCity      &&      bIsp)
                {
                    PrintToChatAll(" \x01Player\x04 %N\x01 Joined From\x05 %s\x01 [\x05 %s\x01 ]",  nUser, szCity,      szIsp);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bCountry)
                {
                    PrintToChatAll(" \x01Player\x04 %N\x01 Joined From\x05 %s",                     nUser, szCountry);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bCity)
                {
                    PrintToChatAll(" \x01Player\x04 %N\x01 Joined From\x05 %s",                     nUser, szCity);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bIsp)
                {
                    PrintToChatAll(" \x01Player\x04 %N\x01 Joined [\x05 %s\x01 ]",                  nUser, szIsp);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

#if     defined     SHOW_EVEN_IF_NOT_DETECTED

                else
                {
                    PrintToChatAll(" \x01Player\x04 %N\x01 Joined",                                 nUser);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

#endif

            }

            default:
            {
                if (bCountry        &&      bCity       &&      bIsp)
                {
                    PrintToChatAll("Player %N Joined From %s, %s",      nUser, szCity,      szCountry);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bCountry   &&      bCity)
                {
                    PrintToChatAll("Player %N Joined From %s, %s",      nUser, szCity,      szCountry);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bCountry   &&      bIsp)
                {
                    PrintToChatAll("Player %N Joined From %s [ %s ]",   nUser, szCountry,   szIsp);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bCity      &&      bIsp)
                {
                    PrintToChatAll("Player %N Joined From %s [ %s ]",   nUser, szCity,      szIsp);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bCountry)
                {
                    PrintToChatAll("Player %N Joined From %s",          nUser, szCountry);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bCity)
                {
                    PrintToChatAll("Player %N Joined From %s",          nUser, szCity);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

                else if (bIsp)
                {
                    PrintToChatAll("Player %N Joined [ %s ]",           nUser, szIsp);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

#if     defined     SHOW_EVEN_IF_NOT_DETECTED

                else
                {
                    PrintToChatAll("Player %N Joined",                  nUser);

                    g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                }

#endif

            }
        }
    }

    return Plugin_Continue;
}

public Action OnUserJoin_Pre(Handle xEv, const char[] szEvName, bool bEvNoBC)
{
    if (xEv != INVALID_HANDLE)
    {
        if (bEvNoBC == false)
        {
            SetEventBroadcast(xEv, true);
        }
    }

    return Plugin_Continue;
}

public Action OnUserLeave_Pre(Handle xEv, const char[] szEvName, bool bEvNoBC)
{

#if defined SHOW_PLAYER_DISCONNECT_CHAT

    static Handle xPack = INVALID_HANDLE;
    static char szName[PLATFORM_MAX_PATH] = { EOS, ... }, szReason[PLATFORM_MAX_PATH] = { EOS, ... }, szRandom[PLATFORM_MAX_PATH] = { EOS, ... };

    if (EOS == szRandom[0])
    {
        FormatEx(szRandom, sizeof (szRandom), "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
            GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'), GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'),
            GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'), GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'),
            GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'), GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'),
            GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'), GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'),
            GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'), GetRandomInt('A', 'Z'), GetRandomInt('0', '9'), GetRandomInt('a', 'z'));
    }

#endif

    if (xEv != INVALID_HANDLE)
    {
        if (bEvNoBC == false)
        {
            SetEventBroadcast(xEv, true);
        }

#if defined SHOW_PLAYER_DISCONNECT_CHAT

        GetEventString(xEv, "name", szName, sizeof (szName), szRandom);
        {
            if (strcmp(szName, szRandom, false))
            {
                GetEventString(xEv, "reason", szReason, sizeof (szReason), szRandom);
                {
                    if (strcmp(szReason, szRandom, false))
                    {
                        xPack = CreateDataPack();
                        {
                            if (xPack != INVALID_HANDLE)
                            {
                                WritePackString(xPack, szName);
                                WritePackString(xPack, szReason);

                                CreateTimer(GetRandomFloat(0.100000, 1.500000), Timer_Leave, xPack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
                            }
                        }
                    }
                }
            }
        }

#endif

    }

    return Plugin_Continue;
}

public Action OnUserTeam_Pre(Handle xEv, const char[] szEvName, bool bEvNoBC)
{

#if defined SHOW_PLAYER_TEAM_CHAT

    static Handle xPack = INVALID_HANDLE;
    static int nUserId = 0, nTeam = 0;

#endif

    if (xEv != INVALID_HANDLE)
    {
        if (bEvNoBC == false)
        {
            SetEventBroadcast(xEv, true);
        }

#if defined SHOW_PLAYER_TEAM_CHAT

        nUserId = GetEventInt(xEv, "userid", -16384);
        {
            if (nUserId > -1)
            {
                nTeam = GetEventInt(xEv, "team", -16384);
                {
                    if (nTeam > 0)
                    {
                        xPack = CreateDataPack();
                        {
                            if (xPack != INVALID_HANDLE)
                            {
                                WritePackCell(xPack, nUserId);
                                WritePackCell(xPack, nTeam);

                                CreateTimer(GetRandomFloat(0.100000, 1.500000), Timer_Team, xPack, TIMER_FLAG_NO_MAPCHANGE | TIMER_DATA_HNDL_CLOSE);
                            }
                        }
                    }
                }
            }
        }

#endif

    }

    return Plugin_Continue;
}

#if defined SHOW_PLAYER_TEAM_CHAT

public Action Timer_Team(Handle xTimer, any xPack)
{
    static int nUserId = 0, nUser = 0, nTeam = 0;
    static char szIpAddr[PLATFORM_MAX_PATH] = { EOS, ... };
    static float fEngTime = 0.0;

    if (xPack != INVALID_HANDLE)
    {
        if ((fEngTime = GetEngineTime()) > g_fStamp)
        {
            ResetPack(xPack);
            {
                if ((nUserId = ReadPackCell(xPack)) > -1)
                {
                    if ((nUser = GetClientOfUserId(nUserId)) > 0 && nUser <= MaxClients)
                    {
                        if (IsClientConnected(nUser) && IsClientInGame(nUser) && !IsFakeClient(nUser) && !IsClientSourceTV(nUser) &&
                            !IsClientReplay(nUser) && !IsClientInKickQueue(nUser) && !IsClientTimingOut(nUser) &&
                            GetClientIP(nUser, szIpAddr, sizeof (szIpAddr), true) && -1 != StrContains(szIpAddr, ".", false))
                        {
                            if ((nTeam = ReadPackCell(xPack)) > 0)
                            {
                                if (g_xEngVs == Engine_CSGO)
                                {
                                    switch (nTeam)
                                    {
                                        case 1:
                                        {
                                            PrintToChatAll(" \x01Player\x05 %N\x01 Became A\x08 SPECTATOR",         nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }

                                        case 2:
                                        {
                                            PrintToChatAll(" \x01Player\x05 %N\x01 Became A\x07 TERRORIST",         nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }

                                        case 3:
                                        {
                                            PrintToChatAll(" \x01Player\x05 %N\x01 Became A\x0B COUNTER TERRORIST", nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }
                                    }
                                }

                                else if (g_xEngVs == Engine_CSS)
                                {
                                    switch (nTeam)
                                    {
                                        case 1:
                                        {
                                            PrintToChatAll("Player %N Became A SPECTATOR",          nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }

                                        case 2:
                                        {
                                            PrintToChatAll("Player %N Became A TERRORIST",          nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }

                                        case 3:
                                        {
                                            PrintToChatAll("Player %N Became A COUNTER TERRORIST",  nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }
                                    }
                                }

                                else if (g_xEngVs == Engine_DODS)
                                {
                                    switch (nTeam)
                                    {
                                        case 1:
                                        {
                                            PrintToChatAll("Player %N Became A SPECTATOR",      nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }

                                        case 2:
                                        {
                                            PrintToChatAll("Player %N Joined The US ARMY",      nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }

                                        case 3:
                                        {
                                            PrintToChatAll("Player %N Joined The WEHRMACHT",    nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }
                                    }
                                }

                                else
                                {
                                    switch (nTeam)
                                    {
                                        case 1:
                                        {
                                            PrintToChatAll("Player %N Became A SPECTATOR",  nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }

                                        case 2:
                                        {
                                            PrintToChatAll("Player %N Joined The TEAM #1",  nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }

                                        case 3:
                                        {
                                            PrintToChatAll("Player %N Joined The TEAM #2",  nUser);

                                            g_fStamp = fEngTime + CHAT_SPAM_DELAY;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return Plugin_Continue;
}

#endif

#if defined SHOW_PLAYER_DISCONNECT_CHAT

static int FixLine(char[] szItm, int nMax)
{
    static int nItr = 0, nLen = 0, nChr = 0, nSze = 0;
    static char szTmp[PLATFORM_MAX_PATH] = { EOS, ... }, cTmp = EOS;

    if (nMax < 1)
    {
        szItm[0] = EOS;
        return 0;
    }

    nLen = strlen(szItm);

    if (nLen < 1)
    {
        szItm[0] = EOS;
        return 0;
    }

    for (nItr = 0, nChr = 0, cTmp = ' ', nSze = sizeof (szTmp); ((nItr < nLen) && (nChr < nSze) && (nChr < nMax)); nItr++)
    {
        if (szItm[nItr] == '\n' || szItm[nItr] == '\r' || szItm[nItr] == '\v' || szItm[nItr] == '\f' || szItm[nItr] == '\a' || szItm[nItr] == '\b')
        {
            continue;
        }

        if (szItm[nItr] == '\t')
        {
            szItm[nItr] = ' ';
        }

        if (szItm[nItr] != ' ' || cTmp != ' ')
        {
            szTmp[nChr++] = szItm[nItr];

            cTmp = szItm[nItr];
        }
    }

    if (nChr < 1)
    {
        szItm[0] = EOS;
        return 0;
    }

    szTmp[nChr] = EOS;

    if (szTmp[nChr - 1] == ' ')
    {
        szTmp[--nChr] = EOS;
    }

    if (nChr < 1)
    {
        szItm[0] = EOS;
        return 0;
    }

    if (nMax <= nChr)
    {
        nChr = nMax;

        if (szTmp[nChr - 1] == ' ')
        {
            szTmp[--nChr] = EOS;
        }

        if (nChr < 1)
        {
            szItm[0] = EOS;
            return 0;
        }
    }

    return strcopy(szItm, nMax, szTmp);
}

public Action Timer_Leave(Handle xTimer, any xPack)
{
    static char szName[PLATFORM_MAX_PATH] = { EOS, ... }, szReason[PLATFORM_MAX_PATH] = { EOS, ... }, szNewReason[PLATFORM_MAX_PATH] = { EOS, ... };
    static float fEngTime = 0.0;
    static int nIter = 0, nReasonLen = 0, nNewReasonLen = 0;

    if (xPack != INVALID_HANDLE)
    {
        if ((fEngTime = GetEngineTime()) > g_fStamp)
        {
            ResetPack(xPack);
            {
                ReadPackString(xPack, szName,   sizeof (szName));
                ReadPackString(xPack, szReason, sizeof (szReason));

                if (Engine_CSGO == g_xEngVs)
                {
                    switch ((nReasonLen = strlen(szReason)))
                    {
                        case 0:
                        {
                            PrintToChatAll(" \x01Player\x05 %s\x01 Flew Away", szName);
                        }

                        default:
                        {
                            for (nIter = 0, nNewReasonLen = 0; nIter < nReasonLen; nIter++)
                            {
                                if (szReason[nIter] == ' ' || IsCharAlpha(szReason[nIter]) || IsCharNumeric(szReason[nIter]) || IsCharMB(szReason[nIter]) > 0)
                                {
                                    szNewReason[nNewReasonLen++] = CharToUpper(szReason[nIter]);
                                }
                            }

                            if (nNewReasonLen > 0)
                            {
                                szNewReason[nNewReasonLen] = EOS;
                                {
                                    if (FixLine(szNewReason, sizeof (szNewReason)) > 0)
                                    {
                                        PrintToChatAll(" \x01Player\x05 %s\x01 Flew Away,\x09 %s", szName, szNewReason);
                                    }

                                    else
                                    {
                                        PrintToChatAll(" \x01Player\x05 %s\x01 Flew Away", szName);
                                    }
                                }
                            }

                            else
                            {
                                PrintToChatAll(" \x01Player\x05 %s\x01 Flew Away", szName);
                            }
                        }
                    }
                }

                else
                {
                    switch ((nReasonLen = strlen(szReason)))
                    {
                        case 0:
                        {
                            PrintToChatAll("Player %s Flew Away", szName);
                        }

                        default:
                        {
                            for (nIter = 0, nNewReasonLen = 0; nIter < nReasonLen; nIter++)
                            {
                                if (szReason[nIter] == ' ' || IsCharAlpha(szReason[nIter]) || IsCharNumeric(szReason[nIter]) || IsCharMB(szReason[nIter]) > 0)
                                {
                                    szNewReason[nNewReasonLen++] = CharToUpper(szReason[nIter]);
                                }
                            }

                            if (nNewReasonLen > 0)
                            {
                                szNewReason[nNewReasonLen] = EOS;
                                {
                                    if (FixLine(szNewReason, sizeof (szNewReason)) > 0)
                                    {
                                        PrintToChatAll("Player %s Flew Away, %s", szName, szNewReason);
                                    }

                                    else
                                    {
                                        PrintToChatAll("Player %s Flew Away", szName);
                                    }
                                }
                            }

                            else
                            {
                                PrintToChatAll("Player %s Flew Away", szName);
                            }
                        }
                    }
                }

                g_fStamp = fEngTime + CHAT_SPAM_DELAY;
            }
        }
    }

    return Plugin_Continue;
}

#endif
