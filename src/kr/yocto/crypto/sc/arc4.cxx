void arc4::init( const uint8_t *key, size_t len ) throw()
{
	x=y=0;
    S[0x00] = 0x00;
    S[0x01] = 0x01;
    S[0x02] = 0x02;
    S[0x03] = 0x03;
    S[0x04] = 0x04;
    S[0x05] = 0x05;
    S[0x06] = 0x06;
    S[0x07] = 0x07;
    S[0x08] = 0x08;
    S[0x09] = 0x09;
    S[0x0a] = 0x0a;
    S[0x0b] = 0x0b;
    S[0x0c] = 0x0c;
    S[0x0d] = 0x0d;
    S[0x0e] = 0x0e;
    S[0x0f] = 0x0f;
    S[0x10] = 0x10;
    S[0x11] = 0x11;
    S[0x12] = 0x12;
    S[0x13] = 0x13;
    S[0x14] = 0x14;
    S[0x15] = 0x15;
    S[0x16] = 0x16;
    S[0x17] = 0x17;
    S[0x18] = 0x18;
    S[0x19] = 0x19;
    S[0x1a] = 0x1a;
    S[0x1b] = 0x1b;
    S[0x1c] = 0x1c;
    S[0x1d] = 0x1d;
    S[0x1e] = 0x1e;
    S[0x1f] = 0x1f;
    S[0x20] = 0x20;
    S[0x21] = 0x21;
    S[0x22] = 0x22;
    S[0x23] = 0x23;
    S[0x24] = 0x24;
    S[0x25] = 0x25;
    S[0x26] = 0x26;
    S[0x27] = 0x27;
    S[0x28] = 0x28;
    S[0x29] = 0x29;
    S[0x2a] = 0x2a;
    S[0x2b] = 0x2b;
    S[0x2c] = 0x2c;
    S[0x2d] = 0x2d;
    S[0x2e] = 0x2e;
    S[0x2f] = 0x2f;
    S[0x30] = 0x30;
    S[0x31] = 0x31;
    S[0x32] = 0x32;
    S[0x33] = 0x33;
    S[0x34] = 0x34;
    S[0x35] = 0x35;
    S[0x36] = 0x36;
    S[0x37] = 0x37;
    S[0x38] = 0x38;
    S[0x39] = 0x39;
    S[0x3a] = 0x3a;
    S[0x3b] = 0x3b;
    S[0x3c] = 0x3c;
    S[0x3d] = 0x3d;
    S[0x3e] = 0x3e;
    S[0x3f] = 0x3f;
    S[0x40] = 0x40;
    S[0x41] = 0x41;
    S[0x42] = 0x42;
    S[0x43] = 0x43;
    S[0x44] = 0x44;
    S[0x45] = 0x45;
    S[0x46] = 0x46;
    S[0x47] = 0x47;
    S[0x48] = 0x48;
    S[0x49] = 0x49;
    S[0x4a] = 0x4a;
    S[0x4b] = 0x4b;
    S[0x4c] = 0x4c;
    S[0x4d] = 0x4d;
    S[0x4e] = 0x4e;
    S[0x4f] = 0x4f;
    S[0x50] = 0x50;
    S[0x51] = 0x51;
    S[0x52] = 0x52;
    S[0x53] = 0x53;
    S[0x54] = 0x54;
    S[0x55] = 0x55;
    S[0x56] = 0x56;
    S[0x57] = 0x57;
    S[0x58] = 0x58;
    S[0x59] = 0x59;
    S[0x5a] = 0x5a;
    S[0x5b] = 0x5b;
    S[0x5c] = 0x5c;
    S[0x5d] = 0x5d;
    S[0x5e] = 0x5e;
    S[0x5f] = 0x5f;
    S[0x60] = 0x60;
    S[0x61] = 0x61;
    S[0x62] = 0x62;
    S[0x63] = 0x63;
    S[0x64] = 0x64;
    S[0x65] = 0x65;
    S[0x66] = 0x66;
    S[0x67] = 0x67;
    S[0x68] = 0x68;
    S[0x69] = 0x69;
    S[0x6a] = 0x6a;
    S[0x6b] = 0x6b;
    S[0x6c] = 0x6c;
    S[0x6d] = 0x6d;
    S[0x6e] = 0x6e;
    S[0x6f] = 0x6f;
    S[0x70] = 0x70;
    S[0x71] = 0x71;
    S[0x72] = 0x72;
    S[0x73] = 0x73;
    S[0x74] = 0x74;
    S[0x75] = 0x75;
    S[0x76] = 0x76;
    S[0x77] = 0x77;
    S[0x78] = 0x78;
    S[0x79] = 0x79;
    S[0x7a] = 0x7a;
    S[0x7b] = 0x7b;
    S[0x7c] = 0x7c;
    S[0x7d] = 0x7d;
    S[0x7e] = 0x7e;
    S[0x7f] = 0x7f;
    S[0x80] = 0x80;
    S[0x81] = 0x81;
    S[0x82] = 0x82;
    S[0x83] = 0x83;
    S[0x84] = 0x84;
    S[0x85] = 0x85;
    S[0x86] = 0x86;
    S[0x87] = 0x87;
    S[0x88] = 0x88;
    S[0x89] = 0x89;
    S[0x8a] = 0x8a;
    S[0x8b] = 0x8b;
    S[0x8c] = 0x8c;
    S[0x8d] = 0x8d;
    S[0x8e] = 0x8e;
    S[0x8f] = 0x8f;
    S[0x90] = 0x90;
    S[0x91] = 0x91;
    S[0x92] = 0x92;
    S[0x93] = 0x93;
    S[0x94] = 0x94;
    S[0x95] = 0x95;
    S[0x96] = 0x96;
    S[0x97] = 0x97;
    S[0x98] = 0x98;
    S[0x99] = 0x99;
    S[0x9a] = 0x9a;
    S[0x9b] = 0x9b;
    S[0x9c] = 0x9c;
    S[0x9d] = 0x9d;
    S[0x9e] = 0x9e;
    S[0x9f] = 0x9f;
    S[0xa0] = 0xa0;
    S[0xa1] = 0xa1;
    S[0xa2] = 0xa2;
    S[0xa3] = 0xa3;
    S[0xa4] = 0xa4;
    S[0xa5] = 0xa5;
    S[0xa6] = 0xa6;
    S[0xa7] = 0xa7;
    S[0xa8] = 0xa8;
    S[0xa9] = 0xa9;
    S[0xaa] = 0xaa;
    S[0xab] = 0xab;
    S[0xac] = 0xac;
    S[0xad] = 0xad;
    S[0xae] = 0xae;
    S[0xaf] = 0xaf;
    S[0xb0] = 0xb0;
    S[0xb1] = 0xb1;
    S[0xb2] = 0xb2;
    S[0xb3] = 0xb3;
    S[0xb4] = 0xb4;
    S[0xb5] = 0xb5;
    S[0xb6] = 0xb6;
    S[0xb7] = 0xb7;
    S[0xb8] = 0xb8;
    S[0xb9] = 0xb9;
    S[0xba] = 0xba;
    S[0xbb] = 0xbb;
    S[0xbc] = 0xbc;
    S[0xbd] = 0xbd;
    S[0xbe] = 0xbe;
    S[0xbf] = 0xbf;
    S[0xc0] = 0xc0;
    S[0xc1] = 0xc1;
    S[0xc2] = 0xc2;
    S[0xc3] = 0xc3;
    S[0xc4] = 0xc4;
    S[0xc5] = 0xc5;
    S[0xc6] = 0xc6;
    S[0xc7] = 0xc7;
    S[0xc8] = 0xc8;
    S[0xc9] = 0xc9;
    S[0xca] = 0xca;
    S[0xcb] = 0xcb;
    S[0xcc] = 0xcc;
    S[0xcd] = 0xcd;
    S[0xce] = 0xce;
    S[0xcf] = 0xcf;
    S[0xd0] = 0xd0;
    S[0xd1] = 0xd1;
    S[0xd2] = 0xd2;
    S[0xd3] = 0xd3;
    S[0xd4] = 0xd4;
    S[0xd5] = 0xd5;
    S[0xd6] = 0xd6;
    S[0xd7] = 0xd7;
    S[0xd8] = 0xd8;
    S[0xd9] = 0xd9;
    S[0xda] = 0xda;
    S[0xdb] = 0xdb;
    S[0xdc] = 0xdc;
    S[0xdd] = 0xdd;
    S[0xde] = 0xde;
    S[0xdf] = 0xdf;
    S[0xe0] = 0xe0;
    S[0xe1] = 0xe1;
    S[0xe2] = 0xe2;
    S[0xe3] = 0xe3;
    S[0xe4] = 0xe4;
    S[0xe5] = 0xe5;
    S[0xe6] = 0xe6;
    S[0xe7] = 0xe7;
    S[0xe8] = 0xe8;
    S[0xe9] = 0xe9;
    S[0xea] = 0xea;
    S[0xeb] = 0xeb;
    S[0xec] = 0xec;
    S[0xed] = 0xed;
    S[0xee] = 0xee;
    S[0xef] = 0xef;
    S[0xf0] = 0xf0;
    S[0xf1] = 0xf1;
    S[0xf2] = 0xf2;
    S[0xf3] = 0xf3;
    S[0xf4] = 0xf4;
    S[0xf5] = 0xf5;
    S[0xf6] = 0xf6;
    S[0xf7] = 0xf7;
    S[0xf8] = 0xf8;
    S[0xf9] = 0xf9;
    S[0xfa] = 0xfa;
    S[0xfb] = 0xfb;
    S[0xfc] = 0xfc;
    S[0xfd] = 0xfd;
    S[0xfe] = 0xfe;
    S[0xff] = 0xff;
    
	if (!len) return;
    
	register unsigned j=0;
	j = ( j + unsigned(S[0x00]) + unsigned( key[ 0x00 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x00]; S[0x00] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x01]) + unsigned( key[ 0x01 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x01]; S[0x01] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x02]) + unsigned( key[ 0x02 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x02]; S[0x02] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x03]) + unsigned( key[ 0x03 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x03]; S[0x03] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x04]) + unsigned( key[ 0x04 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x04]; S[0x04] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x05]) + unsigned( key[ 0x05 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x05]; S[0x05] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x06]) + unsigned( key[ 0x06 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x06]; S[0x06] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x07]) + unsigned( key[ 0x07 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x07]; S[0x07] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x08]) + unsigned( key[ 0x08 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x08]; S[0x08] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x09]) + unsigned( key[ 0x09 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x09]; S[0x09] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x0a]) + unsigned( key[ 0x0a % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x0a]; S[0x0a] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x0b]) + unsigned( key[ 0x0b % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x0b]; S[0x0b] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x0c]) + unsigned( key[ 0x0c % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x0c]; S[0x0c] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x0d]) + unsigned( key[ 0x0d % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x0d]; S[0x0d] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x0e]) + unsigned( key[ 0x0e % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x0e]; S[0x0e] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x0f]) + unsigned( key[ 0x0f % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x0f]; S[0x0f] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x10]) + unsigned( key[ 0x10 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x10]; S[0x10] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x11]) + unsigned( key[ 0x11 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x11]; S[0x11] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x12]) + unsigned( key[ 0x12 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x12]; S[0x12] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x13]) + unsigned( key[ 0x13 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x13]; S[0x13] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x14]) + unsigned( key[ 0x14 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x14]; S[0x14] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x15]) + unsigned( key[ 0x15 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x15]; S[0x15] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x16]) + unsigned( key[ 0x16 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x16]; S[0x16] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x17]) + unsigned( key[ 0x17 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x17]; S[0x17] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x18]) + unsigned( key[ 0x18 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x18]; S[0x18] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x19]) + unsigned( key[ 0x19 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x19]; S[0x19] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x1a]) + unsigned( key[ 0x1a % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x1a]; S[0x1a] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x1b]) + unsigned( key[ 0x1b % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x1b]; S[0x1b] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x1c]) + unsigned( key[ 0x1c % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x1c]; S[0x1c] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x1d]) + unsigned( key[ 0x1d % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x1d]; S[0x1d] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x1e]) + unsigned( key[ 0x1e % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x1e]; S[0x1e] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x1f]) + unsigned( key[ 0x1f % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x1f]; S[0x1f] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x20]) + unsigned( key[ 0x20 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x20]; S[0x20] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x21]) + unsigned( key[ 0x21 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x21]; S[0x21] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x22]) + unsigned( key[ 0x22 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x22]; S[0x22] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x23]) + unsigned( key[ 0x23 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x23]; S[0x23] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x24]) + unsigned( key[ 0x24 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x24]; S[0x24] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x25]) + unsigned( key[ 0x25 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x25]; S[0x25] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x26]) + unsigned( key[ 0x26 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x26]; S[0x26] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x27]) + unsigned( key[ 0x27 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x27]; S[0x27] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x28]) + unsigned( key[ 0x28 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x28]; S[0x28] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x29]) + unsigned( key[ 0x29 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x29]; S[0x29] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x2a]) + unsigned( key[ 0x2a % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x2a]; S[0x2a] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x2b]) + unsigned( key[ 0x2b % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x2b]; S[0x2b] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x2c]) + unsigned( key[ 0x2c % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x2c]; S[0x2c] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x2d]) + unsigned( key[ 0x2d % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x2d]; S[0x2d] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x2e]) + unsigned( key[ 0x2e % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x2e]; S[0x2e] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x2f]) + unsigned( key[ 0x2f % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x2f]; S[0x2f] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x30]) + unsigned( key[ 0x30 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x30]; S[0x30] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x31]) + unsigned( key[ 0x31 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x31]; S[0x31] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x32]) + unsigned( key[ 0x32 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x32]; S[0x32] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x33]) + unsigned( key[ 0x33 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x33]; S[0x33] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x34]) + unsigned( key[ 0x34 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x34]; S[0x34] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x35]) + unsigned( key[ 0x35 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x35]; S[0x35] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x36]) + unsigned( key[ 0x36 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x36]; S[0x36] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x37]) + unsigned( key[ 0x37 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x37]; S[0x37] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x38]) + unsigned( key[ 0x38 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x38]; S[0x38] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x39]) + unsigned( key[ 0x39 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x39]; S[0x39] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x3a]) + unsigned( key[ 0x3a % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x3a]; S[0x3a] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x3b]) + unsigned( key[ 0x3b % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x3b]; S[0x3b] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x3c]) + unsigned( key[ 0x3c % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x3c]; S[0x3c] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x3d]) + unsigned( key[ 0x3d % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x3d]; S[0x3d] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x3e]) + unsigned( key[ 0x3e % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x3e]; S[0x3e] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x3f]) + unsigned( key[ 0x3f % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x3f]; S[0x3f] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x40]) + unsigned( key[ 0x40 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x40]; S[0x40] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x41]) + unsigned( key[ 0x41 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x41]; S[0x41] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x42]) + unsigned( key[ 0x42 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x42]; S[0x42] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x43]) + unsigned( key[ 0x43 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x43]; S[0x43] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x44]) + unsigned( key[ 0x44 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x44]; S[0x44] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x45]) + unsigned( key[ 0x45 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x45]; S[0x45] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x46]) + unsigned( key[ 0x46 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x46]; S[0x46] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x47]) + unsigned( key[ 0x47 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x47]; S[0x47] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x48]) + unsigned( key[ 0x48 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x48]; S[0x48] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x49]) + unsigned( key[ 0x49 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x49]; S[0x49] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x4a]) + unsigned( key[ 0x4a % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x4a]; S[0x4a] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x4b]) + unsigned( key[ 0x4b % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x4b]; S[0x4b] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x4c]) + unsigned( key[ 0x4c % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x4c]; S[0x4c] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x4d]) + unsigned( key[ 0x4d % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x4d]; S[0x4d] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x4e]) + unsigned( key[ 0x4e % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x4e]; S[0x4e] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x4f]) + unsigned( key[ 0x4f % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x4f]; S[0x4f] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x50]) + unsigned( key[ 0x50 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x50]; S[0x50] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x51]) + unsigned( key[ 0x51 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x51]; S[0x51] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x52]) + unsigned( key[ 0x52 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x52]; S[0x52] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x53]) + unsigned( key[ 0x53 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x53]; S[0x53] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x54]) + unsigned( key[ 0x54 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x54]; S[0x54] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x55]) + unsigned( key[ 0x55 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x55]; S[0x55] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x56]) + unsigned( key[ 0x56 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x56]; S[0x56] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x57]) + unsigned( key[ 0x57 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x57]; S[0x57] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x58]) + unsigned( key[ 0x58 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x58]; S[0x58] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x59]) + unsigned( key[ 0x59 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x59]; S[0x59] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x5a]) + unsigned( key[ 0x5a % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x5a]; S[0x5a] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x5b]) + unsigned( key[ 0x5b % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x5b]; S[0x5b] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x5c]) + unsigned( key[ 0x5c % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x5c]; S[0x5c] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x5d]) + unsigned( key[ 0x5d % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x5d]; S[0x5d] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x5e]) + unsigned( key[ 0x5e % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x5e]; S[0x5e] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x5f]) + unsigned( key[ 0x5f % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x5f]; S[0x5f] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x60]) + unsigned( key[ 0x60 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x60]; S[0x60] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x61]) + unsigned( key[ 0x61 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x61]; S[0x61] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x62]) + unsigned( key[ 0x62 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x62]; S[0x62] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x63]) + unsigned( key[ 0x63 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x63]; S[0x63] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x64]) + unsigned( key[ 0x64 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x64]; S[0x64] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x65]) + unsigned( key[ 0x65 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x65]; S[0x65] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x66]) + unsigned( key[ 0x66 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x66]; S[0x66] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x67]) + unsigned( key[ 0x67 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x67]; S[0x67] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x68]) + unsigned( key[ 0x68 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x68]; S[0x68] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x69]) + unsigned( key[ 0x69 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x69]; S[0x69] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x6a]) + unsigned( key[ 0x6a % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x6a]; S[0x6a] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x6b]) + unsigned( key[ 0x6b % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x6b]; S[0x6b] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x6c]) + unsigned( key[ 0x6c % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x6c]; S[0x6c] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x6d]) + unsigned( key[ 0x6d % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x6d]; S[0x6d] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x6e]) + unsigned( key[ 0x6e % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x6e]; S[0x6e] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x6f]) + unsigned( key[ 0x6f % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x6f]; S[0x6f] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x70]) + unsigned( key[ 0x70 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x70]; S[0x70] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x71]) + unsigned( key[ 0x71 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x71]; S[0x71] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x72]) + unsigned( key[ 0x72 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x72]; S[0x72] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x73]) + unsigned( key[ 0x73 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x73]; S[0x73] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x74]) + unsigned( key[ 0x74 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x74]; S[0x74] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x75]) + unsigned( key[ 0x75 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x75]; S[0x75] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x76]) + unsigned( key[ 0x76 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x76]; S[0x76] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x77]) + unsigned( key[ 0x77 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x77]; S[0x77] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x78]) + unsigned( key[ 0x78 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x78]; S[0x78] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x79]) + unsigned( key[ 0x79 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x79]; S[0x79] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x7a]) + unsigned( key[ 0x7a % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x7a]; S[0x7a] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x7b]) + unsigned( key[ 0x7b % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x7b]; S[0x7b] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x7c]) + unsigned( key[ 0x7c % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x7c]; S[0x7c] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x7d]) + unsigned( key[ 0x7d % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x7d]; S[0x7d] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x7e]) + unsigned( key[ 0x7e % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x7e]; S[0x7e] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x7f]) + unsigned( key[ 0x7f % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x7f]; S[0x7f] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x80]) + unsigned( key[ 0x80 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x80]; S[0x80] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x81]) + unsigned( key[ 0x81 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x81]; S[0x81] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x82]) + unsigned( key[ 0x82 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x82]; S[0x82] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x83]) + unsigned( key[ 0x83 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x83]; S[0x83] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x84]) + unsigned( key[ 0x84 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x84]; S[0x84] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x85]) + unsigned( key[ 0x85 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x85]; S[0x85] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x86]) + unsigned( key[ 0x86 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x86]; S[0x86] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x87]) + unsigned( key[ 0x87 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x87]; S[0x87] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x88]) + unsigned( key[ 0x88 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x88]; S[0x88] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x89]) + unsigned( key[ 0x89 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x89]; S[0x89] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x8a]) + unsigned( key[ 0x8a % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x8a]; S[0x8a] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x8b]) + unsigned( key[ 0x8b % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x8b]; S[0x8b] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x8c]) + unsigned( key[ 0x8c % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x8c]; S[0x8c] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x8d]) + unsigned( key[ 0x8d % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x8d]; S[0x8d] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x8e]) + unsigned( key[ 0x8e % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x8e]; S[0x8e] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x8f]) + unsigned( key[ 0x8f % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x8f]; S[0x8f] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x90]) + unsigned( key[ 0x90 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x90]; S[0x90] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x91]) + unsigned( key[ 0x91 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x91]; S[0x91] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x92]) + unsigned( key[ 0x92 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x92]; S[0x92] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x93]) + unsigned( key[ 0x93 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x93]; S[0x93] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x94]) + unsigned( key[ 0x94 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x94]; S[0x94] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x95]) + unsigned( key[ 0x95 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x95]; S[0x95] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x96]) + unsigned( key[ 0x96 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x96]; S[0x96] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x97]) + unsigned( key[ 0x97 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x97]; S[0x97] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x98]) + unsigned( key[ 0x98 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x98]; S[0x98] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x99]) + unsigned( key[ 0x99 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x99]; S[0x99] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x9a]) + unsigned( key[ 0x9a % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x9a]; S[0x9a] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x9b]) + unsigned( key[ 0x9b % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x9b]; S[0x9b] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x9c]) + unsigned( key[ 0x9c % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x9c]; S[0x9c] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x9d]) + unsigned( key[ 0x9d % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x9d]; S[0x9d] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x9e]) + unsigned( key[ 0x9e % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x9e]; S[0x9e] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0x9f]) + unsigned( key[ 0x9f % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0x9f]; S[0x9f] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xa0]) + unsigned( key[ 0xa0 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xa0]; S[0xa0] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xa1]) + unsigned( key[ 0xa1 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xa1]; S[0xa1] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xa2]) + unsigned( key[ 0xa2 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xa2]; S[0xa2] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xa3]) + unsigned( key[ 0xa3 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xa3]; S[0xa3] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xa4]) + unsigned( key[ 0xa4 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xa4]; S[0xa4] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xa5]) + unsigned( key[ 0xa5 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xa5]; S[0xa5] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xa6]) + unsigned( key[ 0xa6 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xa6]; S[0xa6] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xa7]) + unsigned( key[ 0xa7 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xa7]; S[0xa7] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xa8]) + unsigned( key[ 0xa8 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xa8]; S[0xa8] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xa9]) + unsigned( key[ 0xa9 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xa9]; S[0xa9] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xaa]) + unsigned( key[ 0xaa % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xaa]; S[0xaa] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xab]) + unsigned( key[ 0xab % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xab]; S[0xab] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xac]) + unsigned( key[ 0xac % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xac]; S[0xac] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xad]) + unsigned( key[ 0xad % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xad]; S[0xad] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xae]) + unsigned( key[ 0xae % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xae]; S[0xae] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xaf]) + unsigned( key[ 0xaf % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xaf]; S[0xaf] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xb0]) + unsigned( key[ 0xb0 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xb0]; S[0xb0] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xb1]) + unsigned( key[ 0xb1 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xb1]; S[0xb1] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xb2]) + unsigned( key[ 0xb2 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xb2]; S[0xb2] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xb3]) + unsigned( key[ 0xb3 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xb3]; S[0xb3] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xb4]) + unsigned( key[ 0xb4 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xb4]; S[0xb4] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xb5]) + unsigned( key[ 0xb5 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xb5]; S[0xb5] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xb6]) + unsigned( key[ 0xb6 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xb6]; S[0xb6] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xb7]) + unsigned( key[ 0xb7 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xb7]; S[0xb7] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xb8]) + unsigned( key[ 0xb8 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xb8]; S[0xb8] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xb9]) + unsigned( key[ 0xb9 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xb9]; S[0xb9] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xba]) + unsigned( key[ 0xba % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xba]; S[0xba] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xbb]) + unsigned( key[ 0xbb % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xbb]; S[0xbb] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xbc]) + unsigned( key[ 0xbc % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xbc]; S[0xbc] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xbd]) + unsigned( key[ 0xbd % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xbd]; S[0xbd] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xbe]) + unsigned( key[ 0xbe % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xbe]; S[0xbe] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xbf]) + unsigned( key[ 0xbf % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xbf]; S[0xbf] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xc0]) + unsigned( key[ 0xc0 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xc0]; S[0xc0] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xc1]) + unsigned( key[ 0xc1 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xc1]; S[0xc1] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xc2]) + unsigned( key[ 0xc2 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xc2]; S[0xc2] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xc3]) + unsigned( key[ 0xc3 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xc3]; S[0xc3] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xc4]) + unsigned( key[ 0xc4 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xc4]; S[0xc4] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xc5]) + unsigned( key[ 0xc5 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xc5]; S[0xc5] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xc6]) + unsigned( key[ 0xc6 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xc6]; S[0xc6] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xc7]) + unsigned( key[ 0xc7 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xc7]; S[0xc7] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xc8]) + unsigned( key[ 0xc8 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xc8]; S[0xc8] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xc9]) + unsigned( key[ 0xc9 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xc9]; S[0xc9] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xca]) + unsigned( key[ 0xca % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xca]; S[0xca] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xcb]) + unsigned( key[ 0xcb % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xcb]; S[0xcb] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xcc]) + unsigned( key[ 0xcc % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xcc]; S[0xcc] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xcd]) + unsigned( key[ 0xcd % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xcd]; S[0xcd] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xce]) + unsigned( key[ 0xce % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xce]; S[0xce] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xcf]) + unsigned( key[ 0xcf % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xcf]; S[0xcf] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xd0]) + unsigned( key[ 0xd0 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xd0]; S[0xd0] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xd1]) + unsigned( key[ 0xd1 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xd1]; S[0xd1] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xd2]) + unsigned( key[ 0xd2 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xd2]; S[0xd2] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xd3]) + unsigned( key[ 0xd3 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xd3]; S[0xd3] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xd4]) + unsigned( key[ 0xd4 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xd4]; S[0xd4] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xd5]) + unsigned( key[ 0xd5 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xd5]; S[0xd5] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xd6]) + unsigned( key[ 0xd6 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xd6]; S[0xd6] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xd7]) + unsigned( key[ 0xd7 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xd7]; S[0xd7] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xd8]) + unsigned( key[ 0xd8 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xd8]; S[0xd8] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xd9]) + unsigned( key[ 0xd9 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xd9]; S[0xd9] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xda]) + unsigned( key[ 0xda % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xda]; S[0xda] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xdb]) + unsigned( key[ 0xdb % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xdb]; S[0xdb] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xdc]) + unsigned( key[ 0xdc % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xdc]; S[0xdc] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xdd]) + unsigned( key[ 0xdd % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xdd]; S[0xdd] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xde]) + unsigned( key[ 0xde % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xde]; S[0xde] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xdf]) + unsigned( key[ 0xdf % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xdf]; S[0xdf] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xe0]) + unsigned( key[ 0xe0 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xe0]; S[0xe0] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xe1]) + unsigned( key[ 0xe1 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xe1]; S[0xe1] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xe2]) + unsigned( key[ 0xe2 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xe2]; S[0xe2] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xe3]) + unsigned( key[ 0xe3 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xe3]; S[0xe3] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xe4]) + unsigned( key[ 0xe4 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xe4]; S[0xe4] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xe5]) + unsigned( key[ 0xe5 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xe5]; S[0xe5] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xe6]) + unsigned( key[ 0xe6 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xe6]; S[0xe6] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xe7]) + unsigned( key[ 0xe7 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xe7]; S[0xe7] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xe8]) + unsigned( key[ 0xe8 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xe8]; S[0xe8] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xe9]) + unsigned( key[ 0xe9 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xe9]; S[0xe9] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xea]) + unsigned( key[ 0xea % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xea]; S[0xea] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xeb]) + unsigned( key[ 0xeb % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xeb]; S[0xeb] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xec]) + unsigned( key[ 0xec % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xec]; S[0xec] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xed]) + unsigned( key[ 0xed % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xed]; S[0xed] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xee]) + unsigned( key[ 0xee % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xee]; S[0xee] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xef]) + unsigned( key[ 0xef % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xef]; S[0xef] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xf0]) + unsigned( key[ 0xf0 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xf0]; S[0xf0] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xf1]) + unsigned( key[ 0xf1 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xf1]; S[0xf1] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xf2]) + unsigned( key[ 0xf2 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xf2]; S[0xf2] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xf3]) + unsigned( key[ 0xf3 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xf3]; S[0xf3] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xf4]) + unsigned( key[ 0xf4 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xf4]; S[0xf4] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xf5]) + unsigned( key[ 0xf5 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xf5]; S[0xf5] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xf6]) + unsigned( key[ 0xf6 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xf6]; S[0xf6] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xf7]) + unsigned( key[ 0xf7 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xf7]; S[0xf7] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xf8]) + unsigned( key[ 0xf8 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xf8]; S[0xf8] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xf9]) + unsigned( key[ 0xf9 % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xf9]; S[0xf9] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xfa]) + unsigned( key[ 0xfa % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xfa]; S[0xfa] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xfb]) + unsigned( key[ 0xfb % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xfb]; S[0xfb] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xfc]) + unsigned( key[ 0xfc % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xfc]; S[0xfc] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xfd]) + unsigned( key[ 0xfd % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xfd]; S[0xfd] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xfe]) + unsigned( key[ 0xfe % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xfe]; S[0xfe] = S[j]; S[j] = tmp; }
	j = ( j + unsigned(S[0xff]) + unsigned( key[ 0xff % len ] ) ) & 0xff;
	{ const uint8_t tmp = S[0xff]; S[0xff] = S[j]; S[j] = tmp; }
}

uint8_t arc4::call() throw()
{
	x = (1+x)  & 0xFF; const unsigned Sx = S[x];
	y = (y+Sx) & 0xFF; const unsigned Sy = S[y];
	S[x] = uint8_t(Sy);
	S[y] = uint8_t(Sx);
	return S[ ( Sx + Sy ) & 0xFF ];
}
