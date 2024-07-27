/*
 * Copyright (c) 2020-2024 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#ifndef COMMON_VARIABLE_8x8_SPRITE_FONT_H
#define COMMON_VARIABLE_8x8_SPRITE_FONT_H

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_small_numbers_font.h"

namespace aru
{

constexpr bn::utf8_character small_numbers_sprite_font_utf8_characters[] = {
    "Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á", "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"
};

constexpr int8_t small_numbers_sprite_font_character_widths[] = {
    6,  // 32
    3,  // 33 !
    6,  // 34 "
    7,  // 35 #
    8,  // 36 $
    8,  // 37 %
    8,  // 38 &
    3,  // 39 '
    4,  // 40 (
    4,  // 41 )
    6,  // 42 *
    3,  // 43 +
    3,  // 44 ,
    3,  // 45 -
    3,  // 46 .
    8,  // 47 /
    4,  // 48 0
    2,  // 49 1
    4,  // 50 2
    4,  // 51 3
    4,  // 52 4
    4,  // 53 5
    4,  // 54 6
    4,  // 55 7
    4,  // 56 8
    4,  // 57 9
    3,  // 58 :
    3,  // 59 ;
    7,  // 60 <
    7,  // 61 =
    7,  // 62 >
    7,  // 63 ?
    8,  // 64 @
    7,  // 65 A
    7,  // 66 B
    7,  // 67 C
    7,  // 68 D
    7,  // 69 E
    7,  // 70 F
    7,  // 71 G
    7,  // 72 H
    7,  // 73 I
    7,  // 74 J
    7,  // 75 K
    7,  // 76 L
    8,  // 77 M
    7,  // 78 N
    7,  // 79 O
    7,  // 80 P
    8,  // 81 Q
    7,  // 82 R
    7,  // 83 S
    7,  // 84 T
    7,  // 85 U
    8,  // 86 V
    8,  // 87 W
    7,  // 88 X
    7,  // 89 Y
    7,  // 90 Z
    4,  // 91 [
    8,  // 92
    4,  // 93 ]
    7,  // 94 ^
    7,  // 95 _
    3,  // 96 `
    5,  // 97 a
    5,  // 98 b
    5,  // 99 c
    5,  // 100 d
    5,  // 101 e
    5,  // 102 f
    5,  // 103 g
    5,  // 104 h
    5,  // 105 i
    5,  // 106 j
    5,  // 107 k
    5,  // 108 l
    6,  // 109 m
    5,  // 110 n
    5,  // 111 o
    5,  // 112 p
    5,  // 113 q
    5,  // 114 r
    5,  // 115 s
    5,  // 116 t
    6,  // 117 u
    5,  // 118 v
    6,  // 119 w
    5,  // 120 x
    5,  // 121 y
    5,  // 122 z
    5,  // 123 {
    3,  // 124 |
    5,  // 125 }
    8,  // 126 ~
    7,  // Á
    6,  // É
    7,  // Í
    7,  // Ó
    7,  // Ú
    7,  // Ü
    7,  // Ñ
    5,  // á
    5,  // é
    5,  // í
    5,  // ó
    6,  // ú
    6,  // ü
    5,  // ñ
    3,  // ¡
    7,  // ¿
};

constexpr bn::span<const bn::utf8_character> 
        small_numbers_sprite_font_utf8_characters_span(
        small_numbers_sprite_font_utf8_characters);

constexpr auto small_numbers_sprite_font_utf8_characters_map =
        bn::utf8_characters_map<small_numbers_sprite_font_utf8_characters_span>();

constexpr bn::sprite_font small_numbers_sprite_font(
        bn::sprite_items::small_numbers_font, small_numbers_sprite_font_utf8_characters_map.reference(),
        small_numbers_sprite_font_character_widths);

}

#endif
