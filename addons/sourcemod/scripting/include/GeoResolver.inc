
#if defined     _GeoResolver_included

    #endinput

#endif

#define         _GeoResolver_included

#include        < sourcemod >

//
// Yes, you can edit these two,
//  if you know what you're doing.
//

#define         GR_VarType          2       /** 1 = public, 2 = static, & 3 = stock */
#define         GR_FunType          3       /** 1 = public, 2 = static, & 3 = stock */

#if !defined    GR_VarType

    #error      GR_VarType must be 1, 2, or 3.

#endif

#if     GR_VarType == 1

    #define     GR_Var              public

#elseif GR_VarType == 2

    #define     GR_Var              static

#elseif GR_VarType == 3

    #define     GR_Var              stock

#endif

#if !defined    GR_Var

    #error      GR_VarType must be 1, 2, or 3.

#endif

#if !defined    GR_FunType

    #error      GR_FunType must be 1, 2, or 3.

#endif

#if     GR_FunType == 1

    #define     GR_Fun              public

    #define     GR_NoDefFuncParams

#elseif GR_FunType == 2

    #define     GR_Fun              static

#elseif GR_FunType == 3

    #define     GR_Fun              stock

#endif

#if !defined    GR_Fun

    #error      GR_FunType must be 1, 2, or 3.

#endif

enum GR_Db                              /** MAXMIND®            LOADED  DATABASES   */
{
    GEOIP_NONE      =   0       ,       /** NO DATABASES        ARE     IN USE      */

    GEOIP2_LITE     =   1 << 0  ,       /** GeoLite2-City.mmdb  IS      IN USE      */
    GEOIP2_PAID     =   1 << 1  ,       /** GeoIP2-City.mmdb    IS      IN USE      */

    GEOIP_LITE      =   1 << 2  ,       /** GeoLiteCity.dat     IS      IN USE      */
    GEOIP_PAID      =   1 << 3  ,       /** GeoIPCity.dat       IS      IN USE      */

    GEOIP_ISP_LITE  =   1 << 4  ,       /** GeoLiteISP.dat      IS      IN USE      */
    GEOIP_ISP_PAID  =   1 << 5  ,       /** GeoIPISP.dat        IS      IN USE      */

    GEOIP_ISP2_LITE =   1 << 6  ,       /** GeoLite2-ASN.mmdb   IS      IN USE      */
    GEOIP_ISP2_PAID =   1 << 7  ,       /** GeoIP2-ISP.mmdb     IS      IN USE      */
};

//
// For example,
//
//  GeoR_SetOrder(GEOIP_PAID_FIRST);
//

//
// Geo Resolver's default is GEOIP_LITE_FIRST.
//

enum GR_Order
{
    GEOIP_LITE_FIRST    =   0   ,       /** USE LITE DATABASES BEFORE PAID          */
    GEOIP_PAID_FIRST    =   1   ,       /** USE PAID DATABASES BEFORE LITE          */
};

GR_Var  char    g_szCode__              [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szCode3__             [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szCountry__           [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szCity__              [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szRegionCode__        [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szRegion__            [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szTimeZone__          [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szPostalCode__        [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szContinentCode__     [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szContinent__         [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szAutonomSysOrg__     [PLATFORM_MAX_PATH] =   "N/ A"  ;
GR_Var  char    g_szIsp__               [PLATFORM_MAX_PATH] =   "N/ A"  ;

GR_Var  float   g_fLatitude__                               =   0.0     ;
GR_Var  float   g_fLongitude__                              =   0.0     ;

//
// Yes, you don't have to strip out the port.
//

//
// T means threaded.
//
// After calling GeoRT_AddIp,
//  wait at least 40 milliseconds before calling GeoRT_ReadIp.
//
// Geo Resolver's threaded worker processes 25 addresses every second,
//  1 address every 0.04 seconds.
//

//
// If using the threaded system, call GeoRT_Free when a map starts or ends.
//

native  bool    GeoRT_Free();

//
// 0 = Failed.
//
// >0 = Entry   number into the threaded    system.
//

native  int     GeoRT_Add(const char[]      szIpAddr);

native  int     GeoRT_AddIp(const char[]    szIpAddr);

//
// 0 = Failed.
//
// >0 = Entry number into the threaded system.
//
// GeoRT_City,  GeoRT_Country, ...  will return the strcopy value.
//

native  int     GeoRT_Read(         const char[]    szIpAddr                                                ,
                                    char[]          szCode              ,   const int nCodeSize             ,
                                    char[]          szCode3             ,   const int nCode3Size            ,
                                    char[]          szCountry           ,   const int nCountrySize          ,
                                    char[]          szCity              ,   const int nCitySize             ,
                                    char[]          szRegionCode        ,   const int nRegionCodeSize       ,
                                    char[]          szRegion            ,   const int nRegionSize           ,
                                    char[]          szTimeZone          ,   const int nTimeZoneSize         ,
                                    char[]          szPostalCode        ,   const int nPostalCodeSize       ,
                                    char[]          szContinentCode     ,   const int nContinentCodeSize    ,
                                    char[]          szContinent         ,   const int nContinentSize        ,
                                    char[]          szAutonomSysOrg     ,   const int nAutonomSysOrgSize    ,
                                    char[]          szIsp               ,   const int nIspSize              ,
                                    float&          fLatitude                                               ,
                                    float&          fLongitude                                              );

native  int     GeoRT_ReadIp(       const char[]    szIpAddr                                                ,
                                    char[]          szCode              ,   const int nCodeSize             ,
                                    char[]          szCode3             ,   const int nCode3Size            ,
                                    char[]          szCountry           ,   const int nCountrySize          ,
                                    char[]          szCity              ,   const int nCitySize             ,
                                    char[]          szRegionCode        ,   const int nRegionCodeSize       ,
                                    char[]          szRegion            ,   const int nRegionSize           ,
                                    char[]          szTimeZone          ,   const int nTimeZoneSize         ,
                                    char[]          szPostalCode        ,   const int nPostalCodeSize       ,
                                    char[]          szContinentCode     ,   const int nContinentCodeSize    ,
                                    char[]          szContinent         ,   const int nContinentSize        ,
                                    char[]          szAutonomSysOrg     ,   const int nAutonomSysOrgSize    ,
                                    char[]          szIsp               ,   const int nIspSize              ,
                                    float&          fLatitude                                               ,
                                    float&          fLongitude                                              );

//
// Threaded stuff ends here.
//

//
// true on success,
//  false otherwise.
//
// GeoR_City,   GeoR_Country, ...   will return the strcopy value.
//

native  bool    GeoR_CompleteRecord(const char[]    szIpAddr                                                ,
                                    char[]          szCode              ,   const int nCodeSize             ,
                                    char[]          szCode3             ,   const int nCode3Size            ,
                                    char[]          szCountry           ,   const int nCountrySize          ,
                                    char[]          szCity              ,   const int nCitySize             ,
                                    char[]          szRegionCode        ,   const int nRegionCodeSize       ,
                                    char[]          szRegion            ,   const int nRegionSize           ,
                                    char[]          szTimeZone          ,   const int nTimeZoneSize         ,
                                    char[]          szPostalCode        ,   const int nPostalCodeSize       ,
                                    char[]          szContinentCode     ,   const int nContinentCodeSize    ,
                                    char[]          szContinent         ,   const int nContinentSize        ,
                                    char[]          szAutonomSysOrg     ,   const int nAutonomSysOrgSize    ,
                                    char[]          szIsp               ,   const int nIspSize              ,
                                    float&          fLatitude                                               ,
                                    float&          fLongitude                                              );

native  bool    GeoR_FullRecord(    const char[]    szIpAddr                                                ,
                                    char[]          szCode              ,   const int nCodeSize             ,
                                    char[]          szCode3             ,   const int nCode3Size            ,
                                    char[]          szCountry           ,   const int nCountrySize          ,
                                    char[]          szCity              ,   const int nCitySize             ,
                                    char[]          szRegionCode        ,   const int nRegionCodeSize       ,
                                    char[]          szRegion            ,   const int nRegionSize           ,
                                    char[]          szTimeZone          ,   const int nTimeZoneSize         ,
                                    char[]          szPostalCode        ,   const int nPostalCodeSize       ,
                                    char[]          szContinentCode     ,   const int nContinentCodeSize    ,
                                    char[]          szContinent         ,   const int nContinentSize        ,
                                    char[]          szAutonomSysOrg     ,   const int nAutonomSysOrgSize    ,
                                    char[]          szIsp               ,   const int nIspSize              ,
                                    float&          fLatitude                                               ,
                                    float&          fLongitude                                              );

native  bool    GeoR_GetRecord(     const char[]    szIpAddr                                                ,
                                    char[]          szCode              ,   const int nCodeSize             ,
                                    char[]          szCode3             ,   const int nCode3Size            ,
                                    char[]          szCountry           ,   const int nCountrySize          ,
                                    char[]          szCity              ,   const int nCitySize             ,
                                    char[]          szRegionCode        ,   const int nRegionCodeSize       ,
                                    char[]          szRegion            ,   const int nRegionSize           ,
                                    char[]          szTimeZone          ,   const int nTimeZoneSize         ,
                                    char[]          szPostalCode        ,   const int nPostalCodeSize       ,
                                    char[]          szContinentCode     ,   const int nContinentCodeSize    ,
                                    char[]          szContinent         ,   const int nContinentSize        ,
                                    char[]          szAutonomSysOrg     ,   const int nAutonomSysOrgSize    ,
                                    char[]          szIsp               ,   const int nIspSize              ,
                                    float&          fLatitude                                               ,
                                    float&          fLongitude                                              );

native  bool    GeoR_Record(        const char[]    szIpAddr                                                ,
                                    char[]          szCode              ,   const int nCodeSize             ,
                                    char[]          szCode3             ,   const int nCode3Size            ,
                                    char[]          szCountry           ,   const int nCountrySize          ,
                                    char[]          szCity              ,   const int nCitySize             ,
                                    char[]          szRegionCode        ,   const int nRegionCodeSize       ,
                                    char[]          szRegion            ,   const int nRegionSize           ,
                                    char[]          szTimeZone          ,   const int nTimeZoneSize         ,
                                    char[]          szPostalCode        ,   const int nPostalCodeSize       ,
                                    char[]          szContinentCode     ,   const int nContinentCodeSize    ,
                                    char[]          szContinent         ,   const int nContinentSize        ,
                                    char[]          szAutonomSysOrg     ,   const int nAutonomSysOrgSize    ,
                                    char[]          szIsp               ,   const int nIspSize              ,
                                    float&          fLatitude                                               ,
                                    float&          fLongitude                                              );

//
// (bImperial == true) ? Mi : Km
//

native  float   GeoR_Distance(      const float     fLatitude1          ,   const float fLongitude1         ,
                                    const float     fLatitude2          ,   const float fLongitude2         ,
                                    const bool      bImperial = false                                       );

native  float   GeoR_Length(        const float     fLatitude1          ,   const float fLongitude1         ,
                                    const float     fLatitude2          ,   const float fLongitude2         ,
                                    const bool      bImperial = false                                       );

native  float   GeoR_Len(           const float     fLatitude1          ,   const float fLongitude1         ,
                                    const float     fLatitude2          ,   const float fLongitude2         ,
                                    const bool      bImperial = false                                       );

/**
 * RETURNS WHICH    MAXMIND® DATABASES ARE IN USE
 *
 * GR_Db xDb    =   GeoR_Databases();
 *
 * if (xDb      ==  GEOIP_NONE)         { ... }
 * if (xDb      &   GEOIP_ISP_PAID)     { ... }
 *
 */

native  GR_Db       GeoR_Databases      ();
native  GR_Db       GeoR_Db             ();

/**
 * RELOADS ALL  THE MAXMIND® DATABASES
 * ALSO UPDATES THE MAXMIND® DATABASES  THAT ARE INTO THE   "/data/GeoResolver/Update/"     DIRECTORY
 */

native  bool        GeoR_Reload         ();
native  bool        GeoR_Refresh        ();
native  bool        GeoR_Restart        ();

/**
 * CHANGES  THE     ORDER   [   GEOIP_LITE_FIRST    |       GEOIP_PAID_FIRST    ]
 */

native  bool        GeoR_Order          (const  GR_Order    xOrder  =   GEOIP_LITE_FIRST);
native  bool        GeoR_SetOrder       (const  GR_Order    xOrder  =   GEOIP_LITE_FIRST);
native  bool        GeoR_ChangeOrder    (const  GR_Order    xOrder  =   GEOIP_LITE_FIRST);

/**
 * RETRIEVES    THE COUNTRY CODE BY IP ADDRESS
 */

GR_Fun  int     GeoR_Code(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szCode__);
}

/**
 * RETRIEVES    THE COUNTRY CODE BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_Code(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szCode__);
}

/**
 * RETRIEVES    THE COUNTRY CODE (3) BY IP ADDRESS
 */

GR_Fun  int     GeoR_Code3(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szCode3__);
}

/**
 * RETRIEVES    THE COUNTRY CODE (3) BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_Code3(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szCode3__);
}

/**
 * RETRIEVES    THE COUNTRY NAME BY IP ADDRESS
 */

GR_Fun  int     GeoR_Country(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szCountry__);
}

/**
 * RETRIEVES    THE COUNTRY NAME BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_Country(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szCountry__);
}

/**
 * RETRIEVES    THE CITY NAME BY IP ADDRESS
 */

GR_Fun  int     GeoR_City(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szCity__);
}

/**
 * RETRIEVES    THE CITY NAME BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_City(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szCity__);
}

/**
 * RETRIEVES    THE REGION CODE BY IP ADDRESS
 */

GR_Fun  int     GeoR_RegionCode(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szRegionCode__);
}

/**
 * RETRIEVES    THE REGION CODE BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_RegionCode(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szRegionCode__);
}

/**
 * RETRIEVES    THE REGION NAME BY IP ADDRESS
 */

GR_Fun  int     GeoR_Region(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szRegion__);
}

/**
 * RETRIEVES    THE REGION NAME BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_Region(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szRegion__);
}

/**
 * RETRIEVES    THE TIME ZONE BY IP ADDRESS
 */

GR_Fun  int     GeoR_TimeZone(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szTimeZone__);
}

/**
 * RETRIEVES    THE TIME ZONE BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_TimeZone(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szTimeZone__);
}

/**
 * RETRIEVES    THE POSTAL CODE BY IP ADDRESS
 */

GR_Fun  int     GeoR_PostalCode(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szPostalCode__);
}

/**
 * RETRIEVES    THE POSTAL CODE BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_PostalCode(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szPostalCode__);
}

/**
 * RETRIEVES    THE CONTINENT CODE BY IP ADDRESS
 */

GR_Fun  int     GeoR_ContinentCode(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szContinentCode__);
}

/**
 * RETRIEVES    THE CONTINENT CODE BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_ContinentCode(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szContinentCode__);
}

/**
 * RETRIEVES    THE CONTINENT NAME BY IP ADDRESS
 */

GR_Fun  int     GeoR_Continent(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szContinent__);
}

/**
 * RETRIEVES    THE CONTINENT NAME BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_Continent(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szContinent__);
}

/**
 * RETRIEVES    THE AUTONOMOUS SYSTEM ORGANIZATION BY IP ADDRESS
 */

GR_Fun  int     GeoR_AutonomousSysOrg(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szAutonomSysOrg__);
}

/**
 * RETRIEVES    THE AUTONOMOUS SYSTEM ORGANIZATION BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_AutonomousSysOrg(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szAutonomSysOrg__);
}

/**
 * RETRIEVES    THE ISP BY IP ADDRESS
 */

GR_Fun  int     GeoR_ISP(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szIsp__);
}

/**
 * RETRIEVES    THE ISP BY IP ADDRESS THREADED
 */

GR_Fun  int     GeoRT_ISP(const char[] szIpAddr, char[] szBuffer, const int nSize)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return strcopy(szBuffer, nSize, g_szIsp__);
}

/**
 * RETRIEVES    THE LATITUDE BY IP ADDRESS
 */

GR_Fun  float   GeoR_Latitude(const char[] szIpAddr)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return g_fLatitude__;
}

/**
 * RETRIEVES    THE LATITUDE BY IP ADDRESS THREADED
 */

GR_Fun  float   GeoRT_Latitude(const char[] szIpAddr)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return g_fLatitude__;
}

/**
 * RETRIEVES    THE LONGITUDE BY IP ADDRESS
 */

GR_Fun  float   GeoR_Longitude(const char[] szIpAddr)
{
    GeoR_CompleteRecord(szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return g_fLongitude__;
}

/**
 * RETRIEVES    THE LONGITUDE BY IP ADDRESS THREADED
 */

GR_Fun  float   GeoRT_Longitude(const char[] szIpAddr)
{
    GeoRT_ReadIp(       szIpAddr, g_szCode__, sizeof (g_szCode__), g_szCode3__, sizeof (g_szCode3__), g_szCountry__, sizeof (g_szCountry__),
                        g_szCity__, sizeof (g_szCity__), g_szRegionCode__, sizeof (g_szRegionCode__), g_szRegion__, sizeof (g_szRegion__),
                        g_szTimeZone__, sizeof (g_szTimeZone__), g_szPostalCode__, sizeof (g_szPostalCode__), g_szContinentCode__, sizeof (g_szContinentCode__),
                        g_szContinent__, sizeof (g_szContinent__), g_szAutonomSysOrg__, sizeof (g_szAutonomSysOrg__), g_szIsp__, sizeof (g_szIsp__),
                        g_fLatitude__, g_fLongitude__);

    return g_fLongitude__;
}

/**
 * RETRIEVES    THE DISTANCE BETWEEN    A CLIENT AND THE SERVER
 *
 * RETURNS 0.0  ON FAILURE
 *
 * float fKilometers        =   GeoR_DistancePlrSrv(nClient, false);
 *
 * if (fKilometers > 0.0)   { ... }
 * else                     { ... }
 *
 */

#if defined GR_NoDefFuncParams

GR_Fun  float       GeoR_DistancePlrSrv(const int nPlayer,  const bool bImperial)

#else

GR_Fun  float       GeoR_DistancePlrSrv(const int nPlayer,  const bool bImperial = false)

#endif

{
    static char     szPlrIpAddr[PLATFORM_MAX_PATH]          =   { EOS,  ... },
                    szSrvIpAddr[PLATFORM_MAX_PATH]          =   { EOS,  ... };

    static Handle   xSrvIpAddrConVar                        =   INVALID_HANDLE;

    static bool     bSrvIpAddrConVarRetrieved               =   false;

    static int      nSrvIpAddrNum                           =   0;

    static float    fPlrLatitude                            =   0.0,
                    fPlrLongitude                           =   0.0,
                    fSrvLatitude                            =   0.0,
                    fSrvLongitude                           =   0.0;

    if (nPlayer < 1                     ||
        nPlayer > MaxClients            ||
        !IsClientConnected(nPlayer)     ||
        !IsClientInGame(nPlayer)        ||
        IsFakeClient(nPlayer)           ||
        IsClientSourceTV(nPlayer)       ||
        IsClientReplay(nPlayer)         ||
        IsClientTimingOut(nPlayer)      ||
        IsClientInKickQueue(nPlayer)    )
    {
        return              0.0;
    }

    if (false   ==  bSrvIpAddrConVarRetrieved)
    {
        if (xSrvIpAddrConVar    ==  INVALID_HANDLE)
        {
            xSrvIpAddrConVar    =   FindConVar("hostip");
        }

        bSrvIpAddrConVarRetrieved = true;
    }

    if (xSrvIpAddrConVar    ==      INVALID_HANDLE)
    {
        return              0.0;
    }

    nSrvIpAddrNum       =   GetConVarInt(xSrvIpAddrConVar);

    if (nSrvIpAddrNum   ==  0)
    {
        return              0.0;
    }

    if (!GetClientIP(nPlayer,   szPlrIpAddr,        sizeof (szPlrIpAddr),   true))
    {
        return              0.0;
    }

    if (-1 == StrContains(szPlrIpAddr, ".", false))
    {
        return              0.0;
    }

    FormatEx(   szSrvIpAddr                         ,
                sizeof (szSrvIpAddr)                ,
                "%u.%u.%u.%u"                       ,
                (   nSrvIpAddrNum >> 24 )   & 0xFF  ,
                (   nSrvIpAddrNum >> 16 )   & 0xFF  ,
                (   nSrvIpAddrNum >> 8  )   & 0xFF  ,
                (   nSrvIpAddrNum       )   & 0xFF  );

    fPlrLatitude    =   GeoR_Latitude   (szPlrIpAddr);
    fPlrLongitude   =   GeoR_Longitude  (szPlrIpAddr);

    fSrvLatitude    =   GeoR_Latitude   (szSrvIpAddr);
    fSrvLongitude   =   GeoR_Longitude  (szSrvIpAddr);

    return  GeoR_Distance(fPlrLatitude, fPlrLongitude, fSrvLatitude, fSrvLongitude, bImperial);
}

//
// GeoR_DistancePlrSrv
//

#define GeoR_DistanceSrvPlr(%0,%1)              GeoR_DistancePlrSrv(%0, %1)

#define GeoR_DistancePlayerServer(%0,%1)        GeoR_DistancePlrSrv(%0, %1)
#define GeoR_DistanceServerPlayer(%0,%1)        GeoR_DistancePlrSrv(%0, %1)

#define GeoR_LengthPlrSrv(%0,%1)                GeoR_DistancePlrSrv(%0, %1)
#define GeoR_LengthSrvPlr(%0,%1)                GeoR_DistancePlrSrv(%0, %1)

#define GeoR_LengthPlayerServer(%0,%1)          GeoR_DistancePlrSrv(%0, %1)
#define GeoR_LengthServerPlayer(%0,%1)          GeoR_DistancePlrSrv(%0, %1)

#define GeoR_LenPlrSrv(%0,%1)                   GeoR_DistancePlrSrv(%0, %1)
#define GeoR_LenSrvPlr(%0,%1)                   GeoR_DistancePlrSrv(%0, %1)

#define GeoR_LenPlayerServer(%0,%1)             GeoR_DistancePlrSrv(%0, %1)
#define GeoR_LenServerPlayer(%0,%1)             GeoR_DistancePlrSrv(%0, %1)

//
// GeoR_AutonomousSysOrg
//

#define GeoR_AutonomSysOrg(%0,%1,%2)            GeoR_AutonomousSysOrg(%0, %1, %2)

#define GeoR_AutoSysOrg(%0,%1,%2)               GeoR_AutonomousSysOrg(%0, %1, %2)

//
// GeoRT_AutonomousSysOrg
//

#define GeoRT_AutonomSysOrg(%0,%1,%2)           GeoRT_AutonomousSysOrg(%0, %1, %2)

#define GeoRT_AutoSysOrg(%0,%1,%2)              GeoRT_AutonomousSysOrg(%0, %1, %2)

//
// GeoR_ISP
//

#define GeoR_Isp(%0,%1,%2)                      GeoR_ISP(%0, %1, %2)

//
// GeoRT_ISP
//

#define GeoRT_Isp(%0,%1,%2)                     GeoRT_ISP(%0, %1, %2)

//
// GeoR_Code
//

#define GeoR_CountryCode(%0,%1,%2)              GeoR_Code(%0, %1, %2)

#define GeoR_IsoCode(%0,%1,%2)                  GeoR_Code(%0, %1, %2)
#define GeoR_ISOCode(%0,%1,%2)                  GeoR_Code(%0, %1, %2)

#define GeoR_IsoCode2(%0,%1,%2)                 GeoR_Code(%0, %1, %2)
#define GeoR_ISOCode2(%0,%1,%2)                 GeoR_Code(%0, %1, %2)

#define GeoR_CountryCode2(%0,%1,%2)             GeoR_Code(%0, %1, %2)

#define GeoR_Code2(%0,%1,%2)                    GeoR_Code(%0, %1, %2)

#define GeoR_CCode2(%0,%1,%2)                   GeoR_Code(%0, %1, %2)

//
// GeoRT_Code
//

#define GeoRT_CountryCode(%0,%1,%2)             GeoRT_Code(%0, %1, %2)

#define GeoRT_IsoCode(%0,%1,%2)                 GeoRT_Code(%0, %1, %2)
#define GeoRT_ISOCode(%0,%1,%2)                 GeoRT_Code(%0, %1, %2)

#define GeoRT_IsoCode2(%0,%1,%2)                GeoRT_Code(%0, %1, %2)
#define GeoRT_ISOCode2(%0,%1,%2)                GeoRT_Code(%0, %1, %2)

#define GeoRT_CountryCode2(%0,%1,%2)            GeoRT_Code(%0, %1, %2)

#define GeoRT_Code2(%0,%1,%2)                   GeoRT_Code(%0, %1, %2)

#define GeoRT_CCode2(%0,%1,%2)                  GeoRT_Code(%0, %1, %2)

//
// GeoR_Code3
//

#define GeoR_CountryCode3(%0,%1,%2)             GeoR_Code3(%0, %1, %2)

#define GeoR_CCode3(%0,%1,%2)                   GeoR_Code3(%0, %1, %2)

#define GeoR_ISOCode3(%0,%1,%2)                 GeoR_Code3(%0, %1, %2)
#define GeoR_IsoCode3(%0,%1,%2)                 GeoR_Code3(%0, %1, %2)

//
// GeoRT_Code3
//

#define GeoRT_CountryCode3(%0,%1,%2)            GeoRT_Code3(%0, %1, %2)

#define GeoRT_CCode3(%0,%1,%2)                  GeoRT_Code3(%0, %1, %2)

#define GeoRT_ISOCode3(%0,%1,%2)                GeoRT_Code3(%0, %1, %2)
#define GeoRT_IsoCode3(%0,%1,%2)                GeoRT_Code3(%0, %1, %2)

//
// GeoR_Db | GeoR_Databases
//

#define GeoR_Database()                         GeoR_Db()
#define GeoR_DataBase()                         GeoR_Db()
#define GeoR_DATABASE()                         GeoR_Db()

#define GeoR_DataBases()                        GeoR_Db()
#define GeoR_DATABASES()                        GeoR_Db()

#define GeoR_DB()                               GeoR_Db()

#define GeoR_Dbs()                              GeoR_Db()
#define GeoR_DBs()                              GeoR_Db()
#define GeoR_DBS()                              GeoR_Db()

//
// GeoRT_AddIp
//

#define GeoRT_Add(%0)                           GeoRT_AddIp(%0)
#define GeoRT_ADD(%0)                           GeoRT_AddIp(%0)

//
// GeoRT_Free
//

#define GeoRT_FREE()                            GeoRT_Free()

#define GeoRT_FreeAll()                         GeoRT_Free()

#define GeoRT_Clear()                           GeoRT_Free()
#define GeoRT_CLEAR()                           GeoRT_Free()

#define GeoRT_ClearAll()                        GeoRT_Free()

//
// GR_Db
//

#define GR_Database                             GR_Db
#define GR_DataBase                             GR_Db
#define GR_DATABASE                             GR_Db

#define GR_Databases                            GR_Db
#define GR_DataBases                            GR_Db
#define GR_DATABASES                            GR_Db

#define GR_DB                                   GR_Db

#define GR_Dbs                                  GR_Db
#define GR_DBs                                  GR_Db
#define GR_DBS                                  GR_Db

public  Extension   __ext_GeoResolver   =
{
    name        =   "GeoResolver",

    file        =   "GeoResolver.ext",

#if defined(AUTOLOAD_EXTENSIONS) && defined(REQUIRE_EXTENSIONS)

    autoload    =   1,
    required    =   1,

#endif

#if defined(AUTOLOAD_EXTENSIONS) && !defined(REQUIRE_EXTENSIONS)

    autoload    =   1,
    required    =   0,

#endif

#if !defined(AUTOLOAD_EXTENSIONS) && defined(REQUIRE_EXTENSIONS)

    autoload    =   0,
    required    =   1,

#endif

#if !defined(AUTOLOAD_EXTENSIONS) && !defined(REQUIRE_EXTENSIONS)

    autoload    =   0,
    required    =   0,

#endif

};

#if     !defined(REQUIRE_EXTENSIONS)

public  void    __ext_GeoResolver_SetNTVOptional()
{
    MarkNativeAsOptional("GeoRT_Add"            );
    MarkNativeAsOptional("GeoRT_AddIp"          );

    MarkNativeAsOptional("GeoRT_Read"           );
    MarkNativeAsOptional("GeoRT_ReadIp"         );

    MarkNativeAsOptional("GeoRT_Free"           );

    MarkNativeAsOptional("GeoR_CompleteRecord"  );
    MarkNativeAsOptional("GeoR_FullRecord"      );
    MarkNativeAsOptional("GeoR_GetRecord"       );
    MarkNativeAsOptional("GeoR_Record"          );

    MarkNativeAsOptional("GeoR_Distance"        );
    MarkNativeAsOptional("GeoR_Length"          );
    MarkNativeAsOptional("GeoR_Len"             );

    MarkNativeAsOptional("GeoR_Databases"       );
    MarkNativeAsOptional("GeoR_Db"              );

    MarkNativeAsOptional("GeoR_Reload"          );
    MarkNativeAsOptional("GeoR_Refresh"         );
    MarkNativeAsOptional("GeoR_Restart"         );

    MarkNativeAsOptional("GeoR_ChangeOrder"     );
    MarkNativeAsOptional("GeoR_Order"           );
    MarkNativeAsOptional("GeoR_SetOrder"        );
}

#endif
