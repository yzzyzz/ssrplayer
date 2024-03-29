#include "mainwindow.h"
#include "HttpClient.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QStatusBar>
#include <asyncimageloader.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , volume_button_clicked(false)
    , play_button_clicked(false)
    , music_manually_stopped(false)
    , cached_volume(50.0f)
{
    ui->setupUi(this);
    // init widgetlist first since we need to read settings
    //  music_list = std::unique_ptr<ManageList>(new ManageList(ui->musicList));

    tree_play_list = std::unique_ptr<ManageTreeList>(new ManageTreeList(ui->treeListView));

    // load settings
    // you should read settings after ui is set up
    // since you may want to initialize some components in ui
    readSettings();
    // player initialization
    // play_queue = std::unique_ptr<PlayQueue>(new PlayQueue(ui->musicList));
    // play_obj =
    player.setVolume(50.0);
    // set key shortcuts
    setShortCutsForAll();
    // set Menu & actions
    initActions();
    // initMenus();
    // ui setting
    ui->volumeSlider->setValue(50);
    ui->volumeDisplay->setText(QString::number(ui->volumeSlider->value()) + "%");

    auto appIcon = QIcon(":icons/res/musical_notec.png");
    setTrayIcon(appIcon);
    setWindowIcon(appIcon);
    default_music_image = QPixmap(":icons/res/defaultcover.png");
    // this->setProperty("windowOpacity", 1.0);

    // other ui componet settings
    ui->playButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    setDefaultPic();
    // setAttribute(Qt::WA_TranslucentBackground, true);

    // setStyleSheet("QMainWindow::titleBar { background-color: red; }");
    //  阴影效果
    // QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect;
    //  // 阴影色，透明度
    // shadowEffect->setColor(QColor(100, 100, 100));
    // shadowEffect->setBlurRadius(40); // 阴影模糊半径
    //  shadowEffect->setOffset(10); // 阴影的偏移值

    // setGraphicsEffect(shadowEffect);
    // setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    // setWindowOpacity(0.9);
    // setWindowFlags(Qt::FramelessWindowHint); // 设置无边框
    // set stylesheet
    // signal&slot connecttion
    initConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initConnect()
{
    // connect audio_player's state with GUI
    connect(&imageLoader, &AsyncImageLoader::imageLoaded, this, &MainWindow::showPicture);
    connect(
        &player,
        &AudioPlayer::stateChanged,
        this,
        &MainWindow::player_status_changed);
    connect(
        &player,
        &AudioPlayer::metadataChanged,
        this,
        &MainWindow::showMusicInfo);
    // if not using auto connection by ui designer, use below connection
    // connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::on_playButton_clicked); //...
}

void MainWindow::setShortCutsForAll()
{
    ui->playButton->setShortcut(QKeySequence("Space"));
    ui->stopButton->setShortcut(QKeySequence("Ctrl+C"));
    ui->forwardButton->setShortcut(QKeySequence("Ctrl+Right"));
    ui->volumeButton->setShortcut(QKeySequence("Ctrl+O"));
}

// protected
void MainWindow::closeEvent(QCloseEvent* event)
{
    event->accept();
}

// private slot

void MainWindow::on_actionOpen_File_triggered()
{
    QString prompt = "Please Select Your Audio File";
    QString file_format { "ALL (*.mp3 *.wav *.flac *.m4a);;"
                          "MP3 (*.mp3);;WAV (*.wav);;FLAC (*.flac);;M4A (*.m4a)" };
    QString file_dir = default_file_dir == "" ? qApp->applicationDirPath() : default_file_dir;
    QString file_path = QFileDialog::getOpenFileName(this, prompt, file_dir, file_format);

    QFileInfo file_info(file_path);
    if (file_info.absolutePath() != "")
        default_file_dir = file_info.absolutePath();
    if (file_info.fileName() != "")
        startPlayingNew(file_info);
}

void MainWindow::on_actionImport_Music_Resources_triggered()
{
    QString prompt = "Please Select Your Audio File Directory";
    QString import_dir = QFileDialog::getExistingDirectory(this, prompt,
        default_import_dir, QFileDialog::DontUseNativeDialog);
    QDir dir(import_dir);
    // music_list->importToList(dir, ".*.(flac|mp3|wav|m4a)");
    default_import_dir = import_dir;
}

void MainWindow::on_actionSet_Appearance_triggered()
{
    this->setProperty("windowOpacity", 1.0);
}

void MainWindow::on_playButton_clicked()
{
    play_button_clicked = !play_button_clicked;
    music_manually_stopped = false;
    if (play_button_clicked) {
        player.playpause();
        ui->playButton->setIcon(QIcon(":/icons/res/pause_w.png"));
        play_action->setIcon(QIcon(":icons/res/pause.png"));
        play_action->setText("&Pause");
    } else {
        player.playpause();
        ui->playButton->setIcon(QIcon(":/icons/res/play_w.png"));
        play_action->setIcon(QIcon(":icons/res/play.png"));
        play_action->setText("&Play");
    }
}

void MainWindow::on_stopButton_clicked()
{
    // when state changes to stop, <music_manually_stopped>'ll be checked, set it before stop()
    music_manually_stopped = true;
    play_button_clicked = false;
    player.stop();
    ui->playButton->setIcon(QIcon(":/icons/res/play_w.png"));
}

void MainWindow::on_volumeButton_clicked()
{
    volume_button_clicked = !volume_button_clicked;
    if (volume_button_clicked) {
        cached_volume = player.volume();
        player.setVolume(0);
        ui->volumeButton->setIcon(QIcon(":/icons/res/volume_mute_w.png"));
        ui->volumeDisplay->setText("0%");
    } else {
        player.setVolume(cached_volume);
        ui->volumeButton->setIcon(QIcon(":/icons/res/volume_w.png"));
        ui->volumeDisplay->setText(QString::number(static_cast<int>(cached_volume)) + "%");
    }
}

// void MainWindow::on_musicList_itemDoubleClicked(QListWidgetItem* item)
// {
//     int new_item_row = music_list->getRow(item);
//     play_queue->updatePlayingQueue(new_item_row);
//     play_queue->setCurrent_item_row(new_item_row);

//     auto cur_play_mode = play_queue->getPlayMode();
//     play_queue->setPlayMode(PQ::PlayMode::Order);
//     ui->forwardButton->click();
//     play_queue->setPlayMode(cur_play_mode);
// }

// void MainWindow::on_forwardButton_clicked()
// {
//     auto* current_item = play_queue->current();
//     auto* next_item = play_queue->next();
//     if (!next_item || !current_item)
//         return;
//     playListItem(next_item);
//     music_list->updateUIonItemChange(current_item, next_item);
// }

// void MainWindow::on_backwardButton_clicked()
// {
//     auto* current_item = play_queue->current();
//     auto* pre_item = play_queue->previous();
//     if (!pre_item || !current_item)
//         return;
//     playListItem(pre_item);
//     music_list->updateUIonItemChange(current_item, pre_item);
// }

// void MainWindow::on_modeButton_clicked()
// {
//     // ui->modeButton->showMenu();
// }

// play control
void MainWindow::startPlayingNew(QFileInfo file_info)
{ // do not modify this function, it's under inspection
    ui->stopButton->click();
    ; // audio_player->setSource(QUrl::fromLocalFile(file_info.absoluteFilePath()));
    if (play_button_clicked)
        ui->playButton->click();
    ui->playButton->click();
    // cur_file_info = file_info;
}

void MainWindow::startPlayingLive(QString urlString)
{
    // do not modify this function, it's under inspection
    ui->stopButton->click();
    player.open(urlString.toUtf8());
    if (play_button_clicked)
        ui->playButton->click();
    ui->playButton->click();
    ui->stationName->setText(cur_station_name);
    // qDebug() << "startPlayingLive \n\n\n\nvoice numver " << player.volume();
}

void MainWindow::addToPlayQueue()
{
    play_queue->addToUserQueue();
}

void MainWindow::removeFromPlayList()
{
    ;
}

void MainWindow::setOrderLoopMode()
{
    // ui->modeButton->setIcon(QIcon(":icons/res/loopmodec.png"));
    play_queue->setPlayMode(PQ::PlayMode::Order);
}

void MainWindow::setSingleLoopMode()
{
    // ui->modeButton->setIcon(QIcon(":icons/res/loopb.png"));
    play_queue->setPlayMode(PQ::PlayMode::Single);
}

void MainWindow::setRandomLoopMode()
{
    // ui->modeButton->setIcon(QIcon(":icons/res/shuffle.png"));
    play_queue->setPlayMode(PQ::PlayMode::Shuffle);
}

// ui update
void MainWindow::showMusicInfo(QString key, QString value)
{
    QJsonDocument json_data = QJsonDocument::fromJson(value.toUtf8());
    QJsonObject jsonMetadata = json_data.object();

    QString setTex = "";
    if (jsonMetadata.contains("icy-title")) {
        QString sAudioTitle = jsonMetadata.value("icy-title").toString();
        setTex = sAudioTitle;
        setTex.replace("-", "\n").replace(" ", "");
        tray_menu->setTitle(setTex);

        QString PicUrl1 = "http://gz.999887.xyz/getmusicpic.php?title=" + sAudioTitle + "&pictype=hires";
        HttpClient(PicUrl1)
            .success([this](const QString& response) {
                qDebug() << response;
                QJsonObject jsonPicRet = QJsonDocument::fromJson(response.toUtf8()).object();
                if (jsonPicRet.contains("picurl") && jsonPicRet.value("picurl").toString().length() > 6) {
                    QUrl imageUrl(jsonPicRet.value("picurl").toString());
                    imageLoader.loadImage(imageUrl);
                } else {
                    if (!isDefaultPic) {
                        this->setDefaultPic();
                    }
                }
            })
            .get();
        // QString PicUrl1 =  "http://gz.999887.xyz/getmusicpic.php?title="+sAudioTitle+"&pictype=hires";
        // http://gz.999887.xyz/getmusicpic.php?title=%E9%82%93%E4%B8%BD%E5%90%9B-%E7%94%9C%E8%9C%9C%E8%9C%9C&pictype=hires
    } else {
        if (!isDefaultPic) {
            this->setDefaultPic();
        }
    }
    tray_icon->setToolTip(setTex + "...");
    ui->musicNameDisplay->setText(setTex);
}

// save/load settings
void MainWindow::writeSettings()
{
    QSettings settings;
    settings.setValue("file/default_dir", default_file_dir);
    settings.setValue("file/default_import_dir", default_import_dir);
    settings.setValue("file/last_volume_pos", last_position);
    // music_list->saveList(settings, "musicList");
}

void MainWindow::readSettings()
{
    // music_list->importLiveList();
    tree_play_list->importToList("");
    // QSettings settings;
    // default_file_dir = settings.value("file/default_dir", "").toString();
    // default_import_dir = settings.value("file/default_import_dir", default_file_dir).toString();
    // last_position = settings.value("file/last_volume_pos", 25).toInt();
    // music_list->loadList(settings, "musicList");
}

void MainWindow::initActions()
{
    quit_action = std::unique_ptr<QAction>(new QAction("&Quit", this));
    quit_action->setIcon(QIcon(":icons/res/quit.png"));
    connect(quit_action.get(), &QAction::triggered, qApp, &QCoreApplication::quit);
    add_to_queue_action = std::unique_ptr<QAction>(new QAction("&Add To Play Queue", this));
    add_to_queue_action->setIcon(QIcon(":icons/res/add_p1.png"));
    connect(add_to_queue_action.get(), &QAction::triggered, this, &MainWindow::addToPlayQueue);

    remove_from_list_action = std::unique_ptr<QAction>(new QAction("&Remove From List", this));
    remove_from_list_action->setIcon(QIcon(":icons/res/remove_cyan1.png"));
    connect(remove_from_list_action.get(), &QAction::triggered, this, &MainWindow::removeFromPlayList);

    play_next_action = std::unique_ptr<QAction>(new QAction("&Play Next", this));
    play_next_action->setIcon(QIcon(":icons/res/next.png"));
    connect(play_next_action.get(), &QAction::triggered, this, &MainWindow::on_playButton_clicked);

    play_action = std::unique_ptr<QAction>(new QAction(("&Play"), this));
    play_action->setIcon(QIcon(":icons/res/play.png"));
    connect(play_action.get(), &QAction::triggered, this, &MainWindow::on_playButton_clicked);
}

void MainWindow::setTrayIcon(const QIcon& appIcon)
{
    tray_icon = std::unique_ptr<QSystemTrayIcon>(new QSystemTrayIcon(this));
    tray_icon->setIcon(appIcon);
    tray_icon->setToolTip("Not Playing Now");
    setTrayIconMenu();
    tray_icon->show();
    // set System Tray Icon Interaction
    connect(tray_icon.get(), &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
    // QStatusBar* statusBar = new QStatusBar(this);
    // setStatusBar(statusBar);
    // // 在状态栏中显示文字
    // statusBar->showMessage("中消息");
}

void MainWindow::setTrayIconMenu()
{
    // init & set tray menu
    tray_menu = std::unique_ptr<QMenu>(new QMenu(this));
    // tray_menu->setWindowFlag(Qt::FramelessWindowHint);
    //  tray_menu->setAttribute(Qt::WA_TranslucentBackground);
    tray_menu->addAction(play_action.get());
    tray_menu->addAction(quit_action.get());
    // tray_menu->toNSMenu();
    tray_icon->setContextMenu(tray_menu.get());
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        this->tray_icon->showMessage("Playing Now", "To Be Done", QIcon(":icons/res/star_shining.png"));
        break;
    default:;
    }
}

// helper functions
float MainWindow::volumeConvert(int value)
{ // using exponential on volume percent to produce linear changes in perceived loudness
    if (value < 0)
        return 0.0f;
    if (value > 100)
        return 100.0f;
    float converted_volume = static_cast<float>(value) / 1.20f;
    // float converted_volume = (qExp<float>(percent) - 1.0f) / (qExp<float>(1.0f) - 1.0f);
    return converted_volume;
}

static QImage applyEffectToImage(QImage src, QGraphicsEffect* effect, int extent = 0)
{
    if (src.isNull())
        return QImage(); // No need to do anything else!
    if (!effect)
        return src; // No need to do anything else!
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    QImage res(src.size() + QSize(extent * 2, extent * 2), QImage::Format_ARGB32);
    res.fill(Qt::transparent);
    QPainter ptr(&res);
    scene.render(&ptr, QRectF(), QRectF(-extent, -extent, src.width() + extent * 2, src.height() + extent * 2));
    return res;
}

void MainWindow::showPicture(QImage coverimage)
{
    qDebug() << "Image loaded with size:" << coverimage.size();
    if (!coverimage.isNull()) {
        // 设置背景框
        QGraphicsBlurEffect* blur = new QGraphicsBlurEffect;
        blur->setBlurRadius(50);
        QImage result = applyEffectToImage(coverimage, blur);
        QPalette palette;
        palette.setBrush(backgroundRole(), QPixmap::fromImage(result).scaled(this->size()));
        setPalette(palette);
        // 设置图片框
        QPixmap pixmap;
        pixmap.convertFromImage(coverimage);
        ui->musicGraphics->setPixmap(pixmap.scaled(ui->musicGraphics->size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        isDefaultPic = false;
    } else {
        setDefaultPic();
    }
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    player.setVolume(volumeConvert(value));
    ui->volumeDisplay->setText(QString::number(value) + "%");
    last_position = value;
}

void MainWindow::player_status_changed(AudioPlayer::States newstate)
{
    switch (newstate) {
    case AudioPlayer::States::PLAYING:
        ui->playButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
        break;
    case AudioPlayer::States::PAUSED:
        ui->playButton->setEnabled(true);
        ui->stopButton->setEnabled(true);
        break;
    case AudioPlayer::States::STOPPED:
        ui->playButton->setEnabled(true);
        ui->stopButton->setEnabled(false);
        if (!music_manually_stopped) {
            ui->forwardButton->click();
        }
        break;
    default:
        break;
    }
}

void MainWindow::setDefaultPic()
{
    QGraphicsBlurEffect* blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(50);
    QImage result = applyEffectToImage(QImage(":icons/res/defaultcover.png"), blur);
    QPalette palette;
    palette.setBrush(backgroundRole(), QPixmap::fromImage(result).scaled(this->size()));
    setPalette(palette);
    ui->musicGraphics->setPixmap(QPixmap(":icons/res/defaultcover.png"));
    isDefaultPic = true;
}

void MainWindow::on_treeListView_clicked(const QModelIndex& index)
{
    if (index.parent().isValid()) {
        QString file_path = index.data(Qt::UserRole).toString();
        cur_station_name = index.data(Qt::DisplayRole).toString();
        startPlayingLive(file_path);
    } else {
        qDebug() << " no parent";
        tree_play_list->switchExpand(index);
    }
}

void MainWindow::on_treeListView_doubleClicked(const QModelIndex& index)
{
    on_treeListView_clicked(index);
}
