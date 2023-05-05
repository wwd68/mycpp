#include "mainwindow.h"
using M=MainWindow;

M::MainWindow(BaseObjectType *bot,const Glib::RefPtr<Gtk::Builder> &Bdptr)
    :Gtk::ApplicationWindow(bot),builder(Bdptr),img1(nullptr),img2(nullptr)
{
    builder->get_widget<Gtk::Toolbar>("mToolBar",mToolBar);

    builder->get_widget<Gtk::ToolButton>("mToolBtLogin",mToolBtLogin);
    builder->get_widget<Gtk::ToolButton>("mToolBtRun",mToolBtRun);
    builder->get_widget<Gtk::ToolButton>("mToolBtPause",mToolBtPause);
    builder->get_widget<Gtk::ToolButton>("mToolBtStop",mToolBtStop);
    builder->get_widget<Gtk::ToolButton>("mToolBtSet",mToolBtSet);

    builder->get_widget<Gtk::Label>("mLbimage1",mLbimage1);
    builder->get_widget<Gtk::DrawingArea>("mimg1",mDrwArea1);

    builder->get_widget<Gtk::Label>("mLbimage2",mLbimage2);
    builder->get_widget<Gtk::DrawingArea>("mimg2",mDrwArea2);

    builder->get_widget<Gtk::Label>("mlbrunning",mlbrunning);
    builder->get_widget<Gtk::Label>("mlboperator",mlboperator);
    builder->get_widget<Gtk::Label>("mlbdatabase",mlbdatabase);
    builder->get_widget<Gtk::Label>("mlbPLC",mlbPLC);
    builder->get_widget<Gtk::Label>("mlbprogress",mlbprogress);

    builder->get_widget<Gtk::ProgressBar>("mprogressbar",mprogressbar);


    //mimg1->signal_draw().connect(sigc::mem_fun(*this, &M::on_draw_lb));
    //[](){}匿名函数
    auto on_draw1=[this](const ::Cairo::RefPtr< ::Cairo::Context>& cr){
        try
        {
            Glib::Timer t;      //计时器
            t.start();
            std::cout<<"on_draw_lb1"<<std::endl;
            if(!this->img1){return false;}
            cr->save();         //保存上下文
            //this->mtximg1.lock();   //图像缓冲区加锁
            Gdk::Cairo::set_source_pixbuf(cr,this->img1,
                                          (this->mDrwArea1->get_width()-this->img1->get_width())/2,
                                          (this->mDrwArea1->get_height()-this->img1->get_height())/2);   //更新缓冲区
            //this->mtximg1.unlock(); //解锁
            cr->paint();            //显示
            cr->restore();          //恢复上下文
            t.stop();               //停止计时器
            std::cout<<"ondraw1 time:"<<t.elapsed()<<std::endl; //日志显示处理时间长
        }
        catch(const std::exception &ex)
        {
            std::cout<<ex.what()<<std::endl;
        }
        catch( Gdk::PixbufError r)
        {

            std::cout<<r.what()<<std::endl;
        }

        catch (...) {
        }
        return false;
    };
    mDrwArea1->signal_draw().connect(on_draw1);     //连接信号

    auto on_draw2=[this](const ::Cairo::RefPtr< ::Cairo::Context>& cr){
        try {
            std::cout<<"on_draw_lb2"<<std::endl;
            //cr->set_source_rgb(0.3,0.3,0.3);
            if(!this->img2){return false;}
            cr->save();
            //this->mtximg1.lock();
            Gdk::Cairo::set_source_pixbuf(cr,this->img2,
                                          (this->mDrwArea2->get_width()-this->img2->get_width())/2,
                                          (this->mDrwArea2->get_height()-this->img2->get_height())/2);   //更新缓冲区
            //this->mtximg1.unlock();
            cr->paint();
            cr->restore();

        }  catch (...) {
        }
        return false;
    };
    mDrwArea2->signal_draw().connect(on_draw2);

    auto gcolor=Gdk::RGBA("#00ee00");

    mlbrunning->override_color(gcolor);
    mlboperator->override_color(Gdk::RGBA("#ee0000"));
    mlbdatabase->override_color(gcolor);
    mlbPLC->override_color(gcolor);

    mToolBar->override_background_color(Gdk::RGBA("#004444"));
    mLbimage1->override_background_color(Gdk::RGBA("#004400"));
    mLbimage2->override_background_color(Gdk::RGBA("#004400"));

    mToolBtLogin->signal_clicked().connect([](){
        std::cout<<"on_ToolBt_clicked"<<std::endl;
    });
    mToolBtSet->signal_clicked().connect([this](){
        auto refdialog=Gtk::Builder::create_from_resource("/glade/dlg_paramset.glade");
        ParamSetDlg *mDlgParameterSet=nullptr;
        refdialog->get_widget_derived<ParamSetDlg>("mDlgParameterSet",mDlgParameterSet);
        if(mDlgParameterSet)
        {
            //用参数字典初始化对话框参数
            mDlgParameterSet->setmap(param);
            //显示字典内容
            std::cout<<"pm:\n";
            auto itr=param.begin();
            while(itr!=param.end())
            {
                std::cout<<itr->first<<"="<<itr->second<<std::endl;
                itr++;
            }

            //打开对话框
            auto ret=mDlgParameterSet->run();
            switch (ret)
            {
            //如果按钮为ok
            case Gtk::RESPONSE_OK:
            {
                //取得对话框参数字典
                auto pr=mDlgParameterSet->getmap();
                //迭代器指向字典开始位
                auto itr=pr.begin();
                //文件输出流
                std::ofstream fs;
                //打开文件用于写字典内容
                fs.open(paramfile);
                if(fs.is_open())
                {
                    //字典内容没有写完
                    while(itr!=pr.end())
                    {
                        //显示写入内容
                        std::cout<<itr->first<<"="<<itr->second<<std::endl;
                        //写入文件流
                        fs<<itr->first<<"="<<itr->second<<std::endl;
                        itr++;
                    }
                    //显示对话框按下了ok按钮
                    std::cout<<"paramterset ok"<<std::endl;
                }
                //关闭文件
                fs.close();

                InitParam();

                InitFaceDecet();
            }
            break;
                //按下的取消按钮
            case Gtk::RESPONSE_CANCEL:
                //什么也不做,只显示按下了取消按钮
                std::cout<<"paramterset cancel"<<std::endl;
                break;
            }

            delete mDlgParameterSet;
            //refdialog.~RefPtr();
        };
    });
    //运行按钮click信号连接到on_BtRun函数
    mToolBtRun->signal_clicked().connect(sigc::mem_fun(*this, &M::on_BtRun));

    InitParam();

    InitFaceDecet();
}

void M::on_BtRun()
{
    //按钮不能再次按下，即不使能
    this->mToolBtRun->property_sensitive()=false;
    //开启主线程
    this->pMainThread=Thread::create(sigc::mem_fun(*this,&M::MainThread),"MainThread");
}
/// @brief 图像1线程处理程序
void M::thd1run()
{
    std::string dir=param.find("imgpath1")->second;     //参数字典找到图像1目录
    std::cout<<dir<<std::endl;
    Glib::Dir ds(dir);
    try {
        while(true)
        {
            Glib::Timer t;
            t.start();
            auto fs=ds.read_name();     //读一个文件名
            if(fs.empty())break;    //空读下一个
            std::cout<<fs<<std::endl;
            auto title=fs;
            fs=dir+'/'+fs;              //目录+/+文件名
            auto srcimg=cv::imread(fs);      //从文件读图像
            cv::resize(srcimg,srcimg,cv::Size(srcimg.cols/2,srcimg.rows/2),0,0);
            if(srcimg.empty())continue;         //读图像失败下一个
            //进行人脸识别
            faceRecongize1(srcimg);
            cv::cvtColor(srcimg,srcimg,cv::COLOR_BGR2RGB);//COLOR_BGR2GRAY,COLOR_BGR2RGB
            //imshow("face", srcimg);
            //this->mtximg1.lock();       //图像缓冲区加锁
            //this->img1=Gdk::Pixbuf::create_from_file(fs,
            //this->mDrwArea1->get_width(),this->mDrwArea1->get_height());   //用文件更新图像缓冲区
            //Glib::signal_idle().connect([](){});
            //int r=(*srcimg.step.p+3)/4*4;
            this->img1=Gdk::Pixbuf::create_from_data(
                (guint8*)srcimg.data,
                Gdk::COLORSPACE_RGB,
                false,
                8,//srcimg.depth(),
                srcimg.cols,
                srcimg.rows,
                *srcimg.step.p);
            if(!this->img1) {
                std::cout<<"失败"<<std::endl;
            }
            auto mt=Glib::MainContext::get_default();
            mt->invoke([this,title,srcimg](){
                this->mLbimage1->set_label(title);
                this->mDrwArea1->queue_draw();
                return false;
            });
            t.stop();
            std::cout<<"识别时间:"<<t.elapsed()<<std::endl;
            Glib::usleep(50e3);
            //break;
        }

    }  catch (...) {
        return;
    }

}

/// @brief 图像2线程处理程序
void M::thd2run()
{
    std::string dir=param.find("imgpath2")->second;     //参数字典找到图像目录
    std::cout<<dir<<std::endl;
    Glib::Dir ds(dir);
    try {
        while(true)
        {
            Glib::Timer t;
            t.start();
            auto fs=ds.read_name();     //读一个文件名
            if(fs.empty())break;    //空读下一个
            std::cout<<fs<<std::endl;
            auto title=fs;
            fs=dir+'/'+fs;              //目录+/+文件名
            auto srcimg=cv::imread(fs);      //从文件读图像
            cv::resize(srcimg,srcimg,cv::Size(srcimg.cols/2,srcimg.rows/2),0,0);
            if(srcimg.empty())continue;         //读图像失败下一个
            //进行人脸识别
            faceRecongize2(srcimg);
            cv::cvtColor(srcimg,srcimg,cv::COLOR_BGR2RGB);//COLOR_BGR2GRAY,COLOR_BGR2RGB
            this->img2=Gdk::Pixbuf::create_from_data(
                (guint8*)srcimg.data,
                Gdk::COLORSPACE_RGB,
                false,
                8,//srcimg.depth(),
                srcimg.cols,
                srcimg.rows,
                *srcimg.step.p);
            if(!this->img2){
                std::cout<<"失败"<<std::endl;
            }
            auto mt=Glib::MainContext::get_default();
            mt->invoke([this,title,srcimg](){
                this->mLbimage2->set_label(title);
                this->mDrwArea2->queue_draw();
                return false;
            });
            t.stop();
            std::cout<<"识别时间:"<<t.elapsed()<<std::endl;
            Glib::usleep(50e3);
            //break;
        }

    }  catch (...) {
        return;
    }

}
/// @brief 主线程处理程序
void M::MainThread()
{
    //开启图像1线程
    this->thd1=Thread::create(sigc::mem_fun(*this, &M::thd1run),"thd1");
    //开启图像2线程
    this->thd2=Thread::create(sigc::mem_fun(*this, &M::thd2run),"thd2");
    //等待线程1完成
    this->thd1->join();
    //等待线程2完成
    this->thd2->join();
    //运行按钮再次有效，即使能
    this->mToolBtRun->property_sensitive()=true;
}

void M::InitParam()
{
    //输入文件流
    std::ifstream fs;
    //打开参数文件
    fs.open(paramfile);
    if(fs.is_open())
    {
        param.clear();
        //如果文件没有读完
        while(!fs.eof())
        {
            std::string s;
            //读一行
            std::getline(fs,s);
            //如果空行,读下一行
            if(s.length()==0)continue;
            //std::cout<<s<<std::endl;
            //内存流
            std::stringstream ss(s);

            std::string mkey,mvalue;
            //读到=号
            std::getline(ss,mkey,'=');
            //读其余的
            std::getline(ss,mvalue);
            std::cout<<mkey<<"="<<mvalue<<std::endl;
            //加入参数字典
            param[mkey]=mvalue;
        }
        //关闭文件
        fs.close();
    }
}

//初始化分类器 返回true无错误
bool M::InitFaceDecet()
{
    int error=0;
    auto filename=param.find("face_cascade_name")->second;
    if(!filename.empty())
    {
        // 加载脸部分类器文件
        if (!faceCascade1.load(filename))
        {
            std::cout << "load face_cascade_name1 failed. " << std::endl;
            error++;
        }
        if (!faceCascade2.load(filename))
        {
            std::cout << "load face_cascade_name2 failed. " << std::endl;
            error++;
        }
    }
    filename=param.find("eyes_cascade_name")->second;
    if(!filename.empty())
    {
        // 加载眼睛部分分类器文件
        if (!eyesCascade1.load(filename))
        {
            std::cout << "load eyes_cascade_name1 failed. " << std::endl;
            error++;
        }
        // 加载眼睛部分分类器文件
        if (!eyesCascade2.load(filename))
        {
            std::cout << "load eyes_cascade_name2 failed. " << std::endl;
            error++;
        }
    }
    filename=param.find("mouth_cascade_name")->second;

    if(!filename.empty())
    {
        // 加载嘴部分类器文件
        if (!mouthCascade1.load(filename))
        {
            std::cout << "load mouth_cascade_name1 failed. " << std::endl;
            error++;
        }
        // 加载嘴部分类器文件
        if (!mouthCascade2.load(filename))
        {
            std::cout << "load mouth_cascade_name2 failed. " << std::endl;
            error++;
        }
    }
    return error==0;
}
//人脸识别
void M::faceRecongize1(cv::Mat srcimg)
{
    std::vector<cv::Rect> faces;
    // 检测人脸
    faceCascade1.detectMultiScale(srcimg, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
    for (int i = 0; i < faces.size(); i++) {

        // 用椭圆画出人脸部分
        cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(srcimg, center, cv::Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);

        cv::Mat faceROI = srcimg(faces[i]);
        std::vector<cv::Rect> eyes;

        // 检测眼睛
        eyesCascade1.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
        for (int j = 0; j < eyes.size(); j++)
        {
            // 用圆画出眼睛部分
            cv::Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
            int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
            circle(srcimg, eye_center, radius, cv::Scalar(255, 0, 0), 4, 8, 0);
        }

        cv::Mat mouthROI = srcimg(faces[i]);
        std::vector<cv::Rect> mouth;

        // 检测嘴部
        mouthCascade1.detectMultiScale(mouthROI, mouth, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
        for (int k = 0; k < mouth.size(); k++)
        {
            //用长方形画出嘴部
            cv::Rect rect(faces[i].x + mouth[k].x, faces[i].y + mouth[k].y, mouth[k].width, mouth[k].height);
            rectangle(srcimg, rect, cv::Scalar(0, 255, 0), 2, 8, 0);
        }

        // 检测到两个眼睛和一个嘴巴, 可认为检测到有效人脸
        if (eyes.size() >= 2 && mouth.size() >= 1) {

            // 人脸上方区域写字进行标识
            cv::Point centerText(faces[i].x + faces[i].width / 2 - 40, faces[i].y - 20);
            cv::putText(srcimg, "face", centerText, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        }
    }
}
//人脸识别
void M::faceRecongize2(cv::Mat srcimg)
{
    std::vector<cv::Rect> faces;
    // 检测人脸
    faceCascade2.detectMultiScale(srcimg, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
    for (int i = 0; i < faces.size(); i++) {

        // 用椭圆画出人脸部分
        cv::Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(srcimg, center, cv::Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);

        cv::Mat faceROI = srcimg(faces[i]);
        std::vector<cv::Rect> eyes;

        // 检测眼睛
        eyesCascade2.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
        for (int j = 0; j < eyes.size(); j++)
        {
            // 用圆画出眼睛部分
            cv::Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
            int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
            circle(srcimg, eye_center, radius, cv::Scalar(255, 0, 0), 4, 8, 0);
        }

        cv::Mat mouthROI = srcimg(faces[i]);
        std::vector<cv::Rect> mouth;

        // 检测嘴部
        mouthCascade2.detectMultiScale(mouthROI, mouth, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
        for (int k = 0; k < mouth.size(); k++)
        {
            //用长方形画出嘴部
            cv::Rect rect(faces[i].x + mouth[k].x, faces[i].y + mouth[k].y, mouth[k].width, mouth[k].height);
            rectangle(srcimg, rect, cv::Scalar(0, 255, 0), 2, 8, 0);
        }

        // 检测到两个眼睛和一个嘴巴, 可认为检测到有效人脸
        if (eyes.size() >= 2 && mouth.size() >= 1) {

            // 人脸上方区域写字进行标识
            cv::Point centerText(faces[i].x + faces[i].width / 2 - 40, faces[i].y - 20);
            cv::putText(srcimg, "face", centerText, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
        }
    }
}
