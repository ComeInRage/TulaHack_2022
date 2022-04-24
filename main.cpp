#include "TgCore/Application.h"

static std::string tgToken = "5124523219:AAFeeq6K6rOYG1mIq5R5x7vcCUZVeoAzsJw";

int main() {
    tg::Application app{tgToken};
    return app.Run();
}