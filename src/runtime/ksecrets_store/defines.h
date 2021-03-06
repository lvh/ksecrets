/*
    This file is part of the KDE Libraries

    Copyright (C) 2015 Valentin Rusu (valir@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB. If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#ifndef KSECRETS_DEFINES_H
#define KSECRETS_DEFINES_H

#include <syslog.h>

#define KSS_LOG_DEBUG (LOG_AUTH | LOG_DEBUG)
#define KSS_LOG_INFO (LOG_AUTH | LOG_INFO)
#define KSS_LOG_ERR (LOG_AUTH | LOG_ERR)

#define KSECRETS_ITERATIONS 50000

#define FALSE 0
#define TRUE 1
#define UNUSED(x) (void)(x)

#endif
