#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <iostream>
#include <fstream>
#include <sstream>		//内存流
#include "WxGui_global.h"
#include "dlg_paramset.h"
#include <gtkmm.h>
#include <glibmm.h>
#include <glibmm/threads.h>
#include <map>			//标准c++字典
#include <filesystem>
#include <opencv4/opencv2/opencv.hpp>

//声名参数文件名称
#define paramfile "param.prm"
//字典类名太长太复杂重定义一个别名
typedef std::map<std::string,std::string> Dict;
using namespace Glib::Threads;
//与资源界面相配合的主窗口类
class WXGUI_EXPORT MainWindow :public Gtk::ApplicationWindow
{
public:
    //固定格式的构造函数
    MainWindow(BaseObjectType *bot,const Glib::RefPtr<Gtk::Builder> &Bdptr);
private:
    //由资源创建的Builder指针
    Glib::RefPtr<Gtk::Builder> builder;
    //Toolbar和其中的ToolButton与资源中的ID相同
    Gtk::Toolbar *mToolBar;
    Gtk::ToolButton *mToolBtLogin;
    Gtk::ToolButton *mToolBtRun;
    Gtk::ToolButton *mToolBtPause;
    Gtk::ToolButton *mToolBtStop;
    Gtk::ToolButton *mToolBtSet;

    Gtk::Label *mLbimage1;
    Gtk::DrawingArea *mDrwArea1;    //自由绘制区窗口
    Gtk::Label *mLbimage2;
    Gtk::DrawingArea *mDrwArea2;    //自由绘制区窗口

    Gtk::Label *mlbrunning;
    Gtk::Label *mlboperator;
    Gtk::Label *mlbdatabase;
    Gtk::Label *mlbPLC;

    Gtk::ProgressBar *mProcgess1;
    Gtk::ProgressBar *mProcgess2;
    //声名参数字典
    Dict param;
    //主线程指针，指示主程序运行状态
    Thread *pMainThread=nullptr;

    //子线程1指针，指示运行状态
    Thread *thd1=nullptr;
    //子线程2指针，指示运行状态
    Thread *thd2=nullptr;

    //bool on_draw_lb(const ::Cairo::RefPtr< ::Cairo::Context>& cr);
    //运行按钮，按下响应
    void on_BtRun();

    //主线程处理按钮事件，处理事务，更新界面显示，处理结果保存......
    void MainThread();
    void thd1run();
    void thd2run();

    //图像缓冲区
    Glib::RefPtr<Gdk::Pixbuf> img1;
    Glib::RefPtr<Gdk::Pixbuf> img2;
    //图像缓冲区锁
    //Glib::Mutex mtximg1;
    //Glib::Mutex mtximg2;
    //界面更新用回界面线程的调用
    //Glib::Dispatcher Invoke1;
    //Glib::Dispatcher Invoke2;

    //初始化参数字典
    void InitParam();

    //人脸,眼,嘴识别器对象,不是线程安全的
    //要为每个线程单独分配一个对象
    cv::CascadeClassifier faceCascade1;
    cv::CascadeClassifier eyesCascade1;
    cv::CascadeClassifier mouthCascade1;

    cv::CascadeClassifier faceCascade2;
    cv::CascadeClassifier eyesCascade2;
    cv::CascadeClassifier mouthCascade2;
    //人脸识别,不是线程安全的
    //要为每个线程单独写一个函数
    void faceRecongize1(cv::Mat srcimg);
    void faceRecongize2(cv::Mat srcimg);

    //初始化分类器 返回true无错误
    bool InitFaceDecet();
};

#endif // MAINWINDOW_H
