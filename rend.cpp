#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"


GzRender::GzRender(int xRes, int yRes)
{
/* HW1.1 create a framebuffer for MS Windows display:
 -- set display resolution
 -- allocate memory for framebuffer : 3 bytes(b, g, r) x width x height
 -- allocate memory for pixel buffer
 */
	xres = xRes;
	yres = yRes;

	pixelbuffer = new GzPixel[xres*yres];
	framebuffer = new char[3*xres*yres];
}

GzRender::~GzRender()
{
/* HW1.2 clean up, free buffer memory */
	delete []pixelbuffer;
	delete []framebuffer;
}

int GzRender::GzDefault()
{
/* HW1.3 set pixel buffer to some default values - start a new frame */
	for (int y = 0; y < yres; y++) {
		for (int x = 0; x < xres; x++) {
			GzPixel pixel = { 4095, 4095, 4095, 1, 0 };
			pixelbuffer[ARRAY(x, y)] = pixel;
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzPut(int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{
/* HW1.4 write pixel values into the buffer */
	// Bound check
	if (i < 0 || xres - 1 < i || j < 0 || yres - 1 < j) return GZ_SUCCESS;

	// Intensity value check
	if (r < 0) r = 0;
	else if (4095 < r) r = 4095;

	if (g < 0) g = 0;
	else if (4095 < g) g = 4095;

	if (b < 0) b = 0;
	else if (4095 < b) b = 4095;

	// Right shit (12bits to 8bits)
	pixelbuffer[ARRAY(i, j)] = { r >> 4, g >> 4, b >> 4, a, z };

	return GZ_SUCCESS;
}

int GzRender::GzGet(int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
/* HW1.5 retrieve a pixel information from the pixel buffer */
	// Bound check
	if (i < 0 || xres - 1 < i || j < 0 || yres - 1 < j) return GZ_SUCCESS;
	
	*r = pixelbuffer[ARRAY(i, j)].red;
	*g = pixelbuffer[ARRAY(i, j)].green;
	*b = pixelbuffer[ARRAY(i, j)].blue;
	*a = pixelbuffer[ARRAY(i, j)].alpha;
	*z = pixelbuffer[ARRAY(i, j)].z;

	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2File(FILE* outfile)
{
/* HW1.6 write image to ppm file -- "P6 %d %d 255\n" */
	fprintf(outfile, "P6 %d %d 255\n", xres, yres);

	GzIntensity r = 0;
	GzIntensity g = 0;
	GzIntensity b = 0;
	GzIntensity a = 0;
	GzDepth z = 0;

	for (int y = 0; y < yres; y++) {
		for (int x = 0; x < xres; x++) {
			GzGet(x, y, &r, &g, &b, &a, &z);
			fprintf(outfile, "%c%c%c", (unsigned char) r, (unsigned char)g, (unsigned char)b);
		}
	}

	return GZ_SUCCESS;
}

int GzRender::GzFlushDisplay2FrameBuffer()
{
/* HW1.7 write pixels to framebuffer: 
	- put the pixels into the frame buffer
	- CAUTION: when storing the pixels into the frame buffer, the order is blue, green, and red 
	- NOT red, green, and blue !!!
*/
	GzIntensity r = 0;
	GzIntensity g = 0;
	GzIntensity b = 0;
	GzIntensity a = 0;
	GzDepth z = 0;

	int i = 0;
	for (int y = 0; y < yres; y++) {
		for (int x = 0; x < xres; x++) {
			GzGet(x, y, &r, &g, &b, &a, &z);
			framebuffer[i++] = (unsigned char) b;
			framebuffer[i++] = (unsigned char) g;
			framebuffer[i++] = (unsigned char) r;
		}
	}

	return GZ_SUCCESS;
}
