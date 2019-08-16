
/*
 * -----------------------------------------------------------------------
 * 
 * FUNCTION:  int WINAPI      DeleteOldLogs(int nDaysOld)
 * 
 * PURPOSE:
 * 
 * -----------------------------------------------------------------------
 */
int WINAPI      DeleteOldLogs(int nDaysOld)
{
	struct _find_t  c_file;
	BYTE            szPath[MAXFILENAME];
	BYTE            szLogText[MAXFILENAME + 40];
	BYTE            szDate[20];
	LONG            lToday;
	BYTE            szMonth[3];
	BYTE            szDay[3];
	BYTE            szYear[3];

	_strdate((LPSTR) szDate);

	lstrcpyn((LPSTR) szMonth, (LPCSTR) szDate, 3);
	lstrcpyn((LPSTR) szDay, (LPCSTR) & szDate[3], 3);
	lstrcpyn((LPSTR) szYear, (LPCSTR) & szDate[6], 3);

	lToday = (atoi((LPSTR) szMonth) * 30) + atoi((LPSTR) szDay) + (atoi((LPSTR) szYear) * 365);

	wsprintf((LPSTR) szPath, "%s*.log", (LPSTR) wapiInfo.szLogDir);

	if (!_dos_findfirst((LPSTR) szPath, _A_NORMAL, &c_file))
		do
		{
			if ((c_file.name[2] == '-') && (c_file.name[5] == '-'))
			{
				lstrcpyn((LPSTR) szMonth, (LPCSTR) c_file.name, 3);
				lstrcpyn((LPSTR) szDay, (LPCSTR) & c_file.name[3], 3);
				lstrcpyn((LPSTR) szYear, (LPCSTR) & c_file.name[6], 3);

				if (((lToday - ((atoi((LPSTR) szMonth) * 30) + atoi((LPSTR) szDay) + (atoi((LPSTR) szYear) * 365)))) > nDaysOld)
				{
					wsprintf((LPSTR) szPath, "%s%s", (LPSTR) wapiInfo.szLogDir, (LPSTR) c_file.name);

					DeletePathname((LPSTR) szPath);

					wsprintf((LPSTR) szLogText, "Purged Old Log File: %s", (LPSTR) szPath);
					WriteToCurrentLogFile((LPCSTR) szLogText);
				}
			}
		} while (!_dos_findnext(&c_file));

	return TRUE;
}
