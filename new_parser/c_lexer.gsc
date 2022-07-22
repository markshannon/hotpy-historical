.code
new_stream:
NAME(0,"f") TSTORE_P(0) 
FILE("c_lexer.c") LINE(124) 12 GC_MALLOC NAME(1,"s") TYPE_NAME(1,"Stream") TSTORE_R(1) 
LINE(125) &type_Stream ALOAD_R TLOAD_R(1) 0 RSTORE_R 
LINE(126) TLOAD_P(0) TLOAD_R(1) 4 RSTORE_P 
LINE(127) 4294967295 TLOAD_R(1) 8 RSTORE_U4 
LINE(128) TLOAD_R(1) RETURN_R ;

get_character:
NAME(0,"s") TYPE_NAME(0,"Stream") TSTORE_R(0) 
FILE("c_lexer.c") LINE(135) TLOAD_R(0) 4 RLOAD_P NARG_P &fgetc N_CALL_I4(1) RETURN_I4 ;

fetch_character:
NAME(0,"s") TYPE_NAME(0,"Stream") TSTORE_R(0) 
FILE("c_lexer.c") LINE(140) TLOAD_R(0) 8 RLOAD_U4 4294967295 LT_U4 BRANCH_T(2) 
LINE(141) TLOAD_R(0) 8 RLOAD_U4 NAME(1,"result") TSTORE_I4(1) 
LINE(142) 4294967294 TLOAD_R(0) 8 RSTORE_U4 
LINE(143) TLOAD_I4(1) HOP(3) TARGET(2) 
LINE(145) TLOAD_R(0) &get_character #1 CALL_I4 TARGET(3) RETURN_I4 ;

peek_character:
NAME(0,"s") TYPE_NAME(0,"Stream") TSTORE_R(0) 
FILE("c_lexer.c") LINE(150) TLOAD_R(0) 8 RLOAD_U4 4294967295 LT_U4 BRANCH_T(5) 
LINE(151) TLOAD_R(0) 8 RLOAD_U4 HOP(6) TARGET(5) 
LINE(153) TLOAD_R(0) &get_character #1 CALL_I4 NAME(1,"result") TSTORE_I4(1) 
LINE(154) TLOAD_I4(1) TLOAD_R(0) 8 RSTORE_U4 
LINE(155) TLOAD_I4(1) TARGET(6) RETURN_I4 ;

new_lexer:
NAME(0,"s") TYPE_NAME(0,"Stream") TSTORE_R(0) 
FILE("c_lexer.c") LINE(162) 156 GC_MALLOC NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) 
LINE(163) &type_Lexer ALOAD_R TLOAD_R(1) 0 RSTORE_R 
LINE(164) 0 TLOAD_R(1) 8 RSTORE_U4 
LINE(165) 0 TLOAD_R(1) 12 RSTORE_U4 
LINE(166) 0 TLOAD_R(1) 16 RSTORE_U4 
LINE(167) 0 TLOAD_R(1) 8 RSTORE_U4 
LINE(168) TLOAD_R(0) TLOAD_R(1) 4 RSTORE_R 
LINE(169) 0 TLOAD_R(1) 20 RSTORE_U4 
LINE(170) 1 TLOAD_R(1) 24 RSTORE_U4 
LINE(171) TLOAD_R(1) RETURN_R ;

make_token:
NAME(0,"kind") TSTORE_I4(0) NAME(1,"data") TYPE_NAME(1,"object") TSTORE_R(1) 
FILE("c_lexer.c") LINE(175) 12 GC_MALLOC NAME(2,"t") TYPE_NAME(2,"Token") TSTORE_R(2) 
LINE(176) &type_Token ALOAD_R TLOAD_R(2) 0 RSTORE_R 
LINE(177) TLOAD_I4(0) TLOAD_R(2) 8 RSTORE_U4 
LINE(178) TLOAD_R(1) TLOAD_R(2) 4 RSTORE_R 
LINE(179) TLOAD_R(2) RETURN_R ;

make_string:
NAME(0,"text") TSTORE_P(0) NAME(1,"length") TSTORE_I4(1) 
FILE("c_lexer.c") LINE(183) 12 GC_MALLOC NAME(2,"t") TYPE_NAME(2,"Token") TSTORE_R(2) 
LINE(184) &type_Token ALOAD_R TLOAD_R(2) 0 RSTORE_R 
LINE(185) 2 TLOAD_R(2) 8 RSTORE_U4 
LINE(186) TLOAD_I4(1) TLOAD_P(0) &string_from_chars #2 CALL_R TLOAD_R(2) 4 RSTORE_R 
LINE(187) TLOAD_R(2) RETURN_R ;

new_identifier:
NAME(0,"buffer") TSTORE_P(0) NAME(1,"length") TSTORE_I4(1) 
FILE("c_lexer.c") LINE(191) 12 GC_MALLOC NAME(2,"t") TYPE_NAME(2,"Token") TSTORE_R(2) 
LINE(192) &type_Token ALOAD_R TLOAD_R(2) 0 RSTORE_R 
LINE(193) 1 TLOAD_R(2) 8 RSTORE_U4 
LINE(194) TLOAD_I4(1) TLOAD_P(0) &string_from_chars #2 CALL_R &canonical_string #1 CALL_R TLOAD_R(2) 4 RSTORE_R 
LINE(195) TLOAD_R(2) RETURN_R ;

comment:
DROP NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) 
FILE("c_lexer.c") LINE(201) 0 NAME(4,"lines") TSTORE_I4(4) 
LINE(202) TLOAD_R(1) 4 RLOAD_R NAME(3,"s") TYPE_NAME(3,"Stream") TSTORE_R(3) TARGET(12) 
LINE(203) TARGET(13) 
LINE(205) TLOAD_R(3) &fetch_character #1 CALL_I4 NAME(2,"c") TSTORE_I4(2) 
LINE(206) TLOAD_I4(2) 10 NE_I4 BRANCH_T(13) 
LINE(207) TLOAD_R(3) &peek_character #1 CALL_I4 TSTORE_I4(2) 
LINE(208) TLOAD_I4(2) 35 EQ_I4 BRANCH_T(16) 
LINE(209) TLOAD_R(1) 24 RLOAD_U4 TLOAD_I4(4) ADD_U4 TLOAD_R(1) 24 RSTORE_U4 
LINE(210) 0 HOP(17) TARGET(16) 
LINE(212) TLOAD_I4(4) 1 ADD_I4 TSTORE_I4(4) 
LINE(213) HOP(12) 
LINE(215) 0 TARGET(17) RETURN_R ;

new_line:
DROP NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) 
FILE("c_lexer.c") LINE(219) TLOAD_R(1) 16 RLOAD_U4 0 NE_U4 BRANCH_T(20) 
LINE(220) 12 GC_MALLOC NAME(2,"t") TYPE_NAME(2,"Token") TSTORE_R(2) 
LINE(221) &type_Token ALOAD_R TLOAD_R(2) 0 RSTORE_R 
LINE(222) 6 TLOAD_R(2) 8 RSTORE_U4 
LINE(223) TLOAD_R(1) 24 RLOAD_U4 &Py_newInt #1 CALL_R TLOAD_R(2) 4 RSTORE_R 
LINE(224) TLOAD_R(1) 24 RLOAD_U4 1 ADD_U4 TLOAD_R(1) 24 RSTORE_U4 
LINE(225) 1 TLOAD_R(1) 12 RSTORE_U4 
LINE(226) TLOAD_R(2) HOP(21) TARGET(20) 
LINE(228) 0 TARGET(21) RETURN_R ;

dot:
DROP NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) 
FILE("c_lexer.c") LINE(232) TLOAD_R(1) 4 RLOAD_R NAME(3,"s") TYPE_NAME(3,"Stream") TSTORE_R(3) 
LINE(233) TLOAD_R(3) &peek_character #1 CALL_I4 NAME(2,"c") TSTORE_I4(2) 
LINE(234) TLOAD_I4(2) 46 NE_I4 BRANCH_T(22) 
LINE(235) TLOAD_R(3) &fetch_character #1 CALL_I4 DROP 
LINE(236) TLOAD_R(3) &fetch_character #1 CALL_I4 TSTORE_I4(2) 
LINE(237) TLOAD_I4(2) 46 NE_I4 BRANCH_T(23) 
LINE(238) &token_ELLIPSIS ALOAD_R HOP(24) TARGET(23) 
LINE(240) TLOAD_R(1) 24 RLOAD_U4 &x38 &py_sprintf #2 C_CALL_R NAME(6,"msg") TYPE_NAME(6,"str") TSTORE_R(6) 
LINE(241) TLOAD_R(6) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(242) 0 HOP(24) TARGET(22) 
LINE(244) TLOAD_I4(2) 48 LT_I4 BRANCH_T(27) TLOAD_I4(2) 57 GT_I4 BRANCH_T(27) 
LINE(245) TLOAD_R(1) 0 &make_float #2 CALL_R HOP(24) TARGET(27) 
LINE(247) &token_DOT ALOAD_R TARGET(24) RETURN_R ;

next_token:
NAME(0,"l") TYPE_NAME(0,"Lexer") TSTORE_R(0) 
FILE("c_lexer.c") LINE(251) TARGET(29) 
LINE(255) TLOAD_R(0) 8 RLOAD_U4 0 EQ_U4 BRANCH_T(30) 
LINE(256) TLOAD_R(0) 8 RLOAD_U4 1 SUB_U4 TLOAD_R(0) 8 RSTORE_U4 
LINE(257) &token_DEDENT ALOAD_R HOP(31) TARGET(30) 
LINE(259) TLOAD_R(0) 4 RLOAD_R &fetch_character #1 CALL_I4 NAME(1,"c") TSTORE_I4(1) 
LINE(260) TLOAD_R(0) 12 RLOAD_U4 0 EQ_U4 BRANCH_T(32) TLOAD_R(0) 16 RLOAD_U4 0 NE_U4 BRANCH_T(32) 
LINE(260) TARGET(33) 
LINE(263) 0 NAME(8,"spaces") TSTORE_I4(8) TARGET(34) 
LINE(265) TLOAD_I4(1) 32 NE_I4 BRANCH_T(35) 
LINE(266) TLOAD_I4(8) 1 ADD_I4 TSTORE_I4(8) 
LINE(267) HOP(36) TARGET(35) 
LINE(267) TLOAD_I4(1) 9 NE_I4 BRANCH_T(39) 
LINE(268) TLOAD_I4(8) 8 ADD_I4 TSTORE_I4(8) 
LINE(270) TARGET(36) 
LINE(272) TLOAD_R(0) 4 RLOAD_R &fetch_character #1 CALL_I4 TSTORE_I4(1) 
LINE(273) HOP(34) TARGET(39) 
LINE(274) TLOAD_I4(1) 10 EQ_I4 BRANCH_T(41) 
LINE(275) HOP(42) TARGET(41) 
LINE(276) TLOAD_R(0) 24 RLOAD_U4 1 ADD_U4 TLOAD_R(0) 24 RSTORE_U4 
LINE(277) TLOAD_R(0) 4 RLOAD_R &fetch_character #1 CALL_I4 TSTORE_I4(1) 
LINE(278) HOP(33) TARGET(42) 
LINE(279) TLOAD_I4(8) TLOAD_R(0) 20 RLOAD_U4 2 LSH_U4 TSTORE_U4(38) TLOAD_R(0) 28 TLOAD_U4(38) ADD_U4 RLOAD_U4 LE_U4 BRANCH_T(44) 
LINE(280) TLOAD_R(0) 20 RLOAD_U4 1 ADD_U4 TLOAD_R(0) 20 RSTORE_U4 
LINE(281) TLOAD_R(0) 20 RLOAD_U4 32 NE_U4 BRANCH_T(45) 
LINE(282) TLOAD_R(0) 24 RLOAD_U4 &x63 &py_sprintf #2 C_CALL_R NAME(14,"msg") TYPE_NAME(14,"str") TSTORE_R(14) 
LINE(283) TLOAD_R(14) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(284) TARGET(45) 
LINE(285) TLOAD_I4(8) TLOAD_R(0) 20 RLOAD_U4 2 LSH_U4 TSTORE_U4(39) TLOAD_R(0) 28 TLOAD_U4(39) ADD_U4 RSTORE_U4 
LINE(286) &token_INDENT ALOAD_R HOP(31) TARGET(44) 
LINE(288) TLOAD_I4(8) TLOAD_R(0) 20 RLOAD_U4 2 LSH_U4 TSTORE_U4(40) TLOAD_R(0) 28 TLOAD_U4(40) ADD_U4 RLOAD_U4 GE_U4 BRANCH_T(51) 
LINE(289) TLOAD_R(0) 20 RLOAD_U4 1 SUB_U4 TLOAD_R(0) 20 RSTORE_U4 HOP(47) TARGET(48) 
LINE(291) TLOAD_R(0) 20 RLOAD_U4 1 SUB_U4 TLOAD_R(0) 20 RSTORE_U4 
LINE(292) TLOAD_R(0) 8 RLOAD_U4 1 ADD_U4 TLOAD_R(0) 8 RSTORE_U4 
LINE(293) TARGET(47) 
LINE(290) TLOAD_I4(8) TLOAD_R(0) 20 RLOAD_U4 2 LSH_U4 TSTORE_U4(41) TLOAD_R(0) 28 TLOAD_U4(41) ADD_U4 RLOAD_U4 LT_U4 BRANCH_T(48) 
LINE(294) TLOAD_I4(8) TLOAD_R(0) 20 RLOAD_U4 2 LSH_U4 TSTORE_U4(42) TLOAD_R(0) 28 TLOAD_U4(42) ADD_U4 RLOAD_U4 EQ_U4 BRANCH_T(50) 
LINE(295) TLOAD_R(0) 24 RLOAD_U4 &x71 &py_sprintf #2 C_CALL_R NAME(23,"msg") TYPE_NAME(23,"str") TSTORE_R(23) 
LINE(296) TLOAD_R(23) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(297) TARGET(50) 
LINE(298) &token_DEDENT ALOAD_R HOP(31) 
LINE(300) TARGET(32) 
LINE(301) TLOAD_R(0) 12 RLOAD_U4 TLOAD_R(0) 24 RSTORE_U4 HOP(52) TARGET(53) 
LINE(303) TLOAD_R(0) 4 RLOAD_R &fetch_character #1 CALL_I4 TSTORE_I4(1) 
LINE(304) TARGET(52) 
LINE(302) TLOAD_I4(1) 32 NE_I4 BRANCH_T(54) 1 TSTORE_I4(27) HOP(55) TARGET(54) 0 TSTORE_I4(27) TARGET(55) TLOAD_I4(1) 9 NE_I4 BRANCH_T(56) 1 TSTORE_I4(28) HOP(57) TARGET(56) 0 TSTORE_I4(28) TARGET(57) TLOAD_I4(27) TLOAD_I4(28) OR_I4 0 NE_I4 BRANCH_T(53) 
LINE(305) TARGET(51) 
LINE(306) 0 TLOAD_R(0) 12 RSTORE_U4 
LINE(307) TLOAD_I4(1) 256 GE_I4 BRANCH_T(59) 
LINE(308) TLOAD_I4(1) 2 LSH_I4 &token_table ADD_P ALOAD_R NAME(2,"t") TYPE_NAME(2,"Token") TSTORE_R(2) 
LINE(309) TLOAD_R(2) 0 EQ_U4 BRANCH_T(60) 
LINE(310) TLOAD_R(0) 16 RLOAD_U4 TLOAD_I4(1) 2 LSH_I4 &bracket_table ADD_P ALOAD_I4 ADD_U4 TLOAD_R(0) 16 RSTORE_U4 
LINE(311) TLOAD_R(2) HOP(31) TARGET(60) 
LINE(313) TLOAD_I4(1) 2 LSH_I4 &specials ADD_P ALOAD_P NAME(30,"f") TSTORE_P(30) 
LINE(314) TLOAD_P(30) 0 EQ_U4 BRANCH_T(62) 
LINE(315) TLOAD_R(0) TLOAD_I4(1) TLOAD_P(30) #2 CALL_R TSTORE_R(2) 
LINE(316) TLOAD_R(2) 0 EQ_U4 BRANCH_T(29) 
LINE(317) TLOAD_R(2) HOP(31) 
LINE(319) TARGET(62) 
LINE(321) TLOAD_R(0) 24 RLOAD_U4 TLOAD_I4(1) &x89 &py_sprintf #3 C_CALL_R NAME(32,"msg") TYPE_NAME(32,"str") TSTORE_R(32) 
LINE(322) TLOAD_R(32) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(323) 0 HOP(31) TARGET(59) 
LINE(326) TLOAD_I4(1) EXT_U1 NARG_I4 &UNICODE_WORD_CHARACTER N_CALL_I4(1) 0 EQ_I4 BRANCH_T(67) 
LINE(327) TLOAD_R(0) TLOAD_I4(1) &identifier #2 CALL_R HOP(31) TARGET(67) 
LINE(329) TLOAD_R(0) 24 RLOAD_U4 TLOAD_I4(1) &x92 &py_sprintf #3 C_CALL_R NAME(36,"msg") TYPE_NAME(36,"str") TSTORE_R(36) 
LINE(330) TLOAD_R(36) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(331) 0 TARGET(31) RETURN_R ;

_string[private]:
256 ALLOCA_I1 NAME(24,"__lcc_c_locals") TSTORE_P(24) NAME(0,"start") TSTORE_I4(0) NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) NAME(2,"raw") TSTORE_I4(2) 
FILE("c_lexer.c") LINE(338) TLOAD_R(1) 4 RLOAD_R NAME(4,"s") TYPE_NAME(4,"Stream") TSTORE_R(4) 
LINE(339) 0 NAME(5,"len") TSTORE_I4(5) 
LINE(341) TLOAD_R(4) &fetch_character #1 CALL_I4 NAME(3,"c") TSTORE_I4(3) 
LINE(343) TLOAD_I4(3) TLOAD_I4(0) NE_I4 BRANCH_T(69) 
LINE(344) TLOAD_R(4) &peek_character #1 CALL_I4 NAME(8,"c3") TSTORE_I4(8) 
LINE(345) TLOAD_I4(8) TLOAD_I4(3) NE_I4 BRANCH_T(70) 
LINE(346) TLOAD_R(4) &fetch_character #1 CALL_I4 DROP 
LINE(347) TLOAD_I4(2) TLOAD_R(1) TLOAD_I4(0) &big_string #3 CALL_R HOP(71) TARGET(70) 
LINE(349) 0 0 &make_string #2 CALL_R HOP(71) TARGET(69) TARGET(73) 
LINE(353) TLOAD_I4(3) TLOAD_I4(0) NE_I4 BRANCH_T(74) 
LINE(354) HOP(75) TARGET(74) 
LINE(355) TLOAD_I4(3) 92 NE_I4 BRANCH_T(76) TLOAD_I4(2) 0 NE_I4 BRANCH_T(76) 
LINE(356) TLOAD_R(4) &fetch_character #1 CALL_I4 TSTORE_I4(3) 
LINE(357) TLOAD_I4(3) 120 NE_I4 BRANCH_T(77) 
LINE(358) TLOAD_R(4) &fetch_character #1 CALL_I4 TSTORE_I4(3) 
LINE(359) TLOAD_I4(3) NARG_I4 &hex_digit N_CALL_I4(1) NAME(6,"msh") TSTORE_I4(6) 
LINE(360) TLOAD_R(4) &fetch_character #1 CALL_I4 TSTORE_I4(3) 
LINE(361) TLOAD_I4(6) 4 LSH_I4 0 NE_I4 BRANCH_T(78) TLOAD_I4(3) NARG_I4 &hex_digit N_CALL_I4(1) 0 EQ_I4 BRANCH_T(79) TARGET(78) 1 TSTORE_I4(16) HOP(80) TARGET(79) 0 TSTORE_I4(16) TARGET(80) TLOAD_I4(16) TSTORE_I4(3) 
LINE(362) HOP(85) TARGET(77) 
LINE(363) TLOAD_I4(3) 122 GE_I4 BRANCH_T(82) 
LINE(364) TLOAD_I4(3) &escape_chars ADD_P ALOAD_U1 TSTORE_I4(3) 
LINE(365) TLOAD_I4(3) 0 NE_I4 BRANCH_T(85) 
LINE(366) TLOAD_R(1) 24 RLOAD_U4 TLOAD_I4(3) &x116 &py_sprintf #3 C_CALL_R NAME(18,"msg") TYPE_NAME(18,"str") TSTORE_R(18) 
LINE(367) TLOAD_R(18) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(369) HOP(85) TARGET(82) 
LINE(370) TLOAD_R(1) 24 RLOAD_U4 TLOAD_I4(3) &x116 &py_sprintf #3 C_CALL_R NAME(20,"msg") TYPE_NAME(20,"str") TSTORE_R(20) 
LINE(371) TLOAD_R(20) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(374) HOP(85) TARGET(76) 
LINE(374) TLOAD_I4(3) -1 NE_I4 BRANCH_T(86) 
LINE(375) &x119 &type_SyntaxError ALOAD_R &raise_char #2 CALL_V 
LINE(376) HOP(87) TARGET(86) 
LINE(376) TLOAD_I4(3) 10 NE_I4 BRANCH_T(88) 
LINE(377) &x122 &type_SyntaxError ALOAD_R &raise_char #2 CALL_V 
LINE(378) HOP(89) TARGET(88) 
LINE(379) TLOAD_I4(3) EXT_U1 TLOAD_I4(5) TLOAD_P(24) ADD_P ASTORE_U1 
LINE(380) TLOAD_I4(5) 1 ADD_I4 TSTORE_I4(5) 
LINE(381) TLOAD_I4(5) 256 LT_I4 BRANCH_T(90) 
LINE(382) &x125 &type_SyntaxError ALOAD_R &raise_char #2 CALL_V 
LINE(383) TARGET(90) 
LINE(384) TARGET(89) TARGET(87) TARGET(85) 
LINE(385) TLOAD_R(4) &fetch_character #1 CALL_I4 TSTORE_I4(3) 
LINE(386) HOP(73) TARGET(75) 
LINE(388) TLOAD_I4(5) TLOAD_P(24) &make_string #2 CALL_R TARGET(71) RETURN_R ;

string:
NAME(0,"start") TSTORE_I4(0) NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) 
FILE("c_lexer.c") LINE(392) 0 TLOAD_R(1) TLOAD_I4(0) &_string #3 CALL_R RETURN_R ;

raw_string:
NAME(0,"start") TSTORE_I4(0) NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) 
FILE("c_lexer.c") LINE(396) 0 TLOAD_R(1) TLOAD_I4(0) &_string #3 CALL_R RETURN_R ;

big_string:
256 ALLOCA_I1 NAME(21,"__lcc_c_locals") TSTORE_P(21) NAME(0,"start") TSTORE_I4(0) NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) DROP 
FILE("c_lexer.c") LINE(400) TLOAD_R(1) 4 RLOAD_R NAME(10,"s") TYPE_NAME(10,"Stream") TSTORE_R(10) 
LINE(402) TLOAD_P(21) NAME(6,"buffer") TSTORE_P(6) 
LINE(403) 0 NAME(4,"len") TSTORE_I4(4) 
LINE(404) 256 NAME(5,"limit") TSTORE_I4(5) 
LINE(405) 0 NAME(9,"quotes") TSTORE_I4(9) 
LINE(407) TLOAD_R(10) &fetch_character #1 CALL_I4 NAME(7,"c1") TSTORE_I4(7) 
LINE(408) TLOAD_I4(7) -1 NE_I4 BRANCH_T(95) 
LINE(409) &x119 &type_SyntaxError ALOAD_R &raise_char #2 CALL_V TARGET(95) 
LINE(410) TLOAD_I4(7) TLOAD_I4(0) NE_I4 BRANCH_T(96) 
LINE(411) TLOAD_I4(9) 1 ADD_I4 TSTORE_I4(9) TARGET(96) 
LINE(412) TLOAD_I4(7) 10 NE_I4 BRANCH_T(97) 
LINE(413) TLOAD_R(1) 24 RLOAD_U4 1 ADD_U4 TLOAD_R(1) 24 RSTORE_U4 TARGET(97) 
LINE(414) TLOAD_R(10) &fetch_character #1 CALL_I4 NAME(8,"c2") TSTORE_I4(8) 
LINE(415) TLOAD_I4(8) -1 NE_I4 BRANCH_T(98) 
LINE(416) &x119 &type_SyntaxError ALOAD_R &raise_char #2 CALL_V TARGET(98) 
LINE(417) TLOAD_I4(8) TLOAD_I4(0) NE_I4 BRANCH_T(99) 
LINE(418) TLOAD_I4(9) 1 ADD_I4 TSTORE_I4(9) TARGET(99) 
LINE(419) TLOAD_I4(8) 10 NE_I4 BRANCH_T(100) 
LINE(420) TLOAD_R(1) 24 RLOAD_U4 1 ADD_U4 TLOAD_R(1) 24 RSTORE_U4 TARGET(100) 
LINE(421) TLOAD_R(10) &fetch_character #1 CALL_I4 NAME(3,"c3") TSTORE_I4(3) TARGET(101) 
LINE(423) TLOAD_I4(3) -1 NE_I4 BRANCH_T(102) 
LINE(424) &x119 &type_SyntaxError ALOAD_R &raise_char #2 CALL_V TARGET(102) 
LINE(425) TLOAD_I4(3) 10 NE_I4 BRANCH_T(103) 
LINE(426) TLOAD_R(1) 24 RLOAD_U4 1 ADD_U4 TLOAD_R(1) 24 RSTORE_U4 TARGET(103) 
LINE(427) TLOAD_I4(3) TLOAD_I4(0) NE_I4 BRANCH_T(104) 
LINE(428) TLOAD_I4(9) 2 NE_I4 BRANCH_T(105) 
LINE(429) HOP(106) TARGET(105) 
LINE(431) TLOAD_I4(9) 1 ADD_I4 TSTORE_I4(9) 
LINE(433) HOP(108) TARGET(104) 
LINE(434) 0 TSTORE_I4(9) 
LINE(435) TARGET(108) 
LINE(436) TLOAD_I4(7) EXT_U1 TLOAD_I4(4) TLOAD_P(6) ADD_P ASTORE_U1 
LINE(437) TLOAD_I4(4) 1 ADD_I4 TSTORE_I4(4) 
LINE(438) TLOAD_I4(4) TLOAD_I4(5) NE_I4 BRANCH_T(109) 
LINE(439) 1 TLOAD_I4(5) 1 LSH_I4 MUL_U4 NARG_U4 &malloc N_CALL_P(1) NAME(17,"new_buffer") TSTORE_P(17) 
LINE(440) 1 TLOAD_I4(5) MUL_U4 TLOAD_P(6) TLOAD_P(17) NARG_P NARG_P NARG_U4 &memcpy N_CALL_P(3) DROP 
LINE(441) TLOAD_I4(5) 256 LE_I4 BRANCH_T(110) 
LINE(442) TLOAD_P(6) NARG_P &free N_CALL_V(1) 
LINE(443) TARGET(110) 
LINE(444) TLOAD_I4(5) 1 LSH_I4 TSTORE_I4(5) 
LINE(445) TLOAD_P(17) TSTORE_P(6) 
LINE(446) TARGET(109) 
LINE(447) TLOAD_I4(8) TSTORE_I4(7) 
LINE(448) TLOAD_I4(3) TSTORE_I4(8) 
LINE(449) TLOAD_R(10) &fetch_character #1 CALL_I4 TSTORE_I4(3) 
LINE(450) HOP(101) TARGET(106) 
LINE(452) TLOAD_I4(4) TLOAD_P(6) &make_string #2 CALL_R RETURN_R ;

operator:
NAME(0,"c") TSTORE_I4(0) NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) 
FILE("c_lexer.c") LINE(456) TLOAD_R(1) 4 RLOAD_R NAME(2,"s") TYPE_NAME(2,"Stream") TSTORE_R(2) 
LINE(458) TLOAD_R(2) &peek_character #1 CALL_I4 NAME(3,"c2") TSTORE_I4(3) 
LINE(459) TLOAD_I4(3) 61 NE_I4 BRANCH_T(114) 
LINE(460) TLOAD_R(2) &fetch_character #1 CALL_I4 DROP 
LINE(461) TLOAD_I4(0) 2 LSH_I4 &operator_assign ADD_P ALOAD_R HOP(115) TARGET(114) 
LINE(462) TLOAD_I4(3) TLOAD_I4(0) NE_I4 BRANCH_T(117) 
LINE(463) TLOAD_R(2) &fetch_character #1 CALL_I4 DROP 
LINE(464) TLOAD_R(2) &peek_character #1 CALL_I4 TSTORE_I4(3) 
LINE(465) TLOAD_I4(3) 61 NE_I4 BRANCH_T(118) 
LINE(466) TLOAD_R(2) &fetch_character #1 CALL_I4 DROP 
LINE(467) TLOAD_I4(0) 2 LSH_I4 &operator2_assign ADD_P ALOAD_R HOP(115) TARGET(118) 
LINE(469) TLOAD_I4(0) 2 LSH_I4 &operator2 ADD_P ALOAD_R HOP(115) TARGET(117) 
LINE(472) TLOAD_I4(3) 62 NE_I4 BRANCH_T(121) TLOAD_I4(0) 45 NE_I4 BRANCH_T(121) 
LINE(473) TLOAD_R(2) &fetch_character #1 CALL_I4 DROP 
LINE(474) &token_ARROW ALOAD_R HOP(115) TARGET(121) 
LINE(476) TLOAD_I4(0) 2 LSH_I4 &operator1 ADD_P ALOAD_R TARGET(115) RETURN_R ;

line_continuation:
DROP NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) 
FILE("c_lexer.c") LINE(483) TLOAD_R(1) 4 RLOAD_R &fetch_character #1 CALL_I4 NAME(2,"c") TSTORE_I4(2) TARGET(123) 
LINE(485) TLOAD_I4(2) 10 NE_I4 BRANCH_T(124) 
LINE(486) TLOAD_R(1) 24 RLOAD_U4 1 ADD_U4 TLOAD_R(1) 24 RSTORE_U4 
LINE(487) 0 HOP(125) TARGET(124) 
LINE(488) TLOAD_I4(2) 32 EQ_I4 BRANCH_T(127) TLOAD_I4(2) 9 NE_I4 BRANCH_T(128) TARGET(127) 
LINE(489) TLOAD_R(1) 4 RLOAD_R &fetch_character #1 CALL_I4 DROP 
LINE(490) HOP(129) TARGET(128) 
LINE(491) TLOAD_R(1) 24 RLOAD_U4 &x175 &py_sprintf #2 C_CALL_R NAME(3,"msg") TYPE_NAME(3,"str") TSTORE_R(3) 
LINE(492) TLOAD_R(3) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(493) TARGET(129) 
LINE(494) HOP(123) 
LINE(495) 0 TARGET(125) RETURN_R ;

make_float:
NAME(0,"integral") TSTORE_I4(0) NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) 
FILE("c_lexer.c") LINE(501) TLOAD_R(1) 4 RLOAD_R NAME(3,"s") TYPE_NAME(3,"Stream") TSTORE_R(3) 
LINE(503) &x177 ALOAD_F8 NAME(5,"fraction") TSTORE_F8(5) 
LINE(504) &x178 ALOAD_F8 NAME(4,"power10") TSTORE_F8(4) 
LINE(505) TLOAD_R(3) &peek_character #1 CALL_I4 NAME(2,"c") TSTORE_I4(2) 
LINE(506) 12 GC_MALLOC NAME(6,"t") TYPE_NAME(6,"Token") TSTORE_R(6) 
LINE(507) &type_Token ALOAD_R TLOAD_R(6) 0 RSTORE_R 
LINE(508) 5 TLOAD_R(6) 8 RSTORE_U4 HOP(132) TARGET(133) 
LINE(510) TLOAD_R(3) &fetch_character #1 CALL_I4 DROP 
LINE(511) TLOAD_F8(5) TLOAD_F8(4) TLOAD_I4(2) 48 SUB_I4 I2D MUL_F8 ADD_F8 TSTORE_F8(5) 
LINE(512) TLOAD_F8(4) &x182 ALOAD_F8 DIV_F8 TSTORE_F8(4) 
LINE(513) TLOAD_R(3) &peek_character #1 CALL_I4 TSTORE_I4(2) 
LINE(514) TARGET(132) 
LINE(509) TLOAD_I4(2) 48 LT_I4 BRANCH_T(134) TLOAD_I4(2) 57 LE_I4 BRANCH_T(133) TARGET(134) 
LINE(515) TLOAD_I4(0) I2D TLOAD_F8(5) ADD_F8 NAME(8,"mantissa") TSTORE_F8(8) 
LINE(516) TLOAD_I4(2) 101 EQ_I4 BRANCH_T(136) TLOAD_I4(2) 69 NE_I4 BRANCH_T(137) TARGET(136) 
LINE(517) 0 NAME(15,"neg") TSTORE_I4(15) 
LINE(518) TLOAD_R(3) &fetch_character #1 CALL_I4 DROP 
LINE(519) TLOAD_R(3) &fetch_character #1 CALL_I4 TSTORE_I4(2) 
LINE(520) TLOAD_I4(2) 45 NE_I4 BRANCH_T(138) 
LINE(521) 1 TSTORE_I4(15) 
LINE(522) TLOAD_R(3) &fetch_character #1 CALL_I4 TSTORE_I4(2) 
LINE(523) TARGET(138) 
LINE(524) TLOAD_I4(2) 48 LT_I4 BRANCH_T(139) TLOAD_I4(2) 57 LE_I4 BRANCH_T(140) TARGET(139) 
LINE(525) TLOAD_R(1) 24 RLOAD_U4 &x192 &py_sprintf #2 C_CALL_R NAME(19,"msg") TYPE_NAME(19,"str") TSTORE_R(19) 
LINE(526) TLOAD_R(19) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(527) TARGET(140) 
LINE(528) TLOAD_I4(2) 48 SUB_I4 NAME(14,"exp") TSTORE_I4(14) 
LINE(529) TLOAD_R(3) &peek_character #1 CALL_I4 TSTORE_I4(2) HOP(141) TARGET(142) 
LINE(531) TLOAD_R(3) &fetch_character #1 CALL_I4 DROP 
LINE(532) 10 TLOAD_I4(14) MUL_I4 TSTORE_I4(14) 
LINE(533) TLOAD_I4(14) TLOAD_I4(2) 48 SUB_I4 ADD_I4 TSTORE_I4(14) 
LINE(534) TLOAD_R(3) &peek_character #1 CALL_I4 TSTORE_I4(2) 
LINE(535) TARGET(141) 
LINE(530) TLOAD_I4(2) 48 LT_I4 BRANCH_T(143) TLOAD_I4(2) 57 LE_I4 BRANCH_T(142) TARGET(143) 
LINE(536) TLOAD_I4(15) 0 EQ_I4 BRANCH_T(145) 
LINE(537) TLOAD_I4(14) NARG_U4 &exp10 N_CALL_F8(1) TSTORE_F8(24) TLOAD_F8(8) TLOAD_F8(24) DIV_F8 NAME(7,"value") TSTORE_F8(7) HOP(147) TARGET(145) 
LINE(539) TLOAD_I4(14) NARG_U4 &exp10 N_CALL_F8(1) TSTORE_F8(25) TLOAD_F8(8) TLOAD_F8(25) MUL_F8 TSTORE_F8(7) 
LINE(540) HOP(147) TARGET(137) 
LINE(541) TLOAD_F8(8) TSTORE_F8(7) 
LINE(542) TARGET(147) 
LINE(543) TLOAD_F8(7) &Py_getFloat #2 CALL_R TLOAD_R(6) 4 RSTORE_R 
LINE(544) TLOAD_R(6) RETURN_R ;

make_integer:
NAME(0,"value") TSTORE_I4(0) 
FILE("c_lexer.c") LINE(548) 12 GC_MALLOC NAME(1,"t") TYPE_NAME(1,"Token") TSTORE_R(1) 
LINE(549) &type_Token ALOAD_R TLOAD_R(1) 0 RSTORE_R 
LINE(550) 4 TLOAD_R(1) 8 RSTORE_U4 
LINE(551) TLOAD_I4(0) EXT_I1 TLOAD_I4(0) NE_I4 BRANCH_T(149) TLOAD_I4(0) 2 LSH_I4 &small_integer_table 512 ADD_P ADD_P ALOAD_R TSTORE_R(3) HOP(150) TARGET(149) TLOAD_I4(0) &Py_newInt #1 CALL_R TSTORE_R(3) TARGET(150) TLOAD_R(3) TLOAD_R(1) 4 RSTORE_R 
LINE(552) TLOAD_R(1) RETURN_R ;

non_decimal[private]:
NAME(0,"l") TYPE_NAME(0,"Lexer") TSTORE_R(0) NAME(1,"base") TSTORE_I4(1) 
FILE("c_lexer.c") LINE(556) TLOAD_R(0) 4 RLOAD_R NAME(4,"s") TYPE_NAME(4,"Stream") TSTORE_R(4) 
LINE(557) TLOAD_R(4) &fetch_character #1 CALL_I4 NAME(2,"c") TSTORE_I4(2) 
LINE(558) TLOAD_I4(2) 2 LSH_I4 &digits ADD_P ALOAD_I4 NAME(3,"value") TSTORE_I4(3) 
LINE(559) TLOAD_I4(3) 0 GE_I4 BRANCH_T(152) TLOAD_I4(3) TLOAD_I4(1) GE_I4 BRANCH_T(152) 
LINE(560) TLOAD_R(0) 24 RLOAD_U4 &x207 &py_sprintf #2 C_CALL_R NAME(8,"msg") TYPE_NAME(8,"str") TSTORE_R(8) 
LINE(561) TLOAD_R(8) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(562) TARGET(152) 
LINE(563) TLOAD_R(4) &peek_character #1 CALL_I4 TSTORE_I4(2) 
LINE(564) TLOAD_I4(2) 2 LSH_I4 &digits ADD_P ALOAD_I4 NAME(5,"digit") TSTORE_I4(5) HOP(153) TARGET(154) 
LINE(566) TLOAD_R(4) &fetch_character #1 CALL_I4 DROP 
LINE(567) TLOAD_I4(3) TLOAD_I4(1) MUL_I4 TSTORE_I4(3) 
LINE(568) TLOAD_I4(3) TLOAD_I4(5) ADD_I4 TSTORE_I4(3) 
LINE(569) TLOAD_R(4) &peek_character #1 CALL_I4 TSTORE_I4(2) 
LINE(570) TLOAD_I4(2) 2 LSH_I4 &digits ADD_P ALOAD_I4 TSTORE_I4(5) 
LINE(571) TARGET(153) 
LINE(565) TLOAD_I4(2) 0 LT_I4 BRANCH_T(155) TLOAD_I4(2) TLOAD_I4(1) LT_I4 BRANCH_T(154) TARGET(155) 
LINE(572) TLOAD_I4(3) &make_integer #1 CALL_R RETURN_R ;

number:
NAME(0,"start") TSTORE_I4(0) NAME(1,"l") TYPE_NAME(1,"Lexer") TSTORE_R(1) 
FILE("c_lexer.c") LINE(576) TLOAD_I4(0) 48 SUB_I4 NAME(3,"integral") TSTORE_I4(3) 
LINE(577) TLOAD_R(1) 4 RLOAD_R NAME(4,"s") TYPE_NAME(4,"Stream") TSTORE_R(4) 
LINE(578) TLOAD_R(4) &peek_character #1 CALL_I4 NAME(2,"c") TSTORE_I4(2) 
LINE(579) TLOAD_I4(3) 0 NE_I4 BRANCH_T(169) 
LINE(580) TLOAD_I4(2) 120 EQ_I4 BRANCH_T(159) TLOAD_I4(2) 88 NE_I4 BRANCH_T(160) TARGET(159) 
LINE(581) TLOAD_R(4) &fetch_character #1 CALL_I4 DROP 
LINE(582) 16 TLOAD_R(1) &non_decimal #2 CALL_R HOP(161) TARGET(160) 
LINE(583) TLOAD_I4(2) 111 EQ_I4 BRANCH_T(163) TLOAD_I4(2) 79 NE_I4 BRANCH_T(164) TARGET(163) 
LINE(584) TLOAD_R(4) &fetch_character #1 CALL_I4 DROP 
LINE(585) 8 TLOAD_R(1) &non_decimal #2 CALL_R HOP(161) TARGET(164) 
LINE(586) TLOAD_I4(2) 98 EQ_I4 BRANCH_T(166) TLOAD_I4(2) 66 NE_I4 BRANCH_T(167) TARGET(166) 
LINE(587) TLOAD_R(4) &fetch_character #1 CALL_I4 DROP 
LINE(588) 2 TLOAD_R(1) &non_decimal #2 CALL_R HOP(161) TARGET(167) 
LINE(590) TLOAD_R(1) 24 RLOAD_U4 &x207 &py_sprintf #2 C_CALL_R NAME(12,"msg") TYPE_NAME(12,"str") TSTORE_R(12) 
LINE(591) TLOAD_R(12) &type_SyntaxError ALOAD_R &raise_str #2 CALL_V 
LINE(593) HOP(169) TARGET(170) 
LINE(595) TLOAD_R(4) &fetch_character #1 CALL_I4 DROP 
LINE(596) 10 TLOAD_I4(3) MUL_I4 TSTORE_I4(3) 
LINE(597) TLOAD_I4(3) TLOAD_I4(2) 48 SUB_I4 ADD_I4 TSTORE_I4(3) 
LINE(598) TLOAD_R(4) &peek_character #1 CALL_I4 TSTORE_I4(2) 
LINE(599) TARGET(169) 
LINE(594) TLOAD_I4(2) 48 LT_I4 BRANCH_T(171) TLOAD_I4(2) 57 LE_I4 BRANCH_T(170) TARGET(171) 
LINE(600) TLOAD_I4(2) 46 NE_I4 BRANCH_T(173) 
LINE(601) TLOAD_R(4) &fetch_character #1 CALL_I4 DROP 
LINE(602) TLOAD_R(1) TLOAD_I4(3) &make_float #2 CALL_R HOP(161) TARGET(173) 
LINE(603) TLOAD_I4(2) 101 EQ_I4 BRANCH_T(175) TLOAD_I4(2) 69 NE_I4 BRANCH_T(176) TARGET(175) 
LINE(604) TLOAD_R(1) TLOAD_I4(3) &make_float #2 CALL_R HOP(161) TARGET(176) 
LINE(606) TLOAD_I4(3) &make_integer #1 CALL_R TARGET(161) RETURN_R ;

.opaque
.public escape_chars
.label escape_chars
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
.public bracket_table
.label bracket_table
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
.public operator2_assign
.label operator2_assign
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
.public operator2
.label operator2
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
.public operator_assign
.label operator_assign
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
.public operator1
.label operator1
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
.public token_table
.label token_table
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
.public specials
.label specials
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
int32 0
.label __va_arg_tmp
int32 0
.label x207
string "Illegal token at line %d\000"
.label x192
string "Illegal floating point constant at line %d\000"
.label x182
double 10
.label x178
double 0.1
.label x177
double 0
.label x175
string "Illegal characters after \\ at line %d\000"
.label x125
string "Too long line in single quoted string literal\000"
.label x122
string "Reached end of line in single quoted string literal\000"
.label x119
string "Reached end of file in string literal\000"
.label x116
string "Illegal sequence \'\\%c\' at line %d\000"
.label x92
string "Illegal character U%d at line %d\000"
.label x89
string "Illegal character \'%c\' at line %d\000"
.label x71
string "Illegal indent at line %d\000"
.label x63
string "Too many levels of indentation at line %d\000"
.label x38
string "Illegal token \'..\' at line %d\000"
