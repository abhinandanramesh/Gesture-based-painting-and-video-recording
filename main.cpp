#define _WIN32_WINNT 0x500
#include <cv.h>
#include <highgui.h>
//#include <windows.h>
#include <iostream>
//#include "stdafx.h"
//#include <iostream>
#include "windows.h"
#include "winuser.h"
#include "wingdi.h"
using namespace std;
void MouseMove (int x, int y )
{
  double fScreenWidth    = ::GetSystemMetrics( SM_CXSCREEN )-1;
  double fScreenHeight  = ::GetSystemMetrics( SM_CYSCREEN )-1;
  double fx = x*(100535.0f/fScreenWidth);
  double fy = y*(100535.0f/fScreenHeight);
  INPUT  Input={0};
  Input.type      = INPUT_MOUSE;
  Input.mi.dwFlags  = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
  Input.mi.dx = fx;
  Input.mi.dy = fy;
  ::SendInput(1,&Input,sizeof(INPUT));
}
int main()
{
	CvCapture* cap=cvCreateCameraCapture(0);
	//cvSetCaptureProperty(cap,CV_CAP_PROP_FRAME_WIDTH,320);
	//cvSetCaptureProperty(cap,CV_CAP_PROP_FRAME_HEIGHT,240);
	//int timer=0;
	int checking=0;
	Sleep(5000);

	IplImage* menu=cvLoadImage("F:/Study/embedded systems/open cv/serious/painting final/painting final/main menu.jpg",1);
	int red=0,green=0,blue=0;
	int pointer;

		while(1)
		{
			IplImage* colour=cvQueryFrame(cap);
			IplImage* colour_thresh=cvCreateImage(cvGetSize(colour),8,1);
			CvScalar colour_x,colour_y;
			CvRect square=cvRect(0,0,0,0);
			CvPoint point1,point2;
			IplImage* img_hsv=cvCreateImage(cvGetSize(colour),8,3);
			cvCvtColor(colour,img_hsv,CV_BGR2HSV);
		//IplImage* colour_thresh=cvCreateImage(cvGetSize(img_hsv),8,1);
			cvInRangeS(img_hsv,cvScalar(0,60,50),cvScalar(30,300,250),colour_thresh);
			cvErode(colour_thresh,colour_thresh,0,3);
			cvThreshold(colour_thresh,colour_thresh,1,255,CV_THRESH_BINARY);
		//cvNot(colour_thresh,colour_thresh);
			cvSmooth(colour_thresh,colour_thresh,CV_MEDIAN,1);
			cvDilate(colour_thresh,colour_thresh,0,3);
			cvErode(colour_thresh,colour_thresh,0,25);
			CvMemStorage* menu_storage=cvCreateMemStorage(0);
			CvSeq* menu_contours=0;
			
			int r=cvFindContours(colour_thresh,menu_storage,&menu_contours,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
			printf("contours =%d\n\n",r);
			
				if(r!=1)
				{
				 	menu=cvLoadImage("F:/Study/embedded systems/open cv/serious/painting final/painting final/main menu.jpg",1);
					goto menu_tile;
				}
				else
				{
					square=cvBoundingRect(menu_contours,0);
					point1.x=square.x;
					point1.y=square.y;
					point2.x=square.x+square.width;
					point2.y=square.y+square.height;
					MouseMove((point1.x+point2.x)/2,(point1.y+point2.y)/2);
					pointer=(point1.x+point2.x)/2;
						
					if(pointer<=280)
					{
						menu=cvLoadImage("F:/Study/embedded systems/open cv/serious/painting final/painting final/red.jpg",1);
						cvShowImage("menu",menu);
						red++;
						green=0;
						blue=0;
						
						if(red==35)
						{
							IplImage* scribble=0;
							int tractpt=0;
							static int curr_x=0;
							static int curr_y=0;
							CvPoint pt1,pt2;
							CvRect rect=cvRect(0,0,0,0);
						
							while(1)
							{
								int count=0;
								int flag=0;
								int det=0;
								IplImage* img=cvQueryFrame(cap);
								IplImage* img_thresh=cvCreateImage(cvGetSize(img),8,1);
								int i,j;
								CvScalar x,y;
								IplImage* img_hsv=cvCreateImage(cvGetSize(img),8,3);
								cvCvtColor(img,img_hsv,CV_BGR2HSV);
				//IplImage* colour_thresh=cvCreateImage(cvGetSize(img_hsv),8,1);
								cvInRangeS(img_hsv,cvScalar(0,60,50),cvScalar(30,300,250),img_thresh);
								cvErode(img_thresh,img_thresh,0,3);
								cvThreshold(img_thresh,img_thresh,1,255,CV_THRESH_BINARY);
								cvNot(img_thresh,img_thresh);
								cvSmooth(img_thresh,img_thresh,CV_MEDIAN,1);
								cvDilate(img_thresh,img_thresh,0,10);
								cvErode(img_thresh,img_thresh,0,25);
								CvMemStorage* storage=cvCreateMemStorage(0);
								CvSeq* contours=0;
								CvSeq* paint=0;
									
								int r=cvFindContours(img_thresh,storage,&contours,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
								cout<<r<<endl;
								IplImage* check_thresh=cvCreateImage(cvGetSize(colour),8,1);
								CvScalar check_x,check_y;
									
								for(int i=0;i<colour->height;i++)
								{
									for(int j=0;j<colour->width;j++)
									{
										check_x=cvGet2D(colour,i,j);
									
										if( check_x.val[0]<45 && check_x.val[0]>25  && check_x.val[1]<25  && check_x.val[2]<15 )
										{
											check_y.val[0]=1;
											cvSet2D(check_thresh,i,j,check_y);
										}
									}
								}
								
								cvThreshold(check_thresh,check_thresh,1,255,CV_THRESH_BINARY);
								cvNot(check_thresh,check_thresh);
								cvSmooth(check_thresh,check_thresh,CV_MEDIAN,1);
								cvDilate(check_thresh,check_thresh,0,2);
								cvErode(check_thresh,check_thresh,0,7);
								cvDilate(check_thresh,check_thresh,0,20);
								CvMemStorage* check_storage=cvCreateMemStorage(0);
								CvSeq* boss1=0;
								
								int checklist=cvFindContours(check_thresh,check_storage,&boss1,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
								cout<<checklist<<endl;
								
								if(checklist==1)
								{
								checking++;
			
								if(checking==20)
								{
									checking=0;
									cvDestroyWindow("paint");
									
									if(scribble!=0)
										cvDestroyWindow("Painting Window");
										goto menu_tile;
								}
							}
							else if(checklist!=1)
							{
								checking=0;
						
								if(contours==0)
									goto show;

								for(CvSeq* c=contours;c!=0;c=c->h_next,count++)
								{
									rect=cvBoundingRect(c,0);
									pt1.x=rect.x;
									pt1.y=rect.y;
									pt2.x=rect.x+rect.width;
									pt2.y=rect.y+rect.height;
									tractpt=(pt1.x+pt2.x)/2;
								
									if(tractpt>550 && tractpt<615 )
									{
										flag=1;
										det=count;
									//cout<<det<<endl;
										break;
									}
									else
									{
										MouseMove(tractpt,(pt1.y+pt2.y)/2);
									}
									
									if(det==1 || det==0)
									{
										curr_x=tractpt;
										curr_y=(pt1.y+pt2.y)/2;
									}
								}
									
								if(flag==1)
								{
									for(CvSeq* h=contours;h!=0;h=h->h_next)
									{
										rect=cvBoundingRect(h,0);
										pt1.x=rect.x;
										pt1.y=rect.y;
										pt2.x=rect.x+rect.width;
										pt2.y=rect.y+rect.height;
										cvRectangle(img,pt1,pt2,CV_RGB(0,255,0),11);
									}
							
									if(det==0 && r==2)
									{
										paint=contours;
										paint=paint->h_next;
									}
									else if(det==1 && r==2)
									{
										paint=contours;
									}
									
									if(paint!=0)
									{
										if(scribble==0)
										{
											scribble=cvCreateImage(cvGetSize(img),8,1);
											cvNamedWindow("Painting Window",CV_WINDOW_AUTOSIZE);
										}
									
										rect=cvBoundingRect(paint,0);
										pt1.x=rect.x;
										pt1.y=rect.y;
										pt2.x=rect.x+rect.width;
										pt2.y=rect.y+rect.height;
										int last_x=curr_x;
										int last_y=curr_y;
										curr_x=(pt1.x+pt2.x)/2;
										curr_y=(pt1.y+pt2.y)/2;
										MouseMove(curr_x,curr_y);
										cvLine(scribble,cvPoint(curr_x,curr_y),cvPoint(last_x,last_y),CV_RGB(0,0,0),11);
									}
								}
								show:	cvNamedWindow("paint",CV_WINDOW_AUTOSIZE);
									cvShowImage("paint",img);
									cvShowImage("Painting Window",scribble);
									cvReleaseMemStorage(&storage);
									char c=cvWaitKey(3);
									if(c==27)
										break;

							}


						}

						cout<<"red"<<endl;
					}
				}
				else if(pointer>280 && pointer<=590)
				{
					menu=cvLoadImage("F:/Study/embedded systems/open cv/serious/painting final/painting final/green.jpg",1);
					cvShowImage("menu",menu);
					green++;
					red=0;
					blue=0;
							
					if(green==35)
					{
						CvRect rect=cvRect(0,0,0,0);
						int start_count=0;
						CvPoint pt1,pt2;
						int num=0;
						
						while(1)
						{
							int counter=0;
						//int count=0;
							int min=0;
							int max=0;
							IplImage* click=cvQueryFrame(cap);
							IplImage* dest=cvCreateImage(cvGetSize(click),8,3);
							IplImage* thresh=cvCreateImage(cvGetSize(click),8,1);
							CvScalar x,y;
										
							for(int i=0;i<click->height;i++)
							{
								for(int j=0;j<click->width;j++)
								{
									x=cvGet2D(click,i,j);
										
									if(x.val[0]<40  && x.val[1]<120 && x.val[1]>20 && x.val[2]<200 && x.val[2]>80)
									{
										y.val[0]=1;
										cvSet2D(thresh,i,j,y);
									}
								}
							}
							cvThreshold(thresh,thresh,1,255,CV_THRESH_BINARY);
							cvNot(thresh,thresh);
							cvSmooth(thresh,thresh,CV_MEDIAN,1);
							cvDilate(thresh,thresh,0,8);
							cvErode(thresh,thresh,0,20);
							CvMemStorage* storage=cvCreateMemStorage(0);
							CvSeq* contour=0;
							int count=cvFindContours(thresh,storage,&contour,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
										
							if(count!=2)
								goto graph;
							else if(count==2)
							{
								for(CvSeq* c=contour;c!=0;c=c->h_next,counter++)
								{
									rect=cvBoundingRect(c,0);
									pt1.x=rect.x;
									pt1.y=rect.y;
									pt2.y=rect.y+rect.height;
									pt2.x=rect.x+rect.width;
									cvRectangle(click,pt1,pt2,CV_RGB(0,0,255),11);
									
									if(counter==0)
									{
										min=pt1.x;
										max=pt2.x;
									}
										
									if(counter==1)
									{
										if(pt1.x<=max)
										{
											max=min;
											min=pt2.x;
										}
									else if(max<=pt1.x)
									{
										min=max;
										max=pt1.x;
									}
								}

							}
						}
						IplImage* snap_thresh;
						snap_thresh=cvCreateImage(cvGetSize(click),8,1);
						int length,breadth;
						CvScalar snap_x,snap_y;
						for( length=0;length<click->height;length++)
						{
							for( breadth=0;breadth<click->width;breadth++)
							{
								snap_x=cvGet2D(click,length,breadth);
								
								if( snap_x.val[0]<45 && snap_x.val[0]>25  && snap_x.val[1]<25  && snap_x.val[2]<15 )
								{
									snap_y.val[0]=1;
									cvSet2D(snap_thresh,length,breadth,snap_y);
								}
													}
							}
						cvThreshold(snap_thresh,snap_thresh,1,255,CV_THRESH_BINARY);
						cvNot(snap_thresh,snap_thresh);
						cvSmooth(snap_thresh,snap_thresh,CV_MEDIAN,1);
						cvDilate(snap_thresh,snap_thresh,0,2);
						cvErode(snap_thresh,snap_thresh,0,7);
						cvDilate(snap_thresh,snap_thresh,0,20);
						CvMemStorage* snap_storage;
						snap_storage=cvCreateMemStorage(0);
						CvSeq* snaps;
						snaps=0;
						cvSetImageROI(click,cvRect(min,min,abs(max-min),abs(max-min)));
						cvResize(click,dest,CV_INTER_LINEAR);

						int Blue;
						Blue=cvFindContours(snap_thresh,snap_storage,&snaps,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
						
						if(Blue==1)
						{
							start_count++;
							if(start_count==1)
							{
								start_count=0;
								char filename[80];
								num++;
								sprintf(filename,"F:/Study/embedded systems/open cv/serious/painting final/Directory/photo%u.jpg",num);
								cvSaveImage(filename,dest);
								cvDestroyWindow("zoom");
								cvDestroyWindow("photo");
								cvResetImageROI(click);
								goto menu_tile;
							}
						}
						else start_count=0;

						cout<<start_count<<endl;
						cvNamedWindow("zoom",CV_WINDOW_AUTOSIZE);
						cvShowImage("zoom",dest);
						cvReleaseImage(&dest);
						cvResetImageROI(click);
						graph:	cvNamedWindow("photo",CV_WINDOW_AUTOSIZE);
						cvShowImage("photo",click);
										
						if(!dest)
							cout<<"yes"<<endl;
						else
							cout<<"no"<<endl;
							cvReleaseMemStorage(&storage);
							char c=cvWaitKey(10);
							if(c==27)
								break;
						}
								cout<<"green"<<endl;
					}
				}
				else
				{
					menu=cvLoadImage("F:/Study/embedded systems/open cv/serious/painting final/painting final/blue.jpg",1);
					cvShowImage("menu",menu);
					blue++;
					red=0;
					green=0;
					if(blue==35)
					{
						goto exit;
					//cout<<"blue"<<endl;
					}
				}
			}
			menu_tile: 	cvNamedWindow("menu",CV_WINDOW_AUTOSIZE);
					cvShowImage("menu",menu);
					cvDestroyWindow("Start Window");
					char c=cvWaitKey(10);
					if(c==27)
						break;
					cvReleaseMemStorage(&menu_storage);
		}






		exit:	cvDestroyWindow("Start Window");
		cvReleaseCapture(&cap);
}
