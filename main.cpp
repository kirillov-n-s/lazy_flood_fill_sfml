#include "application.h"

int main()
{
    auto ff = new floodfiller(320, 180);
    application app(ff);
    app.run();
}
