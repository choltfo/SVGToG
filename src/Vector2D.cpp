#include "Vector2D.hpp"

Vector2D::Vector2D(double nX, double nY) {
    SetXY(nX, nY);
}

void Vector2D::SetXY(double nX = 0.0, double nY = 0.0) {
    x = nX;
    y = nY;
}

void Vector2D::SetDM(double dir, double mag) {
    x = cos(dir) * mag;
    y = sin(dir) * mag;
}

double Vector2D::GetDir() {
    return atan2(y, x);
}

double Vector2D::GetMag() {
    return hypot(x, y);
}

double Vector2D::GetSqrMag() {
	return (x*x) + (y*y);
}

// Returns the dot product of this and b.
double Vector2D::DotProduct(Vector2D b) {
	return (x*b.x)+(y*b.y);
}

// Return the magnitude of b projected onto this.
double Vector2D::Project(Vector2D b) {
	return DotProduct(b)/GetMag();
}

void Vector2D::Normalize() {
	double mag = GetMag();
	x = x/mag;
	y = y/mag;
}

Vector2D Vector2D::Normal() {
	return Vector2D(y,x)/GetMag();
}


Vector2D& Vector2D::operator+=(const Vector2D& v) {
    x += v.x;
    y += v.y;
    return (*this);
}

Vector2D& Vector2D::operator-=(const Vector2D& v) {
    x -= v.x;
    y -= v.y;
    return (*this);
}

Vector2D& Vector2D::operator*=(double t) {
    x *= t;
    y *= t;
    return (*this);
}

Vector2D& Vector2D::operator/=(double t) {
    double f = 1.0 / t;
    x *= f;
    y *= f;
    return (*this);
}

Vector2D& Vector2D::operator&=(const Vector2D& v) {
    x *= v.x;
    y *= v.y;
    return (*this);
}

Vector2D Vector2D::operator-(void) const {
    return (Vector2D(-x, -y));
}

Vector2D Vector2D::operator+(const Vector2D& v) const {
    return (Vector2D(x + v.x, y + v.y));
}

Vector2D Vector2D::operator-(const Vector2D& v) const {
    return (Vector2D(x - v.x, y - v.y));
}

Vector2D Vector2D::operator*(double t) const {
    return (Vector2D(x * t, y * t));
}

Vector2D Vector2D::operator/(double t) const {
    double f = 1.0 / t;
    return (Vector2D(x * f, y * f));
}

Vector2D Vector2D::operator&(const Vector2D& v) const {
    return Vector2D(x * v.x, y * v.y);
}

bool Vector2D::operator==(const Vector2D& v) const {
    return ((x == v.x) && (y == v.y));
}

bool Vector2D::operator!=(const Vector2D& v) const {
    return ((x != v.x) || (y != v.y));
}

Vector2D& Vector2D::Rotate(double angle) {
    double s = sin(angle);
    double c = cos(angle);

    double nx = c * x - s * y;
    double ny = s * x + c * y;

    x = nx;
    y = ny;

    return (*this);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

std::istream& operator>>(std::istream& is, Vector2D& v) {
    double tX, tY;
    is >> tX;
    is.ignore(100,',');
	is >> tY;
    v.SetXY(tX, tY);
    return is;
}
