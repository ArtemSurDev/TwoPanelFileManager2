#include "MainWindow.h"
#include "FileOperationsFacade.h"
#include "PathRestrictions.h"
#include <QHBoxLayout>
#include <QWidget>
#include <QKeyEvent>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QLabel>
#include <QFileInfo>

MainWindow::MainWindow() {
    setupUI();
    
    FileOperationsFacade::getInstance()->setOnFileSystemChanged([this]() {
        mediator->refreshAllPanels();
    });
}

MainWindow::~MainWindow() {
    FileOperationsFacade::getInstance()->setOnFileSystemChanged({});
    for (auto cmd : commandHistory) {
        delete cmd;
    }
}

void MainWindow::setupUI() {
    setWindowTitle("Two Panel File Manager");
    auto central = new QWidget();
    auto layout = new QHBoxLayout();
    leftPanel = new FilePanel();
    rightPanel = new FilePanel();
    leftPanel->setPath(QDir::homePath());
    rightPanel->setPath(QDir::homePath());

    
    mediator = new PanelMediator(leftPanel, rightPanel);

    
    leftPanel->setActive(true);
    rightPanel->setActive(false);
    leftPanel->setFocus();

    
    connect(leftPanel, &FilePanel::enterPressed, [this]() { mediator->handleEnter(); });
    connect(rightPanel, &FilePanel::enterPressed, [this]() { mediator->handleEnter(); });

    
    connect(leftPanel, &FilePanel::backspacePressed, [this]() { mediator->handleBackspace(); });
    connect(rightPanel, &FilePanel::backspacePressed, [this]() { mediator->handleBackspace(); });

    
    connect(leftPanel, &FilePanel::tabPressed, [this]() { mediator->switchActivePanel(); });
    connect(rightPanel, &FilePanel::tabPressed, [this]() { mediator->switchActivePanel(); });

    
    connect(leftPanel, &FilePanel::functionKeyPressed, [this](int key) { handleFunctionKey(key); });
    connect(rightPanel, &FilePanel::functionKeyPressed, [this](int key) { handleFunctionKey(key); });

    layout->addWidget(leftPanel);
    layout->addWidget(rightPanel);
    central->setLayout(layout);
    setCentralWidget(central);

    auto hintLabel = new QLabel("Tab=Switch Panel | Return=Open | Delete=Up | F2=Rename | F5=Copy | F6=Move | F7=New Folder | F8=Delete | F9=Paste");
    statusBar()->addPermanentWidget(hintLabel);
    statusBar()->showMessage("Ready");
}


void MainWindow::executeCommand(FileCommand* command) {
    if (command->execute()) {
        commandHistory.append(command);
        statusBar()->showMessage(command->getDescription(), 2000);
    } else {
        QMessageBox::critical(this, "Error", FileOperationsFacade::getInstance()->getLastError());
        delete command;
    }
}

void MainWindow::handleFunctionKey(int key) {
    FilePanel* active = mediator->getActivePanel();
    FilePanel* target = mediator->getInactivePanel();
    QString selected = active->getSelectedItem();

    auto showRestrictedWarning = [this]() {
        QMessageBox::warning(this, "Restricted", "Access to system folders is blocked.");
    };

    auto resolveTargetDir = [](FilePanel* panel) {
        QString base = panel->getCurrentPath();
        QString candidateItem = panel->getSelectedItem();
        if (candidateItem.isEmpty() || candidateItem == "..") {
            return base;
        }
        QString candidatePath = base + "/" + candidateItem;
        QFileInfo info(candidatePath);
        return info.isDir() ? candidatePath : base;
    };

    if (selected.isEmpty() && key != Qt::Key_F7 && key != Qt::Key_F9) {
        return;
    }

    if (selected == ".." && key != Qt::Key_F7 && key != Qt::Key_F9) {
        return;
    }

    QString srcPath = active->getCurrentPath() + "/" + selected;

    switch (key) {
        case Qt::Key_F5: {
            if (isRestrictedPath(srcPath)) {
                showRestrictedWarning();
                return;
            }
            if (!QFileInfo::exists(srcPath)) {
                QMessageBox::warning(this, "Copy", "Selected item does not exist.");
                return;
            }
            clipboardSource = srcPath;
            clipboardHasItem = true;
            statusBar()->showMessage("Copied to buffer: " + QFileInfo(srcPath).fileName(), 2000);
            break;
        }
        case Qt::Key_F6: {
            if (isRestrictedPath(srcPath)) {
                showRestrictedWarning();
                return;
            }
            QString destDir = resolveTargetDir(target);
            QString dstPath = destDir + "/" + QFileInfo(srcPath).fileName();
            if (isRestrictedPath(dstPath)) {
                showRestrictedWarning();
                return;
            }
            executeCommand(new MoveCommand(srcPath, dstPath));
            break;
        }
        case Qt::Key_F9: {
            if (!clipboardHasItem || clipboardSource.isEmpty()) {
                QMessageBox::information(this, "Paste", "Buffer is empty. Use F5 to copy first.");
                return;
            }
            if (isRestrictedPath(clipboardSource)) {
                showRestrictedWarning();
                return;
            }
            QString destDir = resolveTargetDir(active);
            QString dstPath = destDir + "/" + QFileInfo(clipboardSource).fileName();
            if (isRestrictedPath(dstPath)) {
                showRestrictedWarning();
                return;
            }
            executeCommand(new CopyCommand(clipboardSource, dstPath));
            break;
        }
        case Qt::Key_F7: {
            if (isRestrictedPath(active->getCurrentPath())) {
                showRestrictedWarning();
                return;
            }
            bool ok;
            QString name = QInputDialog::getText(this, "New Folder", "Folder name:", QLineEdit::Normal, "", &ok);
            if (ok && !name.isEmpty()) {
                QString newPath = active->getCurrentPath() + "/" + name;
                if (isRestrictedPath(newPath)) {
                    showRestrictedWarning();
                    return;
                }
                executeCommand(new CreateDirCommand(newPath));
            }
            break;
        }
        case Qt::Key_F8:
            if (isRestrictedPath(srcPath)) {
                showRestrictedWarning();
                return;
            }
            if (QMessageBox::question(this, "Delete", "Delete " + selected + "?") == QMessageBox::Yes) {
                executeCommand(new DeleteCommand(srcPath));
            }
            break;
        case Qt::Key_F2: {
            if (isRestrictedPath(srcPath)) {
                showRestrictedWarning();
                return;
            }
            bool ok;
            QString newName = QInputDialog::getText(this, "Rename", "New name:", QLineEdit::Normal, selected, &ok);
            if (ok && !newName.isEmpty()) {
                QString newPath = active->getCurrentPath() + "/" + newName;
                if (isRestrictedPath(newPath)) {
                    showRestrictedWarning();
                    return;
                }
                executeCommand(new RenameCommand(srcPath, newPath));
            }
            break;
        }
        default:
            break;
    }
}

