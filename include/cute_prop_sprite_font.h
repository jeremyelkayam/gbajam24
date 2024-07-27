/*
 * Code copyright (c) 2024 Jeremy Elkayam
 */

#pragma once

#include "bn_sprite_font.h"
#include "bn_utf8_characters_map.h"
#include "bn_sprite_items_cute_prop_font.h"

namespace aru
{

// constexpr bn::utf8_character cute_prop_sprite_font_utf8_characters[] = {
// ?};

constexpr int8_t cute_prop_sprite_font_character_widths[] = {
    6,  // 32
    2,  // 33 !
    4,  // 34 "
    6,  // 35 #
    6,  // 36 $
    6,  // 37 %
    6,  // 38 &
    2,  // 39 '
    4,  // 40 (
    4,  // 41 )
    6,  // 42 *
    6,  // 43 +
    3,  // 44 ,
    6,  // 45 -
    2,  // 46 .
    6,  // 47 /
    6,  // 48 0
    6,  // 49 1
    6,  // 50 2
    6,  // 51 3
    6,  // 52 4
    6,  // 53 5
    6,  // 54 6
    6,  // 55 7
    6,  // 56 8
    6,  // 57 9
    2,  // 58 :
    3,  // 59 ;
    6,  // 60 <
    6,  // 61 =
    6,  // 62 >
    6,  // 63 ?
    7,  // 64 @
    6,  // 65 A
    6,  // 66 B
    6,  // 67 C
    6,  // 68 D
    6,  // 69 E
    6,  // 70 F
    6,  // 71 G
    6,  // 72 H
    4,  // 73 I
    6,  // 74 J
    6,  // 75 K
    6,  // 76 L
    8,  // 77 M
    6,  // 78 N
    6,  // 79 O
    6,  // 80 P
    6,  // 81 Q
    6,  // 82 R
    6,  // 83 S
    6,  // 84 T
    6,  // 85 U
    6,  // 86 V
    8,  // 87 W
    6,  // 88 X
    6,  // 89 Y
    6,  // 90 Z
    4,  // 91 [
    6,  // 92 backslash
    4,  // 93 ]
    6,  // 94 ^
    6,  // 95 _
    3,  // 96 `
    6,  // 97 a
    6,  // 98 b
    6,  // 99 c
    6,  // 100 d
    6,  // 101 e
    5,  // 102 f
    6,  // 103 g
    6,  // 104 h
    4,  // 105 i
    4,  // 106 j
    6,  // 107 k
    4,  // 108 l
    8,  // 109 m
    6,  // 110 n
    6,  // 111 o
    6,  // 112 p
    6,  // 113 q
    5,  // 114 r
    6,  // 115 s
    5,  // 116 t
    6,  // 117 u
    6,  // 118 v
    8,  // 119 w
    6,  // 120 x
    6,  // 121 y
    6,  // 122 z
    5,  // 123 {
    1,  // 124 |
    5,  // 125 }
    6,  // 126 ~
};

constexpr bn::span<const bn::utf8_character> cute_prop_sprite_font_utf8_characters_span;

constexpr auto cute_prop_sprite_font_utf8_characters_map =
        bn::utf8_characters_map<cute_prop_sprite_font_utf8_characters_span>();

constexpr bn::sprite_font cute_prop_sprite_font(
        bn::sprite_items::cute_prop_font, cute_prop_sprite_font_utf8_characters_map.reference(),
        cute_prop_sprite_font_character_widths);

}
