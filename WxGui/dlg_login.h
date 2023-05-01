#ifndef DLG_LOGIN_H
#define DLG_LOGIN_H

#include "WxGui_global.h"
#include <iostream>
#include <gtkmm.h>


class WXGUI_EXPORT Dlg_Login: public Gtk::Dialog
{
public:
    Dlg_Login(BaseObjectType *bot,const Glib::RefPtr<Gtk::Builder> &Bdptr);

private:
    Glib::RefPtr<Gtk::Builder> pbuilder;
};

#endif // DLG_LOGIN_H
