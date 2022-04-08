#pragma once

#include <QBitmap>
#include <QPixmap>
#include <QPoint>
#include <QLine>
#include <vector>

// point in polygon
static bool pnpoly(int nvert, float* vertx, float* verty, float testx, float testy) {
	int i, j, c = 0;
	for (i = 0, j = nvert - 1; i < nvert; j = i++) {
		if (((verty[i] > testy) != (verty[j] > testy)) &&
			(testx < (vertx[j] - vertx[i]) * (testy - verty[i]) / (verty[j] - verty[i]) + vertx[i]))
			c = !c;
	}
	if (c)
		return true;
	else
		return false;
}

// returns true if the given line through rect
static bool containsLine(QRect r, QLine l) {
	int sampleSize = 100;
	int deltaX = (l.p2().x() - l.p1().x()) / sampleSize;
	int deltaY = (l.p2().y() - l.p1().y()) / sampleSize;

	if (r.contains(l.p1()))
		return true;
	else if (r.contains(l.p2()))
		return true;
	else for (int i = 1; i < sampleSize; i++)
		if (r.contains(QPoint(l.p1().x(), l.p1().y()) + i * QPoint(deltaX, deltaY)))
			return true;

	return false;
}

// merges the elements of two vectors together so that the values of one are appended to the end of the previous one
template <typename T>
static std::vector<T> appendVector(std::vector<T> a, std::vector<T> b) {
	a.insert(end(a), begin(b), end(b));
	return a;
}

// distance between points
template <typename T1, typename T2>
static float distance(const T1& p1, const T2& p2) {
	return std::sqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y()));
}

// returns true if the given 'str' ends with 'end'
static bool strends(std::string const& str, std::string const& end) {
	if (str.length() >= end.length())
		return (0 == str.compare(str.length() - end.length(), end.length(), end));
	else
		return false;
}

// loads texture with transparency using the given color as mask
static QPixmap loadTextureTransparent(const std::string& file, QColor mask_color = Qt::magenta) {
	QPixmap pixmap(file.c_str());
	pixmap.setMask(pixmap.createMaskFromColor(mask_color));
	return pixmap;
}

// loads texture from image file
static QPixmap loadTexture(const std::string file, QColor mask_color = Qt::magenta) {
	if (strends(file, ".bmp"))
		return loadTextureTransparent(file, mask_color);
	else
		return QPixmap(file.c_str());
} 
