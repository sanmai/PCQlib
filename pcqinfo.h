/*****************************************************************************
 *
 * FILE:	pcqinfo.h
 * SYNOPSIS:	GPS library: header file for GPS information
 * DATE:	Sat Mar 17 14:45:20 JST 2001
 * Editor: Hirotaka Horie<nandu@ht.sfc.keio.ac.jp>
 * OriginalFILE:	pacyinfo.h
 * OriginalDATE:	Thu, Dec 17 1998
 * OriginalAUTHOR:	Kouichi ABE (WALL)
 * OriginalAuthorE-MAIL:	kouichi@etn.yz.yamagata-u.ac.jp
 *
 *****************************************************************************/

#ifndef	_pcqinfo_h
#define	_pcqinfo_h	1

static char	*prn_str = "ABCDEFGHIJKLMNOPQRSTUVWXabcdefgh";

static char	mapdatum_str[][94] = {	/* 高度補正データ	*/
  "WGS-84",	/* 補正の必要無し */
  "Tokyo",
  "ADINDAN",
  "ARC 1950",
  "MERCHICH",
  "HONG KONG 1963",
  "SOUTH ASIA",
  "LUZON",
  "INDIAN",
  "INDIAN",
  "FERTAU 1948",
  "NORTH AMERICAN 1927 (use for Belize, Costa Rica, El Salvador, "
  "Guatemala, Honduras, Nicaragua)",
  "EUROPEAN 1950, EUROPEAN 1979",
  "IRELAND 1965     ",
  "ORDNANCE SURVEY OF GREAT BRITAN 1936",
  "NAHRWAN",
  "NAHRWAN",
  "OLD EGYPTIAN",
  "NORTH AMERICAN 1927  (use for Canada, Newfoundland Island)",
  "NORTH AMERICAN 1983  (Alaska, Canada, Mexico, Central America, USA)",
  "AUSTRALIAN GEODETIC 1984",
  "GEODETIC DATUM 1949",
  "PROVISIONAL SOUTH AMERICAN 1956",
  "SOUTH AMERICAN 1969",
  "CAMPO INCHAUSPE",
  "CORREGO ALEGRE"
};


static char	dop_str[][8] = {
  "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
  "11 - 12", "13 - 15", "16 - 20", "21 - 30", "31 - 50",
  "51 - 99", "100+"
};

static int	dop_val[] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
  12, 14, 18, 25, 40, 75, 100
};


static char	elevation_str[][7] = {
  " 0.. 5", " 6..15", "16..25", "26..35", "36..45",
  "46..55", "56..65", "66..75", "76..85", "86..90"
};

static int	elevation_val[] = {
  0, 10, 20, 30, 40, 50, 60, 70, 80, 90
};


static char	azimuth_str[][9] = {
  "  0..  5", "  6.. 15", " 16.. 25", " 26.. 35", " 36.. 45",
  " 46.. 55", " 56.. 65", " 66.. 75", " 76.. 85", " 86.. 95",
  " 90..105", "106..115", "116..125", "126..135", "136..145",
  "146..155", "156..165", "166..175", "176..180"
};

static int	azimuth_val[] = {
  0, 10, 20, 30, 40, 50, 60, 70, 80, 90,
  100, 110, 120, 130, 140, 150, 160, 170, 180
};


static char	situation_str[][8] = {
  "SCAN", "LOCK", "READY", "HOLD", "ILL", "OK"
};

static char	situation_char[] = ".o+-x*";

#endif	/* _pcqinfo_h */
