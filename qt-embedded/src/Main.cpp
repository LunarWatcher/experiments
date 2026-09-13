#include <QApplication>
#include <QPushButton>
#include <QLabel>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QLayoutItem>
#include <QMainWindow>
#include <QSplitter>

#include <QWindow>

#include <stc/unix/Process.hpp>

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    auto window = new QMainWindow;
    window->setMinimumSize(480, 200);


    auto contentRoot = new QWidget;
    window->setCentralWidget(contentRoot);
    auto vbox = QVBoxLayout();
    contentRoot->setLayout(&vbox);
    vbox.setContentsMargins(0, 0, 0, 0);
    // contentRoot->setStyleSheet("background-color: black");

    // QHBoxLayout root(contentRoot);
    QSplitter root;
    vbox.addWidget(&root);
    root.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // root.setContentsMargins(0, 0, 0, 0);
    // root.setStyleSheet("background-color: black");

    QLabel l("owo");

    stc::Unix::Process p({"/usr/bin/konsole"});
    int64_t id;

    std::cin >> id;

    QWindow* embed = QWindow::fromWinId(id);
    embed->setFlag(Qt::FramelessWindowHint);

    QWidget* w = QWidget::createWindowContainer(embed);
    

    root.addWidget(&l);
    root.addWidget(w);

    window->show();

    return app.exec();
}
