#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "asyncimageloader.h"
#include "audioplayer.h"
#include "managelist.h"
#include "managetreelist.h"
#include "playqueue.h"
#include <QApplication>
#include <QCloseEvent>
#include <QCoreApplication>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsBlurEffect>
#include <QGraphicsColorizeEffect>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>
#include <QGraphicsScene>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QSettings>
#include <QShortcut>
#include <QSystemTrayIcon>
#include <QTime>
#include <QTreeView>
#include <QtMath>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void positionChanged(qint64 position);
    void showPicture(QImage coverimage);

protected:
    void closeEvent(QCloseEvent* event) override;

private slots:

    void on_playButton_clicked();

    void on_stopButton_clicked();

    // void on_progressSlider_sliderMoved(int position);

    void on_actionOpen_File_triggered();

    void on_volumeButton_clicked();

    void on_actionImport_Music_Resources_triggered();

    void on_actionSet_Appearance_triggered();

    void actionTrayGroupTriggered(QAction* action);

    //   void on_forwardButton_clicked();

    // void on_backwardButton_clicked();

    //  void on_modeButton_clicked();

    void startPlayingLive(QString url);

    void on_volumeSlider_valueChanged(int value);

    void on_treeListView_clicked(const QModelIndex& index);

    void on_treeListView_doubleClicked(const QModelIndex& index);

    void setTrayIconMenu(QList<QStringList> listdata);

private:
    Ui::MainWindow* ui;
    AudioPlayer player;
    AsyncImageLoader imageLoader;

    std::unique_ptr<PlayQueue> play_queue;

    std::unique_ptr<ManageList> music_list;

    std::unique_ptr<ManageTreeList> tree_play_list;

    // maybe add a favorite list
    std::unique_ptr<QSystemTrayIcon> tray_icon;

    std::unique_ptr<QMenu> music_list_menu;
    std::unique_ptr<QMenu> tray_menu;
    std::unique_ptr<QMenu> mode_menu;

    // menu actions
    std::unique_ptr<QAction> quit_action;
    std::unique_ptr<QAction> add_to_queue_action;
    std::unique_ptr<QAction> remove_from_list_action;
    std::unique_ptr<QAction> play_next_action;
    std::unique_ptr<QAction> play_prev_action;
    std::unique_ptr<QAction> play_action;
    std::unique_ptr<QAction> order_loop_action;
    std::unique_ptr<QAction> random_loop_action;
    std::unique_ptr<QAction> single_loop_action;

    // file settings
    QString default_file_dir;
    QString default_import_dir;
    QString cur_station_name;
    int last_position;

    // ui settings
    QPixmap default_music_image;

    // temporary state/ control variable
    bool volume_button_clicked;
    bool play_button_clicked;
    bool music_manually_stopped;
    float cached_volume;
    bool isDefaultPic;

    // usesr interaction settings
    void setShortCutsForAll();

    // signal and slot connection control
    void initConnect();

    // play control
    void startPlayingNew(QFileInfo file_info);
    inline void playListItem(QListWidgetItem* item);
    void addToPlayQueue();
    void removeFromPlayList();
    void setOrderLoopMode();
    void setSingleLoopMode();
    void setRandomLoopMode();
    void setDefaultPic();

    // ui update
    void showMusicInfo(QString key, QString value);
    void player_status_changed(AudioPlayer::States newstate);
    inline void updateItemSelectedUI(QListWidgetItem* cur_item, QListWidgetItem* new_item);

    // save/load settings
    void writeSettings();
    void readSettings();

    // manage menu actions
    void initActions();

    // set tool button
    void setModeButton();

    // manage context Menu
    void setMusicListMenu();
    void showMusicListMenu(const QPoint& pos);

    void setTrayIcon(const QIcon& appIcon);
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

    // helper functions
    float volumeConvert(int value);
    int setYesOrNoMessageBox(QString message, QString window_title);
};

#endif // MAINWINDOW_H
