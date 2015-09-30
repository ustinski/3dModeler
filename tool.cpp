#include "tool.h"
#include "mainwindow.h"


Tool::Tool(MainWindow *mainWindow, QString buttonText)
{
    _mainWindow = mainWindow;
    button = new QPushButton(buttonText);
    button->setMaximumWidth(70);
    button->setCheckable(true);
    connect(button, SIGNAL(clicked(bool)), this, SLOT(handleClick(
                                                          bool)));
    _activeWidget = _mainWindow->getActiveWidget();
}

void Tool::setActive(bool value)
{
    _isActive = value;
    button->setChecked(value);
}

void Tool::handleClick(bool pressed)
{
   // cerr << pressed << '\n';
    if(pressed) _mainWindow->setActiveTool(this);
    else button->setChecked(true);
}
