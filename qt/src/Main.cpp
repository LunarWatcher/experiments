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

#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QStandardItem>

#include <filesystem>

std::vector<std::string> mockData = {
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
    "Test 1", "Test 2", "Test 3", "Test 4",
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    std::string path = "../../../dotfiles/assets/eepyfox.png";

    if (!std::filesystem::exists(path)) {
        throw std::runtime_error("Good job idiot");
    }

    auto window = new QMainWindow;
    window->setMinimumSize(480, 200);


    auto contentRoot = new QWidget;
    window->setCentralWidget(contentRoot);
    auto vbox = QVBoxLayout();
    contentRoot->setLayout(&vbox);
    vbox.setContentsMargins(0, 0, 0, 0);
    // contentRoot->setStyleSheet("background-color: black");

    QIcon eepyfox(path.c_str());

    // QHBoxLayout root(contentRoot);
    QSplitter root;
    vbox.addWidget(&root);
    root.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // root.setContentsMargins(0, 0, 0, 0);
    // root.setStyleSheet("background-color: black");

    QTreeView w;
    w.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // w.setContentsMargins(0, 0, 0, 0);
    
    // w.setHeaderLabel("Tests");
    // w.setMinimumWidth(300);

    QStandardItemModel model;
    QSortFilterProxyModel filter;
    filter.setSourceModel(&model);
    w.setModel(&filter);

    for (auto& it : mockData) {

        QList<QStandardItem*> items;
        auto item = new QStandardItem(it.c_str());
        item->setIcon(eepyfox);

        model.appendRow(item);
        
        while (rand() > RAND_MAX / 2) {
            auto subItem = new QStandardItem("Child item");
            item->appendRow(subItem);
        }
    }

    QGridLayout content;
    content.setContentsMargins(0, 0, 0, 0);
    auto text = new QLabel("Hewwo");
    text->setTextInteractionFlags(Qt::TextSelectableByMouse);
    content.addWidget(text, 0, 0, 2, 1);

    auto container = new QWidget;
    container->setLayout(&content);

    auto btn = new QPushButton("Delete item") ;
    // QObject::connect(btn, &QPushButton::released, [&]() {
    //     auto* it = items.back();
    //     items.pop_back();
    //     // `delete` appears to be enough?
    //     // w.removeItemWidget(it, 0);
    //     delete it;
    // });
    content.addWidget(btn, 1, 0, 1, 1);

    root.addWidget(&w);
    root.addWidget(container);

    filter.setFilterFixedString("4");

    window->show();

    return app.exec();
}
