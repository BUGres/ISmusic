#ifndef WIDGET_H
#define WIDGET_H

#include <QT>
#include <QWidget>
#include <QMouseEvent>//用于重载鼠标事件函数
#include <QFile>
#include <QLabel>
#include <QMediaPlayer>
#include <QAudioOutput>

#include <QDebug>


#include <io.h>//文件操作
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


class menuChoose{//菜单选择类
public:
    int tar;
    void nemuChoose(){
        tar=0;
    }
    bool canChange(int input){
        if(input==tar)return false;
        else return true;
    }
};
class songSheet{//歌单类
public:
    vector<QString> QpathMenu;
    vector<QImage> QimageMenu;
    int Count(){
        if (QpathMenu.size()==QimageMenu.size())return QpathMenu.size();
        return 0;
    }
    void add(QString strinput,QImage imginput){
        QpathMenu.push_back(strinput);
        QimageMenu.push_back(imginput);
    }
};
class songClass{
public:

};

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QLabel *labelMenu[8] = {new QLabel(this),new QLabel(this),new QLabel(this),new QLabel(this),new QLabel(this),new QLabel(this),new QLabel(this),new QLabel(this)};//歌单八个按钮
    QLabel *labelList[10]={new QLabel(this)};

    QAudioOutput *audioOutPut = new QAudioOutput;//输出源
    QMediaPlayer *player = new QMediaPlayer;//播放器

    void startplay(int number);//播放音乐 参数：歌单id 歌曲顺序
    void loadSongSheet(int id);//载入歌单 参数: 歌单id
    void setVolumeEx(float volume);//声音设置


    songSheet songsheet;//歌单实例
    void songSheetShow();//打开歌单
    void songSheetClose();//关闭歌单

    void songListShow();//打开歌曲表
    void songListClose();//关闭歌曲表

    void NewsShow();
    void NewsClose();
    void showSongListPage(int page);

protected:
    virtual void mousePressEvent(QMouseEvent *event);    //重写鼠标点击事件
    virtual void mouseMoveEvent(QMouseEvent *event);     //重写鼠标点击事件
    virtual void mouseReleaseEvent(QMouseEvent *event);  //重写鼠标点击事件

private slots:
    void onPlayerEnd(QMediaPlayer::MediaStatus status);//用于处理player播放结束的事件

    void on_closeButton_clicked();

    void on_minButton_clicked();

    void on_menuButton0_clicked();

    void on_menuButton1_clicked();

    void on_menuButton2_clicked();

    void on_playCloseButton_clicked();

    void on_sheetButton0_clicked();

    void on_sheetButton1_clicked();

    void on_sheetButton2_clicked();

    void on_sheetButton3_clicked();

    void on_sheetButton4_clicked();

    void on_sheetButton5_clicked();

    void on_sheetButton6_clicked();

    void on_sheetButton7_clicked();

    void on_songButton0_clicked();

    void on_songButton2_clicked();

    void on_songButton1_clicked();

    void on_songButton3_clicked();

    void on_songButton4_clicked();

    void on_songButton5_clicked();

    void on_songButton6_clicked();

    void on_songButton7_clicked();

    void on_songButton8_clicked();

    void on_songButton9_clicked();

    void on_pushButtonPause_clicked();

    void on_pageButtonLeft_clicked();

    void on_pageButtonRight_clicked();

    void on_pushButtonPlayMode_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
