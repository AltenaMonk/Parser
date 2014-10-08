#ifndef SERVERBAR_MODULE_SERVERBARMODULE_H__INCLUDE_GUARD
#define SERVERBAR_MODULE_SERVERBARMODULE_H__INCLUDE_GUARD

#include <Library/BaseModuleManager.h>
#include <Library/Module.h>

namespace Library
{
class XML;
class Time;
}

namespace Parser
{

class ParserModule : public Library::Module
{
public:
    ParserModule();
    virtual ~ParserModule();

    virtual void InitializeFactories();
    virtual bool ParseCommandLineParameters(TParameters const & parameters, bool isManagersReady);
    virtual bool InitializeManagers(Library::BaseModuleManager::TParameters const & parameters);

private:
    template <typename TType>
    void InitFactory();

    template <typename TType>
    bool InitManager(Library::BaseModuleManager::TParameters const & parameters);

    virtual void InitializeThreads(Library::BaseModuleManager::TParameters const & );

    bool InitLogManager();

    virtual void OnRootPermissions() const;
};

ParserModule * GetModule();

} // namespace Parser

#endif//SERVERBAR_MODULE_SERVERBARMODULE_H__INCLUDE_GUARD
