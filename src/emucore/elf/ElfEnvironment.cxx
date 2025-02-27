//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2024 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#include "ElfEnvironment.hxx"

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
constexpr uInt8 elfEnvironment::OVERBLANK_PROGRAM[] = {
  0xa0,0x00,			  // ldy #0
  0xa5,0xe0,			  // lda $e0
        					  // OverblankLoop:
  0x85,0x02,			  // sta WSYNC
  0x85,0x2d,			  // sta AUDV0 (currently using $2d instead to disable audio until fully implemented
  0x98,				      // tya
  0x18,				      // clc
  0x6a,				      // ror
  0xaa,				      // tax
  0xb5,0xe0,			  // lda $e0,x
  0x90,0x04,			  // bcc
  0x4a,				      // lsr
  0x4a,				      // lsr
  0x4a,				      // lsr
  0x4a,				      // lsr
  0xc8,				      // iny
  0xc0, 0x1d,			  // cpy #$1d
  0xd0, 0x04,			  // bne
  0xa2, 0x02,			  // ldx #2
  0x86, 0x00,			  // stx VSYNC
  0xc0, 0x20,			  // cpy #$20
  0xd0, 0x04,			  // bne SkipClearVSync
  0xa2, 0x00,			  // ldx #0
  0x86, 0x00,			  // stx VSYNC
  					        // SkipClearVSync:
  0xc0, 0x3f,			  // cpy #$3f
  0xd0, 0xdb,			  // bne OverblankLoop
  					        // WaitForCart:
  0xae, 0xff, 0xff,	// ldx $ffff
  0xd0, 0xfb,			  // bne WaitForCart
  0x4c, 0x00, 0x10	// jmp $1000
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
constexpr uInt32 elfEnvironment::OVERBLANK_PROGRAM_SIZE = sizeof(elfEnvironment::OVERBLANK_PROGRAM);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const uInt8 elfEnvironment::LOOKUP_TABLES[3 * 256] = {
    // Ntsc2600
		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
		0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
		0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
		0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
		0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
		0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
		0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
		0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
		0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
		0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
		0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,

    // Pal2600
		0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
		0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
		0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
		0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
		0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
		0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
		0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
		0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
		0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
		0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
		0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
		0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
		0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,

    // ReverseByte
  	0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0,	0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
	  0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8,	0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
	  0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4,	0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
	  0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec,	0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
	  0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2,	0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
	  0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea,	0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
	  0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6,	0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
	  0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee,	0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
	  0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1,	0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
	  0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,	0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
	  0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5,	0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
	  0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed,	0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
	  0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3,	0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
	  0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb,	0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
	  0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7,	0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
	  0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef,	0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
vector<ElfLinker::ExternalSymbol> elfEnvironment::externalSymbols(elfEnvironment::SystemType systemType)
{
  static const vector<ElfLinker::ExternalSymbol> EXTERNAL_SYMBOLS_STATIC = {
    {"ADDR_IDR", ADDR_ADDR_IDR + 1},
    {"DATA_IDR", ADDR_DATA_IDR + 1},
    {"DATA_ODR", ADDR_DATA_ODR + 1},
    {"DATA_MODER", ADDR_DATA_MODER + 1},
    {"memset", ADDR_MEMSET + 1},
    {"memcpy", ADDR_MEMCPY + 1},
    {"vcsLdaForBusStuff2", ADDR_VCS_LDA_FOR_BUS_STUFF2 + 1},
    {"vcsLdxForBusStuff2", ADDR_VCS_LDX_FOR_BUS_STUFF2 + 1},
    {"vcsLdyForBusStuff2", ADDR_VCS_LDY_FOR_BUS_STUFF2 + 1},
    {"vcsWrite3", ADDR_VCS_WRITE3 + 1},
    {"vcsJmp3", ADDR_VCS_JMP3 + 1},
    {"vcsNop2", ADDR_VCS_NOP2 + 1},
    {"vcsNop2n", ADDR_VCS_NOP2N + 1},
    {"vcsWrite5", ADDR_VCS_WRITE5 + 1},
    {"vcsWrite6", ADDR_VCS_WRITE6 + 1},
    {"vcsLda2", ADDR_VCS_LDA2 + 1},
    {"vcsLdx2", ADDR_VCS_LDX2 + 1},
    {"vcsLdy2", ADDR_VCS_LDY2 + 1},
    {"vcsSax3", ADDR_VCS_SAX3 + 1},
    {"vcsSta3", ADDR_VCS_STA3 + 1},
    {"vcsStx3", ADDR_VCS_STX3 + 1},
    {"vcsSty3", ADDR_VCS_STY3 + 1},
    {"vcsSta4", ADDR_VCS_STA4 + 1},
    {"vcsStx4", ADDR_VCS_STX4 + 1},
    {"vcsSty4", ADDR_VCS_STY4 + 1},
    {"vcsCopyOverblankToRiotRam", ADDR_VCS_COPY_OVERBLANK_TO_RIOT_RAM + 1},
    {"vcsStartOverblank", ADDR_VCS_START_OVERBLANK + 1},
    {"vcsEndOverblank", ADDR_VCS_END_OVERBLANK + 1},
    {"vcsRead4", ADDR_VCS_READ4 + 1},
    {"randint", ADDR_RANDINT + 1},
    {"vcsTxs2", ADDR_VCS_TXS2 + 1},
    {"vcsJsr6", ADDR_VCS_JSR6 + 1},
    {"vcsPha3", ADDR_VCS_PHA3 + 1},
    {"vcsPhp3", ADDR_VCS_PHP3 + 1},
    {"vcsPla4", ADDR_VCS_PLA4 + 1},
    {"vcsPlp4", ADDR_VCS_PLP4 + 1},
    {"vcsPla4Ex", ADDR_VCS_PLA4_EX + 1},
    {"vcsPlp4Ex", ADDR_VCS_PLP4_EX + 1},
    {"vcsJmpToRam3", ADDR_VCS_JMP_TO_RAM3 + 1},
    {"vcsWaitForAddress", ADDR_VCS_WAIT_FOR_ADDRESS + 1},
    {"injectDmaData", ADDR_INJECT_DMA_DATA + 1},
    {"ReverseByte", ADDR_TABLE_COLOR_LOOKUP_REVERSE_BYTE}
  };

  vector<ElfLinker::ExternalSymbol> externalSymbols;
  externalSymbols.reserve(EXTERNAL_SYMBOLS_STATIC.size() + 1);

  externalSymbols = EXTERNAL_SYMBOLS_STATIC;
  externalSymbols.push_back({
    "ColorLookup",
    systemType == SystemType::ntsc ? ADDR_TABLE_COLOR_LOOKUP_NTSC : ADDR_TABLE_COLOR_LOOKUP_PAL
  });

  return externalSymbols;
}
