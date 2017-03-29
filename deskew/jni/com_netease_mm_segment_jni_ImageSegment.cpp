#include "com_netease_mm_segment_jni_ImageSegment.h"
#include "BCRDeskew.h"

using namespace cv;
/*
 * Class:     com_netease_mm_segment_jni_ImageSegment
 * Method:    segment
 * Signature: (J)J
 */
//extern int longedge;
JNIEXPORT jbyteArray JNICALL Java_com_netease_mm_segment_jni_ImageSegment_segment(JNIEnv * env, jclass, jstring impath, jint length){
	//longedge = length;
	//uchar* data = (uchar*)(&src);
	try{
	Mat src_mat = imread(env->GetStringUTFChars(impath, NULL));//imdecode( InputArray(Mat((CvMat*)data)),CV_LOAD_IMAGE_COLOR);
	if(src_mat.empty())
		return NULL;
	Mat im_org = src_mat;
	//Quadrangle myResQuad;
	Mat resImg = BCRDeskew(im_org, /*&myResQuad,*/ length);
	if(resImg.empty())
		return NULL;
	vector<uchar> out;
	imencode(".jpg",resImg,out);
	jbyteArray res = env->NewByteArray(out.size());
	env->SetByteArrayRegion(res,0,out.size(),(jbyte*)out.data());
	return res;
	}
	catch(...){return NULL;}
}
