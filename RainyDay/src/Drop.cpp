#include "Drop.h"
#include "ofApp.h"

Drop::Drop(ofImage *image, ofImage *blur, ofVec2f pos, float rad, int width, int height, float ratio){
    position = pos;
    radius = rad;
    drawWidth = width;
    drawHeight = height;
    moving = false;
    imageRatio = ratio;
    
    bgImage.allocate(radius, radius, OF_IMAGE_COLOR);
    //bgImage.cropFrom(*blur, position.x * ratio, position.y * ratio, radius * ratio, radius * ratio);
    bgImage.cropFrom(*blur, position.x, position.y, radius, radius);
    
    //float ratio = image->getWidth() / float(drawWidth);
    float cropWidth = drawWidth / 8.0 * ratio;
    float cropHeight = drawHeight / 8.0 * ratio;
    float cropX = ofMap(position.x, 0, drawWidth, cropWidth * ratio, (drawWidth - cropWidth) * ratio * 0.9);
    float cropY = ofMap(position.y, 0, drawHeight, cropHeight * ratio, (drawHeight- cropHeight) * ratio * 0.9);
    
    inputImage.allocate(cropWidth, cropHeight, OF_IMAGE_COLOR);
    inputImage.cropFrom(*image, cropX, cropY, cropWidth, cropHeight);
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

void Drop::update(){
    if (moving) {
        position += velocity * radius;
        if (position.y > drawHeight) {
            position.y -= drawHeight;
        }
        velocity *= 0.95;
        //checkCollision();
    }
    if (velocity.length() < 0.001) {
        moving = false;
        velocity.set(0, 0);
    }
}


void Drop::draw(){
    dropImage.draw(position, radius, radius);
}

void Drop::kill(){
    ((ofApp*)ofGetAppPtr())->dropFbo.begin();
    bgImage.draw(position);
    ((ofApp*)ofGetAppPtr())->dropFbo.end();
}

void Drop::checkCollision(){
    int size = ((ofApp*)ofGetAppPtr())->baseDrops.size();
    for (int i = 0; i < size; i++) {
        float dist = ofDist(position.x, position.y,
                            ((ofApp*)ofGetAppPtr())->baseDrops[i]->position.x,
                            ((ofApp*)ofGetAppPtr())->baseDrops[i]->position.y);
        if (dist < radius * 4) {
            ((ofApp*)ofGetAppPtr())->baseDrops[i]->kill();
        }
    }
}

Drop::~Drop(){
    
}