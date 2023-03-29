/*
*   This file is part of Luma3DS
*   Copyright (C) 2016-2020 Aurora Wright, TuxSH
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include <3ds.h>
#include <3ds/os.h>
#include "menus.h"
#include "menu.h"
#include "draw.h"
#include "menus/process_list.h"
#include "menus/n3ds.h"
#include "menus/debugger.h"
#include "menus/miscellaneous.h"
#include "menus/sysconfig.h"
#include "menus/screen_filters.h"
#include "plugin.h"
#include "ifile.h"
#include "memory.h"
#include "fmt.h"
#include "process_patches.h"
#include "luminance.h"
#include "luma_config.h"

Menu rosalinaMenu = {
    "Menu Rosalina",
    {
        { "Faire une capture d'ecran", METHOD, .method = &RosalinaMenu_TakeScreenshot },
        { "Changer la luminosite", METHOD, .method = &RosalinaMenu_ChangeScreenBrightness },
        { "Triche...", METHOD, .method = &RosalinaMenu_Cheats },
        { "", METHOD, .method = PluginLoader__MenuCallback},
        { "Liste de processus", METHOD, .method = &RosalinaMenu_ProcessList },
        { "Options de debogage...", MENU, .menu = &debuggerMenu },
        { "Configuration systeme...", MENU, .menu = &sysconfigMenu },
        { "Filtres d'ecran...", MENU, .menu = &screenFiltersMenu },
        { "Menu New 3DS...", MENU, .menu = &N3DSMenu, .visibility = &menuCheckN3ds },
        { "Options diverses...", MENU, .menu = &miscellaneousMenu },
        { "Sauvegarder les parametres", METHOD, .method = &RosalinaMenu_SaveSettings },
        { "Eteindre", METHOD, .method = &RosalinaMenu_PowerOff },
        { "Redemarrer", METHOD, .method = &RosalinaMenu_Reboot },
        { "Remerciements", METHOD, .method = &RosalinaMenu_ShowCredits },
        { "Infos de debogage", METHOD, .method = &RosalinaMenu_ShowDebugInfo, .visibility = &rosalinaMenuShouldShowDebugInfo },
        {},
    }
};

bool rosalinaMenuShouldShowDebugInfo(void)
{
    // Don't show on release builds

    s64 out;
    svcGetSystemInfo(&out, 0x10000, 0x200);
    return out == 0;
}

void RosalinaMenu_SaveSettings(void)
{
    Result res = LumaConfig_SaveSettings();
    Draw_Lock();
    Draw_ClearFramebuffer();
    Draw_FlushFramebuffer();
    Draw_Unlock();

    do
    {
        Draw_Lock();
        Draw_DrawString(10, 10, COLOR_TITLE, "Sauvegarder les parametres");
        if(R_SUCCEEDED(res))
            Draw_DrawString(10, 30, COLOR_WHITE, "Operation reussie.");
        else
            Draw_DrawFormattedString(10, 30, COLOR_WHITE, "Echec de l'operation (0x%08lx).", res);
        Draw_FlushFramebuffer();
        Draw_Unlock();
    }
    while(!(waitInput() & KEY_B) && !menuShouldExit);
}

void RosalinaMenu_ShowDebugInfo(void)
{
    Draw_Lock();
    Draw_ClearFramebuffer();
    Draw_FlushFramebuffer();
    Draw_Unlock();

    char memoryMap[512];
    formatMemoryMapOfProcess(memoryMap, 511, CUR_PROCESS_HANDLE);

    s64 kextAddrSize;
    svcGetSystemInfo(&kextAddrSize, 0x10000, 0x300);
    u32 kextPa = (u32)((u64)kextAddrSize >> 32);
    u32 kextSize = (u32)kextAddrSize;

    u32 kernelVer = osGetKernelVersion();
    FS_SdMmcSpeedInfo speedInfo;

    do
    {
        Draw_Lock();
        Draw_DrawString(10, 10, COLOR_TITLE, "Rosalina -- Debug info");

        u32 posY = Draw_DrawString(10, 30, COLOR_WHITE, memoryMap);
        posY = Draw_DrawFormattedString(10, posY, COLOR_WHITE, "Kernel ext PA: %08lx - %08lx\n\n", kextPa, kextPa + kextSize);
        posY = Draw_DrawFormattedString(
            10, posY, COLOR_WHITE, "Kernel version: %lu.%lu-%lu\n",
            GET_VERSION_MAJOR(kernelVer), GET_VERSION_MINOR(kernelVer), GET_VERSION_REVISION(kernelVer)
        );
        if (mcuFwVersion != 0)
        {
            posY = Draw_DrawFormattedString(
                10, posY, COLOR_WHITE, "MCU FW version: %lu.%lu\n",
                GET_VERSION_MAJOR(mcuFwVersion), GET_VERSION_MINOR(mcuFwVersion)
            );
        }

        if (R_SUCCEEDED(FSUSER_GetSdmcSpeedInfo(&speedInfo)))
        {
            u32 clkDiv = 1 << (1 + (speedInfo.sdClkCtrl & 0xFF));
            posY = Draw_DrawFormattedString(
                10, posY, COLOR_WHITE, "SDMC speed: HS=%d %lukHz\n",
                (int)speedInfo.highSpeedModeEnabled, SYSCLOCK_SDMMC / (1000 * clkDiv)
            );
        }
        if (R_SUCCEEDED(FSUSER_GetNandSpeedInfo(&speedInfo)))
        {
            u32 clkDiv = 1 << (1 + (speedInfo.sdClkCtrl & 0xFF));
            posY = Draw_DrawFormattedString(
                10, posY, COLOR_WHITE, "NAND speed: HS=%d %lukHz\n",
                (int)speedInfo.highSpeedModeEnabled, SYSCLOCK_SDMMC / (1000 * clkDiv)
            );
        }
        {
            posY = Draw_DrawFormattedString(
                10, posY, COLOR_WHITE, "APPMEMTYPE: %lu\n",
                OS_KernelConfig->app_memtype
            );
        }
        Draw_FlushFramebuffer();
        Draw_Unlock();
    }
    while(!(waitInput() & KEY_B) && !menuShouldExit);
}

void RosalinaMenu_ShowCredits(void)
{
    Draw_Lock();
    Draw_ClearFramebuffer();
    Draw_FlushFramebuffer();
    Draw_Unlock();

    do
    {
        Draw_Lock();
        Draw_DrawString(10, 10, COLOR_TITLE, "Rosalina -- Luma3DS-FR remerciements");

        u32 posY = Draw_DrawString(10, 30, COLOR_WHITE, "Luma3DS (c) 2016-2023 AuroraWright, TuxSH") + SPACING_Y;

        posY = Draw_DrawString(10, posY + SPACING_Y, COLOR_WHITE, "Chargement de code 3DSX par fincs");
        posY = Draw_DrawString(10, posY + SPACING_Y, COLOR_WHITE, "InputRedirection par Stary (PoC par ShinyQuagsire)");
        posY = Draw_DrawString(10, posY + SPACING_Y, COLOR_WHITE, "3GX Loader par Nanquitas");
        posY = Draw_DrawString(10, posY + SPACING_Y, COLOR_WHITE, "Traduction en francais par BlackFalcon1961");

        posY += 2 * SPACING_Y;

        Draw_DrawString(10, posY, COLOR_WHITE,
            (
                "Remerciement special a :\n"
                " - fincs, WinterMute, mtheall, piepie62,\n"
                " - Contributeurs Luma3DS, contributeurs libctru,\n"
                " - autres personnes"
            ));

        Draw_FlushFramebuffer();
        Draw_Unlock();
    }
    while(!(waitInput() & KEY_B) && !menuShouldExit);
}

void RosalinaMenu_Reboot(void)
{
    Draw_Lock();
    Draw_ClearFramebuffer();
    Draw_FlushFramebuffer();
    Draw_Unlock();

    do
    {
        Draw_Lock();
        Draw_DrawString(10, 10, COLOR_TITLE, "Redemarrer");
        Draw_DrawString(10, 30, COLOR_WHITE, "Pressez A pour redemarrer, B pour revenir.");
        Draw_FlushFramebuffer();
        Draw_Unlock();

        u32 pressed = waitInputWithTimeout(1000);

        if(pressed & KEY_A)
        {
            menuLeave();
            APT_HardwareResetAsync();
            return;
        } else if(pressed & KEY_B)
            return;
    }
    while(!menuShouldExit);
}

void RosalinaMenu_ChangeScreenBrightness(void)
{
    Draw_Lock();
    Draw_ClearFramebuffer();
    Draw_FlushFramebuffer();
    Draw_Unlock();

    // gsp:LCD GetLuminance is stubbed on O3DS so we have to implement it ourselves... damn it.
    // Assume top and bottom screen luminances are the same (should be; if not, we'll set them to the same values).
    u32 luminance = getCurrentLuminance(false);
    u32 minLum = getMinLuminancePreset();
    u32 maxLum = getMaxLuminancePreset();

    do
    {
        Draw_Lock();
        Draw_DrawString(10, 10, COLOR_TITLE, "Luminosite ecran");
        u32 posY = 30;
        posY = Draw_DrawFormattedString(
            10,
            posY,
            COLOR_WHITE,
            "Luminosite actuelle: %lu (min. %lu, max. %lu)\n\n",
            luminance,
            minLum,
            maxLum
        );
        posY = Draw_DrawString(10, posY, COLOR_WHITE, "Controles: Haut/Bas pour +-1, Droite/Gauche pour +-10.\n");
        posY = Draw_DrawString(10, posY, COLOR_WHITE, "Pressez A pour demarrer, B pour quitter.\n\n");

        posY = Draw_DrawString(10, posY, COLOR_RED, "ATTENTION: \n");
        posY = Draw_DrawString(10, posY, COLOR_WHITE, "  * les valeurs sont limitees par les presets.\n");
        posY = Draw_DrawString(10, posY, COLOR_WHITE, "  * le framebuffer inferieur sera restaure quand\nvous quitterez.");
        Draw_FlushFramebuffer();
        Draw_Unlock();

        u32 pressed = waitInputWithTimeout(1000);

        if (pressed & KEY_A)
            break;

        if (pressed & KEY_B)
            return;
    }
    while (!menuShouldExit);

    Draw_Lock();

    Draw_RestoreFramebuffer();
    Draw_FreeFramebufferCache();

    svcKernelSetState(0x10000, 2); // unblock gsp
    gspLcdInit(); // assume it doesn't fail. If it does, brightness won't change, anyway.

    // gsp:LCD will normalize the brightness between top/bottom screen, handle PWM, etc.

    s32 lum = (s32)luminance;

    do
    {
        u32 pressed = waitInputWithTimeout(1000);
        if (pressed & DIRECTIONAL_KEYS)
        {
            if (pressed & KEY_UP)
                lum += 1;
            else if (pressed & KEY_DOWN)
                lum -= 1;
            else if (pressed & KEY_RIGHT)
                lum += 10;
            else if (pressed & KEY_LEFT)
                lum -= 10;

            lum = lum < (s32)minLum ? (s32)minLum : lum;
            lum = lum > (s32)maxLum ? (s32)maxLum : lum;

            // We need to call gsp here because updating the active duty LUT is a bit tedious (plus, GSP has internal state).
            // This is actually SetLuminance:
            GSPLCD_SetBrightnessRaw(BIT(GSP_SCREEN_TOP) | BIT(GSP_SCREEN_BOTTOM), lum);
        }

        if (pressed & KEY_B)
            break;
    }
    while (!menuShouldExit);

    gspLcdExit();
    svcKernelSetState(0x10000, 2); // block gsp again

    if (R_FAILED(Draw_AllocateFramebufferCache(FB_BOTTOM_SIZE)))
    {
        // Shouldn't happen
        __builtin_trap();
    }
    else
        Draw_SetupFramebuffer();

    Draw_Unlock();
}

void RosalinaMenu_PowerOff(void) // Soft shutdown.
{
    Draw_Lock();
    Draw_ClearFramebuffer();
    Draw_FlushFramebuffer();
    Draw_Unlock();

    do
    {
        Draw_Lock();
        Draw_DrawString(10, 10, COLOR_TITLE, "Eteindre");
        Draw_DrawString(10, 30, COLOR_WHITE, "Pressez A pour eteindre, B pour revenir.");
        Draw_FlushFramebuffer();
        Draw_Unlock();

        u32 pressed = waitInputWithTimeout(1000);

        if(pressed & KEY_A)
        {
            menuLeave();
            srvPublishToSubscriber(0x203, 0);
            return;
        }
        else if(pressed & KEY_B)
            return;
    }
    while(!menuShouldExit);
}


#define TRY(expr) if(R_FAILED(res = (expr))) goto end;

static s64 timeSpentConvertingScreenshot = 0;
static s64 timeSpentWritingScreenshot = 0;

static Result RosalinaMenu_WriteScreenshot(IFile *file, u32 width, bool top, bool left)
{
    u64 total;
    Result res = 0;
    u32 lineSize = 3 * width;
    u32 remaining = lineSize * 240;

    TRY(Draw_AllocateFramebufferCacheForScreenshot(remaining));

    u8 *framebufferCache = (u8 *)Draw_GetFramebufferCache();
    u8 *framebufferCacheEnd = framebufferCache + Draw_GetFramebufferCacheSize();

    u8 *buf = framebufferCache;
    Draw_CreateBitmapHeader(framebufferCache, width, 240);
    buf += 54;

    u32 y = 0;
    // Our buffer might be smaller than the size of the screenshot...
    while (remaining != 0)
    {
        s64 t0 = svcGetSystemTick();
        u32 available = (u32)(framebufferCacheEnd - buf);
        u32 size = available < remaining ? available : remaining;
        u32 nlines = size / lineSize;
        Draw_ConvertFrameBufferLines(buf, width, y, nlines, top, left);

        s64 t1 = svcGetSystemTick();
        timeSpentConvertingScreenshot += t1 - t0;
        TRY(IFile_Write(file, &total, framebufferCache, (y == 0 ? 54 : 0) + lineSize * nlines, 0)); // don't forget to write the header
        timeSpentWritingScreenshot += svcGetSystemTick() - t1;

        y += nlines;
        remaining -= lineSize * nlines;
        buf = framebufferCache;
    }
    end:

    Draw_FreeFramebufferCache();
    return res;
}

void RosalinaMenu_TakeScreenshot(void)
{
    IFile file;
    Result res = 0;

    char filename[64];
    char dateTimeStr[32];

    FS_Archive archive;
    FS_ArchiveID archiveId;
    s64 out;
    bool isSdMode;

    timeSpentConvertingScreenshot = 0;
    timeSpentWritingScreenshot = 0;

    if(R_FAILED(svcGetSystemInfo(&out, 0x10000, 0x203))) svcBreak(USERBREAK_ASSERT);
    isSdMode = (bool)out;

    archiveId = isSdMode ? ARCHIVE_SDMC : ARCHIVE_NAND_RW;
    Draw_Lock();
    Draw_RestoreFramebuffer();
    Draw_FreeFramebufferCache();

    svcFlushEntireDataCache();

    bool is3d;
    u32 topWidth, bottomWidth; // actually Y-dim

    Draw_GetCurrentScreenInfo(&bottomWidth, &is3d, false);
    Draw_GetCurrentScreenInfo(&topWidth, &is3d, true);

    res = FSUSER_OpenArchive(&archive, archiveId, fsMakePath(PATH_EMPTY, ""));
    if(R_SUCCEEDED(res))
    {
        res = FSUSER_CreateDirectory(archive, fsMakePath(PATH_ASCII, "/luma/screenshots"), 0);
        if((u32)res == 0xC82044BE) // directory already exists
            res = 0;
        FSUSER_CloseArchive(archive);
    }

    dateTimeToString(dateTimeStr, osGetTime(), true);

    sprintf(filename, "/luma/screenshots/%s_top.bmp", dateTimeStr);
    TRY(IFile_Open(&file, archiveId, fsMakePath(PATH_EMPTY, ""), fsMakePath(PATH_ASCII, filename), FS_OPEN_CREATE | FS_OPEN_WRITE));
    TRY(RosalinaMenu_WriteScreenshot(&file, topWidth, true, true));
    TRY(IFile_Close(&file));

    sprintf(filename, "/luma/screenshots/%s_bot.bmp", dateTimeStr);
    TRY(IFile_Open(&file, archiveId, fsMakePath(PATH_EMPTY, ""), fsMakePath(PATH_ASCII, filename), FS_OPEN_CREATE | FS_OPEN_WRITE));
    TRY(RosalinaMenu_WriteScreenshot(&file, bottomWidth, false, true));
    TRY(IFile_Close(&file));

    if(is3d && (Draw_GetCurrentFramebufferAddress(true, true) != Draw_GetCurrentFramebufferAddress(true, false)))
    {
        sprintf(filename, "/luma/screenshots/%s_top_right.bmp", dateTimeStr);
        TRY(IFile_Open(&file, archiveId, fsMakePath(PATH_EMPTY, ""), fsMakePath(PATH_ASCII, filename), FS_OPEN_CREATE | FS_OPEN_WRITE));
        TRY(RosalinaMenu_WriteScreenshot(&file, topWidth, true, false));
        TRY(IFile_Close(&file));
    }

end:
    IFile_Close(&file);

    if (R_FAILED(Draw_AllocateFramebufferCache(FB_BOTTOM_SIZE)))
        __builtin_trap(); // We're f***ed if this happens

    svcFlushEntireDataCache();
    Draw_SetupFramebuffer();
    Draw_Unlock();

    do
    {
        Draw_Lock();
        Draw_DrawString(10, 10, COLOR_TITLE, "Capture d'ecran");
        if(R_FAILED(res))
            Draw_DrawFormattedString(10, 30, COLOR_WHITE, "Echec de l'operation (0x%08lx).", (u32)res);
        else
        {
            u32 t1 = (u32)(1000 * timeSpentConvertingScreenshot / SYSCLOCK_ARM11);
            u32 t2 = (u32)(1000 * timeSpentWritingScreenshot / SYSCLOCK_ARM11);
            u32 posY = 30;
            posY = Draw_DrawString(10, posY, COLOR_WHITE, "Operation reussie.\n\n");
            posY = Draw_DrawFormattedString(10, posY, COLOR_WHITE, "Temps ecoule pour convertir:    %5lums\n", t1);
            posY = Draw_DrawFormattedString(10, posY, COLOR_WHITE, "Temps ecoule pour ecrire les fichiers: %5lums\n", t2);
        }

        Draw_FlushFramebuffer();
        Draw_Unlock();
    }
    while(!(waitInput() & KEY_B) && !menuShouldExit);

#undef TRY
}
