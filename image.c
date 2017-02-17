#include <atlimage.h>

/*
[input]
Image: source image,
szNew: size of new image, 
nRotate: 0=no rotate, 1=rotate 90 left, 2=rotate 90 right, 3=rotate 180 upside down
nFlip: 0=no flip, 1=horizontal, 2=vertical
bNegative: true=film like negative image
[return]
pointer to new cimage
*/
CImage* EditImage(CImage& Image, CSize szNew, int nRotate, int nFlip, bool bNegative){

	//get the scale rate
	float fX, fY;
	if (nRotate == 1 || nRotate == 2){
		fX = szNew.cy / (float)Image.GetWidth();
		fY = szNew.cx / (float)Image.GetHeight();
	}
	else{
		fX = szNew.cx / (float)Image.GetWidth();
		fY = szNew.cy / (float)Image.GetHeight();
	}	

	//create new cimage
	CImage* pImage = new CImage;
	pImage->Create(szNew.cx, szNew.cy, 24);

	for (int y = 0; y < szNew.cy; y++){		
		int y0 = y;
		if (nFlip == 2)
			y0 = szNew.cy - y - 1;
		int y1 = y0;
		for (int x = 0; x < szNew.cx; x++){
			int x0 = x;
			if (nFlip == 1)
				x0 = szNew.cx - x - 1;
			int x1 = x0;
			if (nRotate == 1){
				x1 = szNew.cy - y0 - 1;
				y1 = x0;
			}
			else if (nRotate == 2){
				x1 = y0;
				y1 = szNew.cx - x0 - 1;
			}
			else if (nRotate == 3){
				x1 = szNew.cx - x - 1;
				y1 = szNew.cy - y - 1;
				if (nFlip == 1)
					x1 = x;
				else if (nFlip == 2)
					y1 = y;
			}
			
			//get the source image's pixel value
			int xs = x1 / fX;
			int ys = y1 / fY;
			byte *ps = (byte*)Image.GetPixelAddress(xs, ys);
			
			//nearest neighbour, or biliner for better result
			byte *p = (byte*)pImage->GetPixelAddress(x, y);
			if (bNegative){
				*p = 255 - *ps;
				*(p + 1) = 255 - *(ps + 1);
				*(p + 2) = 255 - *(ps + 2);
			}
			else{
				*p = *ps;
				*(p + 1) = *(ps + 1);
				*(p + 2) = *(ps + 2);
			}
		}
	}
	return pImage;
}

void Test(){
  	CImage image;
  	image.Load(L"myimage.png");
  	CImage* pNewImage=EditImage(image,CSize(800,600),3,0,false);
  	pNewImage->Save(L"newimage.jpg",Gdiplus::ImageFormatJPEG);  //Gdiplus::ImageFormatPNG Gdiplus::ImageFormatBMP...JPEG GIF
  	pNewImage->Destroy();
  	delete pNewImage;
}
