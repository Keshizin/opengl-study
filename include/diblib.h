#ifndef DIBLIB_INCLUDE_H
#define DIBLIB_INCLUDE_H

#include <string>

namespace DIBLIB
{
	class WORD
	{
	public:
		WORD();

		unsigned short get() const;
		void set(unsigned short word);
		void swap();
		char getByte1();
		char getByte2();
	private:
		unsigned char byte1;
		unsigned char byte2;
	};

	class DWORD
	{
	public:
		DWORD();

		unsigned long get() const;
		void set(unsigned long dword);
		void swap();
		char getByte1();
		char getByte2();
		char getByte3();
		char getByte4();
	private:
		unsigned char byte1;
		unsigned char byte2;
		unsigned char byte3;
		unsigned char byte4;
	};

	class RGBQUAD
	{
	public:
		RGBQUAD();

		void print();

	private:
		unsigned char rgbBlue;
		unsigned char rgbGreen;
		unsigned char rgbRed;
		unsigned char rgbReserved;
	};

	class BITMAPFILEHEADER
	{
	public:
		void print();
		void swap();

		DWORD getBfOffBits();
		DWORD getBfSize();

	private:
		WORD bfType;
		DWORD bfSize;
		WORD bfReserved1;
		WORD bfReserved2;
		DWORD bfOffBits;
	};

	class BITMAPINFOHEADER
	{
	public:
		void print();
		void swap();

		DWORD getBiSize();
		DWORD getBiSizeImage();
		DWORD getBiWidth();
		DWORD getBiHeight();

	private:
		DWORD biSize;
		DWORD biWidth;
		DWORD biHeight;
		WORD biPlanes;
		WORD biBitCount;
		DWORD biCompression;
		DWORD biSizeImage;
		DWORD biXPelsPerMeter;
		DWORD biYPelsPerMeter;
		DWORD biClrUsed;
		DWORD biClrImportant;
	};
}

class DIB
{
public:
	DIB();
	~DIB();

	void loadFile(std::string filename);
	void printColorIndexDump();
	unsigned long getWidth();
	unsigned long getHeight();

	unsigned char *getColorIndex();

private:
	DIBLIB::BITMAPFILEHEADER bmfHeader;
	DIBLIB::BITMAPINFOHEADER bmiHeader;
	DIBLIB::RGBQUAD *bmiColors;
	unsigned char *colorIndex;
};

#endif