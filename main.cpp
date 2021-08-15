#include "application.h"

int main()
{
    auto map = new tilemap(480, 270);
    application app(map, "Map Editor + Generator", 7);
    app.run();
}
