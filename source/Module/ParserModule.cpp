#include "ParserModule.h"

#include <csignal>
#include <cstring>

#include <Library/BaseSocket.h>
#include <Library/LogManager.h>
#ifdef UNITTEST
#include <Library/FileSystemManager.h>
#endif//UNITTEST
#include <Library/TimeManager.h>
#include <Library/ModuleCreator.h>
#include <Library/TypeToKey.h>

#include "Version.h"

namespace Parser
{

void sigHandler (int)
{
    static int i(0);
    LOG_PROG_INFO << "sigHandler = " << ++i;
    if (i == 1)
    {
        GetModule()->KillThreads(4000);  /// @internal Задержка в ms между установкой флага на выход и срубанием потоков
    }
    else
    {
        LOG_PROG_INFO << "sigHandler blocked!";
    }
}

ParserModule::ParserModule()
    : Library::Module(Version::applicationName, Version::applicationVersion, Version::applicationDate)
{
}

ParserModule::~ParserModule()
{
}

void ParserModule::InitializeFactories()
{
    InitFactory<Library::BaseSocket> ();
}

bool ParserModule::ParseCommandLineParameters(TParameters const & parameters, bool IsManagersReady)
{
    /// @internal Без инициализированных логов проверяем только версию:
    if (IsManagersReady == false)
    {
        /// @internal Если любой из параметров равен "-v" или "--version" - выводим версию и выходим
        for (size_t i(1); i < parameters.size(); ++i)
        {
            if ((Library::String(parameters[i]) == "-v") || (Library::String(parameters[i]) == "--version"))
            {
                Library::Module * module(Library::GetModule());
                std::cout << module->GetApplicationName() << " " << module->GetApplicationVersion() << " " << module->GetApplicationDate() << " " << std::endl;
                return false;
            }
        }
        return true;
    }

    LOG_PROG_INFO <<  "Command line parameters:";
    for (size_t i(0); i < parameters.size(); ++i)
    {
        LOG_PROG_INFO << i << " - " << parameters[i];
    }
    return true;
}

bool ParserModule::InitializeManagers(Library::BaseModuleManager::TParameters const & parameters)
{
    return Library::Module::InitializeManagers(parameters) &&
            InitLogManager();
}

template <typename TType>
void ParserModule::InitFactory()
{
    typedef Library::Factory<Library::String, TType, true, boost::shared_ptr<TType> > TFactory;
    AddFactory(Library::TypeToKey<TFactory>::Value(), boost::shared_ptr<Library::BaseFactory>(new TFactory()));
}

template <typename TType>
bool ParserModule::InitManager(Library::BaseModuleManager::TParameters const & parameters)
{
    boost::shared_ptr<Library::BaseModuleManager> manager(new TType());
    bool result(manager->Initialize(parameters));
    SetManager(Library::TypeToKey<TType>::Value(), manager);
    return result;
}

void ParserModule::InitializeThreads(Library::BaseModuleManager::TParameters const & )
{
}

bool ParserModule::InitLogManager()
{
    if (IsManager<Library::LogManager>() == false)
    {
        return false;
    }
    boost::shared_ptr<Library::LogManager> logManager(GetManager<Library::LogManager>());

    logManager->AddLog("./logs/programmer")
        .SetLogMessageImportanceLevel(Library::LogMessageImportance_Information)
        .SetLogMessageViewers(Library::LogMessageViewer_All)
        .SetStringEnd("\n")
        .SetDateTimeFormat("YYYY/MM/DD hh:mm:ss,lll ")
        .SetLogMessageImportanceEnabled(true)
        .SetDailySeparate(true)
        .SetPrettyFunction(true)
        .SetThreadId(true)
        .SetProcessId(true);
        ;

    LOG_ADMIN_INFO << "";
    LOG_ADMIN_INFO << "";
    LOG_ADMIN_INFO << "";
    LOG_ADMIN_INFO <<  "--------- Start application ---------";
    LOG_ADMIN_INFO << GetApplicationName() << " " << GetApplicationVersion() << " " << GetApplicationDate() << " ";
    LOG_ADMIN_INFO << "";
    return true;
}

void ParserModule::OnRootPermissions() const
{
    /// @internal Ругаемся на то, что запустили с правами root'а.
    std::cerr << "Application run with root privileges." << std::endl;
    /// @internal Выходим.
    exit(0);
}

ParserModule * GetModule()
{
    /// @internal Возвращаем проинициализированный объект.
    return Library::ModuleCreator<ParserModule>::GetModule(true);
}

} // namespace Parser

namespace Library
{

Module * GetModule()
{
    return Parser::GetModule();
}

} // namespace Library
