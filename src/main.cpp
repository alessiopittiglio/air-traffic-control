#include <QApplication>
#include "Form.h"

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);

	(new Form())->show();

	return app.exec();
}
