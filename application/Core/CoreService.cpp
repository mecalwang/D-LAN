/**
  * D-LAN - A decentralized LAN file sharing software.
  * Copyright (C) 2010-2012 Greg Burri <greg.burri@gmail.com>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
  */
  
#include <CoreService.h>
using namespace CoreSpace;

#include <QObject>
#include <QThread>
#include <QRegExp>

#include <Common/Constants.h>

CoreService::CoreService(bool resetSettings, QLocale locale, int argc, char** argv) :
   QtService<CoreApplication>(argc, argv, Common::Constants::SERVICE_NAME), core(new Core(resetSettings, locale))
{
   this->setServiceDescription(tr("A LAN file sharing system"));
   this->setStartupType(QtServiceController::ManualStartup);
   this->setServiceFlags(QtServiceBase::Default);

   // If Core is launched from the console we read user input.
   for (int i = 1; i < argc; i++)
   {
      QString currentArg = QString::fromAscii(argv[i]);
      if (currentArg == "-e" || currentArg == "--exec")
      {
         connect(&this->consoleReader, SIGNAL(newLine(QString)), this, SLOT(processUserInput(QString)), Qt::QueuedConnection);
         this->consoleReader.start();
         QTextStream out(stdout);
         out << "D-LAN Core started with console support" << endl;
         CoreService::printCommands();
         break;
      }
   }
}

CoreService::~CoreService()
{
   delete this->core;
   this->core = 0;
}

void CoreService::changePassword(const QString& newPassword)
{
   this->core->changePassword(newPassword);
}

void CoreService::removePassword()
{
   this->core->removePassword();
}

void CoreService::start()
{
   this->core->start();
}

void CoreService::stop()
{
   delete this->core;
   this->core = 0;

   this->application()->quit();
   this->consoleReader.stop();
}

void CoreService::processUserInput(QString input)
{
   if (input == Common::ConsoleReader::QUIT_COMMAND)
   {
      this->stop();
   }
   else if (input == "dumpwi")
   {
      this->core->dumpWordIndex();
   }
   else if (input == "printsf")
   {
      this->core->printSimilarFiles();
   }
   else
      CoreService::printCommands();
}

void CoreService::printCommands()
{
   QTextStream out(stdout);
   out << "Commands:" << endl
       << " - " << Common::ConsoleReader::QUIT_COMMAND << " : stop the core" << endl
       << " - dumpwi : dump the word index in the log as a warning" << endl
       << " - printsf : print the similar files in the log as a warning" << endl;
}
