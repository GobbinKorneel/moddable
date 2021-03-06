/*
 * Copyright (c) 2019  Moddable Tech, Inc.
 *
 *   This file is part of the Moddable SDK Runtime.
 *
 *   The Moddable SDK Runtime is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   The Moddable SDK Runtime is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with the Moddable SDK Runtime.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "user_interface.h"
#include "xsmc.h"
#ifdef __ets__
	#include "xsHost.h"		// esp platform support
#else
	#error - unsupported platform
#endif
#include "builtinCommon.h"

static uint32_t gDigitalAvailable =
	(1 <<  0) |
	(1 <<  1) |
	(1 <<  2) |
	(1 <<  3) |
	(1 <<  4) |
	(1 <<  5) |
	(1 << 12) |
	(1 << 13) |
	(1 << 14) |
	(1 << 15) |
	(1 << 16);

uint8_t builtinArePinsFree(uint32_t pins)
{
	return (pins == (gDigitalAvailable & pins)) ? 1 : 0;
}

uint8_t builtinUsePins(uint32_t pins)
{
	if (pins != (gDigitalAvailable & pins))
		return 0;

	gDigitalAvailable &= ~pins;
	return 1;
}

void builtinFreePins(uint32_t pins)
{
	gDigitalAvailable |= pins;
}

uint8_t builtinHasCallback(xsMachine *the, xsSlot *target, xsIndex id)
{
	if (xsmcHas(xsArg(0), id))
		return 1;

	if (xsmcHas(*target, id))
		return 1;

	return 0;
}

uint8_t builtinGetCallback(xsMachine *the, xsSlot *target, xsIndex id, xsSlot *slot)
{
	if (xsmcHas(xsArg(0), id)) {
		xsmcGet(*slot, xsArg(0), id);
		return 1;
	}

	if (xsmcHas(*target, id)) {
		xsmcGet(*slot, *target, id);
		return 1;
	}

	return 0;
}

void builtinGetFormat(xsMachine *the, uint8_t format)
{
	if (kIOFormatByte == format)
		xsmcSetString(xsResult, "number");
	else if (kIOFormatBuffer == format)
		xsmcSetString(xsResult, "buffer");
	else
		xsRangeError("bad format");
}

uint8_t builtinSetFormat(xsMachine *the)
{
	char *format = xsmcToString(xsArg(0));

	if (!c_strcmp("number", format))
		return kIOFormatByte;
	if (!c_strcmp("buffer", format))
		return kIOFormatBuffer;
	xsRangeError("unimplemented");
}
