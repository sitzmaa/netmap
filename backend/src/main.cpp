#include "network_server.hpp"
#include "plugin.hpp"
#include <iostream>
#include <cstdlib>
#include <dlfcn.h>  // For dynamic loading on Linux/macOS
#include <dirent.h>  // For reading directories
#include <vector>
#include <string>

void loadPlugins(const std::string& pluginFolder) {
    DIR* dir = opendir(pluginFolder.c_str());
    if (dir == nullptr) {
        std::cerr << "Error opening plugin directory: " << pluginFolder << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        // Only process files that have .so extension
        std::string filename(entry->d_name);
        if (filename.substr(filename.find_last_of(".") + 1) == "so") {
            std::string pluginPath = pluginFolder + "/" + filename;

            // Load the plugin dynamically
            void* handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
            if (!handle) {
                std::cerr << "Error loading plugin " << pluginPath << ": " << dlerror() << std::endl;
                continue;  // Skip this plugin and continue with the others
            }

            // Load the 'create_plugin' and 'destroy_plugin' functions
            typedef Plugin* (*CreatePlugin)();
            typedef void (*DestroyPlugin)(Plugin*);
            CreatePlugin create_plugin = (CreatePlugin) dlsym(handle, "create_plugin");
            DestroyPlugin destroy_plugin = (DestroyPlugin) dlsym(handle, "destroy_plugin");

            if (!create_plugin || !destroy_plugin) {
                std::cerr << "Error loading functions from plugin " << pluginPath << ": " << dlerror() << std::endl;
                dlclose(handle);
                continue;
            }

            // Create the plugin and initialize it
            Plugin* plugin = create_plugin();
            plugin->initialize();

            // Execute the plugin
            plugin->execute();

            std::cout << "Loaded plugin: " << plugin->name() << std::endl;

            // Cleanup and unload the plugin
            plugin->cleanup();
            destroy_plugin(plugin);
            dlclose(handle);
        }
    }

    closedir(dir);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    try {
        // Parse the port number from the command line argument
        short port = std::stoi(argv[1]);

        // Create the server object and start it
        NetworkServer server(port);
        server.start();

        // Load and execute plugins from the ../plugins directory
        loadPlugins("../plugins");

    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid port number. Please provide a valid integer." << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Port number out of valid range." << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}