#define SOUNDPLAY_ALIAS _T("AHK_PlayMe")  // Used by destructor and SoundPlay().
ResultType Line::SoundPlay(LPTSTR aFilespec, bool aSleepUntilDone)
{
	LPTSTR cp = omit_leading_whitespace(aFilespec);
	if (*cp == '*')
		return SetErrorLevelOrThrowBool(!MessageBeep(ATOU(cp + 1)));
		// ATOU() returns 0xFFFFFFFF for -1, which is relied upon to support the -1 sound.
	// See http://msdn.microsoft.com/library/default.asp?url=/library/en-us/multimed/htm/_win32_play.asp
	// for some documentation mciSendString() and related.
	// MAX_PATH note: There's no chance this API supports long paths even on Windows 10.
	// Research indicates it limits paths to 127 chars (not even MAX_PATH), but since there's
	// no apparent benefit to reducing it, we'll keep this size to ensure backward-compatibility.
	// Note that using a relative path does not help, but using short (8.3) names does.
	TCHAR buf[MAX_PATH * 2]; // Allow room for filename and commands.
	mciSendString(_T("status ") SOUNDPLAY_ALIAS _T(" mode"), buf, _countof(buf), NULL);
	if (*buf) // "playing" or "stopped" (so close it before trying to re-open with a new aFilespec).
		mciSendString(_T("close ") SOUNDPLAY_ALIAS, NULL, 0, NULL);
	sntprintf(buf, _countof(buf), _T("open \"%s\" alias ") SOUNDPLAY_ALIAS, aFilespec);
	if (mciSendString(buf, NULL, 0, NULL)) // Failure.
		return SetErrorLevelOrThrow();
	g_SoundWasPlayed = true;  // For use by Script's destructor.
	if (mciSendString(_T("play ") SOUNDPLAY_ALIAS, NULL, 0, NULL)) // Failure.
		return SetErrorLevelOrThrow();
	// Otherwise, the sound is now playing.
	g_ErrorLevel->Assign(ERRORLEVEL_NONE);
	if (!aSleepUntilDone)
		return OK;
	// Otherwise, caller wants us to wait until the file is done playing.  To allow our app to remain
	// responsive during this time, use a loop that checks our message queue:
	// Older method: "mciSendString("play " SOUNDPLAY_ALIAS " wait", NULL, 0, NULL)"
	for (;;)
	{
		mciSendString(_T("status ") SOUNDPLAY_ALIAS _T(" mode"), buf, _countof(buf), NULL);
		if (!*buf) // Probably can't happen given the state we're in.
			break;
		if (!_tcscmp(buf, _T("stopped"))) // The sound is done playing.
		{
			mciSendString(_T("close ") SOUNDPLAY_ALIAS, NULL, 0, NULL);
			break;
		}
		// Sleep a little longer than normal because I'm not sure how much overhead
		// and CPU utilization the above incurs:
		MsgSleep(20);
	}
	return OK;
}


void PlayWavMusic()
{
PlaySound("GAME_WIN.WAV",NULL/*AfxGetInstanceHandle()*/, SND_FILENAME|SND_ASYNC);
}

void PlayMp3Music_1()
{
mciSendString(L"open C:\\Users\\Administrator\\Videos\\MagicSword.mp3" , NULL, 0, 0);
mciSendString(L"play C:\\Users\\Administrator\\Videos\\MagicSword.mp3", NULL, 0, 0);
}
文链接：https://blog.csdn.net/woshiyuanlei/article/details/46291187