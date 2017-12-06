//
//  BreakerScene.h
//  Breaker
//
//  Created by Naiding Z on 12/3/17.
//

#ifndef BreakerScene_h
#define BreakerScene_h

#include "cocos2d.h"
USING_NS_CC;

/////////////////////////////////////////
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <vector>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/nonfree.hpp>

//using namespace cv;
//using namespace std;
/////////////////////////////////////////

class Breaker : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Breaker);

    void keyPressedCallback(EventKeyboard::KeyCode keyCode, Event *event);
    void keyReleasedCallback(EventKeyboard::KeyCode keyCode, Event *event);
    virtual bool onContactBegin(PhysicsContact& contact);

    virtual void update(float delta);
    virtual void onEnter();
    virtual void onExit();
    
private:
    
    bool isStart;
    bool isPause;
    void gameInit();
    void gameStart();
    void gameOver();
    
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    int playerTag;
    int ballTag;
    int statusLabelTag;
    
    bool moveLeft;
    bool moveRight;
    float playerMoveSpeed;
    Vec2 ballMoveSpeed;
    void createPlayer(int tag);
    void createBall(int tag);
    void createStatusLabel(int tag);
    Sprite* createBrickAtPosition(Vec2 p, int tag);
    
    void registerKeyboardListener();
    
    int playTimes;
    
    cocos2d::Vector<Sprite*> bricksVec;

    ////////////////////////////
    
    void initGridDetector();
    
    double dis;
    double outdis;
    double disnew;
    
    bool gridMovingLeft;
    bool gridMovingRight;
    
    cv::VideoCapture cap;
    
    cv::Mat src;
    std::vector<cv::Point> srcCorner;
    std::vector<cv::Point> dstCorner;
    
    cv::Ptr<cv::FeatureDetector> featureDetector;
    cv::Ptr<cv::DescriptorExtractor> descriptorExtractor;
    cv::Ptr<cv::DescriptorMatcher> descriptorMatcher;
    
    cv::Mat frame;
    cv::Mat grayFrame;
    cv::Mat frameImg;
    cv::Mat queryDescriptor;
    cv::Mat trainDescriptor;
    std::vector<cv::KeyPoint> queryKeypoints;
    std::vector<cv::KeyPoint> trainKeypoints;
    
    bool refineMatchesWithHomography(const std::vector<cv::KeyPoint>& queryKeypoints,
                                     const std::vector<cv::KeyPoint>& trainKeypoints,
                                     float reprojectionThreshold,
                                     std::vector<cv::DMatch>& matches,
                                     cv::Mat& homography,
                                     cv::Mat& src,
                                     cv::Mat& frameImg);
    
    bool matchingDescriptor(const std::vector<cv::KeyPoint>& queryKeyPoints,
                            const std::vector<cv::KeyPoint>& trainKeyPoints,
                            const cv::Mat& queryDescriptors,
                            const cv::Mat& trainDescriptors,
                            cv::Ptr<cv::DescriptorMatcher>& descriptorMatcher,
                            cv::Mat& src,
                            cv::Mat& frameImg,
                            std::vector<cv::Point>& srcCorner,
                            std::vector<cv::Point>& dstCorner,
                            bool enableRatioTest = true);
    
    void checkGridOnce();
    bool isCheckingGrid;
    ///////////////////////////
    
};

#endif /* BreakerScene_h */
