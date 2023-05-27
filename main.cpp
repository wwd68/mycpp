#include <iostream>
#include "gtkmm.h"
#include "dlg_paramset.h"
#include "mainwindow.h"
#include "mysql/mysql.h"

using namespace std;
using namespace Gtk;

/// @brief 主程序入口
int main(int argc,char **argv)
{
    Main app(argc,argv);    //初始化GTK::Main 实例，创建主线程
    //从glib-compile-resources 编译生成的资料包，读取资源
    auto myrc=Gio::Resource::create_from_file("WxGui/mygrec.rc");
    //资源注册为全局
    myrc->register_global();
    auto ss=myrc->enumerate_children("/");
    //从全局资源中读取主窗口界面
    auto myuiptr=Gtk::Builder::create_from_resource("/glade/MainWindow.glade");
    MainWindow *mwin=nullptr;
    //得到主窗口
    myuiptr->get_widget_derived<MainWindow>("MainWindow",mwin);
    //如果ID中没有MainWindow,返回空指针nullptr,空指针退出程序
    if(!mwin)return -1;
    mwin->maximize();   //窗口最大化
    app.run(*mwin);     //运行主线程
    delete mwin;        //析构主窗口
    return 0;
}
