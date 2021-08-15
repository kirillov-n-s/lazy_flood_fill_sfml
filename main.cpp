#include "application.h"

int main()
{
    auto ff = new floodfiller(480, 270);
    application app(ff, "Map Editor + Generator (using Lazy Flood Fill)", 7);
    app.run();
}
