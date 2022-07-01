//-----------------------------------------------------------------------------
// Copyright (C) Proxmark3 contributors. See AUTHORS.md for details.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// See LICENSE.txt for the text of the license.
//-----------------------------------------------------------------------------
// High frequency proximity cards from TEXCOM commands
//-----------------------------------------------------------------------------

#ifndef CMDHFTEXCOM_H__
#define CMDHFTEXCOM_H__

#include "common.h"
#include "pm3_cmd.h"

enum TK17Bits {
    TK17WrongBit,
    TK17Bit00,
    TK17Bit01,
    TK17Bit10,
    TK17Bit11
};

enum TexkomModulation {
    TexkomModError,
    TexkomModTK13,
    TexkomModTK17
};

int CmdHFTexkom(const char *Cmd);

#endif
