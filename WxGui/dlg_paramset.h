#ifndef DLG_PARAMSET_H
#define DLG_PARAMSET_H
#include "WxGui_global.h"
#include <iostream>
#include <string>
#include <gtkmm.h>
#include <map>

typedef std::map<std::string,std::string> Dict;
/// @brief
class WXGUI_EXPORT ParamSetDlg : public Gtk::Dialog
{
public:
  ParamSetDlg(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder);
  ~ParamSetDlg() override;

  Dict getmap();
  void setmap(const Dict &refmap);

private:

  Gtk::Button * mBtOk;
  Gtk::Button * mBtCancle;

  Gtk::Label * mLbTitle;

  Gtk::Label * mLbimg1;
  Gtk::Label * mLbimg2;
  Gtk::Entry * mEtimgPath1;
  Gtk::Entry * mEtimgPath2;
  Gtk::Button * mBtPathFind1;
  Gtk::Button * mBtPathFind2;
  //人脸识别模型Xml文件
  Gtk::Entry *mEtFaceXml;
  Gtk::Entry *mEtEyeXml;
  Gtk::Entry *mEtMouthXml;
  Gtk::Button *mBtFaceFileSelect;
  Gtk::Button *mBtEyeFileSelect;
  Gtk::Button *mBtMouthFileSelect;


  //从glade文件读取的界面
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  //参数字典
  Dict param;
};

#endif // DLG_PARAMSET_H
