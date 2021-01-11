#include "include/flutter_secure_storage/flutter_secure_storage_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>
#include <wincred.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>

namespace
{

  class FlutterSecureStoragePlugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    FlutterSecureStoragePlugin();

    virtual ~FlutterSecureStoragePlugin();

  private:
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    static const char* label;
  };

  // static
  void FlutterSecureStoragePlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "plugins.it_nomads.com/flutter_secure_storage",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<FlutterSecureStoragePlugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result) {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  
  }

  FlutterSecureStoragePlugin::FlutterSecureStoragePlugin() {
  }

  FlutterSecureStoragePlugin::~FlutterSecureStoragePlugin() {}

  void FlutterSecureStoragePlugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    const flutter::EncodableValue *args = method_call.arguments();
    const flutter::EncodableMap *argsMap = std::get_if<flutter::EncodableMap>(args);

    const std::string *key, *value;

    try {
      const auto keyEncoded = argsMap->at(flutter::EncodableValue("key"));
      key = std::get_if<std::string>(&keyEncoded);
    } catch(std::out_of_range) {
      key = NULL;
    }

    try {
      const auto valueEncoded = argsMap->at(flutter::EncodableValue("value"));
      value = std::get_if<std::string>(&valueEncoded);
    } catch(std::out_of_range) {
      value = NULL;
    }
      
    if (method_call.method_name().compare("read") == 0)
    {
      PCREDENTIALW cred = { 0 };
      if (!key) {
        result->Error("Bad arguments", "Key is required to read a value");
      }

      if(!CredRead(LPWSTR(L""), CRED_TYPE_GENERIC, 0, &cred)) {

      }

    }
    else if (method_call.method_name().compare("readAll") == 0)
    {
     
    }
    else if (method_call.method_name().compare("write") == 0)
    {
       if (!key || !value) {
        result->Error("Bad arguments", "Key and value are required to write a value");
      }
    }
    else if (method_call.method_name().compare("delete") == 0)
    {
       if (!key) {
        result->Error("Bad arguments", "Key is required to delete a value");
      } 
    }
    else if (method_call.method_name().compare("deleteAll") == 0)
    {
    }
    else
    {
      result->NotImplemented();
    }

    result->Success();
  }

} // namespace

void FlutterSecureStoragePluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  FlutterSecureStoragePlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
