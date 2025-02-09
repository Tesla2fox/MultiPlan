#ifndef _GPS_TO_LOCAL_H
#define _GPS_TO_LOCAL_H

#if defined(LIBGPS2LOCAL_EXPORTS)
#if defined(_MSC_VER)
#define LIBGPS2LOCAL_API __declspec(dllexport)
#else
#define LIBGPS2LOCAL_API __attribute__((__visibility__("default")))
#endif
#elif defined(_MSC_VER)
#define LIBGPS2LOCAL_API __declspec(dllimport)
#else
#define LIBGPS2LOCAL_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ??????????? GPS2Local/Local2GPS
 * ????: GPS???? ?? ???????????????
 *		 ?????????????, x?????????????????�֩�????? ?????GPSSetAnchor/GPSGetAnchor????/???
 *       (???x?????, y?????)
 *
 * GPS????: lat ???, lon ????, th_gps ??????(???, ???90, ???0, ???????????????)
 *      ??: lat = 41.761128182994, lon = 123.441580684147, heading = 45
 * ???????: x (??), y (??), th_local ??????(???, x?????0, y?????90)
 *
 * ??????????????????, ???? GPS2Local(lat, lon, 0, &x, &y, NULL)
 *                              Local2GPS(x, y, 0, &lat, &lon, NULL) ????
 */
LIBGPS2LOCAL_API void GPS2Local(double lat, double lon, double th_gps, double* x, double* y, double* th_local);
LIBGPS2LOCAL_API void Local2GPS(double x, double y, double th_local, double* lat, double* lon, double* th_gps);

/*
 * ???????????????
 * ????: ????/??? ????????????? ?? ??????
 */
LIBGPS2LOCAL_API void GPSSetAnchor(double lat, double lon, double th_gps);
LIBGPS2LOCAL_API void GPSGetAnchor(double* lat, double* lon, double* th_gps);

/*
 * ??????????��??GPS??????
 * ??????????:

 ref_lat 39.962953093632
 ref_lon 116.304291427993
 ref_th  0

 */
LIBGPS2LOCAL_API void GPSLoadConfig(const char* config_file);

#ifdef __cplusplus
}      // extern "C"
#endif

#endif // _GPS_TO_LOCAL_H
