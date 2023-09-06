#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

class Point3D {
public:
    double x, y, z;

    Point3D(double x, double y, double z) : x(x), y(y), z(z) {}
};

class Vector3D {
public:
    double x, y, z;

    Vector3D(double x, double y, double z) : x(x), y(y), z(z) {}
};

class Curve {
public:
    virtual ~Curve() = default;

    virtual Point3D getPointAtT(double t) const = 0;
    virtual Vector3D getDerivativeAtT(double t) const = 0;
    virtual std::string getType() const = 0;
};

class Circle : public Curve {
private:
    double radius;

public:
    Circle(double radius) : radius(radius) {}
    
    Point3D getPointAtT(double t) const override {
        double x = radius * cos(t);
        double y = radius * sin(t);
        double z = 0.0;
        return Point3D(x, y, z);
    }

    Vector3D getDerivativeAtT(double t) const override {
        double dx_dt = -radius * sin(t);
        double dy_dt = radius * cos(t);
        double dz_dt = 0.0;
        return Vector3D(dx_dt, dy_dt, dz_dt);
    }

    std::string getType() const override {
        return "Circle";
    }

    std::double_t getRadius() const {
        return radius;
    }
};

class Ellipse : public Curve {
private:
    double radiusX;
    double radiusY;

public:
    Ellipse(double radiusX, double radiusY) : radiusX(radiusX), radiusY(radiusY) {}
    std::double_t getRadius() const {
        return radiusX, radiusY;
    }
    Point3D getPointAtT(double t) const override {
        double x = radiusX * cos(t);
        double y = radiusY * sin(t);
        double z = 0.0;
        return Point3D(x, y, z);
    }

    Vector3D getDerivativeAtT(double t) const override {
        double dx_dt = -radiusX * sin(t);
        double dy_dt = radiusY * cos(t);
        double dz_dt = 0.0;
        return Vector3D(dx_dt, dy_dt, dz_dt);
    }

    std::string getType() const override {
        return "Ellipse";
    }
};

class Helix : public Curve {
private:
    double radius;
    double step;

public:
    Helix(double radius, double step) : radius(radius), step(step) {}
    std::double_t getRadius() const {
        return radius;
    }
    Point3D getPointAtT(double t) const override {
        double x = radius * cos(t);
        double y = radius * sin(t);
        double z = step * t / (2 * (atan(1) * 4));
        return Point3D(x, y, z);
    }

    Vector3D getDerivativeAtT(double t) const override {
        double dx_dt = -radius * sin(t);
        double dy_dt = radius * cos(t);
        double dz_dt = step / (2 * (atan(1) * 4));
        return Vector3D(dx_dt, dy_dt, dz_dt);
    }

    std::string getType() const override {
        return "Helix";
    }

    void runExample() {
        std::vector<Curve*> curves;

        curves.push_back(new Circle(5.0));
        curves.push_back(new Ellipse(3.0, 4.0));
        curves.push_back(new Helix(2.0, 1.0));

        for (Curve* curve : curves) {
            Point3D point = curve->getPointAtT((atan(1) * 4) / 4);
            Vector3D derivative = curve->getDerivativeAtT((atan(1) * 4) / 4);

            std::cout << "Curve: " << curve->getType() << std::endl;
            std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
            std::cout << "Derivative: (" << derivative.x << ", " << derivative.y << ", " << derivative.z << ")" << std::endl;
            std::cout << std::endl;
        }

        std::vector<Circle*> circles;

        for (Curve* curve : curves) {
            if (Circle* circle = dynamic_cast<Circle*>(curve)) {
                circles.push_back(circle);
            }
        }

        std::sort(circles.begin(), circles.end(), [](Circle* c1, Circle* c2) {
            return c1->getRadius() < c2->getRadius();
            });

        double totalSum = 0.0;

#pragma omp parallel for reduction(+:totalSum)
        for (int i = 0; i < circles.size(); i++) {
            totalSum += circles[i]->getRadius();
        }

        std::cout << "Total sum of radii: " << totalSum << std::endl;

        for (Curve* curve : curves) {
            delete curve;
        }
    }
};

int main() {
    Helix helix(2.0, 1.0);
    helix.runExample();

    return 0;
}
