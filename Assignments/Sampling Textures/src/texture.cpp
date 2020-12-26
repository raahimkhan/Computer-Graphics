#include "texture.h"
#include "CGL/color.h"
#include "drawrend.h"
#include <math.h>

namespace CGL {

Color Texture::sample(const SampleParams &sp) {
  // Part 5: this funtion chooses between nearest pixel sampling and bilinear pixel interpolation sampling 
  if (sp.psm == P_NEAREST) {
    return sample_nearest(sp.p_uv);
  }
  else {
    return sample_bilinear(sp.p_uv);
  }
}
Color Texture::sample_nearest(Vector2D uv) {
  // Part 5: Understand how this funtion has been implemented.
  // keep value of level to 0
  int level = 0;
  int w = mipmap[level].width;
  int h = mipmap[level].height;

  int X = uv.x * w;
  int Y = uv.y * h;
  if (X >= 0 && X < w && Y >= 0 && Y < h) {
    int index = 4 * (Y * w + X);

    float r = (mipmap[level].texels[index]) / 255.0f;
    float g = (mipmap[level].texels[index+1]) / 255.0f;
    float b = (mipmap[level].texels[index+2]) / 255.0f;
    return Color(r, g, b);
  }

  return Color();
}

Color Texture::sample_bilinear(Vector2D uv) {
  // Part 5: Fill this in.
  // Hint: look at sample_nearest

  int level = 0;
  int w = mipmap[level].width;
  int h = mipmap[level].height;

  // ignore the values that are commented i.e 14.5 20.2 I wrote them for my help in calculations they are dummy values

  float originalX = uv.x * w ; // 14.5
  float originalY = uv.y * h ; // 20.2


  // first point (top left)
  int x = floor(originalX) ; //14
  int y = floor(originalY) ; //20
  float r1 , g1 , b1 ;

  if (x >= 0 && x < w && y >= 0 && y < h) {
    int index = 4 * (y * w + x);

    r1 = (mipmap[level].texels[index]) / 255.0f;
    g1 = (mipmap[level].texels[index+1]) / 255.0f;
    b1 = (mipmap[level].texels[index+2]) / 255.0f;
  }

  // second point (top right)
  int x2 = ceil(originalX) ; //15
  // y will remain same i.e 20
  float r2 , g2 , b2 ;

  if (x2 >= 0 && x2 < w && y >= 0 && y < h) {
    int index = 4 * (y * w + x2);

    r2 = (mipmap[level].texels[index]) / 255.0f;
    g2 = (mipmap[level].texels[index+1]) / 255.0f;
    b2 = (mipmap[level].texels[index+2]) / 255.0f;
  }

  // third point (bottom left)
  // x will remain same i.e 14
  int y2 = ceil(originalY) ; // 21
  float r3 , g3 , b3 ;

  if (x >= 0 && x < w && y2 >= 0 && y2 < h) {
    int index = 4 * (y2 * w + x);

    r3 = (mipmap[level].texels[index]) / 255.0f;
    g3 = (mipmap[level].texels[index+1]) / 255.0f;
    b3 = (mipmap[level].texels[index+2]) / 255.0f;
  }

  // fourth point (top right)
  // x2 will remain same
  // y2 will remain same
  float r4 , g4 , b4 ;

  if (x2 >= 0 && x2 < w && y2 >= 0 && y2 < h) {
    int index = 4 * (y2 * w + x2);

    r4 = (mipmap[level].texels[index]) / 255.0f;
    g4 = (mipmap[level].texels[index+1]) / 255.0f;
    b4 = (mipmap[level].texels[index+2]) / 255.0f;
  }


  float topc1 = ( ((x2 - originalX) / (x2 - x)) * r1 ) + ( ((originalX - x) / (x2 - x)) * r2 ) ;
  float topc2 = ( ((x2 - originalX) / (x2 - x)) * g1 ) + ( ((originalX - x) / (x2 - x)) * g2 ) ;
  float topc3 = ( ((x2 - originalX) / (x2 - x)) * b1 ) + ( ((originalX - x) / (x2 - x)) * b2 ) ;

  float bottomc1 = ( ((x2 - originalX) / (x2 - x)) * r3 ) + ( ((originalX - x) / (x2 - x)) * r4 ) ;
  float bottomc2 = ( ((x2 - originalX) / (x2 - x)) * g3 ) + ( ((originalX - x) / (x2 - x)) * g4 ) ;
  float bottomc3 = ( ((x2 - originalX) / (x2 - x)) * b3 ) + ( ((originalX - x) / (x2 - x)) * b4 ) ;

  float finalc1 = ( ((y2 - originalY) / (y2 - y)) * topc1 ) + ( ((originalY - y) / (y2 - y)) * bottomc1 ) ;
  float finalc2 = ( ((y2 - originalY) / (y2 - y)) * topc2 ) + ( ((originalY - y) / (y2 - y)) * bottomc2 ) ;
  float finalc3 = ( ((y2 - originalY) / (y2 - y)) * topc3 ) + ( ((originalY - y) / (y2 - y)) * bottomc3 ) ;

  Color final_interpolated = Color(finalc1, finalc2, finalc3) ;

  return final_interpolated ;
}


/****************************************************************************/



inline void uint8_to_float(float dst[4], unsigned char *src) {
  uint8_t *src_uint8 = (uint8_t *)src;
  dst[0] = src_uint8[0] / 255.f;
  dst[1] = src_uint8[1] / 255.f;
  dst[2] = src_uint8[2] / 255.f;
  dst[3] = src_uint8[3] / 255.f;
}

inline void float_to_uint8(unsigned char *dst, float src[4]) {
  uint8_t *dst_uint8 = (uint8_t *)dst;
  dst_uint8[0] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[0])));
  dst_uint8[1] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[1])));
  dst_uint8[2] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[2])));
  dst_uint8[3] = (uint8_t)(255.f * max(0.0f, min(1.0f, src[3])));
}

void Texture::generate_mips(int startLevel) {

  // make sure there's a valid texture
  if (startLevel >= mipmap.size()) {
    std::cerr << "Invalid start level";
  }

  // allocate sublevels
  int baseWidth = mipmap[startLevel].width;
  int baseHeight = mipmap[startLevel].height;
  int numSubLevels = (int)(log2f((float)max(baseWidth, baseHeight)));

  numSubLevels = min(numSubLevels, kMaxMipLevels - startLevel - 1);
  mipmap.resize(startLevel + numSubLevels + 1);

  int width = baseWidth;
  int height = baseHeight;
  for (int i = 1; i <= numSubLevels; i++) {

    MipLevel &level = mipmap[startLevel + i];

    // handle odd size texture by rounding down
    width = max(1, width / 2);
    //assert (width > 0);
    height = max(1, height / 2);
    //assert (height > 0);

    level.width = width;
    level.height = height;
    level.texels = vector<unsigned char>(4 * width * height);
  }

  // create mips
  int subLevels = numSubLevels - (startLevel + 1);
  for (int mipLevel = startLevel + 1; mipLevel < startLevel + subLevels + 1;
       mipLevel++) {

    MipLevel &prevLevel = mipmap[mipLevel - 1];
    MipLevel &currLevel = mipmap[mipLevel];

    int prevLevelPitch = prevLevel.width * 4; // 32 bit RGBA
    int currLevelPitch = currLevel.width * 4; // 32 bit RGBA

    unsigned char *prevLevelMem;
    unsigned char *currLevelMem;

    currLevelMem = (unsigned char *)&currLevel.texels[0];
    prevLevelMem = (unsigned char *)&prevLevel.texels[0];

    float wDecimal, wNorm, wWeight[3];
    int wSupport;
    float hDecimal, hNorm, hWeight[3];
    int hSupport;

    float result[4];
    float input[4];

    // conditional differentiates no rounding case from round down case
    if (prevLevel.width & 1) {
      wSupport = 3;
      wDecimal = 1.0f / (float)currLevel.width;
    } else {
      wSupport = 2;
      wDecimal = 0.0f;
    }

    // conditional differentiates no rounding case from round down case
    if (prevLevel.height & 1) {
      hSupport = 3;
      hDecimal = 1.0f / (float)currLevel.height;
    } else {
      hSupport = 2;
      hDecimal = 0.0f;
    }

    wNorm = 1.0f / (2.0f + wDecimal);
    hNorm = 1.0f / (2.0f + hDecimal);

    // case 1: reduction only in horizontal size (vertical size is 1)
    if (currLevel.height == prevLevel.height) {
      //assert (currLevel.height == 1);

      for (int i = 0; i < currLevel.width; i++) {
        wWeight[0] = wNorm * (1.0f - wDecimal * i);
        wWeight[1] = wNorm * 1.0f;
        wWeight[2] = wNorm * wDecimal * (i + 1);

        result[0] = result[1] = result[2] = result[3] = 0.0f;

        for (int ii = 0; ii < wSupport; ii++) {
          uint8_to_float(input, prevLevelMem + 4 * (2 * i + ii));
          result[0] += wWeight[ii] * input[0];
          result[1] += wWeight[ii] * input[1];
          result[2] += wWeight[ii] * input[2];
          result[3] += wWeight[ii] * input[3];
        }

        // convert back to format of the texture
        float_to_uint8(currLevelMem + (4 * i), result);
      }

      // case 2: reduction only in vertical size (horizontal size is 1)
    } else if (currLevel.width == prevLevel.width) {
      //assert (currLevel.width == 1);

      for (int j = 0; j < currLevel.height; j++) {
        hWeight[0] = hNorm * (1.0f - hDecimal * j);
        hWeight[1] = hNorm;
        hWeight[2] = hNorm * hDecimal * (j + 1);

        result[0] = result[1] = result[2] = result[3] = 0.0f;
        for (int jj = 0; jj < hSupport; jj++) {
          uint8_to_float(input, prevLevelMem + prevLevelPitch * (2 * j + jj));
          result[0] += hWeight[jj] * input[0];
          result[1] += hWeight[jj] * input[1];
          result[2] += hWeight[jj] * input[2];
          result[3] += hWeight[jj] * input[3];
        }

        // convert back to format of the texture
        float_to_uint8(currLevelMem + (currLevelPitch * j), result);
      }

      // case 3: reduction in both horizontal and vertical size
    } else {

      for (int j = 0; j < currLevel.height; j++) {
        hWeight[0] = hNorm * (1.0f - hDecimal * j);
        hWeight[1] = hNorm;
        hWeight[2] = hNorm * hDecimal * (j + 1);

        for (int i = 0; i < currLevel.width; i++) {
          wWeight[0] = wNorm * (1.0f - wDecimal * i);
          wWeight[1] = wNorm * 1.0f;
          wWeight[2] = wNorm * wDecimal * (i + 1);

          result[0] = result[1] = result[2] = result[3] = 0.0f;

          // convolve source image with a trapezoidal filter.
          // in the case of no rounding this is just a box filter of width 2.
          // in the general case, the support region is 3x3.
          for (int jj = 0; jj < hSupport; jj++)
            for (int ii = 0; ii < wSupport; ii++) {
              float weight = hWeight[jj] * wWeight[ii];
              uint8_to_float(input, prevLevelMem +
                                        prevLevelPitch * (2 * j + jj) +
                                        4 * (2 * i + ii));
              result[0] += weight * input[0];
              result[1] += weight * input[1];
              result[2] += weight * input[2];
              result[3] += weight * input[3];
            }

          // convert back to format of the texture
          float_to_uint8(currLevelMem + currLevelPitch * j + 4 * i, result);
        }
      }
    }
  }
}

}
