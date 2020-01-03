#pragma once

#include "Resource.h"

#define SHIFT_JIS_CP    (932)

#define GET_BYTE(pBase, lOffset)        (*(BYTE *)((BYTE *)(pBase) + (lOffset)))
#define GET_WORD(pBase, lOffset)        (*(WORD *)((BYTE *)(pBase) + (lOffset)))
#define GET_DWORD(pBase, lOffset)       (*(DWORD *)((BYTE *)(pBase) + (lOffset)))
#define GET_STRING(pBase, lOffset)      ((char *)(pBase) + (lOffset))

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   480

#define EMPTY_MASK  0x0000
#define UP_MASK     0x0011
#define DOWN_MASK   0x0088
#define LEFT_MASK   0x0022
#define RIGHT_MASK  0x0044
#define ENTER_MASK  0x0600
#define ESCAPE_MASK 0x0900
#define Z_KEY_MASK  0x01
#define X_KEY_MASK  0x02

#define CVT_BUF_SIZE            0x200ul
#define MAX_ARCH_NAME_SIZE      12
#define UNPACK_WIN_SIZE         0x1000ul
#define UNPACK_WIN_START_OFFSET 0x0FEEul
#define UNPACK_WIN_MASK         0x0FFFul
#define FLIST_ENTRY_COUNT       0x400ul
#define MAX_BUF_SIZE            0x100000ul
#define SCRIPT_BUF_SIZE         MAX_BUF_SIZE
#define DATA_BUF_SIZE           (MAX_BUF_SIZE * 2)
#define SOUND_BUF_SIZE          MAX_BUF_SIZE

#define GAME_SCREEN_SIZE_X      640
#define GAME_SCREEN_SIZE_Y      480

#define VIDEO_SIZE_X            480
#define VIDEO_SIZE_Y            264 //240

#define SHOW_MENU_MOUSE_POS_Y   5

#define FONT_HEIGHT             19
#define FONT_WIDHT              10
#define MAX_STRING_LENGTH       46
#define TAB_SIZE                4

#define FLIP_BOOK_POS_X         396
#define MAX_TEXT_LINE_COUNT     3
#define FLIP_BOOK_SIZE_X        18
#define FLIP_BOOK_SIZE_Y        18
#define FLIP_BOOK_OFFSET_Y      144

#define PERSONAGE_SIZE_X        288
#define PERSONAGE_SIZE_Y        376
#define PERSONAGE_POS_Y         8

#define MAX_MENU_STRING_LEN     64
#define MAX_MENU_STRINGS        20

#define SAVE_SLOT_COUNT         10

#define EFFECTS_DELAY           15  // [ms]

enum    enPicOrigins {
	eMainScreen = 0,                // 0
	eRightFromScreen_TranspOverlay, // 1
	eBottomFromScreen_Overlay,      // 2
	eRightAndBottomFromScreen_BG,   // 3
	eLeftPartOfScreen,              // 4
	eRightPartOfScreen              // 5
};

enum    enBlitEffects {
	eNoEffect = 0,              // 0
	eOrderedBlocks,             // 1
	eHorizontalBlinds,          // 2
	eHorizontalBlindsCrossing,  // 3
	eVerticalBlinds             // 4
};

enum    enMidiCommands {
	eMIDI_OpenAndPlay = 0,      // 0
	eMIDI_StopAndClose,         // 1
	eMIDI_Play,                 // 2
	eMIDI_Stop,                 // 3
	eMIDI_RewindAndPlay,        // 4
	eMIDI_GetCurrentDev         // 5
};

enum    enWaveCommands {
	eWAVE_LoadFormArcAndPlay = 2,   // 2
	eWAVE_Stop,                 // 3
	eWAVE_StoreFilename         // 4
};

enum    enVideoCommands {
	eVideo_OpenAndPlay = 0,     // 0
	eVideo_StopAndClose,        // 1
	eVideo_Play,                // 2
	eVideo_Stop,                // 3
	eVideo_RewindAndPlay,       // 4
	eVideo_GetCurrentDev        // 5
};

enum    enBgmExtrasCommands {
	ePlayButton = 0,            // 0
	eStopButton,                // 1
	eFBButton,                  // 2
	eFFButton,                  // 3
	eUpdateBGMName,             // 4
	eShutdown                   // 5
};

enum    enEngineCommands {
	eProcessScript = 0,         // 0
	eShowIntro,                 // 1
	eProcessMainMenu,           // 2
	eLoadSave,                  // 3
	eReserved                   // 4
};

enum    enMainMenuCommands {
	eStart = 0,                 // 0
	eSave,                      // 1
	eLoad,                      // 2
	eConfig,                    // 3
	eEnd                        // 4
};

enum    enConfigMenuCommands {
	eVoice = 0,                 // 0
	eSound,                     // 1
	eBGM,                       // 2
	eVGA                        // 3
};

enum    enExtrasMenuCommands {
	eCgHitDisplay = 0,          // 0
	eExtras,                    // 1
	eSaveBmp                    // 2
};

enum    enYesNoMenuCommands {
	eYes = 0,                   // 0
	eNo                         // 1
};

#pragma pack(push, 1)

struct SaveFileState {
	char pcScriptFile[32];
	char pcBgPictureName[32];
	char pcFgPictureName[32];
	char pcReserved1[32];
	char pcCharacter1Name[32];
	char pcCharacter2Name[32];
	char pcFgOverlayName[32];
	char pcBgMusicName[32];
	long dwReserved2;
	long iPosInScript;
	long iSavedPosInScript;
	short psFlags[2000];    // 0 - 999 temporary flags, 1000 - 1999 persistent flags
};

struct SysFileStruc {
	char pcSaveNames[SAVE_SLOT_COUNT][25];
	short sNamesStopSign;
	char bSaveExists[SAVE_SLOT_COUNT];
	char pcCurrentGameTime[32];
	short pbGameEventsAchieved[100];
	short bCgAvailable[1900];
};

struct ArhiveHdr {
	BYTE    bSign1;
	BYTE    bSign2;
	BYTE    bSign3;
	BYTE    bSign4;
	BYTE    bSign5;
	BYTE    bSign6;
	WORD    wReserved1;
	WORD    wEntryCount;
	DWORD   dwFileTableOffset;
	WORD    wReserved2;
};

struct ArhiveFileEntry {
	char    pcFileName[MAX_ARCH_NAME_SIZE];
	DWORD   dwFileSize;
};

struct PackedFileHdr {
	BYTE    bSign1;
	BYTE    bSign2;
	DWORD   dwPackedSize;
	DWORD   dwUnpackedSize;
};

struct OFFSET_STRUCT {
	short   x;
	short   y;
};

struct MyRect {
	short   sLeft;
	short   sTop;
	short   sRight;
	short   sBottom;
};

struct BITMAP_FILE {
	BITMAPFILEHEADER    bmfh;
	BITMAPINFO          bmi;
};

#pragma pack(pop)

// Forward declarations of functions included in this code module:
ATOM                RegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void                SetupOrigin(HWND hWnd, HDC hdc);
int                 InitializeGame(HWND hWnd);
void                InitPalette();
DWORD               ReadFileIntoBuffer(const TCHAR* lpFileName, LPVOID lpBuffer);
void                UpdateTimeStamp(DWORD ulTimeStamp);
void                SetWindowSize(HWND hWnd, int iWidth, int iHeight);
void                CenterTheWindow(HWND hWnd);
bool                LoadSGArhiveTable(TCHAR* ptFileName);
bool                LoadWVArhiveTable(TCHAR* ptFileName);
bool                FindAndReadFileInSG(const TCHAR* cptFileName, LPVOID lpBuffer);
int                 MarkCGAsSeen(const char* pcFileToFind);                             // <<<--- Needs correction
void                MakeCapsAndCatExt(const TCHAR* cptFName, const TCHAR* cptFExt);
bool                UnpackData(void* pvDstBuf, void* pvSrcBuf, DWORD ulReceivingBufSize);
int                 FindPicOrigin(short sWidth, short sHeight);
void                FreeSGArhiveFileTable();
void                FreeWVArhiveFileTable();
DWORD               OverwriteFile(const TCHAR* cptFileName, LPCVOID lpBuffer, DWORD dwNumberOfBytesToWrite);
MCIERROR            MIDIPlaybackCtrl(HWND hWnd, int iMidiCommand, const TCHAR* cptFileToPlay, bool bMidiIsPlaying);
bool                LoadWVFile(const TCHAR* cptFileToFind, LPVOID lpBuffer);
bool                WavePlaybackCtrl(const TCHAR* cptFileName, int iCommand);
MCIERROR            VideoPlaybackCtrl(HWND hWnd, int iCommand, const TCHAR* cptFileToPlay);
void                ShutdownGame();
INT_PTR CALLBACK    Dialog_Func(HWND hWndDlg, UINT uiMsg, WPARAM wParam, LPARAM lParam);
WORD                CheckKeysToProc(WORD sKeyMask);
short               VideoModeAndIntro();
void                EraseBGBlitWEffects(short sEffect);
BOOL                BlitFromBuffersToScreen(int iDstX, int iDstY, int iCX, int iCY, int iSrcX, int iSrcY);
void                BlitPicWithEffects(short sEffect);
void                CopyPictureWithTranspColor(unsigned short TransferType, unsigned short xOffs, unsigned short yOffs);
int                 BitBltWithTranspColor(HDC hdcDst, unsigned short usDstX, unsigned short usDstY, unsigned short usCX, unsigned short usCY, HDC hdcSrc, unsigned short usSrcX, unsigned short usSrcY, COLORREF crTranspColor);
WORD                WaitForKeyPress();
void                BlitWaitingAnimation();
short               ProcessMenu(int iX, int iY, short iMaxMenuItemCnt, const TCHAR* ptMenuStrings);
bool                CopyPartOfString(int iSrcOffset, int iDstOffset, TCHAR* ptString);
bool                CheckIfContainsSpaces(int iStrLength, TCHAR* ptStr);
void                SplitStringsToFitOnScreen(TCHAR* ptString);
int                 OutputString(HDC hCDC, HDC hDC, int x, int y, signed int iMaxLen, TCHAR* pcString, DWORD dwSleepTimeBetweenChars);
short               ShowMenuAndWaitForSel(int iX, int iY, short sMenuItemsPerColumn);
BOOL                RestoreImage(int x, int y, int cx, int cy, int x1, int y1);
bool                LoadScriptFile(const TCHAR* cptScriptName);
bool                LoadPicWithTransparency(int x, int y, const TCHAR* cptCGName);
bool                OverlayPicWithTransparency(int iX, int iY);
void                LoadSave(short sSaveFileToLoad);
DWORD               WriteToFile(const TCHAR* cptFileName, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite);
DWORD               SaveScreenshot(HDC hdcSrc, BITMAP_FILE* lpBuffer);
short               CGHitDisplay();
void                ProcessBGMInExtraMode(short sCmd);
int                 ShowCG(const TCHAR* cptCGName);
int                 ShowCG2(const TCHAR* cptCGName);
short               ShowCGNumberX(short iCGnum);
short               PrepareExtraModeMenu(short sCGPage);
short               ExtraModeMenu();
short               SelectItemInGraphicalMode();
short               ProcessRightMenu();
short               ProcessMainStartMenu();
int                 ShowCredits();
int                 ConvertMBCSToUni(const char* cpcMBString, wchar_t* pwcUString, int iUStringSize);
int                 ConvertUniToMBCS(const wchar_t* cpwUString, char* pcMBString, int iMBStringSize);
int                 DimScreenToBlack();
int                 DimmScreenToWhite();
void                SaveGame(short sSaveFileNumber);
short               ProcessInGameMenu();
bool                LoadAndBlitPicToBuf2(int a1, unsigned short iOriginPos, const TCHAR* cptFileName);
DWORD WINAPI        MainGameThreadProc(LPVOID lpParameter);
bool                StopMidiPlayback();
bool                PlayMidiFile(const TCHAR* cptFileNameToPlay, bool bMidiIsPlaying);
