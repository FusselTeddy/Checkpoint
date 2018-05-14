/*
*   This file is part of Checkpoint
*   Copyright (C) 2017-2018 Bernardo Giordano
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

#include "archive.hpp"

static FS_Archive mSdmc;
static Mode_t mMode = MODE_SAVE;

Mode_t Archive::mode(void)
{
    return mMode;
}

void Archive::mode(Mode_t v)
{
    mMode = v;
}

Result Archive::init(void)
{
    return FSUSER_OpenArchive(&mSdmc, ARCHIVE_SDMC, fsMakePath(PATH_EMPTY, ""));
}

void Archive::exit(void)
{
    FSUSER_CloseArchive(mSdmc);
}

FS_Archive Archive::sdmc(void)
{
    return mSdmc;
}

Result Archive::save(FS_Archive* archive, FS_MediaType mediatype, u32 lowid, u32 highid)
{
    const u32 path[3] = { mediatype, lowid, highid };
    return FSUSER_OpenArchive(archive, ARCHIVE_USER_SAVEDATA, {PATH_BINARY, 12, path});
}

Result Archive::extdata(FS_Archive* archive, u32 ext)
{
    const u32 path[3] = { MEDIATYPE_SD, ext, 0 };
    return FSUSER_OpenArchive(archive, ARCHIVE_EXTDATA, {PATH_BINARY, 12, path});
}

bool Archive::accessible(FS_MediaType mediatype, u32 lowid, u32 highid)
{
    FS_Archive archive;
    Result res = save(&archive, mediatype, lowid, highid);
    if (R_SUCCEEDED(res))
    {
        FSUSER_CloseArchive(archive);
        return true;
    }
    return false;
}

bool Archive::accessible(u32 ext)
{
    FS_Archive archive;
    Result res = extdata(&archive, ext);
    if (R_SUCCEEDED(res))
    {
        FSUSER_CloseArchive(archive);
        return true;
    }
    return false;
}