#include "./r15.hpp"

namespace yocto { namespace gfx {

		 const rgb_t ramp15[256]=
{
	rgb_t(  0,  0,255),
	rgb_t(  0,  3,255),
	rgb_t(  0,  7,255),
	rgb_t(  0, 11,255),
	rgb_t(  0, 15,255),
	rgb_t(  0, 19,255),
	rgb_t(  0, 23,255),
	rgb_t(  0, 27,255),
	rgb_t(  0, 31,255),
	rgb_t(  0, 35,255),
	rgb_t(  0, 39,255),
	rgb_t(  0, 43,255),
	rgb_t(  0, 47,255),
	rgb_t(  0, 51,255),
	rgb_t(  0, 55,255),
	rgb_t(  0, 59,255),
	rgb_t(  0, 63,255),
	rgb_t(  0, 67,255),
	rgb_t(  0, 71,255),
	rgb_t(  0, 75,255),
	rgb_t(  0, 79,255),
	rgb_t(  0, 83,255),
	rgb_t(  0, 87,255),
	rgb_t(  0, 91,255),
	rgb_t(  0, 95,255),
	rgb_t(  0, 99,255),
	rgb_t(  0,103,255),
	rgb_t(  0,107,255),
	rgb_t(  0,111,255),
	rgb_t(  0,115,255),
	rgb_t(  0,119,255),
	rgb_t(  0,123,255),
	rgb_t(  0,127,255),
	rgb_t(  0,132,255),
	rgb_t(  0,135,255),
	rgb_t(  0,140,255),
	rgb_t(  0,143,255),
	rgb_t(  0,148,255),
	rgb_t(  0,151,255),
	rgb_t(  0,156,255),
	rgb_t(  0,159,255),
	rgb_t(  0,164,255),
	rgb_t(  0,167,255),
	rgb_t(  0,172,255),
	rgb_t(  0,175,255),
	rgb_t(  0,180,255),
	rgb_t(  0,183,255),
	rgb_t(  0,188,255),
	rgb_t(  0,191,255),
	rgb_t(  0,196,255),
	rgb_t(  0,199,255),
	rgb_t(  0,204,255),
	rgb_t(  0,207,255),
	rgb_t(  0,212,255),
	rgb_t(  0,215,255),
	rgb_t(  0,220,255),
	rgb_t(  0,223,255),
	rgb_t(  0,228,255),
	rgb_t(  0,231,255),
	rgb_t(  0,236,255),
	rgb_t(  0,239,255),
	rgb_t(  0,244,255),
	rgb_t(  0,247,255),
	rgb_t(  0,252,255),
	rgb_t(  0,255,254),
	rgb_t(  0,255,249),
	rgb_t(  0,255,246),
	rgb_t(  0,255,241),
	rgb_t(  0,255,238),
	rgb_t(  0,255,233),
	rgb_t(  0,255,230),
	rgb_t(  0,255,225),
	rgb_t(  0,255,222),
	rgb_t(  0,255,217),
	rgb_t(  0,255,214),
	rgb_t(  0,255,209),
	rgb_t(  0,255,206),
	rgb_t(  0,255,201),
	rgb_t(  0,255,198),
	rgb_t(  0,255,193),
	rgb_t(  0,255,190),
	rgb_t(  0,255,185),
	rgb_t(  0,255,182),
	rgb_t(  0,255,177),
	rgb_t(  0,255,174),
	rgb_t(  0,255,169),
	rgb_t(  0,255,166),
	rgb_t(  0,255,161),
	rgb_t(  0,255,158),
	rgb_t(  0,255,153),
	rgb_t(  0,255,150),
	rgb_t(  0,255,145),
	rgb_t(  0,255,142),
	rgb_t(  0,255,137),
	rgb_t(  0,255,134),
	rgb_t(  0,255,129),
	rgb_t(  0,255,126),
	rgb_t(  0,255,122),
	rgb_t(  0,255,118),
	rgb_t(  0,255,114),
	rgb_t(  0,255,110),
	rgb_t(  0,255,106),
	rgb_t(  0,255,102),
	rgb_t(  0,255, 98),
	rgb_t(  0,255, 94),
	rgb_t(  0,255, 90),
	rgb_t(  0,255, 86),
	rgb_t(  0,255, 82),
	rgb_t(  0,255, 78),
	rgb_t(  0,255, 74),
	rgb_t(  0,255, 70),
	rgb_t(  0,255, 66),
	rgb_t(  0,255, 61),
	rgb_t(  0,255, 57),
	rgb_t(  0,255, 53),
	rgb_t(  0,255, 49),
	rgb_t(  0,255, 45),
	rgb_t(  0,255, 41),
	rgb_t(  0,255, 37),
	rgb_t(  0,255, 33),
	rgb_t(  0,255, 29),
	rgb_t(  0,255, 25),
	rgb_t(  0,255, 21),
	rgb_t(  0,255, 17),
	rgb_t(  0,255, 13),
	rgb_t(  0,255,  9),
	rgb_t(  0,255,  5),
	rgb_t(  0,255,  1),
	rgb_t(  1,255,  0),
	rgb_t(  5,255,  0),
	rgb_t(  9,255,  0),
	rgb_t( 13,255,  0),
	rgb_t( 17,255,  0),
	rgb_t( 21,255,  0),
	rgb_t( 25,255,  0),
	rgb_t( 29,255,  0),
	rgb_t( 33,255,  0),
	rgb_t( 37,255,  0),
	rgb_t( 41,255,  0),
	rgb_t( 45,255,  0),
	rgb_t( 49,255,  0),
	rgb_t( 53,255,  0),
	rgb_t( 57,255,  0),
	rgb_t( 61,255,  0),
	rgb_t( 66,255,  0),
	rgb_t( 70,255,  0),
	rgb_t( 74,255,  0),
	rgb_t( 78,255,  0),
	rgb_t( 82,255,  0),
	rgb_t( 86,255,  0),
	rgb_t( 90,255,  0),
	rgb_t( 94,255,  0),
	rgb_t( 98,255,  0),
	rgb_t(102,255,  0),
	rgb_t(106,255,  0),
	rgb_t(110,255,  0),
	rgb_t(114,255,  0),
	rgb_t(118,255,  0),
	rgb_t(122,255,  0),
	rgb_t(126,255,  0),
	rgb_t(129,255,  0),
	rgb_t(134,255,  0),
	rgb_t(137,255,  0),
	rgb_t(142,255,  0),
	rgb_t(145,255,  0),
	rgb_t(150,255,  0),
	rgb_t(153,255,  0),
	rgb_t(158,255,  0),
	rgb_t(161,255,  0),
	rgb_t(166,255,  0),
	rgb_t(169,255,  0),
	rgb_t(174,255,  0),
	rgb_t(177,255,  0),
	rgb_t(182,255,  0),
	rgb_t(185,255,  0),
	rgb_t(190,255,  0),
	rgb_t(193,255,  0),
	rgb_t(198,255,  0),
	rgb_t(201,255,  0),
	rgb_t(206,255,  0),
	rgb_t(209,255,  0),
	rgb_t(214,255,  0),
	rgb_t(217,255,  0),
	rgb_t(222,255,  0),
	rgb_t(225,255,  0),
	rgb_t(230,255,  0),
	rgb_t(233,255,  0),
	rgb_t(238,255,  0),
	rgb_t(241,255,  0),
	rgb_t(246,255,  0),
	rgb_t(249,255,  0),
	rgb_t(254,255,  0),
	rgb_t(255,255,  2),
	rgb_t(255,255,  6),
	rgb_t(255,255, 10),
	rgb_t(255,255, 14),
	rgb_t(255,255, 18),
	rgb_t(255,255, 22),
	rgb_t(255,255, 26),
	rgb_t(255,255, 30),
	rgb_t(255,255, 35),
	rgb_t(255,255, 39),
	rgb_t(255,255, 43),
	rgb_t(255,255, 47),
	rgb_t(255,255, 51),
	rgb_t(255,255, 55),
	rgb_t(255,255, 59),
	rgb_t(255,255, 63),
	rgb_t(255,255, 67),
	rgb_t(255,255, 71),
	rgb_t(255,255, 75),
	rgb_t(255,255, 79),
	rgb_t(255,255, 83),
	rgb_t(255,255, 87),
	rgb_t(255,255, 91),
	rgb_t(255,255, 95),
	rgb_t(255,255, 99),
	rgb_t(255,255,103),
	rgb_t(255,255,107),
	rgb_t(255,255,111),
	rgb_t(255,255,115),
	rgb_t(255,255,119),
	rgb_t(255,255,123),
	rgb_t(255,255,127),
	rgb_t(255,255,130),
	rgb_t(255,255,135),
	rgb_t(255,255,138),
	rgb_t(255,255,143),
	rgb_t(255,255,146),
	rgb_t(255,255,151),
	rgb_t(255,255,154),
	rgb_t(255,255,159),
	rgb_t(255,255,162),
	rgb_t(255,255,167),
	rgb_t(255,255,170),
	rgb_t(255,255,175),
	rgb_t(255,255,178),
	rgb_t(255,255,183),
	rgb_t(255,255,186),
	rgb_t(255,255,191),
	rgb_t(255,255,194),
	rgb_t(255,255,199),
	rgb_t(255,255,202),
	rgb_t(255,255,207),
	rgb_t(255,255,210),
	rgb_t(255,255,215),
	rgb_t(255,255,218),
	rgb_t(255,255,223),
	rgb_t(255,255,226),
	rgb_t(255,255,231),
	rgb_t(255,255,234),
	rgb_t(255,255,239),
	rgb_t(255,255,242),
	rgb_t(255,255,247),
	rgb_t(255,255,250),
	rgb_t(255,255,255)
};

}}
