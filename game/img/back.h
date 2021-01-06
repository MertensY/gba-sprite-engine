
//{{BLOCK(back)

//======================================================================
//
//	back, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 44 tiles (t|f|p reduced) not compressed
//	+ regular map (in SBBs), not compressed, 32x32 
//	Total size: 512 + 2816 + 2048 = 5376
//
//	Time-stamp: 2021-01-06, 17:12:14
//	Exported by Cearn's GBA Image Transmogrifier, v0.8.6
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BACK_H
#define GRIT_BACK_H

#define backTilesLen 2816
extern const unsigned int backTiles[704];

#define backMapLen 2048
extern const unsigned short backMap[1024];

#define backPalLen 512
extern const unsigned short backPal[256];

#endif // GRIT_BACK_H

//}}BLOCK(back)
