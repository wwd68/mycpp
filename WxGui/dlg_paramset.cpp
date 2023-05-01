#include "dlg_paramset.h"

/// @brief 
ParamSetDlg::ParamSetDlg(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refBuilder)
    :Gtk::Dialog(cobject),m_refBuilder(refBuilder)
{
    m_refBuilder->get_widget<Gtk::Button>("mBtOk",mBtOk);
    m_refBuilder->get_widget<Gtk::Button>("mBtCancle",mBtCancle);

    m_refBuilder->get_widget<Gtk::Label>("mLbTitle",mLbTitle);
    m_refBuilder->get_widget<Gtk::Label>("mLbimg1",mLbimg1);
    m_refBuilder->get_widget<Gtk::Label>("mLbimg2",mLbimg2);

    m_refBuilder->get_widget<Gtk::Entry>("mEtimgPath1",mEtimgPath1);
    m_refBuilder->get_widget<Gtk::Entry>("mEtimgPath2",mEtimgPath2);

    m_refBuilder->get_widget<Gtk::Button>("mBtPathFind1",mBtPathFind1);
    m_refBuilder->get_widget<Gtk::Button>("mBtPathFind2",mBtPathFind2);

    mLbTitle->override_background_color(Gdk::RGBA("#004400"));
    //人脸识别模型Xml文件
    m_refBuilder->get_widget<Gtk::Entry>("mEtFaceXml",mEtFaceXml);
    m_refBuilder->get_widget<Gtk::Entry>("mEtEyeXml",mEtEyeXml);
    m_refBuilder->get_widget<Gtk::Entry>("mEtMouthXml",mEtMouthXml);

    m_refBuilder->get_widget<Gtk::Button>("mBtFaceFileSelect",mBtFaceFileSelect);
    m_refBuilder->get_widget<Gtk::Button>("mBtEyeFileSelect",mBtEyeFileSelect);
    m_refBuilder->get_widget<Gtk::Button>("mBtMouthFileSelect",mBtMouthFileSelect);

    //    auto p1=mEtimgPath1;
    mBtPathFind1->signal_clicked().connect([this](){

        Gtk::FileChooserDialog dirs("选择第一图像目录",Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
        dirs.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_ACCEPT);
        dirs.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);

        auto ret=dirs.run();

        if(ret==Gtk::RESPONSE_ACCEPT)
        {
            this->mEtimgPath1->set_text(dirs.get_filename());
        }
    });
    //    auto p2=mEtimgPath2;
    mBtPathFind2->signal_clicked().connect([this](){

        Gtk::FileChooserDialog dirs("选择第二图像目录",Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER);
        dirs.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_ACCEPT);
        dirs.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
        dirs.resize(960,540);

        auto ret=dirs.run();

        if(ret==Gtk::RESPONSE_ACCEPT)
        {
            this->mEtimgPath2->set_text(dirs.get_filename());
        }
    });

    mBtFaceFileSelect->signal_clicked().connect([this](){
        Gtk::FileChooserDialog facexml("选择人脸识别模型文件xml",Gtk::FILE_CHOOSER_ACTION_OPEN);
        facexml.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_ACCEPT);
        facexml.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
        facexml.resize(960,540);
        auto ret=facexml.run();

        if(ret==Gtk::RESPONSE_ACCEPT)
        {
            this->mEtFaceXml->set_text(facexml.get_filename());
        }

    });
    mBtEyeFileSelect->signal_clicked().connect([this](){
        Gtk::FileChooserDialog Eyexml("选择人眼识别模型文件xml",Gtk::FILE_CHOOSER_ACTION_OPEN);
        Eyexml.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_ACCEPT);
        Eyexml.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
        Eyexml.resize(960,540);
        auto ret=Eyexml.run();

        if(ret==Gtk::RESPONSE_ACCEPT)
        {
            this->mEtEyeXml->set_text(Eyexml.get_filename());
        }

    });
    mBtMouthFileSelect->signal_clicked().connect([this](){
        Gtk::FileChooserDialog mouthxml("选择人嘴识别模型文件xml",Gtk::FILE_CHOOSER_ACTION_OPEN);
        mouthxml.add_button(Gtk::Stock::OPEN,Gtk::RESPONSE_ACCEPT);
        mouthxml.add_button(Gtk::Stock::CANCEL,Gtk::RESPONSE_CANCEL);
        mouthxml.resize(960,540);
        auto ret=mouthxml.run();

        if(ret==Gtk::RESPONSE_ACCEPT)
        {
            this->mEtMouthXml->set_text(mouthxml.get_filename());
        }

    });



    mBtOk->signal_clicked().connect([this]()
    {
        this->response(Gtk::RESPONSE_OK);
        this->param.clear();
        this->param["imgpath1"]=this->mEtimgPath1->get_text();
        this->param["imgpath2"]=this->mEtimgPath2->get_text();

        this->param["face_cascade_name"]=this->mEtFaceXml->get_text();
        this->param["eyes_cascade_name"]=this->mEtEyeXml->get_text();
        this->param["mouth_cascade_name"]=this->mEtMouthXml->get_text();


        this->close();}
    );
    mBtCancle->signal_clicked().connect([this](){this->response(Gtk::RESPONSE_CANCEL); this->close();});
}

ParamSetDlg::~ParamSetDlg()
{

}

Dict ParamSetDlg::getmap()
{
    return param;
}
void ParamSetDlg::setmap(const Dict &refmap)
{
    const std::string ss[]{
        "imgpath1",
        "imgpath2",
        "face_cascade_name",
        "eyes_cascade_name",
        "mouth_cascade_name"
    };
    Gtk::Entry *ets[]
    {
        mEtimgPath1,
        mEtimgPath2,
        mEtFaceXml,
        mEtEyeXml,
        mEtMouthXml
    };

    for (int i=0;i<5 ;i++ ) {
       ets[i]->set_text(refmap.find(ss[i])->second);
    }
}
