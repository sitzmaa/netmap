#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include <string>

class Plugin {
public:
    virtual ~Plugin() {}

    // Initialize the plugin
    virtual void initialize() = 0;

    // Execute the plugin (e.g., update stats or provide functionality)
    virtual void execute() = 0;

    // Return a name or description for the plugin
    virtual std::string name() const = 0;

    // Cleanup any resources used by the plugin
    virtual void cleanup() = 0;
};

extern "C" {
    Plugin* create_plugin();
    void destroy_plugin(Plugin* plugin);
}

#endif // PLUGIN_HPP