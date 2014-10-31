#include "Drop.h"
#include "ofApp.h"

Drop::Drop(ofImage *image, ofImage *mask, ofVec2f pos, float rad){
    position = pos;
    radius = rad;
    
    float cropWidth = ofGetWidth() / 8.0;
    float cropHeight = ofGetHeight() / 8.0;
    float cropX = ofMap(position.x, 0, ofGetWidth(), cropWidth, ofGetWidth() - cropWidth);
    float cropY = ofMap(position.y, 0, ofGetHeight(), cropHeight, ofGetHeight()- cropHeight);
    
    dropImage = *image;
    dropImage.crop(cropX, cropY, cropWidth, cropHeight);
    
    
    /*
    cv::Mat image_src  = ofxCv::toCv(*image);
    cv::Mat image_mask = ofxCv::toCv(*mask);
    cv::Mat image_dest;
    
    // 素材画像をチャンネル(RGB)ごとに分離してvectorに格納する
    std::vector<cv::Mat> mv;
    cv::split(image_src, mv);
    
    // 注目領域：マスク画像の中心の素材画像の大きさの領域
    cv::Rect rect((image_mask.cols - image_src.cols) / 2,
                  (image_mask.rows - image_src.rows) / 2,
                  image_src.cols,
                  image_src.rows);
    // vectorの最後尾にマスク画像の注目領域を追加する
    mv.push_back(cv::Mat(image_mask, rect));
    
    // vectorを結合して加工後の画像とする
    cv::merge(mv, image_dest);
    
    // 加工後の画像を出力する
    ofxCv::toOf(image_dest, dropImage);
    */
}

void Drop::draw(){
    ofSetColor(255, 255, 255);
    dropImage.draw(position, radius*2, radius*2);
}