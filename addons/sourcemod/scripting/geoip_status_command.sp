
#include        <   sourcemod       >
#include        <   GeoResolver     >

#define         SM_STATUS_IS_FOR_EVERYONE                           // COMMENT THIS LINE OUT, IF YOU WANT (WITH A '//' AT THE BEGINNING)

#if !defined    SM_STATUS_IS_FOR_EVERYONE

    #define     SM_STATUS_COMMAND_ACCESS        ADMFLAG_GENERIC     // REQUIRED ADMIN ACCESS FLAG, FOR THE COMMAND

#endif

#if !defined    HUD_PRINTCONSOLE

    #define     HUD_PRINTCONSOLE                (2)

#endif

public  Plugin  myinfo  =
{
    name            =   "GeoResolver: Status Command"                           ,
    author          =   "Hattrick HKS (claudiuhks)"                             ,
    description     =   "Prints Users' Geographical Information"                ,
    version         =   __DATE__                                                ,
    url             =   "https://forums.alliedmods.net/showthread.php?t=267805" ,
};

static EngineVersion g_xEngVs = Engine_Unknown;

public void OnPluginStart()
{
    g_xEngVs =      GetEngineVersion();

#if !defined SM_STATUS_IS_FOR_EVERYONE

    RegAdminCmd     ("sm_status",               CmdStatus,      SM_STATUS_COMMAND_ACCESS,   "sm_status - Prints Players' Geographical Information",     "status",   FCVAR_NONE  );
    RegAdminCmd     ("sm_countries",            CmdStatus,      SM_STATUS_COMMAND_ACCESS,   "sm_countries - Prints Players' Geographical Information",  "status",   FCVAR_NONE  );
    RegAdminCmd     ("sm_cities",               CmdStatus,      SM_STATUS_COMMAND_ACCESS,   "sm_cities - Prints Players' Geographical Information",     "status",   FCVAR_NONE  );
    RegAdminCmd     ("sm_locations",            CmdStatus,      SM_STATUS_COMMAND_ACCESS,   "sm_locations - Prints Players' Geographical Information",  "status",   FCVAR_NONE  );

#else

    RegConsoleCmd   ("sm_status",               CmdStatus,                                  "sm_status - Prints Players' Geographical Information",                 FCVAR_NONE  );
    RegConsoleCmd   ("sm_countries",            CmdStatus,                                  "sm_countries - Prints Players' Geographical Information",              FCVAR_NONE  );
    RegConsoleCmd   ("sm_cities",               CmdStatus,                                  "sm_cities - Prints Players' Geographical Information",                 FCVAR_NONE  );
    RegConsoleCmd   ("sm_locations",            CmdStatus,                                  "sm_locations - Prints Players' Geographical Information",              FCVAR_NONE  );

#endif

    HookEventEx     ("player_connect",          OnUserJoin_Pre, EventHookMode_Pre);
    HookEventEx     ("player_connect_client",   OnUserJoin_Pre, EventHookMode_Pre);
    HookEventEx     ("player_client_connect",   OnUserJoin_Pre, EventHookMode_Pre);
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

static bool sendConLine(int nUser, const char[] szLine, any ...)
{
    static UserMsg xMsgId = INVALID_MESSAGE_ID;
    static bool bMsgIdRetrieved = false;
    static int nUsers[1] = { 0, };
    static Handle xMsg = INVALID_HANDLE;
    static char szBuffer[PLATFORM_MAX_PATH] = { EOS, ... };

    if (szLine[0] != EOS)
    {
        if (g_xEngVs != Engine_CSGO)
        {
            if (!bMsgIdRetrieved)
            {
                bMsgIdRetrieved = true;
                {
                    xMsgId = GetUserMessageId("TextMsg");
                }
            }

            if (xMsgId != INVALID_MESSAGE_ID)
            {
                if (VFormat(szBuffer, sizeof (szBuffer), szLine, 3) > 0)
                {
                    nUsers[0] = nUser;
                    {
                        xMsg = StartMessageEx(xMsgId, nUsers, sizeof (nUsers), 0);
                        {
                            if (INVALID_HANDLE != xMsg)
                            {
                                BfWriteByte(xMsg, HUD_PRINTCONSOLE);
                                {
                                    BfWriteString(xMsg, szBuffer);
                                    {
                                        BfWriteString(xMsg, "");
                                        BfWriteString(xMsg, "");
                                        BfWriteString(xMsg, "");
                                        BfWriteString(xMsg, "");
                                        {
                                            EndMessage();
                                            {
                                                return true;
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
    }

    return false;
}

public Action CmdStatus(int nUser, int nArgs)
{
    static int nPlayer = 0, nRow = 0;
    static GR_Db xDb = GEOIP_NONE;
    static ReplySource xSrc = SM_REPLY_TO_CONSOLE;
    static char szIpAddr[PLATFORM_MAX_PATH] = { EOS, ... }, szName[PLATFORM_MAX_PATH] = { EOS, ... }, szCountry[PLATFORM_MAX_PATH] = { EOS, ... },
        szCity[PLATFORM_MAX_PATH] = { EOS, ... }, szIsp[PLATFORM_MAX_PATH] = { EOS, ... };

    if ((nUser < 0) || (nUser > MaxClients) || (nUser && (!IsClientConnected(nUser) || !IsClientInGame(nUser) || IsFakeClient(nUser) ||
        IsClientTimingOut(nUser) || IsClientInKickQueue(nUser) || IsClientReplay(nUser) || IsClientInKickQueue(nUser) ||
        !GetClientIP(nUser, szIpAddr, sizeof (szIpAddr), true) || -1 == StrContains(szIpAddr, ".", false))))
    {
        return Plugin_Handled;
    }

    xSrc = GetCmdReplySource();

    if ((xDb = GeoR_Databases()) == GEOIP_NONE)
    {
        switch (nUser)
        {
            case 0:
            {
                PrintToServer("\nNo MaxMind® Databases Are Actually In Use\n");
            }

            default:
            {
                if (xSrc == SM_REPLY_TO_CHAT)
                {
                    if (Engine_CSGO == g_xEngVs)
                    {
                        PrintToChat(nUser, " \x01No\x04 MaxMind® Databases\x01 Are Actually In Use");
                    }

                    else
                    {
                        PrintToChat(nUser, "No MaxMind® Databases Are Actually In Use");
                    }
                }

                else
                {
                    if (Engine_CSGO == g_xEngVs)
                    {
                        PrintToConsole(nUser, "\nNo MaxMind® Databases Are Actually In Use\n");
                    }

                    else
                    {
                        sendConLine(nUser, "\n\nNo MaxMind® Databases Are Actually In Use\n\n");
                    }
                }
            }
        }

        return Plugin_Handled;
    }

    switch (g_xEngVs)
    {
        case Engine_CSGO:
        {
            if (xDb & GEOIP_ISP_PAID || xDb & GEOIP_ISP_LITE || xDb & GEOIP_ISP2_PAID || xDb & GEOIP_ISP2_LITE)
            {
                if (xSrc == SM_REPLY_TO_CONSOLE)
                {
                    if (nUser == 0)
                    {
                        PrintToServer("\n%-2s %-48s %-48s %-48s %s\n", "#", "Name", "Country", "City", "ISP");
                    }

                    else
                    {
                        PrintToConsole(nUser, "\n%-2s %-48s %-48s %-48s %s\n", "#", "Name", "Country", "City", "ISP");
                    }
                }

                else
                {
                    PrintToConsole(nUser, "\n%-2s %-48s %-48s %-48s %s\n", "#", "Name", "Country", "City", "ISP");
                }
            }

            else
            {
                if (xSrc == SM_REPLY_TO_CONSOLE)
                {
                    if (nUser == 0)
                    {
                        PrintToServer("\n%-2s %-48s %-48s %s\n", "#", "Name", "Country", "City");
                    }

                    else
                    {
                        PrintToConsole(nUser, "\n%-2s %-48s %-48s %s\n", "#", "Name", "Country", "City");
                    }
                }

                else
                {
                    PrintToConsole(nUser, "\n%-2s %-48s %-48s %s\n", "#", "Name", "Country", "City");
                }
            }
        }

        default:
        {
            if (xDb & GEOIP_ISP_PAID || xDb & GEOIP_ISP_LITE || xDb & GEOIP_ISP2_PAID || xDb & GEOIP_ISP2_LITE)
            {
                if (xSrc == SM_REPLY_TO_CONSOLE)
                {
                    if (nUser == 0)
                    {
                        PrintToServer("\n%-2s %-48s %-48s %-48s %s\n", "#", "Name", "Country", "City", "ISP");
                    }

                    else
                    {
                        sendConLine(nUser, "\n\n%-2s %-48s %-48s %-48s %s\n\n", "#", "Name", "Country", "City", "ISP");
                    }
                }

                else
                {
                    sendConLine(nUser, "\n%-2s %-48s %-48s %-48s %s\n\n", "#", "Name", "Country", "City", "ISP");
                }
            }

            else
            {
                if (xSrc == SM_REPLY_TO_CONSOLE)
                {
                    if (nUser == 0)
                    {
                        PrintToServer("\n%-2s %-48s %-48s %s\n", "#", "Name", "Country", "City");
                    }

                    else
                    {
                        sendConLine(nUser, "\n\n%-2s %-48s %-48s %s\n\n", "#", "Name", "Country", "City");
                    }
                }

                else
                {
                    sendConLine(nUser, "\n%-2s %-48s %-48s %s\n\n", "#", "Name", "Country", "City");
                }
            }
        }
    }

    for (nRow = 0, nPlayer = 1; nPlayer <= MaxClients; nPlayer++)
    {
        if (IsClientConnected(nPlayer) && IsClientInGame(nPlayer) && !IsFakeClient(nPlayer) && !IsClientSourceTV(nPlayer) &&
            !IsClientReplay(nPlayer) && !IsClientTimingOut(nPlayer) && !IsClientInKickQueue(nPlayer) &&
            GetClientIP(nPlayer, szIpAddr, sizeof (szIpAddr), true) && -1 != StrContains(szIpAddr, ".", false))
        {
            GetClientName       (nPlayer,   szName,     sizeof (szName));

            GeoRT_Country       (szIpAddr,  szCountry,  sizeof (szCountry));
            GeoRT_City          (szIpAddr,  szCity,     sizeof (szCity));

            if (0 == strcmp     (szCountry, "N/ A",     false))
            {
                GeoRT_Continent (szIpAddr,  szCountry,  sizeof (szCountry));
            }

            if (0 == strcmp     (szCity,    "N/ A",     false))
            {
                GeoRT_Region    (szIpAddr,  szCity,     sizeof (szCity));
            }

            if (xDb & GEOIP_ISP_PAID || xDb & GEOIP_ISP_LITE || xDb & GEOIP_ISP2_PAID || xDb & GEOIP_ISP2_LITE)
            {
                GeoRT_ISP   (szIpAddr,  szIsp,      sizeof (szIsp));
                {
                    if (nUser == 0)
                    {
                        PrintToServer("%-2d %-48s %-48s %-48s %s", ++nRow, szName, szCountry, szCity, szIsp);
                    }

                    else
                    {
                        if (Engine_CSGO == g_xEngVs)
                        {
                            PrintToConsole(nUser, "%-2d %-48s %-48s %-48s %s", ++nRow, szName, szCountry, szCity, szIsp);
                        }

                        else
                        {
                            sendConLine(nUser, "%-2d %-48s %-48s %-48s %s", ++nRow, szName, szCountry, szCity, szIsp);
                        }
                    }
                }
            }

            else
            {
                if (nUser == 0)
                {
                    PrintToServer("%-2d %-48s %-48s %s", ++nRow, szName, szCountry, szCity);
                }

                else
                {
                    if (Engine_CSGO == g_xEngVs)
                    {
                        PrintToConsole(nUser, "%-2d %-48s %-48s %s", ++nRow, szName, szCountry, szCity);
                    }

                    else
                    {
                        sendConLine(nUser, "%-2d %-48s %-48s %s", ++nRow, szName, szCountry, szCity);
                    }
                }
            }
        }
    }

    if (nRow > 0)
    {
        if (nUser == 0)
        {
            PrintToServer("\nListed %d %s\n", nRow, nRow == 1 ? "Row" : "Rows");
        }

        else
        {
            if (Engine_CSGO == g_xEngVs)
            {
                PrintToConsole(nUser, "\nListed %d %s\n", nRow, nRow == 1 ? "Row" : "Rows");
            }

            else
            {
                sendConLine(nUser, "\nListed %d %s\n\n", nRow, nRow == 1 ? "Row" : "Rows");
            }

            if (xSrc == SM_REPLY_TO_CHAT)
            {
                if (Engine_CSGO == g_xEngVs)
                {
                    PrintToChat(nUser, " \x01Listed\x04 %d\x01 %s In Your\x05 CONSOLE", nRow, nRow == 1 ? "Row" : "Rows");
                }

                else
                {
                    PrintToChat(nUser, "Listed %d %s In Your CONSOLE", nRow, nRow == 1 ? "Row" : "Rows");
                }
            }
        }
    }

    else
    {
        if (nUser == 0)
        {
            PrintToServer("Listed %d %s\n", nRow, nRow == 1 ? "Row" : "Rows");
        }

        else
        {
            if (Engine_CSGO == g_xEngVs)
            {
                PrintToConsole(nUser, "Listed %d %s\n", nRow, nRow == 1 ? "Row" : "Rows");
            }

            else
            {
                sendConLine(nUser, "Listed %d %s\n\n", nRow, nRow == 1 ? "Row" : "Rows");
            }

            if (xSrc == SM_REPLY_TO_CHAT)
            {
                if (Engine_CSGO == g_xEngVs)
                {
                    PrintToChat(nUser, " \x01Listed\x04 %d\x01 %s In Your\x05 CONSOLE", nRow, nRow == 1 ? "Row" : "Rows");
                }

                else
                {
                    PrintToChat(nUser, "Listed %d %s In Your CONSOLE", nRow, nRow == 1 ? "Row" : "Rows");
                }
            }
        }
    }

    return Plugin_Handled;
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
