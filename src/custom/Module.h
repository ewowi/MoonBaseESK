/**
    @title     MoonLight
    @file      Module.h
    @repo      https://github.com/MoonModules/MoonLight, submit changes to this file as PRs
    @Authors   https://github.com/MoonModules/MoonLight/commits/main
    @Doc       https://moonmodules.org/MoonLight/custom/animation/
    @Copyright © 2025 Github MoonLight Commit Authors
    @license   GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007
    @license   For non GPL-v3 usage, commercial licenses must be purchased. Contact moonmodules@icloud.com
**/

#ifndef Module_h
#define Module_h

#undef TAG
#define TAG "🌙"

#include <EventSocket.h>
#include <HttpEndpoint.h>
#include <EventEndpoint.h>
#include <WebSocketServer.h>
#include <PsychicHttp.h>
#include <FSPersistence.h>
#include <ESP32SvelteKit.h>

#if FT_ENABLED(FT_FILEMANAGER)
    #include "FilesService.h"
#endif

struct UpdatedItem {
    String parent;
    String name;
    String value;

    int getParentIndex(int depth = 0);
    String getParentName(int depth = 0);
};

class ModuleState
{
public:
    std::vector<UpdatedItem> updatedItems;

    JsonDocument data;

    std::function<void(JsonArray root)> setupDefinition = nullptr;

    void setupData();
    void compareRecursive(JsonString parent, JsonVariant oldData, JsonVariant newData);

    static void read(ModuleState &state, JsonObject &root);
    static StateUpdateResult update(JsonObject &root, ModuleState &state);
};

class Module : public StatefulService<ModuleState>
{
public:
    String _moduleName = "";

    Module(String moduleName, PsychicHttpServer *server,
                      ESP32SvelteKit *sveltekit
                      #if FT_ENABLED(FT_FILEMANAGER)
                          , FilesService *filesService
                      #endif
                    );

    void begin();

    virtual void setupDefinition(JsonArray root);

    virtual void onUpdate(UpdatedItem updatedItem);

protected:
    EventSocket *_socket;
    #if FT_ENABLED(FT_FILEMANAGER)
        FilesService *_filesService;
    #endif

private:
    HttpEndpoint<ModuleState> _httpEndpoint;
    EventEndpoint<ModuleState> _eventEndpoint;
    WebSocketServer<ModuleState> _webSocketServer;
    FSPersistence<ModuleState> _fsPersistence;
    PsychicHttpServer *_server;

    void onConfigUpdated();

};

#endif