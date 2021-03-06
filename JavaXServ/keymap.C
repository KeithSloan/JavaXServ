// This file is part of JavaXServ.
// http://SourceForge.Net/p/JavaXServ
// Copyright (C) Keith Sloan 2008-2013 keith@sloan-home.co.uk
//
// JavaXServ is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License
// as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// JavaXServ is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with JavaXServ.  If not, see <http://www.gnu.org/licenses/>.


static int keymap[399] = {
    0x5c, 0x7c, 0x00,   0x31, 0x21, 0x00,  0x32, 0x21, 0xb2,  /*  9, 10, 11 */
    0x33, 0xa3, 0xb3,   0x34, 0x24, 0x00,  0x35, 0x25, 0xbd,  /* 12, 13, 14 */ 
    0x36, 0x26, 0x00,   0x37, 0x27, 0x00,  0x38, 0x28, 0x00,  /* 15, 16, 17 */
    0x39, 0x29, 0xb1,   0x30, 0x23, 0xb0,  0x2d, 0x3d, 0x00,  // 18, 19, 20 
    0xac, 0xaf, 0x00,   0x00, 0x00, 0x00,  0xff08,  0, 0x00,  // 21, 22, 23
    0xff09, 0,  0x00,   0x71, 0x51, 0x00,  0x77, 0x57, 0x00,  // 24, 25, 26
    0x65, 0x45, 0x00,   0x72, 0x52, 0x00,  0x74, 0x54, 0x00,  // 27, 28, 29
    0x79, 0x59, 0x00,   0x75, 0x55, 0x00,  0x69, 0x49, 0x00,  // 30, 31, 32
    0x6f, 0x4f, 0x00,   0x70, 0x50, 0x00,  0x40, 0x60, 0x00,  // 33, 34, 35
    0x5b, 0x7b, 0x7e,   0x00, 0x00, 0x00,  0xffe5,  0, 0x00,  // 36, 37, 38
    0x61, 0x41, 0x00,   0x73, 0x53, 0x00,  0x64, 0x44, 0x00,  // 39, 40, 41
    0x66, 0x46, 0x00,   0x67, 0x47, 0x00,  0x68, 0x48, 0x00,  // 42, 43, 44
    0x6a, 0x4a, 0x00,   0x6b, 0x4b, 0x00,  0x6c, 0x4c, 0x00,  // 45, 46, 47
    0x3b, 0x2b, 0x00,   0x3a, 0x2a, 0x5e,  0x5d, 0x7d, 0x00,  // 48, 49, 50
    0xff0d, 0xff0d,0,   0xffe1,  0, 0x00,  0x7c, 0x5f, 0x00,  // 51, 52, 53
    0x7a, 0x5a, 0x00,   0x78, 0x58, 0x00,  0x63, 0x43, 0x00,  // 54, 55, 56
    0x76, 0x56, 0x00,   0x62, 0x42, 0x00,  0x6e, 0x4e, 0x00,  // 57, 58, 59
    0x6d, 0x4d, 0xb5,   0x2c, 0x3c, 0x00,  0x2e, 0x3e, 0x00,  // 60, 61, 62
    0x2f, 0x3f, 0x00,   0x00, 0x00, 0x00,  0xffe2,  0, 0x00,  // 63, 64, 65
    0xffe3,  0, 0x00,   0x00, 0x00, 0x00,  0xffe9,  0, 0x00,  // 66, 67, 68
    0x20, 0x20, 0x00,   0x00, 0x00, 0x00,  0xff7e,  0, 0x00,  // 69, 70, 71
    0x00, 0x00, 0x00,   0xff62,  0, 0x00,  0x00, 0x00, 0x00,  // 72, 73, 74
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  // 75, 76, 77
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  // 78, 79, 80
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,  0xff63,  0, 0x00,  // 81, 82, 83
    0xffff,  0, 0x00,   0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  // 84, 85, 86
    0xff51,  0, 0x00,   0xff50,  0, 0x00,  0xff57,  0, 0x00,  // 87, 88, 89
    0x00, 0x00, 0x00,   0xff52,  0, 0x00,  0xff54,  0, 0x00,  // 90, 91, 92
    0xff55,  0, 0x00,   0xff56,  0, 0x00,  0x00, 0x00, 0x00,  // 93, 94, 95
    0x00, 0x00, 0x00,   0xff53,  0, 0x00,  0xff7f,  0, 0x00,  // 96, 97, 98
    0xffb7,  0, 0x00,   0xffb4,  0, 0x00,  0xffb1,  0, 0x00,  // 99, 00, 01
    0x00, 0x00, 0x00,   0xffaf,  0, 0x00,  0xffb8,  0, 0x00,  // 02, 03, 04
    0xffb5,  0, 0x00,   0xffb2,  0, 0x00,  0xffb0,  0, 0x00,  // 05, 06, 07
    0xffaa,  0, 0x00,   0xffb9,  0, 0x00,  0xffb6,  0, 0x00,  // 08, 09, 10
    0xffb3,  0, 0x00,   0xffae,  0, 0x00,  0xffad,  0, 0x00,  // 11, 12, 13
    0xffab,  0, 0x00,   0x00, 0x00, 0x00,  0xff8d,  0, 0x00,  // 14, 15, 16
    0x00,  0x00, 0x00,  0xff1b,  0, 0x00,  0x00, 0x00, 0x00,  // 17, 18, 19 
    0xffbe,  0, 0x00,   0xffbf,  0, 0x00,  0xffc0,  0, 0x00,  // 20, 21, 22
    0xffc1,  0, 0x00,   0xffc2,  0, 0x00,  0xffc3,  0, 0x00,  // 23, 24, 25
    0xffc4,  0, 0x00,   0xffc5,  0, 0x00,  0xffc6,  0, 0x00,  // 26, 27, 28 
    0xffc7,  0, 0x00,   0xffc8,  0, 0x00,  0xffc9,  0, 0x00,  // 29, 30, 31
    0xff61,  0, 0x00,   0xff69,  0, 0x00,  0xff13,  0, 0x00,  // 32, 33, 34
    0x00, 0x00, 0x00,   0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  // 35, 36, 37 
    0x00, 0x00, 0x00,   0xff20,  0, 0x00,  0xff21,  0, 0x00,  // 38, 39, 40
    0xff7e, 0x00, 0x00					      // 41
    };
