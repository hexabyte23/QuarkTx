/*
Command.cpp - QuarkTx
Copyright (c) 2015-2018 Thierry & Betrand WILMOT.  All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(QUARKTX_MAIN)
Q_DECLARE_LOGGING_CATEGORY(QUARKTX_LINK_SERIAL)
Q_DECLARE_LOGGING_CATEGORY(QUARKTX_LINK_BT)
Q_DECLARE_LOGGING_CATEGORY(QUARKTX_LINK_COMMAND)
Q_DECLARE_LOGGING_CATEGORY(QUARKTX_RCL)

#define QUARKTX_MIN_WAIT_DEVICE  3000

#endif
