// ScriptDumper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define GET_BYTE(pBase, lOffset)        (*(BYTE *)((BYTE *)(pBase) + (lOffset)))
#define GET_WORD(pBase, lOffset)        (*(WORD *)((BYTE *)(pBase) + (lOffset)))
#define GET_DWORD(pBase, lOffset)       (*(DWORD *)((BYTE *)(pBase) + (lOffset)))
#define GET_STRING(pBase, lOffset)      ((char *)(pBase) + (lOffset))

#define CVT_BUF_SIZE    512

int ConvertMBCSToUni(const char *cpcMBString, wchar_t *pwcUString, int iUStringSize)
{
    return MultiByteToWideChar(932 /* Shift-JIS */, 0, cpcMBString, -1, pwcUString, iUStringSize);
}

int _tmain(int argc, _TCHAR* argv[])
{
    HANDLE  hFile;
    HANDLE  hFileMap;
    DWORD   dwOffset, dwCmdOffset;
    WORD    wCommand;
    void    *pvBuffer;
    LARGE_INTEGER   ilFileSize;
    char    *pcText;
    TCHAR   ptCvtString[CVT_BUF_SIZE];
    TCHAR   ptCvtString2[CVT_BUF_SIZE];
    bool    bError;

    DWORD   dwArg1;
    WORD    wArg1, wArg2, wArg3, wArg4;
    BYTE    bArg1;

    if (argv[1] == NULL){
        _tprintf(_T("Use as: ScriptDumper.exe <ScriptFile>"));
        return -1;
    }

    hFile = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE){
        _tprintf(_T("Can't open script file: %s"), argv[1]);
        return -2;
    }

    hFileMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (hFileMap == INVALID_HANDLE_VALUE){
        _tprintf(_T("Couldn't create file mapping: %s"), argv[1]);
        CloseHandle(hFile);
        return -3;
    }

    pvBuffer = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
    if (pvBuffer == NULL){
        _tprintf(_T("Couldn't map view of file: %s"), argv[1]);
        CloseHandle(hFileMap);
        CloseHandle(hFile);
        return -4;
    }

    GetFileSizeEx(hFile, &ilFileSize);
    dwOffset = 0;
    bError = false;

    do{
        dwCmdOffset = dwOffset;
        wCommand = GET_WORD(pvBuffer, dwOffset);
        dwOffset += 2;
        switch (wCommand){
        case 0x0000:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Show text and wait input\n\tParameter 1: %s - text\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        case 0x0001:
            dwArg1 = GET_DWORD(pvBuffer, dwOffset);
            dwOffset += 4;
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Insert one menu item\n\tParameter 1: %d - jump address in script\n\tParameter 2: %s - menu item label\n"), dwCmdOffset, wCommand, dwArg1, ptCvtString);
            break;

        case 0x0002:
            dwArg1 = GET_DWORD(pvBuffer, dwOffset);
            dwOffset += 4;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Jump in script command\n\tParameter 1: %d - jump address in script\n"), dwCmdOffset, wCommand, dwArg1);
            break;

        case 0x0003:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg2 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg3 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Store value into multiple flags\n\tParameter 1: %d - start index\n\tParameter 2: %d - end index\n\tParameter 3: %d - value\n"), dwCmdOffset, wCommand, wArg1, wArg2, wArg3);
            break;

        case 0x0004:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            bArg1 = GET_BYTE(pvBuffer, dwOffset);
            dwOffset += 1;
            wArg2 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Process flag math commands\n\tParameter 1: %d - flag index\n\tParameter 2: %C - flag command\n\tParameter 3: %d - value\n"), dwCmdOffset, wCommand, wArg1, bArg1, wArg2);
            break;

        case 0x0005:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Wait for key press command\n"), dwCmdOffset, wCommand);
            break;

        case 0x0006:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Save position in script & prepare for new menu\n"), dwCmdOffset, wCommand);
            break;

        case 0x0007:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Process main menus and in-game menu\n"), dwCmdOffset, wCommand);
            break;

        case 0x000A:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Restore saved position\n"), dwCmdOffset, wCommand);
            break;

        case 0x000D:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Skip 2 bytes and continue (NOP?)\n\tSkipped bytes: %d\n"), dwCmdOffset, wCommand, wArg1);
            break;

        case 0x000E:
        case 0x000F:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg2 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Read flag Index and Value\n\tParameter 1: %d - flag index\n\tParameter 2: %d - value\n"), dwCmdOffset, wCommand, wArg1, wArg2);
            break;

        case 0x0010:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            bArg1 = GET_BYTE(pvBuffer, dwOffset);
            dwOffset += 1;
            wArg2 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            dwArg1 = GET_DWORD(pvBuffer, dwOffset);
            dwOffset += 4;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Compare flag and jump if condition false\n\tParameter 1: %d - flag index\n\tParameter 2: %C - comparison command\n\tParameter 3: %d - value\n\tParameter 4: %d - jump address in script\n"), dwCmdOffset, wCommand, wArg1, bArg1, wArg2, dwArg1);
            break;

        case 0x0011:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Sleep command\n\tParameter 1: %d - sleep time [ms]\n"), dwCmdOffset, wCommand, wArg1 * 10);
            break;

        case 0x0012:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Sleep and wait for key press command\n\tParameter 1: %d - sleep time [ms]\n"), dwCmdOffset, wCommand, wArg1 * 10);
            break;

        case 0x0013:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Load and display BG command\n\tParameter 1: %s - BG file name\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        case 0x0014:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Blit CG with effect command\n\tParameter 1: %d - effect\n"), dwCmdOffset, wCommand, wArg1);
            break;

        case 0x0016:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Load and display CG with transparent color command, remove personages\n\tParameter 1: %s - CG file name\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        case 0x0017:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg2 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Remove image from screen with effect\n\tParameter 1: %d - ?\n\tParameter 2: %d - effect\n"), dwCmdOffset, wCommand, wArg1, wArg2);
            break;

        case 0x0018:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Load script command\n\tParameter 1: %s - script file name\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        case 0x0019:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Show credits and return to start screen\n"), dwCmdOffset, wCommand);
            break;

        case 0x001E:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Dim screen to black\n"), dwCmdOffset, wCommand);
            break;

        case 0x001F:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Dim screen to white\n"), dwCmdOffset, wCommand);
            break;

        case 0x0025:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg2 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg3 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Store random value into a flag\n\tParameter 1: %d - flag index\n\tParameter 2: %d - range start\n\tParameter 3: %d - range end\n"), dwCmdOffset, wCommand, wArg1, wArg2, wArg3);
            break;

        case 0x0026:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Play BG music\n\tParameter 1: %s - BG music file name\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        case 0x0028:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Stop BG music playback\n"), dwCmdOffset, wCommand);
            break;

        case 0x0029:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Skip cycle command\n"), dwCmdOffset, wCommand);
            break;

        case 0x002B:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Play wave file (voice)\n\tParameter 1: %s - wave file name\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        case 0x0030:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg2 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg3 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg4 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Load pic origin and size\n\tParameter 1: %d - origin x\n\tParameter 2: %d - origin y\n\tParameter 3: %d - width\n\tParameter 4: %d - height\n"), dwCmdOffset, wCommand, wArg1, wArg2, wArg3, wArg4);
            break;

        case 0x0031:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Finished some point in game\n"), dwCmdOffset, wCommand);
            break;

        case 0x0032:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Set g_bDWORD_41CB3C false\n"), dwCmdOffset, wCommand);
            break;

        case 0x0033:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Set g_bDWORD_41CB3C true\n"), dwCmdOffset, wCommand);
            break;

        case 0x0035:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Play wave file (effects)\n\tParameter 1: %s - wave file name\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        case 0x0036:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Stop wave playback\n"), dwCmdOffset, wCommand);
            break;

        case 0x0037:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString2, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Show 2 pics with horizontal blinds effect\n\tParameter 1: %s - pic 1 file name\n\tParameter 2: %s - pic 2 file name\n"), dwCmdOffset, wCommand, ptCvtString, ptCvtString2);
            break;

        case 0x0038:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - ?\n"), dwCmdOffset, wCommand);
            break;

        case 0x0040:
            dwArg1 = GET_DWORD(pvBuffer, dwOffset);
            dwOffset += 4;
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg2 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg3 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg4 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Load graphical menu item's link in script and bounding rectangle on screen\n\tParameter 1: %d - new address in script\n\tParameter 2: %d - left\n\tParameter 3: %d - top\n\tParameter 4: %d - right\n\tParameter 5: %d - bottom\n"), dwCmdOffset, wCommand, dwArg1, wArg1, wArg2, wArg3, wArg4);
            break;

        case 0x0041:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Select from graphical menu\n"), dwCmdOffset, wCommand);
            break;

        case 0x0042:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            wArg2 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Output string at specified coordinates\n\tParameter 1: %d - x\n\tParameter 2: %d - y\n\tParameter 3: %s - pic file name\n"), dwCmdOffset, wCommand, wArg1, wArg2, ptCvtString);
            break;

        case 0x0043:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Paint black and restore image\n"), dwCmdOffset, wCommand);
            break;

        case 0x0044:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Load picture to buffer right from screen\n\tParameter 1: %d - ?\n\tParameter 2: %s - pic file name\n"), dwCmdOffset, wCommand, wArg1, ptCvtString);
            break;

        case 0x0045:
            dwArg1 = GET_DWORD(pvBuffer, dwOffset);
            dwOffset += 4;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - ?\n\tParameter 1: %d - ?\n"), dwCmdOffset, wCommand, dwArg1);
            break;

        case 0x0046:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Display BG command, clear other parts\n\tParameter 1: %s - BG file name\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        case 0x0047:
        case 0x0048:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Load and display CG command\n\tParameter 1: %s - CG file name\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        case 0x0049:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Set required action to 4 - no meaning\n"), dwCmdOffset, wCommand);
            break;

        case 0x004A:
            wArg1 = GET_WORD(pvBuffer, dwOffset);
            dwOffset += 2;

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Blit picture with specified effect from bottom of screen (show clear background)\n\tParameter 1: %d - effect\n"), dwCmdOffset, wCommand, wArg1);
            break;

        case 0x004B:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Load and show personage\n\tParameter 1: %s - pic file name\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        case 0x004C:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString2, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Show two personages on screen\n\tParameter 1: %s - personage 1 pic file name\n\tParameter 2: %s - personage 2 pic file name\n"), dwCmdOffset, wCommand, ptCvtString, ptCvtString2);
            break;

        case 0x004D:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Show a light flash, the end of game\n"), dwCmdOffset, wCommand);
            break;

        case 0x004E:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Slide image I_101A over main image I_101\n"), dwCmdOffset, wCommand);
            break;

        case 0x004F:
            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Slide image I_101B over image I_101A\n"), dwCmdOffset, wCommand);
            break;

        case 0x0050:
            pcText = GET_STRING(pvBuffer, dwOffset);
            dwOffset += strlen(pcText) + 1;
            ConvertMBCSToUni(pcText, ptCvtString, CVT_BUF_SIZE);

            _tprintf(_T("Offset %06d\n\tCommand: 0x%04X - Set current game time\n\tParameter 1: %s - time\n"), dwCmdOffset, wCommand, ptCvtString);
            break;

        default:
            _tprintf(_T("Offset %06d\n\tUnknown command: 0x%04X\n"), dwCmdOffset, wCommand);
            bError = true;
            break;
        }
    } while (dwOffset < ilFileSize.LowPart && !bError);

    UnmapViewOfFile(pvBuffer);
    CloseHandle(hFileMap);
    CloseHandle(hFile);

    return 0;
}