#ifndef __REGION_CLASSIFIER_H__
#define __REGION_CLASSIFIER_H__

#include <stdint.h>
#include <vector>

#include <field_color_detector.h>
#include <linesegment.h>
#include <point_2d.h>

namespace htwk {

#define maxEdgesPerScanline 16 //to reduce memory amount and cpu time

struct Scanline{
	int vx,vy;
	int edgeCnt;
	int edgesX[maxEdgesPerScanline];
	int edgesY[maxEdgesPerScanline];
	int edgesIntensity[maxEdgesPerScanline];
	int regionsCy[maxEdgesPerScanline];
	int regionsCb[maxEdgesPerScanline];
	int regionsCr[maxEdgesPerScanline];
	bool regionsIsGreen[maxEdgesPerScanline];
	bool regionsIsWhite[maxEdgesPerScanline];
	int link[maxEdgesPerScanline];
};

class RegionClassifier{
private:
	int *lutCb;
	int *lutCr;
	const int width_;
	const int height_;

	inline uint8_t getY(uint8_t *img,int32_t x,int32_t y) const __attribute__((nonnull)) __attribute__((pure)){
		CHECK_RANGE(x,0,width_-1);
		CHECK_RANGE(y,0,height_-1);
		return img[(x+y*width_)<<1];
	}
	inline uint8_t getCb(uint8_t *img,int32_t x,int32_t y) const __attribute__((nonnull)) __attribute__((pure)){
		CHECK_RANGE(x,0,width_-1);
		CHECK_RANGE(y,0,height_-1);
		return img[((x+y*width_)<<1)+lutCb[x]];
	}
	inline uint8_t getCr(uint8_t *img,int32_t x,int32_t y) const __attribute__((nonnull)) __attribute__((pure)){
		CHECK_RANGE(x,0,width_-1);
		CHECK_RANGE(y,0,height_-1);
		return img[((x+y*width_)<<1)+lutCr[x]];
	}

	RegionClassifier();
	RegionClassifier(const RegionClassifier & cpy);
	RegionClassifier operator= (RegionClassifier & cpy);
public:

    inline void setY(uint8_t* const img, const int32_t x,int32_t y, const uint8_t c) __attribute__((nonnull)){
        CHECK_RANGE(x,0,width-1);
        CHECK_RANGE(y,0,height-1);
        img[(x+y*width_)<<1]=c;
    }

	static const int lineSpacing=16;
    static const int matchRadius=2;
    static const int searchRadius=2;
    static const int searchLen=8;

	Scanline *scanVertical;
	Scanline *scanHorizontal;

	static int tEdge;
	static int maxEdgesInLine;
	static int maxLineBorder;
	int lineRegionsCnt;
    int pattern[matchRadius*2+1];
	std::vector<LineSegment*> *lineSegments;

	RegionClassifier(int *lutCb, int *lutCr, int width, int height) __attribute__((nonnull));
	~RegionClassifier();

    static int getLineSpacing() { return lineSpacing; }
    int getScanVerticalSize() { return width_/lineSpacing; }
	static void classifyGreenRegions(Scanline *sl, FieldColorDetector *field) __attribute__((nonnull));
	static void classifyWhiteRegions(Scanline *sl) __attribute__((nonnull));
	bool addEdge(uint8_t *img, Scanline *scanline, int xPeak, int yPeak, int edgeIntensity, bool optimize) const __attribute__((nonnull));

	void proceed(uint8_t *img, FieldColorDetector *field) __attribute__((nonnull));
	void scan(uint8_t *img, int xPos, int yPos, FieldColorDetector *field, Scanline *scanline) const __attribute__((nonnull));
	point_2d getGradientVector(int x, int y, int lineWidth, uint8_t *img) __attribute__((nonnull));
	void getColorsFromRegions(uint8_t *img, Scanline *sl, int dirX, int dirY) const __attribute__((nonnull));
	void addSegments(Scanline *scanlines, int scanlineCnt, uint8_t *img)  __attribute__((nonnull));
	std::vector<LineSegment*> getLineSegments(const int* const fieldborder);
    Scanline *getScanVertical() const { return scanVertical; }
    Scanline *getScanHorizontal() const { return scanHorizontal; }
};

}  // namespace htwk

#endif  // __REGION_CLASSIFIER_H__
