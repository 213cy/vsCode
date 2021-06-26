ToolTip(LPTSTR aText, LPTSTR aX, LPTSTR aY, LPTSTR aID)
{
	int window_index = *aID ? ATOI(aID) - 1 : 0;
	if (window_index < 0 || window_index >= MAX_TOOLTIPS)
		return LineError(_T("Max window number is ") MAX_TOOLTIPS_STR _T("."), FAIL, aID);
	HWND tip_hwnd = g_hWndToolTip[window_index];

	// Destroy windows except the first (for performance) so that resources/mem are conserved.
	// The first window will be hidden by the TTM_UPDATETIPTEXT message if aText is blank.
	// UPDATE: For simplicity, destroy even the first in this way, because otherwise a script
	// that turns off a non-existent first tooltip window then later turns it on will cause
	// the window to appear in an incorrect position.  Example:
	// ToolTip
	// ToolTip, text, 388, 24
	// Sleep, 1000
	// ToolTip, text, 388, 24
	if (!*aText)
	{
		if (tip_hwnd && IsWindow(tip_hwnd))
			DestroyWindow(tip_hwnd);
		g_hWndToolTip[window_index] = NULL;
		return OK;
	}

	// Use virtual desktop so that tooltip can move onto non-primary monitor in a multi-monitor system:
	RECT dtw;
	GetVirtualDesktopRect(dtw);

	bool one_or_both_coords_unspecified = !*aX || !*aY;
	POINT pt, pt_cursor;
	if (one_or_both_coords_unspecified)
	{
		// Don't call GetCursorPos() unless absolutely needed because it seems to mess
		// up double-click timing, at least on XP.  UPDATE: Is isn't GetCursorPos() that's
		// interfering with double clicks, so it seems it must be the displaying of the ToolTip
		// window itself.
		GetCursorPos(&pt_cursor);
		pt.x = pt_cursor.x + 16;  // Set default spot to be near the mouse cursor.
		pt.y = pt_cursor.y + 16;  // Use 16 to prevent the tooltip from overlapping large cursors.
		// Update: Below is no longer needed due to a better fix further down that handles multi-line tooltips.
		// 20 seems to be about the right amount to prevent it from "warping" to the top of the screen,
		// at least on XP:
		//if (pt.y > dtw.bottom - 20)
		//	pt.y = dtw.bottom - 20;
	}

	POINT origin = {0};
	if (*aX || *aY) // Need the offsets.
		CoordToScreen(origin, COORD_MODE_TOOLTIP);

	// This will also convert from relative to screen coordinates if appropriate:
	if (*aX)
		pt.x = ATOI(aX) + origin.x;
	if (*aY)
		pt.y = ATOI(aY) + origin.y;

	TOOLINFO ti = {0};
	ti.cbSize = sizeof(ti) - sizeof(void *); // Fixed for v1.0.36.05: Tooltips fail to work on Windows 2000 unless the size for the *lpReserved member in _WIN32_WINNT 0x0501 is omitted.
	ti.uFlags = TTF_TRACK;
	ti.lpszText = aText;
	// Note that the ToolTip won't work if ti.hwnd is assigned the HWND from GetDesktopWindow().
	// All of ti's other members are left at NULL/0, including the following:
	//ti.hinst = NULL;
	//ti.uId = 0;
	//ti.rect.left = ti.rect.top = ti.rect.right = ti.rect.bottom = 0;

	// My: This does more harm that good (it causes the cursor to warp from the right side to the left
	// if it gets to close to the right side), so for now, I did a different fix (above) instead:
	//ti.rect.bottom = dtw.bottom;
	//ti.rect.right = dtw.right;
	//ti.rect.top = dtw.top;
	//ti.rect.left = dtw.left;

	// No need to use SendMessageTimeout() since the ToolTip() is owned by our own thread, which
	// (since we're here) we know is not hung or heavily occupied.

	// v1.0.40.12: Added the IsWindow() check below to recreate the tooltip in cases where it was destroyed
	// by external means such as Alt-F4 or WinClose.
	if (!tip_hwnd || !IsWindow(tip_hwnd))
	{
		// This this window has no owner, it won't be automatically destroyed when its owner is.
		// Thus, it will be explicitly by the program's exit function.
		tip_hwnd = g_hWndToolTip[window_index] = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_NOPREFIX | TTS_ALWAYSTIP
			, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);
		SendMessage(tip_hwnd, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
		// v1.0.21: GetSystemMetrics(SM_CXSCREEN) is used for the maximum width because even on a
		// multi-monitor system, most users would not want a tip window to stretch across multiple monitors:
		SendMessage(tip_hwnd, TTM_SETMAXTIPWIDTH, 0, (LPARAM)GetSystemMetrics(SM_CXSCREEN));
		// Must do these next two when the window is first created, otherwise GetWindowRect() below will retrieve
		// a tooltip window size that is quite a bit taller than it winds up being:
		SendMessage(tip_hwnd, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(pt.x, pt.y));
		SendMessage(tip_hwnd, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
	}
	// Bugfix for v1.0.21: The below is now called unconditionally, even if the above newly created the window.
	// If this is not done, the tip window will fail to appear the first time it is invoked, at least when
	// all of the following are true:
	// 1) Windows XP;
	// 2) Common controls v6 (via manifest);
	// 3) "Control Panel >> Display >> Effects >> Use transition >> Fade effect" setting is in effect.
	SendMessage(tip_hwnd, TTM_UPDATETIPTEXT, 0, (LPARAM)&ti);

	RECT ttw = {0};
	GetWindowRect(tip_hwnd, &ttw); // Must be called this late to ensure the tooltip has been created by above.
	int tt_width = ttw.right - ttw.left;
	int tt_height = ttw.bottom - ttw.top;

	// v1.0.21: Revised for multi-monitor support.  I read somewhere that dtw.left can be negative (perhaps
	// if the secondary monitor is to the left of the primary).  So it seems best to assume it is possible:
	if (pt.x + tt_width >= dtw.right)
		pt.x = dtw.right - tt_width - 1;
	if (pt.y + tt_height >= dtw.bottom)
		pt.y = dtw.bottom - tt_height - 1;
	// It seems best not to have each of the below paired with the above.  This is because it allows
	// the flexibility to explicitly move the tooltip above or to the left of the screen.  Such a feat
	// should only be possible if done via explicitly passed-in negative coordinates for aX and/or aY.
	// In other words, it should be impossible for a tooltip window to follow the mouse cursor somewhere
	// off the virtual screen because:
	// 1) The mouse cursor is normally trapped within the bounds of the virtual screen.
	// 2) The tooltip window defaults to appearing South-East of the cursor.  It can only appear
	//    in some other quadrant if jammed against the right or bottom edges of the screen, in which
	//    case it can't be partially above or to the left of the virtual screen unless it's really
	//    huge, which seems very unlikely given that it's limited to the maximum width of the
	//    primary display as set by TTM_SETMAXTIPWIDTH above.
	//else if (pt.x < dtw.left) // Should be impossible for this to happen due to mouse being off the screen.
	//	pt.x = dtw.left;      // But could happen if user explicitly passed in a coord that was too negative.
	//...
	//else if (pt.y < dtw.top)
	//	pt.y = dtw.top;

	if (one_or_both_coords_unspecified)
	{
		// Since Tooltip is being shown at the cursor's coordinates, try to ensure that the above
		// adjustment doesn't result in the cursor being inside the tooltip's window boundaries,
		// since that tends to cause problems such as blocking the tray area (which can make a
		// tooltip script impossible to terminate).  Normally, that can only happen in this case
		// (one_or_both_coords_unspecified == true) when the cursor is near the bottom-right
		// corner of the screen (unless the mouse is moving more quickly than the script's
		// ToolTip update-frequency can cope with, but that seems inconsequential since it
		// will adjust when the cursor slows down):
		ttw.left = pt.x;
		ttw.top = pt.y;
		ttw.right = ttw.left + tt_width;
		ttw.bottom = ttw.top + tt_height;
		if (pt_cursor.x >= ttw.left && pt_cursor.x <= ttw.right && pt_cursor.y >= ttw.top && pt_cursor.y <= ttw.bottom)
		{
			// Push the tool tip to the upper-left side, since normally the only way the cursor can
			// be inside its boundaries (when one_or_both_coords_unspecified == true) is when the
			// cursor is near the bottom right corner of the screen.
			pt.x = pt_cursor.x - tt_width - 3;    // Use a small offset since it can't overlap the cursor
			pt.y = pt_cursor.y - tt_height - 3;   // when pushed to the the upper-left side of it.
		}
	}

	SendMessage(tip_hwnd, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(pt.x, pt.y));
	// And do a TTM_TRACKACTIVATE even if the tooltip window already existed upon entry to this function,
	// so that in case it was hidden or dismissed while its HWND still exists, it will be shown again:
	SendMessage(tip_hwnd, TTM_TRACKACTIVATE, TRUE, (LPARAM)&ti);
	return OK;
}

