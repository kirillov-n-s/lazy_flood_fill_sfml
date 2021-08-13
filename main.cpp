#include "application.h"

int main()
{
    auto ff = new floodfiller(640, 360);
    application app(ff, "", 5);
    app.run();
}
