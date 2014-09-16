#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;


/// Global variables
Mat src, src_gray, dst;



class bg :  public BackgroundSubtractorMOG2
{

public:

	void setnmixtures()
	{
		this->backgroundRatio = 0.9;
		this->bShadowDetection = true;
		this->fVarInit = 50;
		this->fVarMin = 10;
		this->fVarMax = 100;
		this->fCT = 0.05;
	}

};

int videoProcess(string dir, string video)
{
	VideoCapture cap;
//	cap.open("E:/SL Dataset/process/" + video); //almost all perform well
	cap.open(dir + video);


//	cap.open("C:/Users/vk2382/Dropbox/SL Identification Project/videosbatch/2FzYNk3fs_A.mp4");
	if( !cap.isOpened() )
	{
		puts("***Could not initialize capturing...***\n");
		return 0;
	}
//	namedWindow( "Capture ", CV_WINDOW_AUTOSIZE);

	Mat frame,foreground,image, initial;

	bg back;
//	back.setnmixtures();
	BackgroundSubtractorMOG2 mog = back;

	int fps=cap.get(CV_CAP_PROP_FPS);
	cout<<fps<<endl;

	int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));
	char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};
	Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH),(int) cap.get(CV_CAP_PROP_FRAME_HEIGHT));
	cout<<EXT<<endl;

	cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height << " of nr#: " << cap.get(CV_CAP_PROP_FRAME_COUNT) << endl;
	cout << "Input codec type: " << EXT << endl;
	int format = CV_FOURCC('M','J','P','G');
	cout<<" format : "<<format;
//	VideoWriter outputVideo("E:/SL Dataset/process/" + video.substr(0,video.find(".")) + "_foregnd.avi", CV_FOURCC('M','S','V','C'), 30, frame.size(), true);
	VideoWriter outputVideo(dir + video.substr(0,video.find(".")) + "_foregnd.avi", CV_FOURCC('M','S','V','C'), 30, frame.size(), true);
	if (!outputVideo.isOpened())
	 {
	        cout  << "Could not open the output video for write: " << endl;
	        return -1;
	 }

	if(fps<=0)
		fps=10;
	else
		fps=1000/fps;

	int count =0;
	for(;;)
	{
		cap>>frame;   // Bir frame alýyoruz
		image=frame.clone();
		count = count + 1;
		// Arka plan çýkarma kýsmý



		if( frame.empty() )
				break;

		mog(frame,foreground,0.01);

		// Ufak tefek temizlik

		count = count + 1;
		if(count == 3600) break;


	//	imshow("mog ",foreground);
		threshold(foreground,foreground,25,255,THRESH_BINARY);

	//	imshow("Thresholded ",foreground);
		medianBlur(foreground,foreground,0.01);
	//	imshow("blurred ",foreground);
		erode(foreground,foreground,Mat());
	//	imshow("Eroded ",foreground);
		dilate(foreground,foreground,Mat());
		outputVideo << foreground;


	//	imshow( "Capture ",image );
		// image.copyTo(foreground,foreground); // birde böyle deneyin bakalým ne olacak
//		imshow("Dilated ",foreground);



		/*char c = (char)waitKey(fps);
		if( c == 27 )   // ESC tuþuna basýlana kadar çalýþ
			break;*/

	}
}


int main(int arc, char * argv[])
{
	cout<<argv[1]<<endl;
	cout<<argv[2]<<endl;
	videoProcess(argv[1], argv[2]);
}
