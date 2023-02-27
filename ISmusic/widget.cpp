#include "widget.h"
#include "ui_widget.h"


bool isInitSongPack=false;//初始化歌单
int songSheetID=0;//聚焦歌单id
int songSheetPage=0;//歌单页码
int songSheetNumber=1;
vector<string> songInformation;//保存歌单信息
float globalvolume;
bool isLoop=false;


void getFiles(string path, vector<string>& files, const char* sType)//遍历文件
{
    //文件句柄
    long long hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    string p;
    hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo);
    if (hFile != -1)
    {
        do
        {
            //如果是目录,跳过不查找子文件
            //如果不是,加入列表
            if ((fileinfo.attrib & _A_SUBDIR))
            {

            }
            else
            {
                char* pName = fileinfo.name;
                char* pFind = strstr(pName,sType);//比对
                if (pFind != NULL)
                {
                    files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                }
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
}


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::FramelessWindowHint);//设置无边框


    if(!isInitSongPack){
        for(int i=0;i<8;i++){
            labelMenu[i]->setGeometry(0,0,0,0);
            labelMenu[i]->raise();
        }
        vector<string> files;

        TCHAR lpFileName[MAX_PATH];//获取安装路径
        char pathChar[MAX_PATH];
        GetModuleFileName(NULL, lpFileName, MAX_PATH);

        wcstombs(pathChar, lpFileName, MAX_PATH);//TCHAR->char
        string strPath(pathChar);//char->string

        for(;;){//处理路径字符串
            for(;;){
                int place=strPath.find('\\');
                if (place==-1)break;
                strPath.replace(place,1,"@");
            }
            for(;;){
                int place=strPath.find('@');
                if (place==-1)break;
                strPath.replace(place,1,"\\");
                //strPath.insert(place+1,"\\");
            }
            int placeinstr=strPath.find("ISmusic.exe");
            strPath.replace(placeinstr,11,"Music");
            break;
        }

        getFiles(strPath,files,".png");

        for(int i=0;i<(int)files.size();i++){//写入songsheet歌单类
            QString qstr=QString::fromStdString(files.at(i));
            songsheet.add(qstr,QImage(qstr));
        }

        for(int i=0;i<songsheet.Count();i++){//分配位置载入
            if(i<4)
            labelMenu[i]->setGeometry(20+i*200,120,160,160);
            else labelMenu[i]->setGeometry(20+(i-4)*200,320,160,160);
            labelMenu[i]->setPixmap(QPixmap::fromImage(songsheet.QimageMenu.at(i)));
        }

        songSheetClose();//初始化时执行关闭歌单函数
        songListClose();//初始化关闭歌曲表单

        FILE *fp=fopen("config.txt","rt");
        if(fp==NULL){
            setVolumeEx(0.1);//无配置文件初始化音量为0.1f
        }
        else{
            char c=fgetc(fp);
            char cc=fgetc(fp);
            int ccc;
            if(cc!='\0') ccc=10*((int)c-0x30)+(int)cc-0x30;
            else ccc=(int)c-0x30;
            float v=ccc/100.0f;
            setVolumeEx(v);
        }

        connect(player,&QMediaPlayer::mediaStatusChanged,this,&Widget::onPlayerEnd);
        //player->setLoops(-1);设置循环（已弃用）
        isInitSongPack=true;
    }

    Qt::WindowFlags m_flags = windowFlags();//置顶窗口！！！！！！！！！
    setWindowFlags(m_flags | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::songSheetShow(){
    for(int i=0;i<songsheet.Count();i++){
        if(i<4)
        labelMenu[i]->setGeometry(20+i*200,120,160,160);
        else labelMenu[i]->setGeometry(20+(i-4)*200,320,160,160);
    }

    ui->sheetButton0->setGeometry(20,120,160,160);
    ui->sheetButton1->setGeometry(220,120,160,160);
    ui->sheetButton2->setGeometry(420,120,160,160);
    ui->sheetButton3->setGeometry(620,120,160,160);
    ui->sheetButton4->setGeometry(20,320,160,160);
    ui->sheetButton5->setGeometry(220,320,160,160);
    ui->sheetButton6->setGeometry(420,320,160,160);
    ui->sheetButton7->setGeometry(620,320,160,160);

    ui->sheetButton0->raise();
    ui->sheetButton1->raise();
    ui->sheetButton2->raise();
    ui->sheetButton3->raise();
    ui->sheetButton4->raise();
    ui->sheetButton5->raise();
    ui->sheetButton6->raise();
    ui->sheetButton7->raise();
}


void Widget::songSheetClose(){
    for(int i=0;i<8;i++){
        labelMenu[i]->setGeometry(0,0,0,0);
    }
    ui->sheetButton0->setGeometry(0,0,0,0);
    ui->sheetButton1->setGeometry(0,0,0,0);
    ui->sheetButton2->setGeometry(0,0,0,0);
    ui->sheetButton3->setGeometry(0,0,0,0);
    ui->sheetButton4->setGeometry(0,0,0,0);
    ui->sheetButton5->setGeometry(0,0,0,0);
    ui->sheetButton6->setGeometry(0,0,0,0);
    ui->sheetButton7->setGeometry(0,0,0,0);
}


void Widget::songListShow(){
    ui->songButton0->setGeometry(0,103+0*35,800,35);
    ui->songButton1->setGeometry(0,103+1*35,800,35);
    ui->songButton2->setGeometry(0,103+2*35,800,35);
    ui->songButton3->setGeometry(0,103+3*35,800,35);
    ui->songButton4->setGeometry(0,103+4*35,800,35);
    ui->songButton5->setGeometry(0,103+5*35,800,35);
    ui->songButton6->setGeometry(0,103+6*35,800,35);
    ui->songButton7->setGeometry(0,103+7*35,800,35);
    ui->songButton8->setGeometry(0,103+8*35,800,35);
    ui->songButton9->setGeometry(0,103+9*35,800,35);

    ui->pageButtonLeft->setGeometry(15,460,34,34);
    ui->pageButtonRight->setGeometry(60,460,34,34);
}


void Widget::songListClose(){
    ui->songButton0->setGeometry(0,0,0,0);
    ui->songButton1->setGeometry(0,0,0,0);
    ui->songButton2->setGeometry(0,0,0,0);
    ui->songButton3->setGeometry(0,0,0,0);
    ui->songButton4->setGeometry(0,0,0,0);
    ui->songButton5->setGeometry(0,0,0,0);
    ui->songButton6->setGeometry(0,0,0,0);
    ui->songButton7->setGeometry(0,0,0,0);
    ui->songButton8->setGeometry(0,0,0,0);
    ui->songButton9->setGeometry(0,0,0,0);

    ui->pageButtonLeft->setGeometry(0,0,0,0);
    ui->pageButtonRight->setGeometry(0,0,0,0);
}


void Widget::NewsShow(){
    ui->newsLabel0->setGeometry(0,110,800,380);
    ui->newsLabel1->setGeometry(690,390,110,110);

}


void Widget::NewsClose(){
    ui->newsLabel0->setGeometry(0,0,0,0);
    ui->newsLabel1->setGeometry(0,0,0,0);
}


void Widget::on_closeButton_clicked()//关闭按钮
{
    this->close();
}


void Widget::on_minButton_clicked()//最小化
{
    this->showMinimized();
}


menuChoose mC;//选项单选择

void Widget::on_menuButton0_clicked()//首页
{
    int id=0;
    if(mC.canChange(id)){
        mC.tar=id;
        ui->menuUnderline->setGeometry(0,100,150,3);
        NewsShow();
        songSheetClose();
        songListClose();
    }
}


void Widget::on_menuButton1_clicked()//歌单
{
    int id=1;
    if(mC.canChange(id)){
        mC.tar=id;
        ui->menuUnderline->setGeometry(150,100,150,3);
        NewsClose();
        songSheetShow();
        songListClose();
    }
}

void Widget::on_menuButton2_clicked()//曲目
{
    int id=2;
    if(mC.canChange(id)){
        mC.tar=id;
        ui->menuUnderline->setGeometry(300,100,150,3);
        NewsClose();
        songSheetClose();
        songListShow();
    }
}


bool mouse_is_press;     //鼠标是否按下
QPoint  mouse_move_len;  //鼠标移动事件中，移动的距离
//重写鼠标按下事件
void Widget::mousePressEvent(QMouseEvent *event)
{

    mouse_move_len = event->globalPos() - this->pos();     //globalPos()是当前鼠标相对于屏幕的位置， pos()是窗口相对于屏幕的位置
    if(mouse_move_len.y()>=0&&mouse_move_len.y()<=50)mouse_is_press = true; //判断鼠标是否在标题位置
    else mouse_is_press=false;
}
//重写鼠标移动事件
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    //(event->buttons() && Qt::LeftButton)  //按下鼠标并且按下的是左键
    //获取鼠标移动中，移动后窗口在整个屏幕的坐标，将移动之前窗口的位置 move到 这个坐标（移动后窗口的位置）
    //通过事件event->globalPos()知道鼠标坐标，鼠标坐标减去鼠标相对于窗口位置，就是窗口在整个屏幕的坐标
    if (  mouse_is_press && (event->buttons() && Qt::LeftButton) &&
          (event->globalPos()-mouse_move_len).manhattanLength() > QApplication::startDragDistance()) //这句话其实就是：鼠标移动的距离大于最小可识别的距离
    {
        move(event->globalPos() - mouse_move_len);
        mouse_move_len = event->globalPos() - pos();
    }
}
//重写鼠标释放事件
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    mouse_is_press = false;
}


bool playIsShow=false;//播放栏是否最大化
void Widget::on_playCloseButton_clicked()//播放栏最大化,下拉菜单
{
    if(!playIsShow){//打开菜单
        ui->playBG->setGeometry(0,200,800,400);
        ui->playBG->raise();
        ui->playCloseButton->setGeometry(760,210,26,26);
        ui->playCloseButton->raise();
        ui->playBGtext->setGeometry(0,300,800,400);
        ui->playBGtext->raise();

        if(!songInformation.empty()){
            int count = songSheetNumber-1;
            if(songSheetNumber<=(int)(songInformation.size()/9)){
                QString info0=QString::fromLocal8Bit(songInformation.at(count*9));
                QString info1=QString::fromLocal8Bit(songInformation.at(count*9 + 1));
                QString info2=QString::fromLocal8Bit(songInformation.at(count*9 + 2));
                QString info3=QString::fromLocal8Bit(songInformation.at(count*9 + 3));
                QString info4=QString::fromLocal8Bit(songInformation.at(count*9 + 4));
                QString info5=QString::fromLocal8Bit(songInformation.at(count*9 + 5));
                QString info6=QString::fromLocal8Bit(songInformation.at(count*9 + 6));
                QString info7=QString::fromLocal8Bit(songInformation.at(count*9 + 7));
                QString info8=QString::fromLocal8Bit(songInformation.at(count*9 + 8));
                ui->playBGtext->setText("           标题           " + info0 +
                                        "           艺术家         " + info1 +
                                        "           唱片           " + info2 +
                                        "           唱片艺术家      " + info3 +
                                        "           作曲           " + info4 +
                                        "           年份           " + info5 +
                                        "           流派           " + info6 +
                                        "           编码           " + info7 +
                                        "           发布           " + info8
                                        );
            }
        }
        else{
            ui->playBGtext->setText("          未选中音乐");
        }



        //添加新的组件，这里是下拉菜单,上下Y轴差300


        ui->pushButtonPlayModeL->setGeometry(300,220,64,64);
        ui->pushButtonPlayModeL->raise();
        ui->pushButtonPlayMode->setGeometry(300,220,64,64);
        ui->pushButtonPlayMode->raise();
        ui->pushButtonLabel->setGeometry(370,220,64,64);
        ui->pushButtonLabel->raise();
        ui->pushButtonPause->setGeometry(370,220,64,64);
        ui->pushButtonPause->raise();

        playIsShow=true;
        return;
    }
    else{
        ui->playBG->setGeometry(0,500,800,400);
        ui->playBG->raise();
        ui->playCloseButton->setGeometry(760,510,26,26);
        ui->playCloseButton->raise();
        ui->playBGtext->setGeometry(0,600,800,400);
        ui->playBGtext->raise();


        ui->pushButtonPlayModeL->setGeometry(300,520,64,64);
        ui->pushButtonPlayModeL->raise();
        ui->pushButtonPlayMode->setGeometry(300,520,64,64);
        ui->pushButtonPlayMode->raise();
        ui->pushButtonLabel->setGeometry(370,520,64,64);
        ui->pushButtonLabel->raise();
        ui->pushButtonPause->setGeometry(370,520,64,64);
        ui->pushButtonPause->raise();
        playIsShow=false;
        return;
    }
}


void Widget::on_sheetButton0_clicked()
{
    if(songsheet.Count()>=1){
        loadSongSheet(0);
        songSheetID=0;
        Widget::on_menuButton2_clicked();
    }
}


void Widget::on_sheetButton1_clicked()
{
    if(songsheet.Count()>=2){
        loadSongSheet(1);
        songSheetID=1;
        Widget::on_menuButton2_clicked();
    }
}


void Widget::on_sheetButton2_clicked()
{
    if(songsheet.Count()>=3){
        loadSongSheet(2);
        songSheetID=2;
        Widget::on_menuButton2_clicked();
    }
}


void Widget::on_sheetButton3_clicked()
{
    if(songsheet.Count()>=4){
        loadSongSheet(3);
        songSheetID=3;
        Widget::on_menuButton2_clicked();
    }
}


void Widget::on_sheetButton4_clicked()
{
    if(songsheet.Count()>=5){
        loadSongSheet(4);
        songSheetID=4;
        Widget::on_menuButton2_clicked();
    }
}


void Widget::on_sheetButton5_clicked()
{
    if(songsheet.Count()>=6){
        loadSongSheet(5);
        songSheetID=5;
        Widget::on_menuButton2_clicked();
    }
}


void Widget::on_sheetButton6_clicked()
{
    if(songsheet.Count()>=7){
        loadSongSheet(6);
        songSheetID=6;
        Widget::on_menuButton2_clicked();
    }
}


void Widget::on_sheetButton7_clicked()
{
    if(songsheet.Count()>=8){
        loadSongSheet(7);
        songSheetID=7;
        Widget::on_menuButton2_clicked();
    }
}


void Widget::setVolumeEx(float volume){
    audioOutPut->setVolume(volume);
}


void Widget::startplay(int number){//播放音乐
    songSheetNumber=number;//具体播放的歌曲号码
    int id=songSheetID;
    string str = songsheet.QpathMenu.at(id).toStdString();//QString->string==str
    int tar = str.find(".png");

    string stradd="\\";
    stradd.append(to_string(number));//int->string
    stradd.append(".mp3");

    str.replace(tar,stradd.size(),stradd);
    QString s = QString::fromStdString(str);//string->QString==s

    player->setSource(QUrl::fromLocalFile(s));
    player->setAudioOutput(audioOutPut);

    //play->setPlaybackMode(QMediaPlaylist::Loop);
    player->play();
}


void Widget::onPlayerEnd(QMediaPlayer::MediaStatus status){//列表循环实现的槽函数，信号为

    if(status==6){
        if(isLoop){
            Widget::startplay(songSheetNumber);
            return;
        }
        if(songSheetNumber==(int)songInformation.size()/9){
            Widget::startplay(1);//到结尾re0
            return;
        }
        Widget::startplay(songSheetNumber+1);
    }
}


void Widget::showSongListPage(int page){//显示page的函数
    songSheetPage=page-1;
    ui->songButton0->setText("          >_<");
    ui->songButton1->setText("          >_<");
    ui->songButton2->setText("          >_<");
    ui->songButton3->setText("          >_<");
    ui->songButton4->setText("          >_<");
    ui->songButton5->setText("          >_<");
    ui->songButton6->setText("          >_<");
    ui->songButton7->setText("          >_<");
    ui->songButton8->setText("          >_<");
    ui->songButton9->setText("          >_<");
    if(songInformation.empty()) {
        return;
    }
    page--;

    int songcount=songInformation.size()/9;
    int n=songcount%10;
    bool isFinal=false;
    if(page+1>songcount/10)isFinal=true;


    QString line1;
    QString line2;
    int count;

    count=0*9+page*10*9;
    line1=QString::fromLocal8Bit("          "+songInformation.at(count));
    line2=QString::fromLocal8Bit("          "+songInformation.at(count+1).substr(0,songInformation.at(count+1).size()-1)+"    "+songInformation.at(count+2));
    ui->songButton0->setText(line1+line2);
    if(n==1&&isFinal)return;

    count=1*9+page*10*9;
    line1=QString::fromLocal8Bit("        >"+songInformation.at(count));
    line2=QString::fromLocal8Bit("          "+songInformation.at(count+1).substr(0,songInformation.at(count+1).size()-1)+"    "+songInformation.at(count+2));
    ui->songButton1->setText(line1+line2);
    if(n==2&&isFinal)return;

    count=2*9+page*10*9;
    line1=QString::fromLocal8Bit("          "+songInformation.at(count));
    line2=QString::fromLocal8Bit("          "+songInformation.at(count+1).substr(0,songInformation.at(count+1).size()-1)+"    "+songInformation.at(count+2));
    ui->songButton2->setText(line1+line2);
    if(n==3&&isFinal)return;

    count=3*9+page*10*9;
    line1=QString::fromLocal8Bit("        >"+songInformation.at(count));
    line2=QString::fromLocal8Bit("          "+songInformation.at(count+1).substr(0,songInformation.at(count+1).size()-1)+"    "+songInformation.at(count+2));
    ui->songButton3->setText(line1+line2);
    if(n==4&&isFinal)return;


    count=4*9+page*10*9;
    line1=QString::fromLocal8Bit("          "+songInformation.at(count));
    line2=QString::fromLocal8Bit("          "+songInformation.at(count+1).substr(0,songInformation.at(count+1).size()-1)+"    "+songInformation.at(count+2));
    ui->songButton4->setText(line1+line2);
    if(n==5&&isFinal)return;

    count=5*9+page*10*9;
    line1=QString::fromLocal8Bit("        >"+songInformation.at(count));
    line2=QString::fromLocal8Bit("          "+songInformation.at(count+1).substr(0,songInformation.at(count+1).size()-1)+"    "+songInformation.at(count+2));
    ui->songButton5->setText(line1+line2);
    if(n==6&&isFinal)return;

    count=6*9+page*10*9;
    line1=QString::fromLocal8Bit("          "+songInformation.at(count));
    line2=QString::fromLocal8Bit("          "+songInformation.at(count+1).substr(0,songInformation.at(count+1).size()-1)+"    "+songInformation.at(count+2));
    ui->songButton6->setText(line1+line2);
    if(n==7&&isFinal)return;

    count=7*9+page*10*9;
    line1=QString::fromLocal8Bit("        >"+songInformation.at(count));
    line2=QString::fromLocal8Bit("          "+songInformation.at(count+1).substr(0,songInformation.at(count+1).size()-1)+"    "+songInformation.at(count+2));
    ui->songButton7->setText(line1+line2);
    if(n==8&&isFinal)return;

    count=8*9+page*10*9;
    line1=QString::fromLocal8Bit("          "+songInformation.at(count));
    line2=QString::fromLocal8Bit("          "+songInformation.at(count+1).substr(0,songInformation.at(count+1).size()-1)+"    "+songInformation.at(count+2));
    ui->songButton8->setText(line1+line2);
    if(n==9&&isFinal)return;


    count=9*9+page*10*9;
    line1=QString::fromLocal8Bit("        >"+songInformation.at(count));
    line2=QString::fromLocal8Bit("          "+songInformation.at(count+1).substr(0,songInformation.at(count+1).size()-1)+"    "+songInformation.at(count+2));
    ui->songButton9->setText(line1+line2);
}


void Widget::loadSongSheet(int id){//载入歌单
    songSheetID=id;
    songInformation.clear();


    string str = songsheet.QpathMenu.at(id).toStdString();//QString->string==str
    int tar = str.find(".png");
    string stradd="\\list.txt";
    str.replace(tar,stradd.size(),stradd);
    int tar1;
    for(;;){
        tar1=str.find('\\');
        if(tar1==-1)break;
        str.replace(tar1,1,"/");
    }

    FILE *fp;
    const char *path;

    path = str.c_str();

    /*
    string debugstr(path);
    QString qdebugstring=QString::fromStdString(debugstr);//test
    qDebug("%s", qPrintable(qdebugstring));
    */

    fp=fopen(path,"rt");
    if(fp==NULL) {
        showSongListPage(1);
        fclose(fp);
        return;
    }

    char charp[100];
    for(;fgets(charp,100,fp)!=NULL;){
        string str(charp);
        songInformation.push_back(str);
    }
    fclose(fp);
    showSongListPage(1);
    songSheetPage=0;
}


int countListTar(int i){
    return songSheetPage*10+i;
}


void Widget::on_songButton0_clicked()
{
    Widget::startplay(countListTar(1));
}



void Widget::on_songButton1_clicked()
{
    Widget::startplay(countListTar(2));
}


void Widget::on_songButton2_clicked()
{
    Widget::startplay(countListTar(3));
}



void Widget::on_songButton3_clicked()
{
    Widget::startplay(countListTar(4));
}


void Widget::on_songButton4_clicked()
{
    Widget::startplay(countListTar(5));
}


void Widget::on_songButton5_clicked()
{
    Widget::startplay(countListTar(6));
}


void Widget::on_songButton6_clicked()
{
    Widget::startplay(countListTar(7));
}


void Widget::on_songButton7_clicked()
{
    Widget::startplay(countListTar(8));
}


void Widget::on_songButton8_clicked()
{
    Widget::startplay(countListTar(9));
}


void Widget::on_songButton9_clicked()
{
    Widget::startplay(countListTar(10));
}


bool isPause=false;
void Widget::on_pushButtonPause_clicked()//暂停播放
{
    if(player->playbackState()==0)return;
    if(player->playbackState()==1){
        QImage image(QString(":/Pictures/play-fill.png"));
        ui->pushButtonLabel->setPixmap(QPixmap::fromImage(image));
        player->pause();
        isPause=true;
    }
    else{
        QImage image(QString(":/Pictures/pause-fill.png"));
        ui->pushButtonLabel->setPixmap(QPixmap::fromImage(image));
        player->play();
        isPause=false;
    }
}


void Widget::on_pageButtonLeft_clicked()//翻页
{
    if(songSheetPage==0)return;
    songSheetPage--;
    showSongListPage(songSheetPage+1);
}


void Widget::on_pageButtonRight_clicked()
{
    songSheetPage++;
    int songcount=songInformation.size()/9;
    bool isFinal=false;
    if(songSheetPage>songcount/10)isFinal=true;

    if(isFinal){
        songSheetPage--;
        return;
    }

    ui->songButton0->setText("          >_<");
    ui->songButton1->setText("          >_<");
    ui->songButton2->setText("          >_<");
    ui->songButton3->setText("          >_<");
    ui->songButton4->setText("          >_<");
    ui->songButton5->setText("          >_<");
    ui->songButton6->setText("          >_<");
    ui->songButton7->setText("          >_<");
    ui->songButton8->setText("          >_<");
    ui->songButton9->setText("          >_<");

    showSongListPage(songSheetPage+1);
}


void Widget::on_pushButtonPlayMode_clicked()
{
    if(isLoop){
        isLoop=false;
        QImage image(QString(":/Pictures/repeat-2-fill.png"));
        ui->pushButtonPlayModeL->setPixmap(QPixmap::fromImage(image));
    }
    else{
        isLoop=true;
        QImage image(QString(":/Pictures/repeat-one-fill.png"));
        ui->pushButtonPlayModeL->setPixmap(QPixmap::fromImage(image));
    }
}

