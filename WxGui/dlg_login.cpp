#include "dlg_login.h"

Dlg_Login::Dlg_Login(BaseObjectType *bot, const Glib::RefPtr<Gtk::Builder> &Bdptr)
    :Gtk::Dialog(bot),pbuilder(Bdptr)
{

}
