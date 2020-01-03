// DD.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DD.h"

// Global Variables:
HINSTANCE   g_hInst;                                    // Current instance
const TCHAR g_cptClassName[] = _T("DIVI-DEAD_GAME");        // The title bar text
const TCHAR g_cptWindowName[] = _T("DIVI-DEAD C'sWARE");    // The main window class name
HMENU       g_hMenu;            // The main window menu handle
int         g_iScreenX;         // Display size X
int         g_iScreenY;         // Display size Y
int         g_iWindowSizeX = 640;       // Window size X
int         g_iWindowSizeY = 480;       // Window size Y
UINT        g_uiWindowStyle;    // Window style
bool        g_bWindowModeReal;
bool        g_bWindowModeSelected;
bool        g_bScreenUpdatedOrMouseMoved;
bool        g_bResetMenuSelection;
HWND        g_hMainWindow;
bool        g_bShowingFullScrMenu;
HDC         g_hMainDc;
HDC         g_hDcBuffer1;
HDC         g_hDcBuffer2;
HDC         g_hDcMenuGfx;
HBITMAP     g_hBuf1Bitmap;
HBITMAP     g_hBuf2Bitmap;
HBITMAP     g_hMenuGFX_Bitmap;
HBITMAP     g_hOldBuf1Bitmap;
HBITMAP     g_hOldBuf2Bitmap;
HBITMAP     g_hOld_MenuGFX_Bitmap;
bool        g_bPaletteSupported;
int         g_iScreenDepth;
UINT        g_uMIDIDevsCnt;
UINT        g_uWaveDevsCnt;
bool        g_bSoundEnabled;
bool        g_bVoiceEnabled;
bool        g_bBgmEnabled;
void        *g_pvScriptBuffer;
void        *g_pvDataBuffer;
void        *g_pvSoundBuffer;
HFONT       g_hFont;
HFONT       g_hMainDcOldFont;
HFONT       g_hBuf1OldFont;
TCHAR       g_ptSaveFolder[MAX_PATH];
TCHAR       g_ptWorkingDir[MAX_PATH];
TCHAR       g_ptFullFileName[MAX_PATH];
int         g_iCurrPosInScript;
SaveFileState   g_sGameState;
SysFileStruc    g_sSysFile;
HANDLE          g_hFile;
DWORD           g_dwNumberOfBytesRead;
DWORD           g_ulTimeStamp;
TCHAR           g_ptStartScriptFile[64] = _T("AASTART");
WORD            g_wKeyCommand;
WORD            g_sPrevKeyCommand;
BYTE            g_bKeyCommand2;
bool            g_bCtrlPressed;
bool            g_bShiftPressed;
TCHAR           g_pcSGFileName[MAX_PATH];
ArhiveHdr       g_pcSGFileHdr;
ArhiveFileEntry *g_pcSGArhiveFileTable;
TCHAR           g_pcWVFileName[MAX_PATH];
ArhiveHdr       g_pcWVFileHdr;
ArhiveFileEntry *g_pcWVArhiveFileTable;
WORD            g_uiArhiveEntryCount;
ArhiveFileEntry g_pcFLIST[FLIST_ENTRY_COUNT];
char            g_pcHDR[16];
DWORD           g_lDistanceToMove;
DWORD           g_dwNumberOfBytesToRead;
ArhiveFileEntry g_pcFileToFind;
PackedFileHdr   g_sPackedFileHdr;
BYTE            g_pbSlideWinBuf[4096];
BYTE            *g_pbUnpackedFileEndPos;
short           g_sPicWidth = 640;
short           g_sPicHeight = 480;
int             g_iOriginX;
int             g_iOriginY;
OFFSET_STRUCT   g_psPicOriginPositions[6] = {
    0, 0,           // This part goes to the visible area
    640, 0,         // This part goes to the right from visible area
    0, 480,         // This part goes to the bottom from visible area
    640, 480,       // This part goes to the right and bottom from visible area
};

POINT   g_sTextWindowPos = { 112, 398 };

int             g_iX;
int             g_iY;
short           g_sPicOffsetNum;
int             g_iRequiredAction;
HANDLE          g_hThread;
DWORD           g_dwThreadId;
DWORD           g_dwNumberOfBytesWritten;
int             g_iMCICommandResult;
char            g_pcWVFileToFind[16];
long            g_lWVFileSize;
TCHAR           g_pcVideoFileToPlay[MAX_PATH];
long                g_lVideoPlaybackSpeed;
bool                g_bVideoIsPlaying;
bool                g_bDisplayModeChanged;
bool                g_bGraphicEffectsRunning;
short               g_iMousePosX;
short               g_iMousePosY;
bool                g_bTrackingMouse;
int                 g_iCloseWindowResponce;
bool                g_bFullScrModeFromDlg;
DEVMODE             g_sDevModePars;
TCHAR               g_cptSceneFileName[64];
short               g_sIntermediateResult;
bool                g_bClearScreen;
bool                g_bWaitingUserInput;
int                 g_iBookProgressIdx;
bool                g_bMenuOnScreen;
int                 g_iStringHeight = 22;
int                 g_iTextLineHeight = 22;
int                 g_iMenuLineHeight = 22;

TCHAR               g_pptIngameMenuStrs[6][25] = {
    _T("         START          "),
    _T("          SAVE          "),
    _T("          LOAD          "),
    _T("         CONFIG         "),
    _T("          END           "),
    _T("\x00")
};

TCHAR               g_pptConfigMenuStrs[5][25] = {
    _T("     VOICE  :  ON       "),
    _T("     SOUND  :  ON       "),
    _T("       BGM  :  ON       "),
    _T("       VGA  :  ON       "),
    _T("\x00")
};

TCHAR               g_pptStartMenuStrs[5][25] = {
    _T("         START          "),
    _T("          LOAD          "),
    _T("         CONFIG         "),
    _T("          END           "),
    _T("\x00")
};

TCHAR               g_pptExtrasMenuStrs[4][25] = {
    _T("     CG Hit Display     "),
    _T("         Extras         "),
    _T("        Save BMP        "),
    _T("\x00")
};

TCHAR               g_pptYesNoStrs[3][25] = {
    _T("          YES           "),
    _T("           NO           "),
    _T("\x00")
};

TCHAR               g_pptOffOn[3][10] = {
    _T("OFF      "),
    _T("ON       "),
    _T("\x00")
};

TCHAR               g_ptSaveLoadMenuStrings[11][25];

short               g_iSelectedMenuItem = -1;
int                 g_iNewPosInScript;
POINT               g_sPoint;
short               g_sMenuLength;
TCHAR               g_pptMenuStrings[MAX_MENU_STRINGS][MAX_MENU_STRING_LEN];
int                 g_piMenuStringLengths[MAX_MENU_STRINGS];
TCHAR               g_ptScriptName[MAX_PATH];
COLORREF            g_crMenuFGColor;
COLORREF            g_crMenuBGColor;
TCHAR               g_ptStrings[1024];
int                 g_iMenuItemToSelect;
long                g_lPicSizeReduced;
TCHAR               g_ptSaveFileName[10] = _T("DATA0.DAT");
long                g_iSavedPosInScript;
long                g_bReserved0001;
bool                g_bStartingMenu;
int                 g_iYIdx;
int                 g_iCurrentMidiFile;
bool                g_bMidiPlaying;
COLORREF            g_crColor;
MyRect              g_sCGRects[30];
long                g_lIdx2;
short               g_sSaveToLoad;
int                 g_iSleepTimeBetweenChars/* = 50*/;
bool                g_bVoiceWavePlaying;
unsigned short      g_usiSleepTime;
int                 g_iMenuItemsPosInScript[14];
TCHAR               g_ptPersonageOnScr2File[MAX_PATH];
SYSTEMTIME          g_sSystemTime;
char                g_pcCvtString[CVT_BUF_SIZE];
TCHAR               g_ptCvtString[CVT_BUF_SIZE];

HANDLE              g_hCreateDoneEvent;


int ConvertMBCSToUni(const char *cpcMBString, wchar_t *pwcUString, int iUStringSize)
{
    return MultiByteToWideChar(SHIFT_JIS_CP, 0, cpcMBString, -1, pwcUString, iUStringSize);
}

int ConvertUniToMBCS(const wchar_t *cpwUString, char *pcMBString, int iMBStringSize)
{
    return WideCharToMultiByte(SHIFT_JIS_CP, 0, cpwUString, -1, pcMBString, iMBStringSize, NULL, NULL);
}

ATOM RegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX sWCEX;

    sWCEX.cbSize = sizeof(WNDCLASSEX);

    sWCEX.style = CS_BYTEALIGNCLIENT | CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
    sWCEX.lpfnWndProc = WndProc;
    sWCEX.cbClsExtra = 0;
    sWCEX.cbWndExtra = 0;
    sWCEX.hInstance = hInstance;
    sWCEX.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DD));
    sWCEX.hCursor = LoadCursor(NULL, IDC_ARROW);
    sWCEX.hbrBackground = NULL;
    sWCEX.lpszMenuName = NULL;
    sWCEX.lpszClassName = g_cptClassName;
    sWCEX.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DD));

    return RegisterClassEx(&sWCEX);
}

//----- (00401000) --------------------------------------------------------
// return: true - success; false - fail
bool LoadSGArhiveTable(TCHAR *ptFileName)
{
    HANDLE  hFile;
    DWORD   dwNumberOfBytesRead;

    if (!ptFileName){
        return false;
    }

    _tcscpy_s(g_pcSGFileName, ptFileName);
    hFile = CreateFile(g_pcSGFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        return false;
    }

    ReadFile(hFile, &g_pcSGFileHdr, sizeof(ArhiveHdr), &dwNumberOfBytesRead, NULL);
    if (dwNumberOfBytesRead != sizeof(ArhiveHdr) || g_pcSGFileHdr.bSign1 != 'D' || g_pcSGFileHdr.bSign2 != 'L' || g_pcSGFileHdr.bSign3 != '1'){
        CloseHandle(hFile);
        return false;
    }

    g_pcSGArhiveFileTable = (ArhiveFileEntry *)malloc((g_pcSGFileHdr.wEntryCount + 1) * sizeof(ArhiveFileEntry));
    if (!g_pcSGArhiveFileTable){
        CloseHandle(hFile);
        return false;
    }

    SetFilePointer(hFile, g_pcSGFileHdr.dwFileTableOffset, 0, FILE_BEGIN);
    ReadFile(hFile, g_pcSGArhiveFileTable, g_pcSGFileHdr.wEntryCount * sizeof(ArhiveFileEntry), &dwNumberOfBytesRead, NULL);
    CloseHandle(hFile);
    if (g_pcSGFileHdr.wEntryCount * sizeof(ArhiveFileEntry) != dwNumberOfBytesRead){
        return false;
    }

    return g_pcSGFileHdr.wEntryCount != 0;
}

//----- (00401110) --------------------------------------------------------
void FreeSGArhiveFileTable()
{
    if (g_pcSGArhiveFileTable){
        free(g_pcSGArhiveFileTable);
    }
}

//----- (00401140) --------------------------------------------------------
// return: true - success; false - fail
bool FindAndReadFileInSG(const TCHAR *cptFileName, LPVOID lpBuffer)
{
    char    pcEntryNameToFind[MAX_ARCH_NAME_SIZE];
    DWORD   dwIdx;
    DWORD   dwOffset;
    DWORD   dwNumberOfBytesRead;
    HANDLE  hFile;

    if (!cptFileName || !g_pcSGArhiveFileTable){
        return false;
    }

    memset(pcEntryNameToFind, 0, MAX_ARCH_NAME_SIZE);
    ConvertUniToMBCS(cptFileName, pcEntryNameToFind, MAX_ARCH_NAME_SIZE);

    dwOffset = sizeof(ArhiveHdr);
    dwNumberOfBytesRead = 0;

    for (dwIdx = 0; dwIdx < g_pcSGFileHdr.wEntryCount; dwIdx++){
        if (strncmp(g_pcSGArhiveFileTable[dwIdx].pcFileName, pcEntryNameToFind, MAX_ARCH_NAME_SIZE) == 0){
            break;
        }
        dwOffset += g_pcSGArhiveFileTable[dwIdx].dwFileSize;
    }

    if (dwIdx >= g_pcSGFileHdr.wEntryCount){
        return false;
    }

    hFile = CreateFile(g_pcSGFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        return false;
    }

    SetFilePointer(hFile, dwOffset, 0, FILE_BEGIN);
    ReadFile(hFile, lpBuffer, g_pcSGArhiveFileTable[dwIdx].dwFileSize, &dwNumberOfBytesRead, NULL);
    CloseHandle(hFile);

    return g_pcSGArhiveFileTable[dwIdx].dwFileSize == dwNumberOfBytesRead;
}

//----- (00401280) --------------------------------------------------------
// Read file into specified buffer and return a number of bytes read
DWORD ReadFileIntoBuffer(const TCHAR* lpFileName, LPVOID lpBuffer)
{
    HANDLE  hFile;
    DWORD   dwNumberOfBytesRead;

    hFile = CreateFile(lpFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        return 0;
    }

    ReadFile(hFile, lpBuffer, 0x100000u, &dwNumberOfBytesRead, NULL);   // 1 megabyte
    CloseHandle(hFile);

    return dwNumberOfBytesRead;
}

//----- (004012E0) --------------------------------------------------------
// Write file, replace existing and return a number of bytes written
DWORD OverwriteFile(const TCHAR *cptFileName, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite)
{
    HANDLE  hFile;
    DWORD   dwNumberOfBytesWritten;

    hFile = CreateFile(cptFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        return 0;
    }

    WriteFile(hFile, lpBuffer, dwNumberOfBytesToWrite, &dwNumberOfBytesWritten, 0);
    CloseHandle(hFile);

    return dwNumberOfBytesWritten;
}

//----- (00401340) --------------------------------------------------------
// Write file and return a number of bytes written
DWORD WriteToFile(const TCHAR *cptFileName, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
{
    HANDLE  hFile;
    DWORD   dwNumberOfBytesWritten;

    hFile = CreateFile(cptFileName, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        return 0;
    }

    WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, &dwNumberOfBytesWritten, 0);
    CloseHandle(hFile);

    return dwNumberOfBytesWritten;
}

//----- (004013A0) --------------------------------------------------------
void InitPalette()
{
    int         iIdx;
    HDC         hDc;
    HPALETTE    hPal;
    HPALETTE    hOldPal;
    LOGPALETTE  *psPal;

    psPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * 255);

    psPal->palVersion = 0x300;
    psPal->palNumEntries = 256;
    memset(&psPal->palPalEntry, 0, 256 * 4);

    for (iIdx = 0; iIdx < 256; iIdx++){
        psPal->palPalEntry[iIdx].peRed = 0;
        psPal->palPalEntry[iIdx].peGreen = 0;
        psPal->palPalEntry[iIdx].peBlue = 0;
        psPal->palPalEntry[iIdx].peFlags = PC_NOCOLLAPSE;
    }

    hDc = GetDC(NULL);
    hPal = CreatePalette(psPal);

    if (hPal){
        hOldPal = SelectPalette(hDc, hPal, FALSE);
        RealizePalette(hDc);
        SelectPalette(hDc, hOldPal, FALSE);
        DeleteObject(hPal);
    }

    free(psPal);

    ReleaseDC(NULL, hDc);
}

//----- (00401440) --------------------------------------------------------
void SetWindowSize(HWND hWnd, int iWidth, int iHeight)
{
    POINT   sPoint;
    RECT    sRect;

    sPoint.x = 0;
    sPoint.y = 0;
    ClientToScreen(hWnd, &sPoint);
    GetWindowRect(hWnd, &sRect);
    MoveWindow(hWnd, sRect.left, sRect.top, iWidth + 2 * (sPoint.x - sRect.left), iHeight + sPoint.x - sRect.left + sPoint.y - sRect.top, FALSE);
}

//----- (004014B0) --------------------------------------------------------
void CenterTheWindow(HWND hWnd)
{
    RECT    sRect;
    int     iSrceenSizeX;
    int     iSrceenSizeY;

    iSrceenSizeX = GetSystemMetrics(SM_CXSCREEN);
    iSrceenSizeY = GetSystemMetrics(SM_CYSCREEN);

    if (iSrceenSizeX > 640 || iSrceenSizeY > 480){
        GetWindowRect(hWnd, &sRect);
        sRect.left = (iSrceenSizeX - (sRect.right - sRect.left)) / 2;
        sRect.top = (iSrceenSizeY - (sRect.bottom - sRect.top)) / 2;
        SetWindowPos(hWnd, HWND_TOP, sRect.left, sRect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
    }
}

//----- (00401550) --------------------------------------------------------
void SetupOrigin(HWND hWnd, HDC hdc)
{
    POINT   sPoint;
    RECT    sRect;

    sPoint.x = 0;
    sPoint.y = 0;

    ClientToScreen(hWnd, &sPoint);
    GetWindowRect(hWnd, &sRect);

    SetViewportOrgEx(hdc, sPoint.x - sRect.left, sPoint.y - sRect.top, NULL);

    if (GetWindowLong(hWnd, GWL_STYLE) & WS_POPUP){
        SetWindowOrgEx(hdc, sPoint.x - sRect.left, sPoint.y - sRect.top, NULL);
    }
    else{
        SetWindowOrgEx(hdc, 0, 0, NULL);
    }
}

//----- (004015F0) --------------------------------------------------------
// return: true - success; false - fail
bool UnpackData(void *pvDstBuf, void *pvSrcBuf, DWORD ulReceivingBufSize)
{
    PackedFileHdr   sPackedFileHdr;
    BYTE            pbSlideWinBuf[UNPACK_WIN_SIZE];
    WORD            wOffsetInWin;
    WORD            wReadOffsetInWin;
    DWORD           dwBytesUnpacked;
    BYTE            *pbSrcBufPos;
    BYTE            *pbDstBufPos;
    BYTE            bIdx;
    BYTE            bFlagByte;
    BYTE            bRepeatCount;

    if (pvSrcBuf == NULL || pvDstBuf == NULL){
        return false;
    }

    memcpy(&sPackedFileHdr, pvSrcBuf, sizeof(PackedFileHdr));

    if (sPackedFileHdr.bSign1 != 'L' || sPackedFileHdr.bSign2 != 'Z' || ulReceivingBufSize < sPackedFileHdr.dwUnpackedSize){
        return false;
    }

    memset(pbSlideWinBuf, 0, UNPACK_WIN_SIZE);

    pbSrcBufPos = (BYTE*)pvSrcBuf + sizeof(PackedFileHdr);
    pbDstBufPos = (BYTE*)pvDstBuf;
    dwBytesUnpacked = 0;
    wOffsetInWin = UNPACK_WIN_START_OFFSET;

    do{
        bFlagByte = pbSrcBufPos[0];
        pbSrcBufPos++;

        for (bIdx = 0; bIdx < 8; bIdx++){
            if (bFlagByte & 1){
                pbDstBufPos[0] = GET_BYTE(pbSrcBufPos, 0);
                pbSlideWinBuf[wOffsetInWin] = GET_BYTE(pbSrcBufPos, 0);
                pbSrcBufPos++;
                pbDstBufPos++;
                dwBytesUnpacked++;
                wOffsetInWin = (wOffsetInWin + 1) & UNPACK_WIN_MASK;
            }
            else{
                wReadOffsetInWin = GET_WORD(pbSrcBufPos, 0);
                pbSrcBufPos += 2;

                bRepeatCount = ((wReadOffsetInWin >> 8) & 0x0F) + 3;
                wReadOffsetInWin = ((wReadOffsetInWin >> 4) & 0x0F00) | (wReadOffsetInWin & 0x00FF);

                for (; bRepeatCount > 0; bRepeatCount--){
                    pbDstBufPos[0] = pbSlideWinBuf[wReadOffsetInWin];
                    pbSlideWinBuf[wOffsetInWin] = pbSlideWinBuf[wReadOffsetInWin];
                    pbDstBufPos++;
                    dwBytesUnpacked++;
                    wReadOffsetInWin = (wReadOffsetInWin + 1) & UNPACK_WIN_MASK;
                    wOffsetInWin = (wOffsetInWin + 1) & UNPACK_WIN_MASK;
                }
            }
            bFlagByte >>= 1;
        }
    } while (dwBytesUnpacked < sPackedFileHdr.dwUnpackedSize);

    return true;
}

//----- (004016C0) --------------------------------------------------------
// return: true - success; false - fail
bool LoadWVArhiveTable(TCHAR *ptFileName)
{
    HANDLE  hFile;
    DWORD   dwNumberOfBytesRead;

    if (!ptFileName){
        return false;
    }

    _tcscpy_s(g_pcWVFileName, ptFileName);
    hFile = CreateFile(g_pcWVFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        return false;
    }

    ReadFile(hFile, &g_pcWVFileHdr, sizeof(ArhiveHdr), &dwNumberOfBytesRead, NULL);
    if (dwNumberOfBytesRead != sizeof(ArhiveHdr) || g_pcWVFileHdr.bSign1 != 'D' || g_pcWVFileHdr.bSign2 != 'L' || g_pcWVFileHdr.bSign3 != '1'){
        CloseHandle(hFile);
        return false;
    }

    g_pcWVArhiveFileTable = (ArhiveFileEntry *)malloc((g_pcWVFileHdr.wEntryCount + 1) * sizeof(ArhiveFileEntry));
    if (!g_pcWVArhiveFileTable){
        CloseHandle(hFile);
        return false;
    }

    SetFilePointer(hFile, g_pcWVFileHdr.dwFileTableOffset, 0, FILE_BEGIN);
    ReadFile(hFile, g_pcWVArhiveFileTable, g_pcWVFileHdr.wEntryCount * sizeof(ArhiveFileEntry), &dwNumberOfBytesRead, NULL);
    CloseHandle(hFile);
    if (g_pcWVFileHdr.wEntryCount * sizeof(ArhiveFileEntry) != dwNumberOfBytesRead){
        return false;
    }

    return g_pcWVFileHdr.wEntryCount != 0;
}

//----- (004017D0) --------------------------------------------------------
void FreeWVArhiveFileTable()
{
    if (g_pcWVArhiveFileTable){
        free(g_pcWVArhiveFileTable);
    }
}

//----- (00401800) --------------------------------------------------------
// return: true - success; false - fail
bool LoadWVFile(const TCHAR *cptFileToFind, LPVOID lpBuffer)
{
    char    pcEntryNameToFind[MAX_ARCH_NAME_SIZE];
    DWORD   dwIdx;
    DWORD   dwOffset;
    DWORD   dwNumberOfBytesRead;
    HANDLE  hFile;

    if (!cptFileToFind || !g_pcWVArhiveFileTable){
        return false;
    }

    memset(pcEntryNameToFind, 0, MAX_ARCH_NAME_SIZE);
    ConvertUniToMBCS(cptFileToFind, pcEntryNameToFind, MAX_ARCH_NAME_SIZE);

    dwOffset = sizeof(ArhiveHdr);
    dwNumberOfBytesRead = 0;

    for (dwIdx = 0; dwIdx < g_pcWVFileHdr.wEntryCount; dwIdx++){
        if (strncmp(g_pcWVArhiveFileTable[dwIdx].pcFileName, pcEntryNameToFind, MAX_ARCH_NAME_SIZE) == 0){
            break;
        }
        dwOffset += g_pcWVArhiveFileTable[dwIdx].dwFileSize;
    }

    if (dwIdx >= g_pcWVFileHdr.wEntryCount){
        return false;
    }

    hFile = CreateFile(g_pcWVFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE){
        return false;
    }

    SetFilePointer(hFile, dwOffset, 0, FILE_BEGIN);
    ReadFile(hFile, lpBuffer, g_pcWVArhiveFileTable[dwIdx].dwFileSize, &dwNumberOfBytesRead, NULL);
    CloseHandle(hFile);

    return g_pcWVArhiveFileTable[dwIdx].dwFileSize == dwNumberOfBytesRead;
}

HANDLE  hLogFile1;
HANDLE  hLogFile2;

void GetMsgName(UINT uiMsg, TCHAR *ptName)
{
    switch (uiMsg) {
    case 0x0000:
        _tcscpy_s(ptName, 32, _T("WM_NULL"));
        break;

    case 0x0001:
        _tcscpy_s(ptName, 32, _T("WM_CREATE"));
        break;

    case 0x0002:
        _tcscpy_s(ptName, 32, _T("WM_DESTROY"));
        break;

    case 0x0003:
        _tcscpy_s(ptName, 32, _T("WM_MOVE"));
        break;

    case 0x0005:
        _tcscpy_s(ptName, 32, _T("WM_SIZE"));
        break;

    case 0x0006:
        _tcscpy_s(ptName, 32, _T("WM_ACTIVATE"));
        break;

    case 0x0007:
        _tcscpy_s(ptName, 32, _T("WM_SETFOCUS"));
        break;

    case 0x0008:
        _tcscpy_s(ptName, 32, _T("WM_KILLFOCUS"));
        break;

    case 0x000A:
        _tcscpy_s(ptName, 32, _T("WM_ENABLE"));
        break;

    case 0x000B:
        _tcscpy_s(ptName, 32, _T("WM_SETREDRAW"));
        break;

    case 0x000C:
        _tcscpy_s(ptName, 32, _T("WM_SETTEXT"));
        break;

    case 0x000D:
        _tcscpy_s(ptName, 32, _T("WM_GETTEXT"));
        break;

    case 0x000E:
        _tcscpy_s(ptName, 32, _T("WM_GETTEXTLENGTH"));
        break;

    case 0x000F:
        _tcscpy_s(ptName, 32, _T("WM_PAINT"));
        break;

    case 0x0010:
        _tcscpy_s(ptName, 32, _T("WM_CLOSE"));
        break;

    case 0x0011:
        _tcscpy_s(ptName, 32, _T("WM_QUERYENDSESSION"));
        break;

    case 0x0012:
        _tcscpy_s(ptName, 32, _T("WM_QUIT"));
        break;

    case 0x0013:
        _tcscpy_s(ptName, 32, _T("WM_QUERYOPEN"));
        break;

    case 0x0014:
        _tcscpy_s(ptName, 32, _T("WM_ERASEBKGND"));
        break;

    case 0x0015:
        _tcscpy_s(ptName, 32, _T("WM_SYSCOLORCHANGE"));
        break;

    case 0x0016:
        _tcscpy_s(ptName, 32, _T("WM_ENDSESSION"));
        break;

    case 0x0018:
        _tcscpy_s(ptName, 32, _T("WM_SHOWWINDOW"));
        break;

    case 0x001A:
        _tcscpy_s(ptName, 32, _T("WM_SETTINGCHANGE"));
        break;

    case 0x001B:
        _tcscpy_s(ptName, 32, _T("WM_DEVMODECHANGE"));
        break;

    case 0x001C:
        _tcscpy_s(ptName, 32, _T("WM_ACTIVATEAPP"));
        break;

    case 0x001D:
        _tcscpy_s(ptName, 32, _T("WM_FONTCHANGE"));
        break;

    case 0x001E:
        _tcscpy_s(ptName, 32, _T("WM_TIMECHANGE"));
        break;

    case 0x001F:
        _tcscpy_s(ptName, 32, _T("WM_CANCELMODE"));
        break;

    case 0x0020:
        _tcscpy_s(ptName, 32, _T("WM_SETCURSOR"));
        break;

    case 0x0021:
        _tcscpy_s(ptName, 32, _T("WM_MOUSEACTIVATE"));
        break;

    case 0x0022:
        _tcscpy_s(ptName, 32, _T("WM_CHILDACTIVATE"));
        break;

    case 0x0023:
        _tcscpy_s(ptName, 32, _T("WM_QUEUESYNC"));
        break;

    case 0x0024:
        _tcscpy_s(ptName, 32, _T("WM_GETMINMAXINFO"));
        break;

    case 0x0026:
        _tcscpy_s(ptName, 32, _T("WM_PAINTICON"));
        break;

    case 0x0027:
        _tcscpy_s(ptName, 32, _T("WM_ICONERASEBKGND"));
        break;

    case 0x0028:
        _tcscpy_s(ptName, 32, _T("WM_NEXTDLGCTL"));
        break;

    case 0x002A:
        _tcscpy_s(ptName, 32, _T("WM_SPOOLERSTATUS"));
        break;

    case 0x002B:
        _tcscpy_s(ptName, 32, _T("WM_DRAWITEM"));
        break;

    case 0x002C:
        _tcscpy_s(ptName, 32, _T("WM_MEASUREITEM"));
        break;

    case 0x002D:
        _tcscpy_s(ptName, 32, _T("WM_DELETEITEM"));
        break;

    case 0x002E:
        _tcscpy_s(ptName, 32, _T("WM_VKEYTOITEM"));
        break;

    case 0x002F:
        _tcscpy_s(ptName, 32, _T("WM_CHARTOITEM"));
        break;

    case 0x0030:
        _tcscpy_s(ptName, 32, _T("WM_SETFONT"));
        break;

    case 0x0031:
        _tcscpy_s(ptName, 32, _T("WM_GETFONT"));
        break;

    case 0x0032:
        _tcscpy_s(ptName, 32, _T("WM_SETHOTKEY"));
        break;

    case 0x0033:
        _tcscpy_s(ptName, 32, _T("WM_GETHOTKEY"));
        break;

    case 0x0037:
        _tcscpy_s(ptName, 32, _T("WM_QUERYDRAGICON"));
        break;

    case 0x0039:
        _tcscpy_s(ptName, 32, _T("WM_COMPAREITEM"));
        break;

    case 0x003D:
        _tcscpy_s(ptName, 32, _T("WM_GETOBJECT"));
        break;

    case 0x0041:
        _tcscpy_s(ptName, 32, _T("WM_COMPACTING"));
        break;

    case 0x0044:
        _tcscpy_s(ptName, 32, _T("WM_COMMNOTIFY"));
        break;

    case 0x0046:
        _tcscpy_s(ptName, 32, _T("WM_WINDOWPOSCHANGING"));
        break;

    case 0x0047:
        _tcscpy_s(ptName, 32, _T("WM_WINDOWPOSCHANGED"));
        break;

    case 0x0048:
        _tcscpy_s(ptName, 32, _T("WM_POWER"));
        break;

    case 0x004A:
        _tcscpy_s(ptName, 32, _T("WM_COPYDATA"));
        break;

    case 0x004B:
        _tcscpy_s(ptName, 32, _T("WM_CANCELJOURNAL"));
        break;

    case 0x004E:
        _tcscpy_s(ptName, 32, _T("WM_NOTIFY"));
        break;

    case 0x0050:
        _tcscpy_s(ptName, 32, _T("WM_INPUTLANGCHANGEREQUEST"));
        break;

    case 0x0051:
        _tcscpy_s(ptName, 32, _T("WM_INPUTLANGCHANGE"));
        break;

    case 0x0052:
        _tcscpy_s(ptName, 32, _T("WM_TCARD"));
        break;

    case 0x0053:
        _tcscpy_s(ptName, 32, _T("WM_HELP"));
        break;

    case 0x0054:
        _tcscpy_s(ptName, 32, _T("WM_USERCHANGED"));
        break;

    case 0x0055:
        _tcscpy_s(ptName, 32, _T("WM_NOTIFYFORMAT"));
        break;

    case 0x007B:
        _tcscpy_s(ptName, 32, _T("WM_CONTEXTMENU"));
        break;

    case 0x007C:
        _tcscpy_s(ptName, 32, _T("WM_STYLECHANGING"));
        break;

    case 0x007D:
        _tcscpy_s(ptName, 32, _T("WM_STYLECHANGED"));
        break;

    case 0x007E:
        _tcscpy_s(ptName, 32, _T("WM_DISPLAYCHANGE"));
        break;

    case 0x007F:
        _tcscpy_s(ptName, 32, _T("WM_GETICON"));
        break;

    case 0x0080:
        _tcscpy_s(ptName, 32, _T("WM_SETICON"));
        break;

    case 0x0081:
        _tcscpy_s(ptName, 32, _T("WM_NCCREATE"));
        break;

    case 0x0082:
        _tcscpy_s(ptName, 32, _T("WM_NCDESTROY"));
        break;

    case 0x0083:
        _tcscpy_s(ptName, 32, _T("WM_NCCALCSIZE"));
        break;

    case 0x0084:
        _tcscpy_s(ptName, 32, _T("WM_NCHITTEST"));
        break;

    case 0x0085:
        _tcscpy_s(ptName, 32, _T("WM_NCPAINT"));
        break;

    case 0x0086:
        _tcscpy_s(ptName, 32, _T("WM_NCACTIVATE"));
        break;

    case 0x0087:
        _tcscpy_s(ptName, 32, _T("WM_GETDLGCODE"));
        break;

    case 0x0088:
        _tcscpy_s(ptName, 32, _T("WM_SYNCPAINT"));
        break;

    case 0x00A0:
        _tcscpy_s(ptName, 32, _T("WM_NCMOUSEMOVE"));
        break;

    case 0x00A1:
        _tcscpy_s(ptName, 32, _T("WM_NCLBUTTONDOWN"));
        break;

    case 0x00A2:
        _tcscpy_s(ptName, 32, _T("WM_NCLBUTTONUP"));
        break;

    case 0x00A3:
        _tcscpy_s(ptName, 32, _T("WM_NCLBUTTONDBLCLK"));
        break;

    case 0x00A4:
        _tcscpy_s(ptName, 32, _T("WM_NCRBUTTONDOWN"));
        break;

    case 0x00A5:
        _tcscpy_s(ptName, 32, _T("WM_NCRBUTTONUP"));
        break;

    case 0x00A6:
        _tcscpy_s(ptName, 32, _T("WM_NCRBUTTONDBLCLK"));
        break;

    case 0x00A7:
        _tcscpy_s(ptName, 32, _T("WM_NCMBUTTONDOWN"));
        break;

    case 0x00A8:
        _tcscpy_s(ptName, 32, _T("WM_NCMBUTTONUP"));
        break;

    case 0x00A9:
        _tcscpy_s(ptName, 32, _T("WM_NCMBUTTONDBLCLK"));
        break;

    case 0x00AB:
        _tcscpy_s(ptName, 32, _T("WM_NCXBUTTONDOWN"));
        break;

    case 0x00AC:
        _tcscpy_s(ptName, 32, _T("WM_NCXBUTTONUP"));
        break;

    case 0x00AD:
        _tcscpy_s(ptName, 32, _T("WM_NCXBUTTONDBLCLK"));
        break;

    case 0x00FF:
        _tcscpy_s(ptName, 32, _T("WM_INPUT"));
        break;

    case 0x0100:
        _tcscpy_s(ptName, 32, _T("WM_KEYDOWN"));
        break;

    case 0x0101:
        _tcscpy_s(ptName, 32, _T("WM_KEYUP"));
        break;

    case 0x0102:
        _tcscpy_s(ptName, 32, _T("WM_CHAR"));
        break;

    case 0x0103:
        _tcscpy_s(ptName, 32, _T("WM_DEADCHAR"));
        break;

    case 0x0104:
        _tcscpy_s(ptName, 32, _T("WM_SYSKEYDOWN"));
        break;

    case 0x0105:
        _tcscpy_s(ptName, 32, _T("WM_SYSKEYUP"));
        break;

    case 0x0106:
        _tcscpy_s(ptName, 32, _T("WM_SYSCHAR"));
        break;

    case 0x0107:
        _tcscpy_s(ptName, 32, _T("WM_SYSDEADCHAR"));
        break;

    case 0x0108:
        _tcscpy_s(ptName, 32, _T("WM_KEYLAST"));
        break;

    case 0x0109:
        _tcscpy_s(ptName, 32, _T("WM_UNICHAR"));
        break;

    case 0x010D:
        _tcscpy_s(ptName, 32, _T("WM_IME_STARTCOMPOSITION"));
        break;

    case 0x010E:
        _tcscpy_s(ptName, 32, _T("WM_IME_ENDCOMPOSITION"));
        break;

    case 0x010F:
        _tcscpy_s(ptName, 32, _T("WM_IME_COMPOSITION"));
        break;

    case 0x0110:
        _tcscpy_s(ptName, 32, _T("WM_INITDIALOG"));
        break;

    case 0x0111:
        _tcscpy_s(ptName, 32, _T("WM_COMMAND"));
        break;

    case 0x0112:
        _tcscpy_s(ptName, 32, _T("WM_SYSCOMMAND"));
        break;

    case 0x0113:
        _tcscpy_s(ptName, 32, _T("WM_TIMER"));
        break;

    case 0x0114:
        _tcscpy_s(ptName, 32, _T("WM_HSCROLL"));
        break;

    case 0x0115:
        _tcscpy_s(ptName, 32, _T("WM_VSCROLL"));
        break;

    case 0x0116:
        _tcscpy_s(ptName, 32, _T("WM_INITMENU"));
        break;

    case 0x0117:
        _tcscpy_s(ptName, 32, _T("WM_INITMENUPOPUP"));
        break;

    case 0x011F:
        _tcscpy_s(ptName, 32, _T("WM_MENUSELECT"));
        break;

    case 0x0120:
        _tcscpy_s(ptName, 32, _T("WM_MENUCHAR"));
        break;

    case 0x0121:
        _tcscpy_s(ptName, 32, _T("WM_ENTERIDLE"));
        break;

    case 0x0122:
        _tcscpy_s(ptName, 32, _T("WM_MENURBUTTONUP"));
        break;

    case 0x0123:
        _tcscpy_s(ptName, 32, _T("WM_MENUDRAG"));
        break;

    case 0x0124:
        _tcscpy_s(ptName, 32, _T("WM_MENUGETOBJECT"));
        break;

    case 0x0125:
        _tcscpy_s(ptName, 32, _T("WM_UNINITMENUPOPUP"));
        break;

    case 0x0126:
        _tcscpy_s(ptName, 32, _T("WM_MENUCOMMAND"));
        break;

    case 0x0127:
        _tcscpy_s(ptName, 32, _T("WM_CHANGEUISTATE"));
        break;

    case 0x0128:
        _tcscpy_s(ptName, 32, _T("WM_UPDATEUISTATE"));
        break;

    case 0x0129:
        _tcscpy_s(ptName, 32, _T("WM_QUERYUISTATE"));
        break;

    case 0x0132:
        _tcscpy_s(ptName, 32, _T("WM_CTLCOLORMSGBOX"));
        break;

    case 0x0133:
        _tcscpy_s(ptName, 32, _T("WM_CTLCOLOREDIT"));
        break;

    case 0x0134:
        _tcscpy_s(ptName, 32, _T("WM_CTLCOLORLISTBOX"));
        break;

    case 0x0135:
        _tcscpy_s(ptName, 32, _T("WM_CTLCOLORBTN"));
        break;

    case 0x0136:
        _tcscpy_s(ptName, 32, _T("WM_CTLCOLORDLG"));
        break;

    case 0x0137:
        _tcscpy_s(ptName, 32, _T("WM_CTLCOLORSCROLLBAR"));
        break;

    case 0x0138:
        _tcscpy_s(ptName, 32, _T("WM_CTLCOLORSTATIC"));
        break;

    case 0x0200:
        _tcscpy_s(ptName, 32, _T("WM_MOUSEMOVE"));
        break;

    case 0x0201:
        _tcscpy_s(ptName, 32, _T("WM_LBUTTONDOWN"));
        break;

    case 0x0202:
        _tcscpy_s(ptName, 32, _T("WM_LBUTTONUP"));
        break;

    case 0x0203:
        _tcscpy_s(ptName, 32, _T("WM_LBUTTONDBLCLK"));
        break;

    case 0x0204:
        _tcscpy_s(ptName, 32, _T("WM_RBUTTONDOWN"));
        break;

    case 0x0205:
        _tcscpy_s(ptName, 32, _T("WM_RBUTTONUP"));
        break;

    case 0x0206:
        _tcscpy_s(ptName, 32, _T("WM_RBUTTONDBLCLK"));
        break;

    case 0x0207:
        _tcscpy_s(ptName, 32, _T("WM_MBUTTONDOWN"));
        break;

    case 0x0208:
        _tcscpy_s(ptName, 32, _T("WM_MBUTTONUP"));
        break;

    case 0x0209:
        _tcscpy_s(ptName, 32, _T("WM_MBUTTONDBLCLK"));
        break;

    case 0x020A:
        _tcscpy_s(ptName, 32, _T("WM_MOUSEWHEEL"));
        break;

    case 0x020B:
        _tcscpy_s(ptName, 32, _T("WM_XBUTTONDOWN"));
        break;

    case 0x020C:
        _tcscpy_s(ptName, 32, _T("WM_XBUTTONUP"));
        break;

    case 0x020D:
        _tcscpy_s(ptName, 32, _T("WM_XBUTTONDBLCLK"));
        break;

    case 0x0210:
        _tcscpy_s(ptName, 32, _T("WM_PARENTNOTIFY"));
        break;

    case 0x0211:
        _tcscpy_s(ptName, 32, _T("WM_ENTERMENULOOP"));
        break;

    case 0x0212:
        _tcscpy_s(ptName, 32, _T("WM_EXITMENULOOP"));
        break;

    case 0x0213:
        _tcscpy_s(ptName, 32, _T("WM_NEXTMENU"));
        break;

    case 0x0214:
        _tcscpy_s(ptName, 32, _T("WM_SIZING"));
        break;

    case 0x0215:
        _tcscpy_s(ptName, 32, _T("WM_CAPTURECHANGED"));
        break;

    case 0x0216:
        _tcscpy_s(ptName, 32, _T("WM_MOVING"));
        break;

    case 0x0218:
        _tcscpy_s(ptName, 32, _T("WM_POWERBROADCAST"));
        break;

    case 0x0219:
        _tcscpy_s(ptName, 32, _T("WM_DEVICECHANGE"));
        break;

    case 0x0220:
        _tcscpy_s(ptName, 32, _T("WM_MDICREATE"));
        break;

    case 0x0221:
        _tcscpy_s(ptName, 32, _T("WM_MDIDESTROY"));
        break;

    case 0x0222:
        _tcscpy_s(ptName, 32, _T("WM_MDIACTIVATE"));
        break;

    case 0x0223:
        _tcscpy_s(ptName, 32, _T("WM_MDIRESTORE"));
        break;

    case 0x0224:
        _tcscpy_s(ptName, 32, _T("WM_MDINEXT"));
        break;

    case 0x0225:
        _tcscpy_s(ptName, 32, _T("WM_MDIMAXIMIZE"));
        break;

    case 0x0226:
        _tcscpy_s(ptName, 32, _T("WM_MDITILE"));
        break;

    case 0x0227:
        _tcscpy_s(ptName, 32, _T("WM_MDICASCADE"));
        break;

    case 0x0228:
        _tcscpy_s(ptName, 32, _T("WM_MDIICONARRANGE"));
        break;

    case 0x0229:
        _tcscpy_s(ptName, 32, _T("WM_MDIGETACTIVE"));
        break;

    case 0x0230:
        _tcscpy_s(ptName, 32, _T("WM_MDISETMENU"));
        break;

    case 0x0231:
        _tcscpy_s(ptName, 32, _T("WM_ENTERSIZEMOVE"));
        break;

    case 0x0232:
        _tcscpy_s(ptName, 32, _T("WM_EXITSIZEMOVE"));
        break;

    case 0x0233:
        _tcscpy_s(ptName, 32, _T("WM_DROPFILES"));
        break;

    case 0x0234:
        _tcscpy_s(ptName, 32, _T("WM_MDIREFRESHMENU"));
        break;

    case 0x0281:
        _tcscpy_s(ptName, 32, _T("WM_IME_SETCONTEXT"));
        break;

    case 0x0282:
        _tcscpy_s(ptName, 32, _T("WM_IME_NOTIFY"));
        break;

    case 0x0283:
        _tcscpy_s(ptName, 32, _T("WM_IME_CONTROL"));
        break;

    case 0x0284:
        _tcscpy_s(ptName, 32, _T("WM_IME_COMPOSITIONFULL"));
        break;

    case 0x0285:
        _tcscpy_s(ptName, 32, _T("WM_IME_SELECT"));
        break;

    case 0x0286:
        _tcscpy_s(ptName, 32, _T("WM_IME_CHAR"));
        break;

    case 0x0288:
        _tcscpy_s(ptName, 32, _T("WM_IME_REQUEST"));
        break;

    case 0x0290:
        _tcscpy_s(ptName, 32, _T("WM_IME_KEYDOWN"));
        break;

    case 0x0291:
        _tcscpy_s(ptName, 32, _T("WM_IME_KEYUP"));
        break;

    case 0x02A0:
        _tcscpy_s(ptName, 32, _T("WM_NCMOUSEHOVER"));
        break;

    case 0x02A1:
        _tcscpy_s(ptName, 32, _T("WM_MOUSEHOVER"));
        break;

    case 0x02A2:
        _tcscpy_s(ptName, 32, _T("WM_NCMOUSELEAVE"));
        break;

    case 0x02A3:
        _tcscpy_s(ptName, 32, _T("WM_MOUSELEAVE"));
        break;

    case 0x02B1:
        _tcscpy_s(ptName, 32, _T("WM_WTSSESSION_CHANGE"));
        break;

    case 0x02C0:
        _tcscpy_s(ptName, 32, _T("WM_TABLET_FIRST"));
        break;

    case 0x02DF:
        _tcscpy_s(ptName, 32, _T("WM_TABLET_LAST"));
        break;

    case 0x0300:
        _tcscpy_s(ptName, 32, _T("WM_CUT"));
        break;

    case 0x0301:
        _tcscpy_s(ptName, 32, _T("WM_COPY"));
        break;

    case 0x0302:
        _tcscpy_s(ptName, 32, _T("WM_PASTE"));
        break;

    case 0x0303:
        _tcscpy_s(ptName, 32, _T("WM_CLEAR"));
        break;

    case 0x0304:
        _tcscpy_s(ptName, 32, _T("WM_UNDO"));
        break;

    case 0x0305:
        _tcscpy_s(ptName, 32, _T("WM_RENDERFORMAT"));
        break;

    case 0x0306:
        _tcscpy_s(ptName, 32, _T("WM_RENDERALLFORMATS"));
        break;

    case 0x0307:
        _tcscpy_s(ptName, 32, _T("WM_DESTROYCLIPBOARD"));
        break;

    case 0x0308:
        _tcscpy_s(ptName, 32, _T("WM_DRAWCLIPBOARD"));
        break;

    case 0x0309:
        _tcscpy_s(ptName, 32, _T("WM_PAINTCLIPBOARD"));
        break;

    case 0x030A:
        _tcscpy_s(ptName, 32, _T("WM_VSCROLLCLIPBOARD"));
        break;

    case 0x030B:
        _tcscpy_s(ptName, 32, _T("WM_SIZECLIPBOARD"));
        break;

    case 0x030C:
        _tcscpy_s(ptName, 32, _T("WM_ASKCBFORMATNAME"));
        break;

    case 0x030D:
        _tcscpy_s(ptName, 32, _T("WM_CHANGECBCHAIN"));
        break;

    case 0x030E:
        _tcscpy_s(ptName, 32, _T("WM_HSCROLLCLIPBOARD"));
        break;

    case 0x030F:
        _tcscpy_s(ptName, 32, _T("WM_QUERYNEWPALETTE"));
        break;

    case 0x0310:
        _tcscpy_s(ptName, 32, _T("WM_PALETTEISCHANGING"));
        break;

    case 0x0311:
        _tcscpy_s(ptName, 32, _T("WM_PALETTECHANGED"));
        break;

    case 0x0312:
        _tcscpy_s(ptName, 32, _T("WM_HOTKEY"));
        break;

    case 0x0317:
        _tcscpy_s(ptName, 32, _T("WM_PRINT"));
        break;

    case 0x0318:
        _tcscpy_s(ptName, 32, _T("WM_PRINTCLIENT"));
        break;

    case 0x0319:
        _tcscpy_s(ptName, 32, _T("WM_APPCOMMAND"));
        break;

    case 0x031A:
        _tcscpy_s(ptName, 32, _T("WM_THEMECHANGED"));
        break;

    case 0x0358:
        _tcscpy_s(ptName, 32, _T("WM_HANDHELDFIRST"));
        break;

    case 0x035F:
        _tcscpy_s(ptName, 32, _T("WM_HANDHELDLAST"));
        break;

    case 0x0360:
        _tcscpy_s(ptName, 32, _T("WM_AFXFIRST"));
        break;

    case 0x037F:
        _tcscpy_s(ptName, 32, _T("WM_AFXLAST"));
        break;

    case 0x0380:
        _tcscpy_s(ptName, 32, _T("WM_PENWINFIRST"));
        break;

    case 0x038F:
        _tcscpy_s(ptName, 32, _T("WM_PENWINLAST"));
        break;

    default:
        if (uiMsg < WM_USER) {
            wsprintf(ptName, _T("SYS_MSG 0x%04X"), uiMsg);
        }
        if (uiMsg >= WM_USER && uiMsg < WM_APP) {
            wsprintf(ptName, _T("WM_USER + %ud"), uiMsg - WM_USER);
        }
        if (uiMsg >= WM_APP) {
            wsprintf(ptName, _T("WM_APP + %ud"), uiMsg - WM_APP);
        }

        break;
    }
}

//----- (00401960) --------------------------------------------------------
int WINAPI _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MSG     sMsg;
    HACCEL  hAccelTable;

    if (FindWindowEx(NULL, NULL, g_cptClassName, NULL)){
        return 0;
    }

    g_hCreateDoneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

#ifdef _DEBUG
    /* Message Logging */
    TCHAR   ptString[512];
    TCHAR   ptMsgName[32];
    DWORD   dwBytesWritten;

    hLogFile1 = CreateFile(_T("Msg1.log"), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hLogFile1 == INVALID_HANDLE_VALUE){
        return 0;
    }

    hLogFile2 = CreateFile(_T("Msg2.log"), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hLogFile2 == INVALID_HANDLE_VALUE){
        CloseHandle(hLogFile1);
        return 0;
    }
#endif

    if (hPrevInstance || RegisterClass(hInstance)){

#ifdef _DEBUG
        g_hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(_MENU11));
#else
        g_hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(_MENU1));
#endif

        g_iScreenX = GetSystemMetrics(SM_CXSCREEN);
        g_iScreenY = GetSystemMetrics(SM_CYSCREEN);
        if (g_iScreenX > 640 || g_iScreenY > 480){
            g_uiWindowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
            g_bWindowModeReal = true;
            g_bWindowModeSelected = true;
        }
        else{
            g_uiWindowStyle = WS_POPUP;
            g_bWindowModeReal = false;
            g_bWindowModeSelected = false;
        }

        g_hMainWindow = CreateWindowEx(WS_EX_LEFT, g_cptClassName, g_cptWindowName, g_uiWindowStyle,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
        if (!g_hMainWindow){
            return 0;
        }

        ShowWindow(g_hMainWindow, SW_SHOWNORMAL);
        UpdateWindow(g_hMainWindow);

        hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DD));

        SetEvent(g_hCreateDoneEvent);

        while (GetMessage(&sMsg, 0, 0, 0) > 0){
            TranslateMessage(&sMsg);

#ifdef _DEBUG
            GetMsgName(LOWORD(sMsg.message), ptMsgName);
            wsprintf(ptString, _T("Message time: %d; Message: %s; wParam: %d; lParam: %d\n"), sMsg.time, ptMsgName, sMsg.wParam, sMsg.lParam);
            WriteFile(hLogFile1, ptString, sizeof(TCHAR) * _tcslen(ptString), &dwBytesWritten, NULL);
#endif

            DispatchMessage(&sMsg);
        }

#ifdef _DEBUG
        CloseHandle(hLogFile2);
        CloseHandle(hLogFile1);
#endif
        CloseHandle(g_hCreateDoneEvent);

        return (int)sMsg.wParam;
    }

#ifdef _DEBUG
    CloseHandle(hLogFile2);
    CloseHandle(hLogFile1);
#endif
    CloseHandle(g_hCreateDoneEvent);

    return 0;
}

//----- (00401B80) --------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWND, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT sPaint;
    HBRUSH      hBlackBrush;
    RECT        sRect;
    HWND        hDesktopHWND;

#ifdef _DEBUG
    LARGE_INTEGER   liFreq;
    LARGE_INTEGER   liStartTime;
    LARGE_INTEGER   liEndTime;
    long double     ldTime;
    TCHAR           ptString[512];
    TCHAR           ptMsgName[32];
    DWORD           dwBytesWritten;

    QueryPerformanceFrequency(&liFreq);
    QueryPerformanceCounter(&liStartTime);
    GetMsgName(LOWORD(uiMsg), ptMsgName);
    ldTime = (long double)liStartTime.QuadPart * 1000.0 / (long double)liFreq.QuadPart;
    _stprintf_s(ptString, _T("Time: %lf[ms]; Message:   %s; wParam: %d; lParam: %d\n"), ldTime, ptMsgName, wParam, lParam);
    WriteFile(hLogFile2, ptString, sizeof(TCHAR) * _tcslen(ptString), &dwBytesWritten, NULL);
#endif

    switch (uiMsg){
    case WM_PAINT:
        BeginPaint(hWND, &sPaint);
        EndPaint(hWND, &sPaint);
        if (!g_bShowingFullScrMenu){
            BitBlt(g_hMainDc, 0, 0, g_iWindowSizeX, g_iWindowSizeY, g_hDcBuffer1, 0, 0, SRCCOPY);
            g_bScreenUpdatedOrMouseMoved = true;
            g_bResetMenuSelection = true;
        }
        break;

    case WM_CREATE:
        HDC hDc = GetDC(hWND);
        g_bPaletteSupported = (GetDeviceCaps(hDc, RASTERCAPS) & RC_PALETTE) != FALSE;
        g_iScreenDepth = GetDeviceCaps(hDc, BITSPIXEL);
        ReleaseDC(hWND, hDc);
        if (InitializeGame(hWND)){
            ChangeDisplaySettings(NULL, 0);
            DestroyWindow(hWND);
            break;
        }
        SetWindowSize(hWND, g_iWindowSizeX, g_iWindowSizeY);
        CenterTheWindow(hWND);
        ShowWindow(hWND, SW_SHOWNORMAL);
        UpdateWindow(hWND);
        if (g_iScreenDepth <= 8){
            MessageBox(hWND, _T("This game cannot run in 256 colors"), _T("DIVI-DEAD C'sWARE"), MB_OK);
            g_wKeyCommand = EMPTY_MASK;
            g_sPrevKeyCommand = EMPTY_MASK;
            ChangeDisplaySettings(NULL, 0);
            DestroyWindow(hWND);
            break;
        }
        //_tcsncpy_s(g_ptStartScriptFile, 64, _T("AASTART"), 64);
        g_iCurrPosInScript = 0;

        _tcscpy_s(g_ptFullFileName, g_ptWorkingDir);
        _tcsncat_s(g_ptFullFileName, _T("SG.DL1"), _TRUNCATE);
        if (!LoadSGArhiveTable(g_ptFullFileName)){
            MessageBox(hWND, _T("Cannot read data file"), _T("DIVI-DEAD C'sWARE"), MB_OK);
            ChangeDisplaySettings(NULL, 0);
            DestroyWindow(hWND);
            break;
        }

        _tcscpy_s(g_ptFullFileName, g_ptWorkingDir);
        _tcsncat_s(g_ptFullFileName, _T("WV.DL1"), _TRUNCATE);
        if (!LoadWVArhiveTable(g_ptFullFileName)){
            MessageBox(hWND, _T("Cannot read data file"), _T("DIVI-DEAD C'sWARE"), MB_OK);
            ChangeDisplaySettings(NULL, 0);
            DestroyWindow(hWND);
            break;
        }
        g_pcFLIST[0].pcFileName[0] = '/';
        g_pcFLIST[0].pcFileName[1] = 0;

        if (!FindAndReadFileInSG(_T("FLIST"), g_pvDataBuffer)){
            MessageBox(hWND, _T("Cannot read file list"), _T("DIVI-DEAD C'sWARE"), MB_OK);
        }
        memcpy(g_pcFLIST, g_pvDataBuffer, FLIST_ENTRY_COUNT * sizeof(ArhiveFileEntry));

        LoadAndBlitPicToBuf2(0, eMainScreen, _T("WAKU_P"));                             // Load menu images
        BitBlt(g_hDcMenuGfx, 0, 0, GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y, g_hDcBuffer2, 0, 0, SRCCOPY);
        SetRect(&sRect, 0, 0, GAME_SCREEN_SIZE_X, GAME_SCREEN_SIZE_Y);
        hBlackBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
        FillRect(g_hDcBuffer2, &sRect, hBlackBrush);                               // Paint main part of screen black

        g_iRequiredAction = eShowIntro;

        g_hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainGameThreadProc, hWND, 0, &g_dwThreadId);
        if (!g_hThread){
            MessageBox(hWND, _T("Cannot create a thread"), _T("DIVI-DEAD C'sWARE"), MB_OK);
            ChangeDisplaySettings(NULL, 0);
            DestroyWindow(hWND);
            break;
        }

        break;

    case WM_DESTROY:
        FreeSGArhiveFileTable();
        FreeWVArhiveFileTable();
        WinHelp(hWND, _T("DIVIDEAD.HLP"), HH_DISPLAY_INDEX, 0);
        if (g_hThread){
            TerminateThread(g_hThread, 0);
        }
        ShutdownGame();
        g_bWindowModeSelected = false;                      // Force next call to switch to window mode
        SendMessage(hWND, WM_SWITCH_SCREEN_MODE, 0, 0);     // Switch display mode
        Sleep(100u);
        PostQuitMessage(0);
        break;

    case WM_ACTIVATE:
        if (wParam != WA_INACTIVE){
            if (!g_bWindowModeReal){
                g_bShowingFullScrMenu = false;
                SetMenu(hWND, NULL);
                SetupOrigin(hWND, g_hMainDc);
            }
            g_bCtrlPressed = 0;
            g_bShiftPressed = 0;
            g_bKeyCommand2 = 0;
            break;
        }
        if (wParam > WA_INACTIVE && wParam <= WA_CLICKACTIVE){  // Window is active
            GetClientRect(hWND, &sRect);
            InvalidateRect(hWND, &sRect, FALSE);
            if (g_bVideoIsPlaying){
                SetCapture(hWND);
            }
        }
        break;

    case WM_SIZE:
        GetClientRect(hWND, &sRect);
        InvalidateRect(hWND, &sRect, FALSE);
        break;

    case WM_DISPLAYCHANGE:
        if (!g_bDisplayModeChanged){
            g_iScreenX = GetSystemMetrics(SM_CXSCREEN);
            g_iScreenY = GetSystemMetrics(SM_CYSCREEN);
            if (g_iScreenX > GAME_SCREEN_SIZE_X || g_iScreenY > GAME_SCREEN_SIZE_Y){
                g_bWindowModeSelected = true;
            }
            else{
                g_bWindowModeSelected = false;
            }
        }
        g_bDisplayModeChanged = false;
        if (g_bWindowModeSelected){
            g_bShowingFullScrMenu = false;
            g_bWindowModeReal = true;
            g_uiWindowStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
            SetWindowLong(hWND, GWL_STYLE, g_uiWindowStyle);
            SetMenu(hWND, g_hMenu);
            SetupOrigin(hWND, g_hMainDc);
            SetWindowSize(hWND, g_iWindowSizeX, g_iWindowSizeY);
            CenterTheWindow(hWND);
            ShowWindow(hWND, SW_SHOWNORMAL);
            UpdateWindow(hWND);
            hDesktopHWND = GetDesktopWindow();
            GetClientRect(hDesktopHWND, &sRect);
            InvalidateRect(hDesktopHWND, &sRect, FALSE);
            ShowWindow(hDesktopHWND, SW_SHOWNORMAL);
            UpdateWindow(hDesktopHWND);
        }
        else{
            g_bWindowModeReal = false;
            g_uiWindowStyle = WS_POPUP;
            SetWindowLong(hWND, GWL_STYLE, g_uiWindowStyle);
            ShowWindow(hWND, SW_SHOWMAXIMIZED);
            UpdateWindow(hWND);
            SetWindowSize(hWND, g_iWindowSizeX, g_iWindowSizeY);
            SetMenu(hWND, NULL);
            SetupOrigin(hWND, g_hMainDc);
        }
        break;

    case WM_KEYDOWN:
        switch (wParam){                           // Process WM_KEYDOWN
        case VK_UP:
        case VK_NUMPAD8:
            g_wKeyCommand |= UP_MASK;
            break;

        case VK_DOWN:
        case VK_NUMPAD2:
            g_wKeyCommand |= DOWN_MASK;
            break;

        case VK_LEFT:
        case VK_NUMPAD4:
            g_wKeyCommand |= LEFT_MASK;
            break;

        case VK_RIGHT:
        case VK_NUMPAD6:
            g_wKeyCommand |= RIGHT_MASK;
            break;

        case VK_CONTROL:
        case VK_NUMPAD5:
            g_bCtrlPressed = true;
            break;

        case VK_SHIFT:
            g_bShiftPressed = true;
            break;

        case 'Z':
        case VK_NUMPAD7:
            g_bKeyCommand2 |= Z_KEY_MASK;
            break;

        case 'X':
        case VK_NUMPAD9:
            g_bKeyCommand2 |= X_KEY_MASK;
            break;

        case VK_RETURN:
        case VK_SPACE:
            g_wKeyCommand |= ENTER_MASK;
            break;

        case VK_ESCAPE:
        case VK_NUMPAD0:
            g_wKeyCommand |= ESCAPE_MASK;
            break;

        default:
            break;
        }
        break;

    case WM_MOUSEMOVE:
        if (!g_bGraphicEffectsRunning){
            g_iMousePosX = LOWORD(lParam);
            g_iMousePosY = HIWORD(lParam);
            g_bScreenUpdatedOrMouseMoved = true;
            if (!g_bWindowModeReal){
                if (g_bShowingFullScrMenu){
                    g_bShowingFullScrMenu = false;
                    SetMenu(hWND, NULL);
                    SetupOrigin(hWND, g_hMainDc);
                    g_bResetMenuSelection = true;
                }
                else if (g_iMousePosY < SHOW_MENU_MOUSE_POS_Y && g_bTrackingMouse && GetActiveWindow()){    // Show menu in full screen mode
                    g_bShowingFullScrMenu = true;
                    SetMenu(hWND, g_hMenu);
                    SetupOrigin(hWND, g_hMainDc);
                }
            }
        }
        break;

    case WM_SYSCOMMAND:
        if (wParam == SC_CLOSE){
            g_iCloseWindowResponce = MessageBox(hWND, _T("Do you wish to quit?"), _T("DIVI-DEAD C'sWARE"), MB_YESNO);
            g_wKeyCommand = EMPTY_MASK;
            g_sPrevKeyCommand = EMPTY_MASK;
            if (g_iCloseWindowResponce != IDYES){
                break;
            }
            SetupOrigin(hWND, g_hMainDc);
            ChangeDisplaySettings(NULL, 0);
            DestroyWindow(hWND);
        }

#ifdef _DEBUG
        QueryPerformanceCounter(&liEndTime);
        ldTime = (long double)liEndTime.QuadPart * 1000.0 / (long double)liFreq.QuadPart;
        _stprintf_s(ptString, _T("Time: %lf[ms]; Processed: %s; wParam: %d; lParam: %d\n"), ldTime, ptMsgName, wParam, lParam);
        WriteFile(hLogFile2, ptString, sizeof(TCHAR) * _tcslen(ptString), &dwBytesWritten, NULL);
#endif

        return DefWindowProc(hWND, WM_SYSCOMMAND, wParam, lParam);

    case WM_SHOW_SCREEN_MODE_DIALOG:
        if (!g_bWindowModeReal){
            g_bShowingFullScrMenu = false;
            SetMenu(hWND, NULL);
            SetupOrigin(hWND, g_hMainDc);
        }
        DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG2), hWND, (DLGPROC)Dialog_Func, 0);
        break;

    case WM_PLAY_LOGO_VIDEO:
        g_bVideoIsPlaying = true;
        VideoPlaybackCtrl(hWND, eVideo_OpenAndPlay, _T("CS_ROGO.AVI"));// Open file
        break;

    case WM_STOP_LOGO_VIDEO:
        VideoPlaybackCtrl(hWND, eVideo_StopAndClose, NULL);        // Stop playback
        g_bVideoIsPlaying = false;
        GetClientRect(hWND, &sRect);
        InvalidateRect(hWND, &sRect, FALSE);
        break;

    case WM_PLAY_INTRO_VIDEO:
        SetCapture(hWND);
        g_bVideoIsPlaying = true;
        VideoPlaybackCtrl(hWND, eVideo_OpenAndPlay, _T("OPEN.AVI"));// // Open file
        break;

    case WM_STOP_INTRO_VIDEO:
        VideoPlaybackCtrl(hWND, eVideo_StopAndClose, NULL);        // Stop playback
        g_bVideoIsPlaying = false;
        ReleaseCapture();
        GetClientRect(hWND, &sRect);
        InvalidateRect(hWND, &sRect, FALSE);
        break;

    case WM_SWITCH_SCREEN_MODE:
        g_bDisplayModeChanged = true;
        if (g_bWindowModeSelected){
            g_bWindowModeSelected = false;
            g_sDevModePars.dmSize = sizeof(DEVMODE);
            g_sDevModePars.dmPelsWidth = GAME_SCREEN_SIZE_X;
            g_sDevModePars.dmPelsHeight = GAME_SCREEN_SIZE_Y;
            g_sDevModePars.dmFields = DM_PELSHEIGHT | DM_PELSWIDTH;
            ShowWindow(hWND, SW_HIDE);
            ChangeDisplaySettings(&g_sDevModePars, 0);
        }
        else{
            g_bWindowModeSelected = true;
            ShowWindow(hWND, SW_HIDE);
            ShowWindow(hWND, SW_HIDE);
            ChangeDisplaySettings(NULL, 0);
        }
        break;

    case MM_MCINOTIFY:
        // MIDI file finished playing => rewind and start playing again
        if (lParam == MIDIPlaybackCtrl(hWND, eMIDI_GetCurrentDev, NULL, false) && wParam == MCI_NOTIFY_SUCCESSFUL){ // GetMIDIDevice
            MIDIPlaybackCtrl(hWND, eMIDI_RewindAndPlay, NULL, false);   // Seek & play
        }

        // Video file finished playing => stop and close device
        if (lParam == VideoPlaybackCtrl(hWND, eVideo_GetCurrentDev, NULL) && wParam == MCI_NOTIFY_SUCCESSFUL){  // GetVideoDevice
            VideoPlaybackCtrl(hWND, eVideo_StopAndClose, NULL); // StopPlayback
            g_bVideoIsPlaying = false;
            break;
        }

        break;

    case WM_LBUTTONDOWN:
        g_wKeyCommand |= ENTER_MASK;
        break;

    case WM_RBUTTONDOWN:
        g_wKeyCommand |= ESCAPE_MASK;
        break;

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
        g_wKeyCommand = EMPTY_MASK;
        break;

    case WM_KEYUP:
        switch (wParam){    // Process WM_KEYUP
        case VK_CONTROL:
        case VK_NUMPAD5:
            g_bCtrlPressed = false;
            break;

        case VK_SHIFT:
            g_bShiftPressed = false;
            break;

        case 'X':
        case 'Z':
        case VK_NUMPAD7:
        case VK_NUMPAD9:
            g_bKeyCommand2 = EMPTY_MASK;
            break;

        case VK_RETURN:
        case VK_ESCAPE:
        case VK_SPACE:
        case VK_LEFT:
        case VK_UP:
        case VK_RIGHT:
        case VK_DOWN:
        case VK_NUMPAD0:
        case VK_NUMPAD2:
        case VK_NUMPAD4:
        case VK_NUMPAD6:
        case VK_NUMPAD8:
            g_wKeyCommand = EMPTY_MASK;
            break;
        default:
            break;
        }
        break;

    case WM_COMMAND:
        switch (wParam){
        case IDM_ABOUT_HELP:        //0x3D4u:   // Show help
            if (!g_bWindowModeReal){
                g_bShowingFullScrMenu = false;
                SetMenu(hWND, NULL);
                SetupOrigin(hWND, g_hMainDc);
            }
            WinHelp(hWND, _T("DIVIDEAD.HLP"), HH_SYNC, NULL);
            break;

        case IDM_DEBUG_DIALOGCHECK: //0x3E4u:   // Menu item Debug/Dialog Check
            DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG4), hWND, (DLGPROC)Dialog_Func, 0);
            break;

        case IDM_DEBUG_EXPLANATION: //0x3E5u:   // Menu item Debug/Explanation
            DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG3), hWND, (DLGPROC)Dialog_Func, 0);
            break;

        case IDM_DEBUG_SCENENAME:   //0x3E6u:   // Menu item Debug/Scene Name
            MessageBox(hWND, g_cptSceneFileName, _T("File Name"), MB_OK);
            break;

        case IDM_FILE_QIUT:         //0x3E7u:   // Menu item File/Quit
            if (!g_bWindowModeReal){
                g_bShowingFullScrMenu = false;
                SetMenu(hWND, NULL);
                SetupOrigin(hWND, g_hMainDc);
            }
            g_iCloseWindowResponce = MessageBox(hWND, _T("Do you wish to quit?"), _T("DIVI-DEAD C'sWARE"), MB_YESNO);
            g_wKeyCommand = EMPTY_MASK;
            g_sPrevKeyCommand = EMPTY_MASK;
            if (g_iCloseWindowResponce != IDYES){
                break;
            }
            SetupOrigin(hWND, g_hMainDc);
            ChangeDisplaySettings(NULL, 0);
            DestroyWindow(hWND);
            break;

        default:
            break;
        }
        break;

    default:
#ifdef _DEBUG
        QueryPerformanceCounter(&liEndTime);
        ldTime = (long double)liEndTime.QuadPart * 1000.0 / (long double)liFreq.QuadPart;
        _stprintf_s(ptString, _T("Time: %lf[ms]; Processed: %s; wParam: %d; lParam: %d\n"), ldTime, ptMsgName, wParam, lParam);
        WriteFile(hLogFile2, ptString, sizeof(TCHAR) * _tcslen(ptString), &dwBytesWritten, NULL);
#endif

        return DefWindowProc(hWND, uiMsg, wParam, lParam);
    }

#ifdef _DEBUG
    QueryPerformanceCounter(&liEndTime);
    ldTime = (long double)liEndTime.QuadPart * 1000.0 / (long double)liFreq.QuadPart;
    _stprintf_s(ptString, _T("Time: %lf[ms]; Processed: %s; wParam: %d; lParam: %d\n"), ldTime, ptMsgName, wParam, lParam);
    WriteFile(hLogFile2, ptString, sizeof(TCHAR) * _tcslen(ptString), &dwBytesWritten, NULL);
#endif

    return 0;
}

//----- (00402A60) --------------------------------------------------------
int InitializeGame(HWND hWnd)
{
    COLORREF    crWindowFrameColor;
    LOGFONT     sLF;
    int         iIdx;
    DWORD       dwCWDLength;
    DWORD       dwStartTime;

    SetMenu(hWnd, g_hMenu);
    g_hMainDc = GetWindowDC(hWnd);
    SetupOrigin(hWnd, g_hMainDc);

    g_hBuf1Bitmap = CreateCompatibleBitmap(g_hMainDc, g_iWindowSizeX, g_iWindowSizeY);
    g_hDcBuffer1 = CreateCompatibleDC(g_hMainDc);
    g_hOldBuf1Bitmap = (HBITMAP)SelectObject(g_hDcBuffer1, g_hBuf1Bitmap);

    g_hBuf2Bitmap = CreateCompatibleBitmap(g_hMainDc, 2 * g_iWindowSizeX, 2 * g_iWindowSizeY);
    g_hDcBuffer2 = CreateCompatibleDC(g_hMainDc);
    g_hOldBuf2Bitmap = (HBITMAP)SelectObject(g_hDcBuffer2, g_hBuf2Bitmap);

    g_hMenuGFX_Bitmap = CreateCompatibleBitmap(g_hMainDc, g_iWindowSizeX, g_iWindowSizeY);
    g_hDcMenuGfx = CreateCompatibleDC(g_hMainDc);
    g_hOld_MenuGFX_Bitmap = (HBITMAP)SelectObject(g_hDcMenuGfx, g_hMenuGFX_Bitmap);

    SetStretchBltMode(g_hMainDc, COLORONCOLOR);
    SetStretchBltMode(g_hDcBuffer1, COLORONCOLOR);
    SetStretchBltMode(g_hDcBuffer2, COLORONCOLOR);

    PatBlt(g_hMainDc, 0, 0, g_iWindowSizeX, g_iWindowSizeY, BLACKNESS);
    PatBlt(g_hDcBuffer1, 0, 0, g_iWindowSizeX, g_iWindowSizeY, BLACKNESS);
    PatBlt(g_hDcBuffer2, 0, 0, g_iWindowSizeX, g_iWindowSizeY, BLACKNESS);

    SetTextColor(g_hMainDc, RGB(0, 0, 0));
    SetTextColor(g_hDcBuffer1, RGB(0, 0, 0));
    SetTextColor(g_hDcBuffer2, RGB(0, 0, 0));

    SetBkMode(g_hMainDc, TRANSPARENT);
    SetBkMode(g_hDcBuffer1, TRANSPARENT);
    SetBkMode(g_hDcBuffer2, TRANSPARENT);
    SetBkMode(g_hDcMenuGfx, TRANSPARENT);

    crWindowFrameColor = GetSysColor(COLOR_WINDOWFRAME);
    SetBkColor(g_hMainDc, crWindowFrameColor);

    g_uMIDIDevsCnt = midiOutGetNumDevs();
    g_uWaveDevsCnt = waveOutGetNumDevs();
    g_bBgmEnabled = g_uMIDIDevsCnt > 0;
    if (g_uWaveDevsCnt > 0){
        g_bVoiceEnabled = true;
        g_bSoundEnabled = true;
    }
    else{
        g_bVoiceEnabled = false;
        g_bSoundEnabled = false;
    }

    g_pvScriptBuffer = malloc(SCRIPT_BUF_SIZE * sizeof(unsigned char));
    g_pvDataBuffer = malloc(DATA_BUF_SIZE * sizeof(unsigned char));
    g_pvSoundBuffer = malloc(SOUND_BUF_SIZE * sizeof(unsigned char));

    if (!g_pvScriptBuffer || !g_pvDataBuffer || !g_pvSoundBuffer){
        MessageBox(hWnd, _T("There is not enough memory"), _T("DIVI-DEAD C'sWARE"), MB_OK);
    }

    sLF.lfHeight = FONT_HEIGHT;
    sLF.lfWidth = FONT_WIDHT;
    sLF.lfEscapement = 0;
    sLF.lfOrientation = 0;
    sLF.lfWeight = FW_NORMAL;
    sLF.lfItalic = FALSE;
    sLF.lfUnderline = FALSE;
    sLF.lfStrikeOut = FALSE;
    sLF.lfCharSet = SHIFTJIS_CHARSET;
    sLF.lfOutPrecision = OUT_DEFAULT_PRECIS;
    sLF.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    sLF.lfQuality = DEFAULT_QUALITY;
    sLF.lfPitchAndFamily = FIXED_PITCH;
    _tcscpy_s(sLF.lfFaceName, _T("System"));
    g_hFont = CreateFontIndirect(&sLF);
    g_hBuf1OldFont = (HFONT)SelectObject(g_hDcBuffer1, g_hFont);
    g_hMainDcOldFont = (HFONT)SelectObject(g_hMainDc, g_hFont);

    InitPalette();

    GetWindowsDirectory(g_ptSaveFolder, MAX_PATH);
    g_ptSaveFolder[2] = _T('\x00');                         // Leave only drive letter
    _tcsncat_s(g_ptSaveFolder, _T("\\CSWARE"), _TRUNCATE);
    CreateDirectory(g_ptSaveFolder, NULL);
    _tcsncat_s(g_ptSaveFolder, _T("\\DIVIDEAD"), _TRUNCATE);
    CreateDirectory(g_ptSaveFolder, NULL);
    _tcsncat_s(g_ptSaveFolder, _T("\\"), _TRUNCATE);

    dwCWDLength = GetCurrentDirectory(MAX_PATH, g_ptWorkingDir);
    if (g_ptWorkingDir[dwCWDLength - 1] != _T('\\')){
        _tcsncat_s(g_ptWorkingDir, _T("\\"), _TRUNCATE);
    }

    memset(&g_sGameState, 0, sizeof(SaveFileState));
    memset(&g_sSysFile, 0, sizeof(SysFileStruc));

    g_iCurrPosInScript = 0;

    strcpy_s(g_sGameState.pcScriptFile, " ");
    strcpy_s(g_sGameState.pcBgPictureName, " ");
    strcpy_s(g_sGameState.pcFgPictureName, " ");
    strcpy_s(g_sGameState.pcReserved1, " ");
    strcpy_s(g_sGameState.pcCharacter1Name, " ");
    strcpy_s(g_sGameState.pcCharacter2Name, " ");
    strcpy_s(g_sGameState.pcFgOverlayName, " ");
    strcpy_s(g_sGameState.pcBgMusicName, " ");
    strcpy_s(g_sSysFile.pcCurrentGameTime, "No comments");

    for (iIdx = 0; iIdx < SAVE_SLOT_COUNT; iIdx++){
        strcpy_s(g_sSysFile.pcSaveNames[iIdx], "    There is no data    ");
    }

    g_sSysFile.sNamesStopSign = 0;

    _tcscpy_s(g_ptFullFileName, g_ptSaveFolder);
    _tcsncat_s(g_ptFullFileName, _T("SYS.DAT"), _TRUNCATE);
    if (ReadFileIntoBuffer(g_ptFullFileName, g_pvDataBuffer)){
        memcpy(&g_sSysFile, g_pvDataBuffer, sizeof(SysFileStruc));
    }

    srand(GetTickCount());

    return 0;
}

//----- (00402F90) --------------------------------------------------------
void ShutdownGame()
{
    TCHAR lpcFilePath[MAX_PATH];
    _tcscpy_s(lpcFilePath, g_ptSaveFolder);
    _tcsncat_s(lpcFilePath, _T("SYS.DAT"), _TRUNCATE);
    OverwriteFile(lpcFilePath, &g_sSysFile, sizeof(SysFileStruc));

    if (g_uMIDIDevsCnt > 0){
        MIDIPlaybackCtrl(g_hMainWindow, eMIDI_StopAndClose, NULL, false);
    }
    if (g_uWaveDevsCnt > 0){
        WavePlaybackCtrl(NULL, eWAVE_Stop);
    }
    VideoPlaybackCtrl(g_hMainWindow, eVideo_StopAndClose, NULL);

    SelectObject(g_hMainDc, g_hMainDcOldFont);
    DeleteObject(SelectObject(g_hDcBuffer1, g_hBuf1OldFont));

    if (g_pvDataBuffer){
        free(g_pvDataBuffer);
    }
    if (g_pvScriptBuffer){
        free(g_pvScriptBuffer);
    }
    if (g_pvSoundBuffer){
        free(g_pvSoundBuffer);
    }

    DeleteObject(SelectObject(g_hDcBuffer1, g_hOldBuf1Bitmap));
    DeleteObject(SelectObject(g_hDcBuffer2, g_hOldBuf2Bitmap));
    DeleteObject(SelectObject(g_hDcMenuGfx, g_hOld_MenuGFX_Bitmap));
    DeleteDC(g_hDcBuffer1);
    DeleteDC(g_hDcBuffer2);
    DeleteDC(g_hDcMenuGfx);
}

//----- (00403110) --------------------------------------------------------
INT_PTR CALLBACK Dialog_Func(HWND hWndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
    /*if(uiMsg != WM_INITDIALOG){
        return TRUE;
        }*/

    if (uiMsg != WM_COMMAND){
        return FALSE;
    }

    switch (wParam){
    case IDC_FULL_SCREEN_BTN:
        g_bFullScrModeFromDlg = true;
        EndDialog(hWndDlg, IDC_FULL_SCREEN_BTN);
        break;

    case IDC_WINDOW_BTN:
        g_bFullScrModeFromDlg = false;
        EndDialog(hWndDlg, IDC_WINDOW_BTN);
        break;

    default:
        break;
    }

    return TRUE;
}

//----- (00403170) --------------------------------------------------------
// return: pressed key
WORD CheckKeysToProc(WORD wKeyMask)
{
    WORD wResult;

    if ((g_wKeyCommand & wKeyMask) == (g_sPrevKeyCommand & wKeyMask)){
        wResult = 0;
    }
    else{
        g_sPrevKeyCommand = g_wKeyCommand;
        wResult = (g_wKeyCommand & wKeyMask) != 0 ? wKeyMask : EMPTY_MASK;
    }

    return wResult;
}

//----- (004031B0) --------------------------------------------------------
void MakeCapsAndCatExt(const TCHAR *cptFName, const TCHAR *cptFExt)
{
    TCHAR *ptStr;

    _tcsncpy_s(g_ptFullFileName, cptFName, MAX_PATH);
    ptStr = _tcsrchr(g_ptFullFileName, _T('.'));
    if (ptStr != NULL){
        ptStr[0] = _T('\x00');
    }
    _tcsupr_s(g_ptFullFileName);
    _tcsncat_s(g_ptFullFileName, _T("."), 2);
    _tcsncat_s(g_ptFullFileName, cptFExt, MAX_PATH);
}

//----- (00403230) --------------------------------------------------------
MCIERROR MIDIPlaybackCtrl(HWND hWnd, int iMidiCommand, const TCHAR *cptFileToPlay, bool bLoopMidiPlayback)
{
    static MCIDEVICEID      hMidiMciDevice;
    static bool             bLoopedMidiPlayback;
    MCI_PLAY_PARMS          sMidiMciPlayPars;
    MCI_OPEN_PARMS          sMciOpenPars;
    MCI_GENERIC_PARMS       sMciGenericPars;
    MCIERROR                dwResult;
    TCHAR                   ptStr[MAX_PATH];

    dwResult = 0;

    switch (iMidiCommand){
    case eMIDI_OpenAndPlay: // Open and play midi file
        _tcscpy_s(ptStr, g_ptWorkingDir);
        _tcsncat_s(ptStr, _T("MID\\"), _TRUNCATE);
        _tcsncat_s(ptStr, cptFileToPlay, _TRUNCATE);
        _tcsncat_s(ptStr, _T(".MID"), _TRUNCATE);

        if (hMidiMciDevice){
            mciSendCommand(hMidiMciDevice, MCI_STOP, 0, (DWORD_PTR)&sMciGenericPars);
            mciSendCommand(hMidiMciDevice, MCI_CLOSE, 0, (DWORD_PTR)&sMciGenericPars);
        }

        sMciOpenPars.dwCallback = NULL;
        sMciOpenPars.lpstrDeviceType = _T("sequencer");
        sMciOpenPars.lpstrElementName = ptStr;
        dwResult = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&sMciOpenPars);
        if (dwResult){  // Error happened
            mciGetErrorString(dwResult, ptStr, MAX_PATH);
            MessageBox(hWnd, ptStr, _T("MCI error MIDI"), MB_OK);
            mciSendCommand(-1, MCI_CLOSE, MCI_WAIT, (DWORD_PTR)&sMciGenericPars);
            dwResult = 0;
            break;
        }

        hMidiMciDevice = sMciOpenPars.wDeviceID;
        sMidiMciPlayPars.dwCallback = (DWORD_PTR)hWnd;
        bLoopedMidiPlayback = bLoopMidiPlayback;
        dwResult = mciSendCommand(sMciOpenPars.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&sMidiMciPlayPars);

        break;

    case eMIDI_StopAndClose:    // Stop playback and close file
        if (hMidiMciDevice){
            mciSendCommand(hMidiMciDevice, MCI_STOP, 0, (DWORD_PTR)&sMciGenericPars);
            dwResult = mciSendCommand(hMidiMciDevice, MCI_CLOSE, 0, (DWORD_PTR)&sMciGenericPars);
            hMidiMciDevice = NULL;
        }
        break;

    case eMIDI_Play:    // Start playback
        if (hMidiMciDevice){
            sMidiMciPlayPars.dwCallback = (DWORD_PTR)hWnd;
            bLoopedMidiPlayback = bLoopMidiPlayback;
            dwResult = mciSendCommand(hMidiMciDevice, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&sMidiMciPlayPars);
        }
        break;

    case eMIDI_Stop:    // Stop playback
        if (hMidiMciDevice){
            dwResult = mciSendCommand(hMidiMciDevice, MCI_STOP, 0, (DWORD_PTR)&sMciGenericPars);
            bLoopedMidiPlayback = false;
        }
        break;

    case eMIDI_RewindAndPlay:   // Rewind and start playback
        if (hMidiMciDevice && bLoopedMidiPlayback){
            mciSendCommand(hMidiMciDevice, MCI_SEEK, MCI_SEEK_TO_START | MCI_WAIT, NULL);
            dwResult = mciSendCommand(hMidiMciDevice, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&sMidiMciPlayPars);
        }
        break;

    case eMIDI_GetCurrentDev:   // Get current MIDI MCI device
        if (hMidiMciDevice){
            dwResult = hMidiMciDevice;
        }
        break;

    default:
        break;
    }
    return dwResult;
}

//----- (004034E0) --------------------------------------------------------
MCIERROR VideoPlaybackCtrl(HWND hWnd, int iCommand, const TCHAR *cptFileToPlay)
{
    static MCIDEVICEID      hAviMCIDevice;
    MCI_PLAY_PARMS          sAviMCIPlayPars;
    MCI_ANIM_OPEN_PARMS     sAviFileMciOpenPars;
    MCI_ANIM_RECT_PARMS     sMciAnimRectPars;
    MCI_ANIM_WINDOW_PARMS   sMciAnimWindowPars;
    MCI_STATUS_PARMS        sMciStatusPars;
    MCIERROR                dwResult;
    TCHAR                   ptStr[MAX_PATH];

    dwResult = 0;

    switch (iCommand){
    case eVideo_OpenAndPlay:    // Open and play file
        _tcscpy_s(ptStr, g_ptWorkingDir);
        _tcsncat_s(ptStr, cptFileToPlay, _TRUNCATE);

        if (hAviMCIDevice){
            mciSendCommand(hAviMCIDevice, MCI_STOP, 0, 0);
            mciSendCommand(hAviMCIDevice, MCI_CLOSE, 0, 0);
        }

        sAviFileMciOpenPars.dwCallback = NULL;
        sAviFileMciOpenPars.wDeviceID = NULL;
        sAviFileMciOpenPars.lpstrDeviceType = _T("avivideo");
        sAviFileMciOpenPars.lpstrElementName = ptStr;
        sAviFileMciOpenPars.lpstrAlias = NULL;
        sAviFileMciOpenPars.dwStyle = WS_CHILD;
        sAviFileMciOpenPars.hWndParent = hWnd;
        dwResult = mciSendCommand(NULL, MCI_OPEN, MCI_ANIM_OPEN_PARENT | MCI_ANIM_OPEN_WS | MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&sAviFileMciOpenPars);
        if (dwResult){  // Error happened
            mciGetErrorString(dwResult, ptStr, MAX_PATH);
            MessageBox(hWnd, ptStr, _T("MCI error AVI"), MB_OK);
            mciSendCommand(-1, MCI_CLOSE, MCI_WAIT, NULL);
            dwResult = 0;
            break;
        }

        hAviMCIDevice = sAviFileMciOpenPars.wDeviceID;
        sMciAnimRectPars.rc.left = (GAME_SCREEN_SIZE_X - VIDEO_SIZE_X) / 2;
        sMciAnimRectPars.rc.top = (GAME_SCREEN_SIZE_Y - VIDEO_SIZE_Y) / 2 /*- 20*/;
        sMciAnimRectPars.rc.right = VIDEO_SIZE_X;
        sMciAnimRectPars.rc.bottom = VIDEO_SIZE_Y;

        mciSendCommand(hAviMCIDevice, MCI_PUT, MCI_DGV_PUT_CLIENT | MCI_DGV_PUT_WINDOW | MCI_DGV_RECT, (DWORD_PTR)&sMciAnimRectPars);

        sMciAnimWindowPars.dwCallback = NULL;
        sMciAnimWindowPars.hWnd = NULL;
        sMciAnimWindowPars.nCmdShow = SW_SHOW;
        sMciAnimWindowPars.lpstrText = NULL;
        mciSendCommand(hAviMCIDevice, MCI_WINDOW, MCI_ANIM_WINDOW_STATE, (DWORD_PTR)&sMciAnimWindowPars);

        sMciStatusPars.dwItem = MCI_ANIM_STATUS_SPEED;
        mciSendCommand(hAviMCIDevice, MCI_STATUS, MCI_STATUS_ITEM, (DWORD_PTR)&sMciStatusPars);
        g_lVideoPlaybackSpeed = sMciStatusPars.dwReturn;

        sAviMCIPlayPars.dwCallback = (DWORD_PTR)hWnd;
        sAviMCIPlayPars.dwTo = 0;
        sAviMCIPlayPars.dwFrom = 0;
        dwResult = mciSendCommand(hAviMCIDevice, MCI_PLAY, MCI_NOTIFY, (DWORD_PTR)&sAviMCIPlayPars);

        break;

    case eVideo_StopAndClose:   // Stop and close
        if (hAviMCIDevice){
            mciSendCommand(hAviMCIDevice, MCI_STOP, MCI_WAIT, 0);
            dwResult = mciSendCommand(hAviMCIDevice, MCI_CLOSE, MCI_WAIT, 0);
            hAviMCIDevice = NULL;
        }
        break;

    case eVideo_GetCurrentDev:  // Get AVI MCI Device
        if (hAviMCIDevice){
            dwResult = hAviMCIDevice;
        }
        break;

    default:
        break;
    }

    return dwResult;
}

//----- (00403700) --------------------------------------------------------
// return: number of characters printed
int OutputString(HDC hDc1, HDC hDc2, int iX, int iY, int iMaxLen, TCHAR *pcString, DWORD dwSleepTimeBetweenChars)
{
    int         iStrLen;
    int         iIdx;
    bool        bControlSymbEncountered;
    int         iCursorY;
    int         iCursorX;
    TCHAR       *ptStringToDraw;
    int         iLineNumber;
    signed int  iCharsPrinted;
    signed int  iAlignOffset;
    int         iLineHeight;
    TCHAR       tChar;

    iStrLen = 0;
    iIdx = 0;
    iCharsPrinted = 0;
    iLineNumber = 0;
    iAlignOffset = 0;
    iLineHeight = g_iTextLineHeight;

    SplitStringsToFitOnScreen(pcString);

    while (1){
        bControlSymbEncountered = false;
        tChar = g_ptStrings[iIdx + 1];
        iIdx++;

        switch (tChar){
        case _T('\\'):
            tChar = _totupper(g_ptStrings[iIdx + 1]);
            iIdx++;

            switch (tChar){
            case _T('N'):   // New line
                iStrLen = iAlignOffset;
                iLineNumber++;
                bControlSymbEncountered = true;
                break;

            case _T('W'):   // Sleep
                Sleep(500);
                bControlSymbEncountered = true;
                break;

            case _T('T'):   // Tab
                iStrLen = iStrLen + TAB_SIZE;
                bControlSymbEncountered = true;
                break;

            case _T('S'):   // Stop
                GdiFlush();
                return 1;

            default:
                GdiFlush();
                return -1;
            }
            break;

        case _T('|'):   // Set align offset
            iAlignOffset = iStrLen;
            bControlSymbEncountered = true;
            break;

        case _T('\t'):  // Tab
            iStrLen = iStrLen + TAB_SIZE;
            bControlSymbEncountered = true;
            break;

        case _T('\0'):  // Stop
            GdiFlush();
            return iCharsPrinted;

        default:
            break;
        }

        if (!bControlSymbEncountered){
            // '、' - 0x8141 - Unicode 0x3001 Ideographic comma
            // '。' - 0x8142 - Unicode 0x3002 Ideographic full stop
            // '）' - 0x816A - Unicode 0xFF09 Full width right parenthesis
            // '」' - 0x8176 - Unicode 0x300D Right corner bracket
            if (iStrLen >= iMaxLen){
                if (g_ptStrings[iIdx] != _T('、') && g_ptStrings[iIdx] != _T('。') && g_ptStrings[iIdx] != _T('」') && g_ptStrings[iIdx] != _T('）')){
                    iStrLen = iAlignOffset;
                    iLineNumber++;
                }
            }

            iCursorY = iY + iLineNumber * iLineHeight;
            iCursorX = iX + (FONT_WIDHT - 1) * iStrLen;
            //if((tChar <= 0x007F || tChar >= 0x00A0) && tChar < 0x00E0){   // Needed only for Shift-JIS
            ptStringToDraw = &g_ptStrings[iIdx];
            if (ptStringToDraw[0] == _T('@')){
                ptStringToDraw[0] = _T('"');
            }

            TextOut(hDc1, iCursorX, iCursorY, ptStringToDraw, 1);
            if (hDc2 != hDc1){
                TextOut(hDc2, iCursorX, iCursorY, ptStringToDraw, 1);
            }
            ++iStrLen;
            ++iCharsPrinted;
            /*}
            else{       // Needed only for Shift-JIS
            TextOut(hDc1, iX, y + iLineNumber * iLineHeight, &g_ptStrings[iIdx], 2);
            TextOut(hDc2, iX, iY, &g_ptStrings[iIdx], 2);
            iIdx++;
            iStrLen += 2;
            iCharsPrinted += 2;
            }*/

            if (dwSleepTimeBetweenChars){
                GdiFlush();
                Sleep(dwSleepTimeBetweenChars);
            }
        }
    }
}

//----- (004039C0) --------------------------------------------------------
// return: key pressed
WORD WaitForKeyPress()
{
    WORD wKeyPressed;

    while (1){
        Sleep(50);
        if (g_bWaitingUserInput){
            BlitWaitingAnimation();
        }

        if (g_bCtrlPressed || g_bKeyCommand2){
            return 0;
        }
        wKeyPressed = CheckKeysToProc(ESCAPE_MASK) | CheckKeysToProc(ENTER_MASK);
        if ((wKeyPressed & ESCAPE_MASK) && g_bMenuOnScreen){
            wKeyPressed = 0;
        }
        if (wKeyPressed){
            return wKeyPressed;
        }
    }
}

//----- (00403A40) --------------------------------------------------------
void BlitWaitingAnimation()
{
    BitBlt(g_hMainDc,
        g_sTextWindowPos.x + FLIP_BOOK_POS_X, g_sTextWindowPos.y + (MAX_TEXT_LINE_COUNT - 1) * g_iStringHeight + 4,
        FLIP_BOOK_SIZE_X, FLIP_BOOK_SIZE_Y,
        g_hDcMenuGfx,
        FLIP_BOOK_SIZE_X * (g_iBookProgressIdx / 3), FLIP_BOOK_OFFSET_Y, SRCCOPY);

    g_iBookProgressIdx = (g_iBookProgressIdx + 1) % 21;
}

//----- (00403AD0) --------------------------------------------------------
// return: false - success; true - fail
bool LoadScriptFile(const TCHAR *cptScriptName)
{
    if (!cptScriptName){
        return true;
    }

    ConvertUniToMBCS(cptScriptName, g_pcCvtString, CVT_BUF_SIZE);
    strcpy_s(g_sGameState.pcScriptFile, 32, g_pcCvtString);
    MakeCapsAndCatExt(cptScriptName, _T("AB"));
    return !FindAndReadFileInSG(g_ptFullFileName, g_pvScriptBuffer);

}

//----- (00403B20) --------------------------------------------------------
DWORD WINAPI MainGameThreadProc(LPVOID lpParameter)
{
    HWND    hWnd;
    int     iPrevPosInScript;
    int     iNewPosInScript;
    WORD    wScriptCMD = 0x0FFF;
    short   iKeyCommand;
    short   sRangeStart;
    short   sRangeEnd;
    char    bCompareCommand;
    short   sFlagToCompare;
    int     sOffsetInScript;
    int     iIdx1;
    WORD    sFlagVal;
    short   sFlagResult;
    WORD    sFlagIdx;
    WORD    sEndFlagIdx;
    BYTE    bFlagCmd;
    char    *pcText;

    hWnd = (HWND)lpParameter;

    WaitForSingleObject(g_hCreateDoneEvent, INFINITE);

    while (1){
        if (g_iRequiredAction == eShowIntro){                       // Show intro
            g_sIntermediateResult = VideoModeAndIntro();

            g_iRequiredAction = eProcessMainMenu;
        }

        if (g_iRequiredAction == eProcessMainMenu){                     // Show start screen and process main menu
            sFlagResult = ProcessMainStartMenu();

            if (sFlagResult == 3){                      // Exit game
                MessageBox(hWnd, _T("Cannot read the script"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                g_bWindowModeSelected = false;
                SendMessage(hWnd, WM_SWITCH_SCREEN_MODE/*0x40Au*/, 0, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }

            if (sFlagResult == 4){
                g_bWindowModeSelected = false;
                SendMessage(hWnd, WM_SWITCH_SCREEN_MODE/*0x40Au*/, 0, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }

        }

        if (g_iRequiredAction == eLoadSave){                // Load save
            LoadSave(g_sSaveToLoad);
            g_wKeyCommand = EMPTY_MASK;
            g_sPrevKeyCommand = EMPTY_MASK;
        }

        iPrevPosInScript = g_iCurrPosInScript;
        g_iRequiredAction = eProcessScript;
        iNewPosInScript = g_iCurrPosInScript;
        wScriptCMD = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
        g_iCurrPosInScript += 2;

        switch (wScriptCMD){

        case 0x0000:    // Show text and wait input
            g_iNewPosInScript = iPrevPosInScript;

            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            g_crColor = RGB(0xFF, 0xFF, 0xFF);
            SetTextColor(g_hMainDc, g_crColor);
            SetTextColor(g_hDcBuffer1, g_crColor);
            g_iTextLineHeight = 16;
            OutputString(g_hDcBuffer1, g_hMainDc, g_sTextWindowPos.x, g_sTextWindowPos.y, 46, g_ptScriptName, g_iSleepTimeBetweenChars);

            g_bWaitingUserInput = true;
            g_iBookProgressIdx = 0;
            g_bTrackingMouse = true;

            sFlagResult = WaitForKeyPress();

            iKeyCommand = sFlagResult;
            g_bTrackingMouse = false;
            g_bWaitingUserInput = false;

            RestoreImage(g_sTextWindowPos.x, g_sTextWindowPos.y, 432, 3 * g_iStringHeight, g_sTextWindowPos.x, g_sTextWindowPos.y);

            if (g_bVoiceWavePlaying){
                WavePlaybackCtrl(NULL, eWAVE_Stop);
                g_bVoiceWavePlaying = false;
            }

            if (g_bKeyCommand2 & Z_KEY_MASK || iKeyCommand & ENTER_MASK && g_iMousePosX > 16 && g_iMousePosX < 80 && g_iMousePosY > 400 && g_iMousePosY < 464){
                g_bKeyCommand2 = 0;

                sFlagResult = ProcessInGameMenu();

                if (sFlagResult == 4){  // Exit game
                    g_bWindowModeSelected = 0;
                    SendMessage(hWnd, WM_SWITCH_SCREEN_MODE /*0x40Au*/, 0, 0);
                    PostMessage(hWnd, WM_DESTROY, 0, 0);
                    ExitThread(0);
                }
                if (sFlagResult == 3){  // Show start screen and process main menu
                    g_iRequiredAction = eProcessMainMenu;
                }
                if (sFlagResult != -1){ // Menu was not canceled
                    continue;
                }

                g_iCurrPosInScript = g_iNewPosInScript;
                continue;
            }

            if (!(g_bKeyCommand2 & X_KEY_MASK) && (!(iKeyCommand & ENTER_MASK) || g_iMousePosX <= 560 || g_iMousePosX >= 624 || g_iMousePosY <= 400 || g_iMousePosY >= 464)){
                continue;
            }

            g_bKeyCommand2 = 0;
            sFlagResult = ProcessRightMenu();
            g_iCurrPosInScript = g_iNewPosInScript;
            continue;

        case 0x0001:    // Insert one text menu item
            sOffsetInScript = GET_DWORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 4;

            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            g_iMenuItemsPosInScript[g_sMenuLength] = sOffsetInScript;
            _tcsncpy_s(g_pptMenuStrings[g_sMenuLength], 64, g_ptScriptName, 63);
            g_sMenuLength++;
            continue;

        case 0x0002:    // Jump in script command
            g_iCurrPosInScript = GET_DWORD(g_pvScriptBuffer, g_iCurrPosInScript);
            continue;

        case 0x0003:    // Store value into multiple flags
            sFlagIdx = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);  // Starting flag index
            g_iCurrPosInScript += 2;
            sEndFlagIdx = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);       // Ending flag index
            g_iCurrPosInScript += 2;
            sFlagResult = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);       // Flag value to set
            g_iCurrPosInScript += 2;
            sFlagVal = sEndFlagIdx - sFlagIdx;                          // Flag count

            if (sFlagIdx >= 1000 || sFlagVal >= 1000){
                wsprintf(g_ptFullFileName, _T("Flag number is too large (%d)_FCLR"), sFlagIdx);
                MessageBox(g_hMainWindow, g_ptFullFileName, _T("DIVI-DEAD C'sWARE"), MB_OK);
            }

            for (g_iX = 0; g_iX < sFlagVal; g_iX++){
                g_sGameState.psFlags[sFlagIdx + g_iX] = sFlagResult;
            }
            continue;

        case 0x0004:    // Process flag math commands
            sFlagIdx = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);      // Flag index
            g_iCurrPosInScript += 2;
            bFlagCmd = GET_BYTE(g_pvScriptBuffer, g_iCurrPosInScript);      // Flag command
            g_iCurrPosInScript++;
            sFlagVal = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);      // Flag value
            g_iCurrPosInScript += 2;

            iPrevPosInScript = g_sGameState.psFlags[sFlagIdx];

            if (sFlagIdx >= 1000){
                wsprintf(g_ptFullFileName, _T("Flag number is too large (%d)_FSET"), sFlagIdx);
                MessageBox(g_hMainWindow, g_ptFullFileName, _T("DIVI-DEAD C'sWARE"), MB_OK);
            }

            if (sFlagVal == 128){
                sFlagVal = g_sGameState.psFlags[0];
            }

            switch (bFlagCmd){
            case 0x3D:  // Move flag command
                sFlagResult = sFlagVal;
                break;

            case 0x2B:  // Add command
                sFlagResult = iPrevPosInScript + sFlagVal;
                break;

            case 0x2D:  // Subtract command
                sFlagResult = iPrevPosInScript - sFlagVal;
                if (sFlagResult < 0){
                    sFlagResult = 0;
                }
                break;

            case 0x2A:  // Multiply command
                sFlagResult = iPrevPosInScript * sFlagVal;
                break;

            case 0x2F:  // Divide command
                if (!sFlagVal){
                    MessageBox(hWnd, _T("Error divide"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                    break;
                }
                sFlagResult = iPrevPosInScript / sFlagVal;
                break;

            case 0x2E:  // Modulo command
                if (sFlagVal){
                    sFlagResult = iPrevPosInScript % sFlagVal;
                }
                else{
                    MessageBox(hWnd, _T("Error divide"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                }
                break;

            case 0x7C:  // Logical OR command
                sFlagResult = sFlagVal | iPrevPosInScript;
                break;

            case 0x24:  // Logical AND command
                sFlagResult = sFlagVal & iPrevPosInScript;
                break;

            default:
                break;
            }
            g_sGameState.psFlags[sFlagIdx] = sFlagResult;
            continue;

        case 0x0005:    // Wait for key press command
            sFlagResult = WaitForKeyPress();
            continue;

        case 0x0006:    // Save position in script & prepare for new text menu
            g_iSavedPosInScript = iPrevPosInScript;
            g_bStartingMenu = true;
            g_sMenuLength = 0;
            g_iSelectedMenuItem = -1;
            continue;

        case 0x0007:    // Process main menus and in-game text menu
            g_iCurrPosInScript = g_iNewPosInScript = g_iSavedPosInScript;
            //g_crColor = RGB(255, 255, 255);
            g_crMenuFGColor = RGB(255, 255, 255);
            g_crMenuBGColor = RGB(0, 0, 0);

            g_bTrackingMouse = true;
            g_sPoint.x = g_sTextWindowPos.x - 8;
            g_sPoint.y = g_sTextWindowPos.y + 4;
            ClientToScreen(g_hMainWindow, &g_sPoint);
            SetCursorPos(g_sPoint.x, g_sPoint.y);
            g_iMenuLineHeight = 16;

            g_sIntermediateResult = ShowMenuAndWaitForSel(g_sTextWindowPos.x, g_sTextWindowPos.y, 4);

            g_bTrackingMouse = false;
            RestoreImage(g_sTextWindowPos.x, g_sTextWindowPos.y, 436, 3 * g_iStringHeight, g_sTextWindowPos.x, g_sTextWindowPos.y);

            g_iCurrPosInScript = g_iMenuItemsPosInScript[g_sIntermediateResult];

            if (g_bKeyCommand2 & Z_KEY_MASK || g_iMousePosX > 16 && g_iMousePosX < 80 && g_iMousePosY > 400 && g_iMousePosY < 464){
                g_bKeyCommand2 = 0;
                sFlagResult = ProcessInGameMenu();
                if (sFlagResult == 4){
                    ChangeDisplaySettings(NULL, 0);
                    PostMessage(hWnd, WM_DESTROY, 0, 0);
                    ExitThread(0);
                }
                if (sFlagResult == 3){
                    MessageBox(hWnd, _T("Cannot read the script"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                    ChangeDisplaySettings(NULL, 0);
                    PostMessage(hWnd, WM_DESTROY, 0, 0);
                    ExitThread(0);
                }
                if (sFlagResult != -1){
                    continue;
                }
                g_iCurrPosInScript = g_iNewPosInScript;
            }
            else{
                if (g_bKeyCommand2 & X_KEY_MASK || g_iMousePosX > 560 && g_iMousePosX < 624 && g_iMousePosY > 400 && g_iMousePosY < 464){
                    g_bKeyCommand2 = 0;
                    sFlagResult = ProcessRightMenu();
                    g_iCurrPosInScript = g_iNewPosInScript;
                }
            }
            continue;

        case 0x000A:    // Restore saved position in script
            g_iCurrPosInScript = g_iSavedPosInScript;
            g_iMenuItemToSelect = g_sIntermediateResult;
            continue;

        case 0x000D:    // Skip 2 bytes and continue (NOP?)
            g_iCurrPosInScript += 2;
            continue;

        case 0x000E:    // Read flag Index and Value
        case 0x000F:
            sFlagIdx = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            sFlagVal = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            continue;

        case 0x0010:    // Compare flag and jump if condition false
            sFlagIdx = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);          // Flag index
            g_iCurrPosInScript += 2;
            bCompareCommand = GET_BYTE(g_pvScriptBuffer, g_iCurrPosInScript);   // Comparison command
            g_iCurrPosInScript++;
            sFlagVal = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);          // Value to compare stored flag with
            g_iCurrPosInScript += 2;
            iNewPosInScript = GET_DWORD(g_pvScriptBuffer, g_iCurrPosInScript);  // Jump to if comparison successful
            g_iCurrPosInScript += 4;
            sFlagToCompare = g_sGameState.psFlags[sFlagIdx];

            if ((short)sFlagVal < 0){
                sFlagVal = g_sGameState.psFlags[0];
            }

            switch (bCompareCommand){
            case 0x3D:  // Equal command
                if (sFlagToCompare != sFlagVal){
                    bCompareCommand = 0;
                }
                break;

            case 0x21:  // Not equal command
                if (sFlagToCompare == sFlagVal){
                    bCompareCommand = 0;
                }
                break;

            case 0x7B:  // Less than command
                if (sFlagToCompare >= sFlagVal){
                    bCompareCommand = 0;
                }
                break;

            case 0x7D:  // Greater than command
                if (sFlagToCompare <= sFlagVal){
                    bCompareCommand = 0;
                }
                break;

            default:
                break;
            }

            if (!bCompareCommand){
                g_iCurrPosInScript = iNewPosInScript;
            }
            continue;

        case 0x0011:    // Sleep command
            g_usiSleepTime = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            Sleep(g_usiSleepTime * 10);

            continue;

        case 0x0012:    // Sleep and wait for key press command
            g_usiSleepTime = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            Sleep(g_usiSleepTime * 10);

            sFlagResult = WaitForKeyPress();
            continue;

        case 0x0013:    // Load and display BG command
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            if (LoadAndBlitPicToBuf2(0, eRightAndBottomFromScreen_BG, g_ptScriptName)){
                MessageBox(hWnd, _T("Cannot read CG"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }

            CopyPictureWithTranspColor(eRightAndBottomFromScreen_BG, 0, 0);
            continue;

        case 0x0014:    // Blit CG with effect command
            sFlagIdx = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            BlitPicWithEffects(sFlagIdx);
            continue;

        case 0x0016:    // Load and display CG with transparent color command, remove personages
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            if (LoadAndBlitPicToBuf2(2, eRightFromScreen_TranspOverlay, g_ptScriptName)){
                MessageBox(hWnd, _T("Cannot read CG"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }
            CopyPictureWithTranspColor(eRightFromScreen_TranspOverlay, 0, 0);

            strcpy_s(g_sGameState.pcFgOverlayName, 32, pcText);
            strcpy_s(g_sGameState.pcCharacter1Name, 32, " ");
            strcpy_s(g_sGameState.pcCharacter2Name, 32, " ");
            continue;

        case 0x0017:    // Remove image from screen with effect
            sFlagIdx = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            sFlagVal = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;

            EraseBGBlitWEffects(sFlagVal);

            g_sPicOffsetNum = FindPicOrigin(g_sPicWidth, g_sPicHeight);
            if (g_sPicOffsetNum == 1){
                strcpy_s(g_sGameState.pcBgPictureName, " ");
                strcpy_s(g_sGameState.pcFgPictureName, " ");
                strcpy_s(g_sGameState.pcCharacter1Name, " ");
                strcpy_s(g_sGameState.pcCharacter2Name, " ");
                strcpy_s(g_sGameState.pcFgOverlayName, " ");
            }
            if (g_sPicOffsetNum == 2){
                strcpy_s(g_sGameState.pcFgPictureName, " ");
                strcpy_s(g_sGameState.pcCharacter1Name, " ");
                strcpy_s(g_sGameState.pcCharacter2Name, " ");
                strcpy_s(g_sGameState.pcFgOverlayName, " ");
            }
            continue;

        case 0x0018:    // Load script command
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            if (LoadScriptFile(g_ptScriptName)){
                MessageBox(hWnd, _T("Cannot read the script"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }
            g_iCurrPosInScript = 0;
            continue;

        case 0x0019:    // Show credits and return to start screen
            g_sSysFile.pbGameEventsAchieved[0] = 1; // Game is completed

            _tcsncpy_s(g_ptFullFileName, MAX_PATH, g_ptSaveFolder, MAX_PATH);
            _tcsncat_s(g_ptFullFileName, MAX_PATH, _T("SYS.DAT"), MAX_PATH);
            OverwriteFile(g_ptFullFileName, &g_sSysFile, sizeof(g_sSysFile)/*0x10C6u*/);

            ShowCredits();

            strcpy_s(g_sGameState.pcBgMusicName, 32, " ");
            StopMidiPlayback();
            Sleep(100);

            g_iRequiredAction = eProcessMainMenu;                       // Return to start screen

            continue;

        case 0x001E:    // Dim screen to black
            DimScreenToBlack();
            continue;

        case 0x001F:    // Dim screen to white
            DimmScreenToWhite();
            continue;

        case 0x0025:    // Store random value into a flag
            sFlagIdx = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);          // Flag index
            g_iCurrPosInScript += 2;
            sRangeStart = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);       // Random value: range start
            g_iCurrPosInScript += 2;
            sRangeEnd = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);         // Random value: range end
            g_iCurrPosInScript += 2;
            sFlagResult = (sRangeStart + rand() % (sRangeEnd - sRangeStart));
            g_sGameState.psFlags[sFlagIdx] = sFlagResult;
            continue;

        case 0x0026:    // Play BG music
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            if (PlayMidiFile(g_ptScriptName, true)){
                MessageBox(hWnd, _T("Music cannot be read"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }

            strcpy_s(g_sGameState.pcBgMusicName, 32, pcText);
            continue;

        case 0x0028:    // Stop BG music playback
            strcpy_s(g_sGameState.pcBgMusicName, 32, " ");
            StopMidiPlayback();
            continue;

        case 0x0029:    // Skip cycle command
            continue;

        case 0x002B:    // Play wave file (voice)
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            g_bVoiceWavePlaying = false;
            if (!g_bVoiceEnabled || g_bCtrlPressed){
                continue;
            }
            if (WavePlaybackCtrl(g_ptScriptName, eWAVE_LoadFormArcAndPlay)){
                MessageBox(hWnd, _T("Cannot read wav file"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }
            g_bVoiceWavePlaying = true;
            continue;

        case 0x0030:    // Load pic origin and size
            g_iOriginX = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            g_iOriginY = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            g_sPicWidth = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            g_sPicHeight = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            continue;

        case 0x0031:    // Game event achieved - unused
            sFlagResult = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            g_sSysFile.pbGameEventsAchieved[sFlagResult] = 1;
            _tcscpy_s(g_ptFullFileName, g_ptSaveFolder);
            _tcsncat_s(g_ptFullFileName, _T("SYS.DAT"), _TRUNCATE);
            OverwriteFile(g_ptFullFileName, &g_sSysFile, sizeof(g_sSysFile)/*0x10C6u*/);
            continue;

        case 0x0032:    // Set flag to false
            g_bReserved0001 = false;
            continue;

        case 0x0033:    // Set flag to true
            g_bReserved0001 = true;
            continue;

        case 0x0035:    // Play wave file (effects)
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            if (!g_bSoundEnabled){
                continue;
            }
            if (WavePlaybackCtrl(g_ptScriptName, eWAVE_LoadFormArcAndPlay)){
                MessageBox(hWnd, _T("Cannot read wav file"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }
            continue;

        case 0x0036:    // Stop wave playback
            WavePlaybackCtrl(NULL, eWAVE_Stop);
            continue;

        case 0x0037:    // Prepare 2 pics for graphical menu, display first with horizontal blinds effect
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            if (LoadAndBlitPicToBuf2(0, eMainScreen, g_ptScriptName)){
                MessageBox(hWnd, _T("Cannot read CG"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }

            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            if (LoadAndBlitPicToBuf2(0, eRightFromScreen_TranspOverlay, g_ptScriptName)){
                MessageBox(hWnd, _T("Cannot read CG"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }
            BlitPicWithEffects(eHorizontalBlinds);
            continue;

        case 0x0038:    // Save position in script & prepare for new graphic menu
            g_iSavedPosInScript = iPrevPosInScript;
            g_bStartingMenu = true;
            g_sMenuLength = 0;
            g_iSelectedMenuItem = -1;
            continue;

        case 0x0040:    // Load graphical menu item's link in script and bounding rectangle on screen
            g_iMenuItemsPosInScript[g_sMenuLength] = GET_DWORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 4;
            g_sCGRects[g_sMenuLength].sLeft = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript) + 32;
            g_iCurrPosInScript += 2;
            g_sCGRects[g_sMenuLength].sTop = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript) + 8;
            g_iCurrPosInScript += 2;
            g_sCGRects[g_sMenuLength].sRight = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript) + 32;
            g_iCurrPosInScript += 2;
            g_sCGRects[g_sMenuLength].sBottom = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript) + 8;
            g_iCurrPosInScript += 2;
            g_sMenuLength++;
            continue;

        case 0x0041:    // Select from graphical menu
            g_crColor = RGB(255, 255, 255);
            SetTextColor(g_hMainDc, g_crColor);
            SetTextColor(g_hDcBuffer1, g_crColor);

            g_iTextLineHeight = 16;
            OutputString(g_hDcBuffer1, g_hMainDc, g_sTextWindowPos.x, g_sTextWindowPos.y, 46, _T("Choose destination"), 0);

            g_iCurrPosInScript = g_iSavedPosInScript;

            do{
                g_sIntermediateResult = SelectItemInGraphicalMode();
            } while (g_sIntermediateResult == -1);

            g_iCurrPosInScript = g_iMenuItemsPosInScript[g_sIntermediateResult];
            RestoreImage(g_sTextWindowPos.x, g_sTextWindowPos.y, 432, 3 * g_iStringHeight, g_sTextWindowPos.x, g_sTextWindowPos.y);
            continue;

        case 0x0042:    // Output string at specified coordinates
            sFlagIdx = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;
            sFlagVal = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;

            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            g_crColor = RGB(255, 255, 255);
            SetTextColor(g_hMainDc, g_crColor);
            SetTextColor(g_hDcBuffer1, g_crColor);
            g_iTextLineHeight = 16;
            OutputString(g_hDcBuffer1, g_hMainDc, sFlagIdx, sFlagVal, 46, g_ptScriptName, 0);
            continue;

        case 0x0043:    // Paint black and restore image
            PatBlt(g_hDcBuffer2, sFlagIdx, sFlagVal, 414, 3 * g_iStringHeight, BLACKNESS);
            RestoreImage(sFlagIdx, sFlagVal, 432, 3 * g_iStringHeight, sFlagIdx, sFlagVal);
            continue;

        case 0x0044:    // Load picture to buffer right from screen
            sFlagResult = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;

            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            if (LoadAndBlitPicToBuf2(0, eRightFromScreen_TranspOverlay, g_ptScriptName)){
                MessageBox(hWnd, _T("Cannot read CG"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }
            continue;

        case 0x0045:    // Is menu on screen
            g_bMenuOnScreen = GET_DWORD(g_pvScriptBuffer, g_iCurrPosInScript) != 0;
            g_iCurrPosInScript += 4;
            continue;

        case 0x0046:    // Display BG command, clear other parts
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);


            if (LoadAndBlitPicToBuf2(0, eRightAndBottomFromScreen_BG, g_ptScriptName)){
                MessageBox(hWnd, _T("Cannot read CG"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }

            strcpy_s(g_sGameState.pcBgPictureName, 32, pcText);
            strcpy_s(g_sGameState.pcFgPictureName, 32, " ");
            strcpy_s(g_sGameState.pcCharacter1Name, 32, " ");
            strcpy_s(g_sGameState.pcCharacter2Name, 32, " ");
            strcpy_s(g_sGameState.pcFgOverlayName, 32, " ");

            CopyPictureWithTranspColor(eRightAndBottomFromScreen_BG, 0, 0);
            continue;

        case 0x0047:    // Load and display CG command
        case 0x0048:
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            if (LoadAndBlitPicToBuf2(0, eBottomFromScreen_Overlay, g_ptScriptName))
            {
                MessageBox(hWnd, _T("Cannot read CG"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }
            CopyPictureWithTranspColor(eBottomFromScreen_Overlay, 0, 0);

            strcpy_s(g_sGameState.pcFgPictureName, pcText);
            strcpy_s(g_sGameState.pcCharacter1Name, " ");
            strcpy_s(g_sGameState.pcCharacter2Name, " ");
            strcpy_s(g_sGameState.pcFgOverlayName, " ");
            continue;

        case 0x0049:    // Set required action to ?
            g_iRequiredAction = eReserved;
            continue;

        case 0x004A:    // Blit picture with specified effect from bottom of screen (show clear background)
            sFlagIdx = GET_WORD(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += 2;

            g_iOriginX = 32;
            g_iOriginY = 8;
            g_sPicWidth = 576;
            g_sPicHeight = 376;
            CopyPictureWithTranspColor(eBottomFromScreen_Overlay, 0, 0);
            BlitPicWithEffects(sFlagIdx);

            strcpy_s(g_sGameState.pcCharacter1Name, 32, " ");
            strcpy_s(g_sGameState.pcCharacter2Name, 32, " ");
            strcpy_s(g_sGameState.pcFgOverlayName, 32, " ");
            continue;

        case 0x004B:    // Load and show personage
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            g_iOriginX = 32;
            g_iOriginY = 8;
            g_sPicWidth = 576;
            g_sPicHeight = 376;
            CopyPictureWithTranspColor(eBottomFromScreen_Overlay, 0, 0);

            strcpy_s(g_sGameState.pcCharacter1Name, 32, " ");
            strcpy_s(g_sGameState.pcCharacter2Name, 32, " ");
            strcpy_s(g_sGameState.pcFgOverlayName, 32, " ");

            g_sPicWidth = 288;
            g_sPicHeight = 376;
            if (LoadPicWithTransparency(176, 8, g_ptScriptName)){
                MessageBox(hWnd, _T("Cannot read CG"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }
            strcpy_s(g_sGameState.pcCharacter1Name, 32, pcText);
            g_iOriginY = 8;
            g_iOriginX = 320 - (g_sPicWidth / 2);
            g_sPicWidth = 480;
            g_sPicHeight = 376;
            continue;

        case 0x004C:    // Show two personages on screen
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptScriptName, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            g_iOriginX = 32;
            g_iOriginY = 8;
            g_sPicWidth = 576;
            g_sPicHeight = 376;
            CopyPictureWithTranspColor(eBottomFromScreen_Overlay, 0, 0);

            strcpy_s(g_sGameState.pcCharacter1Name, 32, " ");
            strcpy_s(g_sGameState.pcCharacter2Name, 32, " ");
            strcpy_s(g_sGameState.pcFgOverlayName, 32, " ");

            g_sPicWidth = 288;
            g_sPicHeight = 376;
            if (LoadPicWithTransparency(32, 8, g_ptScriptName)){
                MessageBox(hWnd, _T("Cannot read CG"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }
            strcpy_s(g_sGameState.pcCharacter1Name, 32, pcText);

            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            ConvertMBCSToUni(pcText, g_ptCvtString, CVT_BUF_SIZE);
            _tcsncpy_s(g_ptPersonageOnScr2File, MAX_PATH, g_ptCvtString, CVT_BUF_SIZE);

            g_sPicWidth = 288;
            g_sPicHeight = 376;
            if (LoadPicWithTransparency(320, 8, g_ptPersonageOnScr2File)){
                MessageBox(hWnd, _T("Cannot read CG"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                ChangeDisplaySettings(NULL, 0);
                PostMessage(hWnd, WM_DESTROY, 0, 0);
                ExitThread(0);
            }
            strcpy_s(g_sGameState.pcCharacter2Name, 32, pcText);

            g_iOriginX = 32;
            g_iOriginY = 8;
            g_sPicWidth = 576;
            g_sPicHeight = 376;
            continue;

        case 0x004D:    // Show a light flash, the end of game
            LoadAndBlitPicToBuf2(0, eMainScreen, _T("I_20A"));
            BlitFromBuffersToScreen(0, 0, g_sPicWidth, g_sPicHeight, 0, 0);
            Sleep(50);
            LoadAndBlitPicToBuf2(0, eMainScreen, _T("I_20B"));
            BlitFromBuffersToScreen(0, 0, g_sPicWidth, g_sPicHeight, 0, 0);
            Sleep(50);
            LoadAndBlitPicToBuf2(0, eMainScreen, _T("I_20C"));
            BlitFromBuffersToScreen(0, 0, g_sPicWidth, g_sPicHeight, 0, 0);
            Sleep(50);
            LoadAndBlitPicToBuf2(0, eMainScreen, _T("I_20D"));
            BlitFromBuffersToScreen(0, 0, g_sPicWidth, g_sPicHeight, 0, 0);
            Sleep(50);
            LoadAndBlitPicToBuf2(0, eMainScreen, _T("I_20E"));
            BlitFromBuffersToScreen(0, 0, g_sPicWidth, g_sPicHeight, 0, 0);
            Sleep(50);
            LoadAndBlitPicToBuf2(0, eMainScreen, _T("I_20F"));
            BlitFromBuffersToScreen(0, 0, g_sPicWidth, g_sPicHeight, 0, 0);

            strcpy_s(g_sGameState.pcCharacter1Name, 32, " ");
            strcpy_s(g_sGameState.pcCharacter2Name, 32, " ");
            strcpy_s(g_sGameState.pcFgOverlayName, 32, " ");
            strcpy_s(g_sGameState.pcFgPictureName, 32, "I_20F");
            g_sSysFile.pbGameEventsAchieved[1] = true;  // Flash event achieved
            continue;

        case 0x004E:    // Slide image I_101A over main image I_101
            strcpy_s(g_sGameState.pcCharacter1Name, 32, " ");
            strcpy_s(g_sGameState.pcCharacter2Name, 32, " ");
            strcpy_s(g_sGameState.pcFgOverlayName, 32, " ");

            LoadAndBlitPicToBuf2(0, eMainScreen, _T("I_101A"));
            BitBlt(g_hDcBuffer2, 32, 384, 576, 376, g_hDcBuffer2, 32, 8, SRCCOPY);
            LoadAndBlitPicToBuf2(0, eMainScreen, _T("I_101"));

            strcpy_s(g_sGameState.pcFgPictureName, 32, "I_101A");
            for (iIdx1 = 8; iIdx1 <= 384; iIdx1 += 2){
                BitBlt(g_hDcBuffer1, 32, 8, 576, 376, g_hDcBuffer2, 32, iIdx1, SRCCOPY);
                BitBlt(g_hMainDc, 32, 8, 576, 376, g_hDcBuffer1, 32, 8, SRCCOPY);
                Sleep(10);
            }
            BitBlt(g_hDcBuffer2, 32, 8, 576, 376, g_hDcBuffer2, 32, 384, SRCCOPY);
            BitBlt(g_hDcBuffer2, 0, 384, 640, 96, g_hDcBuffer2, 640, 480 + 384, SRCCOPY);
            continue;

        case 0x004F:    // Slide image I_101B over image I_101A
            strcpy_s(g_sGameState.pcCharacter1Name, 32, " ");
            strcpy_s(g_sGameState.pcCharacter2Name, 32, " ");
            strcpy_s(g_sGameState.pcFgOverlayName, 32, " ");

            LoadAndBlitPicToBuf2(0, eMainScreen, _T("I_101A"));
            BitBlt(g_hDcBuffer2, 32, 384, 576, 376, g_hDcBuffer2, 32, 8, SRCCOPY);
            LoadAndBlitPicToBuf2(0, eMainScreen, _T("I_101B"));

            strcpy_s(g_sGameState.pcFgPictureName, 32, "I_101B");
            for (iIdx1 = 384; iIdx1 >= 8; iIdx1 -= 2){
                BitBlt(g_hDcBuffer1, 32, 8, 576, 376, g_hDcBuffer2, 32, iIdx1, SRCCOPY);
                BitBlt(g_hMainDc, 32, 8, 576, 376, g_hDcBuffer1, 32, 8, SRCCOPY);
                Sleep(10);
            }
            BitBlt(g_hDcBuffer2, 0, 384, 640, 96, g_hDcBuffer2, 640, 480 + 384, SRCCOPY);
            continue;

        case 0x0050:    // Set current game time
            pcText = GET_STRING(g_pvScriptBuffer, g_iCurrPosInScript);
            g_iCurrPosInScript += strlen(pcText) + 1;

            /*ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);
            _tcsncpy(g_ptScriptName, ptCvtString, CVT_BUF_SIZE);*/

            strcpy_s(g_sSysFile.pcCurrentGameTime, 12, "           ");

            for (iIdx1 = 0; iIdx1 < 10; iIdx1++){
                if (!pcText[iIdx1]){
                    break;
                }
                g_sSysFile.pcCurrentGameTime[iIdx1] = pcText[iIdx1];
            }
            continue;

        default:
            MessageBox(hWnd, _T("There is an error in the command"), _T("DIVI-DEAD C'sWARE"), MB_OK);
            ChangeDisplaySettings(NULL, 0);
            PostMessage(hWnd, WM_DESTROY, 0, 0);
            ExitThread(0);
        }
    }
}

//----- (00405900) --------------------------------------------------------
void SaveGame(short sSaveFileNumber)
{
    char    pcText[25];
    TCHAR   ptFilePath[MAX_PATH];

    if (sSaveFileNumber <= SAVE_SLOT_COUNT){
        g_sGameState.iPosInScript = g_iNewPosInScript;
        g_sGameState.iSavedPosInScript = g_iSavedPosInScript;
        g_sGameState.dwReserved2 = g_bReserved0001;

        GetLocalTime(&g_sSystemTime);
        wsprintfA(pcText, " %02d/%02d %02d:%02d ", g_sSystemTime.wMonth, g_sSystemTime.wDay, g_sSystemTime.wHour, g_sSystemTime.wMinute);
        strcat_s(pcText, 25, g_sSysFile.pcCurrentGameTime);
        strcpy_s(g_sSysFile.pcSaveNames[sSaveFileNumber], 25, pcText);

        g_sSysFile.bSaveExists[sSaveFileNumber] = 1;
        g_ptSaveFileName[4] = sSaveFileNumber + _T('0');

        _tcsncpy_s(ptFilePath, MAX_PATH, g_ptSaveFolder, MAX_PATH);
        _tcsncat_s(ptFilePath, MAX_PATH, g_ptSaveFileName, MAX_PATH);
        OverwriteFile(ptFilePath, &g_sGameState, sizeof(g_sGameState));

        _tcsncpy_s(ptFilePath, MAX_PATH, g_ptSaveFolder, MAX_PATH);
        _tcsncat_s(ptFilePath, MAX_PATH, _T("SYS.DAT"), MAX_PATH);
        OverwriteFile(ptFilePath, &g_sSysFile, sizeof(g_sSysFile));
    }
    else{
        MessageBox(g_hMainWindow, _T("Cannot save here"), _T("DIVI-DEAD C'sWARE"), MB_OK);
    }
}

//----- (00405A40) --------------------------------------------------------
void LoadSave(short sSaveFileToLoad)
{
    int     iPosX;
    TCHAR   ptFilePath[MAX_PATH];

    if (sSaveFileToLoad > SAVE_SLOT_COUNT){
        MessageBox(g_hMainWindow, _T("Incorrect load position"), _T("DIVI-DEAD C'sWARE"), MB_OK);
        return;
    }

    g_ptSaveFileName[4] = sSaveFileToLoad + _T('0');
    _tcscpy_s(ptFilePath, g_ptSaveFolder);
    _tcsncat_s(ptFilePath, g_ptSaveFileName, _TRUNCATE);

    if (ReadFileIntoBuffer(ptFilePath, g_pvDataBuffer)){
        memcpy(&g_sGameState, g_pvDataBuffer, sizeof(g_sGameState));
    }

    ConvertMBCSToUni(g_sGameState.pcScriptFile, g_ptCvtString, CVT_BUF_SIZE);
    LoadScriptFile(g_ptCvtString);
    g_iCurrPosInScript = g_sGameState.iPosInScript;
    g_iSavedPosInScript = g_sGameState.iSavedPosInScript;
    g_bReserved0001 = g_sGameState.dwReserved2;
    strcpy_s(g_sSysFile.pcCurrentGameTime, "           ");
    strncpy_s(g_sSysFile.pcCurrentGameTime, &g_sSysFile.pcSaveNames[sSaveFileToLoad][13], 12 - 1);

    g_iOriginX = 0;
    g_iOriginY = 0;
    g_sPicWidth = 640;
    g_sPicHeight = 480;

    EraseBGBlitWEffects(eHorizontalBlinds);

    if (g_sGameState.pcBgPictureName[0] != ' '){
        ConvertMBCSToUni(g_sGameState.pcBgPictureName, g_ptCvtString, CVT_BUF_SIZE);
        LoadAndBlitPicToBuf2(0, eRightAndBottomFromScreen_BG, g_ptCvtString);
        CopyPictureWithTranspColor(eRightAndBottomFromScreen_BG, 0, 0);
    }
    if (g_sGameState.pcFgPictureName[0] != ' '){
        ConvertMBCSToUni(g_sGameState.pcFgPictureName, g_ptCvtString, CVT_BUF_SIZE);
        LoadAndBlitPicToBuf2(0, eBottomFromScreen_Overlay, g_ptCvtString);
        CopyPictureWithTranspColor(eBottomFromScreen_Overlay, 0, 0);
    }
    BitBlt(g_hDcBuffer1, 0, 0, 640, 480, g_hDcBuffer2, 0, 0, SRCCOPY);

    if (g_sGameState.pcCharacter1Name[0] != ' '){
        g_sPicWidth = PERSONAGE_SIZE_X;
        g_sPicHeight = PERSONAGE_SIZE_Y;
        if (g_sGameState.pcCharacter2Name[0] == ' '){
            iPosX = (GAME_SCREEN_SIZE_X - PERSONAGE_SIZE_X) / 2;
        }
        else{
            iPosX = (GAME_SCREEN_SIZE_X - PERSONAGE_SIZE_X * 2) / 2;
        }
        ConvertMBCSToUni(g_sGameState.pcCharacter1Name, g_ptCvtString, CVT_BUF_SIZE);
        LoadPicWithTransparency(iPosX, PERSONAGE_POS_Y, g_ptCvtString);
    }
    if (g_sGameState.pcCharacter2Name[0] != ' '){
        g_sPicWidth = PERSONAGE_SIZE_X;
        g_sPicHeight = PERSONAGE_SIZE_Y;
        ConvertMBCSToUni(g_sGameState.pcCharacter2Name, g_ptCvtString, CVT_BUF_SIZE);
        LoadPicWithTransparency(GAME_SCREEN_SIZE_X / 2, PERSONAGE_POS_Y, g_ptCvtString);
    }
    if (g_sGameState.pcFgOverlayName[0] != ' '){
        ConvertMBCSToUni(g_sGameState.pcFgOverlayName, g_ptCvtString, CVT_BUF_SIZE);
        LoadAndBlitPicToBuf2(0, eRightFromScreen_TranspOverlay, g_ptCvtString);
        CopyPictureWithTranspColor(eRightFromScreen_TranspOverlay, 0, 0);
    }
    if (g_sGameState.pcBgMusicName[0] != ' '){
        ConvertMBCSToUni(g_sGameState.pcBgMusicName, g_ptCvtString, CVT_BUF_SIZE);
        PlayMidiFile(g_ptCvtString, true);
    }

    g_iOriginX = 0;
    g_iOriginY = 0;
    g_sPicWidth = 640;
    g_sPicHeight = 480;

    BlitPicWithEffects(eHorizontalBlinds);
}

//----- (00405CA0) --------------------------------------------------------
short ProcessMenu(int iX, int iY, short sMenuItemsPerColumn, const TCHAR *ptMenuStrings)
{
    long    lOffsetInString;
    short   sResult;

    lOffsetInString = 0;
    g_sMenuLength = 0;

    do{ // Load strings into table
        _tcsncpy_s(g_pptMenuStrings[g_sMenuLength], MAX_MENU_STRING_LEN, &ptMenuStrings[lOffsetInString], MAX_MENU_STRING_LEN - 1);
        g_sMenuLength++;
        lOffsetInString += _tcslen(&ptMenuStrings[lOffsetInString]) + 1;
    } while (ptMenuStrings[lOffsetInString]);

    g_crMenuFGColor = RGB(255, 255, 255);
    g_crMenuBGColor = RGB(0, 0, 0);
    g_bMenuOnScreen = true;
    g_iMenuLineHeight = 22;
    sResult = ShowMenuAndWaitForSel(iX, iY, sMenuItemsPerColumn);
    g_bMenuOnScreen = false;

    return sResult;
}

//----- (00405D40) --------------------------------------------------------
short ShowMenuAndWaitForSel(int iX, int iY, short sMenuItemsPerColumn)
{
    short   sMenuIdx;
    HBRUSH  hWhiteBrush;
    short   sMenuItemToSelect;
    short   sSelectedMenuItem;
    RECT    sRect;

    SetTextColor(g_hMainDc, g_crMenuFGColor);
    SetTextColor(g_hDcBuffer1, g_crMenuFGColor);

    if (g_sMenuLength){ // Print menu strings
        for (sMenuIdx = 0; sMenuIdx < g_sMenuLength; sMenuIdx++){
            g_piMenuStringLengths[sMenuIdx] =
                OutputString(g_hDcBuffer1, g_hMainDc,
                iX + 220 * (sMenuIdx / sMenuItemsPerColumn),
                iY + g_iMenuLineHeight * (sMenuIdx % sMenuItemsPerColumn),
                50, g_pptMenuStrings[sMenuIdx], 0);
        }
    }

    g_bResetMenuSelection = false;
    g_bScreenUpdatedOrMouseMoved = true;
    sMenuItemToSelect = 0;
    sSelectedMenuItem = -1;
    SetTextColor(g_hMainDc, g_crMenuBGColor);

    do{
        if (g_bResetMenuSelection){
            g_bResetMenuSelection = false;
            sSelectedMenuItem = -1;
        }
        if (!g_bScreenUpdatedOrMouseMoved){                 // Process keyboard events
            if (CheckKeysToProc(UP_MASK)){
                --sMenuItemToSelect;
            }
            if (CheckKeysToProc(DOWN_MASK)){
                ++sMenuItemToSelect;
            }
            if (CheckKeysToProc(RIGHT_MASK)){
                sMenuItemToSelect += sMenuItemsPerColumn;
            }
            if (CheckKeysToProc(LEFT_MASK)){
                sMenuItemToSelect -= sMenuItemsPerColumn;
            }
            if (!g_bMenuOnScreen && g_bKeyCommand2){
                Sleep(10);
                break;
            }
            if (CheckKeysToProc(ENTER_MASK)){
                Sleep(10);
                break;
            }
            if (g_bMenuOnScreen && CheckKeysToProc(ESCAPE_MASK)){
                sMenuItemToSelect = -1;
                Sleep(10);
                break;
            }
            if (sMenuItemToSelect < 0){
                sMenuItemToSelect = g_sMenuLength - 1;
            }
        }
        else                                                    // Process mouse move events
        {
            g_bScreenUpdatedOrMouseMoved = false;
            if (g_sMenuLength > 0){
                for (sMenuIdx = 0; sMenuIdx < g_sMenuLength; sMenuIdx++){

                    sRect.left = iX + 220 * (sMenuIdx / sMenuItemsPerColumn);
                    sRect.right = sRect.left + (FONT_WIDHT - 1) * g_piMenuStringLengths[sMenuIdx];
                    sRect.bottom = iY + g_iMenuLineHeight * (sMenuIdx % sMenuItemsPerColumn);
                    sRect.top = sRect.bottom + g_iMenuLineHeight;

                    if (g_iMousePosY > sRect.bottom && g_iMousePosY < sRect.top && g_iMousePosX > sRect.left && g_iMousePosX < sRect.right){
                        sMenuItemToSelect = sMenuIdx;
                    }
                }
            }
        }
        if (sMenuItemToSelect >= g_sMenuLength){
            sMenuItemToSelect = 0;
        }
        if (sMenuItemToSelect != sSelectedMenuItem){
            if (sSelectedMenuItem != -1){   // Restore image under previously selected item
                sRect.left = iX + 220 * (sSelectedMenuItem / sMenuItemsPerColumn);
                sRect.right = FONT_WIDHT * g_piMenuStringLengths[sSelectedMenuItem];    // Width
                sRect.bottom = iY + g_iMenuLineHeight * (sSelectedMenuItem % sMenuItemsPerColumn);
                sRect.top = g_iMenuLineHeight;  // Height

                BitBlt(g_hMainDc, sRect.left, sRect.bottom, sRect.right, sRect.top, g_hDcBuffer1, sRect.left, sRect.bottom, SRCCOPY);
            }

            sSelectedMenuItem = sMenuItemToSelect;

            sRect.left = iX + 220 * (sSelectedMenuItem / sMenuItemsPerColumn);
            sRect.right = sRect.left + (FONT_WIDHT - 1) * g_piMenuStringLengths[sSelectedMenuItem];
            sRect.top = iY + g_iMenuLineHeight * (sSelectedMenuItem % sMenuItemsPerColumn);
            sRect.bottom = sRect.top + g_iMenuLineHeight;

            hWhiteBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
            FillRect(g_hMainDc, &sRect, hWhiteBrush);
            OutputString(g_hMainDc, g_hMainDc, sRect.left, sRect.top, 50, g_pptMenuStrings[sSelectedMenuItem], 0);
        }
        Sleep(10);
    } while (1);

    return sMenuItemToSelect;
}

//----- (004061A0) --------------------------------------------------------
short SelectItemInGraphicalMode()
{
    RECT    sRect;
    POINT   sPoint;
    short   sMenuIndex;
    short   sMenuItemToSelect;
    short   sSelectedMenuItem;

    g_bResetMenuSelection = false;
    g_bTrackingMouse = true;
    g_bScreenUpdatedOrMouseMoved = true;

    sMenuItemToSelect = -1;
    sSelectedMenuItem = -1;

    do{
        if (g_bResetMenuSelection){
            g_bResetMenuSelection = false;
            sSelectedMenuItem = -1;
        }
        if (!g_bScreenUpdatedOrMouseMoved){                 // Process keyboard events
            if (CheckKeysToProc(ENTER_MASK) && sMenuItemToSelect != -1){
                if (sSelectedMenuItem != -1){
                    sRect.left = g_sCGRects[sSelectedMenuItem].sLeft;
                    sRect.right = g_sCGRects[sSelectedMenuItem].sRight - g_sCGRects[sSelectedMenuItem].sLeft + 1;   // Width
                    sRect.top = g_sCGRects[sSelectedMenuItem].sTop;
                    sRect.bottom = g_sCGRects[sSelectedMenuItem].sBottom - g_sCGRects[sSelectedMenuItem].sTop + 1;  // Height

                    BitBlt(g_hDcBuffer1, sRect.left, sRect.top, sRect.right, sRect.bottom, g_hDcBuffer2, sRect.left, sRect.top, SRCCOPY);
                    BitBlt(g_hMainDc, sRect.left, sRect.top, sRect.right, sRect.bottom, g_hDcBuffer2, sRect.left, sRect.top, SRCCOPY);
                }
                Sleep(10);
                break;
            }
            if (CheckKeysToProc(ESCAPE_MASK)){
                sMenuItemToSelect = -1;
                Sleep(10);
                break;
            }
            if (CheckKeysToProc(UP_MASK)){
                GetCursorPos(&sPoint);
                if (!g_bShiftPressed){
                    sPoint.y -= 5;
                }
                else{
                    sPoint.y -= 15;
                }
                SetCursorPos(sPoint.x, sPoint.y);
                g_bScreenUpdatedOrMouseMoved = true;
            }
            if (CheckKeysToProc(DOWN_MASK)){
                GetCursorPos(&sPoint);
                if (g_bShiftPressed){
                    sPoint.y += 15;
                }
                else{
                    sPoint.y += 5;
                }
                SetCursorPos(sPoint.x, sPoint.y);
                g_bScreenUpdatedOrMouseMoved = true;
            }
            if (CheckKeysToProc(RIGHT_MASK)){
                GetCursorPos(&sPoint);
                if (g_bShiftPressed){
                    sPoint.x += 15;
                }
                else{
                    sPoint.x += 5;
                }
                SetCursorPos(sPoint.x, sPoint.y);
                g_bScreenUpdatedOrMouseMoved = true;
            }
            if (CheckKeysToProc(LEFT_MASK)){
                GetCursorPos(&sPoint);
                if (g_bShiftPressed){
                    sPoint.x -= 15;
                }
                else{
                    sPoint.x -= 5;
                }
                SetCursorPos(sPoint.x, sPoint.y);
                g_bScreenUpdatedOrMouseMoved = true;
            }
        }
        else{                   // Process mouse events
            g_bScreenUpdatedOrMouseMoved = false;
            sMenuItemToSelect = -1;
            if (g_sMenuLength > 0){ // Do hit test
                for (sMenuIndex = 0; sMenuIndex < g_sMenuLength; sMenuIndex++){
                    if (g_iMousePosX >= g_sCGRects[sMenuIndex].sLeft  && g_iMousePosX <= g_sCGRects[sMenuIndex].sRight
                        && g_iMousePosY >= g_sCGRects[sMenuIndex].sTop && g_iMousePosY <= g_sCGRects[sMenuIndex].sBottom){
                        sMenuItemToSelect = sMenuIndex;
                        break;
                    }
                }
            }
        }
        if (sMenuItemToSelect != sSelectedMenuItem){
            if (sSelectedMenuItem != -1){
                sRect.left = g_sCGRects[sSelectedMenuItem].sLeft;
                sRect.right = g_sCGRects[sSelectedMenuItem].sRight - g_sCGRects[sSelectedMenuItem].sLeft + 1;   // Width
                sRect.top = g_sCGRects[sSelectedMenuItem].sTop;
                sRect.bottom = g_sCGRects[sSelectedMenuItem].sBottom - g_sCGRects[sSelectedMenuItem].sTop + 1;  // Height

                BitBlt(g_hDcBuffer1, sRect.left, sRect.top, sRect.right, sRect.bottom, g_hDcBuffer2, sRect.left + g_psPicOriginPositions[eMainScreen].x, sRect.top + g_psPicOriginPositions[eMainScreen].y, SRCCOPY);
                BitBlt(g_hMainDc, sRect.left, sRect.top, sRect.right, sRect.bottom, g_hDcBuffer2, sRect.left + g_psPicOriginPositions[eMainScreen].x, sRect.top + g_psPicOriginPositions[eMainScreen].y, SRCCOPY);
            }

            sSelectedMenuItem = sMenuItemToSelect;

            if (sSelectedMenuItem != -1){
                sRect.left = g_sCGRects[sSelectedMenuItem].sLeft;
                sRect.right = g_sCGRects[sSelectedMenuItem].sRight - g_sCGRects[sSelectedMenuItem].sLeft + 1;   // Width
                sRect.top = g_sCGRects[sSelectedMenuItem].sTop;
                sRect.bottom = g_sCGRects[sSelectedMenuItem].sBottom - g_sCGRects[sSelectedMenuItem].sTop + 1;  // Height

                BitBlt(g_hDcBuffer1, sRect.left, sRect.top, sRect.right, sRect.bottom, g_hDcBuffer2, sRect.left + g_psPicOriginPositions[eRightFromScreen_TranspOverlay].x, sRect.top + g_psPicOriginPositions[eRightFromScreen_TranspOverlay].y, SRCCOPY);
                BitBlt(g_hMainDc, sRect.left, sRect.top, sRect.right, sRect.bottom, g_hDcBuffer2, sRect.left + g_psPicOriginPositions[eRightFromScreen_TranspOverlay].x, sRect.top + g_psPicOriginPositions[eRightFromScreen_TranspOverlay].y, SRCCOPY);
            }
        }
        Sleep(10);
    } while (1);

    g_bTrackingMouse = false;

    return sMenuItemToSelect;
}

//----- (004066A0) --------------------------------------------------------
// Return: false - success; true - error
bool PlayMidiFile(const TCHAR *cptFileNameToPlay, bool bLoopMidiPlayback)
{
    if (!cptFileNameToPlay){
        return true;
    }

    if (g_uMIDIDevsCnt > 0 && g_bBgmEnabled){
        return MIDIPlaybackCtrl(g_hMainWindow, eMIDI_OpenAndPlay, cptFileNameToPlay, bLoopMidiPlayback) != 0;
    }

    return false;
}

//----- (004066E0) --------------------------------------------------------
// Return: false - success; true - error
bool StopMidiPlayback()
{
    if (g_uMIDIDevsCnt == 0 || !g_bBgmEnabled){
        return false;
    }

    return MIDIPlaybackCtrl(g_hMainWindow, eMIDI_Stop, NULL, false) != 0;
}

//----- (00406710) --------------------------------------------------------
// Return: false - success; true - error
bool WavePlaybackCtrl(const TCHAR *cptFileName, int iCommand)
{

    TCHAR ptStr[MAX_PATH];

    switch (iCommand){
    case eWAVE_LoadFormArcAndPlay:
        if (!cptFileName){
            return true;
        }

        _tcsncpy_s(ptStr, MAX_PATH, cptFileName, MAX_PATH);
        MakeCapsAndCatExt(ptStr, _T("WAV"));

        if (!LoadWVFile(g_ptFullFileName, g_pvSoundBuffer)){
            return true;
        }

        PlaySound((LPCTSTR)g_pvSoundBuffer, NULL, SND_MEMORY | SND_ASYNC);
        break;

    case eWAVE_Stop:
        PlaySound(NULL, NULL, SND_SYNC);
        break;

    case eWAVE_StoreFilename:
        if (!cptFileName){
            return true;
        }

        _tcsncpy_s(ptStr, MAX_PATH, g_ptWorkingDir, MAX_PATH);
        _tcsncat_s(ptStr, MAX_PATH, _T("WAVE\\"), MAX_PATH);
        _tcsncat_s(ptStr, MAX_PATH, cptFileName, MAX_PATH);
        MakeCapsAndCatExt(ptStr, _T("WAV"));
        break;

    default:
        return true;
    }

    return false;
}

//----- (00406820) --------------------------------------------------------
int FindPicOrigin(short sWidth, short sHeight)
{
    int iIdx;
    OFFSET_STRUCT psPicSizes[7] = {
        0, 0,
        640, 480,
        576, 376,
        512, 320,
        640, 304,
        0, 0,
        -1, -1
    };

    OFFSET_STRUCT psPicOrigins[7] = {
        0, 0,
        0, 0,
        32, 8,
        64, 32,
        0, 64,
        0, 0,
        -1, -1
    };


    for (iIdx = 0; psPicSizes[iIdx].x != -1; iIdx++){
        if (sWidth == psPicSizes[iIdx].x && sHeight == psPicSizes[iIdx].y){
            g_iOriginX = psPicOrigins[iIdx].x;
            g_iOriginY = psPicOrigins[iIdx].y;
            break;
        }
    }

    if (psPicSizes[iIdx].x == -1){
        g_iOriginX = 0;
        g_iOriginY = 0;
        return 0;
    }

    return iIdx;
}

//----- (00406960) --------------------------------------------------------
// Return: false - success; true - error
bool LoadAndBlitPicToBuf2(int a1, unsigned short iOriginPos, const TCHAR *cptFileName)
{
    BITMAP_FILE *psBF;

    if (!cptFileName){
        return true;
    }

    ConvertUniToMBCS(cptFileName, g_pcCvtString, CVT_BUF_SIZE);
    MarkCGAsSeen(g_pcCvtString);
    MakeCapsAndCatExt(cptFileName, _T("BMP"));

    if (!FindAndReadFileInSG(g_ptFullFileName, g_pvDataBuffer)){
        return true;
    }

    Sleep(5);

    if (UnpackData(((BYTE*)g_pvDataBuffer + MAX_BUF_SIZE), g_pvDataBuffer, MAX_BUF_SIZE)){
        psBF = (BITMAP_FILE *)((BYTE*)g_pvDataBuffer + MAX_BUF_SIZE);
    }
    else{
        psBF = (BITMAP_FILE *)g_pvDataBuffer;
    }

    if (psBF->bmfh.bfType != 0x4D42 || (psBF->bmi.bmiHeader.biSize != 12 && psBF->bmi.bmiHeader.biSize < 16)){
        return true;
    }

    g_sPicWidth = (short)psBF->bmi.bmiHeader.biWidth;   // Used
    g_sPicHeight = (short)psBF->bmi.bmiHeader.biHeight;
    g_sPicOffsetNum = FindPicOrigin((short)psBF->bmi.bmiHeader.biWidth, (short)psBF->bmi.bmiHeader.biHeight);

    StretchDIBits(
        g_hDcBuffer2,
        g_iOriginX + g_psPicOriginPositions[iOriginPos].x,
        g_iOriginY + g_psPicOriginPositions[iOriginPos].y,
        psBF->bmi.bmiHeader.biWidth,
        psBF->bmi.bmiHeader.biHeight,
        0,
        0,
        psBF->bmi.bmiHeader.biWidth,
        psBF->bmi.bmiHeader.biHeight,
        (BYTE*)psBF + psBF->bmfh.bfOffBits,
        &(psBF->bmi),
        0,
        SRCCOPY);

    return false;
}

//----- (00406AE0) --------------------------------------------------------
// Return: false - success; true - error
bool LoadPicWithTransparency(int x, int y, const TCHAR *cptCGName)
{
    TCHAR ptStr[MAX_ARCH_NAME_SIZE];
    TCHAR *ptFoundChar;

    if (LoadAndBlitPicToBuf2(0, eRightFromScreen_TranspOverlay, cptCGName)){
        return true;
    }

    _tcsncpy_s(ptStr, cptCGName, _TRUNCATE);
    ptFoundChar = _tcschr(ptStr, _T('_'));
    if (ptFoundChar == NULL){
        return true;
    }

    ptFoundChar[0] = 0;
    _tcsncat_s(ptStr, MAX_ARCH_NAME_SIZE, _T("_0.BMP"), _TRUNCATE);
    if (LoadAndBlitPicToBuf2(0, eMainScreen, ptStr)){
        return true;
    }

    OverlayPicWithTransparency(x, y);
    return false;
}

//----- (00406B70) --------------------------------------------------------
// Return: false - success; true - error
bool OverlayPicWithTransparency(int iX, int iY)
{
    HBITMAP     hTempBitmap;
    HBITMAP     hBitmap_Old;
    HDC         hTempDc;
    BYTE        *pbTempBitmapBuffer;
    void        *pvBuf;
    int         iYIdx;
    int         iXYIdx;
    BYTE        bInvertedMaskByte;
    BYTE        bDirectMaskByte;
    BITMAPINFO  bmi;

    bmi.bmiHeader.biSize = 40;
    bmi.bmiHeader.biWidth = PERSONAGE_SIZE_X;
    bmi.bmiHeader.biHeight = -8;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;

    hTempBitmap = CreateDIBSection(g_hDcBuffer1, &bmi, DIB_RGB_COLORS, &pvBuf, NULL, 0);
    if (hTempBitmap == NULL){
        return true;
    }

    hTempDc = CreateCompatibleDC(g_hDcBuffer1);
    hBitmap_Old = (HBITMAP)SelectObject(hTempDc, hTempBitmap);

    for (iYIdx = 0; iYIdx < (PERSONAGE_SIZE_Y / 2); iYIdx++)
    {   // 288 x 8
        BitBlt(hTempDc, 0, 0, PERSONAGE_SIZE_X, 2, g_hDcBuffer1, iX, iY + 2 * iYIdx, SRCCOPY); // Destination
        BitBlt(hTempDc, 0, 2, PERSONAGE_SIZE_X, 2, g_hDcBuffer2, g_psPicOriginPositions[eRightFromScreen_TranspOverlay].x, g_psPicOriginPositions[eRightFromScreen_TranspOverlay].y + 2 * iYIdx, SRCCOPY); // Source
        BitBlt(hTempDc, 0, 4, PERSONAGE_SIZE_X, 2, g_hDcBuffer2, g_psPicOriginPositions[eMainScreen].x, g_psPicOriginPositions[eMainScreen].y + 2 * iYIdx, SRCCOPY);               // Mask copy
        BitBlt(hTempDc, 0, 6, PERSONAGE_SIZE_X, 2, g_hDcBuffer2, g_psPicOriginPositions[eMainScreen].x, g_psPicOriginPositions[eMainScreen].y + 2 * iYIdx, NOTSRCCOPY);            // Mask inverted

        pbTempBitmapBuffer = (BYTE *)pvBuf;

        for (iXYIdx = 0; iXYIdx < PERSONAGE_SIZE_X * 2; iXYIdx++)
        {
            bDirectMaskByte = pbTempBitmapBuffer[(PERSONAGE_SIZE_X * 4 * 3)];   // Mask copy
            bInvertedMaskByte = pbTempBitmapBuffer[(PERSONAGE_SIZE_X * 6 * 3)]; // Mask inverted
            //      v---Destination                     Source----v                                                         Destination----v
            *pbTempBitmapBuffer = (bDirectMaskByte * pbTempBitmapBuffer[PERSONAGE_SIZE_X * 2 * 3]) / 255 + (bInvertedMaskByte * pbTempBitmapBuffer[PERSONAGE_SIZE_X * 0 * 3]) / 255;
            pbTempBitmapBuffer++;
            *pbTempBitmapBuffer = (bDirectMaskByte * pbTempBitmapBuffer[PERSONAGE_SIZE_X * 2 * 3]) / 255 + (bInvertedMaskByte * pbTempBitmapBuffer[PERSONAGE_SIZE_X * 0 * 3]) / 255;
            pbTempBitmapBuffer++;
            *pbTempBitmapBuffer = (bDirectMaskByte * pbTempBitmapBuffer[PERSONAGE_SIZE_X * 2 * 3]) / 255 + (bInvertedMaskByte * pbTempBitmapBuffer[PERSONAGE_SIZE_X * 0 * 3]) / 255;
            pbTempBitmapBuffer++;
        }

        BitBlt(g_hDcBuffer1, iX, iY + 2 * iYIdx, PERSONAGE_SIZE_X, 2, hTempDc, 0, 0, SRCCOPY);
    }
    DeleteObject(SelectObject(hTempDc, hBitmap_Old));
    DeleteDC(hTempDc);
    Sleep(10);
    BitBlt(g_hDcBuffer2, 0, 0, 640, 480, g_hDcBuffer1, 0, 0, SRCCOPY);

    return false;
}

//----- (00406E00) --------------------------------------------------------
void CopyPictureWithTranspColor(unsigned short TransferSource, unsigned short xOffs, unsigned short yOffs)
{
    COLORREF crTranspColor;

    switch (TransferSource){
    case eRightFromScreen_TranspOverlay:
        crTranspColor = GetPixel(g_hDcBuffer2, g_iOriginX + g_psPicOriginPositions[TransferSource].x, g_iOriginY + g_psPicOriginPositions[TransferSource].y);

        BitBltWithTranspColor(
            g_hDcBuffer2,
            g_iOriginX + xOffs, g_iOriginY + yOffs, g_sPicWidth, g_sPicHeight,
            g_hDcBuffer2,
            g_iOriginX + g_psPicOriginPositions[TransferSource].x, g_iOriginY + g_psPicOriginPositions[TransferSource].y,
            crTranspColor);
        break;

    case eBottomFromScreen_Overlay:
    case eRightAndBottomFromScreen_BG:
        BitBlt(g_hDcBuffer2,
            g_iOriginX + xOffs, g_iOriginY + yOffs, g_sPicWidth, g_sPicHeight,
            g_hDcBuffer2,
            g_iOriginX + g_psPicOriginPositions[TransferSource].x, g_iOriginY + g_psPicOriginPositions[TransferSource].y,
            SRCCOPY);
        break;

    default:
        break;
    }
}

//----- (00406F30) --------------------------------------------------------
void EraseBGBlitWEffects(short sEffect)
{
    g_bClearScreen = true;
    BlitPicWithEffects(sEffect);
    g_bClearScreen = false;
}

//----- (00406F60) --------------------------------------------------------
void BlitPicWithEffects(short sEffect)
{
    int iYIdx2;
    int iYIdx1;
    unsigned short usIdx1;
    unsigned short usIdx2;
    unsigned short usIdx3;
    unsigned short usBlockIdx;
    unsigned short iXBlockCount;
    unsigned short iYBlockCount;
    int iStepCount;
    unsigned long ulPicSegSize;
    unsigned long ulIdx4;
    int iYPos;
    int iTopIndex;
    int iBottomIndex;
    int iXpos;
    unsigned short pusIdxBuf[4800];

    if (g_bShowingFullScrMenu){
        g_bShowingFullScrMenu = false;
        SetMenu(g_hMainWindow, NULL);
        SetupOrigin(g_hMainWindow, g_hMainDc);
    }

    g_bGraphicEffectsRunning = true;

    switch (sEffect){
    case eNoEffect: // Blit with no effects
        BlitFromBuffersToScreen(0, 0, g_sPicWidth, g_sPicHeight, 0, 0);
        break;

    case eOrderedBlocks:    // Blit with effect: interleaved 8 pixel blocks appearing from left to right
        iXBlockCount = g_sPicWidth / 8;
        iYBlockCount = g_sPicHeight / 8;
        iStepCount = iYBlockCount * iXBlockCount;
        iYIdx2 = iYBlockCount;

        if (iStepCount <= 4800){
            usIdx2 = 0;
            ulPicSegSize = iStepCount / 17;

            for (usIdx1 = 0; usIdx1 <= 17; usIdx1++){
                if (ulPicSegSize > 0){
                    usIdx3 = usIdx1;
                    for (ulIdx4 = 0; ulIdx4 < ulPicSegSize + 1; ulIdx4++){
                        if (usIdx2 < iStepCount && usIdx3 < iStepCount){
                            pusIdxBuf[usIdx2] = usIdx3;
                            ++usIdx2;
                        }
                        usIdx3 += 17;
                    }
                }
            }

            iYIdx2 = 0;

            if (iStepCount > 0){
                while (iYIdx2 < iStepCount){
                    if (!(iYIdx2 % ulPicSegSize)){
                        Sleep(EFFECTS_DELAY);
                    }

                    usBlockIdx = pusIdxBuf[iYIdx2];
                    BlitFromBuffersToScreen(8 * (usBlockIdx % iXBlockCount), 8 * (usBlockIdx / iXBlockCount), 8, 8, 8 * (usBlockIdx % iXBlockCount), 8 * (usBlockIdx / iXBlockCount));
                    iYIdx2++;
                }
            }
            usIdx1 = 0;
        }
        break;

    case eHorizontalBlinds: // Blit with effect: horizontal blinds rolling down by 1 pixel in 16 pixel steps
        iStepCount = (g_sPicHeight / 16) + 16;
        if (iStepCount > 0){
            for (iYIdx1 = 0; iYIdx1 <= iStepCount; iYIdx1++){
                Sleep(20);
                for (iYIdx2 = 0; iYIdx2 <= iYIdx1; iYIdx2++){
                    iYPos = iYIdx1 + 16 * iYIdx2;
                    if (iYPos < g_sPicHeight){
                        BlitFromBuffersToScreen(0, iYPos, g_sPicWidth, 1, 0, iYPos);
                    }
                }
            }
        }
        break;

    case eHorizontalBlindsCrossing: // Blit with effect: horizontal blinds rolling from top and bottom by 2 pixels
        iTopIndex = 0;
        iBottomIndex = g_sPicHeight + 2;
        iYIdx1 = g_sPicHeight / 4 + 1;

        for (iYIdx2 = 0; iYIdx2 < iYIdx1; iYIdx2++){
            if (!(iYIdx2 % 2)){
                Sleep(EFFECTS_DELAY);
            }
            if (iTopIndex < g_sPicHeight){
                BlitFromBuffersToScreen(0, iTopIndex, g_sPicWidth, 2, 0, iTopIndex);
            }
            if (iBottomIndex < g_sPicHeight){
                BlitFromBuffersToScreen(0, iBottomIndex, g_sPicWidth, 2, 0, iBottomIndex);
            }
            iBottomIndex -= 4;
            iTopIndex += 4;
        }
        break;

    case eVerticalBlinds:   // Blit with effect: vertical blinds rolling from left to right by 1 pixel in 8 pixel steps
        iStepCount = g_sPicWidth / 8;
        for (iYIdx1 = 0; iYIdx1 < 8; iYIdx1++){
            for (iYIdx2 = 0; iYIdx2 < iStepCount; iYIdx2++){
                iXpos = iYIdx1 + 8 * iYIdx2;
                if (iXpos < g_sPicWidth){
                    //BitBlt(g_hDCBuffer1, g_iOriginX + iXpos, g_iOriginY, 1, g_sPicHeight, g_hDCBuffer2, g_iOriginX + iXpos, g_iOriginY, SRCCOPY);
                    BlitFromBuffersToScreen(iXpos, 0, 1, g_sPicHeight, iXpos, 0);
                }
            }
            //BitBlt(g_hMainDC, g_iOriginX, g_iOriginY, g_sPicWidth, g_sPicHeight, g_hDCBuffer1, g_iOriginX, g_iOriginY, SRCCOPY);
            BlitFromBuffersToScreen(0, 0, g_sPicWidth, g_sPicHeight, 0, 0);
            Sleep(EFFECTS_DELAY);
        }
        break;

    default:
        BlitFromBuffersToScreen(0, 0, g_sPicWidth, g_sPicHeight, 0, 0);
        break;
    }

    g_bGraphicEffectsRunning = false;
}

//----- (00407420) --------------------------------------------------------
BOOL BlitFromBuffersToScreen(int iDstX, int iDstY, int iCX, int iCY, int iSrcX, int iSrcY)
{
    DWORD iRop;

    if (g_bClearScreen)
        iRop = BLACKNESS;
    else
        iRop = SRCCOPY;

    BitBlt(g_hDcBuffer1, g_iOriginX + iDstX, g_iOriginY + iDstY, iCX, iCY, g_hDcBuffer2, g_iOriginX + iSrcX, g_iOriginY + iSrcY, iRop);
    return BitBlt(g_hMainDc, g_iOriginX + iDstX, g_iOriginY + iDstY, iCX, iCY, g_hDcBuffer1, g_iOriginX + iSrcX, g_iOriginY + iSrcY, iRop);
}

//----- (00407540) --------------------------------------------------------
BOOL RestoreImage(int x, int y, int cx, int cy, int x1, int y1)
{
    BitBlt(g_hDcBuffer1, x, y, cx, cy, g_hDcBuffer2, x1, y1, SRCCOPY);
    return BitBlt(g_hMainDc, x, y, cx, cy, g_hDcBuffer1, x1, y1, SRCCOPY);
}

//----- (004075B0) --------------------------------------------------------
int BitBltWithTranspColor(HDC hdcDst, unsigned short usDstX, unsigned short usDstY, unsigned short usCX, unsigned short usCY, HDC hdcSrc, unsigned short usSrcX, unsigned short usSrcY, COLORREF crTranspColor)
{
    HDC hDcSrc_Mask;
    HBITMAP hSrcMaskBitmap;
    HBITMAP hSrcMaskBitmap_Old;
    HDC hDcDst;
    HBITMAP hDstBitmap;
    HBITMAP hDstBitmap_Old;
    HDC hDcSrc;
    HBITMAP hSrcBtimap;
    HBITMAP hSrcBtimap_Old;
    COLORREF crOldBackColor;

    if (crTranspColor)
    {
        hDcSrc_Mask = CreateCompatibleDC(hdcSrc);
        hSrcMaskBitmap = CreateBitmap(usCX, usCY, 1, 1, NULL);
        hSrcMaskBitmap_Old = (HBITMAP)SelectObject(hDcSrc_Mask, hSrcMaskBitmap);
        hDcDst = CreateCompatibleDC(hdcDst);
        hDstBitmap = CreateCompatibleBitmap(hdcDst, usCX, usCY);
        hDstBitmap_Old = (HBITMAP)SelectObject(hDcDst, hDstBitmap);
        hDcSrc = CreateCompatibleDC(hdcSrc);
        hSrcBtimap = CreateCompatibleBitmap(hdcSrc, usCX, usCY);
        hSrcBtimap_Old = (HBITMAP)SelectObject(hDcSrc, hSrcBtimap);
        BitBlt(hDcSrc, 0, 0, usCX, usCY, hdcSrc, usSrcX, usSrcY, SRCCOPY);
        crOldBackColor = SetBkColor(hDcSrc, crTranspColor);
        BitBlt(hDcSrc_Mask, 0, 0, usCX, usCY, hDcSrc, 0, 0, SRCCOPY);
        BitBlt(hDcDst, 0, 0, usCX, usCY, hDcSrc_Mask, 0, 0, SRCCOPY);
        BitBlt(hdcDst, usDstX, usDstY, usCX, usCY, hDcDst, 0, 0, SRCAND);
        PatBlt(hDcDst, 0, 0, usCX, usCY, DSTINVERT);
        BitBlt(hDcSrc, 0, 0, usCX, usCY, hDcDst, 0, 0, SRCAND);
        BitBlt(hdcDst, usDstX, usDstY, usCX, usCY, hDcSrc, 0, 0, SRCPAINT);
        SetBkColor(hDcSrc, crOldBackColor);
        DeleteObject(SelectObject(hDcSrc_Mask, hSrcMaskBitmap_Old));
        DeleteObject(SelectObject(hDcDst, hDstBitmap_Old));
        DeleteObject(SelectObject(hDcSrc, hSrcBtimap_Old));
        DeleteDC(hDcSrc_Mask);
        DeleteDC(hDcDst);
        DeleteDC(hDcSrc);
    }
    else
    {
        hDcSrc_Mask = CreateCompatibleDC(NULL);
        hSrcMaskBitmap = CreateBitmap(usCX, usCY, 1, 1, NULL);
        hSrcMaskBitmap_Old = (HBITMAP)SelectObject(hDcSrc_Mask, hSrcMaskBitmap);
        hDcDst = CreateCompatibleDC(NULL);
        hDstBitmap = CreateCompatibleBitmap(hdcDst, usCX, usCY);
        hDstBitmap_Old = (HBITMAP)SelectObject(hDcDst, hDstBitmap);
        crOldBackColor = SetBkColor(hdcSrc, RGB(0, 0, 0));
        BitBlt(hDcSrc_Mask, 0, 0, usCX, usCY, hdcSrc, usSrcX, usSrcY, SRCCOPY);
        BitBlt(hDcDst, 0, 0, usCX, usCY, hDcSrc_Mask, 0, 0, SRCCOPY);
        BitBlt(hdcDst, usDstX, usDstY, usCX, usCY, hDcDst, 0, 0, SRCAND);
        BitBlt(hdcDst, usDstX, usDstY, usCX, usCY, hdcSrc, usSrcX, usSrcY, SRCPAINT);
        DeleteObject(SelectObject(hDcSrc_Mask, hSrcMaskBitmap_Old));
        DeleteObject(SelectObject(hDcDst, hDstBitmap_Old));
        DeleteDC(hDcSrc_Mask);
        DeleteDC(hDcDst);
        SetBkColor(hdcSrc, crOldBackColor);
    }
    return 0;
}

//----- (004078F0) --------------------------------------------------------
short ProcessInGameMenu()
{
    int iYIdx;
    short result;
    int iXOffset;

    result = 0;

    if (g_uWaveDevsCnt > 0 && g_bSoundEnabled){
        PlaySound(MAKEINTRESOURCE(CHI), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
    }

    if (g_sGameState.pcBgPictureName[5] == 'a' || g_sGameState.pcBgPictureName[5] == 'A'){
        iXOffset = 0;
    }
    else{
        iXOffset = 128;
    }
    BitBlt(g_hDcBuffer1, 16, 400, 64, 64, g_hDcMenuGfx, iXOffset, 80, SRCCOPY);
    BitBlt(g_hMainDc, 16, 400, 64, 64, g_hDcMenuGfx, iXOffset, 80, SRCCOPY);

    while (1)
    {
        BitBltWithTranspColor(g_hDcBuffer1, 48, 16, 240, 36, g_hDcMenuGfx, 0, 0, RGB(0, 255, 0));
        for (iYIdx = 0; iYIdx < 5; iYIdx++){
            BitBltWithTranspColor(g_hDcBuffer1, 48, 23 * iYIdx + 52, 240, 23, g_hDcMenuGfx, 0, 36, RGB(0, 255, 0));
        };
        BitBltWithTranspColor(g_hDcBuffer1, 48, 23 * iYIdx + 52, 240, 20, g_hDcMenuGfx, 0, 60, RGB(0, 255, 0));
        BitBlt(g_hMainDc, 48, 16, 240, 171, g_hDcBuffer1, 48, 16, SRCCOPY);

        g_sPoint.x = 52;
        g_sPoint.y = 58;
        ClientToScreen(g_hMainWindow, &g_sPoint);
        SetCursorPos(g_sPoint.x, g_sPoint.y);
        g_sIntermediateResult = ProcessMenu(60, 54, 5, (const TCHAR *)g_pptIngameMenuStrs);
        BitBlt(g_hDcBuffer1, 48, 16, 240, 171, g_hMainDc, 48, 16, SRCCOPY);

        if (g_sIntermediateResult == -1){
            RestoreImage(48, 16, 240, 171, 48, 16);
            RestoreImage(16, 400, 64, 64, 16, 400);
            return -1;
        }
        if (g_sIntermediateResult == 0){
            g_iCloseWindowResponce = MessageBox(g_hMainWindow, _T("Start game from the beginning"), _T("DIVI-DEAD C'sWARE"), MB_YESNO);
            g_wKeyCommand = EMPTY_MASK;
            g_sPrevKeyCommand = EMPTY_MASK;

            if (g_iCloseWindowResponce != IDYES){
                RestoreImage(48, 16, 390, 286, 48, 16);
                continue;
            }

            g_wKeyCommand = EMPTY_MASK;
            g_sPrevKeyCommand = EMPTY_MASK;
            WavePlaybackCtrl(NULL, eWAVE_Stop);
            StopMidiPlayback();
            g_iOriginX = 0;
            g_iOriginY = 0;
            g_sPicWidth = 640;
            g_sPicHeight = 480;
            EraseBGBlitWEffects(eHorizontalBlinds);
            if (LoadScriptFile(g_ptStartScriptFile)){
                result = 3;
            }
            else{
                g_iCurrPosInScript = 0;
                result = 0;
            }

            return result;
        }
        if (g_sIntermediateResult == 1){
            BitBltWithTranspColor(g_hDcBuffer1, 150, 16, 240, 36, g_hDcMenuGfx, 0, 0, RGB(0, 255, 0));
            for (g_iX = 0; g_iX < 10; g_iX++){
                BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 23, g_hDcMenuGfx, 0, 36, RGB(0, 255, 0));
            }
            BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 20, g_hDcMenuGfx, 0, 60, RGB(0, 255, 0));
            BitBlt(g_hMainDc, 150, 16, 240, 286, g_hDcBuffer1, 150, 16, SRCCOPY);

            g_sPoint.x = 154;
            g_sPoint.y = 58;
            ClientToScreen(g_hMainWindow, &g_sPoint);
            SetCursorPos(g_sPoint.x, g_sPoint.y);

            for (g_iX = 0; g_iX < SAVE_SLOT_COUNT; g_iX++){
                ConvertMBCSToUni(g_sSysFile.pcSaveNames[g_iX], g_ptSaveLoadMenuStrings[g_iX], 25);
            }
            g_ptSaveLoadMenuStrings[g_iX][0] = _T('\x00');

            g_sIntermediateResult = ProcessMenu(162, 54, 10, (const TCHAR *)g_ptSaveLoadMenuStrings);

            if (g_sIntermediateResult != -1){
                if (g_sSysFile.bSaveExists[g_sIntermediateResult]){
                    g_iCloseWindowResponce = MessageBox(g_hMainWindow, _T("Do you wish to save?"), _T("DIVI-DEAD C'sWARE"), MB_YESNO);
                    g_wKeyCommand = EMPTY_MASK;
                    g_sPrevKeyCommand = EMPTY_MASK;
                }
                if (g_iCloseWindowResponce == IDYES || !g_sSysFile.bSaveExists[g_sIntermediateResult]){
                    SaveGame(g_sIntermediateResult);
                    MessageBox(g_hMainWindow, _T("Save completed"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                    RestoreImage(48, 16, 390, 286, 48, 16);
                    RestoreImage(16, 400, 64, 64, 16, 400);
                    g_wKeyCommand = EMPTY_MASK;
                    g_sPrevKeyCommand = EMPTY_MASK;
                    return -1;
                }
            }
            RestoreImage(48, 16, 390, 286, 48, 16);
            continue;
        }
        if (g_sIntermediateResult == 2){
            BitBltWithTranspColor(g_hDcBuffer1, 150, 16, 240, 36, g_hDcMenuGfx, 0, 0, RGB(0, 255, 0));
            for (g_iX = 0; g_iX < 10; g_iX++){
                BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 23, g_hDcMenuGfx, 0, 36, RGB(0, 255, 0));
            }
            BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 20, g_hDcMenuGfx, 0, 60, RGB(0, 255, 0));
            BitBlt(g_hMainDc, 150, 16, 240, 286, g_hDcBuffer1, 150, 16, SRCCOPY);

            g_sPoint.x = 154;
            g_sPoint.y = 58;
            ClientToScreen(g_hMainWindow, &g_sPoint);
            SetCursorPos(g_sPoint.x, g_sPoint.y);

            for (g_iX = 0; g_iX < SAVE_SLOT_COUNT; g_iX++){
                ConvertMBCSToUni(g_sSysFile.pcSaveNames[g_iX], g_ptSaveLoadMenuStrings[g_iX], 25);
            }
            g_ptSaveLoadMenuStrings[g_iX][0] = _T('\x00');

            g_sIntermediateResult = ProcessMenu(162, 54, 10, (const TCHAR *)g_ptSaveLoadMenuStrings);
            if (g_sIntermediateResult != -1){
                if (g_sSysFile.bSaveExists[g_sIntermediateResult]){
                    WavePlaybackCtrl(NULL, eWAVE_Stop);
                    StopMidiPlayback();
                    LoadSave(g_sIntermediateResult);
                    g_wKeyCommand = EMPTY_MASK;
                    g_sPrevKeyCommand = EMPTY_MASK;
                    return 0;
                }
                MessageBox(g_hMainWindow, _T("Cannot find data"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                g_wKeyCommand = EMPTY_MASK;
                g_sPrevKeyCommand = EMPTY_MASK;
            }
            RestoreImage(48, 16, 390, 286, 48, 16);
            continue;
        }
        if (g_sIntermediateResult == 3){
            BitBltWithTranspColor(g_hDcBuffer1, 150, 16, 240, 36, g_hDcMenuGfx, 0, 0, RGB(0, 255, 0));
            for (g_iX = 0; g_iX < 4; g_iX++){
                BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 23, g_hDcMenuGfx, 0, 36, RGB(0, 255, 0));
            }
            BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 20, g_hDcMenuGfx, 0, 60, RGB(0, 255, 0));
            BitBlt(g_hMainDc, 150, 16, 240, 286, g_hDcBuffer1, 150, 16, SRCCOPY);

            g_sPoint.x = 154;
            g_sPoint.y = 58;
            ClientToScreen(g_hMainWindow, &g_sPoint);
            SetCursorPos(g_sPoint.x, g_sPoint.y);

            _tcsncpy_s(&g_pptConfigMenuStrs[0][15], 10, g_pptOffOn[g_bVoiceEnabled], 9);
            _tcsncpy_s(&g_pptConfigMenuStrs[1][15], 10, g_pptOffOn[g_bSoundEnabled], 9);
            _tcsncpy_s(&g_pptConfigMenuStrs[2][15], 10, g_pptOffOn[g_bBgmEnabled], 9);
            _tcsncpy_s(&g_pptConfigMenuStrs[3][15], 10, g_pptOffOn[!g_bWindowModeSelected], 9);

            g_sIntermediateResult = ProcessMenu(162, 54, 10, (const TCHAR *)g_pptConfigMenuStrs);

            if (g_sIntermediateResult != -1){
                RestoreImage(48, 16, 390, 286, 48, 16);
                RestoreImage(16, 400, 64, 64, 16, 400);
                if (!g_sIntermediateResult && g_uWaveDevsCnt > 0){
                    g_bVoiceEnabled = !g_bVoiceEnabled;
                }
                if (g_sIntermediateResult == 1 && g_uWaveDevsCnt > 0){
                    g_bSoundEnabled = !g_bSoundEnabled;
                }
                if (g_sIntermediateResult == 2 && g_uMIDIDevsCnt > 0){
                    g_bBgmEnabled = !g_bBgmEnabled;
                    if (g_bBgmEnabled && g_sGameState.pcBgMusicName[0] != ' '){
                        ConvertMBCSToUni(g_sGameState.pcBgMusicName, g_ptCvtString, CVT_BUF_SIZE);
                        PlayMidiFile(g_ptCvtString, true);
                    }
                    else{
                        MIDIPlaybackCtrl(g_hMainWindow, eMIDI_Stop, NULL, false);
                    }
                }
                if (g_sIntermediateResult == 3 && g_iScreenX > 640 && g_iScreenY > 480){
                    SendMessage(g_hMainWindow, WM_SWITCH_SCREEN_MODE, 0, 0);
                }
                RestoreImage(48, 16, 390, 286, 48, 16);
                RestoreImage(16, 400, 64, 64, 16, 400);
                g_wKeyCommand = EMPTY_MASK;
                g_sPrevKeyCommand = EMPTY_MASK;
                return -1;
            }
            RestoreImage(48, 16, 390, 286, 48, 16);
            continue;
        }
        if (g_sIntermediateResult != 4){
            return 0;
        }

        BitBltWithTranspColor(g_hDcBuffer1, 150, 16, 240, 36, g_hDcMenuGfx, 0, 0, RGB(0, 255, 0));
        for (g_iX = 0; g_iX < 2; g_iX++){
            BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 23, g_hDcMenuGfx, 0, 36, RGB(0, 255, 0));
        }
        BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 20, g_hDcMenuGfx, 0, 60, RGB(0, 255, 0));
        BitBlt(g_hMainDc, 150, 16, 240, 102, g_hDcBuffer1, 150, 16, SRCCOPY);

        g_sPoint.x = 154;
        g_sPoint.y = 58;
        ClientToScreen(g_hMainWindow, &g_sPoint);
        SetCursorPos(g_sPoint.x, g_sPoint.y);
        g_sIntermediateResult = ProcessMenu(162, 54, 10, (const TCHAR *)g_pptYesNoStrs);

        if (g_sIntermediateResult != -1){
            if (g_sIntermediateResult != 1){
                g_wKeyCommand = EMPTY_MASK;
                g_sPrevKeyCommand = EMPTY_MASK;
                return 4;
            }
        }
        RestoreImage(48, 16, 390, 102, 48, 16);
    }

    return result;
}

//----- (004084A0) --------------------------------------------------------
short ProcessRightMenu()
{
    int iIdx;
    int iXOffset;

    if (g_uWaveDevsCnt > 0 && g_bSoundEnabled)
    {
        PlaySound(MAKEINTRESOURCE(CHI), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
    }
    if (g_sGameState.pcBgPictureName[5] == 'a' || g_sGameState.pcBgPictureName[5] == 'A')   // Redraw button pressed; Night mode
    {
        iXOffset = 64;
    }
    else                                                                    // Day mode
    {
        iXOffset = 192;
    }
    BitBlt(g_hDcBuffer1, 560, 400, 64, 64, g_hDcMenuGfx, iXOffset, 80, SRCCOPY);
    BitBlt(g_hMainDc, 560, 400, 64, 64, g_hDcMenuGfx, iXOffset, 80, SRCCOPY);

    while (1)
    {
        while (1)
        {
            BitBltWithTranspColor(g_hDcBuffer1, 48, 16, 240, 36, g_hDcMenuGfx, 0, 0, RGB(0x00, 0xFF, 0x00));// Draw top of the menu

            for (iIdx = 0; iIdx < 3; iIdx++)
                BitBltWithTranspColor(g_hDcBuffer1, 48, 23 * iIdx + 52, 240, 23, g_hDcMenuGfx, 0, 36, RGB(0x00, 0xFF, 0x00));// Draw middle of the menu

            BitBltWithTranspColor(g_hDcBuffer1, 48, 23 * iIdx + 52, 240, 20, g_hDcMenuGfx, 0, 60, RGB(0x00, 0xFF, 0x00));// Draw bottom of the menu

            BitBlt(g_hMainDc, 48, 16, 240, 125, g_hDcBuffer1, 48, 16, SRCCOPY);// Blit to main display
            g_sPoint.x = 52;                           // Move mouse cursor to the menu top
            g_sPoint.y = 58;
            ClientToScreen(g_hMainWindow, &g_sPoint);
            SetCursorPos(g_sPoint.x, g_sPoint.y);
            g_sIntermediateResult = ProcessMenu(60, 54, 5, (const TCHAR *)g_pptExtrasMenuStrs);// Show right menu
            BitBlt(g_hDcBuffer1, 48, 16, 240, 125, g_hMainDc, 48, 16, SRCCOPY);
            if (g_sIntermediateResult == -1) // Escape selected
            {
                RestoreImage(48, 16, 240, 125, 48, 16);
                RestoreImage(560, 400, 64, 64, 560, 400);
                return -1;
            }
            if (g_sIntermediateResult)
                break;
            CGHitDisplay();                         // Item number 0  selected - "CG Hit display"
            g_wKeyCommand = EMPTY_MASK;
            g_sPrevKeyCommand = EMPTY_MASK;
            RestoreImage(48, 16, 390, 286, 48, 16);
        }
        if (g_sIntermediateResult != 1)
            break;
        RestoreImage(48, 16, 390, 286, 48, 16);     // Item number 1 selected - "Extras"
        RestoreImage(560, 400, 64, 64, 560, 400);
        BitBlt(g_hDcBuffer2, 640, 480, 640, 480, g_hDcBuffer1, 0, 0, SRCCOPY);
        ExtraModeMenu();
        ProcessBGMInExtraMode(eShutdown);
        BitBlt(g_hDcBuffer2, 0, 0, 640, 480, g_hDcBuffer2, 640, 480, SRCCOPY);
        BitBlt(g_hDcBuffer1, 0, 0, 640, 480, g_hDcBuffer2, 640, 480, SRCCOPY);
        BitBlt(g_hMainDc, 0, 0, 640, 480, g_hDcBuffer2, 640, 480, SRCCOPY);
        if (g_sGameState.pcBgMusicName[0] != ' '){
            ConvertMBCSToUni(g_sGameState.pcBgMusicName, g_ptCvtString, CVT_BUF_SIZE);
            PlayMidiFile(g_ptCvtString, true);
        }
    }
    if (g_sIntermediateResult == 2)             // Item number 2 selected - "Save BMP"
    {
        RestoreImage(48, 16, 240, 125, 48, 16);
        RestoreImage(560, 400, 64, 64, 560, 400);
        SaveScreenshot(g_hMainDc, (BITMAP_FILE *)g_pvDataBuffer);
        g_wKeyCommand = EMPTY_MASK;
        g_sPrevKeyCommand = EMPTY_MASK;
    }
    return 0;
}

short PrepareExtraModeMenu(short sCGPage)
{
    int     iPG0_X;
    int     iPG0_Y;

    switch (sCGPage){
    case 0:
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_1"));                       // Load bg page
        BitBlt(g_hDcBuffer1, 0, 0, 640, 480, g_hDcBuffer2, 0, 0, SRCCOPY);
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_A"));                       // Load bg page with images
        break;
    case 1:
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_2"));
        BitBlt(g_hDcBuffer1, 0, 0, 640, 480, g_hDcBuffer2, 0, 0, SRCCOPY);
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_B"));
        break;
    case 2:
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_2"));
        BitBlt(g_hDcBuffer1, 0, 0, 640, 480, g_hDcBuffer2, 0, 0, SRCCOPY);
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_C"));
        break;
    case 3:
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_2"));
        BitBlt(g_hDcBuffer1, 0, 0, 640, 480, g_hDcBuffer2, 0, 0, SRCCOPY);
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_D"));
        break;
    case 4:
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_2"));
        BitBlt(g_hDcBuffer1, 0, 0, 640, 480, g_hDcBuffer2, 0, 0, SRCCOPY);
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_E"));
        break;
    case 5:
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_3"));
        BitBlt(g_hDcBuffer1, 0, 0, 640, 480, g_hDcBuffer2, 0, 0, SRCCOPY);
        LoadAndBlitPicToBuf2(0, eMainScreen, _T("CGMODE_F"));
        break;
    default:
        return -1;
    }

    for (g_sMenuLength = 0; g_sMenuLength < 25; g_sMenuLength++){
        iPG0_X = 112 * (g_sMenuLength % 5);
        iPG0_Y = 74 * (g_sMenuLength / 5);

        if (g_sSysFile.bCgAvailable[25 * sCGPage + g_sMenuLength])  // Blit image if available
            BitBlt(g_hDcBuffer1, iPG0_X + 42, iPG0_Y + 13, 107, 70, g_hDcBuffer2, iPG0_X + 42, iPG0_Y + 13, SRCCOPY);

        g_sCGRects[g_sMenuLength].sLeft = iPG0_X + 42;
        g_sCGRects[g_sMenuLength].sTop = iPG0_Y + 13;
        g_sCGRects[g_sMenuLength].sRight = iPG0_X + 149;
        g_sCGRects[g_sMenuLength].sBottom = iPG0_Y + 83;
    }
    g_sCGRects[g_sMenuLength].sLeft = 406;               // Play button
    g_sCGRects[g_sMenuLength].sTop = 404;
    g_sCGRects[g_sMenuLength].sRight = 467;
    g_sCGRects[g_sMenuLength].sBottom = 437;
    g_sMenuLength++;
    g_sCGRects[g_sMenuLength].sLeft = 471;               // Stop Button
    g_sCGRects[g_sMenuLength].sTop = 404;
    g_sCGRects[g_sMenuLength].sRight = 532;
    g_sCGRects[g_sMenuLength].sBottom = 437;
    g_sMenuLength++;
    g_sCGRects[g_sMenuLength].sLeft = 406;               // FB Button
    g_sCGRects[g_sMenuLength].sTop = 442;
    g_sCGRects[g_sMenuLength].sRight = 467;
    g_sCGRects[g_sMenuLength].sBottom = 461;
    g_sMenuLength++;
    g_sCGRects[g_sMenuLength].sLeft = 471;               // FF Button
    g_sCGRects[g_sMenuLength].sTop = 442;
    g_sCGRects[g_sMenuLength].sRight = 532;
    g_sCGRects[g_sMenuLength].sBottom = 461;
    g_sMenuLength++;

    // Update display
    BitBlt(g_hMainDc, 0, 0, 640, 480, g_hDcBuffer1, 0, 0, SRCCOPY);
    BitBlt(g_hDcBuffer2, 0, 0, 640, 480, g_hDcBuffer1, 0, 0, SRCCOPY);
    BitBlt(g_hDcBuffer2, 640, 0, 640, 480, g_hDcBuffer1, 0, 0, SRCCOPY);

    ProcessBGMInExtraMode(eUpdateBGMName);               // Update BGM text
    return 0;
}

//----- (004088C0) --------------------------------------------------------
short ExtraModeMenu()
{
    short   iCurrPage;
    bool    bUpdate;

    iCurrPage = 0;

    if (g_sSysFile.pbGameEventsAchieved[0]) // Game was finished
    {
        g_sSysFile.bCgAvailable[53] = 1;
        g_sSysFile.bCgAvailable[140] = 1;
        g_sSysFile.bCgAvailable[141] = 1;
        g_sSysFile.bCgAvailable[142] = 1;
        g_sSysFile.bCgAvailable[143] = 1;
        g_sSysFile.bCgAvailable[144] = 1;
        g_sSysFile.bCgAvailable[145] = 1;
        g_sSysFile.bCgAvailable[146] = 1;
        g_sSysFile.bCgAvailable[147] = 1;
        g_sSysFile.bCgAvailable[148] = 1;
        g_sSysFile.bCgAvailable[149] = 1;
    }

    BitBlt(g_hDcBuffer2, 391, 392, 152, 80, g_hDcMenuGfx, 0, 162, SRCCOPY);// Copy play/Pause buttons

    while (1){
        PrepareExtraModeMenu(iCurrPage);

        bUpdate = false;

        while (2){ //-V654
            g_sIntermediateResult = SelectItemInGraphicalMode();

            if (g_sIntermediateResult == -1){           // Exit
                return 0;
            }

            switch (iCurrPage){
            case 0:
                if (g_sIntermediateResult == 24){           // PG down
                    iCurrPage++;
                    bUpdate = true;
                }
                break;

            case 1:
            case 2:
            case 3:
            case 4:
                if (g_sIntermediateResult == 0){        // PG up
                    iCurrPage--;
                    bUpdate = true;
                }
                if (g_sIntermediateResult == 24){       // PG down
                    iCurrPage++;
                    bUpdate = true;
                }
                break;

            case 5:
                if (g_sIntermediateResult == 0){        // PG up
                    iCurrPage--;
                    bUpdate = true;
                }
                break;

            default:
                continue;
            }

            if (bUpdate){
                break;
            }

            if (g_sIntermediateResult == 25){           // Play
                ProcessBGMInExtraMode(ePlayButton);
                ProcessBGMInExtraMode(eUpdateBGMName);
                continue;
            }
            if (g_sIntermediateResult == 26){           // Stop
                ProcessBGMInExtraMode(eStopButton);
                continue;
            }
            if (g_sIntermediateResult == 27){           // FB
                ProcessBGMInExtraMode(eFBButton);
                ProcessBGMInExtraMode(eUpdateBGMName);
                continue;
            }
            if (g_sIntermediateResult == 28){           // FF
                ProcessBGMInExtraMode(eFFButton);
                ProcessBGMInExtraMode(eUpdateBGMName);
                continue;
            }

            if (g_sSysFile.bCgAvailable[25 * iCurrPage + g_sIntermediateResult] && g_sIntermediateResult < 25){ // Show CG
                ShowCGNumberX(25 * iCurrPage + g_sIntermediateResult);
                break;
            }
            else{
                g_wKeyCommand = EMPTY_MASK;
                g_sPrevKeyCommand = EMPTY_MASK;
                if (!g_sSysFile.bCgAvailable[25 * iCurrPage + g_sIntermediateResult] && g_sIntermediateResult < 25)
                    continue;
            }
        }
    }
}

//----- (00409EB0) --------------------------------------------------------
void ProcessBGMInExtraMode(short sCmd)
{
    TCHAR *pptBgmFileNames[10];
    TCHAR *pptBgmNames[10];

    pptBgmFileNames[0] = _T("OPENING");
    pptBgmFileNames[1] = _T("BGM_1");
    pptBgmFileNames[2] = _T("BGM_2");
    pptBgmFileNames[3] = _T("BGM_3");
    pptBgmFileNames[4] = _T("BGM_4");
    pptBgmFileNames[5] = _T("BGM_5");
    pptBgmFileNames[6] = _T("BGM_6");
    pptBgmFileNames[7] = _T("BGM_7");
    pptBgmFileNames[8] = _T("BGM_8");
    pptBgmFileNames[9] = _T("BGM_9");
    pptBgmNames[0] = _T("Opening     ");
    pptBgmNames[1] = _T("BGM01       ");
    pptBgmNames[2] = _T("BGM02       ");
    pptBgmNames[3] = _T("BGM03       ");
    pptBgmNames[4] = _T("BGM04       ");
    pptBgmNames[5] = _T("BGM05       ");
    pptBgmNames[6] = _T("BGM06       ");
    pptBgmNames[7] = _T("BGM07       ");
    pptBgmNames[8] = _T("BGM08       ");
    pptBgmNames[9] = _T("BGM09       ");

    switch (sCmd)
    {
    case ePlayButton:                                     // Play Button pushed
        BitBlt(g_hMainDc, 406, 404, 61, 33, g_hDcMenuGfx, 15, 254, SRCCOPY);// Play button pushed
        BitBlt(g_hDcBuffer1, 406, 404, 61, 33, g_hDcMenuGfx, 15, 254, SRCCOPY);
        BitBlt(g_hDcBuffer2, 406, 404, 61, 33, g_hDcMenuGfx, 15, 254, SRCCOPY);
        PlayMidiFile(pptBgmFileNames[g_iCurrentMidiFile], true);
        BitBlt(g_hMainDc, 406, 404, 61, 33, g_hDcMenuGfx, 15, 334, SRCCOPY);// Play button active
        BitBlt(g_hDcBuffer1, 406, 404, 61, 33, g_hDcMenuGfx, 15, 334, SRCCOPY);
        BitBlt(g_hDcBuffer2, 406, 404, 61, 33, g_hDcMenuGfx, 15, 334, SRCCOPY);
        BitBlt(g_hDcBuffer2, 406 + 640, 404, 61, 33, g_hDcMenuGfx, 15, 334, SRCCOPY);
        g_bMidiPlaying = true;
        break;
    case eStopButton:                                     // Stop button pushed
        if (g_bMidiPlaying)
        {
            BitBlt(g_hMainDc, 471, 404, 61, 33, g_hDcMenuGfx, 80, 254, SRCCOPY);// Stop button pushed
            BitBlt(g_hDcBuffer1, 471, 404, 61, 33, g_hDcMenuGfx, 80, 254, SRCCOPY);
            BitBlt(g_hDcBuffer2, 471, 404, 61, 33, g_hDcMenuGfx, 80, 254, SRCCOPY);
            StopMidiPlayback();
            Sleep(100);
            BitBlt(g_hMainDc, 391, 392, 152, 80, g_hDcMenuGfx, 0, 162, SRCCOPY);// Whole buttons block
            BitBlt(g_hDcBuffer1, 391, 392, 152, 80, g_hDcMenuGfx, 0, 162, SRCCOPY);
            BitBlt(g_hDcBuffer2, 391, 392, 152, 80, g_hDcMenuGfx, 0, 162, SRCCOPY);
            BitBlt(g_hDcBuffer2, 391 + 640, 392, 152, 80, g_hDcMenuGfx, 0, 162, SRCCOPY);
            g_bMidiPlaying = false;
        }
        break;
    case eFBButton:                                     // FB Button pushed
        if (g_iCurrentMidiFile)
        {
            g_iCurrentMidiFile--;
            BitBlt(g_hMainDc, 406, 442, 61, 19, g_hDcMenuGfx, 15, 292, SRCCOPY);// FB Button pushed
            BitBlt(g_hDcBuffer1, 406, 442, 61, 19, g_hDcMenuGfx, 15, 292, SRCCOPY);
            BitBlt(g_hDcBuffer2, 406, 442, 61, 19, g_hDcMenuGfx, 15, 292, SRCCOPY);
            if (g_bMidiPlaying)
                PlayMidiFile(pptBgmFileNames[g_iCurrentMidiFile], true);
            else
                Sleep(100);
            BitBlt(g_hMainDc, 406, 442, 61, 19, g_hDcMenuGfx, 15, 372, SRCCOPY);// FB Button active
            BitBlt(g_hDcBuffer1, 406, 442, 61, 19, g_hDcMenuGfx, 15, 372, SRCCOPY);
            BitBlt(g_hDcBuffer2, 406, 442, 61, 19, g_hDcMenuGfx, 15, 372, SRCCOPY);
            BitBlt(g_hDcBuffer2, 406 + 640, 442, 61, 19, g_hDcMenuGfx, 15, 372, SRCCOPY);
        }
        break;
    case eFFButton:                                     // FF Button pushed
        if (g_iCurrentMidiFile != 9)
        {
            g_iCurrentMidiFile++;
            BitBlt(g_hMainDc, 471, 442, 61, 19, g_hDcMenuGfx, 80, 292, SRCCOPY);// FF Button pushed
            BitBlt(g_hDcBuffer1, 471, 442, 61, 19, g_hDcMenuGfx, 80, 292, SRCCOPY);
            BitBlt(g_hDcBuffer2, 471, 442, 61, 19, g_hDcMenuGfx, 80, 292, SRCCOPY);
            if (g_bMidiPlaying)
                PlayMidiFile(pptBgmFileNames[g_iCurrentMidiFile], true);
            else
                Sleep(100);
            BitBlt(g_hMainDc, 471, 442, 61, 19, g_hDcMenuGfx, 80, 372, SRCCOPY);// FF Button active
            BitBlt(g_hDcBuffer1, 471, 442, 61, 19, g_hDcMenuGfx, 80, 372, SRCCOPY);
            BitBlt(g_hDcBuffer2, 471, 442, 61, 19, g_hDcMenuGfx, 80, 372, SRCCOPY);
            BitBlt(g_hDcBuffer2, 471 + 640, 442, 61, 19, g_hDcMenuGfx, 80, 372, SRCCOPY);
        }
        break;
    case eUpdateBGMName:                                     // Update BGM name
        RestoreImage(g_sTextWindowPos.x, g_sTextWindowPos.y, 180, 2 * g_iStringHeight, g_sTextWindowPos.x, g_sTextWindowPos.y);
        g_crColor = RGB(255, 255, 255);
        SetTextColor(g_hMainDc, g_crColor);
        SetTextColor(g_hDcBuffer1, g_crColor);
        g_iTextLineHeight = 16;
        OutputString(g_hDcBuffer1, g_hMainDc, g_sTextWindowPos.x, g_sTextWindowPos.y, 20, pptBgmNames[g_iCurrentMidiFile], 0);
        break;
    case eShutdown:                                     // Stop everything
        g_bMidiPlaying = 0;
        g_iCurrentMidiFile = 0;
        StopMidiPlayback();
        Sleep(100);
        break;
    default:
        return;
    }
}

//----- (0040A5D0) --------------------------------------------------------
int ShowCG(const TCHAR *cptCGName)
{
    int     iResult;
    int     iOffsetX;
    short   sBGMCommand;

    LoadAndBlitPicToBuf2(0, eMainScreen, cptCGName);
    BlitPicWithEffects(eNoEffect);
    do
    {
        while (1)
        {
            while (1)
            {
                g_bWaitingUserInput = false;
                g_iBookProgressIdx = 0;
                g_bTrackingMouse = true;
                iResult = WaitForKeyPress() & ENTER_MASK;
                g_bTrackingMouse = false;
                g_bWaitingUserInput = false;
                if (!iResult){
                    break;
                }
                else{
                    if (g_iMousePosX > 406 && g_iMousePosX < 467 && g_iMousePosY > 404 && g_iMousePosY < 437)
                    {
                        sBGMCommand = ePlayButton;
                    }
                    else{
                        if (g_iMousePosX > 471 && g_iMousePosX < 532 && g_iMousePosY > 404 && g_iMousePosY < 437)
                        {
                            sBGMCommand = eStopButton;
                        }
                        else{
                            if (g_iMousePosX > 406 && g_iMousePosX < 467 && g_iMousePosY > 442 && g_iMousePosY < 461)
                            {
                                sBGMCommand = eFBButton;
                            }
                            else{
                                if (g_iMousePosX <= 471 || g_iMousePosX >= 532 || g_iMousePosY <= 442 || g_iMousePosY >= 461){
                                    break;
                                }
                                else{
                                    sBGMCommand = eFFButton;
                                }
                            }
                        }
                    }
                }
                ProcessBGMInExtraMode(sBGMCommand);
                ProcessBGMInExtraMode(eUpdateBGMName);
            }
            if (!(g_bKeyCommand2 & X_KEY_MASK) && (!iResult || g_iMousePosX <= 560 || g_iMousePosX >= 624 || g_iMousePosY <= 400 || g_iMousePosY >= 464))
                break;
            g_bKeyCommand2 = 0x00;
            if (g_uWaveDevsCnt > 0 && g_bSoundEnabled)
            {
                PlaySound(MAKEINTRESOURCE(CHI), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
            }
            if (g_sGameState.pcBgPictureName[5] == 'a' || g_sGameState.pcBgPictureName[5] == 'A')
            {
                iOffsetX = 64;
            }
            else
            {
                iOffsetX = 192;
            }
            BitBlt(g_hDcBuffer1, 560, 400, 64, 64, g_hDcMenuGfx, iOffsetX, 80, SRCCOPY);
            BitBlt(g_hMainDc, 560, 400, 64, 64, g_hDcMenuGfx, iOffsetX, 80, SRCCOPY);
            SaveScreenshot(g_hMainDc, (BITMAP_FILE *)g_pvDataBuffer);
            RestoreImage(560, 400, 64, 64, 560, 400);
            g_wKeyCommand = EMPTY_MASK;
            g_sPrevKeyCommand = EMPTY_MASK;
        }
    } while (g_bKeyCommand2 & Z_KEY_MASK);
    return iResult;
}

//----- (0040A840) --------------------------------------------------------
int ShowCG2(const TCHAR *cptCGName)
{
    int     iResult;
    int     iOffsetX;
    short   sBGMCommand;

    LoadAndBlitPicToBuf2(2, eRightFromScreen_TranspOverlay, cptCGName);
    CopyPictureWithTranspColor(eRightFromScreen_TranspOverlay, 0, 0);
    BlitPicWithEffects(eNoEffect);
    do
    {
        while (1)
        {
            while (1)
            {
                g_bWaitingUserInput = false;
                g_iBookProgressIdx = 0;
                g_bTrackingMouse = true;
                iResult = WaitForKeyPress() & ENTER_MASK;
                g_bTrackingMouse = false;
                g_bWaitingUserInput = false;
                if (!iResult){
                    break;
                }
                else{
                    if (iResult && g_iMousePosX > 406 && g_iMousePosX < 467 && g_iMousePosY > 404 && g_iMousePosY < 437)
                    {
                        sBGMCommand = ePlayButton;
                    }
                    else{
                        if (g_iMousePosX > 471 && g_iMousePosX < 532 && g_iMousePosY > 404 && g_iMousePosY < 437)
                        {
                            sBGMCommand = eStopButton;
                        }
                        else{
                            if (g_iMousePosX > 406 && g_iMousePosX < 467 && g_iMousePosY > 442 && g_iMousePosY < 461)
                            {
                                sBGMCommand = eFBButton;
                            }
                            else{
                                if (!iResult || g_iMousePosX <= 471 || g_iMousePosX >= 532 || g_iMousePosY <= 442 || g_iMousePosY >= 461){
                                    break;
                                }
                                else{
                                    sBGMCommand = eFFButton;
                                }
                            }
                        }
                    }
                }
                ProcessBGMInExtraMode(sBGMCommand);
                ProcessBGMInExtraMode(eUpdateBGMName);
            }
            if (!(g_bKeyCommand2 & X_KEY_MASK) && (!iResult || g_iMousePosX <= 560 || g_iMousePosX >= 624 || g_iMousePosY <= 400 || g_iMousePosY >= 464))
                break;
            g_bKeyCommand2 = 0x00;
            if (g_uWaveDevsCnt > 0 && g_bSoundEnabled)
            {
                PlaySound(MAKEINTRESOURCE(CHI), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
            }
            if (g_sGameState.pcBgPictureName[5] == 'a' || g_sGameState.pcBgPictureName[5] == 'A')
            {
                iOffsetX = 64;
            }
            else
            {
                iOffsetX = 192;
            }
            BitBlt(g_hDcBuffer1, 560, 400, 64, 64, g_hDcMenuGfx, iOffsetX, 80, SRCCOPY);
            BitBlt(g_hMainDc, 560, 400, 64, 64, g_hDcMenuGfx, iOffsetX, 80, SRCCOPY);
            SaveScreenshot(g_hMainDc, (BITMAP_FILE *)g_pvDataBuffer);
            RestoreImage(560, 400, 64, 64, 560, 400);
            g_wKeyCommand = EMPTY_MASK;
            g_sPrevKeyCommand = EMPTY_MASK;
        }
    } while (g_bKeyCommand2 & Z_KEY_MASK);
    return iResult;
}

//----- (0040AAC0) --------------------------------------------------------
short ShowCGNumberX(short iCGnum)
{
    int     iIdx;
    char    pcString[16];

    switch (iCGnum){
    case 3:
        ShowCG(_T("I_03"));
        ShowCG2(_T("I_03A"));
        break;
    case 16:
        ShowCG(_T("I_17"));
        ShowCG2(_T("I_17A"));
        ShowCG2(_T("I_17B"));
        break;
    case 19:
        ShowCG(_T("I_20"));
        ShowCG(_T("I_20_"));
        if (g_sSysFile.pbGameEventsAchieved[1]){    // Flash event achieved
            ShowCG(_T("I_20A"));
            ShowCG(_T("I_20B"));
            ShowCG(_T("I_20C"));
            ShowCG(_T("I_20D"));
            ShowCG(_T("I_20E"));
            ShowCG(_T("I_20F"));
        }
        break;
    case 27:
        ShowCG(_T("I_26"));
        ShowCG(_T("I_26A"));
        break;
    case 30:
        ShowCG(_T("I_29"));
        ShowCG2(_T("I_29A"));
        break;
    case 31:
        ShowCG(_T("I_30"));
        ShowCG2(_T("I_30A"));
        ShowCG2(_T("I_30B"));
        break;
    case 34:
        ShowCG(_T("I_34"));
        ShowCG(_T("I_34A"));
        ShowCG2(_T("I_34B"));
        break;
    case 36:
        ShowCG(_T("I_36"));
        ShowCG2(_T("I_36A"));
        break;
    case 46:
        ShowCG(_T("I_46"));
        ShowCG(_T("I_46A"));
        break;
    case 60:
        ShowCG(_T("I_58"));
        ShowCG2(_T("I_58A"));
        break;
    case 67:
        ShowCG(_T("I_65"));
        ShowCG2(_T("I_65A"));
        break;
    case 83:
        ShowCG(_T("I_79"));
        ShowCG2(_T("I_79A"));
        break;
    case 89:
        ShowCG(_T("I_85"));
        ShowCG(_T("I_85A"));
        break;
    case 91:
        ShowCG(_T("I_88"));
        ShowCG2(_T("I_88A"));
        break;
    case 92:
        ShowCG(_T("I_89"));
        ShowCG(_T("I_94"));
        break;
    case 93:
        ShowCG(_T("I_90"));
        ShowCG2(_T("I_90A"));
        break;
    case 94:
        ShowCG(_T("I_91"));
        ShowCG(_T("I_91A"));
        break;
    case 95:
        ShowCG(_T("I_92"));
        ShowCG(_T("I_92A"));
        break;
    case 98:
        ShowCG(_T("I_96"));
        ShowCG(_T("I_96A"));
        ShowCG2(_T("I_96B"));
        ShowCG(_T("I_96C"));
        break;
    case 101:
        ShowCG(_T("I_97"));
        ShowCG2(_T("I_97A"));
        break;
    case 110:
        ShowCG(_T("H_A0"));
        ShowCG(_T("H_A1"));
        ShowCG(_T("H_A2"));
        ShowCG(_T("H_A3"));
        break;
    case 111:
        ShowCG(_T("H_B0"));
        ShowCG(_T("H_B1"));
        ShowCG(_T("H_B2"));
        ShowCG(_T("H_B3"));
        break;
    case 112:
        ShowCG(_T("H_C0"));
        ShowCG(_T("H_C1"));
        ShowCG(_T("H_C2"));
        ShowCG(_T("H_C3"));
        break;
    case 113:
        ShowCG(_T("H_D0"));
        ShowCG(_T("H_D1"));
        ShowCG(_T("H_D2"));
        ShowCG(_T("H_D3"));
        ShowCG(_T("H_D4"));
        break;
    case 114:
        ShowCG(_T("H_EA0"));
        ShowCG(_T("H_EA1"));
        ShowCG(_T("H_EA2"));
        ShowCG(_T("H_EA3"));
        ShowCG(_T("H_EB0"));
        ShowCG(_T("H_EB1"));
        ShowCG(_T("H_EB2"));
        ShowCG(_T("H_EB3"));
        ShowCG(_T("H_EC0"));
        ShowCG(_T("H_EC1"));
        ShowCG(_T("H_EC2"));
        ShowCG(_T("H_EC3"));
        ShowCG(_T("H_ED0"));
        ShowCG(_T("H_ED1"));
        ShowCG(_T("H_ED2"));
        ShowCG(_T("H_ED3"));
        break;
    case 115:
        ShowCG(_T("H_F0"));
        ShowCG(_T("H_F3"));
        break;
    case 116:
        ShowCG(_T("H_GA0"));
        ShowCG(_T("H_GA3"));
        ShowCG(_T("H_GB0"));
        ShowCG(_T("H_GB3"));
        ShowCG(_T("H_GC0"));
        ShowCG(_T("H_GC3"));
        break;
    case 117:
        ShowCG(_T("H_H0"));
        ShowCG(_T("H_H1"));
        ShowCG(_T("H_H2"));
        ShowCG(_T("H_H3"));
        ShowCG(_T("H_H4"));
        break;
    case 118:
        ShowCG(_T("H_I0"));
        ShowCG(_T("H_I2"));
        ShowCG(_T("H_I3"));
        break;
    case 119:
        ShowCG(_T("H_J0"));
        ShowCG(_T("H_J1"));
        ShowCG(_T("H_J2"));
        ShowCG(_T("H_J3"));
        break;
    case 120:
        ShowCG(_T("H_K0"));
        ShowCG(_T("H_K1"));
        ShowCG(_T("H_K2"));
        ShowCG(_T("H_K3"));
        break;
    case 121:
        ShowCG(_T("H_L0"));
        ShowCG(_T("H_L1"));
        ShowCG(_T("H_L2"));
        ShowCG(_T("H_L3"));
        break;
    case 122:
        ShowCG(_T("H_M0"));
        ShowCG(_T("H_M3"));
        break;
    case 123:
        ShowCG(_T("H_N0"));
        ShowCG(_T("H_N3"));
        break;
    case 126:
        ShowCG(_T("H_O0"));
        ShowCG(_T("H_O1"));
        ShowCG(_T("H_O2"));
        ShowCG(_T("H_O3"));
        break;
    case 127:
        ShowCG(_T("H_PA0"));
        ShowCG(_T("H_PA2"));
        ShowCG(_T("H_PA3"));
        ShowCG(_T("H_PB0"));
        ShowCG(_T("H_PB2"));
        ShowCG(_T("H_PB3"));
        ShowCG(_T("H_PC0"));
        ShowCG(_T("H_PC2"));
        ShowCG(_T("H_PC3"));
        ShowCG(_T("H_PD0"));
        ShowCG(_T("H_PD2"));
        ShowCG(_T("H_PD3"));
        break;
    case 128:
        ShowCG(_T("H_Q0"));
        ShowCG(_T("H_Q1"));
        ShowCG(_T("H_Q2"));
        ShowCG(_T("H_Q3"));
        break;
    case 129:
        ShowCG(_T("H_R0"));
        ShowCG(_T("H_R3"));
        break;
    case 130:
        ShowCG(_T("H_S0"));
        ShowCG(_T("H_S3"));
        ShowCG(_T("H_S4"));
        break;
    case 131:
        ShowCG(_T("H_T0"));
        ShowCG(_T("H_T3"));
        break;
    case 132:
        ShowCG(_T("H_U0"));
        break;
    case 141:
        ShowCG(_T("OMAKE_2"));
        ShowCG2(_T("OMAKE_2A"));
        break;
    case 144:
        ShowCG(_T("OMAKE_5"));
        ShowCG2(_T("OMAKE_5A"));
        break;
    case 145:
        ShowCG(_T("OMAKE_6"));
        ShowCG2(_T("OMAKE_6A"));
        break;
    case 147:
        ShowCG(_T("OMAKE_8"));
        ShowCG2(_T("OMAKE_8A"));
        break;
    case 148:
        ShowCG(_T("OMAKE_9"));
        ShowCG(_T("OMAKE_9A"));
        break;
    case 149:
        ShowCG(_T("OMAKE_0"));
        ShowCG2(_T("OMAKE_0A"));
        ShowCG(_T("OMAKE_0B"));
        break;
    default:
        for (iIdx = 0; iIdx < sizeof(ArhiveFileEntry); iIdx++){
            if (g_pcFLIST[iCGnum].pcFileName[iIdx] == ' '){
                break;
            }
            pcString[iIdx] = g_pcFLIST[iCGnum].pcFileName[iIdx];
        }
        pcString[iIdx] = 0;
        ConvertMBCSToUni(pcString, g_ptCvtString, CVT_BUF_SIZE);
        ShowCG(g_ptCvtString);
        break;
    }

    g_bTrackingMouse = false;
    g_bWaitingUserInput = false;
    return 0;
}

//----- (0040B450) --------------------------------------------------------
short VideoModeAndIntro()
{
    int iIdx;
    short sKeysPressed;

    if (g_iScreenX > 640 && g_iScreenY > 480)
    {
        SendMessage(g_hMainWindow, WM_SHOW_SCREEN_MODE_DIALOG/*0x3DEu*/, 0, 0);// Show Dialog2
        if (g_bFullScrModeFromDlg)
        {
            g_bWindowModeSelected = true;
            SendMessage(g_hMainWindow, WM_SWITCH_SCREEN_MODE/*0x40Au*/, 0, 0);// Switch display mode
        }
    }
    g_bVideoIsPlaying = true;
    SendMessage(g_hMainWindow, WM_PLAY_LOGO_VIDEO/*0x402u*/, 0, 0); // Play CS_ROGO.AVI
    iIdx = 0;
    do{
        Sleep(100);
        if (!g_bVideoIsPlaying) break;
        ++iIdx;
    } while (iIdx < 100);
    SendMessage(g_hMainWindow, WM_STOP_LOGO_VIDEO/*0x403u*/, 0, 0); // Stop video playback
    Sleep(100);
    g_bVideoIsPlaying = true;
    SendMessage(g_hMainWindow, WM_PLAY_INTRO_VIDEO/*0x400u*/, 0, 0); // Play OPEN.AVI
    Sleep(50);

    do{
        sKeysPressed = CheckKeysToProc(ENTER_MASK);
        sKeysPressed = sKeysPressed | CheckKeysToProc(ESCAPE_MASK);
        Sleep(50);
    } while (!sKeysPressed || !g_bVideoIsPlaying);

    SendMessage(g_hMainWindow, WM_STOP_INTRO_VIDEO/*0x401u*/, 0, 0); // Stop playback
    Sleep(100);
    Sleep(100);
    return 0;
}

//----- (0040B590) --------------------------------------------------------
short ProcessMainStartMenu()
{
    short   sKeyCommand;
    int     iPicPosX;
    bool    bRestart;
    int     iIdx;
    short   sResult;

    g_iOriginX = 0;
    g_iOriginY = 0;
    g_sPicWidth = 640;
    g_sPicHeight = 480;
    EraseBGBlitWEffects(eNoEffect);
    LoadAndBlitPicToBuf2(0, eRightAndBottomFromScreen_BG, _T("WAKU_A1"));
    CopyPictureWithTranspColor(eRightAndBottomFromScreen_BG, 0, 0);
    LoadAndBlitPicToBuf2(0, eBottomFromScreen_Overlay, _T("TITLE"));
    CopyPictureWithTranspColor(eBottomFromScreen_Overlay, 0, 0);
    g_iOriginX = 0;
    g_iOriginY = 0;
    g_sPicWidth = 640;
    g_sPicHeight = 480;
    BlitPicWithEffects(eHorizontalBlinds);

    do{
        bRestart = false;
        g_bWaitingUserInput = false;
        g_iBookProgressIdx = 0;
        g_bTrackingMouse = true;
        sKeyCommand = WaitForKeyPress();
        g_bTrackingMouse = false;
        while (!(g_bKeyCommand2 & Z_KEY_MASK))// NOT Z_Key
        {
            // ENTER or mouse LB pressed
            if (sKeyCommand & ENTER_MASK && g_iMousePosX > 16 && g_iMousePosX < 80 && g_iMousePosY > 400 && g_iMousePosY < 464)
                break;

            // X key or mouse LB pressed
            if (g_bKeyCommand2 & X_KEY_MASK || sKeyCommand & ENTER_MASK && g_iMousePosX > 560 && g_iMousePosX < 624 && g_iMousePosY > 400 && g_iMousePosY < 464)
            {
                g_bKeyCommand2 = 0x00;
                ProcessRightMenu();
                g_iCurrPosInScript = g_iNewPosInScript;
            }
            g_bWaitingUserInput = false;
            g_iBookProgressIdx = 0;
            g_bTrackingMouse = true;
            sKeyCommand = WaitForKeyPress();
            g_bTrackingMouse = false;
        }
        g_bKeyCommand2 = 0x00;
        if (g_uWaveDevsCnt > 0 && g_bSoundEnabled)
        {
            PlaySound(MAKEINTRESOURCE(CHI), GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC);
        }
        if (g_sGameState.pcBgPictureName[5] == 'a' || g_sGameState.pcBgPictureName[5] == 'A')   // Day mode
            iPicPosX = 0;
        else                                                                    // Night mode
            iPicPosX = 128;
        BitBlt(g_hDcBuffer1, 16, 400, 64, 64, g_hDcMenuGfx, iPicPosX, 80, SRCCOPY);
        BitBlt(g_hMainDc, 16, 400, 64, 64, g_hDcMenuGfx, iPicPosX, 80, SRCCOPY);

        while (1)                                   // Main menu loop
        {
            // Blit top menu part
            BitBltWithTranspColor(g_hDcBuffer1, 48, 16, 240, 36, g_hDcMenuGfx, 0, 0, RGB(0x00, 0xFF, 0x00));

            // Blit 4 middle menu parts
            for (iIdx = 0; iIdx < 4; iIdx++){
                BitBltWithTranspColor(g_hDcBuffer1, 48, 23 * iIdx + 52, 240, 23, g_hDcMenuGfx, 0, 36, RGB(0x00, 0xFF, 0x00));
            }

            // Blit bottom menu part
            BitBltWithTranspColor(g_hDcBuffer1, 48, 23 * iIdx + 52, 240, 20, g_hDcMenuGfx, 0, 60, RGB(0x00, 0xFF, 0x00));

            BitBlt(g_hMainDc, 48, 16, 240, 171, g_hDcBuffer1, 48, 16, SRCCOPY);
            g_sPoint.x = 52;
            g_sPoint.y = 58;
            ClientToScreen(g_hMainWindow, &g_sPoint);
            SetCursorPos(g_sPoint.x, g_sPoint.y);

            g_sIntermediateResult = ProcessMenu(60, 54, 5, (const TCHAR *)g_pptStartMenuStrs);
            BitBlt(g_hDcBuffer1, 48, 16, 240, 171, g_hMainDc, 48, 16, SRCCOPY);
            if (g_sIntermediateResult == -1){
                RestoreImage(48, 16, 240, 171, 48, 16);
                RestoreImage(16, 400, 64, 64, 16, 400);
                bRestart = true;
                break;
            }
            if (!g_sIntermediateResult){        // Start selected
                break;
            }
            if (g_sIntermediateResult == 1){    // Load selected
                BitBltWithTranspColor(g_hDcBuffer1, 150, 16, 240, 36, g_hDcMenuGfx, 0, 0, RGB(0x00, 0xFF, 0x00));
                for (g_iX = 0; g_iX < 10; g_iX++){
                    BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 23, g_hDcMenuGfx, 0, 36, RGB(0x00, 0xFF, 0x00));
                }
                BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 20, g_hDcMenuGfx, 0, 60, RGB(0x00, 0xFF, 0x00));
                BitBlt(g_hMainDc, 150, 16, 240, 286, g_hDcBuffer1, 150, 16, SRCCOPY);
                g_sPoint.x = 154;
                g_sPoint.y = 58;
                ClientToScreen(g_hMainWindow, &g_sPoint);
                SetCursorPos(g_sPoint.x, g_sPoint.y);

                for (g_iX = 0; g_iX < SAVE_SLOT_COUNT; g_iX++){
                    ConvertMBCSToUni(g_sSysFile.pcSaveNames[g_iX], g_ptSaveLoadMenuStrings[g_iX], 25);
                }
                g_ptSaveLoadMenuStrings[g_iX][0] = _T('\x00');

                g_sIntermediateResult = ProcessMenu(162, 54, 10, (const TCHAR *)g_ptSaveLoadMenuStrings);
                if (g_sIntermediateResult != -1){
                    if (g_sSysFile.bSaveExists[g_sIntermediateResult]){
                        WavePlaybackCtrl(NULL, eWAVE_Stop);
                        StopMidiPlayback();
                        LoadSave(g_sIntermediateResult);
                        g_wKeyCommand = EMPTY_MASK;
                        g_sPrevKeyCommand = EMPTY_MASK;
                        return 0;
                    }
                    MessageBox(g_hMainWindow, _T("Cannot find data"), _T("DIVI-DEAD C'sWARE"), MB_OK);
                    g_wKeyCommand = EMPTY_MASK;
                    g_sPrevKeyCommand = EMPTY_MASK;
                }
                RestoreImage(48, 16, 390, 286, 48, 16);
            }
            if (g_sIntermediateResult == 2){    // Config selected
                BitBltWithTranspColor(g_hDcBuffer1, 150, 16, 240, 36, g_hDcMenuGfx, 0, 0, RGB(0x00, 0xFF, 0x00));
                for (g_iX = 0; g_iX < 4; g_iX++){
                    BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 23, g_hDcMenuGfx, 0, 36, RGB(0x00, 0xFF, 0x00));
                }
                BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 20, g_hDcMenuGfx, 0, 60, RGB(0x00, 0xFF, 0x00));
                BitBlt(g_hMainDc, 150, 16, 240, 286, g_hDcBuffer1, 150, 16, SRCCOPY);
                g_sPoint.x = 154;
                g_sPoint.y = 58;
                ClientToScreen(g_hMainWindow, &g_sPoint);
                SetCursorPos(g_sPoint.x, g_sPoint.y);
                _tcsncpy_s(&g_pptConfigMenuStrs[0][15], 10, g_pptOffOn[g_bVoiceEnabled], 9); // - Cripples data
                _tcsncpy_s(&g_pptConfigMenuStrs[1][15], 10, g_pptOffOn[g_bSoundEnabled], 9);
                _tcsncpy_s(&g_pptConfigMenuStrs[2][15], 10, g_pptOffOn[g_bBgmEnabled], 9);
                _tcsncpy_s(&g_pptConfigMenuStrs[3][15], 10, g_pptOffOn[!g_bWindowModeSelected], 9);

                g_sIntermediateResult = ProcessMenu(162, 54, 10, (const TCHAR *)g_pptConfigMenuStrs);

                if (g_sIntermediateResult == -1){
                    RestoreImage(48, 16, 390, 286, 48, 16);
                }
                else{
                    RestoreImage(48, 16, 390, 286, 48, 16);
                    RestoreImage(16, 400, 64, 64, 16, 400);
                    if (g_sIntermediateResult == 0 && g_uWaveDevsCnt > 0)// Toggle voice
                        g_bVoiceEnabled = !g_bVoiceEnabled;
                    if (g_sIntermediateResult == 1 && g_uWaveDevsCnt > 0)// Toggle Sound
                        g_bSoundEnabled = !g_bSoundEnabled;
                    if (g_sIntermediateResult == 2 && g_uMIDIDevsCnt > 0)// Toggle BGM
                    {
                        g_bBgmEnabled = !g_bBgmEnabled;
                        if (g_bBgmEnabled && g_sGameState.pcBgMusicName[0] != ' '){
                            ConvertMBCSToUni(g_sGameState.pcBgMusicName, g_ptCvtString, CVT_BUF_SIZE);
                            PlayMidiFile(g_ptCvtString, true);
                        }
                        else{
                            MIDIPlaybackCtrl(g_hMainWindow, eMIDI_Stop, NULL, false);
                        }
                    }
                    if (g_sIntermediateResult == 3 && g_iScreenX > 640 && g_iScreenY > 480)// Toggle Fullscreen
                        SendMessage(g_hMainWindow, WM_SWITCH_SCREEN_MODE/*0x40Au*/, 0, 0);
                    RestoreImage(48, 16, 390, 286, 48, 16);
                    g_wKeyCommand = EMPTY_MASK;
                    g_sPrevKeyCommand = EMPTY_MASK;
                }
            }
            else                                            // End selected
            {
                if (g_sIntermediateResult != 3) // Error in selection
                    return 0;
                BitBltWithTranspColor(g_hDcBuffer1, 150, 16, 240, 36, g_hDcMenuGfx, 0, 0, RGB(0x00, 0xFF, 0x00));
                for (g_iX = 0; g_iX < 2; g_iX++)
                {
                    BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 23, g_hDcMenuGfx, 0, 36, RGB(0x00, 0xFF, 0x00));
                }
                BitBltWithTranspColor(g_hDcBuffer1, 150, 23 * g_iX + 52, 240, 20, g_hDcMenuGfx, 0, 60, RGB(0x00, 0xFF, 0x00));
                BitBlt(g_hMainDc, 150, 16, 240, 102, g_hDcBuffer1, 150, 16, SRCCOPY);
                g_sPoint.x = 154;
                g_sPoint.y = 58;
                ClientToScreen(g_hMainWindow, &g_sPoint);
                SetCursorPos(g_sPoint.x, g_sPoint.y);
                g_sIntermediateResult = ProcessMenu(162, 54, 10, (const TCHAR *)g_pptYesNoStrs);
                if (g_sIntermediateResult == -1)
                {
                    RestoreImage(48, 16, 390, 102, 48, 16);
                }
                else
                {
                    if (g_sIntermediateResult != 1)
                    {
                        g_wKeyCommand = EMPTY_MASK;
                        g_sPrevKeyCommand = EMPTY_MASK;
                        return 4;
                    }
                    RestoreImage(48, 16, 390, 102, 48, 16);
                }
            }
        }                                             // End of main menu loop
    } while (bRestart);
    g_wKeyCommand = EMPTY_MASK;
    g_sPrevKeyCommand = EMPTY_MASK;
    WavePlaybackCtrl(NULL, eWAVE_Stop);
    StopMidiPlayback();
    g_iOriginX = 0;
    g_iOriginY = 0;
    g_sPicWidth = 640;
    g_sPicHeight = 480;
    EraseBGBlitWEffects(eHorizontalBlinds);
    if (LoadScriptFile(g_ptStartScriptFile))
    {
        sResult = 3;
    }
    else
    {
        g_iCurrPosInScript = 0;
        sResult = 0;
    }
    return sResult;
}

//----- (0040C070) --------------------------------------------------------
int DimScreenToBlack()
{
    int         iIdx1;
    int         iIdx2;
    HDC         hCompatibleDc;
    HBITMAP     hBitmap;
    HBITMAP     hOldBitmap;
    BYTE        *pucBitmapArray;
    BITMAP_FILE sBitmapFile;
    short       sComponentVal;

    if (&sBitmapFile){
        sBitmapFile.bmfh.bfType = 0x4D42;
        sBitmapFile.bmfh.bfSize = 921654;
        sBitmapFile.bmfh.bfReserved1 = 0;
        sBitmapFile.bmfh.bfReserved2 = 0;
        sBitmapFile.bmfh.bfOffBits = 54;
        sBitmapFile.bmi.bmiHeader.biSize = 40;
        sBitmapFile.bmi.bmiHeader.biWidth = 640;
        sBitmapFile.bmi.bmiHeader.biHeight = 480;
        sBitmapFile.bmi.bmiHeader.biPlanes = 1;
        sBitmapFile.bmi.bmiHeader.biBitCount = 24;
        sBitmapFile.bmi.bmiHeader.biCompression = 0;
        sBitmapFile.bmi.bmiHeader.biSizeImage = 0;
        sBitmapFile.bmi.bmiHeader.biXPelsPerMeter = 0;
        sBitmapFile.bmi.bmiHeader.biYPelsPerMeter = 0;
        sBitmapFile.bmi.bmiHeader.biClrUsed = 0;
        sBitmapFile.bmi.bmiHeader.biClrImportant = 0;

        hBitmap = CreateDIBSection(g_hDcBuffer1, &sBitmapFile.bmi, 0, (void**)&pucBitmapArray, 0, 0);
        if (hBitmap){
            hCompatibleDc = CreateCompatibleDC(g_hDcBuffer1);
            hOldBitmap = (HBITMAP)SelectObject(hCompatibleDc, hBitmap);

            BitBlt(hCompatibleDc, 0, 0, 640, 480, g_hDcBuffer1, 0, 0, SRCCOPY);

            for (iIdx1 = 0; iIdx1 < 16; iIdx1++){
                for (iIdx2 = 0; iIdx2 < 640 * 480 * 3; iIdx2++){
                    sComponentVal = (short)(pucBitmapArray[iIdx2]) - 16;
                    if (sComponentVal < 0) sComponentVal = 0;
                    pucBitmapArray[iIdx2] = (BYTE)sComponentVal;
                }
                BitBlt(g_hDcBuffer1, 0, 0, 640, 480, hCompatibleDc, 0, 0, SRCCOPY);
                BitBlt(g_hMainDc, 0, 0, 640, 480, g_hDcBuffer1, 0, 0, SRCCOPY);

                Sleep(20);
            }
            DeleteObject(SelectObject(hCompatibleDc, (HGDIOBJ)hOldBitmap));
            DeleteDC(hCompatibleDc);

            Sleep(80);
        }
    }
    return 0;
}

//----- (0040C240) --------------------------------------------------------
int DimmScreenToWhite()
{
    int         iIdx1;
    int         iIdx2;
    HDC         hCompatibleDc;
    HBITMAP     hBitmap;
    HBITMAP     hOldBitmap;
    BITMAP_FILE sBitmapFile;
    BYTE        *pucBitmapArray;
    short       sComponentVal;

    if (&sBitmapFile){
        sBitmapFile.bmfh.bfType = 0x4D42;
        sBitmapFile.bmfh.bfSize = 921654;
        sBitmapFile.bmfh.bfReserved1 = 0;
        sBitmapFile.bmfh.bfReserved2 = 0;
        sBitmapFile.bmfh.bfOffBits = 54;
        sBitmapFile.bmi.bmiHeader.biSize = 40;
        sBitmapFile.bmi.bmiHeader.biWidth = 640;
        sBitmapFile.bmi.bmiHeader.biHeight = 480;
        sBitmapFile.bmi.bmiHeader.biPlanes = 1;
        sBitmapFile.bmi.bmiHeader.biBitCount = 24;
        sBitmapFile.bmi.bmiHeader.biCompression = 0;
        sBitmapFile.bmi.bmiHeader.biSizeImage = 0;
        sBitmapFile.bmi.bmiHeader.biXPelsPerMeter = 0;
        sBitmapFile.bmi.bmiHeader.biYPelsPerMeter = 0;
        sBitmapFile.bmi.bmiHeader.biClrUsed = 0;
        sBitmapFile.bmi.bmiHeader.biClrImportant = 0;

        hBitmap = CreateDIBSection(g_hDcBuffer1, &sBitmapFile.bmi, 0, (void**)&pucBitmapArray, 0, 0);
        if (hBitmap){
            hCompatibleDc = CreateCompatibleDC(g_hDcBuffer1);
            hOldBitmap = (HBITMAP)SelectObject(hCompatibleDc, hBitmap);

            BitBlt(hCompatibleDc, 0, 0, 640, 480, g_hDcBuffer1, 0, 0, SRCCOPY);

            for (iIdx1 = 0; iIdx1 < 16; iIdx1++){
                for (iIdx2 = 0; iIdx2 < 640 * 480 * 3; iIdx2++){
                    sComponentVal = (short)(pucBitmapArray[iIdx2]) + 16;
                    if (sComponentVal > 255) sComponentVal = 255;
                    pucBitmapArray[iIdx2] = (BYTE)sComponentVal;
                }
                BitBlt(g_hDcBuffer1, 0, 0, 640, 480, hCompatibleDc, 0, 0, SRCCOPY);
                BitBlt(g_hMainDc, 0, 0, 640, 480, g_hDcBuffer1, 0, 0, SRCCOPY);

                Sleep(20);
            }
            DeleteObject(SelectObject(hCompatibleDc, (HGDIOBJ)hOldBitmap));
            DeleteDC(hCompatibleDc);

            Sleep(80);
        }
    }
    return 0;
}

//----- (0040C3F0) --------------------------------------------------------
short CGHitDisplay()
{
    short           *psCGSeen;
    ArhiveFileEntry *pcFLIST;
    int             iCGCount;
    int             iSeenCGCount;
    TCHAR           ptString[32];
    TCHAR           ptMessageText[128];

    psCGSeen = g_sSysFile.bCgAvailable;
    pcFLIST = g_pcFLIST;
    iCGCount = 0;
    iSeenCGCount = 0;
    do
    {
        if (pcFLIST[0].pcFileName[0] == _T('/'))
            break;
        if (pcFLIST[0].pcFileName[0] != _T(' '))
        {
            if (pcFLIST[0].pcFileName[0] != _T('O'))
            {
                iCGCount++;
                if (*psCGSeen)
                    iSeenCGCount++;
            }
        }
        pcFLIST += sizeof(ArhiveFileEntry);
        psCGSeen++;
    } while (pcFLIST - g_pcFLIST < FLIST_ENTRY_COUNT * sizeof(ArhiveFileEntry));
    if (iCGCount)
    {
        wsprintf(ptString, _T("CG Hit Display %3d.%d%%\r\n"), 1000 * iSeenCGCount / iCGCount / 10, 1000 * iSeenCGCount / iCGCount % 10);
        _tcscpy_s(ptMessageText, ptString);
        MessageBox(g_hMainWindow, ptMessageText, _T("DIVI-DEAD C'sWARE"), MB_OK);
    }
    return 0;
}

//----- (0040C4A0) --------------------------------------------------------
int MarkCGAsSeen(const char *pcFileToFind)
{
    int             iIdx;
    ArhiveFileEntry *pcFileInFList;
    bool            bFound;

    strncpy_s(g_pcFileToFind.pcFileName, MAX_ARCH_NAME_SIZE, "           ", MAX_ARCH_NAME_SIZE);

    for (iIdx = 0; iIdx < MAX_ARCH_NAME_SIZE; iIdx++){
        if (pcFileToFind[iIdx] == '.' || pcFileToFind[iIdx] == 0){
            g_pcFileToFind.pcFileName[iIdx] = 0;
            break;
        }
        g_pcFileToFind.pcFileName[iIdx] = pcFileToFind[iIdx];
    }

    pcFileInFList = g_pcFLIST;
    bFound = false;

    for (iIdx = 0; iIdx < FLIST_ENTRY_COUNT; iIdx++){
        if (pcFileInFList[0].pcFileName[0] == '/'){
            iIdx = -1;
            break;
        }
        if (strncmp(pcFileInFList[0].pcFileName, g_pcFileToFind.pcFileName, 12) == 0){
            bFound = true;
            break;
        }
        pcFileInFList++;
    }

    if (iIdx != -1 && bFound) g_sSysFile.bCgAvailable[iIdx] = 1;

    return 0;
}

//----- (0040C540) --------------------------------------------------------
DWORD SaveScreenshot(HDC hdcSrc, BITMAP_FILE *lpBuffer)
{
    DWORD   dwResult;
    HDC     hDcSrc;
    HBITMAP hBitmap;
    HBITMAP hBitmap_Old;
    int     iFileIdx;
    void    *ppvBits;
    TCHAR   ptFileName[MAX_PATH];
    TCHAR   ptFileNameWPath[MAX_PATH];

    g_iCloseWindowResponce = MessageBox(g_hMainWindow, _T("Do you wish to save this screen?"), _T("DIVI-DEAD C'sWARE"), MB_YESNO);
    g_wKeyCommand = EMPTY_MASK;
    g_sPrevKeyCommand = EMPTY_MASK;
    if (g_iCloseWindowResponce == IDYES)
    {
        if (lpBuffer)
        {
            lpBuffer->bmfh.bfType = 0x4D42;
            lpBuffer->bmfh.bfSize = 649782;
            lpBuffer->bmfh.bfReserved1 = 0;
            lpBuffer->bmfh.bfReserved2 = 0;
            lpBuffer->bmfh.bfOffBits = 54;
            lpBuffer->bmi.bmiHeader.biSize = 40;
            lpBuffer->bmi.bmiHeader.biWidth = 576;
            lpBuffer->bmi.bmiHeader.biHeight = 376;
            lpBuffer->bmi.bmiHeader.biPlanes = 1;
            lpBuffer->bmi.bmiHeader.biBitCount = 24;
            lpBuffer->bmi.bmiHeader.biCompression = 0;
            lpBuffer->bmi.bmiHeader.biSizeImage = 0;
            lpBuffer->bmi.bmiHeader.biXPelsPerMeter = 0;
            lpBuffer->bmi.bmiHeader.biYPelsPerMeter = 0;
            lpBuffer->bmi.bmiHeader.biClrUsed = 0;
            lpBuffer->bmi.bmiHeader.biClrImportant = 0;
            hBitmap = CreateDIBSection(hdcSrc, &lpBuffer->bmi, 0, &ppvBits, 0, 0);
            if (hBitmap)
            {
                hDcSrc = CreateCompatibleDC(hdcSrc);
                hBitmap_Old = (HBITMAP)SelectObject(hDcSrc, hBitmap);

                BitBlt(hDcSrc, 0, 0, 576, 376, hdcSrc, 32, 8, SRCCOPY);
                memcpy((BYTE *)lpBuffer + lpBuffer->bmfh.bfOffBits, ppvBits, 576 * 376 * 3);
                DeleteObject(SelectObject(hDcSrc, hBitmap_Old));
                DeleteDC(hDcSrc);
                iFileIdx = 0;
                while (1)
                {
                    _tcscpy_s(ptFileNameWPath, g_ptSaveFolder);
                    wsprintf(ptFileName, _T("DIVI%03d.BMP"), iFileIdx);
                    _tcsncat_s(ptFileNameWPath, ptFileName, _TRUNCATE);
                    dwResult = WriteToFile(ptFileNameWPath, lpBuffer, lpBuffer->bmfh.bfSize);
                    if (dwResult)
                        break;

                    iFileIdx++;

                    if (iFileIdx >= 1000)
                        return dwResult;
                }
                wsprintf(ptFileName, _T("BMP file has been created in the following folders \r\n\r\n%s"), ptFileNameWPath);
                MessageBox(g_hMainWindow, ptFileName, _T("DIVI-DEAD C'sWARE"), MB_OK);
            }
            else
            {
                dwResult = 0;
            }
        }
        else
        {
            dwResult = 0;
        }
    }
    else
    {
        dwResult = 1;
    }
    return dwResult;
}

//----- (0040C740) --------------------------------------------------------
int ShowCredits()
{
    Sleep(100);

    g_iOriginX = 0;
    g_iOriginY = 0;
    g_sPicWidth = 640;
    g_sPicHeight = 480;

    EraseBGBlitWEffects(eNoEffect);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("A"));
    BitBlt(g_hDcBuffer1, 32, 158, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 158, 576, 376, g_hDcBuffer1, 32, 158, SRCCOPY);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("01"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 8;
    g_sPicWidth = 576;
    g_sPicHeight = 376;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("B"));
    BitBlt(g_hDcBuffer1, 32, 158, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 158, 576, 376, g_hDcBuffer1, 32, 158, SRCCOPY);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("02"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 8;
    g_sPicWidth = 576;
    g_sPicHeight = 376;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("C"));
    BitBlt(g_hDcBuffer1, 32, 158, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 158, 576, 376, g_hDcBuffer1, 32, 158, SRCCOPY);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("03"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 8;
    g_sPicWidth = 576;
    g_sPicHeight = 376;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("D"));
    BitBlt(g_hDcBuffer1, 32, 158, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 158, 576, 376, g_hDcBuffer1, 32, 158, SRCCOPY);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("04"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000u);

    g_iOriginX = 32;
    g_iOriginY = 8;
    g_sPicWidth = 576;
    g_sPicHeight = 376;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("E"));
    BitBlt(g_hDcBuffer1, 32, 158, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 158, 576, 376, g_hDcBuffer1, 32, 158, SRCCOPY);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("05"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 8;
    g_sPicWidth = 576;
    g_sPicHeight = 376;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("F"));
    BitBlt(g_hDcBuffer1, 32, 158, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 158, 576, 376, g_hDcBuffer1, 32, 158, SRCCOPY);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("06"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 8;
    g_sPicWidth = 576;
    g_sPicHeight = 376;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("G"));
    BitBlt(g_hDcBuffer1, 32, 158, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 158, 576, 376, g_hDcBuffer1, 32, 158, SRCCOPY);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("07"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 8;
    g_sPicWidth = 576;
    g_sPicHeight = 376;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("H"));
    BitBlt(g_hDcBuffer1, 32, 158, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 158, 576, 376, g_hDcBuffer1, 32, 158, SRCCOPY);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("08"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 238;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("09"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 238;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("10"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 238;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("11"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 8;
    g_sPicWidth = 576;
    g_sPicHeight = 376;
    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("I"));
    BitBlt(g_hDcBuffer1, 32, 158, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 158, 576, 376, g_hDcBuffer1, 32, 158, SRCCOPY);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("12"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 238;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("13"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 238;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("14"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 238;

    EraseBGBlitWEffects(eNoEffect);

    Sleep(500);

    LoadAndBlitPicToBuf2(0, eMainScreen, _T("15"));
    BitBlt(g_hDcBuffer1, 32, 238, 576, 80, g_hDcBuffer2, 0, 0, SRCCOPY);
    BitBlt(g_hMainDc, 32, 238, 576, 376, g_hDcBuffer1, 32, 238, SRCCOPY);

    Sleep(3000);

    g_iOriginX = 32;
    g_iOriginY = 8;
    g_sPicWidth = 576;
    g_sPicHeight = 376;

    EraseBGBlitWEffects(eNoEffect);

    return 0;
}

//----- (0040D3C0) --------------------------------------------------------
// Return false if no spaces found, return true if found
bool CheckIfContainsSpaces(int iStrLength, TCHAR *ptString)
{
    int iIdx;

    for (iIdx = 0; iIdx < MAX_STRING_LENGTH; iIdx++){
        if (ptString[iStrLength - iIdx - 1] == _T(' ')){
            return true;
        }
    }

    return false;
}

//----- (0040D3F0) --------------------------------------------------------
// Return false if not last string, return true if last string
bool CopyPartOfString(int iSrcOffset, int iDstOffset, TCHAR *ptString)
{
    int iIdx;

    for (iIdx = 0; iIdx < MAX_STRING_LENGTH; iIdx++){
        g_ptStrings[iDstOffset + iIdx + 1] = ptString[iSrcOffset + iIdx];
        if (ptString[iSrcOffset + iIdx] == 0){
            return true;
        }
    }

    return false;
}

//----- (0040D420) --------------------------------------------------------
void SplitStringsToFitOnScreen(TCHAR *ptString)
{
    int iSrcIdx;
    int iDstIdx;
    bool bLastString;
    TCHAR tChar;

    iSrcIdx = 0;
    iDstIdx = 0;

    do{
        bLastString = CopyPartOfString(iSrcIdx, iDstIdx, ptString);
        iSrcIdx += MAX_STRING_LENGTH;
        iDstIdx += MAX_STRING_LENGTH;
        if (!bLastString){  // Split string
            if (ptString[iSrcIdx] >= _T('!') && ptString[iSrcIdx] <= _T('}') && ptString[iSrcIdx - 1] >= _T('!') && ptString[iSrcIdx - 1] <= _T('}')){  // Need to find last space where we can break sentence
                if (CheckIfContainsSpaces(iSrcIdx, ptString)){
                    do{ // Step back till the last space
                        tChar = ptString[iSrcIdx];
                        iSrcIdx--;
                        iDstIdx--;
                    } while (tChar != _T(' '));

                    iDstIdx++;
                    g_ptStrings[iDstIdx + 1] = _T('\\');
                    iDstIdx++;
                    g_ptStrings[iDstIdx + 1] = _T('N');
                    iDstIdx++;
                    iSrcIdx += 2;
                }
            }
            if (ptString[iSrcIdx] == _T(' ')){  // If contains space - erase it, no need to insert control symbols
                iSrcIdx++;
            }
        }
    } while (!bLastString);
}
