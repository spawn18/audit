#include "Editor.hpp"

#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    try
    {
        QApplication app(argc, argv);
        Editor editor;
        editor.show();
        return app.exec();
    }
    catch(...)
    {
        QMessageBox::information(nullptr, "Error", "Unknown error occured");
    }

    return EXIT_FAILURE;
}
