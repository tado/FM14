#include "Drop.h"
#include "ofApp.h"

Drop::Drop(ofImage *image, ofVec2f pos, float rad){
    position = pos;
    radius = rad;
    
    float cropWidth = ofGetWidth() / 3.0;
    float cropHeight = ofGetHeight() / 3.0;
    float cropX = ofMap(position.x, 0, ofGetWidth(), cropWidth, ofGetWidth() - cropWidth);
    float cropY = ofMap(position.y, 0, ofGetHeight(), cropHeight, ofGetHeight()- cropHeight);
    
    inputImage = *image;
    inputImage.crop(cropX, cropY, cropWidth, cropHeight);
    inputImage.resize(radius*2, radius*2);
    
    // Crop image to circle
    cv::Mat src_img, dst_img, mask_img;
    src_img = ofxCv::toCv(inputImage);
    std::vector<cv::Mat> mv;
    cv::split(src_img, mv);
    mask_img = cv::Mat::zeros(src_img.rows, src_img.cols, CV_8U);
    cv::circle(mask_img, cv::Point(radius, radius), radius, cv::Scalar(255,255,255), -1, 8, 0);
    mv.push_back(mask_img);
    cv::merge(mv, dst_img);
    ofxCv::toOf(dst_img, dropImage);
    dropImage.update();
}

void Drop::draw(){
    ofSetColor(255, 255, 255);
    //inputImage.draw(position, radius, radius);
    dropImage.draw(position, radius, radius);
}