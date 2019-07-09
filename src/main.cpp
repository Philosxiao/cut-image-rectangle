// OpenCv
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/aruco.hpp>

#include <vector>
#include <stdio.h>

#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

cv::Mat source_image,dst,img,tmp;

//event是鼠标点击事件，表示鼠标在进行什么操作
void on_mouse(int event,int x,int y,int flags,void *ustc)//event鼠标事件代号，x,y鼠标坐标，flags拖拽和键盘操作的代号
{
	static Point pre_pt(-1,-1);//初始坐标
	static Point cur_pt(-1,-1);//实时坐标
	char temp[16];
	if (event == CV_EVENT_LBUTTONDOWN && CV_EVENT_FLAG_CTRLKEY)//按住ctrl健，鼠标左键按下，读取初始坐标，并在图像上该点处划圆
	{
		source_image.copyTo(img);//将原始图片复制到img中
		sprintf(temp,"(%d,%d)",x,y);
		pre_pt = Point(x,y);
		putText(img,temp,pre_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),1,8);//在窗口上显示坐标
		circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);//划圆
		imshow("img",img);
	}
    else if (event == CV_EVENT_LBUTTONDOWN)//不按住ctrl健，鼠标左键按下，读取初始坐标，并在图像上该点处划圆
	{
		source_image.copyTo(img);//将原始图片复制到img中
		sprintf(temp,"(%d,%d)",x,y);
		putText(img,temp,pre_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255),1,8);//在窗口上显示坐标
		circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);//划圆
		imshow("img",img);
	}
	else if (event == CV_EVENT_MOUSEMOVE && !(flags & CV_EVENT_FLAG_LBUTTON))//左键没有按下的情况下鼠标移动的处理函数
	{
		img.copyTo(tmp);//将img复制到临时图像tmp上，用于显示实时坐标
		sprintf(temp,"(%d,%d)",x,y);
		cur_pt = Point(x,y);
		putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));//只是实时显示鼠标移动的坐标
		imshow("img",tmp);
	}
    else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON) && CV_EVENT_FLAG_CTRLKEY)//按住ctrl健，鼠标左键按下时，鼠标移动，则在图像上划矩形
	{
		img.copyTo(tmp);
		sprintf(temp,"(%d,%d)",x,y);
		cur_pt = Point(x,y);
		putText(tmp,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
		rectangle(tmp,pre_pt,cur_pt,Scalar(0,255,0,0),1,8,0);//在临时图像上实时显示鼠标拖动时形成的矩形
		imshow("img",tmp);
	}
	else if (event == CV_EVENT_LBUTTONUP  && CV_EVENT_FLAG_CTRLKEY)//按住ctrl健，鼠标左键松开，将在图像上划矩形
	{
		source_image.copyTo(img);
		sprintf(temp,"(%d,%d)",x,y);
		cur_pt = Point(x,y);
		putText(img,temp,cur_pt,FONT_HERSHEY_SIMPLEX,0.5,Scalar(0,0,0,255));
		circle(img,pre_pt,2,Scalar(255,0,0,0),CV_FILLED,CV_AA,0);
		rectangle(img,pre_pt,cur_pt,Scalar(0,255,0,0),1,8,0);//根据初始点和结束点，将矩形画到img上
		imshow("img",img);
		img.copyTo(tmp);
		//截取矩形包围的图像，并保存到dst中
		int width = abs(pre_pt.x - cur_pt.x);
		int height = abs(pre_pt.y - cur_pt.y);
		if (width == 0 || height == 0)
		{
			printf("width == 0 || height == 0");
			return;
		}
		dst = source_image(Rect(min(cur_pt.x,pre_pt.x),min(cur_pt.y,pre_pt.y),width,height));
		imwrite("../dst_cut.jpg",dst);//保存到本地路径下
	}
}

int main(int argc, char *argv[])
{
    cv::namedWindow("img", WINDOW_AUTOSIZE);
    while (1)
    {
        source_image=imread("../test.jpg", -1);
        source_image.copyTo(img);
        source_image.copyTo(tmp);
	    setMouseCallback("img",on_mouse,0);//调用回调函数
        imshow("img", source_image);
	    cv::waitKey(0);
    }
}
