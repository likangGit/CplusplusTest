#include <QCoreApplication>
#include <QThreadPool>
#include <QRunnable>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <QMutex>
QMutex mutex;
std::vector<int> num;
std::vector<double> before,after;
int64 min=-1,max=0;
class A : public QRunnable
{
public:
    A(int id){ m_nID = id;}
    void run(){
        //std::cout <<m_nID<< std::endl;
        mutex.lock();
        num.push_back(m_nID);
        mutex.unlock();

        int64 t = cv::getTickCount();
        if (min<0) min = t;
        if (min>t) min = t;
//        double f = cv::getTickFrequency();
//        mutex.lock();
//        before.push_back(t/f);
//        mutex.unlock();
        //memcpy(dst,src,1080*1920*3);
        for ( int i=0; i<1080*1920*3; ++i) {
            src[i]+dst[i];
        }
//        mutex.lock();
//        after.push_back((double)cv::getTickCount()/f);
//        mutex.unlock();
        int64 x = cv::getTickCount();
        if (max < x) max = x;
        std::cout <<m_nID <<" "<< (double)(x - t)/cv::getTickFrequency()<<std::endl;
    }
private:
    int m_nID;
    uchar src[1080*1920*3]={1};
    uchar dst[1080*1920*3]={0};
};
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    int maxNum = 40;
    QThreadPool threadPool;
    threadPool.setMaxThreadCount(7);
    std::vector<A*> a_s(maxNum,NULL);
    for (int i=0; i<maxNum; ++i) {
        a_s[i] = new A(i);
    }
    for (int i=0; i<maxNum; ++i) {
        threadPool.start(a_s[i]);
    }
    std::getchar();
    for (int i=0; i<maxNum; ++i) {
        std::cout <<num[i]<<std::endl;//" " << before[i] << " " << after[i]<<std::endl;
    }
    std::cout <<(max-min)/cv::getTickFrequency()<<std::endl;
    return a.exec();
}
