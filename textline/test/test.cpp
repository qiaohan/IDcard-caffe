#include "Textline4idcard.h"
#include "engine_caffe.h"
#include "FilePathClass.h"
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace singlechar;

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while(std::string::npos != pos2){
		v.push_back(s.substr(pos1, pos2-pos1));
		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if(pos1 != s.length())
		v.push_back(s.substr(pos1));
}

string recogoneline(string fname, CharEngine * engine, string savename="res/"){
	Mat textl_color = imread(fname,CV_LOAD_IMAGE_COLOR);
	/*
	Mat textl;
	cvtColor(textl_color, textl, CV_BGR2HSV);
	vector<Mat> matvec;
	split(textl,matvec);
	textl = matvec[0];
	*/
	static IDcardTextlineReader textline;
	textline.setImgName(savename);
	vector<Rect> rects;
	vector<int> chars;
	stringstream ress;
	if( !textline.ReadText(textl_color,rects,chars) )
	{
		cout<<"failed"<<endl;
		return "null";
	}

	for(int i=0;i<rects.size();i++){
		//cout<<i<<','<<rects[i]<<endl;
		int cls=-1;
		float conf=-1;
		Mat img = textl_color(rects[i]);
		Mat imgg =AlignRect(img);
		imgg.convertTo(imgg, CV_32FC3);
		if(NULL!=engine)
			engine -> RecognizeChar(imgg, cls, conf);
		ress<<rects[i].tl().x<<'_'<<rects[i].tl().y<<'_'<<rects[i].br().x<<'_'<<rects[i].br().y<<'_'<<cls<<' ';
		stringstream ss;
		ss<<i;
		string charfname = "tmp/"+savename+"_"+ss.str()+".jpg";
		cout<<charfname<<endl;
		imwrite(charfname,AlignRect(img));
	}
	return ress.str();
}
int main1(int argc,char ** argv){
	vector<PathPair> imgpaths = FilePathClass::getFilePath("/home/hzqiaohan/OCR/MusicSegment/Result/",".png"); 
	cout<<"start test"<<endl;
	/*
	for(int i=0; i<imgpaths.size(); i++){
		recogoneline(imgpaths[i].fullPath,NULL,imgpaths[i].Name);
	}*/
	recogoneline("/home/hzqiaohan/OCR/MusicSegment/Result/datatestset1920_1152虾米screen (8)_datatestset1920_1152虾米screen (8)_2.artist.png",NULL,"datatestset1920_1152虾米screen (8)_datatestset1920_1152虾米screen (8)_2.artist.png");
}
int main(int argc,char ** argv){
	//SplitString(string(argv[1]),fnames,":");
	//cout<<fnames[0]<<endl;
	//CharEngine * engine = new CharEngineCaffe("/home/hzqiaohan/OCR/danzi_deep/casia_v4_cls3817_v2v3datasets_bg_extra/deploy_casia_v4.prototxt","/home/hzqiaohan/OCR/danzi_deep/IDcardBG/xiahou/ckpt/ocr_iter_50000.caffemodel",64,64);
	if(argc==2){
		cout<<recogoneline(argv[1],NULL,"test.png")<<endl;
		return 0;
	}
	ofstream fres("textlineresult.txt",ios::app);
	vector<PathPair> imgpaths = FilePathClass::getFilePath("/home/hzqiaohan/OCR/CTPN/textlines/",".jpg"); 
	//vector<PathPair> imgpaths = FilePathClass::getFilePath("idcards/testIDtextline/",".jpg"); 
	for(int i=0; i<imgpaths.size(); i++){
		fres<<imgpaths[i].Name<<' '<<recogoneline(imgpaths[i].fullPath,NULL,"test/"+imgpaths[i].Name)<<endl;
		//fres<<fnames[i]<<' '<<recogoneline(fnames[i],engine)<<endl;
	}
	fres.close();
	//delete engine;
	return 0;
}
bool getbt(Mat img, int &bottom, int top){
	Mat binaryimg;
	int thre = 45;
	equalizeHist(img,img);
	//imwrite("gray/"+m_Imagename+".jpg",img);
	if(1)
		threshold(img,binaryimg,thre,255,THRESH_BINARY_INV);
	else
		threshold(img,binaryimg,thre,255,THRESH_BINARY);
	//imwrite("bi/"+m_Imagename+".jpg",binaryimg);
	/*
	 * projection in vertical direction
	 */
	vector<float> verproj(binaryimg.rows);
	top = -1;
	bottom = -1;
	float threv = 0.1;
	float maxv = 0.0;
	for(int j=0; j<verproj.size(); j++){
		verproj[j] = 0;
		for(int i=0; i<binaryimg.cols; i++){
			if(binaryimg.at<uchar>(j,i)>1)
				verproj[j]++;
		}
		maxv = maxv>verproj[j]?maxv:verproj[j];
	}
	for(int i=0; i<verproj.size(); i++)
		verproj[i]/=maxv;
	for(int j=1; j<verproj.size()/2; j++){
		if(verproj[j]>threv&&verproj[j-1]<threv)
		{
			top = j;
			break;
		}
	}
	for(int j=verproj.size()-2; j>verproj.size()/2; j--){
		if(verproj[j]>threv&&verproj[j+1]<threv)
		{
			bottom = j;
			break;
		}
	}
	if(-1==top || -1==bottom)
		return false;
	else
		return true;
}
int getpatchs(string impath,string savepath){
	Mat textl_mat = imread(impath);
	Mat him,bimg;
	cvtColor(textl_mat, him, CV_BGR2GRAY);
	equalizeHist(him,him);
	threshold(him,bimg,100,1,THRESH_BINARY_INV);
	int start;
	for(int i=0; i<bimg.cols; i++){
		int cnt=0;
		for(int j=0; j<bimg.rows; j++)
			cnt += bimg.at<uchar>(i,j);
		if(cnt>0){
			start = i;
			break;
		}
	}
	cout<<"start:"<<start<<endl;
	int b,t;
	if(!getbt(him.clone(),b,t))
		return 0;
	cout<<"bottom:"<<b<<" top:"<<t<<endl;
	int end = (b-t)*3+start;
	uchar bb = textl_mat.at<uchar>(0,0);
	uchar g = textl_mat.at<uchar>(0,1);
	uchar r = textl_mat.at<uchar>(0,2);
	Mat patchim = Mat((b-t),3*(b-t),textl_mat.type(),Scalar(bb,g,r));
	if(end>him.cols){
		cout<<"end > im cols"<<endl;
		Rect patchrt = Rect(start,t,him.cols-start,b-t);
		textl_mat(patchrt).copyTo( patchim );//(Rect(0,0,him.cols-start,patchim.rows)) );
	}
	else{
		cout<<"end <= im cols"<<endl;
/*
		for(int i=end; i<bimg.cols; i++){
			int cnt=0;
			for(int j=0; j<bimg.rows; j++)
				cnt += bimg.at<uchar>(i,j);
			if(cnt<=5||i==bimg.cols-1){
				end = i;
				break;
			}
		}
*/
		Rect patchrt = Rect(start,t,end-start,b-t);
		cout<<patchrt<<','<<textl_mat.size()<<endl;
		cout<<Rect(0,0,end-start,patchim.rows)<<','<<patchim.size()<<endl;
		textl_mat(patchrt).copyTo( patchim ); //(Rect(0,0,end-start,patchim.rows)) );
	}
	imwrite(savepath,patchim);
	//imwrite(savepath,bimg*255);
	//imwrite(savepath,bimg1.mul(bimg2)*255);
	return 1;
}

int main2(int argc,char ** argv){
	ofstream fres("textlineresult.txt",ios::app);
	//vector<PathPair> imgpaths = FilePathClass::getFilePath("/home/hzqiaohan/OCR/CTPN/textlines/",".jpg"); 
	vector<PathPair> imgpaths = FilePathClass::getFilePath("/home/hzqiaohan/OCR/QHreadtextlineDP/building/idcards/testIDtextline/",".jpg"); 
	for(int i=0; i<imgpaths.size(); i++){
		fres<<imgpaths[i].Name<<' '<<getpatchs(imgpaths[i].fullPath,"patchs/"+imgpaths[i].Name)<<endl;
		//fres<<fnames[i]<<' '<<recogoneline(fnames[i],engine)<<endl;
	}
	fres.close();
	//delete engine;
	return 0;
}
