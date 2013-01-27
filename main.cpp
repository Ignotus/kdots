#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>
#include "mainwindow.h"

void initAboutData(KAboutData& aboutData) {
  aboutData.setHomepage("https://github.com/Ignotus/kdots");
  aboutData.setBugAddress("nlminhtl@gmail.com");
  aboutData.addAuthor(ki18n("Minh Ngo"),
                      ki18n("Architecture. Project Management"),
                      "nlminhtl@gmail.com",
                      "http://ignotusp.fedoraproject.org");
                      
  aboutData.addCredit(ki18n("Bohdan Rybak"),
                      ki18n("Game engine tester"),
                      "brainerazer@gmail.com");
                      
  aboutData.addCredit(ki18n("Artem Sereda"),
                      ki18n("Arch Linux package maintainer"),
                      "overmind88@gmail.com");
}

int main(int argc, char **argv) {
  KAboutData aboutData("kdots", 0,
                       ki18n("kdots"), "v2",
                       ki18n("A prototype of the game of dots."),
                       KAboutData::License_BSD,
                       ki18n("Copyright (c) 2011-2013 Minh Ngo"));
  
  KCmdLineArgs::init(argc, argv, &aboutData);
  
  KApplication app;

  MainWindow w;
  w.show();
  
  return app.exec();
}