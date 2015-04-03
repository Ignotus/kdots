/*
 * KDots
 * Copyright (c) 2011, 2012, 2014, 2015 Minh Ngo <minh@fedoraproject.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES(INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <QApplication>
#include <QCommandLineParser>

#include <KAboutData>
#include <KLocale>
#include <KLocalizedString>

#include <config.hpp>
#include <gameconfig.hpp>

#include "mainwindow.hpp"

void initAboutData(KAboutData& aboutData)
{
  aboutData.setHomepage("https://github.com/Ignotus/kdots");
  aboutData.setBugAddress("nlminhtl@gmail.com");
  aboutData.addAuthor("Minh Ngo",
      i18n("Architecture. Project Management"),
      "nlminhtl@gmail.com",
      "http://ignotusp.fedoraproject.org");

  aboutData.addCredit("Bohdan Rybak",
      i18n("Game engine tester"),
      "brainerazer@gmail.com");

  aboutData.addCredit("Artem Sereda",
      i18n("Arch Linux package maintainer"),
      "overmind88@gmail.com");
}

int main(int argc, char **argv)
{
  KAboutData aboutData(QLatin1String("kdots"),
    QLatin1String("kdots"), VERSION,
    i18n("A prototype of the game of dots."),
    KAboutLicense::BSDL,
    QLatin1String("Copyright(c) 2011-2012 Minh Ngo"));

  initAboutData(aboutData);

  QCommandLineParser parser;
  KAboutData::setApplicationData(aboutData);
  parser.addVersionOption();
  parser.addHelpOption();

  QApplication app(argc, argv);
  parser.process(app);

  KDots::GameConfig::registerMeta();

  KDots::MainWindow *w = new KDots::MainWindow();
  w->show();

  return app.exec();
}
